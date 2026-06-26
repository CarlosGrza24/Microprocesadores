#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lcdb.h"

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

unsigned char fila_jugador = 0;
unsigned char col_jugador = 0;
unsigned char col_balon = 2;

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

unsigned char jugador[8] = {
    0b00100,
    0b01110,
    0b00100,
    0b01110,
    0b10101,
    0b00110,
    0b01001,
    0b10000
};

void ADC_Init(){
    ANSEL = 0x03;      // RA0 y RA1 analogicos
    ANSELH = 0x00;

    ADCON0 = 0x81;
    ADCON1 = 0x80;
}

unsigned int ADC_Read(unsigned char canal){
    ADCON0 = (canal << 2) | 0x01;
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

void Leer_Joystick(){
    unsigned int adc_y;
    unsigned int adc_x;

    adc_y = ADC_Read(0);   // RA0 = Y
    adc_x = ADC_Read(1);   // RA1 = X

    if(adc_y < 400){
        fila_jugador = 0;
    }
    else if(adc_y > 600){
        fila_jugador = 1;
    }

    col_jugador = ((unsigned long)adc_x * 12) / 1023;
    col_balon = col_jugador + 2;

    if(col_balon > 14){
        col_balon = 14;
    }
}

void Mandar_Fila_Balon(unsigned char fila){
    if(fila == 0){
        PORTBbits.RB2 = 0;
    }
    else{
        PORTBbits.RB2 = 1;
    }
}

void Dibujar_Jugador_Balon(){
    LCD_Clear();

    LCD_Set_Cursor(fila_jugador, col_jugador);
    LCD_putc(1);

    LCD_Set_Cursor(fila_jugador, col_balon);
    LCD_putc(0);

    LCD_Set_Cursor(0,15);
    LCD_putc('|');

    LCD_Set_Cursor(1,15);
    LCD_putc('|');
}

void Reiniciar_Balon(){
    fila_jugador = 0;
    col_jugador = 0;
    col_balon = 2;

    PORTBbits.RB2 = 0;
    PORTBbits.RB3 = 0;

    LCD_Clear();

    LCD_Set_Cursor(fila_jugador, col_jugador);
    LCD_putc(1);

    LCD_Set_Cursor(fila_jugador, col_balon);
    LCD_putc(0);

    LCD_Set_Cursor(0,15);
    LCD_putc('|');

    LCD_Set_Cursor(1,15);
    LCD_putc('|');
}

void Mostrar_Gol(){
    LCD_Clear();

    LCD_Set_Cursor(0,0);
    LCD_putrs("GOOOO");

    LCD_Set_Cursor(1,0);
    LCD_putrs("Gol!");

    __delay_ms(10000);

    Reiniciar_Balon();
}

void Mostrar_Atajada(){
    LCD_Clear();

    LCD_Set_Cursor(0,0);
    LCD_putrs("ATAJADA!");

    LCD_Set_Cursor(1,0);
    LCD_putrs("Reinicio");

    __delay_ms(5000);

    Reiniciar_Balon();
}

void Esperar_Resultado(){
    unsigned int tiempo = 0;

    while(tiempo < 20000){

        if(PORTBbits.RB5 == 1){
            Mostrar_Gol();
            return;
        }

        if(PORTBbits.RB4 == 1){
            Mostrar_Atajada();
            return;
        }

        __delay_ms(100);
        tiempo += 100;
    }

    Reiniciar_Balon();
}

void Disparo(){
    unsigned char col;
    unsigned char fila_tiro;
    unsigned char col_tiro;

    fila_tiro = fila_jugador;
    col_tiro = col_balon;

    Mandar_Fila_Balon(fila_tiro);

    for(col = col_tiro; col <= 15; col++){

        Leer_Joystick();

        LCD_Clear();

        LCD_Set_Cursor(fila_jugador, col_jugador);
        LCD_putc(1);

        LCD_Set_Cursor(fila_tiro, col);
        LCD_putc(0);

        LCD_Set_Cursor(0,15);
        LCD_putc('|');

        LCD_Set_Cursor(1,15);
        LCD_putc('|');

        __delay_ms(70);
    }

    PORTBbits.RB3 = 1;
    __delay_ms(500);
    PORTBbits.RB3 = 0;

    Esperar_Resultado();
}

void main(void){
    ADC_Init();

    TRISAbits.TRISA0 = 1;   // Joystick Y
    TRISAbits.TRISA1 = 1;   // Joystick X

    TRISBbits.TRISB0 = 1;   // Boton SW del joystick
    TRISBbits.TRISB2 = 0;   // Manda fila
    TRISBbits.TRISB3 = 0;   // Manda tiro
    TRISBbits.TRISB4 = 1;   // Recibe atajada
    TRISBbits.TRISB5 = 1;   // Recibe gol

    OPTION_REGbits.nRBPU = 0;   // Pull-ups de PORTB activados
    WPUBbits.WPUB0 = 1;         // Pull-up en RB0

    PORTBbits.RB2 = 0;
    PORTBbits.RB3 = 0;

    LCD_Init(lcd);

    CrearCaracter(0, balon_negro);
    CrearCaracter(1, jugador);

    LCD_Clear();

    while(1){
        Leer_Joystick();
        Mandar_Fila_Balon(fila_jugador);
        Dibujar_Jugador_Balon();

        // Boton del joystick presionado = 0
        if(PORTBbits.RB0 == 0){
            __delay_ms(50);

            if(PORTBbits.RB0 == 0){
                Disparo();

                while(PORTBbits.RB0 == 0);
                __delay_ms(300);
            }
        }

        __delay_ms(100);
    }
}