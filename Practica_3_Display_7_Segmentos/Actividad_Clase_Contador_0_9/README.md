# Actividad en clase — Display de 7 segmentos: contador 0-9

## Descripción

En esta actividad se realizó un contador decimal del **0 al 9** utilizando un **display de 7 segmentos** y el microcontrolador **PIC16F887**. El objetivo principal fue representar números en el display mediante patrones binarios enviados desde el puerto D del microcontrolador.

Cada número se forma encendiendo una combinación específica de segmentos. Para facilitar la programación, se utilizó un arreglo llamado `numeros`, donde cada posición contiene el valor binario necesario para mostrar un dígito del 0 al 9.

Esta práctica permitió reforzar el uso de salidas digitales, arreglos en lenguaje C, valores binarios y la relación entre el código programado y la visualización obtenida en el display de 7 segmentos.

---

## Componentes utilizados

* PIC16F887
* Display de 7 segmentos
* 7 resistencias de 240 Ω
* Cristal oscilador 8 mHz
* Botón de reset
* Resistencia para MCLR
* Fuente Vcc
* Tierra GND
* MPLAB X IDE
* Compilador XC8
* Proteus Design Suite

---

## Evidencias

### Simulación en Proteus

A continuación se muestran los números del contador decimal visualizados en el display de 7 segmentos durante la simulación en Proteus.

<table>
  <tr>
    <td align="center"><strong>0</strong></td>
    <td align="center"><strong>1</strong></td>
    <td align="center"><strong>2</strong></td>
    <td align="center"><strong>3</strong></td>
    <td align="center"><strong>4</strong></td>
  </tr>
  <tr>
    <td><img src="./simulacion_0.png" width="180"></td>
    <td><img src="./simulacion_1.png" width="180"></td>
    <td><img src="./simulacion_2.png" width="180"></td>
    <td><img src="./simulacion_3.png" width="180"></td>
    <td><img src="./simulacion_4.png" width="180"></td>
  </tr>
  <tr>
    <td align="center"><strong>5</strong></td>
    <td align="center"><strong>6</strong></td>
    <td align="center"><strong>7</strong></td>
    <td align="center"><strong>8</strong></td>
    <td align="center"><strong>9</strong></td>
  </tr>
  <tr>
    <td><img src="./simulacion_5.png" width="180"></td>
    <td><img src="./simulacion_6.png" width="180"></td>
    <td><img src="./simulacion_7.png" width="180"></td>
    <td><img src="./simulacion_8.png" width="180"></td>
    <td><img src="./simulacion_9.png" width="180"></td>
  </tr>
</table>

### Video de funcionamiento en simulación

El siguiente enlace abre el video completo del contador decimal funcionando en Proteus.

[Video de funcionamiento](./video_funcionamiento.mp4)

---

## Funcionamiento del circuito

En la simulación se conectó un display de 7 segmentos al puerto D del microcontrolador **PIC16F887**. Cada pin del puerto controla un segmento del display mediante una resistencia de 240 Ω, la cual limita la corriente para proteger tanto al display como al microcontrolador.

El display utilizado trabaja con una combinación específica de segmentos para formar cada número. Por ejemplo, para mostrar el número 0 se encienden casi todos los segmentos excepto el segmento central. Para mostrar el número 1, solamente se encienden los segmentos correspondientes al lado derecho.

El programa envía al puerto D un valor binario distinto para cada número. Estos valores se encuentran almacenados en un arreglo, por lo que el contador solo necesita recorrer el arreglo desde la posición 0 hasta la posición 9.

---

## Lógica de programación

Primero se define un arreglo llamado `numeros`, el cual contiene los patrones necesarios para mostrar los números del 0 al 9:

```c
const unsigned char numeros[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};
```

Después se deshabilitan las entradas analógicas del PIC16F887 para que los pines funcionen como entradas o salidas digitales:

```c
ANSEL = 0x00;
ANSELH = 0x00;
```

Luego se configura el puerto D como salida:

```c
TRISD = 0x00;
```

Dentro del ciclo infinito, el programa recorre los valores del arreglo y los envía al puerto D:

```c
for(unsigned char i = 0; i < 10; i++) {
    PORTD = numeros[i];
    __delay_ms(500);
}
```

El retardo de 500 ms permite observar cada número en el display antes de pasar al siguiente.

---

## Tabla de referencia

| Número | Valor binario | Valor hexadecimal |
| ------ | ------------- | ----------------- |
| 0      | `00111111`    | `0x3F`            |
| 1      | `00000110`    | `0x06`            |
| 2      | `01011011`    | `0x5B`            |
| 3      | `01001111`    | `0x4F`            |
| 4      | `01100110`    | `0x66`            |
| 5      | `01101101`    | `0x6D`            |
| 6      | `01111101`    | `0x7D`            |
| 7      | `00000111`    | `0x07`            |
| 8      | `01111111`    | `0x7F`            |
| 9      | `01101111`    | `0x6F`            |

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

const unsigned char numeros[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

void main(void) {

    ANSEL = 0x00;
    ANSELH = 0x00;
    TRISD = 0x00;

    while(1) {
        for(unsigned char i = 0; i < 10; i++) {
            PORTD = numeros[i];
            __delay_ms(500);
        }
    }
}
```

---

## Resultados

Al ejecutar la simulación, el display de 7 segmentos debe mostrar los números del **0 al 9** de forma secuencial. Cada número permanece visible durante 500 ms antes de cambiar al siguiente. Al llegar al número 9, el ciclo vuelve a comenzar desde 0.

---

## Conclusión

Esta actividad permitió comprender cómo controlar un display de 7 segmentos mediante el microcontrolador PIC16F887. Se reforzó el uso de salidas digitales, arreglos de patrones binarios y configuración de puertos. Además, la simulación en Proteus permitió visualizar la relación directa entre los valores enviados al puerto D y los números mostrados en el display.
