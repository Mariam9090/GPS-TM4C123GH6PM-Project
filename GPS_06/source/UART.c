#include "../headers/UART.h"
#include "../headers/tm4c123gh6pm.h"
#include <TM4C123.h>
#include <stdbool.h>
#include <stdio.h>

void GPS_Init(void) {
    // Enable the clock for GPIO Port C and UART1
    SYSCTL_RCGCUART_R |= 0x02;  
    SYSCTL_RCGCGPIO_R |= 0x04;
    while ((SYSCTL_PRGPIO_R & 0x04) == 0); 
		UART1_CTL_R &= ~0x0003;
    UART1_IBRD_R = 104; // IBRD = int(1600000/(16*9600)) = int(104.11)
    UART1_FBRD_R = 11; // FBRD = int(0.8333 * 64 + 0.5) = int(53.125)
    UART1_LCRH_R = 0x0070;
    UART1_CTL_R = 0x0301;
    GPIO_PORTC_AFSEL_R |= 0x30;
    GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF) + 0x00220000 ; // PC4/5 for UART1 RX/TX

    GPIO_PORTC_DEN_R |= 0x30; 
    GPIO_PORTC_AMSEL_R &= ~0x30; 
  	SCB->CPACR |= 0x00F00000; // Enable the FPU
}

// UART get char
char UART_GetChar(void) {
    while ((UART1_FR_R & 0x0010) != 0 ){};
    return  (char)(UART1_DR_R & 0xFF);
}

void UART5_Init(void) {
    // Enable the clock for GPIO Port E and UART5
    SYSCTL_RCGCUART_R |= 0x0020;  
    SYSCTL_RCGCGPIO_R |= 0x0010;  
    UART5_CTL_R &= ~0x0001; 
    UART5_IBRD_R = 104;
    UART5_FBRD_R = 11; 
    UART5_LCRH_R = 0x0070; 
    UART5_CTL_R = 0x0301; 
    GPIO_PORTE_AFSEL_R |= 0x030; 
    GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R&0xFF00FFFF) + 0x00110000 ; // PE4/5 for UART0 RX/TX
    GPIO_PORTE_DEN_R |= 0x30; 
    GPIO_PORTE_AMSEL_R &= ~0x30; 
}

void UART5_PutChar(char data) {
	while ((UART5_FR_R & 0x0020) != 0 );
	UART5_DR_R = data;
}

void print5_str(char *str)
{
	while(*str)
	{
		if(*str=='\n')
			UART5_PutChar('\r');
		UART5_PutChar(*str++);
	}
}
