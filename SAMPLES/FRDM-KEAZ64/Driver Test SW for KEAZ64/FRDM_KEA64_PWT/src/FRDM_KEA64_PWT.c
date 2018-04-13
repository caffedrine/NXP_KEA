
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
* @file FRDM_KEA64_PWT.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief providing framework of demo cases for MCU.
* 		This example generates a toggle pulse on PTD0 (J6_1) at 10 Hz frequency with RTC library.
* 		The period and frequency is measure with PWT library on pin PTD5 (J1_8).
* 		Note: Connect PTD0 to PDT5
*
*******************************************************************************/

#include "ics.h"
#include "uart.h"
#include "pwt.h"
#include "rtc.h"
#include "gpio.h"
#include "printf.h"
/******************************************************************************
* Global variables
******************************************************************************/

float u32PositivePulse[64], u32NegativePulse[64];
float Frequency=0.0;
uint8_t Frequency_decimals=0;
uint32_t u32SampleCycle = 0,  u32Inc;
uint32_t u32IntervalTime=0;
/******************************************************************************
* Constants and macros
******************************************************************************/

#define DIVIDER_RATIO 128
#define TIMER_CLK     48000000

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
void PWT_Task(void);
void Pulse_Calculation(void);

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

	UART_Init(UART2,&UART_Config);			/*Initialization of UART utilities*/

	printf("\nRunning the FRDM_KEA64_PWT project.\n\n");

	


    RTC_ConfigType  RTCConfig = {0};

    RTCConfig.u16ModuloValue = 9375;        			   	/* Configure RTC to interrupt every 50 ms */
    RTCConfig.bInterruptEn   = RTC_INTERRUPT_ENABLE;     	/* Enable interrupt */
    RTCConfig.bClockSource   = RTC_CLKSRC_BUS;      		/* Clock source is Bus clock at 24Mhz*/
    RTCConfig.bClockPrescaler = RTC_CLK_PRESCALER_128;   	/* Prescaler is 128*/
    RTC_SetCallback(RTC_Task);
	RTC_Init(&RTCConfig);

    PWT_ConfigType  PWTConfig = {0};
    PWTConfig.bOverflowIntEn      = TRUE;    				  /* Enable overflow interrupt*/
    PWTConfig.bReadyIntEn         = TRUE;                     /* Enable data ready interrupt*/
    PWTConfig.bPWTIntEn           = TRUE;                     /* Enable PWT module interrupt*/
    PWTConfig.bPWTEn              = TRUE;                     /* Enable PWT module */
    PWTConfig.bClockPrescaler     = PWT_CLK_PRESCALER_128;    /* PWT clock source prescaler is 128 */
    PWTConfig.bEdgeSensitivity    = FALLING_START_CAPTURE_ALL;/* Start to measure pulse from first falling-edge and all edge */
    PWTConfig.bPinSelect          = PWT_PINSEL_PTD5;          /* PWT input clock pin is PTD5 */
    PWTConfig.bClockSource        = PWT_PCLKS_BUS;            /* PWT clock source is bus clock */
    PWT_SetCallback(PWT_Task);
    PWT_Init(&PWTConfig);

    /* Initialize PTD0 near PWT_IN0 input(PTD5) on board*/
    CONFIG_PIN_AS_GPIO(PTD,PTD0,OUTPUT); /* Configure pin PTD0 as an output to connect to PTD5 */
    CONFIG_PIN_AS_GPIO(PTE,PTE7,OUTPUT); /* Configure BLUE LED at PTE7 as an output */


    while(1)
    {
        Pulse_Calculation();
    }
}


/*****************************************************************************//*!
*
* @brief callback routine of RTC driver which does what you want to do at
*        every RTC period.
*
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void RTC_Task(void)
{

    /* Toggle PTD0 to produce 10Hz frequency waveform */
    OUTPUT_TOGGLE(PTD,PTD0);
    /* Toggle led PTE7 to produce 10Hz frequency waveform */
    OUTPUT_TOGGLE(PTE,PTE7);
    u32IntervalTime++;
}


/*****************************************************************************//*!
*
* @brief callback routine of PWT driver
*
*
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void PWT_Task(void)
{
    if(PWT_GetOverflowFlag())
    {
        PWT_ClrOverflowFlag();
    }

    if(PWT_GetReadyFlag())
    {

        if(u32SampleCycle<64)
        {
            u32PositivePulse[u32SampleCycle] = ((PWT->R1>>16)&0xFFFF);   /*put the positive pulse width data to buffer */
            u32NegativePulse[u32SampleCycle] = (PWT->R2&0xFFFF);         /*put the negative pulse width data to buffer */
            u32SampleCycle++;
        }
        if(u32SampleCycle == 64)                                     /*only sample 64 times */
        {
            u32SampleCycle = 0;
        }
        PWT_ClrReadyFlag();
    }
}

/*****************************************************************************//*!
*
* @brief calculate the period of input pulse
*
*
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void Pulse_Calculation(void)
{
    while(u32IntervalTime<=4);    /*calculate pluse width each 1s */
    for(u32Inc=0;u32Inc<64;u32Inc++)
    {
        if(u32PositivePulse[u32Inc] && u32NegativePulse[u32Inc])
        {
        	Frequency =TIMER_CLK/((u32PositivePulse[u32Inc] + u32NegativePulse[u32Inc])*DIVIDER_RATIO);   /*calculate pluse width each 1s */
            Frequency_decimals=(Frequency-(uint8_t)Frequency)*10;
        	printf("Positive Pulse Width =%d, Negative Pulse Width =%d, Frequency = %d.%d Hz\n", (uint16_t)u32PositivePulse[u32Inc], (uint16_t)u32NegativePulse[u32Inc],(uint16_t)Frequency,(uint16_t)Frequency_decimals);
        }
    }

    for(u32Inc=0;u32Inc<64;u32Inc++)
    {
        u32PositivePulse[u32Inc] =0;
        u32NegativePulse[u32Inc] =0;
    }

    u32IntervalTime = 0;

}
