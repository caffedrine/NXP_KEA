#include "derivative.h"

// Drivers
#include "ADC/ADC.h"
#include "UART/UART.h"
#include "SYSTICK/SysTick.h"
#include "GPIO/GPIO.h"
#include "CLK/CLK.h"

#define LED_BIT		PTC0
#define LED_PORT	PTC

// count system ticks
uint32_t currTicksCounter = 0, prevTicksCounter = 0;

// Declarations
void mainTask(void);

int main(void)
{
	/* GPIO Init */
	CONFIG_PIN_AS_GPIO(LED_PORT, LED_BIT, OUTPUT);

	/* Clock Init */
	Clk_Init();

	/* SysTick Init - 1000 ticks*/
	SysTick_Init();

	/* ADC Init - channel 10 with 8bit resolution */
	ADC_Init( 10, 0 );

	/* UART Init - UART2 with 9.6k baud*/
	UART_Init( UART2, DEFAULT_SYSTEM_CLOCK, 9600 );

	for ( ;; )
	{
		if( currTicksCounter != prevTicksCounter)
		{
			prevTicksCounter = currTicksCounter;

			// Do the job
			mainTask();

			// Reset ticks counters
			currTicksCounter = 0;
			prevTicksCounter = 0;

//			// Do the task  1000 miliseconds
//			if(ticks_counter % 1000 == 0)
//			{
//				mainTask();
//					currTicksCounter = 0;
//					prevTicksCounter = 0
//			}
		}
	}
	return 0;
}

// Globals
char str[4];
uint8_t adc_val;
uint32_t i = 0;

void mainTask()
{
//	// Read ADC
//	adc_val = ADC_Read( 10 );		// Read channel 10
//
//	// Send to UART2
//	UART_SendWait( UART2, &adc_val, 1 );
//	UART_SendWait( UART2, "\r", 1 );
//	UART_SendWait( UART2, "\n", 1 );

	OUTPUT_TOGGLE(LED_PORT, LED_BIT);
}

