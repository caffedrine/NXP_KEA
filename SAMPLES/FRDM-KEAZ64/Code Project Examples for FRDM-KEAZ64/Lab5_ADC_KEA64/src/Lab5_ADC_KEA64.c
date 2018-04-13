/******************************************************************************
*    LAB5 - Read ADC Port 		                          					  *
*    FRDM+KEA64 Board														  *
*                                                                             *
*    This lab demonstrates how to use the ADC module to read the data at ADC  *
*    port(potentiometer) and change the LED colors accordingly				  *
*    different values read													  *
******************************************************************************/


/**********************************************************************************************
* External objects
**********************************************************************************************/
#include "derivative.h" /* include peripheral declarations */
#include "CLK.h"
#include "GPIO.h"
#include "ADC.h"
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
void ADC_Interrupt(void);

/**********************************************************************************************
* Local variables
**********************************************************************************************/
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

	/* Initialize the ADC module and read at Channel 10 and 8bit conversion mode is imposed */
	ADC_Init(10,0);

	/* Enable FTM2 interrupt */
	Enable_Interrupt(ADC_IRQn);

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
* @brief    ADC_IRQHandler - Interrupt for ADC channels, Calls user function.
* @param    none
* @return   none
*
************************************************************************************************/
void ADC_IRQHandler()
{
	ADC_Interrupt();
}

/***********************************************************************************************
*
* @brief    ADC_Interrupt(void). User Function called by the ADc module on each interrupt
* @param    none
* @return   none
*
************************************************************************************************/
void ADC_Interrupt(void)
{


	adc_value=ADC_Read(10);

	if(adc_value >=0 && adc_value <=85)
	{
		LED0_ON; 								/* Set Red LED */
		LED1_OFF; 								/* Clear Green LED */
		LED2_OFF; 								/* Clear Blue LED */
	}

	else if(adc_value >85 && adc_value <=170)
	{
		LED0_OFF; 								/* Clear Red LED */
		LED1_ON; 								/* Set Green LED */
		LED2_OFF; 								/* Clear Blue LED */
	}

	else if(adc_value >170 && adc_value <=255)
	{
		LED0_OFF; 								/* Clear Red LED */
		LED1_OFF; 								/* Clear Green LED */
		LED2_ON; 								/* Set Blue LED */
	}
}
