#include <xc.h>
#include <stdint.h>


void PWM_init(void){
    CCP1CON = 0x00;
    CCPR1L = 0x00;
    PR2 = 0x00;

    TRISC &= ~0x04;     

    CCP1CON = 0x0C;     // PWM mode

    // Prescaler 
    T2CON = 0x07;       // [1:0] = 3 for prescaler = 16
    return;
}

void PWM_duty(uint16_t duty){
    CCP1CON &= ~0x30;
    CCP1CON |= (uint8_t) ((duty & 0x0003) << 4);  // LSB
      
    CCPR1L = (uint8_t) (duty >> 2);               //MSB
    return;
}

void PWM_freq(uint16_t freq){
    PR2 = (uint8_t) (62500.0 / (float)freq) - 1;
    return;
}

void PWM_freq_AdaptiveDuty(uint16_t freq){
    uint16_t tempDuty;

    PR2 = (uint8_t) (62500.0 / (float)freq) - 1;
    
    tempDuty = (uint16_t) ((float)(PR2 * 4)) / 2.0;

    CCP1CON &= ~0x30;
    CCP1CON |= (uint8_t) ((tempDuty & 0x0003) << 4);  // LSB
      
    CCPR1L = (uint8_t) (tempDuty >> 2);               //MSB
    return;
}