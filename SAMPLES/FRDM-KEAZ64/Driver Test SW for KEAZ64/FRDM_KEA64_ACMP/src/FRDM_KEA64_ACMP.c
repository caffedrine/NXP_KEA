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
* @file FRDM_KEA64_ACMP.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief providing framework of demo cases for MCU.
* Changing the voltage level of ACMP external pin0 - PTA6 (J5_7) between VDD (J5_10) and GND (J5_12),
* ACMP ISR happens.
* Open an hyper terminal console and set it at 115200 baud rate.
* Every time that  the external voltage on PTA6 crosses the 2.5V threshold, a "Callback happens" message
* will appear on the hyper terminal window.
*
*******************************************************************************/

#include "derivative.h"
#include "acmp.h"
#include "ics.h"
#include "uart.h"
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

/******************************************************************************
* Local functions
******************************************************************************/
int main (void);
void ACMP_ISR(void);

/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{


    /* Perform processor initialization */
  	ICS_ConfigType ICS_set;						/* Declaration of ICS_setup structure */
  	ICS_set.u8ClkMode=ICS_CLK_MODE_FEI;			/* ICS on FEI mode*/
    ICS_set.bdiv=0;								/* BDIV divider equal to 1*/
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

  	printf("\nRunning the FRDM_KEA64_ACMP project.\n");

    ACMP_ConfigType ACMP_Config={{0}};	/* Declaration of ACMP config structure */
    /* init ACMP config parameter */
    ACMP_Config.sCtrlStatus.bits.bIntEn = 1; 	/* Enable interrupt */
    ACMP_Config.sCtrlStatus.bits.bMod	=0x03;	/* ACMP interrupt on falling or rising edge*/
    ACMP_Config.sCtrlStatus.bits.bOutEn = 1;    /* Enable output */
    ACMP_Config.sPinSelect.bits.bNegPin = 0x3;  /* Negative pin:DAC */
    ACMP_Config.sPinSelect.bits.bPosPin = 0;    /* Positive pin:ex0 */
    ACMP_Config.sDacSet.bits.bEn        = 1; 	/* Enable DAC */
    ACMP_Config.sDacSet.bits.bRef       = DAC_REF_VDDA;    /* reference:Vdda */
    ACMP_Config.sDacSet.bits.bVal       = 0x1F; /* Half DAC out */
    ACMP_Config.sPinEnable.bits.bEn     = 1; 	/* Enable ex0 */

    ACMP_SetCallback(ACMP1,ACMP_ISR);     		/* Install callback function */

    ACMP_Init(ACMP1, &ACMP_Config);       		/* Initialize ACMP module */

    ACMP_Enable(ACMP1);                   		/* Enable ACMP */

	while(1)
	{
	}
}

/*****************************************************************************//*!
*
* @brief  call back function.
*
* @param none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/
void ACMP_ISR(void)
{
    if(ACMP_GetFlag(ACMP1))
    {
        ACMP_ClrFlag(ACMP1);
    }
	printf("\nCallback happens.");
}
