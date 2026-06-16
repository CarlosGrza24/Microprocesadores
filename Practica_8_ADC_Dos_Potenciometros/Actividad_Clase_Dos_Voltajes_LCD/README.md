# Actividad en clase — Lectura de dos voltajes con LCD

## Descripción

En esta actividad se utilizó el microcontrolador **PIC16F887** para realizar la lectura de **dos voltajes analógicos** mediante dos potenciómetros conectados a entradas analógicas del microcontrolador.

El objetivo principal fue utilizar el módulo **ADC** para convertir las señales analógicas de ambos potenciómetros en valores digitales, y posteriormente mostrar la información en una pantalla **LCD 16x2**.

Esta práctica permitió reforzar el uso de entradas analógicas, conversión analógica-digital, lectura de más de un canal ADC y visualización de datos en una pantalla LCD.

---

## Componentes utilizados

* PIC16F887
* Pantalla LCD 16x2
* 2 potenciómetros
* Potenciómetro para ajuste de contraste del LCD
* Cristal oscilador
* Botón de reset
* Resistencia para MCLR
* Fuente Vcc
* Tierra GND
* MPLAB X IDE
* Compilador XC8
* Proteus Design Suite
* Librería LCD

---

## Evidencias

### Simulación en Proteus

[![Video de funcionamiento](./evidencias_fisicas/simu_volt1.gif)](./evidencias_fisicas/simu_volt1.mp4)

---

## Evidencias físicas

Además de la simulación en Proteus, la práctica puede implementarse físicamente utilizando el microcontrolador **PIC16F887**, una pantalla LCD 16x2 y dos potenciómetros.

### Armado general del circuito

![Armado físico](./evidencias_fisicas/armado_volt2.jpeg)

### Carpeta completa de evidencias físicas

[Ver evidencias físicas](./evidencias_fisicas)

---

## Funcionamiento del circuito

El circuito utiliza dos potenciómetros conectados a entradas analógicas del microcontrolador. Cada potenciómetro entrega un voltaje variable dependiendo de la posición de su perilla.

El PIC16F887 convierte estos voltajes analógicos en valores digitales mediante el módulo ADC. Posteriormente, el programa calcula el voltaje correspondiente y lo muestra en la pantalla LCD.

La pantalla LCD permite visualizar ambos valores de manera ordenada, por ejemplo:

```text
Voltaje 1: 2.50V
Voltaje 2: 4.10V
```

---

## Lógica de programación

La lógica general de esta actividad consiste en inicializar el módulo ADC, leer dos canales analógicos y mostrar los voltajes obtenidos en la pantalla LCD.

Primero se configuran dos canales analógicos, por ejemplo `AN0` y `AN1`, para leer los dos potenciómetros. Después, el programa realiza la conversión ADC de cada canal.

Cada valor ADC se convierte a voltaje usando una relación proporcional, tomando en cuenta que el ADC del PIC16F887 es de 10 bits, por lo que su rango va de `0` a `1023`.

La fórmula utilizada es:

```text
Voltaje = ADC * 5.0 / 1023
```

Finalmente, los valores calculados se muestran en la pantalla LCD.

---

## Código utilizado

```c
/*
Código pendiente por agregar.

En esta actividad se realizó la lectura de dos voltajes analógicos
usando dos potenciómetros y el módulo ADC del PIC16F887.

Cuando se encuentre el código original, se puede agregar completo
en esta sección.
*/
```

---

## Resultado esperado

Al ejecutar la simulación, la pantalla LCD debe mostrar los voltajes correspondientes a la posición de ambos potenciómetros. Al mover cada potenciómetro, el valor mostrado en la LCD debe cambiar proporcionalmente.

---

## Conclusión

Esta actividad permitió comprender cómo leer más de una señal analógica utilizando el ADC del PIC16F887. También se reforzó el uso de la pantalla LCD para mostrar variables numéricas y el manejo de distintos canales analógicos dentro de un mismo programa.
