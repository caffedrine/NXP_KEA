/******************************************************************************
*    LAB4 - Dims LEDs using Flex Timer(FTM) in  EPWM mode                     *
*    FRDM+KEA64 Board														  *
*                                                                             *
*    This lab demonstrates how to configure the FTM module				 	  *
*    to generate periodical interrupts to dim and brighten the LEDs			  *
*    periodically.													          *
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
uint16_t ChV_old, ChV_new;

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

	/* Initialize the FTM2 module in Edge Aligned Pulse Width Modulation mode */
	FTM2_Init_EPWM();
	
	/* Enable FTM2 interrupt */
	Enable_Interrupt(FTM2_IRQn);

	SIM_PINSEL1 |= SIM_PINSEL1_FTM2PS0(1); /* Select Pins corresponds to the PTE7 for output */

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

/*****************************************************************************//*!
*
* @brief FTM2_Interrupt(void). User Function called by the FTM module on each interrupt.
* 		 FTM2 CH0 will change the duty cycle periodically.
*
* @param  none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/
void FTM2_Interrupt(void)
{
    static uint16_t count;
    static uint8_t DirMark;

    /* clear the flag */
    FTM2_SC &= ~FTM_SC_TOF_MASK;

    if(100 == count)
    {
        count = 0;
        ChV_old = FTM2_C0V;
        if(!DirMark)
        {
            ChV_new = ChV_old + 200;
            if(ChV_new >= FTM2_MOD)
            {
                ChV_new = FTM2_MOD - 200;
                DirMark  = 1;
            }
            else
            {
            }
        }
        else
        {
            ChV_new = ChV_old - 200;
            if(ChV_new <  200)
            {
                ChV_new = 200;
                DirMark  = 0;
            }
            else
            {
            }
        }
        /* update the channel value */
        FTM2_C0V = ChV_new;
    }
    else
    {
        count++;
    }
}
/********************************************************************/
