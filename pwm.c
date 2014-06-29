// File Name: pwm.c
// Author: 		Marko Pavlin
// Date:			July 2014


#include <stm32f10x.h>
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "GPIO_STM32F10x.h"             // Keil::Device:GPIO


void PWM_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// Enable clock for GPIO Port B
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// Setup GPIO pins
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* configure GPIOs used for LEDs */
  RCC->APB2ENR |=  RCC_APB2ENR_IOPBEN;            /* Enable GPIOB clock       */
  GPIOB->ODR   &= ~(0x0000FF00);                  /* switch off LEDs          */
  GPIOB->CRH    =  (0x333333BB);                  /* PB8..9 AF Push Pull      */


	
	// Enable clock for TIM4
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	// Time base configuration: 100kHz clock with 2000 cycles auto reload (standard for servos & ESCs)
  TIM_TimeBaseStructure.TIM_Prescaler = (720 - 1);					// 100kHz
  TIM_TimeBaseStructure.TIM_Period = 2000-1;								// 20ms
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	// PWM1 Mode configuration for PWM Channel 3
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	// PWM1 Mode configuration for PWM Channel 4
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	// enable TIM4 counter 
  TIM_Cmd(TIM4, ENABLE);
	
}


void PWM_set(uint16_t channel, uint16_t value)
{
	switch(channel)
	{
		case 1 : TIM4->CCR1 = value; break;
		case 2 : TIM4->CCR2 = value; break;
		case 3 : TIM4->CCR3 = value; break;
		case 4 : TIM4->CCR4 = value; break;
	}
}



