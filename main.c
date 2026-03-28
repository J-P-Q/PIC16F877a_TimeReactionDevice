/*
 * File:   main.c
 * Author: Johnn
 *
 * Created on March 26, 2026, 8:51 PM
 */


#include <xc.h>

#pragma config FOSC = HS        
#pragma config WDTE = OFF       
#pragma config PWRTE = OFF      
#pragma config BOREN = OFF     
#pragma config LVP = OFF        
#pragma config CPD = OFF       
#pragma config WRT = OFF       
#pragma config CP = OFF         

#define _XTAL_FREQ 4000000  

// Notes delay
#define GAP 20
#define EIGHTH 167-GAP
#define QUARTER 333-GAP
#define HALF 666-GAP

#include <PWM.h>


void playNokia(void);


volatile uint8_t counter = 0x00;

void main(void) {
    
    PWM_init();
    PWM_duty(512);

    while(1){
        playNokia();
        __delay_ms(500);
    }
    
    return;
}


void playNokia(void){
    
    // #1 E5        8th note
    PWM_freq(659);      __delay_ms(EIGHTH);
    PWM_freq(0);    __delay_ms(GAP);

    // #2 D5        8th note
    PWM_freq(587);      __delay_ms(EIGHTH);
    PWM_freq(0);    __delay_ms(GAP);

    // #3 F#4       QUARTER
    PWM_freq(370);      __delay_ms(QUARTER);
    PWM_freq(0);    __delay_ms(GAP);

    // #4 G#4       QUARTER
    PWM_freq(415);      __delay_ms(QUARTER);
    PWM_freq(0);    __delay_ms(GAP);

    // #5 C#5        8th note
    PWM_freq(554);      __delay_ms(EIGHTH);
    PWM_freq(0);    __delay_ms(GAP);

    // #6 B4        8th note
    PWM_freq(494);      __delay_ms(EIGHTH);
    PWM_freq(0);    __delay_ms(GAP);

    // #7 D4       QUARTER
    PWM_freq(294);      __delay_ms(QUARTER);
    PWM_freq(0);    __delay_ms(GAP);

    // #8 E4       QUARTER
    PWM_freq(330);      __delay_ms(QUARTER);
    PWM_freq(0);    __delay_ms(GAP);


    // #9 B4        8th note
    PWM_freq(494);      __delay_ms(EIGHTH);
    PWM_freq(0);    __delay_ms(GAP);

    // #10 A4        8th note
    PWM_freq(440);      __delay_ms(EIGHTH);
    PWM_freq(0);    __delay_ms(GAP);

    // #11 C#4       QUARTER
    PWM_freq(277);      __delay_ms(QUARTER);
    PWM_freq(0);    __delay_ms(GAP);

    // #12 E4       QUARTER
    PWM_freq(330);      __delay_ms(QUARTER);
    PWM_freq(0);    __delay_ms(GAP);


    // #13 A4       HALF
    PWM_freq(440);      __delay_ms(HALF);
    PWM_freq(0);    __delay_ms(GAP);

    return;

}