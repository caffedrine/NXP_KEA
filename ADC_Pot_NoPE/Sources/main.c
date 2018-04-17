#include "derivative.h" /* include peripheral declarations */
#include "CLK.h"
#include "ADC.h"

#define PTH0	24
#define LED 	PTH0

int main(void)
{
	uint8_t adc_value = 0;

	/* Initialize Core Clock to 40MHz, Bus Clock to 20MHz  */
	Clk_Init();

	/* Initialize the ADC module and read at Channel 10 and 8bit conversion mode is imposed */
	ADC_Init(10,0);

	// Set LED as output
	GPIOB_PDDR |= 1<<LED;

	while(1)
	{
		adc_value = ADC_Read(10);

		if(adc_value > 127)
			GPIOB_PSOR |= 1<<LED;
		else
			GPIOB_PCOR |= 1<<LED;
	}

	return 0;
}
