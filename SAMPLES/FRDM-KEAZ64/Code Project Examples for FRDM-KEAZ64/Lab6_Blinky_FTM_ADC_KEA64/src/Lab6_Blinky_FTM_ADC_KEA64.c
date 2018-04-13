/******************************************************************************
*    LAB6 - FTM & ADC Module						                          *
*    FRDM+KEA64 Board														  *
*                                                                             *
*    This lab demonstrates how to use the ADC module to read the analog		  *
*    value of the on-board potentiometer and use FTM to blink LEDs.			  *
*    The application reads the potentiometer, ADC channel 10.                 *
*    LED will change color and speed according to the ADC value.              *
*    FLEX timer is used for that.                                             *
******************************************************************************/



/**********************************************************************************************
* External objects
**********************************************************************************************/
#include "derivative.h" /* include peripheral declarations */
#include "CLK.h"
#include "GPIO.h"
#include "ADC.h"
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
void FTM2_Interrupt();
uint32_t ADC_FTM_ValueFunction();

/**********************************************************************************************
* Local variables
**********************************************************************************************/
uint8_t color=0;
uint8_t adc_value = 0;

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
	FTM2_SetPeriod(0, 15625);		/* Interrupt every 15625 counts,  equal to every 100 ms */

	/* Initialize the ADC module and read at Channel 10 and 8bit conversion mode is imposed */
	ADC_Init(10,EIGHT_BIT);	/* Configure ADC channel 3 in 8-bit resolution mode */

	/* Enable FTM interrupt */
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
* @brief    FTM2_Interrupt() - User Function called by the FTM2 module on each timer interrupt.
* @param    none
* @return   none
*
************************************************************************************************/
void FTM2_Interrupt()
{

	(void)FTM2_C0SC;  								/* Read to clear flag */
	FTM2_C0SC ^= FTM_CnSC_CHF_MASK;  				/* Clear flag */
	uint32_t FLEX_value = ADC_FTM_ValueFunction();  /* ADC handler */
	FTM2_C0V = FTM2_C0V + FTM_CnV_VAL(FLEX_value) ; /* Refresh interrupt period */

	switch(color)
	{
	case 0:
		LED0_TOGGLE;
	break;

	case 1:
		LED1_TOGGLE;
	break;

	case 2:
		LED2_TOGGLE;
	break;
	}
}

/***********************************************************************************************
*
* @brief    ADC_FTM_ValueFunction() - to determine Interrupt period according to ADC value
* @param    none
* @return   none
*
************************************************************************************************/

uint32_t ADC_FTM_ValueFunction()
{

	static uint32_t FLEX_value = 0;

	adc_value=ADC_Read(10); /* Read ADC value from channel 3 */
	if (adc_value >=0 && adc_value < 85)
	{
		LED1_OFF;
		LED2_OFF;
		color=0; //RED
		if (adc_value >= 0 && adc_value < 28)
		{
			FLEX_value = 15625; /* Refresh interrupt period */
		}
		else if (adc_value >= 28 && adc_value < 56)
		{
			FLEX_value = 31250; /* Refresh interrupt period */
		}
		else if (adc_value >= 56  &&  adc_value < 85)
		{
			FLEX_value = 46875; /* Refresh interrupt period */
		}

	}
	else if (adc_value >= 85 && adc_value < 170)
	{
		LED0_OFF;
		LED2_OFF;
		color=1; //GREEN;
		if (adc_value >= 85 && adc_value < 113)
		{
			FLEX_value = 15625; /* Refresh interrupt period */
		}
		else if (adc_value >= 113 && adc_value < 141)
		{
			FLEX_value = 31250; /* Refresh interrupt period */
		}
		else if (adc_value >= 141 && adc_value < 170)
		{
			FLEX_value = 46875; /* Refresh interrupt period */
		}

	}
	else if (adc_value >= 170 && adc_value <= 255)
	{
		LED0_OFF;
		LED1_OFF;
		color = 2; //BLUE
		if (adc_value >= 170 && adc_value < 198)
		{
			FLEX_value = 15625; /* Refresh interrupt period */
		}
		else if (adc_value >= 198 && adc_value < 226)
		{
			FLEX_value = 31250; /* Refresh interrupt period */
		}
		else if (adc_value >= 226 && adc_value <= 255)
		{
			FLEX_value = 46875; /* Refresh interrupt period */
		}
	}

	return FLEX_value;
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
