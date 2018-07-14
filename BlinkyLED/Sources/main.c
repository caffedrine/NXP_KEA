#include "derivative.h"
#include "GPIO/GPIO.h"

static int i = 0;

void delay(void)
{
	uint32_t i, j;
	for (i = 0; i < 8; i++)
		for (j = 0; j < 65535; j++)
			;
}

void blink()
{
	#define LED_BIT		PTC0
	#define LED_PORT  	PTC

	/* Led direction */
	LED_PORT->PDDR |= 1 << LED_BIT;

	for ( ;; )
	{
		/* Set led */
		LED_PORT->PSOR |= 1 << LED_BIT;
		delay(); /* Generate delay */

		/* Clear led */
		LED_PORT->PCOR |= 1 << LED_BIT;
		delay(); /* Generate delay */
	}
}

int main(void)
{
	for ( ;; )
	{
		blink();
	}
	/* Never leave main */
	return 0;
}

