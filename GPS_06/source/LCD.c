#include <stdint.h>
#include "../headers/tm4c123gh6pm.h"
#include "../headers/LCD.h"

void delayMicroseconds(uint32_t us) {
    for (uint32_t i = 0; i < us * 3; i++) {
        __asm("NOP");
    }
}

void delayMilliseconds(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        delayMicroseconds(1000);
    }
}

void LCD_PortInit(void) {
    SYSCTL_RCGCGPIO_R |= (SYSCTL_RCGCGPIO_R1 | SYSCTL_RCGCGPIO_R3);  // Enable Port B & D
    while ((SYSCTL_PRGPIO_R & (SYSCTL_PRGPIO_R1 | SYSCTL_PRGPIO_R3)) !=
           (SYSCTL_PRGPIO_R1 | SYSCTL_PRGPIO_R3));

    // Port B (PB0-PB3) as output for data
    LCD_DATA_DIR |= 0x0F;
    LCD_DATA_DEN |= 0x0F;

    // Port D (PD0, PD1) as output for RS and EN
    LCD_CTRL_DIR |= (LCD_RS | LCD_EN);
    LCD_CTRL_DEN |= (LCD_RS | LCD_EN);
}


void LCD_EnablePulse(void) {
    LCD_CTRL_PORT &= ~LCD_EN;
    delayMicroseconds(1);
    LCD_CTRL_PORT |= LCD_EN;
    delayMicroseconds(1);
    LCD_CTRL_PORT &= ~LCD_EN;
    delayMicroseconds(100); 
}

void LCD_Send4Bits(uint8_t data) {
    LCD_DATA_PORT = (LCD_DATA_PORT & ~0x0F) | (data & 0x0F);
    LCD_EnablePulse();
}

void LCD_Command(uint8_t cmd) {
    LCD_CTRL_PORT &= ~LCD_RS;  // Command mode
    LCD_Send4Bits(cmd >> 4);
    LCD_Send4Bits(cmd & 0x0F);

    if (cmd < 4)
        delayMilliseconds(2);
    else
        delayMicroseconds(40);
}

void LCD_Data(uint8_t data) {
    LCD_CTRL_PORT |= LCD_RS;  // Data mode
    LCD_Send4Bits(data >> 4);
    LCD_Send4Bits(data & 0x0F);
    delayMicroseconds(40);
}

void LCD_Init(void) {
    LCD_PortInit();
    delayMilliseconds(40);

    LCD_Send4Bits(0x03);
    delayMilliseconds(5);
    LCD_Send4Bits(0x03);
    delayMicroseconds(150);
    LCD_Send4Bits(0x03);
    delayMicroseconds(150);
    LCD_Send4Bits(0x02);  // Set to 4-bit mode

    LCD_Command(LCD_FUNCTION_SET);
    LCD_Command(LCD_DISPLAY_OFF);
    LCD_Command(LCD_CLEAR);
    LCD_Command(LCD_ENTRY_MODE);
    LCD_Command(LCD_DISPLAY_ON);	  
}

void LCD_Clear(void) {
    LCD_Command(LCD_CLEAR);
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 0) ? col : (0x40 + col);
    LCD_Command(LCD_SET_CURSOR | addr);
}

void LCD_PutChar(char c) {
    LCD_Data(c);
}

void LCD_PutString(const char* str) {
    while (*str) {
        LCD_PutChar(*str++);
    }
}

void LCD_DisplayString(uint8_t row, uint8_t col, const char* str) {
    LCD_SetCursor(row, col);
    LCD_PutString(str);
}

void LCD_DisplayGPSInfo(const char* landmarkName) {
    LCD_Clear();
    LCD_DisplayString(0, 0, landmarkName);
}
