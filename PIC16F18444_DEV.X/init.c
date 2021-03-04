#include <xc.h>
#include "conbits.h"
#include "header.h"
void init() {
    init_pin();
    PIE0bits.INTE = 1; // external interrupt enable
    INTCONbits.GIE = 1;// global interrupt enable
    INTCONbits.PEIE = 1;// peripheral interrupt enable
    INTCONbits.INTEDG = 1;
    OSCILLATOR_Initialize();
    init_uart();
    init_extern_interrupt();
    init_adc();
    init_timer2();
    TMR0_Initialize();
}

void init_extern_interrupt(void)
{
    PIR0bits.INTF = 0; // Clear the interrupt flag
    INTCONbits.INTEDG = 1; // Set rising edge detect
    PIE0bits.INTE = 1; // Set external interrupt enable
}

void init_pin(void){
    IO_LED = 0;
    IO_VOUT = 0;
    IO_CONN_ADC = 0;
    IO_ADC_C3 = 1;
    IO_SWAKE_BLE = 0;
    SWAKE_BLE = 0;
    IO_CONN_BLE = 1;
    IO_EXT_INT = 1;
    IO_EXT_INT_BIS = 1;
    IO_BUZZER = 0;
    // PPS TX et RX
    RC4PPS = 0x0F;   //RC4->EUSART1:TX1;    
    RX1DTPPS = 0x15;   //RC5->EUSART1:RX1;    
    INTPPS = 0x0F;   //RB7->EXT_INT:INT;  
    WPUCbits.WPUC5 = 1; // weak pull up
    WPUCbits.WPUC4 = 1;
    // ADC 
    ANSELA = 0x00;
    ANSELC = 0x08; // -> 0000 1000  RC3 analog input
    ANSELB = 0x00;
}
void OSCILLATOR_Initialize(void)
{
    // NOSC HFINTOSC; NDIV 1; 
    OSCCON1 = 0x60;
    // CSWHOLD may proceed; SOSCPWR Low power; 
    OSCCON3 = 0x00;
    // MFOEN disabled; LFOEN disabled; ADOEN disabled; SOSCEN disabled; EXTOEN disabled; HFOEN disabled; 
    OSCEN = 0x00;
    // HFFRQ 16_MHz; 
    OSCFRQ = 0x05;
    // HFTUN 0; 
    OSCTUNE = 0x00;
}

void TMR0_Initialize(void)
{
    // Set TMR0 to the options selected in the User Interface

    // T0CS LFINTOSC; T0CKPS 1:128; T0ASYNC not_synchronised; 
    T0CON1 = 0x97;

    // TMR0H 255; 
    TMR0H = 0xFF;

    // TMR0L 0; 
    TMR0L = 0x00;

    // Load TMR0 value to the 16-bit reload variable

    // Clear Interrupt flag before enabling the interrupt
    PIR0bits.TMR0IF = 0;

    // Enabling TMR0 interrupt.
    PIE0bits.TMR0IE = 1;

    // Set Default Interrupt Handler

    // T0OUTPS 1:8; T0EN enabled; T016BIT 16-bit; 
    T0CON0 = 0x97;
}