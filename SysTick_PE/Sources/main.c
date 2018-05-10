/* ###################################################################
 **     Filename    : main.c
 **     Project     : SysTick_PE
 **     Processor   : SKEAZN64MLH2
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2018-05-09, 18:29, # CodeGen: 0
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
#include "SysTick.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "GPIO/GPIO.h"

#define LED_BIT		PTC0
#define LED_PORT	PTC

// count system ticks
uint32_t currTicksCounter = 0, prevTicksCounter = 0;

// Declarations
void mainTask(void);

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	uint32_t tmp;

	/* GPIO Init */
	CONFIG_PIN_AS_GPIO( LED_PORT, LED_BIT, OUTPUT );
	for ( ;; )
	{
		if( currTicksCounter != prevTicksCounter)
		{
			prevTicksCounter = currTicksCounter;

			if(currTicksCounter % 1000 == 0)// every one second
			{
				mainTask();
			}

			if(currTicksCounter != prevTicksCounter)
			{
				//reset
			}
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

void mainTask()
{
//	// Read ADC
//	adc_val = ADC_Read( 10 );		// Read channel 10
//
//	// Send to UART2
//	UART_SendWait( UART2, &adc_val, 1 );
//	UART_SendWait( UART2, "\r", 1 );
//	UART_SendWait( UART2, "\n", 1 );

	OUTPUT_TOGGLE(LED_PORT, LED_BIT);
}

void SysTick_Every1MS(void)
{
	currTicksCounter++;
}


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
