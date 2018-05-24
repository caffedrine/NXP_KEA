#include "derivative.h"

// Drivers
#include "SYSTICK/SysTick.h"
#include "GPIO/GPIO.h"
#include "CLK/CLK.h"

#define CLK_FREQ_HZ	16000000

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

	/* SysTick Init - Reload value is provided in uS*/
	SysTick_Init(CLK_FREQ_HZ/* Clock freq */ / (1000 /* uS */));


	uint32_t taskStartTimestamp = 0, taskStopTimestamp = 0;
	uint32_t precentageLoad = 0, timeElapsedNs = 0;

	for ( ;; )
	{
		if( currTicksCounter != prevTicksCounter)		// one millisecond elapsed
		{
			prevTicksCounter = currTicksCounter;

			taskStartTimestamp = SYST_CVR;
			mainTask();		/* Execute task */
			taskStopTimestamp = SYST_CVR;

			// Calculate processor load
			// SysTick step is 0.125us = 125ns
			timeElapsedNs = (taskStartTimestamp - taskStopTimestamp) * 125;
			precentageLoad = (timeElapsedNs * 100)/( CLK_FREQ_HZ / SYST_RVR * 1000 /* we need ns */ );

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

	for(int i = 0; i<= 100000; i++);
}

