#include "derivative.h"
#include "PWM/PWM.h"
#include "ADC/ADC.h"
#include "CLK/CLK.h"
#include "DbgConsole/DbgConsole.h"

volatile uint16_t LastAdcResult, CurrentAdcResult;

uint16_t mapint(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main()
{
	/* FTM module and it's channel */
	FTM_Type *pFTM = FTM2;
	uint8_t channel = 0;

	/* Initialize Core Clock to 40MHz, Bus Clock to 20MHz  */
	//Clk_Init();

	/* Debug console */
	//DbgConsole_Init(UART2, 16000000, 9600);

	/* Initialize the ADC module and read at Channel 10 and 8bit conversion mode is imposed */
	ADC_Init(10, ADC_TWELVE_BIT);

	/* Init pwm driver */
	PWM_Init( pFTM, 16000000 / 8 /*Clock in Hz with selected prescaler*/, 2245 /*Period in US*/);
	//PWM_Init( pFTM, 16000000 / 8 /*Clock in Hz with selected prescaler*/, 300 /*Period in US*/);

	/* Emable PWM on a given channel*/
	PWM_Enable( pFTM, channel );
	PWM_SetRatio16( pFTM, channel, 20000 );

	int i, j;
	while ( 1 )
	{
		LastAdcResult = CurrentAdcResult;
		CurrentAdcResult =  ADC_Read(10);

		if(CurrentAdcResult != LastAdcResult)
		{
			uint16_t mapped = mapint(CurrentAdcResult, 0, 4095, 1, 32537);
			PWM_SetRatio16(pFTM, channel, mapped);
			//printf("ADC: %d -> %d\n", CurrentAdcResult, mapped);
		}
	}
}
