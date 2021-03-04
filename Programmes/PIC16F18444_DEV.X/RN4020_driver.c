#include <xc.h>
#include "conbits.h"
#include "header.h"
void reset_all_flags(void);
bool wake_RN4020(void);
bool connect_RN4020_to_ESP32(void);
bool send_batt_lvl_to_RN4020(void);
bool read_bbot_desired_state(void);
bool disconnect_and_sleep_RN4020(void);
bool send_bbot_state_to_RN4020(void);
#define MAX_TIME_WAIT_FOR_CONN 50 // nombre de secondes d'attente à la connexion a l'ESP32 avant abandon
#define MAX_TIME_WAIT_FOR_DECONN 20 //
#define MAX_TIME_WAIT_FOR_BLE_WRITE 30
#define MAX_TIME_WAIT_FOR_BLE_READ 30
#define MAX_TIME_WAIT_FOR_WAKE 10 // nombre de secondes d'attente au reveil du RN4020 avant abandon
#define MAX_TIME_WAIT_FOR_SLEEP 30 //


#define MAX_TRY_CONN 3 // nombre max de tentative de redémmarage avant abandon et SLEEP()
#define MAX_CONN_ERROR 15 // nombre max de tentatives de connexion au RN4020 avant tentatives de redémmarrage
//uint8_t battery_level;
bool Bbot_state;
int error_occurancy = 0;
bool restart_rn4020 = false;
bool read_ok;
uint8_t MAX_W_TIME = 1;
void Bot_State_handler(void);

////////////////////////////////////// FCT de routines
bool routine_rn4020(void){
    if(wake_RN4020()) return true;
    if(connect_RN4020_to_ESP32()) return true;
    if (flag_send_status_required) {
        if(send_bbot_state_to_RN4020()) return true;
        else flag_send_status_required = false;
    }
    if(send_batt_lvl_to_RN4020()) return true;
    if(read_bbot_desired_state()) return true;
    if(disconnect_and_sleep_RN4020()) return true;
    Bot_State_handler();
    return false;
}

////////////////////////////////////// FCT APPELLEE
void start_RN4020(void){
    CLRWDT();
    SWAKE_BLE = 0;
    SWAKE_BLE = 1; // Wake from sleep
    CMD_BLE = 1; // Uart control mode
    CMD_BLE = 0;
    while(!flag_CMD){} reset_all_flags();
    uart_send_string("SF,1", true);
    while(!flag_AOK){} reset_all_flags();
    uart_send_string("SR,90000000", true);
    while(!flag_AOK){} reset_all_flags();
    uart_send_string("R,1", true);
    while(!flag_CMD){} //reset_all_flags(); // on ne reset pas les flags car on va les reset dans la boucle  
    CLRWDT();
    
}

bool wake_RN4020(void){
    CLRWDT();
    start_timer2(MAX_TIME_WAIT_FOR_WAKE);
    SWAKE_BLE = 1;
    while(!flag_CMD && !flag_timer_2 && !flag_ERR){} 
    if (flag_timer_2 || flag_ERR){
        reset_all_flags();
        return true;
    }
    else if (flag_CMD){
        reset_all_flags();
        return false;
    }
    else { // cas normalement impossible
        reset_all_flags();
        return true;
    }
    return true;  // cas normalement impossible
}

bool connect_RN4020_to_ESP32(void){ 
    CLRWDT();
    error_occurancy = 0;
    while(error_occurancy <= MAX_CONN_ERROR+1){ 
        if (error_occurancy >= MAX_CONN_ERROR) {
            reset_all_flags();
            return true;
        }
        else {
            uart_send_string("E,0,240AC4DAEB6E", true);
            start_timer2(MAX_TIME_WAIT_FOR_CONN);
            while(CONN_BLE == 0 && !flag_Connection_End && !flag_timer_2 && !flag_ERR ){}

            if (CONN_BLE == 1){
                reset_all_flags();
                return false;
            }
            else if (flag_timer_2) {
                reset_all_flags();
                return true;
            }
            else if (flag_Connection_End || flag_ERR){
                reset_all_flags();
                error_occurancy ++;
            }
            else {  // cas normalement impossible
                reset_all_flags();
                return true; 
            }
        }
    }
    return true; // si on sort de la boucle, cas normalement impossible
}

