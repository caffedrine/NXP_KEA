/*
 * PWM.c
 *
 *  Created on: May 24, 2018
 *      Author: Alex C.
 */

#include "PWM.h"

void PWM_Init(FTM_Type *pFTM, uint32_t u32clock, uint16_t u16periodUS)
{
	/* SIM_SCGC: pFTM=1 */
	SIM_SCGC |= ((pFTM == FTM0) ? SIM_SCGC_FTM0_MASK : (pFTM == FTM1 ? SIM_SCGC_FTM1_MASK : SIM_SCGC_FTM2_MASK));

	if ( pFTM == FTM2 )
	{
		/* Set up mode register */
		/* FTM_MODE: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,FAULTIE=0,FAULTM=0,CAPTEST=0,PWMSYNC=0,WPDIS=1,INIT=0,FTMEN=0 */
		pFTM->MODE = (FTM_MODE_FAULTM(0x00) | FTM_MODE_WPDIS_MASK);

		/* Clear counter initial register */
		/* FTM_CNTIN: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,INIT=0 */
		pFTM->CNTIN = FTM_CNTIN_INIT( 0x00 );
	}

	/* Clear status and control register */
	/* FTM_SC: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,TOF=0,TOIE=0,CPWMS=0,CLKS=0,PS=0 */
	pFTM->SC = (FTM_SC_CLKS(0x00) | FTM_SC_PS( 0x00 ));

	/* Reset counter register */
	/* FTM_CNT: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,COUNT=0 */
	pFTM->CNT = FTM_CNT_COUNT( 0x00 );

	/* Clear channel status and control register */
	/* FTM_C0SC: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,CHF=0,CHIE=0,MSB=0,MSA=0,ELSB=0,ELSA=0,??=0,??=0 */
	FTM_CnSC_REG(pFTM, 0) = 0x00U;

	/* Clear channel status and control register */
	/* FTM_C1SC: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,CHF=0,CHIE=0,MSB=0,MSA=0,ELSB=0,ELSA=0,??=0,??=0 */
	FTM_CnSC_REG(pFTM, 1) = 0x00U;

	if ( pFTM == FTM2 )		// only FTM2 have more than 2 Channels
	{
		/* Clear channel status and control register */
		/* FTM_C2SC: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,CHF=0,CHIE=0,MSB=0,MSA=0,ELSB=0,ELSA=0,??=0,??=0 */
		FTM_CnSC_REG(pFTM, 2) = 0x00U;

		/* Clear channel status and control register */
		/* FTM_C3SC: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,CHF=0,CHIE=0,MSB=0,MSA=0,ELSB=0,ELSA=0,??=0,??=0 */
		FTM_CnSC_REG(pFTM, 3) = 0x00U;

		/* Clear channel status and control register */
		/* FTM_C4SC: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,CHF=0,CHIE=0,MSB=0,MSA=0,ELSB=0,ELSA=0,??=0,??=0 */
		FTM_CnSC_REG(pFTM, 4) = 0x00U;

		/* Clear channel status and control register */
		/* FTM_C5SC: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,CHF=0,CHIE=0,MSB=0,MSA=0,ELSB=0,ELSA=0,??=0,??=0 */
		FTM_CnSC_REG(pFTM, 5) = 0x00U;
	}

	/* Set up modulo register */
	/* FTM_MOD: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,MOD=0x3E7F */
	//pFTM->MOD = FTM_MOD_MOD(0x3E7F);
	pFTM->MOD = FTM_MOD_MOD( u32clock/2/(1000000/u16periodUS) );

	if ( pFTM == FTM2 )
	{
		/* Set up Initial State for Channel Output register */
		/* FTM_OUTINIT: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,CH7OI=0,CH6OI=0,CH5OI=0,CH4OI=0,CH3OI=0,CH2OI=0,CH1OI=0,CH0OI=1 */
		pFTM->OUTINIT = FTM_OUTINIT_CH0OI_MASK;

		/* Initialize the Output Channels */
		/* FTM_MODE: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,FAULTIE=0,FAULTM=0,CAPTEST=0,PWMSYNC=0,WPDIS=1,INIT=1,FTMEN=0 */
		pFTM->MODE = (FTM_MODE_FAULTM(0x00) | FTM_MODE_WPDIS_MASK | FTM_MODE_INIT_MASK);
	}
}

