#include "UartHal.h"
#include "CLK.h"

#define UART_CHANNEL	2
#define UART_PORT		UART2
#define IRQ_NR			UART2_IRQn
#define SYSCLK			(DEFAULT_SYSTEM_CLOCK / 2)
#define UART_BAUD		9600

void delay(void)
{
	uint32_t i, j;
	for (i = 0; i < 8; i++)
		for (j = 0; j < 65535; j++)
			;
}

int main()
{
	// Initialize clock
	Clk_Init();

	// UART INIT
	Uart_Init(UART_PORT, SYSCLK, UART_BAUD);
	NVIC_EnableIRQ( IRQ_NR );

	// Read result
	char readBytes[8] = {0};
	uint8_t readBytesNumber = 0;

	while(1)
	{
		Uart_Read(UART_CHANNEL, readBytes, 1, &readBytesNumber);

		if(readBytesNumber > 0)
		{
			int breakpoint = 0;
		}

		//uint8_t c = UART_GetChar(UART_PORT);
		//UART_PutChar(UART_PORT, c);
	}

	return 0;
}
