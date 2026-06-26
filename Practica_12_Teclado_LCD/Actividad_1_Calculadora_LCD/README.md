# Actividad 1 — Calculadora con teclado matricial y LCD

## Descripción

En esta actividad se desarrolló una calculadora básica utilizando un **teclado matricial 4x4**, una pantalla **LCD 16x2** y el microcontrolador **PIC16F887**.

La calculadora permite realizar operaciones de suma, resta, multiplicación y división. La operación se ingresa desde el teclado y el resultado se muestra en la segunda línea de la LCD.

---

## Componentes utilizados

- PIC16F887
- Teclado matricial 4x4
- Pantalla LCD 16x2
- Potenciómetro para contraste
- Cristal oscilador
- Botón de reset
- Fuente Vcc
- Tierra GND
- MPLAB X IDE
- Compilador XC8
- Proteus Design Suite
- Librería LCD

---

## Librería utilizada

Para el manejo de la pantalla LCD se utilizó la librería general del repositorio:

- [`lcd.h`](../../Libreria_LCD/lcd.h)
- [`lcd.c`](../../Libreria_LCD/lcd.c)

---

## Evidencias

### Simulación en Proteus

![Simulación calculadora](./simulacion.png)

### Video de funcionamiento

[▶ Ver video de funcionamiento](./video_funcionamiento.mp4)

---

## Funcionamiento del programa

El programa lee las teclas del teclado matricial y construye dos números: `num1` y `num2`. Después guarda el operador seleccionado y, al presionar `=`, calcula el resultado.

| Tecla | Función |
|---|---|
| `+` | Suma |
| `-` | Resta |
| `X` | Multiplicación |
| `/` | División |
| `C` | Limpiar |
| `=` | Mostrar resultado |

---

## Lógica de programación

El programa usa una variable `state` para saber qué parte de la operación se está capturando.

| Estado | Descripción |
|---|---|
| 0 | Captura del primer número |
| 1 | Operador seleccionado |
| 2 | Captura del segundo número |
| 3 | Resultado mostrado |

La función `Show_Result()` realiza la operación correspondiente y muestra el resultado en la LCD.

---

## Código utilizado

