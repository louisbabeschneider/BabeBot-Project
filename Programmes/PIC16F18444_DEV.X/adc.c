#include "header.h"
uint8_t battery_level;
char Hex_battery_level[2];
uint16_t adc_tmp;
float volt;
float tension_alim = 3.295;
float x = 0.24139194139194139194139194139194 ;


void init_unused_register(void);
void init_adc(void){
    init_unused_register();
    ADCON1 = 0x00;// ADDSEN disabled; ADGPOL digital_low; ADIPEN disabled; ADPPOL Vss; 
    ADCON2 = 0x00; // ADCRS 0; ADMD Basic_mode; ADACLR disabled; ADPSIS RES; 
    ADCON3 = 0x00; // ADTMD disabled;
    ADSTAT = 0x00; // ADMATH registers not updated; 
    ADREF = 0x00;// ADNREF VSS; ADPREF VDD;
    ADACT = 0x00;// ADACT disabled; 
    ADCLK = 0x00;// ADCS FOSC/2; 
    ADCON0 = 0x84;// ADGO stop; ADFM right; ADON enabled; ADCS FOSC/ADCLK; ADCONT disabled; 
}
void get_battery_lvl(void){ 
        CLRWDT(); 

    adc_tmp = adc_conversion();
    CLRWDT(); 
    battery_level = (uint8_t)((float)adc_tmp*x) - 740;
    if (battery_level < 10) { 
        itoa(battery_level, Hex_battery_level[1], 16);
        Hex_battery_level[0] = 48;
    }
    else itoa(battery_level, Hex_battery_level, 16);
}

uint16_t adc_conversion(void){
    // select the A/D channel
    ADPCH = 0b010011; // -> RC3/ANC3

    // Turn on the ADC module
    ADCON0bits.ADON = 1;
	
    //Disable the continuous mode.
    ADCON0bits.ADCONT = 0;    

    // Start the conversion
    ADCON0bits.ADGO = 1;


    // Wait for the conversion to finish
    while (ADCON0bits.ADGO)
    {
    }
    
    
    // Conversion finished, return the result
    return ((uint16_t)((ADRESH << 8) + ADRESL));
}

void init_unused_register(void){
    ADLTHL = 0x00; //ADLTH and ADUTH are compared with ADERR to set the UTHR and LTHR bits. Depending on the setting of MD, 
    ADLTHH = 0x00; //an interrupt may be triggered by the results of this comparison. --> osef, MD disable
    ADUTHL = 0x00; // same as ADLTHx, upper Threshold, --> osef, MD disable
    ADUTHH = 0x00;
    ADSTPTL = 0x00; // Depending on the CALC bits, it may be used to determine ADERR.
    ADSTPTH = 0x00;// osef
    ADACCU = 0x00; // accumulator mode, osef
    ADRPT = 0x00;// Determines the number of times that the ADC is triggered for a threshold check -> osef
    ADACQL = 0x00; // ADC Acquisition Time Control Register,  
    ADACQH = 0x00;// If ADACQ = 0, software must wait the required acquisition time 
    ADCAP = 0x00; // ADC Additional Sample Capacitor Selection Register -> 0
    ADPREL = 0x00; // ADC Precharge Time Control Register --> osef
    ADPREH = 0x00;
}