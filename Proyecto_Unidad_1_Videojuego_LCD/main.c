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

unsigned char dinosaurio[8] = {
    0b00111,
    0b00101,
    0b10111,
    0b11110,
    0b11111,
    0b01110,
    0b01010,
    0b11011
};

unsigned char carita[8] = {
    0b00000,
    0b01010,
    0b01010,
    0b00000,
    0b10001,
    0b01110,
    0b00000,
    0b00000
};

unsigned int ADC_Read(unsigned char canal){
    ADCON0 = (canal << 2) | 0x01;
    __delay_ms(2);
    GO_nDONE = 1;
    while(GO_nDONE);
    return (ADRESH << 8) + ADRESL;
}

void CrearCaracter(unsigned char pos, unsigned char dibujo[]){
    LCD_Cmd(0x40 + (pos * 8));
    for(int i = 0; i < 8; i++){
        LCD_putc(dibujo[i]);
    }
    LCD_Cmd(0x80);
}

void main(void){
    LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7};
    LCD_Init(lcd);

    ANSEL = 0x03;
    ANSELH = 0x00;
    ADCON1 = 0x80;

    TRISA = 0x03;
    TRISBbits.TRISB0 = 1;

    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0 = 1;

    CrearCaracter(0, dinosaurio);
    CrearCaracter(1, carita);

    int x, y;
    int pos = 7;
    int linea = 0;

    while(1){

        x = ADC_Read(0);
        y = ADC_Read(1);

        if(x < 400){
            pos--;
            __delay_ms(120);
        }

        if(x > 600){
            pos++;
            __delay_ms(120);
        }

        if(pos < 0){
            pos = 15;
        }

        if(pos > 15){
            pos = 0;
        }

        if(y < 400){
            linea = 0;
        }

        if(y > 600){
            linea = 1;
        }

        LCD_Clear();
        LCD_Set_Cursor(linea, pos);

        if(PORTBbits.RB0 == 0){
            LCD_putc(1);
        }
        else{
            LCD_putc(0);
        }

        __delay_ms(80);
    }
}
