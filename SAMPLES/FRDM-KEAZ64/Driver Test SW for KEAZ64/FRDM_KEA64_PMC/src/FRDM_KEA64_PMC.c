/****************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2013 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
*****************************************************************************
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
**************************************************************************//*!
*
* @file FRDM_KEA64_PMC.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief providing framework of demo cases for MCU.
* 		 How to enter low power mode is shown.
* 		 In this example , configuring PMC, puts MCU in stop mode
*		 and LVD is still on in stop mode.
* 		 But, RTC is also on so the RTC Interrupt will wake up MCU
* 		 after 4 seconds.
*
****************************************************************************/

#include "ics.h"
#include "rtc.h"
#include "pmc.h"
#include "uart.h"
#include "printf.h"
#include "gpio.h"


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
void RTC_Task(void);


/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{
	uint8_t __attribute__ ((unused)) u8Ch;

    /* Perform processor initialization */
	ICS_ConfigType ICS_set={0};					/* Declaration of ICS_setup structure */
	ICS_set.u8ClkMode=ICS_CLK_MODE_FEI;			/* ICS in FLL engaged internal mode*/
	ICS_set.bdiv=0;
	ICS_Init(&ICS_set);		             		/* Initialization of Core clock at 48 MHz, Bus clock 24 MHz*/

	/* UART Initialization */
	UART_ConfigType UART_Config={{0}};

	UART_Config.sctrl1settings.bits.bM=0;  		/* 8 bit mode*/
	UART_Config.sctrl1settings.bits.bPe=0;		/* No hardware parity generation or checking*/
	UART_Config.bSbns=0;						/* One stop bit*/
	UART_Config.sctrl2settings.bits.bRe=1;		/* Enable Receiver*/
	UART_Config.sctrl2settings.bits.bTe=1;		/* Enable Transmitter*/
	UART_Config.u32SysClkHz = 24000000;   		/* Bus clock in Hz*/
	UART_Config.u32Baudrate = 115200;     		/* UART baud rate */

	UART_Init(UART2,&UART_Config);				/*Initialization of UART utilities*/

	CONFIG_PIN_AS_GPIO(PTE,PTE7,OUTPUT); 		/*Config LED0 as output */

  	printf("\nRunning the FRDM_KEA64_PMC project.\n\r");

  	/* PMC Initialization */
    PMC_ConfigType  PMC_Config={{0}};

    PMC_Config.sCtrlstatus.bits.bBandgapEn = 1;	/* Enable Bandgap buffer */
    PMC_Config.sCtrlstatus.bits.bLvdStopEn = 0;	/* Disable Low-Voltage Detect(LVD) in Stop mode */
    PMC_Config.sCtrlstatus.bits.bLvdRstEn = 0;	/* LVD events do not generate hardware resets. */

    PMC_Init(PMC, &PMC_Config);					/* Initialization of PMC */
    PMC_DisableLVWInterrupt(PMC);				/* Disable hardware interrupt requests for LVWF */
    u8Ch = PMC_GetLVWFlag(PMC);					/* Get the lvw warning flag */

    /* Initialize RTC */
    RTC_ConfigType  RTC_Config={0};

    RTC_Config.u16ModuloValue = 3;
    RTC_Config.bInterruptEn   = 1;				/* Enable interrupt */
    RTC_Config.bClockSource   = RTC_CLKSRC_1KHZ;/* Clock source is 1khz */
    RTC_Config.bClockPrescaler = RTC_CLK_PRESCALER_1000; /* Prescaler is 100 */
    RTC_SetCallback(RTC_Task);
    RTC_Init(&RTC_Config);						/* Initialization of RTC */

    printf("\nEnter stop mode and will be woken up in about 4s by RTC IRQ.\n\r");
    PMC_SetMode(PMC,PmcModeStop3);				/* Enable PMC in stop mode */
    printf("\nWake up now.\n");

    while(1);

    return 0;

}

/*****************************************************************************//*!
*
* @brief callback routine of RTC driver which does what you want to do at
*        every RTC period.
*
* @param  none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/

void RTC_Task(void)
{
    /* toggle BLUE LED */
	OUTPUT_TOGGLE(PTE,PTE7); 					/* Toggle on BLUE LED */

    printf("\nRTC routine.\n"); 				/* Print Message */
}
