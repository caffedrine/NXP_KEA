
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
* @file FRDM_KEA64_PIT.c
*
* @author FREESCALE
*
* @version 0.0.1
*
* @brief PIT demo.
*
*   This demo sets up PIT timer 1 in chain mode with PIT timer 0 to generate
*   interrupt each 1s. PIT timer 0 runs 2,400,000 cycle, PIT timer 1 run 10 cycle,
*   so total cycle is 24,000,000, bus clock in the code is 24MHz, that's to say,
*   interrupt period is 1s.
*   So, BLUE LED will toggle at every 1s.
*   Note: To see GREEN LED toggling at every 100ms, uncomment the corresponding code
*         and comment the code for BLUE LED.
*
*******************************************************************************/
#include "nvic.h"
#include "uart.h"
#include "pit.h"
#include "ics.h"
#include "gpio.h"
#include "printf.h"
/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/

#define LED0_TOGGLE		OUTPUT_TOGGLE(PTH,PTH1) /* GREEN LED Toggle */
#define LED1_TOGGLE		OUTPUT_TOGGLE(PTE,PTE7) /* BLUE LED Toggle */
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
void PIT_Task1(void);
void PIT_Task0(void);
/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{
    uint8_t         u8Ch;
    uint32_t        u32LoadValue0, u32LoadValue1;

	/* Perform processor initialization */
	ICS_ConfigType ICS_set={0};				/* Declaration of ICS_setup structure */
	ICS_set.u8ClkMode=ICS_CLK_MODE_FEI;
	ICS_set.bdiv=0;
	ICS_Init(&ICS_set);             		/*Initialization of Core clock at 48Mhz, Bus clock at 24Mhz*/


	UART_ConfigType UART_Config={{0}};

	UART_Config.sctrl1settings.bits.bM=0;  	/* 8 bit mode*/
	UART_Config.sctrl1settings.bits.bPe=0;	/* No hardware parity generation or checking*/
	UART_Config.bSbns=0;					/* One stop bit*/
	UART_Config.sctrl2settings.bits.bRe=1;	/* Enable Receiver*/
	UART_Config.sctrl2settings.bits.bTe=1;	/* Enable Transmitter*/
	UART_Config.u32SysClkHz = 24000000;   	/* Bus clock in Hz*/
	UART_Config.u32Baudrate = 115200;     	/* UART baud rate */

	UART_Init(UART2,&UART_Config);			/*Initialization of UART utilities*/

    printf("\nRunning the FRDM_KEA64_PIT project.\n");

    PIT_ConfigType  PIT_Config1 ={0};
    PIT_ConfigType  PIT_Config0 ={0};

    /* configure PIT module in chain mode */
    /* PIT clock source is bus clock,24MHz */
    /* PIT channel 0 load value = (2400000-1), channel 1 load value = (10-1) */
    u32LoadValue0   = 0x249EFF ;               /* PIT CH0 timer load value */
    u32LoadValue1   = 0x9;                     /* PIT CH1 timer load value  */

    /* configure PIT channel 1 in chain mode, enable interrupt and timer */
    PIT_Config1.u32LoadValue      = u32LoadValue1;
    PIT_Config1.bFreeze           = FALSE;
    PIT_Config1.bModuleDis        = FALSE;    	/* Enable PIT module */
    PIT_Config1.bInterruptEn      = TRUE;
    PIT_Config1.bChainMode        = TRUE;
    PIT_Config1.bTimerEn          = TRUE;

    /* configure PIT channel 0, only enable timer */
    PIT_Config0.u32LoadValue      = u32LoadValue0;
    PIT_Config0.bFreeze           = FALSE;
    PIT_Config0.bModuleDis        = FALSE;    	/* Enable PIT module */
    PIT_Config0.bInterruptEn      = TRUE;
    PIT_Config0.bChainMode        = FALSE;
    PIT_Config0.bTimerEn          = TRUE;

    PIT_SetCallback(PIT_CHANNEL0, PIT_Task0);
    PIT_SetCallback(PIT_CHANNEL1, PIT_Task1);
    PIT_Init(PIT_CHANNEL0, &PIT_Config0);
    PIT_Init(PIT_CHANNEL1, &PIT_Config1);


    CONFIG_PIN_AS_GPIO(PTE,PTE7,OUTPUT);		/* Configure BLUE LED at PTE7 as an output */
//	CONFIG_PIN_AS_GPIO(PTH,PTH1,OUTPUT); 		/* Configure GREEN LED at PTH1 as an output Optional: Uncomment to enable green LED to toggle every 100ms */


   	while(1)

	{
        u8Ch = UART_GetChar(UART2);
        UART_PutChar(UART2, u8Ch);
	}

}

/*****************************************************************************//*!
*
* @brief PIT module task
*
* @param none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void PIT_Task1(void)
{
	OUTPUT_TOGGLE(PTE,PTE7);
	PIT_ChannelClrFlags(1);
}

/*****************************************************************************//*!
*
* @brief PIT module task
*
* @param none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void PIT_Task0(void)
{
	/*Do Nothing*/

//	OUTPUT_TOGGLE(PTH,PTH1); //Optional: Uncomment to enable green LED to toggle every 100ms
	PIT_ChannelClrFlags(0);
}
/********************************************************************/
