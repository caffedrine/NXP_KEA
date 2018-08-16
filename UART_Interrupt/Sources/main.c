#include "derivative.h"

#include "DbgConsole/DbgConsole.h"
#include "CLK/CLK.h"
#include "UART/UART.h"

static int i = 0;

int main(void)
{
	/* Clock init */
	Clk_Init(CLK_FREQUENCY_16_MHZ);

	/* Init debug console or UART port directly */
	DbgConsole_Init(UART2, CLK_FREQUENCY_16_MHZ, 115200);

	for ( ;; )
	{
		//printf("Alex will rull the world one day!\r\n");

		UART_SendWait(UART2, "Alex will rull the world one day!\r\n", 35);

		for(int i=0; i<= 100000; i++);
	}
	/* Never leave main */
	return 0;
}
