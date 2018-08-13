/*
 * Author: caffedrine@github
 *
 * PS: Make sure you increase default heap_size and stack_size from *.ld file
 */

#include "derivative.h"
#include "CLK/CLK.h"
#include "DbgConsole/DbgConsole.h"

void delay(void)
{
	uint32_t i, j;
	for ( i = 0; i < 8; i++ )
		for ( j = 0; j < 40535; j++ )
			//for (j = 0; j < 1000; j++)
			;
}


int main(void)
{
	int i = 0;

	/** Init clock */
	Clk_Init(CLK_FREQUENCY_16_MHZ);

	/** Debug console init */
	DbgConsole_Init(UART2, CLK_FREQUENCY_16_MHZ, 115200);

	/* !!! Don't use comma inside float numbers, use dot instead */

	for (;;)
	{

		float a = 12.323232, b=13.231234;

		//printf("Insert a number: ");
		//scanf("%d", &i);
		printf("\r\nNumber inserted is: %d\r\n", i++);
		printf("This is an int number: %d\r\n", 123456);
		printf("This is an float number: %.2f\r\n", 12.2345);
		printf("This is just a string\r\n");
		printf("a = %.2f\r\n", a);
		printf("b = %.3f\r\n", b);
		printf("float numbers: %.4f\r\n", 14.123412);
		printf("float numbers: %.5f\r\n", 15.1234123);
		printf("float numbers: %.6f\r\n", 16.12341234);
		printf("float numbers: %.7f\r\n", 17.123412345);
		printf("this is the end!\r\n=================\r\n\r\n");

		delay();
	}

    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
