#include "UART.h"

/**********************************************************************************************
 * External objects
 **********************************************************************************************/

/**********************************************************************************************
 * Global variables
 **********************************************************************************************/
pt2FuncU8 Uart_Callback;

/**********************************************************************************************
 * Constants and macros
 **********************************************************************************************/

/**********************************************************************************************
 * Local types
 **********************************************************************************************/

/**********************************************************************************************
 * Local function prototypes
 *********************************************************************************************/
void UART2_IRQHandler(void);

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
 * @brief    Uart_Init - Initalizes the Uart2 to run at 9600 bauds assuming bus clock of 20Mhz
 * @param    none
 * @return   none
 *
 ************************************************************************************************/
void UART_Init()
{
	SIM_SCGC |= SIM_SCGC_UART2_MASK; 	/* Enable bus clock in UART2*/
	UART2_BDH = 0; 						/* One stop bit*/
	UART2_BDL = 128; 					/* Baud rate at 9600*/
	UART2_C1 = 0; 						/* No parity enable, 8 bit format*/
	UART2_C2 |= UART_C2_TE_MASK; 		/* Enable Transmitter*/
	UART2_C2 |= UART_C2_RE_MASK; 		/* Enable Receiver*/
	UART2_C2 |= UART_C2_RIE_MASK; 		/* Enable Receiver interrupts*/
}

/***********************************************************************************************
 *
 * @brief    Uart_SetCallback - Set a callback function to execute when a byte is received on the Uart2
 * @param    pointer to function with an UINT8 argument
 * @return   none
 *
 ************************************************************************************************/
void Uart_SetCallback(pt2FuncU8 ptr)
{
	Uart_Callback = ptr;
}

/***********************************************************************************************
 *
 * @brief    Uart_SendChar - Send a single byte on Uart2
 * @param    byte to send
 * @return   none
 *
 ************************************************************************************************/
void Uart_SendChar(uint8_t send)
{
	while ( (UART2_S1 & UART_S1_TDRE_MASK) == 0 )
		; 				/* Wait for transmit buffer to be empty*/

	//for(int i=0; i<=2000; i++);						// This is devil!

	(void)UART2_S1; 	/* Read UART2_S1 register*/
	UART2_D = send; 	/* Send data*/
}

/***********************************************************************************************
 *
 * @brief    Uart_GetChar - Retrieve the received char from the Uart2 buffer (should be called from Interrupt)
 * @param    none
 * @return   received byte
 *
 ************************************************************************************************/
uint8_t Uart_GetChar()
{
	uint8_t recieve;
	while ( ( UART2_S1 & UART_S1_RDRF_MASK) == 0 )
		; 					/* Wait for received buffer to be full*/

	(void) UART2_S1; 		/* Read UART2_S1 register*/
	recieve = UART2_D; 		/* Read received data*/
	return recieve;
}

/***********************************************************************************************
 *
 * @brief    UART2_IRQHandler - Uart2 reception interrupt, calls the user defined callback function
 * @param    none
 * @return   none
 *
 ************************************************************************************************/
void UART2_IRQHandler()
{
	(void)UART2_S1; /* Clear reception flag mechanism*/
	Uart_Callback( Uart_GetChar() );
}
