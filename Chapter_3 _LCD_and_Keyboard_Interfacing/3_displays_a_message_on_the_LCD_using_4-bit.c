/* p3_3.c: Initialize and display "HELLO" on the LCD using 4-bit data mode.
 *
 * Because of 4-bit data mode, every byte of command or data is
 * transmitted twice, one for the upper nibble and one for lower nibble.
 * This program does not poll the status of the LCD.
 * It uses delay to wait out the time LCD controller is busy.
 * For simplicity, all delay below 1 ms uses 1 ms. You may
 * want to adjust the amount of delay for your LCD controller
 * to enhance the performance.
 *
 * The LCD controller is connected to the Nucleo-F446RE
 * board as follows:
 *
 * PC4-PC7 for LCD D0-D7, respectively.
 * PB5 for LCD R/S
 * LCD R/W is tied to ground
 * PB7 for LCD EN
 *
 * This program was tested with Keil uVision v5.24a with DFP v2.11.0
 */

#include "stm32f4xx.h"

#define RS 0x20     /* PB5 mask for reg select */
#define EN 0x80     /* PB7 mask for enable */

void delayMs(int n);
void LCD_nibble_write(char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(char data);
void LCD_init(void);
void PORTS_init(void);


int main(void) {
    /* initialize LCD controller */
    LCD_init();

    while(1) {
        /* Write "HELLO" on LCD */
        LCD_data('H');
        LCD_data('E');
        LCD_data('L');
        LCD_data('L');
        LCD_data('O');
        delayMs(1000);

        /* clear LCD display */
        LCD_command(1);
        delayMs(1000);
    }
}

/* initialize GPIOB/C then initialize LCD controller */
void LCD_init(void) {
    PORTS_init();

    delayMs(20);                /* LCD controller reset sequence */
    LCD_nibble_write(0x30, 0);
    delayMs(5);
    LCD_nibble_write(0x30, 0);
    delayMs(1);
    LCD_nibble_write(0x30, 0);
    delayMs(1);

    LCD_nibble_write(0x20, 0);  /* use 4-bit data mode */
    delayMs(1);
    LCD_command(0x28);          /* set 4-bit data, 2-line, 5x7 font */
    LCD_command(0x06);          /* move cursor right */
    LCD_command(0x01);          /* clear screen, move cursor to home */
    LCD_command(0x0F);          /* turn on display, cursor blinking */
}

void PORTS_init(void) {
    RCC->AHB1ENR |=  0x06;          /* enable GPIOB/C clock */

    /* PORTB 5 for LCD R/S */
    /* PORTB 7 for LCD EN */
    GPIOB->MODER &= ~0x0000CC00;    /* clear pin mode */
    GPIOB->MODER |=  0x00004400;    /* set pin output mode */
    GPIOB->BSRR = 0x00800000;       /* turn off EN */

    /* PC4-PC7 for LCD D4-D7, respectively. */
    GPIOC->MODER &= ~0x0000FF00;    /* clear pin mode */
    GPIOC->MODER |=  0x00005500;    /* set pin output mode */
}

void LCD_nibble_write(char data, unsigned char control) {
    /* populate data bits */
    GPIOC->BSRR = 0x00F00000;       /* clear data bits */
    GPIOC->BSRR = data & 0xF0;      /* set data bits */

    /* set R/S bit */
    if (control & RS)
        GPIOB->BSRR = RS;
    else
        GPIOB->BSRR = RS << 16;

    /* pulse E */
    GPIOB->BSRR = EN;
    delayMs(0);
    GPIOB->BSRR = EN << 16;
}

void LCD_command(unsigned char command) {
    LCD_nibble_write(command & 0xF0, 0);    /* upper nibble first */
    LCD_nibble_write(command << 4, 0);      /* then lower nibble */

    if (command < 4)
        delayMs(2);         /* command 1 and 2 needs up to 1.64ms */
    else
        delayMs(1);         /* all others 40 us */
}

void LCD_data(char data) {
    LCD_nibble_write(data & 0xF0, RS);      /* upper nibble first */
    LCD_nibble_write(data << 4, RS);        /* then lower nibble */

    delayMs(1);
}

/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ;
}