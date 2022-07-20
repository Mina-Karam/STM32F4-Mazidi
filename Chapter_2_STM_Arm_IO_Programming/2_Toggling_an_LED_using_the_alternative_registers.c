/* p2_2.c Toggle Green LED (LD2) on STM32F446RE Nucleo64 board at 1 Hz
 *
 * This program toggles LD2 for 0.5 second ON and 0.5 second OFF
 * by writing a '1' to bit 5 or bit 21 of the Bit Set/Reset 
 * Register (BSRR). The lower 16 bits of BSRR turns on the pins
 * and the upper 16 bits of BSRR turns off the pins.
 * Writing a '1' to bit 5 turns on pin 5. Writing a '1' to 
 * bit 21 (5 + 16) turns off pin 5.
 * The green LED (LD2) is connected to PA5.
 * The LED is high active (a '1' turns on the LED).
 * The default system clock is running at 16 MHz.
 *
 * This program was tested with Keil uVision v5.24a with DFP v2.11.0
 */

#include "stm32f4xx.h"

void delayMs(int n);

int main(void) {
    RCC->AHB1ENR |=  1;             /* enable GPIOA clock */
    
    GPIOA->MODER &= ~0x00000C00;    /* clear pin mode */
    GPIOA->MODER |=  0x00000400;    /* set pin to output mode */
    
    while(1) {
        GPIOA->BSRR = 0x00000020;   /* turn on LED */
        delayMs(500);
        GPIOA->BSRR = 0x00200000;   /* turn off LED */
        delayMs(500);
    }
}

/* 16 MHz SYSCLK */
void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ;
}