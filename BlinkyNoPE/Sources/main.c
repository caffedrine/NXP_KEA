#include "SKEAZN642.h"

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
#define PTE0 0
#define PTE1 1

	GPIOB_PDDR |= 1 << PTE0;
	GPIOB_PDDR |= 1 << PTE1;

	for ( ;; )
	{
		GPIOB_PCOR |= 1 << PTE0;
		GPIOB_PSOR |= 1 << PTE1;
		delay(); /* Generate delay */

		GPIOB_PSOR |= 1 << PTE0;
		GPIOB_PCOR |= 1 << PTE1;
		delay(); /* Generate delay */

		GPIOB_PSOR |= 1 << PTE0;
		GPIOB_PSOR |= 1 << PTE1;
		delay();

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