void PWM_Enable(FTM_Type *pFTM, uint8_t channel)
{
	/* Set up channel status and control register */
	/* FTM_C0SC: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,CHF=0,CHIE=0,MSB=1,MSA=0,ELSB=1,ELSA=0,??=0,??=0 */
	FTM_CnSC_REG(pFTM, channel) = (FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK);

	/* Set up channel value register - 0 default duty--cycle*/
	/* FTM_C0V: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,VAL=0 */
	FTM_CnV_REG(pFTM, channel) = FTM_CnV_VAL( 0x00 );

	/* Initialize clocks, prescalers and other timer stuff */
	pFTM->SC |= 1 << 5;	// CPWMS (Center-Aligned PWM Select): 0 = counting up, 1 = counting down
	pFTM->SC |= 0 << 6;	// TOIE (Timer Overflow Interrupt Enable): 0 = disabled, 1 = enabled
	pFTM->SC |= 0 << 4;	// CLKS (Clock Source Selection) 4-3: 00 - No clock selected, 01 - System clock
	pFTM->SC |= 1 << 3;	// CLKS (Clock Source Selection) 4-3: 10 - Fixed frequency clock, 11 - External clock
	pFTM->SC |= 0 << 2;	// PS (Prescale Factor Selection):
	pFTM->SC |= 1 << 1;	// PS (Prescale Factor Selection):
	pFTM->SC |= 1 << 0;	// PS (Prescale Factor Selection):
//				321
//				000 Divide by 1
//				001 Divide by 2
//				010 Divide by 4
//				011 Divide by 8			V
//				100 Divide by 16
//				101 Divide by 32
//				110 Divide by 64
//				111 Divide by 128

//	/* Select PIN automatically */
//	if ( pFTM == FTM0 )
//	{
//		SIM_PINSEL |= ((channel == 0) ? (SIM_PINSEL_FTM0PS0_MASK) :( SIM_PINSEL_FTM0PS1_MASK));
//	}
//	else if ( pFTM == FTM1 )
//	{
//		SIM_PINSEL |= ((channel == 0) ? (SIM_PINSEL_FTM1PS0_MASK) : ( SIM_PINSEL_FTM1PS1_MASK));
//	}
//	else if ( pFTM == FTM2 )
//	{
//		if ( channel == 0 )
//		{
//			SIM_PINSEL &= (uint32_t)~(uint32_t)(SIM_PINSEL_FTM2PS0_MASK);
//		}
//		else if ( channel == 1 )
//		{
//			SIM_PINSEL &= (uint32_t)~(uint32_t)(SIM_PINSEL_FTM2PS1_MASK);
//		}
//		else if ( channel == 2 )
//		{
//			SIM_PINSEL &= (uint32_t)~(uint32_t)(SIM_PINSEL_FTM2PS2_MASK);
//		}
//		else if ( channel == 3 )
//		{
//			SIM_PINSEL &= (uint32_t)~(uint32_t)(SIM_PINSEL_FTM2PS3_MASK);
//		}
//		else if ( channel == 4 || channel == 5 )
//		{
//			// Not pinsel masks for this SKEAZN642 :(
//			;
//		}
//	}
}

void PWM_SetRatio16(FTM_Type *pFTM, uint8_t ftmChannel, uint16_t ratio)
{
	uint16_t period;
	uint16_t duty;

	// Retrieve period from FTM module
	period = (uint16_t)(pFTM->MOD) + 1;

	if ( period == 0U )
	{
		duty = ratio;
	}
	else
	{
		duty = (uint16_t)((((uint32_t)(period) * ratio) + 0x8000) >> 0x10);
	}

	// Write new duty
	FTM_CnV_REG(pFTM, ftmChannel) = (uint32_t)duty;
}