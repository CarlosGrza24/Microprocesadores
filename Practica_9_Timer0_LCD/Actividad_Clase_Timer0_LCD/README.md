# Actividad en clase — Temporizador con Timer0 y LCD

## Descripción

En esta actividad se implementó un temporizador utilizando el **Timer0** del microcontrolador **PIC16F887**. El tiempo se muestra en una pantalla **LCD 16x2** en formato `MM:SS`.

El objetivo principal fue utilizar interrupciones por desbordamiento del Timer0 para generar una base de tiempo y actualizar continuamente el conteo mostrado en la pantalla LCD.

---

## Componentes utilizados

- PIC16F887
- Pantalla LCD 16x2
- Potenciómetro para contraste del LCD
- Cristal oscilador
- Botón de reset
- Resistencia para MCLR
- Fuente Vcc
- Tierra GND
- MPLAB X IDE
- Compilador XC8
- Proteus Design Suite
- Librería LCD

---

## Librería utilizada

Para el manejo de la pantalla LCD se utilizó la librería general del repositorio:

- [`lcd.h`](../../Libreria_LCD/lcd.h)
- [`lcd.c`](../../Libreria_LCD/lcd.c)

---

## Evidencias

### Simulación en Proteus

[![Simulación Timer0 LCD](./evidencias_fisicas/Timer0_sim.gif)](./evidencias_fisicas/Timer0_sim.mp4)



---

## Evidencias físicas

Además de la simulación en Proteus, el proyecto puede implementarse físicamente utilizando el microcontrolador PIC16F887, una pantalla LCD 16x2.

### Armado general del circuito
![Armado físico LCD](./evidencias_fisicas/armado_general.jpeg)



### Video de funcionamiento físico
[![Simulación Timer0 LCD](./evidencias_fisicas/funcionamiento_fisico.gif)](./evidencias_fisicas/video_fisico.mp4)


---

## Funcionamiento del programa

El programa configura el **Timer0** con prescaler `1:256` mediante el registro `OPTION_REG = 0x07`. El registro `TMR0` se precarga con el valor `178` para ajustar el tiempo de desbordamiento.

Cada vez que Timer0 se desborda, se activa la bandera `T0IF` y se ejecuta la rutina de interrupción. Dentro de la interrupción se incrementa una variable auxiliar llamada `contador`. Cuando esta variable supera el valor establecido, se incrementa `tiempo`, que representa los segundos transcurridos.

El tiempo se muestra en la pantalla LCD usando el formato:

```text
MM:SS
```

---

## Lógica de programación

Primero se inicializa Timer0:

```c
OPTION_REG = 0x07;
TMR0 = 178;
T0IE = 1;
GIE = 1;
```

Después, en la interrupción, se revisa la bandera `T0IF`:

```c
if(T0IF){
    contador++;

    if(contador > 100){
        tiempo++;
        contador = 0;
    }

    TMR0 = 178;
    T0IF = 0;
}
```

En el ciclo principal se actualiza la LCD:

```c
sprintf(exec, "%02u:%02u", tiempo / 60, tiempo % 60);
LCD_putrs(exec);
```

---

## Código utilizado

```c
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <stdbool.h>
#include "lcd.h"

//=============================================================================
// CONFIGURACIÓN DE BITS DE CONFIGURACIÓN
//=============================================================================

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

//=============================================================================
// DEFINICIONES
//=============================================================================

#define _XTAL_FREQ 8000000

//=============================================================================
// VARIABLES GLOBALES
//=============================================================================

volatile unsigned int tiempo = 0;
volatile unsigned int contador = 0;
char exec[6];

//=============================================================================
// CONFIGURACIÓN DE TIMER0
//=============================================================================

void Timer0_Init(){
    OPTION_REG = 0x07;  // Timer0 con prescaler 1:256
    TMR0 = 178;         // Precarga inicial del Timer0
    T0IE = 1;           // Habilita interrupción de Timer0
    GIE = 1;            // Habilita interrupciones globales
}

//=============================================================================
// RUTINA DE INTERRUPCIÓN
//=============================================================================

void __interrupt() ISR(void){
    if(T0IF){
        contador++;

        if(contador > 100){
            tiempo++;
            contador = 0;
        }

        TMR0 = 178;
        T0IF = 0;
    }
}

//=============================================================================
// PROGRAMA PRINCIPAL
//=============================================================================

void main(void){
    Timer0_Init();

    LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7};
    LCD_Init(lcd);

    LCD_Clear();
    LCD_Set_Cursor(0,0);
    LCD_putrs("Tiempo: ");
    LCD_Set_Cursor(1,0);
    LCD_putrs("00:00");

    while(1){
        LCD_Set_Cursor(1,0);
        sprintf(exec, "%02u:%02u", tiempo / 60, tiempo % 60);
        LCD_putrs(exec);
    }
}
```

---

## Resultado esperado

La pantalla LCD debe mostrar el texto `Tiempo:` en la primera línea y el conteo en formato `MM:SS` en la segunda línea.

---

## Conclusión

Esta actividad permitió comprender el uso de Timer0 como base de tiempo mediante interrupciones. También se reforzó el uso de variables `volatile`, rutinas de interrupción y despliegue de información en una pantalla LCD.
