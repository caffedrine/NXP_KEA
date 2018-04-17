/******************************************************************************
*    Lab1 - Blink LEDs using simple loops										*
*    FRDM+KEA64 Board							                            	*
*                                                                             	*
*    This lab demonstrates how to configure CLK in FEI,							*
*    use GPIO and Loops to generates Delays.									*
*    Use thEM to generate blinking of different LED colors at 					*
*    different time-intervals								               		*
******************************************************************************/

#include "derivative.h" /* include peripheral declarations SKEAZ128M4 */
#include "CLK.h"

#define PTH0 24 					/* For RED LED */
#define PTH1 25 					/* For GREEN LED */
#define PTE7 7 						/* For BLUE LED */

void delay(void);					/* Generate Delay */

int main(void)
{
	/* Initialize the RED, GREEN and BLUE LEDs as Output */
	GPIOB_PDDR |= 1<<PTH0; 			/* Setup PTH0 as an output for RED LED */
	GPIOB_PDDR |= 1<<PTH1; 			/* Setup PTH1 as an output for GREEN LED */
	GPIOB_PDDR |= 1<<PTE7; 			/* Setup PTE7 as an output for BLUE LED */

	/* Initialize Clock to 40MHz, Bus Clock to 20MHz  */
	Clk_Init();

	/* Blink LEDs using Delays */
	while(1)
	{
	   	GPIOB_PCOR |= 1<<PTH0; 		/* Clear output port PTH0 */
	   	GPIOB_PSOR |= 1<<PTH1; 		/* Set output port PTH1 */
	   	GPIOB_PSOR |= 1<<PTE7; 		/* Set output port PTE7 */
	   	delay(); 					/* Generate delay */

	   	GPIOB_PSOR |= 1<<PTH0; 		/* Set output port PTH0 */
	   	GPIOB_PCOR |= 1<<PTH1; 		/* Clear output port PTH1 */
	   	GPIOB_PSOR |= 1<<PTE7; 		/* Set output port PTE7 */
	   	delay(); 					/* Generate delay */

	   	GPIOB_PSOR |= 1<<PTH0; 		/* Set output port PTH0*/
	   	GPIOB_PSOR |= 1<<PTH1; 		/* Set output port PTH1*/
	   	GPIOB_PCOR |= 1<<PTE7; 		/* Clear output port PTE7*/
	   	delay(); 					/* Generate delay */

	}
   	return 0;
}

/* Generate Delay using nested loops */
void delay(void)
{
	uint32_t i,j;
	for(i=0 ; i<8 ; i++)
		for(j=0 ; j<65535 ; j++);
}
