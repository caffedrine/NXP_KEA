
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
**************************************************************************//*
*
* @file     FRDM_KEA64_CRC.c
*
* @author   Freescale
*
* @version 0.0.1
*
* @brief    This example demonstrates the working of CRC module in 16-bit
* 			and 32-bit mode. I generates the 16 and 32 bit CRC results for
* 			a given data.
*
*****************************************************************************/
#include "ics.h"
#include "crc.h"
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

/******************************************************************************
* Local functions
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

/*****************************************************************************/
int main (void)
{

    uint32_t        u32Crc_ConverterResult;
    uint32_t        u32SeedValue;
    uint8_t MessageSource[] = {"123456789"} ;	/* Initialize message */

    /* Perform processor initialization */
	ICS_ConfigType ICS_set={0};					/* Declaration of ICS_setup structure */
	ICS_set.u8ClkMode=ICS_CLK_MODE_FEI;			/* ICS in FLL engaged internal mode*/
	ICS_set.bdiv=0;
	ICS_Init(&ICS_set);		             		/* Initialization of Core clock at 48 MHz, Bus clock at 24 MHz*/

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

    printf("\nRunning the FRDM_KEA64_CRC project.\n\r");

    CRC_ConfigType  sCRC_ConfigType ={0};
    CRC_ConfigType  *pCRC_Config=&sCRC_ConfigType;

    /* Initialize CRC register works under 16-bit mode */
    pCRC_Config->u32PolyData            = 0x1021;                   /* Initialize CRC-CCITT poly value */
    u32SeedValue                        = 0xFFFF;                   /* Set CRC-CCITT seed value */
    pCRC_Config->bWidth                 = CRC_WIDTH_16BIT;
    pCRC_Config->bTransposeReadType     = CRC_READ_TRANSPOSE_NONE;  /* Do not transpose in read */
    pCRC_Config->bTransposeWriteType    = CRC_WRITE_TRANSPOSE_NONE; /* Do not transpose in write */

    /* Begin to CRC-CCITT conversion */
    CRC_Init(pCRC_Config);                                          /* CRC module initialized as 16-bit mode */

    u32Crc_ConverterResult = CRC_Cal16(u32SeedValue, &MessageSource[0], (sizeof(MessageSource)-1));
    printf("CRC-CCITT function calculation result = 0x%x @seed = 0x%x .\n\r", (unsigned int)u32Crc_ConverterResult, (unsigned int)u32SeedValue );

    /* Reset CRC register for 32-bit mode calculation*/
    CRC_DeInit();

    /* Initalize CRC register works under 32-bit mode */
    pCRC_Config->u32PolyData            = 0x04C11DB7;               /* Set CRC32 poly value */
    u32SeedValue                        = 0xFFFFFFFF;               /* Set CRC32 seed value */
    pCRC_Config->bWidth                 = CRC_WIDTH_32BIT;
    pCRC_Config->bTransposeReadType     = CRC_READ_TRANSPOSE_ALL;   /* Do not transpose in read */
    pCRC_Config->bTransposeWriteType    = CRC_WRITE_TRANSPOSE_BIT;  /* Do not transpose in write */
    pCRC_Config->bFinalXOR              = TRUE;                     /* Reverse CRC result */

    /* Begin to CRC-CCITT conversion */
    CRC_Init(pCRC_Config);                                          /* CRC module initialized as 32-bit mode */
    u32Crc_ConverterResult = CRC_Cal32(u32SeedValue, &MessageSource[0], (sizeof(MessageSource)-1));
    printf("CRC32 function calculation result = 0x%x @seed = 0x%x .\n\r", (unsigned int)u32Crc_ConverterResult, (unsigned int)u32SeedValue );

    printf("End of program.\n\r");

    return 0;
}
