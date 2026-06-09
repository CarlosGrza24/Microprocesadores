# Actividad en clase — Matriz LED 8x8: figura X

## Descripción

En esta actividad se utilizó una **matriz LED 8x8** controlada mediante el microcontrolador **PIC16F887**. El objetivo principal fue mostrar una figura en forma de **X** mediante el control de filas y columnas de la matriz.

A diferencia de las prácticas anteriores, donde se trabajó con LEDs individuales, en esta actividad fue necesario controlar una matriz completa. Para lograrlo, se aplicó el concepto de **multiplexado**, el cual permite encender una fila a la vez mientras se envía el patrón correspondiente a las columnas.

Esta práctica permitió reforzar el uso de puertos digitales, arreglos de datos, valores hexadecimales y lógica invertida para representar figuras en una matriz de LEDs.

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

## Evidencias

### Simulación en Proteus

![Simulación matriz X](./simulacion_x.png)

### Video de funcionamiento

El siguiente GIF muestra una vista previa de la simulación en Proteus. Al dar clic sobre el GIF, se abre el video completo del funcionamiento.

[![Vista previa matriz X](./matriz_x_preview.gif)](./video_funcionamiento.mp4)

---

## Evidencias físicas

Además de la simulación en Proteus, la práctica fue implementada físicamente utilizando el microcontrolador **PIC16F887** y una matriz LED 8x8. En el circuito se conectaron las filas y columnas de la matriz a los puertos digitales del microcontrolador para mostrar la figura de una X en hardware real.

### Armado general del circuito

![Armado físico matriz X](./evidencias_fisicas/armado_fisico_x.jpeg)

### Video del funcionamiento físico

El siguiente GIF muestra una vista previa del funcionamiento físico. Al dar clic sobre el GIF, se abre el video completo de la evidencia.

[![Vista previa física matriz X](./evidencias_fisicas/matriz_x_fisico.gif)](./evidencias_fisicas/video_fisico_matriz_x.mp4)

### Carpeta completa de evidencias físicas

[Ver evidencias físicas](./evidencias_fisicas)

---

## Funcionamiento del circuito

En la simulación se utilizó el microcontrolador **PIC16F887** conectado a una matriz LED 8x8. Para controlar la matriz, se utilizaron dos puertos digitales: uno para seleccionar las filas y otro para enviar el patrón correspondiente a las columnas.

La matriz no enciende todos los LEDs de la figura al mismo tiempo. En realidad, el programa activa una fila, coloca el patrón de columnas correspondiente, espera unos milisegundos y después pasa a la siguiente fila. Este proceso se repite rápidamente, por lo que visualmente se percibe una X completa y estable.

---

## Lógica de programación

Primero se define un arreglo con los patrones necesarios para formar la X:

```c
unsigned char patrones [8]= {
    0x81, 
    0x42, 
    0x24, 
    0x18, 
    0x18, 
    0x24, 
    0x42, 
    0x81
};
```

Cada valor hexadecimal representa una fila de la matriz. Por ejemplo, `0x81` equivale a `10000001`, por lo que se activan los extremos de la fila. Después, los valores van acercando los puntos hacia el centro hasta formar la figura de una X.

| Fila | Hexadecimal | Binario | Descripción |
|---|---|---|---|
| 1 | `0x81` | `10000001` | Extremos encendidos |
| 2 | `0x42` | `01000010` | Diagonal hacia el centro |
| 3 | `0x24` | `00100100` | Diagonal hacia el centro |
| 4 | `0x18` | `00011000` | Centro de la X |
| 5 | `0x18` | `00011000` | Centro de la X |
| 6 | `0x24` | `00100100` | Diagonal hacia afuera |
| 7 | `0x42` | `01000010` | Diagonal hacia afuera |
| 8 | `0x81` | `10000001` | Extremos encendidos |

Después se configuran los puertos B y D como salidas:

```c
TRISB = 0;
TRISD = 0;
```

Dentro del ciclo infinito, se recorre cada fila de la matriz:

```c
PORTB = 1 << i;
PORTD = ~patrones[i];
```

`PORTB = 1 << i` selecciona la fila activa, mientras que `PORTD = ~patrones[i]` envía el patrón de columnas. Se utiliza el operador `~` porque la matriz trabaja con lógica invertida en las columnas.

Finalmente, se utiliza un retardo pequeño para permitir que cada fila permanezca encendida por un instante antes de pasar a la siguiente:

```c
__delay_ms(5);
```

---

## Código utilizado

```c
#include <xc.h>

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 8000000

unsigned char patrones [8]= {
    0x81, 
    0x42, 
    0x24, 
    0x18, 
    0x18, 
    0x24, 
    0x42, 
    0x81
};

void main (void){
    TRISB = 0;
    TRISD = 0;

    PORTD = 0;
    PORTB = 0;
    
    while (1){
        for (char i = 0; i < 8; i++){
            PORTB = 1 << i;
            PORTD = ~patrones[i];
            __delay_ms(5);      
        }
    }
}
```

---

## Resultado esperado

Al ejecutar el programa, la matriz LED 8x8 debe mostrar una figura en forma de **X**. Aunque el microcontrolador activa una fila a la vez, el cambio entre filas ocurre rápidamente, por lo que la figura se observa completa.

---

## Conclusión

Esta actividad permitió comprender el funcionamiento básico de una matriz LED 8x8 controlada con el microcontrolador PIC16F887. Se reforzó el uso de arreglos para definir patrones visuales, el manejo de valores hexadecimales y el concepto de multiplexado para controlar varias filas y columnas con salidas digitales.
