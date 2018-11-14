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
* @file FRDM_KEA64_FTM_OutputCompare.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief Demo FTM output compare feature.
*        FTM2 CH0 generates toggled waveform on PTH0 connected to RED LED.
*        Counter value is printed every FTM interrupt on hyper terminal.
*
*******************************************************************************/
#include "ics.h"
#include "ftm.h"
#include "nvic.h"
#include "uart.h"
#include "printf.h"

/******************************************************************************
* Global variables
******************************************************************************/
uint32_t count;
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
void FTM_Int();
/******************************************************************************
* Global functions
******************************************************************************/

uint8_t flag;
/********************************************************************/
int main (void)
{
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

    	printf("\nRunning the FRDM_KEA64_FTM_OutputCompare project.\n");

    	FTM_ConfigType FTM2_Config={0};
    	FTM_ChParamsType FTM2CH0_Config={0};

    	FTM2_Config.modulo=65535;
    	FTM2_Config.clk_source=FTM_CLOCK_SYSTEMCLOCK;
    	FTM2_Config.prescaler=FTM_CLOCK_PS_DIV128;
    	FTM2CH0_Config.ctrl.bits.bMode=FTM_OUTPUT_COMPARE;
    	FTM2CH0_Config.ctrl.bits.bOutCmp=FTM_OUTPUT_TOGGLE;
    	FTM2CH0_Config.u16CnV=37500;			/* Set output toggle at 200 ms*/
    	FTM2CH0_Config.ctrl.bits.bCHIE=1;

       	FTM_SetCallback(pFTM2, FTM_Int);
    	FTM_Init(pFTM2,&FTM2_Config);
    	FTM_ChannelInit(pFTM2,0,FTM2CH0_Config);
    	SIM_PINSEL1 |= SIM_PINSEL1_FTM2PS0(1); 	/* Select Pin corresponds to the PTH0 for output */


  	while(1)
	{
	}

}

/*****************************************************************************//*!
*
* @brief callback routine of FTM2 driver. Print the current FTM counter value
*        every time the interrupt occurs.
*
* @param  none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/

void FTM_Int(){
	   FTM_Type* pFTM = (FTM_Type*)FTM2;
	   count=pFTM->CNT;
	   FTM_SetChannelValue(pFTM2,0,(count + 37500));
	   printf("\nFTM counter value.\n 0x%x ",(unsigned int)count);
	   FTM_ClrChannelFlag(pFTM2,FTM_CHANNEL_CHANNEL0);
}
