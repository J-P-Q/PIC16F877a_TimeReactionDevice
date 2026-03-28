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
#define EIGHTH (167 - GAP)
#define QUARTER (333 - GAP)
#define HALF (666 - GAP)

#include <PWM.h>
#include <TMR0.h>


void playNokia(void);
void ExtInt_init(void);


volatile uint16_t counter_ms = 0;
volatile uint8_t buttonPressed = 0;
volatile uint8_t state = 0;

void __interrupt() ISR(void){
    // TMR0IF
    if(INTCON & 0x04){
        INTCON &= ~0x80;

        TMR0_overflowReset();
        counter_ms ++;
        if(counter_ms % 1000 == 0){
            PORTD = ~PORTD;
        }
        

        INTCON &= ~0x04;
        INTCON |= 0x80;
    }

    // INTF
    if(INTCON & 0x02){    
        INTCON &= ~0x80;
        
        buttonPressed = 1;

        INTCON &= ~0x02;
        INTCON |= 0x80;
    }
    return;
}

void main(void) {
    TRISD = 0x00;
    
    
    //PWM_init();
    TMR0_init();
    ExtInt_init();

    while(1){
        

        switch(state){
            case 0:
                // press to start
                break

            case 1:
                // random timer
                // On LED
                while(buttonPressed == 1);      // Semi poll
                INTCON &= ~0x20;                // off timer immediately
                //save time
                state = 2;
                break;

            case 2:
                // display result
                playNokia();
                state = 0;
                break;
            
                
        }
        
        if(buttonPressed){
            __delay_ms(10);
            if(!(PORTB & 0x01)){
                buttonPressed = 0;
            }
        }
    }
    
    return;
}


void playNokia(void){
    
    // #1 E5        8th note
    PWM_freq_AdaptiveDuty(659);      __delay_ms(EIGHTH);
    PWM_freq_AdaptiveDuty(0);    __delay_ms(GAP);

    // #2 D5        8th note
    PWM_freq_AdaptiveDuty(587);      __delay_ms(EIGHTH);
    PWM_freq_AdaptiveDuty(0);    __delay_ms(GAP);

    // #3 F#4       QUARTER
    PWM_freq_AdaptiveDuty(370);      __delay_ms(QUARTER);
    PWM_freq_AdaptiveDuty(0);    __delay_ms(GAP);

    // #4 G#4       QUARTER
    PWM_freq_AdaptiveDuty(415);      __delay_ms(QUARTER);
    PWM_freq_AdaptiveDuty(0);    __delay_ms(GAP);

    // #5 C#5        8th note
    PWM_freq_AdaptiveDuty(554);      __delay_ms(EIGHTH);
    PWM_freq_AdaptiveDuty(0);    __delay_ms(GAP);

    // #6 B4        8th note
    PWM_freq_AdaptiveDuty(494);      __delay_ms(EIGHTH);
    PWM_freq_AdaptiveDuty(0);    __delay_ms(GAP);

    // #7 D4       QUARTER
    PWM_freq_AdaptiveDuty(294);      __delay_ms(QUARTER);
    PWM_freq_AdaptiveDuty(0);    __delay_ms(GAP);

    // #8 E4       QUARTER
    PWM_freq_AdaptiveDuty(330);      __delay_ms(QUARTER);
    PWM_freq_AdaptiveDuty(0);    __delay_ms(GAP);


    // #9 B4        8th note
    PWM_freq_AdaptiveDuty(494);      __delay_ms(EIGHTH);
    PWM_freq_AdaptiveDuty(0);    __delay_ms(GAP);

    // #10 A4        8th note
    PWM_freq_AdaptiveDuty(440);      __delay_ms(EIGHTH);
    PWM_freq_AdaptiveDuty(0);    __delay_ms(GAP);

    // #11 C#4       QUARTER
    PWM_freq_AdaptiveDuty(277);      __delay_ms(QUARTER);
    PWM_freq_AdaptiveDuty(0);    __delay_ms(GAP);

    // #12 E4       QUARTER
    PWM_freq_AdaptiveDuty(330);      __delay_ms(QUARTER);
    PWM_freq_AdaptiveDuty(0);    __delay_ms(GAP);


    // #13 A4       HALF
    PWM_freq_AdaptiveDuty(440);      __delay_ms(HALF);
    PWM_freq_AdaptiveDuty(0);    __delay_ms(GAP);

    return;

}

void ExtInt_init(void){
    TRISB = 0xFF;
    INTCON |= 0x10;

    return;
}

