 /******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2013 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************
*******************************************************************************
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
* @file FRDM_KEA64_UART_Polling.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief providing framework of test cases for MCU.
*
*This example provides a demo for UART polling operation. It demonstrates how to
*send characters using UART polling.
*It will print characters on computer screen via serial port at 115200 baudrate.
*
*
*******************************************************************************/

#include "derivative.h"
#include "ics.h"
#include "uart.h"

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define SEND_BUF_LEN    50

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/
uint8_t send_buf[SEND_BUF_LEN] = "\nUART has sent char by polling \n\r";
uint8_t send_buf2[]="\n\rRunning the FRDM_KEA64_UART_Polling project.\n\r";


/******************************************************************************
* Global functions
******************************************************************************/
int main (void)
{

    /* Perform processor initialization */

    ICS_ConfigType ICS_set;					/* Declaration of ICS_setup structure */

    ICS_set.u8ClkMode=ICS_CLK_MODE_FEI;
    ICS_set.bdiv=0;
    ICS_Init(&ICS_set);						/*Initialization of Core clock at 48 MHz, Bus clock at 24 MHz*/


    UART_ConfigType UART_Config={{0}};
    UART_Config.sctrl1settings.bits.bM=0;  	/* 8 bit mode*/
    UART_Config.sctrl1settings.bits.bPe=0;	/* No hardware parity generation or checking*/
    UART_Config.bSbns=0;					/* One stop bit*/
    UART_Config.sctrl2settings.bits.bRe=1;	/* Enable Receiver*/
    UART_Config.sctrl2settings.bits.bTe=1;	/* Enable Transmitter*/
    UART_Config.u32SysClkHz = 24000000;   	/* Bus clock in Hz*/
    UART_Config.u32Baudrate = 115200;     	/* UART baud rate */

    UART_Init(UART2,&UART_Config);
	
    UART_SendWait(UART2, send_buf2, 50);
    UART_SendWait(UART2, send_buf, 50);

    while (1);
}


