#define _XTAL_FREQ 8000000

#include <stdbool.h>
#include <xc.h>
#include "lcd.h"

LCD lcd;

void LCD_Out(char c) {
    
    if (c & 1) {
        *(lcd.PORT) |= 1 << lcd.D4;
    }
    else {
        *(lcd.PORT) &= ~(1 << lcd.D4);
    }

    if (c & 2) {
        *(lcd.PORT) |= 1 << lcd.D5;
    }
    else {
        *(lcd.PORT) &= ~(1 << lcd.D5);
    }

    if (c & 4) {
        *(lcd.PORT) |= 1 << lcd.D6;
    }
    else {
        *(lcd.PORT) &= ~(1 << lcd.D6);
    }

    if (c & 8) {
        *(lcd.PORT) |= 1 << lcd.D7;
    }
    else {
        *(lcd.PORT) &= ~(1 << lcd.D7);
    }
}

void LCD_Write(unsigned char c) {

    *(lcd.PORT) &= ~(1 << lcd.RS);
    LCD_Out(c);

    *(lcd.PORT) |= 1 << lcd.EN;
    __delay_ms(4);
    *(lcd.PORT) &= ~(1 << lcd.EN);
}

bool LCD_Init(LCD display) {

    lcd = display;

    if (lcd.PORT == &PORTA) {
        TRISA = 0x00;
    }
    else if (lcd.PORT == &PORTB) {
        TRISB = 0x00;
    }
    else if (lcd.PORT == &PORTC) {
        TRISC = 0x00;
    }
#if defined(_16F877) || defined(_16F877A)
    else if (lcd.PORT == &PORTD) {
        TRISD = 0x00;
    }
    else if (lcd.PORT == &PORTE) {
        TRISE = 0x00;
    }
#endif
    else {
        return false;
    }

    __delay_ms(20);

    LCD_Write(0x03);
    __delay_ms(5);
    LCD_Write(0x03);
    __delay_ms(16);
    LCD_Write(0x03);

    LCD_Write(0x02);

    LCD_Cmd(0x28);

    LCD_Cmd(0x06);

    LCD_Display(true, false, false);

    LCD_Clear();
    
    return true;
}

void LCD_putc(char c) {

    *(lcd.PORT) |= 1 << lcd.RS;

    LCD_Out((c & 0xF0) >> 4);

    *(lcd.PORT) |= 1 << lcd.EN;
    __delay_us(40);
    *(lcd.PORT) &= ~(1 << lcd.EN);

    LCD_Out(c & 0x0F);

    *(lcd.PORT) |= 1 << lcd.EN;
    __delay_us(40);
    *(lcd.PORT) &= ~(1 << lcd.EN);
}

void LCD_puts(char *a) {

    for (int i = 0; a[i] != '\0'; ++i) {
        LCD_putc(a[i]);
    }
}

void LCD_putrs(const char *a) {

    for (int i = 0; a[i] != '\0'; ++i) {
        LCD_putc(a[i]);
    }
}
