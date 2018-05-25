#include "ftm.h"

void pwmSetDutyCycle(FTM_Type* pFTM, uint8_t ftmChannel, uint16_t dutyCycle /*0-1000*/)
{
	uint32_t dc = pFTM->MOD - (pFTM->MOD) / (1000 / dutyCycle);
	FTM_SetChannelValue(pFTM, FTM_CHANNEL_CHANNEL1, dc);
}

void PWM_Init(FTM_Type* pFTM, uint8_t ftmChannel, uint32_t clkFreqHz, uint32_t freqHz)
{
	/* FTM Config */
	FTM_ConfigType FTM_Config = { 0 };
	FTM_Config.modulo = 10000;						//adjust period from here
	FTM_Config.clk_source = FTM_CLOCK_SYSTEMCLOCK;
	FTM_Config.prescaler = FTM_CLOCK_PS_DIV2;			// 4kHz @ Clk 16.000Hz
	FTM_Config.mode = 1;			//
	//FTM_Config.toie=1;			// Enable interrupt

	/* FTM Channel Config */
	FTM_ChParamsType FTM_CH_Config = { 0 };
	FTM_CH_Config.ctrl.bits.bMode = FTM_PWMMODE_EDGEALLIGNED;
	FTM_CH_Config.ctrl.bits.bPWMPol = FTM_PWM_HIGHTRUEPULSE;
	FTM_CH_Config.u16CnV = (FTM_Config.modulo);		// 0% dutycycle by default

	/* Set interrupt */
	//FTM_SetCallback(pFTM, call);
	/* Init channel */
	FTM_ChannelInit(pFTM, ftmChannel, FTM_CH_Config);
	/* Init FTM module */
	FTM_Init(pFTM, &FTM_Config);
}

int main(void)
{
	/* Select Pins corresponds to the PTE7 for output */
	SIM_PINSEL0 |= SIM_PINSEL_FTM1PS1_MASK;

	/* Init PWM driver */
	PWM_Init(FTM1/*FTM1*/, 1 /*FTM_CH1*/, DEFAULT_SYSTEM_CLOCK, 10000 /*Hz*/);

	/* Set duty cycle */
	pwmSetDutyCycle(FTM1, 1, 500);	/* 1.5% DutyCycle */

	while (1)
	{

	}
}
