#include "derivative.h"
#include "GPIO/GPIO.h"

#ifdef SKEAZ1284
	#define LED_BIT		PTE7
	#define LED_PORT  	PTE
#endif

#ifdef SKEAZN642
	#define LED_BIT		PTC0
	#define LED_PORT  	PTC
#endif

void delay(void)
{
	uint32_t i, j;
	for (i = 0; i < 8; i++)
		for (j = 0; j < 65535; j++)
			;
}

void blink()
{
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

