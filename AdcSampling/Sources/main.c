#include "derivative.h"
#include "CLK/CLK.h"
#include "ADC/ADC.h"
#include "SYSTICK/SysTick.h"
#include "CircularBuffer/CircularBuffer.h"
#include "DbgConsole/DbgConsole.h"

#define SAMPLES_NO	100

/* Ticks */
uint32_t currTicksCounter, prevTicksCounter;

/* Used on ADC events to have the store location for analog reads */
typedef struct adc_pair
{
	uint8_t adcChannel;
	circ_buff_t *storeLocation;
} adc_pair_t;

/* ADC Samples buffers*/
CIRC_buff_DEF(adc1samples, SAMPLES_NO);
CIRC_buff_DEF(adc2samples, SAMPLES_NO);
CIRC_buff_DEF(adc6samples, SAMPLES_NO);
CIRC_buff_DEF(adc8samples, SAMPLES_NO);
CIRC_buff_DEF(adc9samples, SAMPLES_NO);
CIRC_buff_DEF(adc13samples, SAMPLES_NO);
CIRC_buff_DEF(adc14samples, SAMPLES_NO);
CIRC_buff_DEF(adc15samples, SAMPLES_NO);

/* Select where to store each adc value */
const static adc_pair_t adc_channels[] =
{
	{ 1, &adc1samples },
	{ 2, &adc2samples },
	{ 6, &adc6samples },
	{ 8, &adc8samples },
	{ 9, &adc9samples },
	{ 13, &adc13samples },
	{ 14, &adc14samples },
	{ 15, &adc15samples }
};

void ADC_OnEnd(void)
{

	/// The values are handled on main loop
	// Channels counter
	static int chCount = 0,
			chNumber = (sizeof(adc_channels) / sizeof(adc_pair_t) - 1);

	// Save adc value
	uint16_t tmp = ADC_ReadResultReg(ADC);
	//*(adc_channels[chCount].storeLocation) = tmp;
	circ_buff_push((adc_channels[chCount].storeLocation), tmp);
	// Calculate next channel
	chCount = ((chCount == chNumber) ? (0) : (++chCount));

	// Set current channel to be read
	ADC_SetChannel(ADC, adc_channels[chCount].adcChannel);

	// Enable reading on next channel
	ADC_ContinuousConversion(ADC);
}

int main(void)
{
	/** Init clock */
	Clk_Init(CLK_FREQUENCY_16_MHZ);
	/** Debug console init */
	DbgConsole_Init(UART0, CLK_FREQUENCY_16_MHZ, 115200);

	/* Init ADC with default params */
	ADC_Init_Default();
	/* Set ADC interruption callback */
	ADC_SetCallBack(ADC_OnEnd);
	/* ADC Set channel */
	ADC_SetChannel(ADC, ADC_CHANNEL_AD10);

	/* SysTick Init - Reload value is provided in uS - 1000uS  = 1mS */
	SysTick_Init( CLK_FREQUENCY_16_MHZ/* Clock freq *// (1000 / 2 /* uS */));

	printf("--start!--\r\n");
	static int i = 0;

	for (;;)
	{
		if( currTicksCounter % 100 == 0 ) /* Every 500ms */
		{
			printf("\e[1;1H\e[2J");
			fflush(stdout);
			printf("Measurement: %d\r\n", i++);
			printf("ADC1 Average: %d\r\n", circ_buff_average(&adc1samples) );
			printf("ADC2 Average: %d\r\n", circ_buff_average(&adc2samples) );
			printf("ADC6 Average: %d\r\n", circ_buff_average(&adc6samples) );
			printf("ADC8 Average: %d\r\n", circ_buff_average(&adc8samples) );
			printf("ADC9 Average: %d\r\n", circ_buff_average(&adc9samples) );
			printf("AD13 Average: %d\r\n", circ_buff_average(&adc13samples) );
			printf("AD14 Average: %d\r\n", circ_buff_average(&adc14samples) );
			printf("AD15 Average: %d\r\n", circ_buff_average(&adc15samples) );

		}
	}
	return 0;
}
