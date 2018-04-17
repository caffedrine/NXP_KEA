#include "SKEAZ1284.h"

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
#define PTH1 25
#define LED	PTH1

	GPIOB_PDDR |= 1 << LED;

	for ( ;; )
	{
		GPIOB_PSOR |= 1 << LED;
		delay(); /* Generate delay */

		GPIOB_PCOR |= 1 << LED;
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
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
