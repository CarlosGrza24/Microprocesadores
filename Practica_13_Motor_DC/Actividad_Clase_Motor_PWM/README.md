# Actividad en clase — Control de motor DC con PWM

## Descripción

En esta actividad se controló la velocidad de un **motor DC** utilizando el módulo PWM por hardware del PIC16F887. La velocidad del motor se ajusta mediante un potenciómetro conectado a `RA0/AN0`.

El motor gira en una sola dirección y su velocidad depende del duty cycle generado por el PWM.

---

## Componentes utilizados

- PIC16F887
- Motor DC
- Driver de motor o puente H
- Potenciómetro
- Fuente externa para motor
- Tierra común
- Cristal oscilador
- Botón de reset
- MPLAB X IDE
- Compilador XC8
- Proteus Design Suite

---

## Evidencias

### Simulación en Proteus

[![Simulación PWM Motor](./evidencias_fisicas/motordc_sim.gif)](./evidencias_fisicas/motordc_sim.mp4)

## Evidencias físicas  

### Armado general del circuito 
![Armado físico PWM Motor](./evidencias_fisicas/motordc_armado_general.jpeg) 
### Video de funcionamiento físico 

[![Funcionamiento PWM Motor](./evidencias_fisicas/motordc_fisico.gif)](./evidencias_fisicas/motordc_fisico.mp4)

---

## Funcionamiento del circuito

El potenciómetro conectado a `AN0` se lee mediante el ADC. El valor obtenido se usa para modificar el duty cycle del PWM en `RC2/CCP1`.

Las señales `IN1` e `IN2` definen la dirección del motor. En esta actividad el motor se mantiene en dirección hacia adelante.

---

## Lógica de programación

El motor se configura hacia adelante con:

```c
IN1 = 1;
IN2 = 0;
```

La velocidad se controla con:

```c
pot = ADC_Read(0);
PWM_SetDuty(pot);
```

---

## Código utilizado

```c
#include <xc.h>
#include <stdbool.h>

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 8000000

#define IN1 PORTCbits.RC1
#define IN2 PORTCbits.RC3

void ADC_Init(void);
unsigned int ADC_Read(unsigned char canal);
void PWM_Init(void);
void PWM_SetDuty(unsigned int duty);
void Motor_Adelante(void);
void Motor_Stop(void);

void main(void) {
    unsigned int pot;
    unsigned int velocidad;

    ADC_Init();
    PWM_Init();

    TRISCbits.TRISC1 = 0;   // IN1
    TRISCbits.TRISC3 = 0;   // IN2

    Motor_Adelante();

    while(1) {
        pot = ADC_Read(0);      // Lee RA0/AN0

        velocidad = pot;        // Valor de 0 a 1023

        PWM_SetDuty(velocidad); // Controla velocidad en RC2/CCP1

        __delay_ms(10);
    }
}

void ADC_Init(void) {
    ANSEL = 0x01;      // RA0/AN0 analógico
    ANSELH = 0x00;     // Los demás digitales

    ADCON0 = 0x01;     // ADC encendido, canal AN0
    ADCON1 = 0x80;     // Justificado a la derecha

    TRISAbits.TRISA0 = 1;
}

unsigned int ADC_Read(unsigned char canal) {
    ADCON0 &= 0b11000011;
    ADCON0 |= (canal << 2);

    __delay_us(30);

    GO_nDONE = 1;
    while(GO_nDONE);

    return (unsigned int)(((unsigned int)ADRESH << 8) | ADRESL);
}

void PWM_Init(void) {
    TRISCbits.TRISC2 = 0;    // RC2/CCP1 como salida PWM

    PR2 = 255;               // Periodo PWM

    CCP1CON = 0b00001100;    // CCP1 en modo PWM

    T2CON = 0b00000111;      // Timer2 ON, prescaler 1:16

    PWM_SetDuty(0);
}

void PWM_SetDuty(unsigned int duty) {
    if(duty > 1023) {
        duty = 1023;
    }

    CCPR1L = (unsigned char)(duty >> 2);

    CCP1CON = (CCP1CON & 0b11001111) | ((duty & 0x03) << 4);
}

void Motor_Adelante(void) {
    IN1 = 1;
    IN2 = 0;
}

void Motor_Stop(void) {
    IN1 = 0;
    IN2 = 0;
    PWM_SetDuty(0);
}
```

---

## Resultado esperado

Al mover el potenciómetro, la velocidad del motor debe cambiar proporcionalmente.

---

## Conclusión

Esta actividad permitió aplicar PWM al control de velocidad de un motor DC, integrando lectura ADC y señal de control hacia un driver de motor.
