/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2013 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
***************************************************************************//*!
*
* @file FRDM_KEA64_FTM_Combine.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief FTM2 Ch0 and adjacent Ch1 are combined to generate PWM signal
*        on RED LED (PTH0). RED LED (PTH0) positive pulse width is of 4.5 ms.
*
*        BLUE LED (PTE7) will toggle every 100ms and at the start of PWM signal.
*
*        Use an oscilloscope to measure the PWM signal of RED LED (PTH0)
*
*
*******************************************************************************/
#include "ics.h"
#include "printf.h"
#include "ftm.h"
#include "uart.h"
#include "GPIO.h"

/******************************************************************************
* Global variables
******************************************************************************/
/**********************************************************************************************
* Constants and macros
**********************************************************************************************/

#define LED0_TOGGLE		OUTPUT_TOGGLE(PTH,PTH0)
#define LED1_TOGGLE		OUTPUT_TOGGLE(PTH,PTH1)
#define LED2_TOGGLE		OUTPUT_TOGGLE(PTE,PTE7)

#define LED0_OFF		OUTPUT_CLEAR(PTH,PTH0);
#define LED1_OFF		OUTPUT_CLEAR(PTH,PTH1);
#define LED2_OFF		OUTPUT_CLEAR(PTE,PTE7);

#define LED0_ON			OUTPUT_SET(PORT_H,PTH0);
#define LED1_ON			OUTPUT_SET(PORT_H,PTH1);
#define LED2_ON			OUTPUT_SET(PORT_E,PTE2);
/******************************************************************************
* Constants and macros
******************************************************************************/

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/
FTM_Type* pFTM0 = (FTM_Type*)FTM0;
FTM_Type* pFTM1 = (FTM_Type*)FTM1;
FTM_Type *pFTM2 = (FTM_Type*)FTM2;
/******************************************************************************
* Local functions
******************************************************************************/
//void RTC_Task(void);
void FTM2_Task(void);
void FTM1_Task(void);
/******************************************************************************
* Global functions
******************************************************************************/
/***********************************************************************************************
*
* @brief    GPIO_Init - Initialize the LEDs as outputs
* @param    none
* @return   none
*
************************************************************************************************/
void GPIO_Init()
{
	    CONFIG_PIN_AS_GPIO(PTH,PTH0,OUTPUT); /* Configure RED as an output */
		CONFIG_PIN_AS_GPIO(PTH,PTH1,OUTPUT); /* Configure GREEN as an output */
		CONFIG_PIN_AS_GPIO(PTE,PTE7,OUTPUT); /* Configure BLUE as an output */

		CONFIG_PIN_AS_GPIO(PTE,PTE4,INPUT); /* Configure BTN0 as an input */
		CONFIG_PIN_AS_GPIO(PTE,PTE5,INPUT); /* Configure BTN1 as an input */
		ENABLE_INPUT(PTE,PTE4);			 	/* Enable input BTN0*/
		ENABLE_INPUT(PTE,PTE5);				/*  Enable input BTN1*/

		LED0_OFF;							/* Turn off RED */
		LED1_OFF;							/* Turn off GREEN */
		LED2_OFF;							/* Turn off BLUE */

}


/********************************************************************/
int main (void)
{
	GPIO_Init();
	uint8_t u8Ch;
	/* Perform processor initialization */
		ICS_ConfigType ICS_set={0};				/* Declaration of ICS_setup structure */
		ICS_set.u8ClkMode=ICS_CLK_MODE_FEI;
		ICS_set.bdiv=0;
		ICS_Init(&ICS_set);             		/* Initialization of Core clock at 48 MHz, Bus clock at 24MHz */


		UART_ConfigType UART_Config={{0}};

		UART_Config.sctrl1settings.bits.bM=0;  	/* 8 bit mode*/
		UART_Config.sctrl1settings.bits.bPe=0;	/* No hardware parity generation or checking*/
		UART_Config.bSbns=0;					/* One stop bit*/
		UART_Config.sctrl2settings.bits.bRe=1;	/* Enable Receiver*/
		UART_Config.sctrl2settings.bits.bTe=1;	/* Enable Transmitter*/
		UART_Config.u32SysClkHz = 24000000;   	/* Bus clock in Hz*/
		UART_Config.u32Baudrate = 115200;     	/* UART baud rate */

		UART_Init(UART2,&UART_Config);			/*Initialization of UART utilities*/

		printf("\nRunning the FRDM_KEA64_FTM_Combine project.\n");

		FTM_ConfigType FTM2_Config={0},
					   FTM1_Config={0};

		FTM_ChParamsType FTM2CH0_Config={0},
		 	 	 	 	 FTM2CH1_Config={0},
		 	 	 	 	 FTM1CH1_Config={0};


		FTM2_Config.modulo=3749;
		FTM2_Config.combine=FTM_COMBINE_COMBINE0_MASK|FTM_COMBINE_SYNCEN0_MASK;
		FTM2_Config.clk_source=FTM_CLOCK_SYSTEMCLOCK;
		FTM2_Config.prescaler=FTM_CLOCK_PS_DIV128;
		FTM2_Config.toie=1;

		FTM1_Config.modulo=37490;
		FTM1_Config.combine=FTM_COMBINE_COMBINE0_MASK|FTM_COMBINE_SYNCEN0_MASK;
		FTM1_Config.clk_source=FTM_CLOCK_SYSTEMCLOCK;
		FTM1_Config.prescaler=FTM_CLOCK_PS_DIV128;
		FTM1_Config.toie=1;


		FTM2CH0_Config.ctrl.bits.bMode=FTM_PWMMODE_COMBINE;
		FTM2CH0_Config.ctrl.bits.bPWMPol=FTM_PWM_HIGHTRUEPULSE;
		FTM2CH0_Config.u16CnV=1687;


		FTM2CH1_Config.ctrl.bits.bMode=FTM_PWMMODE_COMBINE;
		FTM2CH1_Config.u16CnV=2062;


		FTM1CH1_Config.ctrl.bits.bMode=FTM_OUTPUT_COMPARE;
		FTM1CH1_Config.ctrl.bits.bOutCmp=FTM_OUTPUT_TOGGLE;
		FTM1CH1_Config.u16CnV=37490;

	    FTM_SetCallback(pFTM2, FTM2_Task);
	    FTM_SetCallback(pFTM1, FTM1_Task);
	    FTM_ChannelInit(pFTM2,0,FTM2CH0_Config);
		FTM_ChannelInit(pFTM2,1,FTM2CH1_Config);
		FTM_ChannelInit(pFTM1,1,FTM1CH1_Config);
		SIM_PINSEL0 |= SIM_PINSEL_FTM1PS1_MASK;	/* Select Pin corresponds to the PTE7 for output */
		SIM_PINSEL1 |= SIM_PINSEL1_FTM2PS0(1); 	/* Select Pins corresponds to the PTH0 for output */

		FTM_Init(pFTM1,&FTM1_Config);   			/* Output toggle at every 100 ms */
	    FTM_Init(pFTM2,&FTM2_Config);   			/* Generate PWM signal */


	while(1)
	{
		u8Ch = UART_GetChar(UART2);
		UART_PutChar(UART2,u8Ch);


	}
}


/*****************************************************************************//*!
*
* @brief callback routine of FTM driver.
*
* @param  none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/
void FTM2_Task(void)
{

    FTM_ClrOverFlowFlag(pFTM2);

}

void FTM1_Task(void)
{

    FTM_ClrOverFlowFlag(pFTM1);

}
/********************************************************************/

