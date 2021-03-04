#include <xc.h>
#include "conbits.h"
#include "header.h"

#define buff_RX_size 20
char buff_RX[buff_RX_size];
uint8_t index_RX = 0;

bool flag_AOK;
bool flag_ERR;
bool flag_CMD;
bool flag_Reboot;
bool flag_Connected;
bool flag_Connection_End;
bool flag_send_status_required;
bool flag_END;
bool flag_desired_Bbot_status;
uint8_t desired_Bbot_status;
bool crepe;



void __interrupt() INTERRUPT_InterruptManager (void){
    if(PIE0bits.TMR0IE == 1 && PIR0bits.TMR0IF == 1)
    {
        PIR0bits.TMR0IF = 0;
        //BUZZER = 1;
    }

    if(PIE0bits.INTE == 1 && PIR0bits.INTF == 1)
    {
        PIR0bits.INTF = 0;
        if(EXT_INT_BIS){
//              reset_RN4020();
            Bbot_state = false;
//            flag_send_status_required = true;
//            CONN_ADC = 0;
//            crepe = true;
            
            
        }
        else {
            Bbot_state = true;
//            flag_send_status_required = true;
//            CONN_ADC = 1;
//            start_timer2(2);
//            crepe = true;
        }
    }
    if(INTCONbits.PEIE == 1) // Si Peripheral Interrupt Enable
    {
        if(PIE3bits.RC1IE == 1 && PIR3bits.RC1IF == 1)
        {
        buff_RX[index_RX] = uart_read();
        if (buff_RX[index_RX] == 10 || buff_RX[index_RX] ==58){
            if (strncmp(buff_RX, "AOK", index_RX-1) == 0  && index_RX > 2) flag_AOK = true;
            if (strncmp(buff_RX, "CMD", index_RX-1) == 0  && index_RX > 2) flag_CMD = true;
            if (strncmp(buff_RX, "ConnParam", index_RX-1) == 0  && index_RX >  6 ) flag_Connected = true;
            if (strncmp(buff_RX, "Reboot", index_RX-1) == 0  && index_RX > 2) flag_Reboot = true;
            if (strncmp(buff_RX, "Connection End", index_RX-1 ) == 0 && index_RX > 6 ) {flag_Connection_End = true;}
            if (strncmp(buff_RX, "ERR", index_RX-1) == 0 && index_RX > 2) flag_ERR = true;
            if (strncmp(buff_RX, "END", index_RX-1) == 0 && index_RX > 2) flag_END = true;
            if (strncmp(buff_RX, "R,0", index_RX-3) == 0 && index_RX > 2){
                if (buff_RX[index_RX-3] == 48){
                    flag_desired_Bbot_status = true;
                    desired_Bbot_status = 0;
                }
                if (buff_RX[index_RX-3] == 49){
                    flag_desired_Bbot_status = true;
                    desired_Bbot_status = 1;
                }
                if (buff_RX[index_RX-3] == 50){
                    flag_desired_Bbot_status = true;
                    desired_Bbot_status = 2;
                }
                if (buff_RX[index_RX-3] == 51){
                    flag_desired_Bbot_status = true;
                    desired_Bbot_status = 3;
                }

                else { /* Traiter les erreurs .. */ }
            }
            index_RX=0;
        }
        else if (index_RX >= buff_RX_size -1 )index_RX=0;
        else index_RX++;
        }
    }
    if(PIE4bits.TMR2IE == 1 && PIR4bits.TMR2IF == 1)
    {
        PIR4bits.TMR2IF = 0;
        ISR_timer2();
    } 
}

