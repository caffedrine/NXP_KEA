#include "derivative.h"
#include "ADC/ADC.h"
#include "GPIO.h"

static int i = 0;
volatile uint16_t adcResult;

void ADC_OnEnd(void)
{
	// Provide the channels to be read
	static uint8_t adc_channels[] =
	{
			ADC_CHANNEL_AD0,
			ADC_CHANNEL_AD6,
			ADC_CHANNEL_AD7,
			ADC_CHANNEL_AD10,
			ADC_CHANNEL_AD12
	};

	// Channels counter
	static int chCount = 0, chNumber = sizeof(adc_channels);

	// Set current channel to be read
	ADC_SetChannel(ADC, adc_channels[chCount]);

	// Process adc result
	adcResult = ADC_ReadResultReg(ADC);
	int tmp = adcResult;

	// Calculate next channel
	chCount = ((chCount == chNumber) ? (0) : (++chCount) );

	// Enable reading on next channel
	ADC_ContinuousConversion(ADC);


}

int main(void)
{
	/* Init ADC with default params */
	ADC_Init_Default();
	/* Set ADC interruption callback */
	ADC_SetCallBack(ADC_OnEnd);
	/* ADC Set channel */
	ADC_SetChannel(ADC, ADC_CHANNEL_AD10);

	for ( ;; )
	{
//		int a = ADC_PollRead(ADC, ADC_CHANNEL_AD10);
//		int b = a;

		i++;
	}
	/* Never leave main */
	return 0;
}


////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
