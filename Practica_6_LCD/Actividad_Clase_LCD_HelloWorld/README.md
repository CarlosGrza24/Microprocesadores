# Actividad en clase — LCD: Hello World y abecedario

## Descripción

En esta actividad se utilizó una **pantalla LCD 16x2** controlada mediante el microcontrolador **PIC16F887**. El objetivo principal fue desplegar mensajes de texto en la pantalla LCD, utilizando dos líneas de visualización.

En la primera línea de la pantalla se mostraba el mensaje:

```text
Hello World!
```

En la segunda línea se mostraban letras del abecedario de forma secuencial:

```text
ABCDEFGHIJLMNOP
```

Esta práctica permitió comprender el funcionamiento básico de una pantalla LCD, la configuración de sus pines de control y datos, y el uso de una librería externa para simplificar el envío de comandos y caracteres.

---

## Componentes utilizados

- PIC16F887
- Pantalla LCD 16x2
- Potenciómetro para ajuste de contraste
- Cristal oscilador
- Botón de reset
- Resistencia para MCLR
- Fuente Vcc
- Tierra GND
- MPLAB X IDE
- Compilador XC8
- Proteus Design Suite
- Librería `lcd.h`

---

## Evidencias

### Simulación en Proteus

[![Simulacion LCD Hello World](./video_simu_lcdclase.gif)](./video_simu_lcdclase.mp4)

---

## Evidencias físicas

Además de la simulación en Proteus, la práctica puede implementarse físicamente utilizando el microcontrolador **PIC16F887** y una pantalla LCD 16x2.

### Armado general del circuito

![Armado físico LCD](./evidencias_fisicas/armado_general_lcdclase.jpeg)

### Funcionamiento físico

[![Vista previa física LCD](./evidencias_fisicas/video_fisico_lcd_clase.gif)](./evidencias_fisicas/video_fisico_lcd_clase.mp4)

### Carpeta completa de evidencias físicas

[Ver evidencias físicas](./evidencias_fisicas)

---

## Funcionamiento del circuito

El circuito utiliza una pantalla **LCD 16x2**, la cual permite mostrar dos líneas de texto. Para controlar la pantalla, el microcontrolador envía comandos y caracteres mediante pines digitales.

La pantalla LCD utiliza pines de control como `RS` y `E`, además de pines de datos. En esta práctica se empleó una librería LCD, la cual facilita el uso de funciones como inicializar la pantalla, posicionar el cursor, limpiar la pantalla y escribir cadenas de texto.

El potenciómetro conectado al pin de contraste permite ajustar la visibilidad de los caracteres en la pantalla.

---

## Lógica de programación

La idea principal de la práctica fue inicializar la pantalla LCD y después escribir texto en sus dos líneas.

Primero se inicializa la pantalla mediante la librería LCD. Después se posiciona el cursor en la primera línea para escribir:

```text
Hello World!
```

Posteriormente, se posiciona el cursor en la segunda línea para mostrar las letras del abecedario:

```text
ABCDEFGHIJLMNOP
```

La función de la librería permite controlar la ubicación del texto dentro de la pantalla, por lo que se puede elegir en qué fila y columna comenzar a escribir.

---

## Código utilizado

```c
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <stdbool.h>
#include "lcd.h"// Biblioteca principal del compilador XC8

//=============================================================================
// CONFIGURACI N DE BITS DE CONFIGURACI N (FUSES)
//=============================================================================

// Selecci n de oscilador (usar XT si est s usando un cristal de 4 MHz)
#pragma config FOSC = HS        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (disabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection (disabled)
#pragma config WRT = OFF        // Flash Program Memory Write Enable (disabled)
#pragma config CP = OFF         // Flash Program Memory Code Protection (disabled)

//=============================================================================
// DEFINICIONES
//=============================================================================

#define _XTAL_FREQ 8000000      // Frecuencia del oscilador (para __delay_ms y __delay_us)

void main(void){
    LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7}; //PORT, RS, EN, D4, D5, D6, D7
    
    LCD_Init(lcd);
    
    while(1){
        LCD_Clear();
        LCD_Set_Cursor(0,0);
        LCD_putrs(" Hello world!");
        
        LCD_Set_Cursor(1,0);
        for(char c = 'A'; c < 'Q'; c++){
            LCD_putc(c);
            __delay_ms(300);
        }
        __delay_ms(1000);
    }
}
```

---

## Resultado esperado

Al ejecutar la simulación, la pantalla LCD debe mostrar el mensaje **Hello World!** en la primera línea. En la segunda línea deben observarse las letras del abecedario indicadas en la práctica.

---

## Conclusión

Esta actividad permitió comprender el uso básico de una pantalla LCD 16x2 con el microcontrolador PIC16F887. Se reforzó el manejo de librerías externas, el posicionamiento del cursor y el despliegue de texto en diferentes líneas de la pantalla.
