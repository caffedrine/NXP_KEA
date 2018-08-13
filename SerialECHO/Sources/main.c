#include <stdint.h>
#include "derivative.h"
#include "UART.h"

/// Configure clock
void Clk_Init()
{
	ICS_C1 |= ICS_C1_IRCLKEN_MASK; 			/* Enable the internal reference clock*/
#ifdef SKEAZ1284
	ICS_C3 = 0x90; 							/* Reference clock frequency = 31.25 kHz*/
#elif defined SKEAZN642
	ICS_C3 = 0x50; 							/* Reference clock frequency = 31.25 kHz*/
#endif
	while ( !(ICS_S & ICS_S_LOCK_MASK) );	/* Wait for PLL lock, now running at 40 MHz (1024*39.0625 kHz) */
	ICS_C2 |= ICS_C2_BDIV( 1 ); 			/*BDIV=2, Bus clock = 20 MHz*/
	//ICS_S |= ICS_S_LOCK_MASK; 				/* Clear Loss of lock sticky bit */ ????????
	//???
}

/// UART2 Callback Handler
void Uart_Interrupt(uint8_t data)
{
	// Print back received character
	Uart_SendChar( data );
}

int main(void)
{
	/// Clock initialization
	Clk_Init(); /* Configure clocks to run at 20 Mhz */

	/// UART2 Initialization
	UART_Init();							/*Initialize Uart2 at 9600 bauds */
	Uart_SetCallback( Uart_Interrupt ); 	/* Set the callback function that the UART driver will call when receiving a char */
	NVIC_EnableIRQ( UART0_IRQn ); 			/* Enable UART2 interrupt */

	// Send "WELCOME\r\n"
	Uart_SendChar( 0x57 );	// W
	Uart_SendChar( 0x45 );	// E
	Uart_SendChar( 0x4c );	// L
	Uart_SendChar( 0x43 );	// C
	Uart_SendChar( 0x4f );	// O
	Uart_SendChar( 0x4d );	// M
	Uart_SendChar( 0x45 );	// E
	Uart_SendChar( 0xD );	//\r
	Uart_SendChar( 0xA );	//\n

	for ( ;; )
	{
	}

	/* Never leave main */
	return 0;
}

