# Actividad en clase — Parpadeo de 4 LEDs

## Descripción

En esta actividad se realizó el manejo básico de salidas digitales utilizando el microcontrolador **PIC16F887**. El objetivo principal fue encender y apagar 4 LEDs conectados al puerto D del microcontrolador, generando un parpadeo con un retardo de 500 ms.

Esta práctica permitió comprender cómo configurar un puerto como salida digital mediante el registro `TRISD` y cómo modificar el estado lógico de los pines usando el registro `PORTD`. Además, se trabajó la relación entre valores binarios, valores hexadecimales y el comportamiento físico de los LEDs en la simulación.

---

## Componentes utilizados

- PIC16F887
- 4 LEDs
- 4 resistencias
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

### Simulación en Proteus — LEDs encendidos

![Simulación LEDs encendidos](./evidencias/simulacion_on.png)

### Simulación en Proteus — LEDs apagados

![Simulación LEDs apagados](./evidencias/simulacion_off.png)

---

## Funcionamiento del circuito

En la simulación se utilizó el microcontrolador **PIC16F887** conectado a 4 LEDs mediante resistencias. Los LEDs se conectaron a salidas digitales del puerto D. Cada salida del microcontrolador controla el estado de un LED, permitiendo encenderlo cuando el pin entrega un nivel lógico alto y apagarlo cuando el pin se encuentra en nivel lógico bajo.

También se incluyó el cristal oscilador para definir la frecuencia de operación del microcontrolador y el circuito de reset conectado al pin MCLR, necesario para asegurar el correcto inicio del PIC.

---

## Lógica de programación

Primero se configura el puerto D como salida:

```c
TRISD = 0b00000000;
```

Después, se inicializa el puerto apagado:

```c
PORTD = 0b00000000;
```

Dentro del ciclo  `while(1)`, se manda el valor `0xF` al puerto D:

```c
PORTD = 0xF;
```

El valor `0xF` en hexadecimal equivale a `00001111` en binario, por lo que se activan los primeros 4 bits del puerto D. Esto hace que los 4 LEDs conectados se enciendan.

Después de 500 ms, se manda el valor `0x0`:

```c
PORTD = 0x0;
```

Esto apaga todos los LEDs. El proceso se repite continuamente, generando el efecto de parpadeo.

---

## Código utilizado

```c
#include <xc.h>         // Biblioteca principal del compilador XC8

//=============================================================================
// CONFIGURACIÓN DE BITS DE CONFIGURACIÓN
//=============================================================================

#pragma config FOSC = XT        // Selección de oscilador XT
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

#define _XTAL_FREQ 8000000      // Frecuencia del oscilador

void main(void){
    TRISD = 0b00000000;         // Configura PORTD como salida
    TRISD = 0;

    PORTD = 0b00000000;         // Inicializa PORTD apagado

    while (1){
        PORTD = 0xF;            // Enciende los primeros 4 LEDs
        __delay_ms(500);        // Espera 500 ms

        PORTD = 0x0;            // Apaga todos los LEDs
        __delay_ms(500);        // Espera 500 ms
    }
}
```

---

## Resultados experimentales

Al ejecutar la simulación, los 4 LEDs deben encenderse durante 500 ms y apagarse durante otros 500 ms. Esta secuencia se repite de manera continua mientras el microcontrolador permanezca energizado.

El resultado demuestra que el puerto D fue configurado correctamente como salida y que el PIC16F887 puede controlar dispositivos externos mediante señales digitales.

---

## Conclusión

Esta actividad permitió comprender el uso básico de salidas digitales en el microcontrolador PIC16F887. Mediante la configuración del registro `TRISD` y la escritura de valores en `PORTD`, se logró controlar el encendido y apagado de varios LEDs. La práctica también ayudó a relacionar la programación en lenguaje C con el comportamiento físico observado en la simulación de Proteus.
