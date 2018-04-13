
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
* @file FRDM_KEA64_SPI_MasterInt.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief providing framework of demo cases for MCU.
*
* This example provides a template for SPI master transfer with interrupt mode.
* it generates an echo with the FRDM+KEA128 using the Slave project. The Master
* send a counter value and waits for the slave to respond with the same value to
* increase the counter
* PTE0 - SPI0_SCK (J3_6)
* PTE1 - SPI0_MOSI (J3_8)
* PTE2 - SPI0_MISO (J3_10)
* PTE3 - SPI0_SS (J3_12)
*
* BLUE will turn on indicating the Master.
* Connect ground (J5_12) of both the boards.
*******************************************************************************/

#include "ics.h"
#include "nvic.h"
#include "uart.h"
#include "spi.h"
#include "printf.h"
#include "gpio.h"


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define SPI_BIT_RATE                1000000     /* ~1Mbps */
#define  BUS_CLK_HZ					24000000L

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/
uint8_t data_send=0;
uint8_t data_receive=0;
uint32_t bytes_to_send=0;
uint32_t bytes_to_receive=0;
uint8_t spi_status=0;

/******************************************************************************
* Local functions
******************************************************************************/
int main (void);
void SPI0_ISR(void);
/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{
	static uint32_t i;

	/* Perform processor initialization */
	ICS_ConfigType ICS_set={0};				/* Declaration of ICS_setup structure */
	ICS_set.u8ClkMode=ICS_CLK_MODE_FEI;
	ICS_set.bdiv=0;
	ICS_Init(&ICS_set);             		/*Initialization of Core clock at 48 MHz, Bus clock at 24 MHz*/


	UART_ConfigType UART_Config={{0}};

	UART_Config.sctrl1settings.bits.bM=0;  	/* 8 bit mode*/
	UART_Config.sctrl1settings.bits.bPe=0;	/* No hardware parity generation or checking*/
	UART_Config.bSbns=0;					/* One stop bit*/
	UART_Config.sctrl2settings.bits.bRe=1;	/* Enable Receiver*/
	UART_Config.sctrl2settings.bits.bTe=1;	/* Enable Transmitter*/
	UART_Config.u32SysClkHz = 24000000;   	/* Bus clock in Hz*/
	UART_Config.u32Baudrate = 115200;     	/* UART baud rate */

	UART_Init(UART2,&UART_Config);			/* Initialization of UART utilities*/



  	printf("\nRunning the FRDM_KEA64_SPI_MasterInt project.\n");


    SIM_PINSEL0|=SIM_PINSEL_SPI0PS_MASK;  	/* SPI on PTE */
	SPI_ConfigType sSPIConfig = {{0}};

    /* Initialize SPI0 as master    */
    sSPIConfig.u32BitRate = SPI_BIT_RATE;
    sSPIConfig.u32BusClkHz = BUS_CLK_HZ;
    sSPIConfig.sSettings.bModuleEn             = 1;
    sSPIConfig.sSettings.bMasterMode           = 1;
    sSPIConfig.sSettings.bClkPhase1            = 1;
    sSPIConfig.sSettings.bClkPolarityLow       = 0;
    sSPIConfig.sSettings.bMasterAutoDriveSS    = 1;
    sSPIConfig.sSettings.bTxIntEn			   = 1;
    sSPIConfig.sSettings.bIntEn				   = 1;
    SPI_SetCallback(SPI0,SPI0_ISR);
    SPI_Init(SPI0, &sSPIConfig);

    CONFIG_PIN_AS_GPIO(PTE,PTE7,OUTPUT); 	/* Configure LED 0 (PTC0) as an output */
    OUTPUT_SET(PTE,PTE7);


    while(1)
	{

		SPI_TxIntEnable(SPI0);         		/* Enable tx interrupt*/
		SPI_IntEnable(SPI0);         		/* Enable rx interrupt*/

        for(i=0;i<0xffff;i++);
		if(data_send==data_receive)
		{
			printf("\nMaster send:%d.\n",data_send);
			printf("\nMaster receive:%d.\n",data_receive);
			data_send++;

		}
	}

}

void SPI0_ISR (void)
{
	 /* check Receiver */
	if(SPI_IsSPRF(SPI0)&& (SPI0_C1 & SPI_C1_SPIE_MASK))
	{
		data_receive = SPI_ReadDataReg(SPI0);
		SPI_IntDisable(SPI0);

	}
	 /* check transmitter */
	else if(SPI_IsSPTEF(SPI0) && (SPI0_C1 & SPI_C1_SPTIE_MASK))
	{
			SPI_WriteDataReg(SPI0,data_send);
			SPI_TxIntDisable(SPI0);
	}
	else
	{
		(void)SPI0_S;
	}

}


