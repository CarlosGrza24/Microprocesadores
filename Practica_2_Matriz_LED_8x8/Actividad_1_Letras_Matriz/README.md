# Actividad 1 — Matriz LED 8x8: letras

## Descripción

En esta actividad se utilizó una **matriz LED 8x8** controlada mediante el microcontrolador **PIC16F887** para mostrar diferentes letras. El objetivo principal fue representar caracteres usando arreglos de valores hexadecimales, donde cada arreglo contiene el patrón visual de una letra.

Las letras programadas fueron **I**, **F**, **L** y **O**. Cada letra se muestra durante un tiempo determinado y posteriormente cambia a la siguiente, generando una secuencia visual en la matriz LED.

Esta actividad permitió reforzar el uso de arreglos, multiplexado, valores binarios, valores hexadecimales y control de puertos digitales.

---

## Componentes utilizados

- PIC16F887
- Matriz LED 8x8
- Cristal oscilador
- Botón de reset
- Resistencia para MCLR
- Fuente Vcc
- Tierra GND
- MPLAB X IDE
- Compilador XC8
- Proteus Design Suite

---

### Simulación en Proteus

A continuación se muestran las letras visualizadas en la matriz LED 8x8 durante la simulación en Proteus.

<table>
  <tr>
    <td align="center"><strong>Letra I</strong></td>
    <td align="center"><strong>Letra F</strong></td>
    <td align="center"><strong>Letra L</strong></td>
    <td align="center"><strong>Letra O</strong></td>
  </tr>
  <tr>
    <td><img src="./simulacion_I.png" width="250"></td>
    <td><img src="./simulacion_F.png" width="250"></td>
    <td><img src="./simulacion_L.png" width="250"></td>
    <td><img src="./simulacion_O.png" width="250"></td>
  </tr>
</table>

![Simulación letras en matriz](./simulacion_letras.png)

### Video de funcionamiento

El siguiente GIF muestra una vista previa de la simulación en Proteus. Al dar clic sobre el GIF, se abre el video completo del funcionamiento.

[![Vista previa letras matriz](./letras_matriz_preview.gif)](./video_funcionamiento.mp4)

---

## Evidencias físicas

Además de la simulación en Proteus, la práctica fue implementada físicamente utilizando el microcontrolador **PIC16F887** y una matriz LED 8x8. En el circuito se conectaron las filas y columnas de la matriz a los puertos digitales del microcontrolador para visualizar las letras programadas directamente en hardware real.

### Armado general del circuito

![Armado físico letras matriz](./evidencias_fisicas/armado_fisico_letras.jpeg)

### Video del funcionamiento físico

El siguiente GIF muestra una vista previa del funcionamiento físico. Al dar clic sobre el GIF, se abre el video completo de la evidencia.

[![Vista previa física letras matriz](./evidencias_fisicas/letras_matriz_fisico.gif)](./evidencias_fisicas/video_fisico_letras_matriz.mp4)

### Carpeta completa de evidencias físicas

[Ver evidencias físicas](./evidencias_fisicas)

---

## Funcionamiento del circuito

El circuito utiliza el microcontrolador **PIC16F887** para controlar una matriz LED 8x8 mediante dos puertos digitales. Uno de los puertos selecciona la fila activa y el otro envía el patrón de columnas correspondiente a la letra que se desea mostrar.

Cada letra está formada por 8 valores hexadecimales, donde cada valor representa una fila de la matriz. Al recorrer rápidamente las filas y repetir el proceso varias veces, se genera una imagen estable de cada letra. Posteriormente, el programa cambia a la siguiente letra, formando una secuencia.

---

## Lógica de programación

Primero se definen los arreglos que contienen los patrones de cada letra. Por ejemplo, la letra `I` se representa de la siguiente manera:

```c
unsigned char letraI[8] = {
    0x7E,
    0x18,
    0x18,
    0x18,
    0x18,
    0x18,
    0x18,
    0x7E
};
```

Cada número hexadecimal corresponde a una fila de la matriz. El valor `0x7E`, por ejemplo, equivale a `01111110`, lo cual forma una línea horizontal dejando apagados los extremos. Los valores `0x18` equivalen a `00011000`, formando la parte vertical central de la letra.

Las letras programadas fueron:

| Letra | Descripción del patrón |
|---|---|
| I | Línea superior e inferior con columna central |
| F | Línea superior, línea media y columna izquierda |
| L | Columna izquierda y línea inferior |
| O | Contorno cerrado simulando una letra circular |

La función `mostrarLetra()` se encarga de recorrer las filas de la matriz:

```c
PORTB = 0x00;
PORTD = ~letra[i];
PORTB = (1 << i);
```

Primero se apaga temporalmente el puerto B para evitar interferencias visuales. Después se carga el patrón de columnas en el puerto D y finalmente se activa la fila correspondiente. Este proceso se repite muchas veces para que la letra permanezca visible antes de pasar a la siguiente.

---

## Código utilizado

```c
#include <xc.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 8000000

unsigned char letraI[8] = {
    0x7E,
    0x18,
    0x18,
    0x18,
    0x18,
    0x18,
    0x18,
    0x7E
};

unsigned char letraF[8] = {
    0x7E,
    0x40,
    0x40,
    0x7C,
    0x40,
    0x40,
    0x40,
    0x40
};

unsigned char letraL[8] = {
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x7E
};

unsigned char letraO[8] = {
    0x3C,
    0x42,
    0x42,
    0x42,
    0x42,
    0x42,
    0x42,
    0x3C
};

void mostrarLetra(unsigned char letra[8]){
    int t;
    char i;
    
    for(t = 0; t < 100; t++){
        for(i = 0; i < 8; i++){
            PORTB = 0x00;
            PORTD = ~letra[i];
            PORTB = (1 << i);
            __delay_ms(2);
        }
    }
}

void main(void){
    TRISB = 0x00;
    TRISD = 0x00;

    PORTB = 0x00;
    PORTD = 0x00;

    while(1){
        mostrarLetra(letraI);
        mostrarLetra(letraF);
        mostrarLetra(letraL);
        mostrarLetra(letraO);
    }
}
```

---

## Resultado esperado

Al ejecutar el programa, la matriz LED 8x8 debe mostrar de manera secuencial las letras **I**, **F**, **L** y **O**. Cada letra debe permanecer visible durante un momento antes de cambiar a la siguiente.

El resultado esperado es una secuencia clara de letras en la matriz, generada mediante multiplexado y control de patrones binarios desde el PIC16F887.

---

## Conclusión

Esta actividad permitió representar letras en una matriz LED 8x8 utilizando el microcontrolador PIC16F887. Se reforzó el uso de arreglos, valores hexadecimales, lógica invertida y multiplexado. Además, la práctica permitió comprender cómo una matriz de LEDs puede ser utilizada para mostrar información visual más compleja que el simple encendido de LEDs individuales.
