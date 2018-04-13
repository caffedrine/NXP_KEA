
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
* @file FRDM_KEA64_I2C_MasterPoll.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief providing framework of demo cases for MCU.
*
*
*	This example provides a template for I2C master with poll mode.
*	it cycle to transfer a frame data(1 byte) and show it on terminal.
*	it also provide a template for how to initialize I2C module.
*
*	- Requirement for demo:
*	1. connect two boards with PTA2(SDA - J2_17), PTA3(SCL - J2_19), and (GND - J2_13)
*	2. place pull up resistors between connections.
*	3. connect the grounds of both Master and Slave devices.
*
*******************************************************************************/

#include "ics.h"
#include "uart.h"
#include "i2c.h"
#include "gpio.h"
#include "printf.h"
#include"derivative.h"
/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define I2C_SLAVE_ADDRESS          0xA0
/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/
I2C_Type* pI2C0 = (I2C_Type*)I2C0;
/******************************************************************************
* Local functions
******************************************************************************/
int main (void);
/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{
    I2C_ConfigType  I2C_Config = {{0}};
    uint8_t data_send[1]={0};
    uint8_t data_receive[1]={0};
    uint32_t i=0;

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

  	printf("\r\nRunning the FRDM_KEA64_I2C_MasterPoll project.\r\n");


    /* Initialize I2C module with poll mode */
  	I2C_Config.u16Slt = 0;
  	I2C_Config.u16F = 0xBC; 				/* Baud rate at 100 kbit/sec, MULT = 4 , ICR=60*/
    I2C_Config.sSetting.bIntEn = 0;
    I2C_Config.sSetting.bI2CEn = 1;
    I2C_Config.sSetting.bMSTEn = 1;

    I2C_Init(pI2C0,&I2C_Config );

  	CONFIG_PIN_AS_GPIO(PTE,PTE7,OUTPUT); 	/* Configure LED 0 (PTC0) as an output */
  	OUTPUT_SET(PTE,PTE7);

	while(1)
	{

		if(data_send[0] == data_receive[0])
		{
		data_send[0]++;
		I2C_MasterSendWait(pI2C0,I2C_SLAVE_ADDRESS,&data_send[0],1);

	  	printf("\r\n Master send:%d \r\n", data_send[0]);



	  	I2C_MasterReadWait(pI2C0,I2C_SLAVE_ADDRESS,&data_receive[0],1);

	  	printf("\r\n Master receive:%d \r\n", data_receive[0]);

		}

        for(i=0;i<0xfffff;i++);
	}

}


