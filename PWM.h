#ifndef PWM_H
#define PWM_H

#include <stdint.h>

void PWM_init(void);
void PWM_duty(uint16_t duty);
void PWM_freq(uint16_t freq);


#endif