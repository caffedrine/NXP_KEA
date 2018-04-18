/* ###################################################################
 **     Filename    : main.c
 **     Project     : HeartBeat_PE
 **     Processor   : SKEAZ64MLH4
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2018-04-18, 11:36, # CodeGen: 0
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file main.c
 ** @version 01.01
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup main_module main module documentation
 **  @{
 */
/* MODULE main */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Pins1.h"
#include "PWM1.h"
#include "PwmLdd1.h"
#include "TU1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"

#include "ADC.h"

_Bool direction = 1;
unsigned char currVal = 0;

/* User includes (#include below this line is not maintained by Processor Expert) */
void delayMs(int ms)
{
	for(int i = 0; i < ms; i++)
	{
		//1ms delay
		for(int j=0; j < 3390; j++)
			__asm("nop");
	}
}


/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	PWM1_SetRatio8( 0 );		// Turn led off

	// Init Adc on Channel 10 - 8bit resolution
	//int readVal = 0;
	//ADC_Init(10,0);
	//readVal = ADC_Read(10);

	unsigned char maxBright = 180;
	int speed = 8;
	for ( ;; )
	{
		int i,j;

		for(i = 0; i < maxBright; i++)	//too much brightness at 255
		{
			PWM1_SetRatio8(i);
			delayMs(speed);
		}

		for(i = maxBright-1; i >= 0; i--)	//too much brightness at 255
		{
			PWM1_SetRatio8(i);
			delayMs(speed);
		}
	}

	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.5 [05.21]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */
