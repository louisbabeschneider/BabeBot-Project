#include <xc.h>
#include "conbits.h"
#include "header.h"

void uart_write(uint8_t txData)
{
    while(0 == PIR3bits.TX1IF)
    {
    }
    TX1REG = txData;    // Write the data byte to the USART.
}

void uart_write_buffer(char TX_buff[], bool CRLF, uint8_t char_length){
    uint8_t j;
    if (CRLF == true) {
        for (j = 0 ; j < char_length ; j++)
        {
            while(0 == PIR3bits.TX1IF || TX1STAbits.TRMT == 0){}
            TX1REG = TX_buff[j];    // Write the data byte to the USART.
        }
        while(0 == PIR3bits.TX1IF || TX1STAbits.TRMT == 0){}
        TX1REG = 13;   
        while(0 == PIR3bits.TX1IF || TX1STAbits.TRMT == 0){}
        TX1REG = 10;   
    }
    else {
        for (j = 0 ; j < char_length ; j++)
        {
            while(0 == PIR3bits.TX1IF || TX1STAbits.TRMT == 0){}
            TX1REG = TX_buff[j];    // Write the data byte to the USART.
        }
    }
}

void uart_send_string(const char* command, bool CRLF){
    uint8_t j=0;
    uint8_t length = strlen(command);

    if (CRLF == true) {
        while(length--){
            while(0 == PIR3bits.TX1IF || TX1STAbits.TRMT == 0){}
            TX1REG = *command++;   
        }
        while(0 == PIR3bits.TX1IF || TX1STAbits.TRMT == 0){}
        TX1REG = 13;   
        while(0 == PIR3bits.TX1IF || TX1STAbits.TRMT == 0){}
        TX1REG = 10;    
    }
    else {
        while(length--){
            while(0 == PIR3bits.TX1IF || TX1STAbits.TRMT == 0){}
            TX1REG = *command++;    
        }
    }
}


uint8_t uart_read(void)
{
    while(!PIR3bits.RC1IF) {}
    
    if(1 == RC1STAbits.OERR) // if Overrun Error bit --> reset receiver
    {
        RC1STAbits.CREN = 0; // Disables receiver
        RC1STAbits.CREN = 1; // Enables receiver
    }

    return RC1REG;
}


void init_uart(void)
{
    // disable interrupts before changing states
    PIE3bits.RC1IE = 0;
    PIE3bits.TX1IE = 0;
    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RC1STA = 0x90;
    //UART config
    TX1STAbits.CSRC = 0; // mode asychrone -> don't care
    TX1STAbits.TX9 = 0; // Selects 8-bit transmission
    TX1STAbits.TXEN = 1; // Transmit enabled
    TX1STAbits.SYNC = 0; // Asynchronous mode
    TX1STAbits.SENDB = 0; // Sync Break transmission disabled or completed 
    TX1STAbits.BRGH = 1; // High speed, if BRG16 = 1, baud rate is baudclk/4; else baudclk/16
    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUD1CON = 0x08;
    // SP1BRGL 34; 
    SP1BRGL = 0x22;
    // SP1BRGH 0; 
    SP1BRGH = 0x00;
    // enable receive interrupt
    PIE3bits.RC1IE = 1;
    
}

