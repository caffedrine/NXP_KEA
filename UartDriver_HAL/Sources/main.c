#include "UartHal.h"
#include "CLK.h"

#define UART_PORT	UART2
#define IRQ_NR		UART2_IRQn
#define SYSCLK		(DEFAULT_SYSTEM_CLOCK / 2)
#define UART_BAUD	9600

int main()
{
	// Initialize clock
	Clk_Init();

	// UART INIT
	Uart_Init(UART_PORT, SYSCLK, UART_BAUD);
	NVIC_EnableIRQ( IRQ_NR );

	while(1)
	{
		//uint8_t c = UART_GetChar(UART_PORT);
		//UART_PutChar(UART_PORT, c);
	}

	return 0;
}
