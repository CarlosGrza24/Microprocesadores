# Actividad clase B — Contador 0-9 con interrupción y LED

## Descripción

En esta actividad se realizó un contador del **0 al 9** utilizando un display de 7 segmentos y el microcontrolador **PIC16F887**. Además, se incorporó una **interrupción externa** mediante un botón conectado al pin `RB0/INT`.

El objetivo principal fue comprender el funcionamiento de las interrupciones externas. Mientras el contador avanza normalmente del 0 al 9, al presionar el botón se ejecuta una rutina de interrupción. Durante esta rutina, el conteo se detiene temporalmente y un LED conectado a `RC0` parpadea de forma intermitente.

Una vez terminada la rutina de interrupción, el programa regresa al ciclo principal y continúa el conteo.

---

## Componentes utilizados

- PIC16F887
- Display de 7 segmentos
- LED
- Botón para interrupción externa
- Resistencias para display, LED y botón
- Cristal oscilador
- Botón de reset
- Resistencia para MCLR
- Fuente Vcc
- Tierra GND
- MPLAB X IDE
- Compilador XC8
- Proteus Design Suite

---

## Evidencias

### Simulación en Proteus

[![Video de funcionamiento](./video_simu_inter.gif)](./video_simu_inter.mp4)

---

## Evidencias físicas

Además de la simulación en Proteus, la práctica puede implementarse físicamente utilizando el microcontrolador **PIC16F887**, un display de 7 segmentos, un botón y un LED indicador.

### Armado general del circuito

![Armado físico](./evidencias_fisicas/armado_general_099.jpeg)

### Funcionamiento físico

[![Vista previa física](./evidencias_fisicas/funcionamiento_fisico.gif)](./evidencias_fisicas/video_fisico.mp4)

### Carpeta completa de evidencias físicas

[Ver evidencias físicas](./evidencias_fisicas)

---

## Funcionamiento del circuito

El contador se muestra en el display de 7 segmentos mediante patrones enviados al puerto `PORTD`. El LED indicador se conecta al pin `RC0`.

El botón de interrupción se conecta al pin `RB0/INT`. Cuando el usuario presiona el botón, se activa la bandera de interrupción externa `INTF`, y el microcontrolador detiene momentáneamente la ejecución normal del programa para atender la rutina de interrupción.

Dentro de la interrupción se llama a la función `blink_led()`, la cual hace parpadear el LED varias veces. Después, se limpia la bandera de interrupción y el programa continúa con el contador.

---

## Lógica de programación

Primero se define el pin del LED:

```c
#define LED PORTCbits.RC0
```

Después se declara la tabla de patrones para mostrar los números del 0 al 9:

```c
unsigned char patron [10]= {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x67  // 9
};
```

La función `blink_led()` se encarga de parpadear el LED:

```c
void blink_led(){
    for(int i = 0; i < 4; i++){
        LED = 1; 
        __delay_ms(500);
        LED = 0;
        __delay_ms(500); 
    }
}
```

Posteriormente se habilitan las interrupciones:

```c
GIE = 1;
INTE = 1;
INTEDG = 0;
```

`GIE` activa las interrupciones globales, `INTE` activa la interrupción externa del pin `RB0/INT`, e `INTEDG = 0` configura la interrupción por flanco de bajada.

La rutina de interrupción verifica la bandera `INTF`, ejecuta el parpadeo del LED y limpia la bandera:

```c
void __interrupt () ISR(void){
    if (INTF){
        GIE = 0;
        blink_led();
        GIE = 1;
        INTF = 0;
    }
}
```

---

## Código utilizado

```c
#include <xc.h>         // Biblioteca principal del compilador XC8

//=============================================================================
// CONFIGURACIÓN DE BITS DE CONFIGURACIÓN
//=============================================================================

#pragma config FOSC = HS        // Selección del oscilador HS
#pragma config WDTE = OFF       // Watchdog Timer desactivado
#pragma config PWRTE = OFF      // Power-up Timer desactivado
#pragma config BOREN = ON       // Brown-out Reset activado
#pragma config LVP = OFF        // Programación en bajo voltaje desactivada
#pragma config CPD = OFF        // Protección EEPROM desactivada
#pragma config WRT = OFF        // Escritura en memoria Flash desactivada
#pragma config CP = OFF         // Protección de código desactivada

//=============================================================================
// DEFINICIONES
//=============================================================================

#define _XTAL_FREQ 8000000      // Frecuencia del oscilador utilizada para retardos

#define LED PORTCbits.RC0       // LED indicador conectado en RC0

unsigned char patron [10]= {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x67  // 9
};

// Función que hace parpadear el LED
void blink_led(){
    for(int i = 0; i < 4; i++){
        LED = 1; 
        __delay_ms(500);
        LED = 0;
        __delay_ms(500); 
    }
}

void main (void){

    ANSEL = 0x00; 
    ANSELH = 0x00; 

    OPTION_REG = OPTION_REG & 0b01111111; // Habilita pull-ups internos en PORTB
    
    TRISC = 0x00; // PORTC como salida para LED
    TRISD = 0x00; // PORTD como salida para display
    TRISB = 0xFF; // PORTB como entrada para botón de interrupción
    
    PORTC = 0x00; 
    PORTD = 0x00;
    
    unsigned char count = 0; 
    
    GIE = 1;     // Activa interrupciones globales
    INTE = 1;    // Activa interrupción externa RB0/INT
    INTEDG = 0;  // Interrupción por flanco de bajada
    
    while(1){
        PORTD = patron[count];          // Muestra el número actual
        count = (count + 1) % 10;       // Avanza de 0 a 9
        __delay_ms(500);
    }
}

void __interrupt () ISR(void){
    if (INTF){
        GIE = 0;        // Desactiva interrupciones globales temporalmente
        blink_led();    // Ejecuta acción de interrupción
        GIE = 1;        // Reactiva interrupciones globales
        INTF = 0;       // Limpia bandera de interrupción externa
    }
}
```

---

## Resultado esperado

El display de 7 segmentos debe mostrar un conteo del `0` al `9`. Al presionar el botón conectado a `RB0/INT`, el conteo se interrumpe temporalmente y el LED conectado a `RC0` parpadea. Cuando termina el parpadeo, el contador continúa su ejecución normal.

---

## Conclusión

Esta actividad permitió comprender el uso de interrupciones externas en el PIC16F887. Se reforzó el manejo del pin `RB0/INT`, la configuración de banderas de interrupción, el control de un LED indicador y la continuidad del programa principal después de atender una interrupción.
