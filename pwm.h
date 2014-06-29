#ifndef __pwm_h
#define __pwm_h
#include <stm32f10x.h>


void PWM_init(void);
void PWM_set(uint16_t channel, uint16_t value);


#endif
