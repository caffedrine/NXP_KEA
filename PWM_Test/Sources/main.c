#include "FTM/FTM.h"
#include "GPIO/GPIO.h"
#include "CLK/CLK.h"

void ftm_callback()
{
     FTM_ClrOverFlowFlag(FTM2);
}

void pwmSetDutyCycle(FTM_Type* pFTM, uint8_t ftmChannel, uint16_t dutyCycle /*0-1000*/)
{
     uint32_t dc = pFTM->MOD - (pFTM->MOD) / (1000 / dutyCycle);
     FTM_SetChannelValue(pFTM, ftmChannel, dc);
}

void PWM_Init(FTM_Type* pFTM, uint8_t ftmChannel, uint32_t clkFreqHz, uint32_t freqHz)
{
     /* FTM Config */
     FTM_ConfigType FTM_Config = { 0 };
     FTM_Config.modulo = 10000;                                   /* Timer count */
     FTM_Config.clk_source = FTM_CLOCK_SYSTEMCLOCK;          /* Set clocking source */
     FTM_Config.prescaler = FTM_CLOCK_PS_DIV1;               /* Clock divider */
     FTM_Config.mode = 1;                                        /* Edge aligned PWM */
     FTM_Config.toie = 0;                                        /* enable channel interrupt */

     /* FTM Channel Config */
     FTM_ChParamsType FTM_CH_Config = { 0 };
     FTM_CH_Config.ctrl.bits.bMode = FTM_PWMMODE_EDGEALLIGNED;     /* PWM Setting */
     FTM_CH_Config.ctrl.bits.bPWMPol = FTM_PWM_HIGHTRUEPULSE;     /* Another PWM Setting */
     FTM_CH_Config.u16CnV = (FTM_Config.modulo);                         /* 0% dutycycle by default */

     /* Init FTM module */
     FTM_Init(pFTM, &FTM_Config);

     /* Set interrupt */
     FTM_SetCallback(pFTM, ftm_callback);
     /* Init channel */
     FTM_ChannelInit(pFTM, ftmChannel, FTM_CH_Config);
}

int main(void)
{
     FTM_Type *pFTM = FTM0;
     uint8_t ftmChannel = 1;

     /* Select Pins corresponds to the for output */
     SIM_PINSEL |= SIM_PINSEL_FTM0PS1_MASK;

     /* Init PWM driver */
     PWM_Init(pFTM, ftmChannel, DEFAULT_SYSTEM_CLOCK, 10000 /*Hz*/);

     /* Set duty cycle */
     pwmSetDutyCycle(pFTM, ftmChannel, 500);     /* DutyCycle - 0 is 0% and 1000 is 100% */

     while (1)
     {


     }
}
