#include "PWM/PWM.h"

int main()
{
	/* FTM module and it's channel */
	FTM_Type *pFTM = FTM1;
	uint8_t channel = 1;

	/* Init pwm driver */
	PWM_Init( pFTM, 16000000/8 /*Clock in Hz with selected prescaler*/, 1000 /*Period in US*/ );

	/* Emable PWM on a given channel*/
	PWM_Enable( pFTM, channel);

	PWM_SetRatio16( pFTM, channel, 10000 );

	int i, j;
	while ( 1 )
	{
		for ( i = 0; i <= 65535; i++ )
		{
			PWM_SetRatio16( pFTM, channel, i );
			for ( int j = 0; j <= 10; j++ )
				;
		}

		for ( i = 65534; i >= 1; i-- )
		{
			PWM_SetRatio16( pFTM, channel, i );
			for ( int j = 0; j <= 10; j++ )
				;
		}
	}
}
