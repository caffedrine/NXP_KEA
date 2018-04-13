
 /******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2013 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************
*******************************************************************************
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
*******************************************************************************/
/*
* @file FRDM_KEA64_UART_Interrupt.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief providing framework of test cases for MCU.
*
*This example provides a demo for UART interrupt operation. It demonstrates how to
*send characters using UART interrupt.
*It will print characters on computer screen via serial port at 115200 baud rate.
*
*******************************************************************************/

#include "ics.h"
#include "uart.h"
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
* Global functions
******************************************************************************/

void UART2_ISR (void);
uint8_t send_buf[] ="\n\rRunning the FRDM_KEA64_UART_Interrupt project.\n";
uint8_t send_buf2[] ="\n\rPrint characters using interrupt mode.\n\r";
uint8_t i,j,read_char;

int main (void)
{
  	ICS_ConfigType ICS_set;					/* Declaration of ICS_setup structure */

	ICS_set.u8ClkMode=ICS_CLK_MODE_FEI;
	ICS_set.bdiv=0;
	ICS_Init(&ICS_set);             		/*Initialization of Core clock at 48 MHz, Bus clock at 24 MHz*/

    UART_ConfigType Uart_Config={{0}};

    Uart_Config.sctrl1settings.bits.bM=0; 	/* 8 bit Mode */
    Uart_Config.sctrl1settings.bits.bPe=0; 	/* Parity disable */
    Uart_Config.bSbns=0;					/* One stop bit*/
    Uart_Config.sctrl2settings.bits.bRe=1;	/* Receiver enable*/
    Uart_Config.sctrl2settings.bits.bTe=1;	/* Transmitter enable*/
    Uart_Config.sctrl2settings.bits.bTie=1;	/* Transmit buffer empty interrupt enable*/
    Uart_Config.u32SysClkHz = 24000000;   	/* Bus clock in Hz*/
    Uart_Config.u32Baudrate = 115200;     	/* Baud rate*/

    /*Initialization of UART module*/
    UART_SetCallback(UART2, UART2_ISR);
    UART_Init(UART2,&Uart_Config);

    while (1);
}

void UART2_ISR (){
	(void)UART2_S1;							/* Read UART2_S1 register*/

	if(UART_IsTxBuffEmpty(UART2))
	{
		if(i<sizeof(send_buf))
		{
			i++;
			UART_WriteDataReg(UART2,send_buf[i]);
		}
		else if(j<sizeof(send_buf2))
		{
			j++;
			UART_WriteDataReg(UART2,send_buf2[j]);
		}
		else
		{
			read_char = UART_GetChar(UART2);
			UART_PutChar(UART2, read_char);
		}

	}

	
}
