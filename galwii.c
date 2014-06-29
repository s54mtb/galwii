/**
 * GalWii - interface for electric skateboard and stuff like this
 *
 * 			Target mcu: STM32F103RB
 *		  Compiler: Keil MDK ARM with CMSIS RTX and STM32 Standartd Peripheral Libratry
 *
 *			Author: Marko Pavlin marko@pavlin.si
 *			Date: July 2014
 * 		  More info: http://gal.pavlin.si/galwii
 *
 *
 *             +------------------+
 *             !                  ! 
 *         	   !                  ! 
 *    +---+    !                  ! 
 *    !GAS!--->! PA1              ! 
 *    +---+    !                  ! 
 *         	   !                  ! 
 *             !              PB8 !>--- PWM ESC1
 *             !              PB9 !>--- PWM ESC2
 *             !                  ! 
 *             !                  ! 
 *             !          PC0-PC3 !<>LCD 
 *             !        PC10-PC12 !<>LCD 
 *             +------------------+
 */


#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC


// Globals
uint32_t 	lastADC;											// Last ADC readout
uint32_t 	lastpwmval1;									// Last PWM value for ESC1
uint32_t 	lastpwmval2;									// Last PWM value for ESC1

/* External references to the RTOS threads */
extern void lcd_thread(void const *args);
extern void adc_thread(void const *args);


/* RTOS thread definitions */
osThreadDef(adc_thread, osPriorityNormal, 1, 0);
osThreadDef(lcd_thread, osPriorityNormal, 1, 0);


int main (void) {                                // program execution starts here
  osKernelInitialize ();                         // initialize RTOS kernel
  osThreadCreate (osThread(lcd_thread), NULL);
  osThreadCreate (osThread(adc_thread), NULL);
  osKernelStart ();                              // start kernel with job2 execution
}
