/*
 * KBI.c
 *
 *  Created on: Sep 26, 2013
 *      Author: B55457
 */

#include "KBI.h"

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
* @brief    KBI_Init - Enable Keyboard interrupts on channels 4 and 5 (SW2 and SW3)
* @param    none
* @return   none
*
************************************************************************************************/  
void KBI_Init()
{
	
	SIM_SCGC |=  SIM_SCGC_KBI1_MASK; /* Enable bus clock on KBI1 */
	KBI1_SC = 0;					 /* Clearing mechanism */
	KBI1_ES |= KBI_ES_KBEDG(0x10);   /* Polarity setting, rising edge high level ,SW2 */
	KBI1_ES |= KBI_ES_KBEDG(0x20);   /* Polarity setting, rising edge high level,SW3 */
	KBI1_PE |= KBI_PE_KBIPE(0x10);   /* Enable KBI1 channel 4 , SW2 */
	KBI1_PE |= KBI_PE_KBIPE(0x20);   /* Enable KBI1 channel 5 , SW3 */
	KBI1_SC = 0;  				  	 /* Clearing flags*/
	KBI1_SC|=KBI_SC_RSTKBSP_MASK;	 /* Clearing RSTKBSP bit*/
	KBI1_SC |= KBI_SC_KBIE_MASK;  	 /* Enable  KBI1 Interrupts */

}

