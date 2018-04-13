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
*******************************************************************************/
/*
* @file FRDM_KEA64_UART_Loopback.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief providing framework of test cases for MCU.
*
* This example shows the loopback mode in UART0 by creating an echo in the hyperteminal.
* When writing an character in the hyperteminal, UART2 will receive the char.
* The character it will be send and received by UART0 in loopback mode.
* Then the char received with UART0 will be printed in the hyperteminal
* using UART2. UART baud rates are at 115200.
*
*******************************************************************************/


#include "uart.h"
#include "derivative.h"
#include "ics.h"
#include "printf.h"

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define SEND_BUF_LEN    1
#define RECEIVE_BUF_LEN 1
/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/
uint8_t send_buf[SEND_BUF_LEN] = {'L'};
uint8_t receive_buf[RECEIVE_BUF_LEN] = {0};


/******************************************************************************
* Global functions
******************************************************************************/
int main (void)
{

    /* *************Perform processor initialization *************************/

     ICS_ConfigType ICS_set;					/* Declaration of ICS_setup structure */
     ICS_set.u8ClkMode=ICS_CLK_MODE_FEI;
     ICS_set.bdiv=0;
     ICS_Init(&ICS_set);            			/* Initialization of Core clock at 48 MHz, Bus clock at 24 MHz*/


    UART_ConfigType Config_UART0={{0}};
    UART_ConfigType Config_UART2={{0}};

    Config_UART0.sctrl1settings.bits.bM=0;  	/* 8 bit mode*/
    Config_UART0.sctrl1settings.bits.bPe=0; 	/* No hardware parity generation or checking*/
    Config_UART0.bSbns=0;				  		/* One stop bit*/
    Config_UART0.sctrl1settings.bits.bLoops=1; 	/* Loopback enable*/
    Config_UART0.sctrl2settings.bits.bRe=1;	  	/* Enable Receiver*/
    Config_UART0.sctrl2settings.bits.bTe=1;		/* Enable Transmitter*/
    Config_UART0.u32SysClkHz = 24000000;		/* Bus clock in Hz*/
    Config_UART0.u32Baudrate = 115200;			/* UART baud rate*/

    Config_UART2.sctrl1settings.bits.bM=0;		/* 8 bit mode*/
    Config_UART2.sctrl1settings.bits.bPe=0;		/* No hardware parity generation or checking*/
    Config_UART2.sctrl2settings.bits.bRe=1;		/* Enable Receiver*/
    Config_UART2.sctrl2settings.bits.bTe=1;		/* Enable Receiver*/
    Config_UART2.bSbns=0; 						/* One stop bit*/
    Config_UART2.u32SysClkHz = 24000000;		/* Bus clock in Hz*/
    Config_UART2.u32Baudrate = 115200;			/* UART baud rate*/

    /*Initialization of UART port 0 and 2*/
    UART_Init(UART0,&Config_UART0);
    UART_Init(UART2,&Config_UART2);
	
	printf("\nRunning the FRDM_KEA64_UART_Loopback project.\n");
    printf("\nEnter any character to echo...\n\r");
	UART_WaitTxComplete(UART2);


    while(1)
    {
        send_buf[0] = UART_GetChar(UART2);  	/* Get char from hyperterminal*/

        /* LOOPBACK*/
        UART_SendWait(UART0, send_buf, 1);  	/* Send hyperteminal char trough UART0*/
        UART_ReceiveWait(UART0, receive_buf, 1);/* Receive data from UART0*/

        UART_PutChar(UART2, receive_buf[0]);	/* Print received data from UART0 in hyperterminal*/
    }
}

