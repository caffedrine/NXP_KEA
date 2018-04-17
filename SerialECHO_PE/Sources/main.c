/* ###################################################################
 **     Filename    : main.c
 **     Project     : SerialECHO
 **     Processor   : SKEAZN64MLH2
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2018-04-02, 16:19, # CodeGen: 0
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
#include "Serial.h"
#include "ASerialLdd1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include <string.h>

void delay(void)
{
	uint32_t i, j;
	for ( i = 0; i < 8; i++ )
		for ( j = 0; j < 65535; j++ )
			//for (j = 0; j < 1000; j++)
			;
}

void Cpu_Delay100us(void)
{
	uint16_t i;
	for ( i = 0; i < 1000; i++ )
	{
		__asm("nop");
		//in CW tools use asm("nop");
	}
}

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */
	int i = 0;
	Serial_Init();

	char buffer[32];
	memset( buffer, '\0', sizeof(buffer) );

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	/* For example: for(;;) { } */

	for ( ;; )
	{
		char tmp;
		if ( Serial_RecvChar( &tmp ) == ERR_OK )
		{
/* !!!!!! */if ( tmp == '0' || i > sizeof(buffer) ) // 0 instead of \n
			{
				for ( int j = 0; j < strlen( buffer ); j++ )
				{
					Serial_SendChar( buffer[j] );
					Cpu_Delay100us();
				}

				// Send final chars
				Serial_SendChar( '\r' );
				Cpu_Delay100us();
				Serial_SendChar( '\n' );
				Cpu_Delay100us();

				// Reset buffer
				memset( buffer, '\0', sizeof(buffer) );
				i = 0;
			}
			else
			{
				buffer[i++] = tmp;
			}
			//*/
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
