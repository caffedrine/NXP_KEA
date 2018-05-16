#include "SKEAZN642.h"
#include "DbgConsole/DbgConsole.h"

void delay(void)
{
	uint32_t i, j;
	for ( i = 0; i < 8; i++ )
		for ( j = 0; j < 65535; j++ )
			//for (j = 0; j < 1000; j++)
			;
}

void Clk_Init()
{
	ICS_C1|=ICS_C1_IRCLKEN_MASK; 			/* Enable the internal reference clock*/
	ICS_C3 = 0x50; 							/* Reference clock frequency = 31.25 kHz*/
	while(!(ICS_S & ICS_S_LOCK_MASK));  	/* Wait for PLL lock, now running at 40 MHz (1280 * 31.25Khz) */
    ICS_C2|=ICS_C2_BDIV(1)  ; 				/*BDIV=2, Bus clock = 20 MHz*/
	//ICS_S |= ICS_S_LOCK_MASK ; 			/* Clear Loss of lock sticky bit */
}

int main(void)
{
	int i = 0;

	/** Init clock */
	Clk_Init();

	/** Debug console init */
	DbgConsole_Init(UART2, DEFAULT_SYSTEM_CLOCK/2, 9600);

	for (;;)
	{

		printf("Insert a number: ");
		scanf("%d", &i);
		printf("\r\nNumber inserted is: %d\r\n", i);

		delay();
	}

    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
