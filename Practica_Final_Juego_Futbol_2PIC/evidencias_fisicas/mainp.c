#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lcdp.h"

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 8000000

LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7};

unsigned char fila_portero = 0;
unsigned char fila_balon = 0;

unsigned char balon_negro[8] = {
    0b01110,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b01110,
    0b00000
};

unsigned char portero[8] = {
    0b00100,
    0b01110,
    0b10101,
    0b00100,
    0b01110,
    0b10101,
    0b01010,
    0b10001
};

void ADC_Init(){
    ANSEL = 0x01;
    ANSELH = 0x00;

    ADCON0 = 0x81;
    ADCON1 = 0x80;
}

unsigned int ADC_Read(){
    ADCON0 = 0x81;
    __delay_ms(2);

    GO_nDONE = 1;
    while(GO_nDONE);

    return ((ADRESH << 8) + ADRESL);
}

void CrearCaracter(unsigned char pos, unsigned char dibujo[]){
    LCD_Cmd(0x40 + (pos * 8));

    for(unsigned char i = 0; i < 8; i++){
        LCD_putc(dibujo[i]);
    }

    LCD_Cmd(0x80);
}

void Leer_Joystick_Portero(){
    unsigned int adc;

    adc = ADC_Read();

    if(adc < 400){
        fila_portero = 0;
    }
    else if(adc > 600){
        fila_portero = 1;
    }
}

void Leer_Fila_Balon(){
    if(PORTBbits.RB1 == 0){
        fila_balon = 0;
    }
    else{
        fila_balon = 1;
    }
}

void Dibujar_Portero(){
    LCD_Clear();

    LCD_Set_Cursor(0,15);
    LCD_putc('|');

    LCD_Set_Cursor(1,15);
    LCD_putc('|');

    LCD_Set_Cursor(fila_portero,14);
    LCD_putc(1);
}

void Reiniciar_Portero(){
    fila_portero = 0;
    fila_balon = 0;

    PORTBbits.RB4 = 0;
    PORTBbits.RB5 = 0;

    LCD_Clear();

    LCD_Set_Cursor(0,15);
    LCD_putc('|');

    LCD_Set_Cursor(1,15);
    LCD_putc('|');

    LCD_Set_Cursor(fila_portero,14);
    LCD_putc(1);
}

void Atajada_Portero(){
    PORTBbits.RB4 = 1;
    __delay_ms(500);
    PORTBbits.RB4 = 0;

    LCD_Clear();

    LCD_Set_Cursor(0,0);
    LCD_putrs("ATAJADA!");

    LCD_Set_Cursor(1,0);
    LCD_putrs("Reinicio");

    __delay_ms(5000);

    Reiniciar_Portero();
}

void Gol_Portero(){
    PORTBbits.RB5 = 1;
    __delay_ms(500);
    PORTBbits.RB5 = 0;

    LCD_Clear();

    LCD_Set_Cursor(0,0);
    LCD_putrs("OOOOOL");

    LCD_Set_Cursor(1,0);
    LCD_putrs("Gol!");

    __delay_ms(10000);

    Reiniciar_Portero();
}

void Recibir_Tiro(){
    unsigned char col;
    unsigned char fila_tiro;

    Leer_Fila_Balon();
    fila_tiro = fila_balon;

    for(col = 0; col <= 14; col++){

        Leer_Joystick_Portero();

        LCD_Clear();

        LCD_Set_Cursor(fila_tiro, col);
        LCD_putc(0);

        LCD_Set_Cursor(0,15);
        LCD_putc('|');

        LCD_Set_Cursor(1,15);
        LCD_putc('|');

        LCD_Set_Cursor(fila_portero,14);
        LCD_putc(1);

        __delay_ms(70);
    }

    Leer_Joystick_Portero();

    if(fila_tiro == fila_portero){
        Atajada_Portero();
    }
    else{
        Gol_Portero();
    }

    __delay_ms(300);
}

void main(void){
    ADC_Init();

    TRISAbits.TRISA0 = 1;   // Joystick Y del portero

    TRISBbits.TRISB1 = 1;   // Recibe fila del balon
    TRISBbits.TRISB3 = 1;   // Recibe senal de tiro
    TRISBbits.TRISB4 = 0;   // Manda atajada
    TRISBbits.TRISB5 = 0;   // Manda gol

    PORTBbits.RB4 = 0;
    PORTBbits.RB5 = 0;

    LCD_Init(lcd);

    CrearCaracter(0, balon_negro);
    CrearCaracter(1, portero);

    LCD_Clear();

    while(1){
        Leer_Joystick_Portero();
        Dibujar_Portero();

        if(PORTBbits.RB3 == 1){
            Recibir_Tiro();
            __delay_ms(300);
        }

        __delay_ms(100);
    }
}