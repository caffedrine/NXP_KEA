
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
* @file FRDM_KEA64_FLASH_FTMRE.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief providing framework of demo cases for MCU.
* 		 Here the sector on the Flash memory is erased and
* 		 the new data has been read. And then the sector is read
* 		 again to ensure that the data is written perfectly.
*
****************************************************************************/

#include "ics.h"
#include "flash.h"
#include "uart.h"
#include "printf.h"
/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define VERIFIED_SECTOR				32
#define BUS_CLK_HZ					24000000
/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/
uint8_t  u8DataBuff[256];
/******************************************************************************
* Local functions
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{
	char ch;
    uint32_t i;

	/* Perform processor initialization */
	ICS_ConfigType ICS_set={0};				/* Declaration of ICS_setup structure */
	ICS_set.u8ClkMode=ICS_CLK_MODE_FEI;		/* ICS in FLL engaged internal mode*/
	ICS_set.bdiv=0;
	ICS_Init(&ICS_set);		             	/* Initialization of Core clock at 48 MHz, Bus clock at 24 MHz */

	/* UART Initialization */
	UART_ConfigType UART_Config={{0}};

	UART_Config.sctrl1settings.bits.bM=0;  	/* 8 bit mode*/
	UART_Config.sctrl1settings.bits.bPe=0;	/* No hardware parity generation or checking*/
	UART_Config.bSbns=0;					/* One stop bit*/
	UART_Config.sctrl2settings.bits.bRe=1;	/* Enable Receiver*/
	UART_Config.sctrl2settings.bits.bTe=1;	/* Enable Transmitter*/
	UART_Config.u32SysClkHz = 24000000;   	/* Bus clock in Hz*/
	UART_Config.u32Baudrate = 115200;     	/* UART baud rate */

	UART_Init(UART2,&UART_Config);			/*Initialization of UART utilities*/

  	printf("\nRunning the FRDM_KEA64_FLASH project.\n\r");

  	/* Initialize the Flash Memory module */
    FLASH_Init(BUS_CLK_HZ);

    /* Erase 99th sector */
    FLASH_EraseSector(VERIFIED_SECTOR*FLASH_SECTOR_SIZE);

    /* Initialize the variable */
    for(i=0;i<256;i++)
    {
        u8DataBuff[i] = (uint8_t)i;
    }

    /* Write data to erased sector */
    FLASH_Program( VERIFIED_SECTOR*FLASH_SECTOR_SIZE,&u8DataBuff[0],256 );
    FLASH_Program( VERIFIED_SECTOR*FLASH_SECTOR_SIZE+256,&u8DataBuff[0],256 );

    printf("\nReading the newly written sector\n\r");

    /* Read data from the newly written sector */
    for( i=0;i<512/16;i++ )
    {
        for(ch =0;ch<16;ch++)
        {
            printf("0x%x,",*((uint8_t *)(i*16+ch+VERIFIED_SECTOR*FLASH_SECTOR_SIZE)));
        }
        printf("\n\r");
    }

    printf("All data has been read.\n\r");
    while(1);

}

