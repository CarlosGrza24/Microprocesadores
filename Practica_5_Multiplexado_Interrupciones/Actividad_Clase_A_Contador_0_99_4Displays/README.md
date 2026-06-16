# Actividad clase A — Contador 0-99 con 4 displays de 7 segmentos

## Descripción

En esta actividad se realizó un contador decimal del **0 al 99** utilizando el microcontrolador **PIC16F887** y una pantalla formada por **4 displays de 7 segmentos**. Aunque la pantalla cuenta con cuatro displays disponibles, en esta práctica se utilizaron principalmente dos posiciones para mostrar las **decenas** y las **unidades** del contador.

El objetivo principal fue comprender el uso del **multiplexado** en displays de 7 segmentos. A diferencia de conectar un display de forma directa, el multiplexado permite controlar varios displays utilizando el mismo puerto para los segmentos y otro puerto para seleccionar qué display se encuentra activo en cada instante.

Esta práctica permitió reforzar el uso de arreglos, operaciones aritméticas, control de puertos digitales y visualización de números mediante displays de 7 segmentos.

---

## Componentes utilizados

- PIC16F887
- Pantalla de 4 displays de 7 segmentos
- Resistencias para los segmentos
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
[![Video de funcionamiento](./video_simu_099.gif)](./evidencias_fisicas/video_simu_099.mp4)

---

## Evidencias físicas

Además de la simulación en Proteus, la práctica puede implementarse físicamente utilizando el microcontrolador **PIC16F887** y una pantalla de 4 displays de 7 segmentos.

### Armado general del circuito

![Armado físico](./evidencias_fisicas/armado_general099.jpeg)

### Funcionamiento físico

[![Vista previa física](./evidencias_fisicas/funcionamiento_fisico.gif)](./evidencias_fisicas/video_fisico.mp4)

### Carpeta completa de evidencias físicas

[Ver evidencias físicas](./evidencias_fisicas)

---

## Funcionamiento del circuito

El circuito utiliza el puerto `PORTD` para enviar el patrón de segmentos que forma cada número en el display. Por otro lado, el puerto `PORTC` se utiliza para seleccionar cuál de los displays se encuentra activo.

El programa separa el número actual en **decenas** y **unidades**. Después, mediante multiplexado, activa primero el display de las decenas, envía su patrón correspondiente, espera un tiempo muy corto y luego activa el display de unidades.

Este proceso se repite muchas veces antes de incrementar el contador. Debido a que el cambio entre displays ocurre rápidamente, el ojo humano percibe ambos dígitos encendidos al mismo tiempo.

---

## Lógica de programación

Primero se define el arreglo `patron`, el cual contiene los valores necesarios para mostrar los números del 0 al 9 en un display de 7 segmentos:

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

Después se configuran los puertos `PORTD` y `PORTC` como salidas:

```c
TRISD = 0;
TRISC = 0;
```

El número se separa en decenas y unidades mediante operaciones aritméticas:

```c
int dec = num / 10;
int uni = num % 10;
```

Para mostrar las decenas, se activa el display correspondiente desde `PORTC` y se manda el patrón al `PORTD`:

```c
PORTC = 0b11111101;
PORTD = patron[dec];
__delay_ms(1);
```

Para mostrar las unidades, se activa otro display y se envía el patrón correspondiente:

```c
PORTC = 0b11111110;
PORTD = patron[uni];
__delay_ms(1);
```

Este proceso se repite dentro de un ciclo para mantener ambos dígitos visibles antes de incrementar el contador.

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

// Tabla de patrones para display de 7 segmentos
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

void main(void){

    TRISD = 0x00;   // PORTD como salida para segmentos
    TRISC = 0x00;   // PORTC como salida para seleccionar displays
    
    int num = 0;    // Variable del contador

    while (1){

        int dec = num / 10; // Extrae las decenas
        int uni = num % 10; // Extrae las unidades
        
        // Multiplexado de los displays
        for (int i = 0; i < 500; i++){

            // Mostrar decenas
            PORTC = 0b11111101;
            PORTD = patron[dec];
            __delay_ms(1);
            
            // Mostrar unidades
            PORTC = 0b11111110;
            PORTD = patron[uni];
            __delay_ms(1);
        }

        num++;              // Incrementa el contador

        if (num == 100){    // Reinicia al llegar a 100
            num = 0;
        }
    }
}
```

---

## Resultado esperado

Al ejecutar la simulación, la pantalla de displays debe mostrar un conteo desde `00` hasta `99`. Cuando el contador llega a `99`, el siguiente valor vuelve a ser `00`.

El cambio de displays ocurre mediante multiplexado, por lo que visualmente se observan las decenas y unidades encendidas al mismo tiempo.

---

## Conclusión

Esta actividad permitió comprender el uso del multiplexado en displays de 7 segmentos. Se reforzó la separación de números en decenas y unidades, el uso de arreglos de patrones y el control de múltiples displays mediante puertos digitales del PIC16F887.
