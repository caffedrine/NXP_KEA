/********************************************************************************
*    LAB8 - Serial Communications Interface (UART)                            	*
*    FRDM+KEA64 Board															*
*                                                                             	*
*    This lab demonstrates how to use the UART module to receive and send  		*
*    characters. Using interrupts, it waits for characters sent from a PC     	*
*    terminal and echoes the received data.									  	*
*    										                                  	*
*    To use this demo you need to open a  serial terminal. The OpenSDA port  	*
*    is used to communicate via UART2 to the KEA128 microcontroller.         	*
*                                                                             	*
*    Open a terminal utility and set the baud rate to 9600, leave all    		*
*    the other settings with the default values. Every character you write 		*
*    to the serial console will be replied by the KEA64 MCU.                   *
********************************************************************************/



/**********************************************************************************************
* External objects
**********************************************************************************************/
#include "derivative.h" /* include peripheral declarations */
#include "UART.h"
#include "CLK.h"
/**********************************************************************************************
* Global variables
**********************************************************************************************/


/**********************************************************************************************
* Constants and macros
**********************************************************************************************/


/**********************************************************************************************
* Local types
**********************************************************************************************/


/**********************************************************************************************
* Local function prototypes
*********************************************************************************************/
void Enable_Interrupt(uint8_t vector_number);
void UART_Interrupt (void);


/**********************************************************************************************
* Local variables
**********************************************************************************************/


/**********************************************************************************************
* Local functions
**********************************************************************************************/


/**********************************************************************************************
* Global functions
**********************************************************************************************/


/***********************************************************************************************
*
* @brief    main() - Program entry function
* @param    none
* @return   none
*
************************************************************************************************/
int main(void)
{
	/* Initialize Core Clock to 40MHz, Bus Clock to 20MHz  */
	Clk_Init();

	/*Initialize UART2 at 9600 bauds */
	UART_Init();

	/* Enable UART2 interrupt */
	Enable_Interrupt(UART2_IRQn);

	UART_SendStr("** UART Example ** \n\n\r");

	while(1);

	return 0;
}


/***********************************************************************************************
*
* @brief    Enable_Interrupt(uint8_t vector_number). Enable interrupts from desired module.
* @param    Module interrupt number from the interrupts vector table
* @return   none
*
************************************************************************************************/
void Enable_Interrupt(uint8_t vector_number)
{

	NVIC_ClearPendingIRQ(vector_number); 		/* Clear pending interrupt register */
	NVIC_EnableIRQ(vector_number); 				/* Enable required Interrupt */

}

/***********************************************************************************************
*
* @brief    UART2_IRQHandler - UART2 reception interrupt, calls the user function
* @param    none
* @return   none
*
************************************************************************************************/
void UART2_IRQHandler()
{
	UART_Interrupt();
}

/***********************************************************************************************
*
* @brief    UART_Interrupt(void). User Function called by the UART module on each interrupt
* @param    none
* @return   none
*
************************************************************************************************/
void UART_Interrupt(void)
{
	(void)UART2_S1;								/* Clear reception flag mechanism*/
	uint8_t send = UART_GetChar();				/* Get the Char */
	UART_SendChar(send);						/* Send the same char to echo */
}
