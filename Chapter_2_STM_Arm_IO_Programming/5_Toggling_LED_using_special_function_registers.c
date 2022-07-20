
/* p2_5.c Toggle Green LED (LD2) on STM32F446RE Nucleo64 board at 1 Hz
 *
 * This program toggles LD2 for 0.5 second ON and 0.5 second OFF.
 * It is identical to Program 2-2 but was written without using
 * the header file. The registers are defined locally in the file.
 *
 * This program was tested with Keil uVision v5.24a with DFP v2.11.0
 */

#define RCC_AHB1ENR (*((volatile unsigned int*)0x40023830))
#define GPIOA_MODER (*((volatile unsigned int*)0x40020000))
#define GPIOA_BSRR  (*((volatile unsigned int*)0x40020018))

void delayMs(int n);

int main(void) {
    RCC_AHB1ENR |=  1;              /* enable GPIOA clock */
    
    GPIOA_MODER &= ~0x00000C00;     /* clear pin mode */
    GPIOA_MODER |=  0x00000400;     /* set pin to output mode */
    
    while(1) {
        GPIOA_BSRR = 0x00000020;    /* turn on LED */
        delayMs(500);
        GPIOA_BSRR = 0x00200000;    /* turn off LED */
        delayMs(500);
    }
}

/* 16 MHz SYSCLK */
void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ;
}