
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
* @file     FRDM_KEA64_RTC.c
*
* @author   Freescale
*
* @version 0.0.1
*
* @brief    Real-time counter (RTC) demo code.
*
* Note:  RTC produces 1s interrupt with LPO clock source, then toggle BLUE LED (PTC0)
*        with the trigger.
*******************************************************************************/

#include "rtc.h"
#include "uart.h"
#include "derivative.h"
#include "ics.h"
#include "gpio.h"
#include "printf.h"
/******************************************************************************
* Global variables
******************************************************************************/
uint16_t value;
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
void RTC_Task(void);
/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{

    uint8_t u8Ch;

    /* Perform processor initialization */
	ICS_ConfigType ICS_set={0};				/* Declaration of ICS_setup structure */
	ICS_set.u8ClkMode=ICS_CLK_MODE_FEI;
	ICS_set.bdiv=0;
	ICS_Init(&ICS_set);             		/* Initialization of Core clock at 48 MHz, Bus clock at 24 MHz*/


	UART_ConfigType UART_Config={{0}};

	UART_Config.sctrl1settings.bits.bM=0;  	/* 8 bit mode*/
	UART_Config.sctrl1settings.bits.bPe=0;	/* No hardware parity generation or checking*/
	UART_Config.bSbns=0;					/* One stop bit*/
	UART_Config.sctrl2settings.bits.bRe=1;	/* Enable Receiver*/
	UART_Config.sctrl2settings.bits.bTe=1;	/* Enable Transmitter*/
	UART_Config.u32SysClkHz = 24000000;   	/* Bus clock in Hz*/
	UART_Config.u32Baudrate = 115200;     	/* UART baud rate */

	UART_Init(UART2,&UART_Config);			/* Initialization of UART utilities*/


    /* Perform processor initialization */
  	printf("\nRunning the FRDM_KEA64_RTC project.\n");

    RTC_ConfigType  pRTC_Config={0};

    /* configure RTC to 1Hz interrupt frequency */
    pRTC_Config.u16ModuloValue = 10;
    pRTC_Config.bInterruptEn   = RTC_INTERRUPT_ENABLE;     /* Enable interrupt */
    pRTC_Config.bClockSource   = RTC_CLKSRC_1KHZ;          /* Clock source is 1khz */
    pRTC_Config.bClockPrescaler = RTC_CLK_PRESCALER_100;    /* Prescaler is 100 */

    RTC_SetCallback(RTC_Task);
	RTC_Init(&pRTC_Config);

	/* Make RED LED as an output pin */
	CONFIG_PIN_AS_GPIO(PTH,PTH0,OUTPUT);

    /* Echo chars received from terminal */
	while(1)
	{
        u8Ch = UART_GetChar(UART2);
        UART_PutChar(UART2, u8Ch);

        }


	
}

/*****************************************************************************//*!
*
* @brief callback for RTC interrupt service routine
*
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void RTC_Task(void)
{
	OUTPUT_TOGGLE(PTH,PTH0);

}
/********************************************************************/
