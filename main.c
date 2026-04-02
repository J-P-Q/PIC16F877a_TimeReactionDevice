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

#define debounce 10

#include <PWM.h>
#include <TMR0.h>


void playNokia(void);
void playLose(void);
void ExtInt_init(void);
void ExtInt_disable(void);
void generateRandom(void);
void displayTime(uint16_t time);
void dislpayCountDown(void);
void displayTooSlow(void);
void displayPlay(void);

/* 
7 Segment
bits:
0 = a
1 = f
2 = b
3 = g
4 = c
5 = d
6 = e
*/
const uint8_t sevSeg[16] ={
    ~0b11110111,     // 0
    ~0b00010100,     // 1
    ~0b01101101,     // 2
    ~0b00111101,     // 3
    ~0b00011110,     // 4
    ~0b00111011,     // 5
    ~0b01111011,     // 6
    ~0b00010101,     // 7
    ~0b01111111,     // 8
    ~0b00111111,     // 9

    ~0b01001111,     // P
    ~0b01100010,     // L
    ~0b01011111,     // A
    ~0b00111110,     // y

    ~0b01110011,     // G
    ~0b01111000     // o
};

volatile uint16_t counter_ms = 0;
volatile uint8_t buttonPressed = 0;
volatile uint8_t state = 0;

volatile uint16_t seed = 12345;  // for random number
volatile uint16_t randomNum = 0;   // for random number

volatile uint16_t timeLedOn = 0x00;
volatile uint16_t timeLedOff = 0x00;
volatile uint16_t tooSlow = 0x00;

void __interrupt() ISR(void){
    // INTF
    if(INTCON & 0x02){    
        buttonPressed = 1;
        //PORTE = 0xFF;     // On LED
        INTCON &= ~0x02;
        return;
    }

    // TMR0IF
    if(INTCON & 0x04){
        INTCON &= ~0x80;

        TMR0_overflowReset();
        counter_ms ++;
        if(counter_ms % 1000 == 0){     // Test, remove this later on
            //PORTE = ~PORTE;
        }
        
        INTCON &= ~0x04;
        INTCON |= 0x80;
        return;
    }

    return;
}

void main(void) {
    ADCON1 = 0x06;
    TRISA = 0x00;
    PORTA = 0x00;

    TRISD = 0x00;
    PORTD = 0x00;

    TRISE = 0x00;
    PORTE = 0x00;
    
    
    PWM_init();
    TMR0_init();
    ExtInt_init();

    while(1){
        

        switch(state){
            case 0:
                displayPlay();
                // Button
                if(buttonPressed){              // DO NOT TOUCH, WORKING BUTTON HERE
                    //__delay_ms(debounce);

                    if(!(PORTB & 0x01)){
                        
                        generateRandom();
                        randomNum = seed & 0x0FFF;
                        state = 1;
                        while(!(PORTB & 0x01)); 
                        ExtInt_disable();
                        
                    }
                    buttonPressed = 0;   
                }

                break;

            case 1:
                PORTE = 0x00;
                dislpayCountDown();

                // Random Time before turning on LED
                uint16_t nowTime;
                uint16_t prevTime;                

                counter_ms = 0x00;     
                prevTime = counter_ms;
                nowTime = counter_ms;
                while(nowTime - prevTime < randomNum){
                    nowTime = counter_ms;
                }
               
                // On LED
                counter_ms = 0x00;     
                timeLedOn = counter_ms;
                PORTE = 0xFF;
                ExtInt_init();

                while(!buttonPressed && counter_ms < 5000);

                if(buttonPressed){              // DO NOT TOUCH, WORKING BUTTON HERE
                    //__delay_ms(debounce);

                    if(!(PORTB & 0x01)){
                        timeLedOff = 0x00;
                        timeLedOff = counter_ms;
                        
                        

                        PORTE = 0x00;
                        tooSlow = 0x00;                        
                        playNokia();
                        while(!(PORTB & 0x01)); 
                        
                    }
                    buttonPressed = 0;   
                }
                else{   // Missed the button press
                    
                    PORTE = 0x00;
                    tooSlow = 0x01;
                    playLose();
                }
                ExtInt_init();
                state = 2;
                break;

            case 2:
                
                if(tooSlow){
                    displayTooSlow();
                }
                else if(!tooSlow){
                    displayTime(timeLedOff - timeLedOn);
                }
                
                  
                if(buttonPressed){              // DO NOT TOUCH, WORKING BUTTON HERE
                    //__delay_ms(debounce);

                    if(!(PORTB & 0x01)){
                        
                        state = 0;
                        PORTA = 0x00;
                        while(!(PORTB & 0x01)); 
                        ExtInt_init();
                    }
                    buttonPressed = 0;   
                }
                 
                break;

            case 5:     // Testing Case 
                displayPlay();
                
                break;
            
                
        }
        

    }
    
    return;
}


