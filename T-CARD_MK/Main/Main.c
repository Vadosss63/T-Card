#include "config.h"
#include "CMD_data.h"  


void sendToCardReader(uint8_t* d, uint8_t size);

///
// data -- по умолчанию (128 байт + (idata 128 стек))
// xdata -- запись в 4kb

///==================UART================================///
#define UART_BUFFERSIZE 32

uint8_t xdata UART_BufferCR[UART_BUFFERSIZE];
uint8_t xdata ParsingBufferCR[24];

uint8_t xdata UART_BufferIface[UART_BUFFERSIZE];
uint8_t xdata ParsingBufferIFace[24];

uint8_t Byte;

// Флаг приняты данные по UART
bit readyUartRX_CR = 0;
bit readyUartRX_Iface = 0;

uint8_t readByteCR = 0;
uint8_t readByteIface = 0;

uint8_t sizeParsingDataIface = 0;
uint8_t sizeParsingDataCR = 0;

uint8_t sizeSend = 0;
uint8_t sendCR = 0;// Отправлено в картридер

bit test = 0;

bit TI1;
bit RI1;
///==================END================================///


void parsingDataCR(){
	readyUartRX_CR = 0;
}

void parsingDataIFace(){
	readyUartRX_Iface = 0;

	if(ParsingBufferIFace[0]== 0xf2)
	{
		if(test){
			LockCardRider lcr;
			lcr = getLock();
			sendToCardReader((uint8_t*)(&lcr), 8);
			test = 0;
		}else{
			UnlockCardRider ulcr;
			ulcr = getUnlock();
			sendToCardReader((uint8_t*)(&ulcr), 8);
			test = 1;
		}
	}
}


void main(void)
{
	
	PCA0MD &= ~0x40; // Disable the watchdog timer 
	Init_Device();
	TMR3CN &= 0x04; // вкл. таймера
	while(1) {
		PCA0MD = 0x00;
		if(readyUartRX_Iface)
			parsingDataIFace();			
								
	}
		
}



///==================UART================================///

void sendToInterface(void)  
{
  	SBUF1 = 0x06;
}

void sendToCardReader(uint8_t* d, uint8_t size)  
{
	sizeSend = size;
	while(size)	{
		size--;
  		UART_BufferCR[size] = d[size];
	}
	sendCR = 0;	
	TI0 = 1;
}


void cardReaderUARTTimer(void) interrupt INTERRUPT_TIMER2
{
	TF2H = 0;			// Остановка прирывания
	TR2 = 0; 	// выключение таймера
	if(readByteCR == 1)	{
		isValidAnswer(UART_BufferCR[0]); // проверка ответа			
	}else{
		if(isValidData(&UART_BufferCR, readByteCR))
		{
			sizeParsingDataCR = readByteCR;			
			while(readByteCR){ //копируем данные в буфер парсинга 
				readByteCR--;
				ParsingBufferCR[readByteCR] = UART_BufferCR[readByteCR];
			}			
			readyUartRX_CR = 1;
		}		
	}
	readByteCR = 0; // обнуление количества принятых байт
}

void cardReaderUART(void) interrupt INTERRUPT_UART0
{
   PCA0MD &= ~0x40;
   if(TI0 == 1){                  // Check if transmit flag is set
   		TI0 = 0;
		if(sendCR < sizeSend) { //вычитываем весь буффур
			SBUF0 = UART_BufferCR[sendCR];
			sendCR++;
		}
   }

   if(RI0 == 1){
   		if(readByteCR == 0){
			TR2 |= 0x04;// вкл. таймера
			TMR2H = 0;
			TMR2L = 0;			
		}		
      	                           		// Clear interrupt flag
      	Byte = SBUF0;                      		// Read a character from UART	
	  	UART_BufferCR[readByteCR] = Byte;      	// Store in array
		readByteCR++;
		TMR2H = 0;
		TMR2L = 0;
		RI0 = 0;	
   }
}

void interfaceUARTTimer(void) interrupt INTERRUPT_TIMER3
{
	// TMR3CN &= // Остановка прирывания
	TMR3CN &= 0x7B; // выкл. таймера
	sizeParsingDataIface = 	readByteIface;
	while(readByteIface){ //копируем данные в буфер парсинга 
		readByteIface--;
		ParsingBufferIFace[readByteIface] = UART_BufferIface[readByteIface];
		readyUartRX_Iface = 1;
	}
	readByteIface = 0; // обнуление количества принятых байт
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
			TMR3CN |= 0x04; // вкл. таймера
			
		}			
	  	SCON1 &= 0xfe;                     		// Clear interrupt flag
      	Byte = SBUF1;                      		// Read a character from UART	
	  	UART_BufferIface[readByteIface] = Byte;      	// Store in array
		readByteIface++;
		
		TMR3H = 0;
		TMR3L = 0;
    	      
   	}
}

