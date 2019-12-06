/////////////////////////////////////
//  Generated Initialization File  //
/////////////////////////////////////

#include "compiler_defs.h"
#include "C8051F340_defs.h"

// Peripheral specific initialization functions,
// Called from the Init_Device() function
void Reset_Sources_Init()
{
    int i = 0;
    VDM0CN    = 0x80;
    for (i = 0; i < 350; i++);  // Wait 100us for initialization
    RSTSRC    = 0x06;
}

void PCA_Init()
{
    PCA0MD    &= ~0x40;
    PCA0MD    = 0x00;
    PCA0CPL4  = 0xFF;
    PCA0CPH4  = 0xFF;
}

void Timer_Init()
{
    TCON      = 0x40;
    TMOD      = 0x20;
    CKCON     = 0x08;
    TH1       = 0xB2;
    TMR2RLL   = 0x78;
    TMR2RLH   = 0xEC;
    TMR3RLL   = 0x78;
    TMR3RLH   = 0xEC;
}

void UART_Init()
{
    SCON0     = 0x30;
    SBRLL1    = 0xB2;
    SBRLH1    = 0xFF;
    SCON1     = 0x10;
    SBCON1    = 0x43;
}

void Port_IO_Init()
{
    // P0.0  -  Skipped,     Open-Drain, Digital
    // P0.1  -  Skipped,     Open-Drain, Digital
    // P0.2  -  Skipped,     Open-Drain, Digital
    // P0.3  -  Skipped,     Open-Drain, Digital
    // P0.4  -  TX0 (UART0), Push-Pull,  Digital
    // P0.5  -  RX0 (UART0), Open-Drain, Digital
    // P0.6  -  TX1 (UART1), Push-Pull,  Digital
    // P0.7  -  RX1 (UART1), Open-Drain, Digital

    // P1.0  -  Unassigned,  Open-Drain, Digital
    // P1.1  -  Unassigned,  Open-Drain, Digital
    // P1.2  -  Unassigned,  Open-Drain, Digital
    // P1.3  -  Unassigned,  Open-Drain, Digital
    // P1.4  -  Unassigned,  Open-Drain, Digital
    // P1.5  -  Unassigned,  Open-Drain, Digital
    // P1.6  -  Unassigned,  Open-Drain, Digital
    // P1.7  -  Unassigned,  Open-Drain, Digital

    // P2.0  -  Unassigned,  Open-Drain, Digital
    // P2.1  -  Unassigned,  Open-Drain, Digital
    // P2.2  -  Unassigned,  Open-Drain, Digital
    // P2.3  -  Unassigned,  Open-Drain, Digital
    // P2.4  -  Unassigned,  Open-Drain, Digital
    // P2.5  -  Unassigned,  Open-Drain, Digital
    // P2.6  -  Unassigned,  Open-Drain, Digital
    // P2.7  -  Unassigned,  Open-Drain, Digital

    // P3.0  -  Unassigned,  Open-Drain, Digital
    // P3.1  -  Unassigned,  Open-Drain, Digital
    // P3.2  -  Unassigned,  Open-Drain, Digital
    // P3.3  -  Unassigned,  Open-Drain, Digital
    // P3.4  -  Unassigned,  Open-Drain, Digital
    // P3.5  -  Unassigned,  Open-Drain, Digital
    // P3.6  -  Unassigned,  Open-Drain, Digital
    // P3.7  -  Unassigned,  Open-Drain, Digital

    P0MDOUT   = 0x50;
    P0SKIP    = 0x0F;
    XBR0      = 0x01;
    XBR1      = 0x40;
    XBR2      = 0x01;
}

void Interrupts_Init()
{
    EIE1      = 0x80;
    EIE2      = 0x02;
    IE        = 0xB0;
}

// Initialization function for device,
// Call Init_Device() from your main program
void Init_Device(void)
{
    Reset_Sources_Init();
    PCA_Init();
    Timer_Init();
    UART_Init();
    Port_IO_Init();
    Interrupts_Init();
}
