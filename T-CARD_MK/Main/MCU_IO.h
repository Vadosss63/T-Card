#ifndef _MCU_IO_H      
#define _MCU_IO_H

//==================== t ========================


#define UART_BUFFERSIZE 64
unsigned char UART_Buffer[UART_BUFFERSIZE];

//Флаг приняты данные по UART
extern bit b_uart_rx = 0;

#define UART_Header 0
#define UART_CMD 2
#define UART_Size 3
#define UART_Data 5

//определение команд
#define CMD_CONTROL 0X05


#endif