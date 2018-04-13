
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
*************************************************************************//*!
*
* @file FRDM_KEA64_Bitband.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief providing framework of demo cases for MCU
* 		 This example demonstrates the bit wise read-modify-write
* 		 operations in the SRAM_U region. The 0th and 1st bit of variable
* 		 (stored in SRAM_U region) is read, then new value is written to
* 		 31st bit and and at the end the variable's value is increased
* 		 by one.
*
****************************************************************************/

#include "ics.h"
#include "uart.h"
#include "bitband.h"
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
uint32_t *p32BitBandTestVariable = (uint32_t*)0x20000100;
BIT_BandPtr pBitBandTestVariablePtr;
/******************************************************************************
* Local functions
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{
    /* Perform processor initialization */
	ICS_ConfigType ICS_set={0};					/* Declaration of ICS_setup structure */
	ICS_set.u8ClkMode=ICS_CLK_MODE_FEI; 		/* ICS in FLL engaged internal mode*/
	ICS_set.bdiv=0;
	ICS_Init(&ICS_set);             			/* Initialization of Core clock at 48 MHz, Bus clock at 24 MHz */

	/* UART Initialization */
	UART_ConfigType UART_Config={{0}};

	UART_Config.sctrl1settings.bits.bM=0;  		/* 8 bit mode*/
	UART_Config.sctrl1settings.bits.bPe=0;		/* No hardware parity generation or checking*/
	UART_Config.bSbns=0;						/* One stop bit*/
	UART_Config.sctrl2settings.bits.bRe=1;		/* Enable Receiver*/
	UART_Config.sctrl2settings.bits.bTe=1;		/* Enable Transmitter*/
	UART_Config.u32SysClkHz = 24000000;   		/* Bus clock in Hz*/
	UART_Config.u32Baudrate = 115200;     		/* UART baud rate */

	UART_Init(UART2,&UART_Config);				/* Initialization of UART utilities*/


  	printf("\n\nRunning the FRDM_KEA64_bitband project.\n\n\r");
    UART_WaitTxComplete(UART2);

    /* Invoke the pointer to access aliased bit-band. Initialize the Bit-band Variable Pointer, and check whether variable is within the SRAM_U range or not. */
    if(!(BIT_BandVariableInit(p32BitBandTestVariable,&pBitBandTestVariablePtr)))
    {
    	/* If invalid memory range then go to end of the program */
    	printf("Invalid Address of Variable\n\r");
    	goto JUMP_EXIT;
    }

    *p32BitBandTestVariable = 0x55555555;


    while(1)
   	{
           printf("New variable is 0x%x!\n\r",(unsigned int)*p32BitBandTestVariable);

           /* Read Operation */
           /* Read the 0th bit of Variable */
           if(pBitBandTestVariablePtr->bBit0)
           {
               printf("The bit0 is 1.\n\r");
           }
           else
           {
               printf("The bit0 is 0.\n\r");
           }
           /* Read the 1st bit of Variable */
           if(pBitBandTestVariablePtr->bBit1)
           {
               printf("The bit1 is 1.\n\n\r");
           }
           else
           {
               printf("The bit1 is 0.\n\n\r");
           }

           /* Write Operation */
           /* Clear the 31st bit  of Variable */
           printf("Clear the bit 31 : ");
           pBitBandTestVariablePtr->bBit31 = 0;
           printf("New variable is 0x%x.\n\r",(unsigned int)*p32BitBandTestVariable);

           /* Set the 31st bit  of Variable */
           printf("Set the bit 31 : ");
           pBitBandTestVariablePtr->bBit31 = 1;
           printf("New variable is 0x%x.\n\r\r",(unsigned int)*p32BitBandTestVariable);

           printf("Input any char to start next test!\n\n\r");
           UART_GetChar(UART2);

           /* Increment the Variable's Value by 1 */
           (*p32BitBandTestVariable) ++;
           printf("Added 1 to variable.\n\r");
   	}

   	JUMP_EXIT:
		printf("End of Program\n\r");

	return 0;
}

