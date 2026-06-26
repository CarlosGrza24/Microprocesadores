# Actividad en clase — Temporizador con Timer1 y LCD

## Descripción

En esta actividad se implementó un temporizador utilizando el **Timer1** del microcontrolador **PIC16F887**. El tiempo se muestra en una pantalla **LCD 16x2** en formato `MM:SS`.

El objetivo fue utilizar Timer1 como fuente de interrupción para generar una base de tiempo y visualizarla en una pantalla LCD.

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

![Simulación Timer1 LCD](./simulacion.png)

### Video de funcionamiento

[▶ Ver video de funcionamiento](./video_funcionamiento.mp4)

---

## Funcionamiento del programa

Timer1 se configura con prescaler `1:8` y una precarga en los registros `TMR1H` y `TMR1L`. Cada vez que Timer1 se desborda, se ejecuta una interrupción.

Dentro de la interrupción se incrementa un contador auxiliar. Cuando este contador alcanza el valor establecido, se incrementa la variable `tiempo`.

---

## Lógica de programación

Timer1 se configura con:

```c
T1CON = 0x31;
TMR1H = 0xF6;
TMR1L = 0x3C;
TMR1IE = 1;
PEIE = 1;
GIE = 1;
```

La rutina de interrupción recarga Timer1 y actualiza el tiempo:

```c
if(TMR1IF){
    contador++;

    if(contador >= 100){
        tiempo++;
        contador = 0;
    }

    TMR1H = 0xF6;
    TMR1L = 0x3C;
    TMR1IF = 0;
}
```

---

## Código utilizado

```c
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lcd.h"

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 8000000

volatile unsigned int tiempo = 0;
volatile unsigned int contador = 0;

char buffer[6];

void Timer1_Init(){
    T1CON = 0x31;      // Timer1 ON, prescaler 1:8

    TMR1H = 0xF6;
    TMR1L = 0x3C;

    TMR1IF = 0;
    TMR1IE = 1;
    PEIE = 1;
    GIE = 1;
}

void __interrupt() ISR(void){
    if(TMR1IF){
        contador++;

        if(contador >= 100){
            tiempo++;
            contador = 0;
        }

        TMR1H = 0xF6;
        TMR1L = 0x3C;

        TMR1IF = 0;
    }
}

void main(void){
    unsigned int segundos;

    Timer1_Init();

    LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7};
    LCD_Init(lcd);

    LCD_Clear();
    LCD_Set_Cursor(0, 0);
    LCD_putrs("Timer1:");

    while(1){
        GIE = 0;
        segundos = tiempo;
        GIE = 1;

        LCD_Set_Cursor(1, 0);
        sprintf(buffer, "%02u:%02u", segundos / 60, segundos % 60);
        LCD_putrs(buffer);

        __delay_ms(200);
    }
}
```

---

## Resultado esperado

La pantalla LCD debe mostrar el texto `Timer1:` y debajo el tiempo en formato `MM:SS`.

---

## Conclusión

Esta actividad permitió comprender el uso de Timer1 mediante interrupciones y su aplicación para generar una base de tiempo visualizada en LCD.
