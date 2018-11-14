/*
 * ics.c
 *
 *  Created on: Nov 6, 2015
 *      Author: B48683
 */
/***************************************************************
 * Custom writing this file for this demo because the SDK macros conflict with the MSF Arduino library and the MSF
 * library does not have a clock initialization function.
 **************************************************************/
#include "ics.h"
#include "derivative.h"

/* This function initializes ICS so that ICSIRCLK is enabled and ICSOUTCLK follows FLL whose source is the IRC
 * Busclk=ICSOUTCLK/2.  Core at 48 MHz and Busclk at 24MHz
 */
void ICS_Init_FEI(){

	ICS_C1 |= ICS_C1_IREFS_MASK; //Select the internal reference clock as source for FLL
	ICS_C1 |= ICS_C1_CLKS(0); //Select the FLL output as ICS source
	ICS_C1 |= ICS_C1_IRCLKEN_MASK; //Enable the output ICSIRCLK
	/* Leave ICS_C2_BDIV=0b001 (default), meaning bus clock divided by 2. */

	ICS_S |= ICS_S_LOLS_MASK; //Clear the LOLS flag in case it has been set. W1C
	while(ICS_S & ICS_S_LOLS_MASK); //Wait until LOLS mask actually clears. Will stay in this while loop as long as ICS_S_LOLS=1
}

