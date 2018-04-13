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
* @file FRDM_KEA64_FTM_EPWM.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief Demonstrate EPWM.
         BLUE LEDs on FRDM-KEA128 board will change their brightness.
*
*******************************************************************************/
#include "ics.h"
#include "ftm.h"
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
FTM_Type* pFTM0 = (FTM_Type*)FTM0;
FTM_Type* pFTM1 = (FTM_Type*)FTM1;
FTM_Type* pFTM2 = (FTM_Type*)FTM2;
/******************************************************************************
* Local functions
******************************************************************************/;
void FTM1_Task(void);
/******************************************************************************
* Global functions
******************************************************************************/
FTM_Type* temp=(FTM_Type*)FTM1;
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

	printf("\nRunning the FRDM_KEA64_FTM_EPWM project.\n");


	FTM_ConfigType FTM1_Config={0};
	FTM_ChParamsType FTM1CH1_Config={0};

	FTM1_Config.modulo=9999;
	FTM1_Config.clk_source=FTM_CLOCK_SYSTEMCLOCK;
	FTM1_Config.prescaler=FTM_CLOCK_PS_DIV1;
	FTM1_Config.mode=1;
	FTM1_Config.toie=1;

	SIM_PINSEL0 |= SIM_PINSEL_FTM1PS1_MASK; /* Select Pins corresponds to the PTE7 for output */

	FTM1CH1_Config.ctrl.bits.bMode=FTM_PWMMODE_EDGEALLIGNED;
	FTM1CH1_Config.ctrl.bits.bPWMPol=FTM_PWM_HIGHTRUEPULSE;
	FTM1CH1_Config.u16CnV=1000;
	
	FTM_SetCallback(pFTM1, FTM1_Task);
	FTM_ChannelInit(pFTM1,1,FTM1CH1_Config);
	FTM_Init(pFTM1,&FTM1_Config);

	while(1);
}

/*****************************************************************************//*!
*
* @brief callback routine of FTM1 driver. FTM1 CH1 will change the duty cycle
*        periodically.
*
* @param  none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/

uint16_t u16ChV_old, u16ChV_new;

void FTM1_Task(void)
{
    static uint16_t u16count;
    static uint8_t u8DirMark;
    FTM_MemMapPtr pFTM = (FTM_MemMapPtr)FTM1;


    /* clear the flag */
    FTM_ClrOverFlowFlag(pFTM1);

    if(100 == u16count)
    {
        u16count = 0;
        u16ChV_old = pFTM->CONTROLS[1].CnV;
        if(!u8DirMark) /* First increase u16ChV value up to maximum value */
        {
            u16ChV_new = u16ChV_old + 200;
            if(u16ChV_new >= pFTM->MOD)
            {
                u16ChV_new = pFTM->MOD - 200;
                u8DirMark  = 1;
            }
            else
            {
            }
        }
        else /* If reached maximum then decrease u16ChV value up to minimum value */
        {
            u16ChV_new = u16ChV_old - 200;
            if(u16ChV_new <  200)
            {
                u16ChV_new = 200;
                u8DirMark  = 0;
            }
            else
            {
            }
        }
        /* update the channel value */
        FTM_SetChannelValue(pFTM1, FTM_CHANNEL_CHANNEL1, u16ChV_new);
    }
    else
    {
        u16count++;
    }
}
/********************************************************************/

