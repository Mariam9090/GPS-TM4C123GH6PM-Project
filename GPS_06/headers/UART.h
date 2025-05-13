#include <stdbool.h>

#ifndef UART
#define UART

void GPS_Init(void);
char UART_GetChar(void);

void UART_PrintFloat(float value);
void print5_str(char *str);
void UART5_PutChar(char data);
void UART5_Init(void);
#endif