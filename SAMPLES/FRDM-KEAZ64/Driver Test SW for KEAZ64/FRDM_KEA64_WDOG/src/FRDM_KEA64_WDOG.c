
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
* @file FRDM_KEA64_WDOG.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief watchdog feed/refresh demo.
*
*This example provides a sample code for watchdog.
*It demonstrates how to call watchdog driver lib to initialize the watchdog
*and how to check when the watchdog reset occurs.
*The watchdog is initialized to be enabled in low power state as well as
*in debug state and generate watchdog reset at around 1s.
*It toggles BLUE LED PTE7 periodically. When a NMI pin is asserted to ground,
*it will then generate watchdog reset and the RED LED PTH0 LED will blink.
*
*To create a NMI interrupt connect PTB4 (J2_4) to the ground (J2_13).
*
*******************************************************************************/
#include "ics.h"
#include "uart.h"
#include "wdog.h"
#include "printf.h"
#include "gpio.h"
#include "rtc.h"

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
 #define  BUS_CLK_HZ		24000000
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
void    DelayUS(uint32_t u32TimeUS);
/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{

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

   	printf("\nRunning the FRDM_KEA64_WDOG project.\n");

    WDOG_ConfigType WDOGConfig = {{0}};     /* Watchdog configuration structure */

    WDOGConfig.sBits.bWaitEnable   = TRUE;
    WDOGConfig.sBits.bStopEnable   = TRUE;
    WDOGConfig.sBits.bDbgEnable    = TRUE;
    WDOGConfig.sBits.bUpdateEnable = FALSE;
    WDOGConfig.sBits.bDisable      = FALSE; /* Enable WDOG */
    WDOGConfig.sBits.bClkSrc       = WDOG_CLK_INTERNAL_1KHZ;
    WDOGConfig.u16TimeOut          = 1000;  /* 1s */
    WDOGConfig.u16WinTime          = 0;

    WDOG_Init(&WDOGConfig);


    if(WDOG_IsReset())                      /* Check if wathdog reset happens */
    {
        /*! watchdog happens. */
    	CONFIG_PIN_AS_GPIO(PTH,PTH0,OUTPUT);/* Configure RED LED (PTH0) as an output */
        while(1)
        {
        	OUTPUT_TOGGLE(PTH,PTH0);        /* Flash RED light if watchdog reset occurs */
            WDOG_Feed();
            DelayUS(50000);                 /* Delay around 50ms */
        }
    }

    /*! watchdog does not happens, then flash blue led. */
    CONFIG_PIN_AS_GPIO(PTE,PTE7,OUTPUT); 	/* Configure BLUE LED (PTE7) as an output */
    while(1)
    {
    	OUTPUT_TOGGLE(PTE,PTE7);            /* Flash BLUE light if no watchdog reset occurs */
        WDOG_Feed();
        DelayUS(50000);                     /* Delay around 50ms */
    }

}


/*****************************************************************************//*!
*
* @brief delay certain period of time in microseconds.
*
* @param[in]    u32TimeUS   delay value in microseconds.
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void DelayUS(uint32_t u32TimeUS)
{
    RTC_ConfigType  RTC_Config = {0};


    /* configure RTC to 1us period */
    RTC_Config.u16ModuloValue = u32TimeUS/(100000000/BUS_CLK_HZ)-1;
    RTC_Config.bInterruptEn   = FALSE;                    /* Enable interrupt */
    RTC_Config.bClockSource   = RTC_CLKSRC_BUS;           /* Clock source is bus clock */
    RTC_Config.bClockPrescaler = RTC_CLK_PRESCALER_100;   /* Prescaler is 100 */

    RTC_Init(&RTC_Config);
    while(!RTC_GetFlags());
    RTC_ClrFlags();
    RTC_DeInit();
}

/*****************************************************************************//*!
*
* @brief NMI interrupt service routine.
*
*   It only stays in endless loop so as to generate watchdog reset.
* @param    none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void NMI_Handler(void)
{
	while(1);
	/*!
     * Endless loop to generate watchdog reset
     */

}
/********************************************************************/


