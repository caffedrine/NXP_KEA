#include "derivative.h"
#include "UART/UART.h"
#include "string.h"

static int i, j = 0;

void Clk_Init()
{
	ICS_C1 |= ICS_C1_IRCLKEN_MASK; /* Enable the internal reference clock*/
#ifdef SKEAZ1284
	ICS_C3 = 0x90; /* Reference clock frequency = 31.25 kHz*/
#elif defined SKEAZN642
	ICS_C3 = 0x50; /* Reference clock frequency = 31.25 kHz*/
#endif
	while ( !(ICS_S & ICS_S_LOCK_MASK) )
		; /* Wait for PLL lock, now running at 40 MHz (1280 * 31.25Khz) */
	ICS_C2 |= ICS_C2_BDIV( 1 ); /*BDIV=2, Bus clock = 20 MHz*/
	//ICS_S |= ICS_S_LOCK_MASK ; 				/* Clear Loss of lock sticky bit */
}

int main(void)
{
	uint8_t uartChannel = 2;

	/* CLK Init */
	Clk_Init();

	/* UART2 Init */
	Uart_Init( uartChannel, 20000000, 9600 );

	uint8_t counter = 0;
	char recvBytes[16];
	memset( recvBytes, '\0', 16 );

//	char *sendMe = "Hello world!\r\n";
//	Uart_Write(uartChannel, sendMe, strlen(sendMe), &counter);
	counter = 0;

	//Un_RxISR(2, 'a');

	while ( 1 )
	{
		if ( Uart_GetRxQueueLevel(uartChannel) >= 10 )
		{
			Uart_Read( uartChannel, recvBytes, 10, &counter );
			if ( counter >= 10 )
			{
				counter = 0;
				Uart_Write( uartChannel, recvBytes, 10, &counter );
				Uart_Write( uartChannel, "\r\n", 2, &counter );

				memset( recvBytes, '\0', 16 );
				counter = 0;
			}
		}
	}

	return 0;
}