bool send_bbot_state_to_RN4020(void){
    CLRWDT();
    if (Bbot_state == true) uart_send_string("CUWV,2B05,01", true);
    else uart_send_string("CUWV,2B05,00", true);
    start_timer2(MAX_TIME_WAIT_FOR_BLE_WRITE);
    while(!flag_AOK && !flag_timer_2 && !flag_ERR && !flag_Connection_End){}
        if (flag_AOK){
            reset_all_flags();
            return false;
        }
        else if (flag_timer_2 || flag_ERR || flag_Connection_End) {
            reset_all_flags();
            return true;
        }
        else {  // cas normalement impossible
            reset_all_flags();
            return true; 
        }
    
}

bool send_batt_lvl_to_RN4020(void){
    CLRWDT();
    uart_send_string("CUWV,2A19,", false);
    uart_write(Hex_battery_level[0]);
    uart_write(Hex_battery_level[1]);
    uart_write(10);
    uart_write(13);
//    CLRWDT();
    start_timer2(MAX_TIME_WAIT_FOR_BLE_WRITE);
    while(!flag_AOK && !flag_timer_2 && !flag_ERR && !flag_Connection_End){}
        if (flag_AOK){
            reset_all_flags();
            return false;
        }
        else if (flag_timer_2 || flag_ERR || flag_Connection_End) {
            reset_all_flags();
            return true;
        }
        else {  // cas normalement impossible
            reset_all_flags();
            return true; 
        }
}
bool read_bbot_desired_state(){
    CLRWDT();
    uart_send_string("CURV,2B06", true);
    start_timer2(MAX_TIME_WAIT_FOR_BLE_READ);
    while(!flag_desired_Bbot_status && !flag_Connection_End && !flag_timer_2 && !flag_ERR){}
        if (flag_desired_Bbot_status){
            reset_all_flags();
            return false;
        }
        else if (flag_timer_2 || flag_ERR || flag_Connection_End) {
            reset_all_flags();
            return true;
        }
        else {  // cas normalement impossible
            reset_all_flags();
            return true; 
        }
}

bool disconnect_and_sleep_RN4020(void){  //// A FINIR
    CLRWDT();
    if(CONN_BLE){
        uart_send_string("K", true);
        start_timer2(MAX_TIME_WAIT_FOR_DECONN);
        while(!flag_Connection_End && !flag_timer_2 && !flag_ERR){}
            if (flag_Connection_End){
                reset_all_flags();
            }
            else if (flag_timer_2 || flag_ERR) {
                reset_all_flags();
                return true;
            }
            else {  // cas normalement impossible
                reset_all_flags();
                return true; 
            }
    }    
    SWAKE_BLE = 0;
    start_timer2(MAX_TIME_WAIT_FOR_SLEEP);
    while(!flag_END && !flag_timer_2 && !flag_ERR){} 
    if (flag_timer_2 || flag_ERR){
        reset_all_flags();
        return true;
    }
    else if (flag_END){
        reset_all_flags();
        return false;
    }
    else { // cas normalement impossible
        reset_all_flags();
        return true;
    }
}

void Bot_State_handler(void){
    if (Bbot_state){
        if (desired_Bbot_status == 0)flag_send_status_required = true;
        else if (desired_Bbot_status == 2){
            flag_send_status_required = true;
            Bbot_state = false;
        }
        else if (desired_Bbot_status == 3)flag_send_status_required = true;
    }
    else {
        if (desired_Bbot_status == 1)flag_send_status_required = true;
        else if (desired_Bbot_status == 3){
            flag_send_status_required = true;
            Bbot_state = true;
        }
        else if (desired_Bbot_status == 2)flag_send_status_required = true;
    }
    VOUT = Bbot_state;
}


void reset_RN4020(void){ 
    CLRWDT();
    SWAKE_BLE = 0;
    SWAKE_BLE = 1; // Wake from sleep
    CMD_BLE = 1; // Uart control mode
    CMD_BLE = 0;
    while(!flag_CMD){} reset_all_flags();
    CLRWDT();
    uart_send_string("SF,1", true);
    while(!flag_AOK){} reset_all_flags();
    CLRWDT();
    uart_send_string("SR,90000000", true);
    while(!flag_AOK){} reset_all_flags();
    CLRWDT();
    uart_send_string("R,1", true);
    while(!flag_CMD){} reset_all_flags();
    CLRWDT();
    SWAKE_BLE = 0;
    while (!flag_END) {}
}









 

void reset_all_flags(){
    flag_AOK = false;
    flag_CMD = false;
    flag_ERR = false;
    flag_END = false;
    flag_Reboot = false;
    flag_Connected = false;
    flag_Connection_End = false;
    flag_desired_Bbot_status = false;
    flag_timer_2 = false;
    stop_reset_timer2();
        CLRWDT();

}