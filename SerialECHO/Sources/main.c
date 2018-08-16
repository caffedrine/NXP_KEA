#include "derivative.h"

#include "CLK/CLK.h"
#include "UART/uart.h"

#define UART_PORT	UART0
#define SYSCLK		CLK_FREQUENCY_16_MHZ
#define UART_BAUD	115200
#define UART_PINS	PTD6_PTD7

int main(void)
{
	// Clock init
	Clk_Init(CLK_FREQUENCY_16_MHZ);

	/* Display clock */
	SIM_SOPT |= SIM_SOPT_CLKOE_MASK;		/* Enable Bus clock on PTH2 */
	SIM_SOPT |= SIM_SOPT_BUSREF(0b111); 	/* Bus clock output on PTH2 divided by 128 */

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
