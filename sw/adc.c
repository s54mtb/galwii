#include "STM32F10x.h"
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "pwm.h"
#include "kalman.h"

extern uint32_t 	lastADC;
extern uint32_t 	lastpwmval1;
extern uint32_t 	lastpwmval2;

/********************************************************************************
  ADC thread 
	*/
void adc_thread(void const *args)
{
	
	int32_t adc;
	int32_t avgadc;
	double fadc;
	ADC_InitTypeDef  adcinit;
	uint16_t pwmval = 0;

	// init PWM
	PWM_init();
	PWM_set(3,0);
	PWM_set(4,0);



	/* Initialize ADC */
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_DeInit(ADC1);
	
  adcinit.ADC_Mode = ADC_Mode_Independent;
	adcinit.ADC_ScanConvMode = DISABLE;												// Disable the scan conversion - one coversion a time
	adcinit.ADC_ContinuousConvMode = DISABLE;									// Don't do contimuous conversions
	adcinit.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // Start conversin by software
	adcinit.ADC_DataAlign = ADC_DataAlign_Right;							// Put result in in the lower 12 bits
	adcinit.ADC_NbrOfChannel = 1;															// One channel is used by the sequencer
	
	ADC_Init (ADC1, &adcinit);																// Init ADC1
	ADC_Cmd(ADC1, ENABLE);																		// Enable ADC
	
	/* Perform ADC1 calibration */
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
	
	while (1)
	{
		// Prepare channel 0 for ADC sample		
	  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_1Cycles5);
		// Start the conversion
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    // Wait until conversion completion
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET) osThreadYield();
    // Get the conversion value
		adc = ADC_GetConversionValue(ADC1);

		
		fadc = 100 * (double)adc;
		avgadc = (int32_t)(kalman_update(fadc)/100);		
		lastADC = avgadc;
		
		// Calculate PWM value for ESC1
		pwmval = 99 + (avgadc-0x400) / 21;
		lastpwmval1 = pwmval;
		PWM_set(3,pwmval);

		// Calculate PWM value for ESC2
		pwmval = 99 + (avgadc-0x400) / 21;
  	lastpwmval2 = pwmval;
		PWM_set(4,pwmval);
		
		osDelay (1);		// Delay 1ms
	}

}





