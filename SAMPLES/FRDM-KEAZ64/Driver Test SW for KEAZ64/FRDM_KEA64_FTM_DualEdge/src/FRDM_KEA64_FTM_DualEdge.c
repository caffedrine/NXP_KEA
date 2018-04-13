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
* @file FRDM_KEA64_FTM_DualEdge.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief Note: First See Output Compare mode example.
* 		 Demonstrate FTM dual edge capture feature.
*        First connect FTM0 CH0 (PTA0 - J4_13) with FTM2 CH4 (PTG6 - J1_13).
*        FTM0 CH0 generates toggled waveform, the period is 12000 FTM counts (2ms),
*        positive pulse width is 6000 (1ms),
*        as the input of FTM2 CH4 and CH5.
*        FTM2 captures both rising/falling edge of the input waveform,
*        and print the counts and period value in the hyperterminal.
*
*
*******************************************************************************/
#include "ics.h"
#include "ftm.h"
#include "uart.h"
#include "printf.h"

/******************************************************************************
* Global variables
******************************************************************************/
uint16_t period;
uint16_t counts;
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
FTM_Type* pFTM2 = (FTM_Type*)FTM2;
/******************************************************************************
* Local functions
******************************************************************************/
void FTM2_Task(void);
/******************************************************************************
* Global functions
******************************************************************************/

uint16_t u16Ch0Value,u16Ch1Value;

/********************************************************************/
int main (void)
{
	/* Perform processor initialization */
		ICS_ConfigType ICS_set={0};				/* Declaration of ICS_setup structure */
		ICS_set.u8ClkMode=ICS_CLK_MODE_FEI;
		ICS_set.bdiv=0;
		ICS_Init(&ICS_set);             		/* Initialization of Core clock at 48 MHz, Bus clock at 24 MHz */


		UART_ConfigType UART_Config={{0}};

		UART_Config.sctrl1settings.bits.bM=0;  	/* 8 bit mode*/
		UART_Config.sctrl1settings.bits.bPe=0;	/* No hardware parity generation or checking*/
		UART_Config.bSbns=0;					/* One stop bit*/
		UART_Config.sctrl2settings.bits.bRe=1;	/* Enable Receiver*/
		UART_Config.sctrl2settings.bits.bTe=1;	/* Enable Transmitter*/
		UART_Config.u32SysClkHz = 24000000;   	/* Bus clock in Hz*/
		UART_Config.u32Baudrate = 115200;     	/* UART baud rate */

		UART_Init(UART2,&UART_Config);			/*Initialization of UART utilities*/

		printf("\nRunning the FRDM_KEA64_FTM_DualEdge project.\n");

		FTM_ConfigType FTM2_Config={0},
					   FTM0_Config={0};

		FTM_ChParamsType FTM0CH0_Config={0},
						 FTM2CH4_Config={0},
						 FTM2CH5_Config={0};

	    //SIM->PINSEL1 |= SIM_PINSEL1_FTM2PS5_MASK;
	    //SIM->PINSEL1 |= SIM_PINSEL1_FTM2PS4_MASK;

		FTM2_Config.modulo=0xFFFF;
		FTM2_Config.clk_source=FTM_CLOCK_SYSTEMCLOCK;
		FTM2_Config.prescaler=FTM_CLOCK_PS_DIV8;
		FTM2_Config.combine= FTM_COMBINE_DECAP2_MASK|FTM_COMBINE_DECAPEN2_MASK; /* Enable dual edge capture in CH4 and CH5*/


		FTM2CH4_Config.ctrl.bits.bMode=FTM_INPUTCAPTURE_DUALEDGE;
		FTM2CH4_Config.ctrl.bits.bDualCapMode=FTM_INPUTCAPTURE_DUALEDGE_ONESHOT;
		FTM2CH4_Config.ctrl.bits.bEdge=FTM_INPUTCAPTURE_DUALEDGE_RISINGEDGE;


		FTM2CH5_Config.ctrl.bits.bMode=FTM_INPUTCAPTURE_DUALEDGE;
		FTM2CH5_Config.ctrl.bits.bDualCapMode=FTM_INPUTCAPTURE_DUALEDGE_ONESHOT;
		FTM2CH5_Config.ctrl.bits.bEdge=FTM_INPUTCAPTURE_DUALEDGE_FALLINGEDGE;
		FTM2CH5_Config.ctrl.bits.bCHIE=1;

		FTM0_Config.modulo=5999;
		FTM0_Config.clk_source=FTM_CLOCK_SYSTEMCLOCK;
		FTM0_Config.prescaler=FTM_CLOCK_PS_DIV8;

		FTM0CH0_Config.ctrl.bits.bMode=FTM_OUTPUT_COMPARE;
		FTM0CH0_Config.ctrl.bits.bOutCmp=FTM_OUTPUT_TOGGLE;
		FTM0CH0_Config.u16CnV=5999;

		FTM_SetCallback(pFTM2, FTM2_Task);
		FTM_ChannelInit(pFTM0,0,FTM0CH0_Config);
	    FTM_ChannelInit(pFTM2,4,FTM2CH4_Config);
	    FTM_ChannelInit(pFTM2,5,FTM2CH5_Config);
	    FTM_Init(pFTM2,&FTM2_Config);
		FTM_Init(pFTM0,&FTM0_Config);

	while(1)
	{
	}
}

/*****************************************************************************//*!
*
* @brief callback routine of FTM2 driver
*
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void FTM2_Task(void)
{

    FTM_ClrChannelFlag(pFTM2, FTM_CHANNELPAIR2);
    FTM_ClrChannelFlag(pFTM2, FTM_CHANNELPAIR2+1);

	FTM_MemMapPtr pFTM = (FTM_MemMapPtr)FTM2;
	u16Ch0Value = pFTM->CONTROLS[4].CnV;
	u16Ch1Value = pFTM->CONTROLS[5].CnV;
	counts= (u16Ch1Value - u16Ch0Value);
	period=(((.000000167)*(counts))*(1000))*(2);   /* Operation to get the period of the output */
	printf("\n Dual edge capture end. The input counts are %d\n",(counts));
	printf("\n The input wave period is %i ms\n",period);
	pFTM->COMBINE |=  FTM_COMBINE_DECAP2_MASK |FTM_COMBINE_DECAPEN2_MASK;
}
/********************************************************************/
