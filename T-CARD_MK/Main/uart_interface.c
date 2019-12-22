#include "uart_interface.h"

bit TI1;
bit RI1;
uint8_t Byte;

void sendToInterface(void)
{
#ifndef QT
    SBUF1 = 0x06;
#endif
}
#ifndef QT
void interfaceUARTTimer(void) interrupt INTERRUPT_TIMER3
{

    TMR3CN &= 0x7B;

    sizeParsingDataIface = 	readByteIface;
    while(readByteIface){
        readByteIface--;
        ParsingBufferIFace[readByteIface] = UART_BufferIface[readByteIface];
        readyUartRX_Iface = 1;
    }
    readByteIface = 0;
}

void interfaceUART(void) interrupt INTERRUPT_UART1
{

    PCA0MD &= ~0x40;

    TI1 = SCON1 & 2u;;	//  UART TX

    if (TI1 == 1)       // Check if transmit flag is set
    {

    }

    RI1 = SCON1 & 1u;	//  UART RX
    if (RI1 == 1)
    {
        if(readByteIface == 0){

            TMR3H = 0;
            TMR3L = 0;
            TMR3CN |= 0x04; // aee. oaeia?a

        }
        SCON1 &= 0xfe;                     		// Clear interrupt flag
        Byte = SBUF1;                      		// Read a character from UART
        UART_BufferIface[readByteIface] = Byte;      	// Store in array
        readByteIface++;

        TMR3H = 0;
        TMR3L = 0;

    }
}
#endif
