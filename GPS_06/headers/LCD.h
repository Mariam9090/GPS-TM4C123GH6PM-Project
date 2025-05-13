#ifndef LCD
#define LCD

#include <stdint.h>

#define LCD_DATA_DIR   GPIO_PORTB_DIR_R
#define LCD_DATA_DEN   GPIO_PORTB_DEN_R
#define LCD_DATA_PORT  GPIO_PORTB_DATA_R

#define LCD_CTRL_DIR   GPIO_PORTD_DIR_R
#define LCD_CTRL_DEN   GPIO_PORTD_DEN_R
#define LCD_CTRL_PORT  GPIO_PORTD_DATA_R

#define LCD_RS         0x01    // PD0
#define LCD_EN         0x02    // PD1

// LCD commands
#define LCD_CLEAR           0x01
#define LCD_HOME            0x02
#define LCD_ENTRY_MODE      0x06
#define LCD_DISPLAY_OFF     0x08
#define LCD_DISPLAY_ON      0x0C
#define LCD_FUNCTION_SET    0x28    // 4-bit, 2 lines, 5x8 dots
#define LCD_SET_CURSOR      0x80
#define LCD_BACKLIGHT_ON    0x08
#define LCD_BACKLIGHT_OFF   0x00

// LCD size
#define LCD_ROWS            2
#define LCD_COLS            16

void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_PutChar(char c);
void LCD_PutString(const char* str);
void LCD_DisplayString(uint8_t row, uint8_t col, const char* str);
void LCD_DisplayGPSInfo(const char* landmarkName);
void delayMilliseconds(uint32_t ms);
void LCD_Command(uint8_t cmd);
#endif
