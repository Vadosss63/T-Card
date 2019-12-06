#include "Uart.h"

//void sendAnswer(void)  
//{
//  SBUF1 = 0x06;
//}

//кр
void UART0_Interrupt (void) interrupt 4
{
   PCA0MD &= ~0x40;
   if (TI0 == 1)                   // Check if transmit flag is set
   {
      	TI0 = 0; 
   }

   if (RI0 == 1)
   {  

      	RI0 = 0;                           // Clear interrupt flag
      	Byte = SBUF0;                      // Read a character from UART	
	  	UART_Buffer0[readByte] = Byte;      // Store in array
		readByte = 0;	      

   }
}

void UART0_TimerInterrupt (void) interrupt 5
{
	TR2 = 0; // выключение таймера
}

void UART1_TimerInterrupt (void) interrupt 14
{
	TMR3CN &= 0xFB; // выключение таймера
}

//ИНТЕРФЕЙС
void UART1_Interrupt (void) interrupt 16
{
    PCA0MD &= ~0x40;

	TI1 = SCON1 & 2u;;	//  UART TX
	RI1 = SCON1 & 1u;	//  UART RX

   	if (TI1 == 1)                   // Check if transmit flag is set
   	{
      SCON1 &= 0xfd; 
   	}

   	if (RI1 == 1)
   	{  

      	SCON1 &= 0xfe;                     // Clear interrupt flag
      	Byte = SBUF1;                      // Read a character from UART	
	  	UART_Buffer1[readByte] = Byte;      // Store in array
    	readByte = 0;      

   	}
}