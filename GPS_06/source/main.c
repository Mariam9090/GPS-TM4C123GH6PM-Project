#include "../headers/GPS.h"
#include "../headers/LCD.h"
#include "../headers/UART.h"
#include "../headers/Buzzer.h"
#include <TM4C123.h>

int main(){ 
	  UART5_Init();
    GPS_Init();
    LCD_Init();
    buzzer_Init();
	
    LCD_Clear();
    LCD_DisplayString(0, 0, "GPS System");
    LCD_DisplayString(1, 0, "Initializing...");
    do {
        GPS_Read();
				static uint8_t dotCount = 0;
        LCD_SetCursor(1, 15);
        if (dotCount % 4 == 0) {
           LCD_PutChar(' ');
        } else {
            LCD_PutChar('.');
        }
        dotCount++;
        delayMilliseconds(500);
				LCD_DisplayString(1, 0, "NO SIGNAL...");
    } while (!GPS_IsFixed());
		const char* old_name=" ";
    while (1) {
       const char* landmarkName = GPS_GetNearestLandmarkName();
			 if(landmarkName!=old_name){
			 old_name=landmarkName;
			 LCD_DisplayGPSInfo(landmarkName);
			 Beep(); 
			 }  
       GPS_Read();
    }

}
