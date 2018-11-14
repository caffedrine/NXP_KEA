/*
 * FTM.c
 *
 *  Created on: Sep 24, 2013
 *      Author: B46911
 */

#include "FTM.h"

/**********************************************************************************************
* External objects
**********************************************************************************************/


/**********************************************************************************************
* Global variables
**********************************************************************************************/


/**********************************************************************************************
* Constants and macros
**********************************************************************************************/


/**********************************************************************************************
* Local types
**********************************************************************************************/


/**********************************************************************************************
* Local function prototypes
*********************************************************************************************/


/**********************************************************************************************
* Local variables
**********************************************************************************************/



/**********************************************************************************************
* Local functions
**********************************************************************************************/


/**********************************************************************************************
* Global functions
**********************************************************************************************/
/***********************************************************************************************
*
* @brief    FTM_Init - Initialize the FTM counter and enable 1 interrupt (CH0)
* @param    none
* @return   none
*
************************************************************************************************/
void FTM2_Init_Output_Cmp()
{

	
	SIM_SCGC |= SIM_SCGC_FTM2_MASK; /* Enable Clock for FTM2 */
	
	
	FTM2_SC |= FTM_SC_PS(7);	/* Select Preescaler in this case 128. 20 Mhz /128 =156.25 Khz. */ 
								/* Counter increase by one every 6.4 us */
	
	/* Enable Channle 0*/
	FTM2_C0SC |= FTM_CnSC_CHIE_MASK; /* Enable channel 0 interrupt */
	FTM2_C0SC |= FTM_CnSC_MSA_MASK;  /* Channel as Output compare mode */
	
	/*Select interrupt frequency*/
	FTM2_C0V = 46875;
	FTM2_SC |= FTM_SC_CLKS(1); /*FTM2 use system clock*/		
	 
}

/***********************************************************************************************
*
* @brief    FTM_Init - Initialize the FTM counter in EPWM mode and enable 1 interrupt (CH0)
* @param    none
* @return   none
*
************************************************************************************************/
void FTM2_Init_EPWM()
{

	SIM_SCGC |= SIM_SCGC_FTM2_MASK; 	/* Enable Clock for FTM2 */

	FTM2_MOD = 9999;
	FTM2_SC = 0;
	FTM2_SC |= FTM_SC_PS(0);			/* Select Prescaler in this case 1. So no division of 20 Mhz */
	FTM2_SC |= FTM_SC_TOIE_MASK;		/* Timer overflow interrupt Enable */

	/* Enable Channle 0*/
	FTM2_C0SC |= FTM_CnSC_MSB_MASK;  	/* Channel as PWM mode */
	FTM2_C0SC |= FTM_CnSC_ELSB_MASK;  	/* Channel as PWM mode */

	/*Select interrupt frequency*/
	FTM2_C0V = 1000;

	FTM2_SC |= FTM_SC_CLKS(1); 			/*FTM2 use system clock*/

}

/***********************************************************************************************
*
* @brief    FTM2_SetPeriod - Set the period of the periodic interrupt
* @param    ch - channel to configure, period - period to set
* @return   none
*
************************************************************************************************/
void FTM2_SetPeriod(uint8_t ch, uint32_t period)
{

	FTM2_C0V = FTM_CnV_VAL(period) ;

}

