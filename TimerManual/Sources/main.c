#include <stdint.h>

#include "SKEAZN642.h"
#include "gpio.h"

#define LED_PORT	PTH
#define LED_BIT		PTH1

void blinkMs(uint16_t ms)
{
	static uint32_t counter = 0, previousCounter = 0;
	if( counter - previousCounter > (ms*420) )
	{
		previousCounter = counter;
		OUTPUT_TOGGLE(LED_PORT, LED_BIT);
	}

	counter++;
}

int main(void)
{

    /* Write your code here */
	CONFIG_PIN_AS_GPIO(LED_PORT, LED_BIT, OUTPUT);


    /* This for loop should be replaced. By default this loop allows a single stepping. */
    for (;;)
    {
        blinkMs(1000);
    }
    /* Never leave main */
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
