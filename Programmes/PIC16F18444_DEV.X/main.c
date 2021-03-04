#include <xc.h>
#include "conbits.h"
#include "header.h"
#include "string.h"
#define max_routine_error 10
void buzzzz(void);
void buzz_buzz(void);
void main(void) {
    init();
    CONN_ADC = 1;
    VOUT = 0;
    uint8_t routine_error_cpt = 0;
    buzz_buzz();
    start_RN4020();
    while(1){
        CLRWDT();
        get_battery_lvl();
        if(routine_rn4020()) {
            disconnect_and_sleep_RN4020();
            if (routine_error_cpt >= max_routine_error){
                reset_RN4020();
                disconnect_and_sleep_RN4020();
                routine_error_cpt = 0;
            }
        }
        SLEEP();
    }
    return;
}   

void buzz_buzz(void){
    CLRWDT();
    BUZZER = 1;
    __delay_ms(100);
    BUZZER = 0;
    __delay_ms(50);
    BUZZER = 1;
    __delay_ms(100);
    BUZZER = 0;

}

void buzzzz(void){
    CLRWDT();
    BUZZER = 1;
    __delay_ms(100);
    BUZZER = 0;
    __delay_ms(50);
    BUZZER = 1;
    __delay_ms(100);
    BUZZER = 0;
    __delay_ms(50);
    BUZZER = 1;
    __delay_ms(100);
    BUZZER = 0;
}


