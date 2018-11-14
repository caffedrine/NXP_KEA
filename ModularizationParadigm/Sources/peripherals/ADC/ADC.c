#include "ADC.h"

void ADC_Init(uint8_t channel, uint8_t mode)
{
	SIM_SCGC |= SIM_SCGC_ADC_MASK;				/* Enable bus clock in ADC*/
	ADC_SC3 |= ADC_SC3_ADICLK(0b00);			/* Bus clock selected*/
	ADC_SC2 |= 0x00;							/* Software Conversion trigger, disable compare function*/
	ADC_SC1 = 0	;								/* Enable ADC by setting ADCH bits as low*/
	ADC_SC1|= ADC_SC1_ADCO_MASK;  				/* Continuous mode operation */	
	ADC_SC1|= ADC_SC1_AIEN_MASK;  				/* ADC Interrupt Enabled */
	ADC_APCTL1 |= ADC_APCTL1_ADPC(1<<channel);  /* Channel selection */	
	ADC_SC3 |= ADC_SC3_MODE(mode);				/* 8,10,12 bit mode operation */
}

uint16_t ADC_Read(uint8_t channel)
{
	ADC_SC1 |= ADC_SC1_ADCH(channel);			/* Select channel to read */
	while(!(ADC_SC1 & ADC_SC1_COCO_MASK));		/* Wait conversion to complete */
	return ADC_R;								/* Return adc value */
}


