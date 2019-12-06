#ifndef _UART_H      
#define _UART_H

#include "Config.h"
//==================== uart ========================

#define UART_BUFFERSIZE 32

unsigned char UART_Buffer0[UART_BUFFERSIZE];

unsigned char UART_Buffer1[UART_BUFFERSIZE];

//Флаг приняты данные по UART
extern bit b_uart_rx0 = 0;
extern bit b_uart_rx1 = 0;

static char Byte;
static int readByte = 0;
static int sizeData = 0;

bit TI1;
bit RI1;

#endif