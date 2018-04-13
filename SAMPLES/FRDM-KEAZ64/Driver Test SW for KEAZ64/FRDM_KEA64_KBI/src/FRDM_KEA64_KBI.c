/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2014 Freescale Semiconductor, Inc.
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
* @file FRDM_KEA64_KBI.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief providing framework of demo cases for MCU.
*
* This example provides a template for the sample code. It demonstrates how to
* to configure KBI module.
* ISR happens when SW2 or SW3 is pressed toggling BLUE LED on PTE7.
*
*******************************************************************************/

#include "ics.h"
#include "uart.h"
#include "kbi.h"
#include "gpio.h"
#include "printf.h"

/******************************************************************************
* Global variables
******************************************************************************/

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
KBI_Type* pKBI1 = (KBI_Type*)KBI1;
/******************************************************************************
* Local functions
******************************************************************************/
void KBI1_Task(void);
/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)

{

	uint32_t	i;
	uint8_t     u8Ch;

    /* Perform processor initialization */
	ICS_ConfigType ICS_set={0};				/* Declaration of ICS_setup structure */
	ICS_set.u8ClkMode=ICS_CLK_MODE_FEI;
	ICS_set.bdiv=0;
	ICS_Init(&ICS_set);             		/* Initialization of Core clock at 48 MHz, Bus Clock at 24 MHz*/


	UART_ConfigType UART_Config={{0}};

	UART_Config.sctrl1settings.bits.bM=0;  	/* 8 bit mode*/
	UART_Config.sctrl1settings.bits.bPe=0;	/* No hardware parity generation or checking*/
	UART_Config.bSbns=0;					/* One stop bit*/
	UART_Config.sctrl2settings.bits.bRe=1;	/* Enable Receiver*/
	UART_Config.sctrl2settings.bits.bTe=1;	/* Enable Transmitter*/
	UART_Config.u32SysClkHz = 24000000;   	/* Bus clock in Hz*/
	UART_Config.u32Baudrate = 115200;     	/* UART baud rate */

	UART_Init(UART2,&UART_Config);			/* Initialization of UART utilities*/

	printf("\nRunning the FRDM_KEA64_KBI project.\n");

	KBI_ConfigType  KBIConfig={{0}};

	/* Disable all the KBI pins */
	for (i = 0; i < KBI_MAX_PINS_PER_PORT; i++)
	{
		KBIConfig.sPin[i].bEn	 = 0;
	}

	KBIConfig.sBits.bRstKbsp   = 1;			/* Writing a 1 to RSTKBSP is to clear the KBIxSP Register*/
	KBIConfig.sBits.bKbspEn   	= 1;		/* The latched value in KBxSP register while interrupt flag occur to be read.*/
	KBIConfig.sBits.bMode   	= KBI_MODE_EDGE_ONLY;
	KBIConfig.sBits.bIntEn  	= 1;

	/*Falling edge/low level select; KBI0_P24 channel enable(SW2 on FRDM+ board) */
	KBIConfig.sPin[4].bEdge = KBI_FALLING_EDGE_LOW_LEVEL;
	KBIConfig.sPin[4].bEn   = 1;

	/*Falling edge/low level select; KBI0_P25 channel enable(SW3 on FRDM+ board) */
	KBIConfig.sPin[5].bEdge = KBI_FALLING_EDGE_LOW_LEVEL;
	KBIConfig.sPin[5].bEn   = 1;

	KBI_SetCallback(pKBI1, &KBI1_Task);
	KBI_Init(pKBI1, &KBIConfig);


	CONFIG_PIN_AS_GPIO(PTE,PTE7,OUTPUT); 	/* Configure BLUE LED at PTE7 as an output */

	
    /* echo chars received from terminal */
	while(1)
	{
		u8Ch = UART_GetChar(UART2);
		UART_PutChar(UART2, u8Ch);
	}

}

/*****************************************************************************//*!
*
* @brief callback routine of KBI driver.
*
* @param  none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/

void KBI1_Task(void)
{
	OUTPUT_TOGGLE(PTE,PTE7);
	if(KBI_GetSP(pKBI1)== 0x0000010)
		{
			printf("\nSW2 pressed!\n");
		}
		else if(KBI_GetSP(pKBI1)== 0x0000020)
		{
			printf("\nSW3 pressed!\n");
		}
	KBI_RstSP(pKBI1);
	KBI_ClrFlags(pKBI1);
}


