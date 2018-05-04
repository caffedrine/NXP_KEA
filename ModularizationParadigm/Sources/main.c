#include <string.h>

#include "derivative.h"

// Drivers
#include "ADC/ADC.h"
#include "UART/UART.h"
#include "SYSTICK/SysTick.h"
#include "CLK.h"

// count system ticks
uint32_t ticks_counter = 0;

int main(void)
{
	char str[4];
	uint8_t adc_val;
	uint32_t i = 0;
	uint32_t prevTick = 0;

	/* Clock Init */
	Clk_Init();
	/* SysTick Init - 1000 ticks*/
	SysTick_Init(1000);
	/* ADC Init - channel 10 with 8bit resolution */
	ADC_Init( 10, 0 );
	/* UART Init - UART2 with 9.6k baud*/
	UART_Init( UART2, DEFAULT_SYSTEM_CLOCK, 9600 );

	for ( ;; )
	{
		if( prevTick != ticks_counter)
		{
			prevTick = ticks_counter;


			// Print every 1000 miliseconds
			if(ticks_counter % 1000 == 0)
			{
				// Read ADC
				adc_val = ADC_Read( 10 );		// Read channel 10

				// Send to UART2
				UART_SendWait( UART2, &adc_val, 1 );
				UART_SendWait( UART2, "\r", 1 );
				UART_SendWait( UART2, "\n", 1 );
			}
		}
	}
	return 0;
}