void playNokia(void){
    TMR0_disable();
    
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

    PWM_init();

    TMR0_init();
    return;
}

void playLose(void){
    uint16_t f;
    TMR0_disable();
    
    PWM_freq_AdaptiveDuty(523); __delay_ms(80);   // C5
    PWM_freq_AdaptiveDuty(0);   __delay_ms(20);
    PWM_freq_AdaptiveDuty(494); __delay_ms(80);   // B4
    PWM_freq_AdaptiveDuty(0);   __delay_ms(20);
    PWM_freq_AdaptiveDuty(466); __delay_ms(80);   // Bb4
    PWM_freq_AdaptiveDuty(0);   __delay_ms(20);
    PWM_freq_AdaptiveDuty(440); __delay_ms(80);   // A4
    PWM_freq_AdaptiveDuty(0);   __delay_ms(20);
    PWM_freq_AdaptiveDuty(415); __delay_ms(80);   // Ab4
    PWM_freq_AdaptiveDuty(0);   __delay_ms(20);
    PWM_freq_AdaptiveDuty(392); __delay_ms(80);   // G4
    PWM_freq_AdaptiveDuty(0);   __delay_ms(20);
    PWM_freq_AdaptiveDuty(370); __delay_ms(80);   // F#4
    PWM_freq_AdaptiveDuty(0);   __delay_ms(20);

    // final low held note
    PWM_freq(294); __delay_ms(600);  // D4
    PWM_freq(0);

    PWM_init();

    TMR0_init();
}

void ExtInt_init(void){
    TRISB = 0xFF;
    INTCON |= 0x10;
    buttonPressed = 0;
    return;
}

void ExtInt_disable(void){
    INTCON &= ~0x10;
    return;
}

void generateRandom(void){
    seed = (seed * 1664525 + 1013904223);  
    return;
}

void displayTime(uint16_t time){
    PORTD = sevSeg[time % 10];
    PORTA = 0x08;
    __delay_ms(2);
    PORTA = 0x00;

    PORTD = sevSeg[(time / 10) % 10];
    PORTA = 0x04;
    __delay_ms(2);
    PORTA = 0x00;

    PORTD = sevSeg[(time / 100) % 10];
    PORTA = 0x02;
    __delay_ms(2);
    PORTA = 0x00;

    PORTD = sevSeg[(time / 1000) % 10];
    PORTA = 0x01;
    __delay_ms(2);
    PORTA = 0x00;
    
    return;
}

void dislpayCountDown(void){
    PORTD = sevSeg[3];
    PORTA = 0x0F;
    __delay_ms(1000);
    PORTA = 0x00;

    PORTD = sevSeg[2];
    PORTA = 0x0F;
    __delay_ms(1000);
    PORTA = 0x00;

    PORTD = sevSeg[1];
    PORTA = 0x0F;
    __delay_ms(1000);
    PORTA = 0x00;

    return;
}

void displayTooSlow(void){
    PORTD = sevSeg[15];
    PORTA = 0x08;
    __delay_ms(2);
    PORTA = 0x00;

    PORTD = sevSeg[11];
    PORTA = 0x04;
    __delay_ms(2);
    PORTA = 0x00;

    PORTD = sevSeg[5];
    PORTA = 0x02;
    __delay_ms(2);
    PORTA = 0x00;

    PORTD = sevSeg[2];
    PORTA = 0x01;
    __delay_ms(2);
    PORTA = 0x00;
    
    return;
}

void displayPlay(void){
    PORTD = sevSeg[13];
    PORTA = 0x08;
    __delay_ms(2);
    PORTA = 0x00;

    PORTD = sevSeg[12];
    PORTA = 0x04;
    __delay_ms(2);
    PORTA = 0x00;

    PORTD = sevSeg[11];
    PORTA = 0x02;
    __delay_ms(2);
    PORTA = 0x00;

    PORTD = sevSeg[10];
    PORTA = 0x01;
    __delay_ms(2);
    PORTA = 0x00;

    return;
}