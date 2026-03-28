#include <xc.h>
#include <stdint.h>

void TMR0_init(void){
    // Tick/sec = Crystal / (Prescaler * 4)     = 125000   
    //125000/s * 1s/1000ms                      = 125tick/ms

    // 256-125  ticks                           = 131  

    OPTION_REG = 0x02;

    TMR0 = 131;
    INTCON |= 0xA0;
    return;
}

void TMR0_overflowReset(void){
    TMR0 = 131;
    INTCON &= ~0x04;
    return;
}