#include "derivative.h"

#include "CLK.h"
#include "uart.h"

#define UART_PORT	UART2
#define SYSCLK		(DEFAULT_SYSTEM_CLOCK / 2)
#define UART_BAUD	9600
#define UART_PINS	PTD6_PTD7

int main(void)
{
	// Clock init
	Clk_Init();

	/* Initialize pin settings for UART communication */
	UART_pin_settings(UART_PINS);

	/* Initialize UART communication */
	UART_Init(UART_PORT, SYSCLK, UART_BAUD);

	UART_EnableTx(UART_PORT);
	UART_EnableRx(UART_PORT);

	for ( ;; )
	{
		uint8_t c = UART_GetChar(UART_PORT);
		UART_PutChar(UART_PORT, c);
	}
	/* Never leave main */
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
