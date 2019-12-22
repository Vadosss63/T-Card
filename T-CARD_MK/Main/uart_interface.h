#ifndef UART_INTERFACE_H
#define UART_INTERFACE_H
#include "InitType.h"
#include "config.h"

#define UART_BUFFER_SIZE 32
uint8_t xdata UART_BufferIface[UART_BUFFER_SIZE];
uint8_t xdata ParsingBufferIFace[24];
bit readyUartRX_Iface = 0;
uint8_t readByteIface = 0;
uint8_t sizeParsingDataIface = 0;

extern void sendToInterface(void);

#endif // UART_INTERFACE_H
