; p2_6.s
; Assembly program to toggle the green LED (LD2)
;   on STM32F446RE Nucleo64 board at 1 Hz
;
            EXPORT  __Vectors
            EXPORT  Reset_Handler
            AREA    vectors, CODE, READONLY

__Vectors   DCD     0x10010000  ; 0x20008000    ; Top of Stack
            DCD     Reset_Handler               ; Reset Handler

RCC_AHB1ENR equ 0x40023830
GPIOA_MODER equ 0x40020000
GPIOA_BSRR  equ 0x40020018

            AREA    PROG, CODE, READONLY
Reset_Handler
            ldr     r4, =RCC_AHB1ENR    ; enable GPIOA clock
            ldr     r5, [r4]
            orr     r5, #1
            str     r5, [r4]
            
            ldr     r4, =GPIOA_MODER    ; set pin to output mode
            ldr     r5, [r4]
            bic     r5, #0x00000C00
            orr     r5, #0x00000400
            str     r5, [r4]

L1          ldr     r4, =GPIOA_BSRR
            ldr     r5, =0x00000020    ; turn on LED
            str     r5, [r4]
            mov     r0, #500
            bl      delay
            
            ldr     r4, =GPIOA_BSRR
            ldr     r5, =0x00200000    ; turn off LED
            str     r5, [r4]
            mov     r0, #500
            bl      delay
            b       L1                 ; loop forever

; delay milliseconds in R0
delay       ldr     r1, =5325
DL1         subs    r1, r1, #1
            bne     DL1
            subs    r0, r0, #1
            bne     delay
            bx      lr
            
            end