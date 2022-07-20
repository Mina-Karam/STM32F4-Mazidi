/* p3_4.c: Matrix keypad detect
 *
 * This program checks a 4x4 matrix keypad to see whether
 * a key is pressed or not. When a key is pressed, it turns
 * on the LD2 (green LED).
 *
 * PC0-3 are connected to the columns and PC4-7 are
 * connected to the rows of the keypad.
 *
 * This program was tested with Keil uVision v5.24a with DFP v2.11.0
 */

#include "stm32f4xx.h"

void delay(void);
void keypad_init(void);
char keypad_kbhit(void);

int main(void) {
    keypad_init();

    // enable PA5 for green LED
    RCC->AHB1ENR |=  1;             /* enable GPIOA clock */
    GPIOA->MODER &= ~0xC0000C00;    /* clear pin mode */
    GPIOA->MODER |=  0x00000400;    /* set pin output mode */

    while(1) {
        if (keypad_kbhit() != 0)    /* if a key is pressed */
            GPIOA->BSRR = 0x00000020;   /* turn on green LED */
        else
            GPIOA->BSRR = 0x00200000;   /* turn off green LED */
    }
}

/* this function initializes PC0-3 (column) and PC4-7 (row).
 * The column pins need to have the pull-up resistors enabled.
 */
void keypad_init(void) {
    RCC->AHB1ENR |=  0x14;          /* enable GPIOC clock */
    GPIOC->MODER &= ~0x0000FFFF;    /* clear pin mode to input */
    GPIOC->PUPDR =   0x00000055;    /* enable pull up resistors for column pins */
}

/* This is a non-blocking function.
 * If a key is pressed, it returns 1. Otherwise, it returns a 0.
 */
char keypad_kbhit(void) {
    int col;

    GPIOC->MODER = 0x00005500;      /* make all row pins output */
    GPIOC->BSRR =  0x00F00000;      /* drive all row pins low */
    delay();                        /* wait for signals to settle */
    col = GPIOC->IDR & 0x000F;      /* read all column pins */
    GPIOC->MODER &= ~0x0000FF00;    /* disable all row pins drive */
    if (col == 0x000F)              /* if all columns are high */
        return 0;                   /* no key pressed */
    else
        return 1;                   /* a key is pressed */
}

/* make a small delay */
void delay(void) {
    int i;
    for (i = 0; i < 20; i++) ;
}