```c
#include <xc.h>
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

void Keypad_Init(void);
char Keypad_Read(void);
char Keypad_GetKey(void);

void Calculator_Reset(void);
void Clear_Line(unsigned char row);
void Print_Number(long num);
void Show_Result(long n1, long n2, char op);

void main(void) {
    char key;
    long num1 = 0;
    long num2 = 0;
    char op = 0;
    unsigned char state = 0;
    unsigned char pos = 0;

    ANSEL = 0x00;
    ANSELH = 0x00;

    LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7};
    LCD_Init(lcd);

    Keypad_Init();
    Calculator_Reset();

    while(1) {
        key = Keypad_GetKey();

        if(key != 0) {
            if(key == 'C') {
                num1 = 0;
                num2 = 0;
                op = 0;
                state = 0;
                pos = 0;
                Calculator_Reset();
            }
            else if(key >= '0' && key <= '9') {
                if(state == 3) {
                    num1 = 0;
                    num2 = 0;
                    op = 0;
                    state = 0;
                    pos = 0;
                    Calculator_Reset();
                }

                if(state == 0) {
                    num1 = (num1 * 10) + (key - '0');
                }
                else if(state == 1 || state == 2) {
                    num2 = (num2 * 10) + (key - '0');
                    state = 2;
                }

                if(pos < 16) {
                    LCD_Set_Cursor(0, pos);
                    LCD_putc(key);
                    pos++;
                }
            }
            else if(key == '+' || key == '-' || key == 'X' || key == '/') {
                if(state == 0 && pos > 0) {
                    op = key;
                    state = 1;

                    if(pos < 16) {
                        LCD_Set_Cursor(0, pos);
                        LCD_putc(key);
                        pos++;
                    }
                }
            }
            else if(key == '=') {
                if(state == 2 && op != 0) {
                    Show_Result(num1, num2, op);
                    state = 3;
                }
            }
        }
    }
}

void Calculator_Reset(void) {
    LCD_Clear();
    LCD_Set_Cursor(0, 0);
    LCD_putrs("0");
    LCD_Set_Cursor(1, 0);
    LCD_putrs("=");
    LCD_Set_Cursor(0, 0);
}

void Clear_Line(unsigned char row) {
    unsigned char i;

    LCD_Set_Cursor(row, 0);
    for(i = 0; i < 16; i++) {
        LCD_putc(' ');
    }
    LCD_Set_Cursor(row, 0);
}

void Show_Result(long n1, long n2, char op) {
    long result;
    long decimal;

    Clear_Line(1);
    LCD_putrs("= ");

    if(op == '+') {
        result = n1 + n2;
        Print_Number(result);
    }
    else if(op == '-') {
        result = n1 - n2;
        Print_Number(result);
    }
    else if(op == 'X') {
        result = n1 * n2;
        Print_Number(result);
    }
    else if(op == '/') {
        if(n2 == 0) {
            LCD_putrs("ERROR");
        }
        else {
            result = n1 / n2;
            decimal = ((n1 % n2) * 100) / n2;

            Print_Number(result);
            LCD_putc('.');

            if(decimal < 10) {
                LCD_putc('0');
            }

            Print_Number(decimal);
        }
    }
}

void Print_Number(long num) {
    char buffer[12];
    char i = 0;

    if(num == 0) {
        LCD_putc('0');
        return;
    }

    if(num < 0) {
        LCD_putc('-');
        num = -num;
    }

    while(num > 0) {
        buffer[i] = (num % 10) + '0';
        num = num / 10;
        i++;
    }

    while(i > 0) {
        i--;
        LCD_putc(buffer[i]);
    }
}

void Keypad_Init(void) {
    TRISD = 0b11110000;   // RD0-RD3 salidas para filas
    PORTD = 0b00001111;   // Filas en 1

    TRISB = 0b11110000;   // RB4-RB7 entradas para columnas
    PORTB = 0x00;

    OPTION_REG &= 0b01111111;   // Activa pull-ups internos de PORTB
    WPUB = 0b11110000;          // Pull-ups en RB4, RB5, RB6, RB7
}

char Keypad_Read(void) {
    const char keys[4][4] = {
        {'1', '2', '3', '-'},
        {'4', '5', '6', 'X'},
        {'7', '8', '9', '/'},
        {'C', '0', '=', '+'}
    };

    unsigned char row;

    for(row = 0; row < 4; row++) {
        PORTD = 0b00001111;

        if(row == 0) PORTD = 0b00001110;
        else if(row == 1) PORTD = 0b00001101;
        else if(row == 2) PORTD = 0b00001011;
        else if(row == 3) PORTD = 0b00000111;

        __delay_us(50);

        if((PORTB & 0b00010000) == 0) {
            return keys[row][0];
        }

        if((PORTB & 0b00100000) == 0) {
            return keys[row][1];
        }

        if((PORTB & 0b01000000) == 0) {
            return keys[row][2];
        }

        if((PORTB & 0b10000000) == 0) {
            return keys[row][3];
        }
    }

    PORTD = 0b00001111;
    return 0;
}

char Keypad_GetKey(void) {
    static char lastKey = 0;
    char key;

    key = Keypad_Read();

    if(key != 0 && lastKey == 0) {
        __delay_ms(5);

        if(Keypad_Read() == key) {
            lastKey = key;
            return key;
        }
    }

    if(key == 0) {
        lastKey = 0;
    }

    return 0;
}
```

---

## Resultado esperado

El usuario puede ingresar operaciones desde el teclado, por ejemplo:

```text
12+8
= 20
```

Si se realiza una división entre cero, la pantalla muestra `ERROR`.

---

## Conclusión

Esta actividad permitió integrar teclado matricial, LCD, manejo de estados y operaciones aritméticas en un sistema interactivo.
