#include <xc.h>
#include "conbits.h"
#include "header.h"
bool flag_timer_2;
uint8_t max_second_wait_RN4020;
uint32_t timer2_count = 0;

void ISR_timer2(void){
    if (++timer2_count >= max_second_wait_RN4020){
        flag_timer_2 = 1;
        timer2_count = 0;
        stop_reset_timer2();
    }
    else timer2_count++;
    CLRWDT();
}

void start_timer2(uint8_t TIME_SEC){ 
    max_second_wait_RN4020 = TIME_SEC;
    TMR2 = 0x00; // au cas ou
    T2CONbits.TMR2ON = 1; 
}

void stop_reset_timer2(void){
    T2CONbits.TMR2ON = 0;
    TMR2 = 0x00;
    timer2_count = 0;
}

void stop_timer2(void){ T2CONbits.TMR2ON = 0; }
void reset_timer2(void){ TMR2 = 0x00;}
void set_period_timer2(uint8_t periodVal){ PR2 = periodVal; }
void write_timer2(uint8_t timerVal){ TMR2 = timerVal; }

uint8_t read_timer2(void){
    uint8_t readVal;
    readVal = TMR2;
    return readVal;
}

void init_timer2(void){
//// Timer Clock Source Selection Register ////
    T2CLKCON = 0x01; // 0001-> T2CS FOSC/4 : 4MHz
//// Timer Hardware Limit Control Register ////
    T2HLTbits.PSYNC = 0; // Timer Prescaler Output is not synchronized to FOSC/4
    T2HLTbits.CKPOL = 0; // Rising edge of input clock
    T2HLTbits.CKSYNC = 1; // ON bit is synchronized to timer clock input
    T2HLTbits.MODE = 0b00000; // Software control
//// Timer External Reset Signal Selection Register ////
    T2RST = 0x00; // -> Osef, on s'en sert pas
//// Timer Period Register ////
    T2PR = 0xF0; // -> Configuré pour faire une interuption toutes les 100.256 ms
// reset du buffer TMR2 
    T2TMR = 0x00;
// Clearing flag before enabling the interrupt.
    PIR4bits.TMR2IF = 0;
    PIE4bits.TMR2IE = 1;
//// Timer2 Control Register ////
    T2CON = 0xFC;// Prescaler à 1:128 Postscaler à 1:13
    T2CONbits.ON = 0; // on ne lance pas le timer dès de début
}