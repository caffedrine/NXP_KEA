#include "derivative.h"

// Drivers
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

	/* SysTick Init */
	SysTick_Init(16000);		// Reload every 1 ms ( 40Mhz frequency )

	for ( ;; )
	{
		if( currTicksCounter != prevTicksCounter)	// one millisecond elapsed
		{
			prevTicksCounter = currTicksCounter;

			if(currTicksCounter%1000 == 0)
			{
				// Do the job
				mainTask();
			}

			if(currTicksCounter != prevTicksCounter)	// if task took more than one millisecond
			{
				// Loop execution time is too long - so a reset is required
				NVIC_SystemReset();
			}
		}
	}
	return 0;
}

void mainTask()
{
	OUTPUT_TOGGLE(LED_PORT, LED_BIT);
}

