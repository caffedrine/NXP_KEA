/******************************************************************************
*    LAB2 - Blink LEDs using Flex Timer(FTM) 		                          *
*    FRDM+KEA64 Board														  *
*                                                                             *
*    This lab demonstrates how to configure GPIO and use the FTM module 	  *
*    to generate periodical interrupts to blink the LEDs 					  *
*    at different time intervals									          *
******************************************************************************/


/**********************************************************************************************
* External objects
**********************************************************************************************/
#include "derivative.h" /* include peripheral declarations */
#include "CLK.h"
#include "GPIO.h"
#include "FTM.h"
/**********************************************************************************************
* Global variables
**********************************************************************************************/


/**********************************************************************************************
* Constants and macros
**********************************************************************************************/
#define PTH0 24 								/* For RED LED */
#define PTH1 25 								/* For GREEN LED */
#define PTE7 7  								/* For BLUE LED */
#define PTE4 4  								/* For SW2 - BTN0 */
#define PTE5 5  								/* For SW3 - BTN1 */

#define LED0_TOGGLE		OUTPUT_TOGGLE(H1,PTH0)
#define LED1_TOGGLE		OUTPUT_TOGGLE(H1,PTH1)
#define LED2_TOGGLE		OUTPUT_TOGGLE(E1,PTE7)

#define LED0_OFF		OUTPUT_CLEAR(H1,PTH0);
#define LED1_OFF		OUTPUT_CLEAR(H1,PTH1);
#define LED2_OFF		OUTPUT_CLEAR(E1,PTE7);

#define LED0_ON			OUTPUT_SET(H1,PTH0);
#define LED1_ON			OUTPUT_SET(H1,PTH1);
#define LED2_ON			OUTPUT_SET(E1,PTE7);


/**********************************************************************************************
* Local types
**********************************************************************************************/


/**********************************************************************************************
* Local function prototypes
*********************************************************************************************/
void Enable_Interrupt(uint8_t vector_number);
void FTM2_Interrupt(void);

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
* @brief    GPIO_Init - Initialize the LEDs as outputs, SWs as inputs.
* @param    none
* @return   none
*
************************************************************************************************/
void GPIO_Init()
{
	CONFIG_PIN_AS_GPIO(H1,PTH0,OUTPUT); 		/* Configure RED LED as an output */
	CONFIG_PIN_AS_GPIO(H1,PTH1,OUTPUT); 		/* Configure GREEN LED as an output */
	CONFIG_PIN_AS_GPIO(E1,PTE7,OUTPUT); 		/* Configure BLUE LED as an output */

	CONFIG_PIN_AS_GPIO(E1,PTE4,INPUT); 			/* Configure BTN0 as an input */
	CONFIG_PIN_AS_GPIO(E1,PTE5,INPUT); 			/* Configure BTN1 as an input */
	ENABLE_INPUT(E1,PTE4);			 			/* Enable input BTN0*/
	ENABLE_INPUT(E1,PTE5);						/* Enable input BTN1*/

	LED0_OFF;									/* Turn off RED */
	LED1_OFF;									/* Turn off GREEN */
	LED2_OFF;									/* Turn off BLUE */
}


/***********************************************************************************************
*
* @brief    main() - Program entry function
* @param    none
* @return   none
*
************************************************************************************************/
int main(void)
{
	/* Initialize Core Clock to 40MHz, Bus Clock to 20MHz  */
	Clk_Init();

	/* Configure button pins as inputs and LED pins as outputs */
	GPIO_Init();

	/* Initialize the FTM2 module in Output Compare mode,
	 * and interrupt at every 46875 counts, equal to every 300 ms */
	FTM2_Init_Output_Cmp();

	/* Enable FTM2 interrupt */
	Enable_Interrupt(FTM2_IRQn);

	while(1);
	
	return 0;
}


/***********************************************************************************************
*
* @brief    Enable_Interrupt(uint8_t vector_number). Enable interrupts from desired module.
* @param    Module interrupt number from the interrupts vector table
* @return   none
*
************************************************************************************************/
void Enable_Interrupt(uint8_t vector_number)
{

	NVIC_ClearPendingIRQ(vector_number); 		/* Clear pending interrupt register */
	NVIC_EnableIRQ(vector_number); 				/* Enable required Interrupt */

}

/***********************************************************************************************
*
* @brief    FTM2_IRQHandler - Interrupt for FTM channels, Call user function.
* @param    none
* @return   none
*
************************************************************************************************/
void FTM2_IRQHandler()
{
	FTM2_Interrupt();
}

/***********************************************************************************************
*
* @brief    FTM2_Interrupt(void). User Function called by the FTM module on each interrupt
* @param    none
* @return   none
*
************************************************************************************************/
void FTM2_Interrupt(void)
{
	(void)FTM2_C0SC;  							/* Read to clear flag */
	FTM2_C0SC ^= FTM_CnSC_CHF_MASK;  			/* Clear flag */
	FTM2_C0V = FTM2_C0V + 46875 ; 				/* Refresh interrupt period */

	static uint8_t color = 0;
	switch(color)
	{
	case 0:
		LED0_ON;
		LED1_OFF;
		LED2_OFF;
		color = 1;
		break;

	case 1:
		LED0_OFF;
		LED1_ON;
		LED2_OFF;
		color = 2;
	break;

	case 2:
		LED0_OFF;
		LED1_OFF;
		LED2_ON;
		color = 0;
	break;
	}

}
