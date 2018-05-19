#include "ftm.h"

FTM_Type* pFTM1 = (FTM_Type*) FTM1;

void FTM1_Task(void);

int main(void)
{
	/* Select Pins corresponds to the PTE7 for output */
	SIM_PINSEL0 |= SIM_PINSEL_FTM1PS1_MASK;

	/* FTM1 Config */
	FTM_ConfigType FTM1_Config = { 0 };
	FTM1_Config.modulo = 19000;							//adjust period from here
	FTM1_Config.clk_source = FTM_CLOCK_SYSTEMCLOCK;
	FTM1_Config.prescaler = FTM_CLOCK_PS_DIV1;
	FTM1_Config.mode = 1;
	FTM1_Config.toie = 1;

	// FTM1CH1 Config
	FTM_ChParamsType FTM1CH1_Config = { 0 };
	FTM1CH1_Config.ctrl.bits.bMode = FTM_PWMMODE_EDGEALLIGNED;
	FTM1CH1_Config.ctrl.bits.bPWMPol = FTM_PWM_HIGHTRUEPULSE;
	FTM1CH1_Config.u16CnV = 1000;

	FTM_SetCallback(pFTM1, FTM1_Task);
	FTM_ChannelInit(pFTM1, 1, FTM1CH1_Config);
	FTM_Init(pFTM1, &FTM1_Config);

	FTM_SetChannelValue(pFTM1, FTM_CHANNEL_CHANNEL1, 18000);

	while (1)
	{

	}
}


void FTM1_Task(void)
{
}
