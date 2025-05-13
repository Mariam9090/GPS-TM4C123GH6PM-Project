#include "../headers/LCD.h"
#include "../headers/Buzzer.h"
#include "../headers/tm4c123gh6pm.h"
#include "../headers/GPS.h"

void buzzer_Init(void) {
		// Port E3
    SYSCTL_RCGCGPIO_R |= 0x00010; 
    GPIO_PORTE_AFSEL_R &=~0x08;
    GPIO_PORTE_PCTL_R &= ~0x000F000;
    GPIO_PORTE_DIR_R |= 0x08; //OUTPUT
    GPIO_PORTE_DEN_R |= 0x08;
    GPIO_PORTE_AMSEL_R &= ~0x08;
}
void Beep(void) 
{
	GPIO_PORTE_DATA_R |=0x08;
	delayMilliseconds(1000);
  GPIO_PORTE_DATA_R &=~0x08;
}




