#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define IO_VOUT TRISCbits.TRISC7
#define IO_CONN_ADC TRISCbits.TRISC6
#define IO_LED TRISAbits.TRISA5
#define IO_SWAKE_BLE TRISAbits.TRISA2
#define IO_CMD_BLE TRISCbits.TRISC0
#define IO_CONN_BLE TRISCbits.TRISC1
#define IO_ADC_C3 TRISCbits.TRISC3
#define IO_EXT_INT TRISBbits.TRISB7
#define IO_EXT_INT_BIS TRISBbits.TRISB6
#define EXT_INT_BIS PORTBbits.RB6

#define IO_BUZZER TRISCbits.TRISC2
#define BUZZER LATCbits.LATC2

#define LED LATAbits.LATA5
#define VOUT LATCbits.LATC7
#define CONN_ADC LATCbits.LATC6
//
#define SWAKE_BLE LATAbits.LATA2
#define CMD_BLE LATCbits.LATC0
#define CONN_BLE PORTCbits.RC1

///////////////////////////////////////////////////////////// CONSTANTES


///////////////////////////////////////////////////////////// BIBLI UTILE
char* reverse(char *buffer, int i, int j);
char* itoa(int value, char* buffer, int form);
inline void swap(char *x, char *y);
int Hex_to_BCD(int hex_val);
uint8_t BCD_to_Hex(uint8_t bcd_val);
bool detect_word_RX(char word[], uint8_t word_length, bool delete);

///////////////////////////////////////////////////////////// EXTERN VARIABLE
//////////////////////////////////// BOT
extern bool Bbot_state;
extern uint8_t battery_level;
extern char Hex_battery_level[2];
extern uint8_t desired_Bbot_status;

//////////////////////////////////// BOT FLAGS
extern bool flag_send_status_required;

//////////////////////////////////// UART FLAGS
extern bool flag_AOK;
extern bool flag_ERR;
extern bool flag_CMD;
extern bool flag_Reboot;
extern bool flag_Connected;
extern bool flag_Connection_End;
extern bool flag_END;
extern bool flag_desired_Bbot_status;
extern bool crepe;



/////////////////////////////////// TIMER_FLAGS
extern bool flag_timer_2;
extern bool flag_timer_0;
extern uint8_t max_second_wait_RN4020;
///////////////////////////////////////////////////////////// INIT
void init(void);
void OSCILLATOR_Initialize(void);
void init_pin(void);
void init_extern_interrupt(void);

///////////////////////////////////////////////////////////// ADC
void init_adc(void);
uint16_t adc_conversion(void);
void get_battery_lvl(void);

///////////////////////////////////////////////////////////// UART
void init_uart(void);
void uart_write(uint8_t txData);
void uart_write_buffer(char TX_buff[], bool CRLF, uint8_t char_length);
void uart_send_string(const char* command, bool CRLF);
uint8_t uart_read(void);

///////////////////////////////////////////////////////////// RN4020 DRIVER
bool routine_rn4020(void);
void start_RN4020(void);
void reset_RN4020(void);
bool disconnect_and_sleep_RN4020(void);



///////////////////////////////////////////////////////////// TIMER 2
void init_timer2(void);
void start_timer2(uint8_t TIME_SEC);
void stop_timer2(void);
void reset_timer2(void);
void stop_reset_timer2(void);
uint8_t read_timer2(void);
void write_timer2(uint8_t timerVal);
void set_period_timer2(uint8_t periodVal);
void ISR_timer2(void);

///////////////////////////////////////////////////////////// TIMER 0
void TMR0_Initialize(void);



#endif	

