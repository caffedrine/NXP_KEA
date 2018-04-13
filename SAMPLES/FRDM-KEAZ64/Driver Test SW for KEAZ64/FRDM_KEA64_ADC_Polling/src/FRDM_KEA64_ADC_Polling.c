
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
* @file FRDM_KEA64_ADC_Polling.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief providing framework of demo cases for MCU.
*
*This demo is continuously reading the Potentiometer
*This but wait until the user press any key
*This values are shown on the screen through serial communication
*******************************************************************************/


#include "ics.h"
#include "uart.h"
#include "adc.h"
#include "printf.h"
#include "derivative.h"

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

/******************************************************************************
* Local functions
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{
	/* Perform processor initialization */
 	ICS_ConfigType ICS_set={0};					/* Declaration of ICS_setup structure */
	ICS_set.u8ClkMode=ICS_CLK_MODE_FEI;
	ICS_set.bdiv=0;
	ICS_Init(&ICS_set);             			/* Initialization of Core clock at 48 MHz, Bus clock at 24 MHz */


	UART_ConfigType UART_Config={{0}};

	UART_Config.sctrl1settings.bits.bM=0;  		/* 8 bit mode*/
	UART_Config.sctrl1settings.bits.bPe=0;		/* No hardware parity generation or checking*/
	UART_Config.bSbns=0;						/* One stop bit*/
	UART_Config.sctrl2settings.bits.bRe=1;		/* Enable Receiver*/
	UART_Config.sctrl2settings.bits.bTe=1;		/* Enable Transmitter*/
	UART_Config.u32SysClkHz = 24000000;   		/* Bus clock in Hz*/
	UART_Config.u32Baudrate = 115200;     		/* UART baud rate */

	UART_Init(UART2,&UART_Config);				/* Initialization of UART utilities*/
	printf("\n\nRunning the FRDM_KEA64_ADC_Polling project.\n\n");

    ADC_ConfigType  ADC_Config={{0}};

    /* Initialization of ADC module */
    ADC_Config.u8ClockDiv = ADC_ADIV_DIVIDE_4;
    ADC_Config.u8ClockSource = CLOCK_SOURCE_BUS_CLOCK;
    ADC_Config.u8Mode = ADC_MODE_12BIT;
    ADC_Config.sSetting.bContinuousEn=1;
    ADC_Config.u16PinControl= 0x0400; 			/* Disable I/O control on ADC channel 10*/
    ADC_Init( ADC, &ADC_Config);

	while(1)
	{
	   ADC_PollRead(ADC,ADC_CHANNEL_AD10);
	   printf("\nPOTENTIOMETER conversion value:0x%x\n",ADC_PollRead(ADC,ADC_CHANNEL_AD10));
       printf("Input any character to start a new conversion!\n\n\n");
       UART_GetChar(UART2);

	}

}


