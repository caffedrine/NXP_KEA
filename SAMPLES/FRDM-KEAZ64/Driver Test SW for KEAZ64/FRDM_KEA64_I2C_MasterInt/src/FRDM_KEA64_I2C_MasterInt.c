
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
* @file FRDM_KEA64_I2C_MasterInt.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief providing framework of demo cases for MCU.
*
*This example provides a template for I2C master with interrupt mode.
it cycle to transfer a frame data(1 byte) and show it on terminal.
it also provide a template for how to initialize I2C module.
*
*	- Requirement for demo:
	1. connect two boards with PTA2(SDA - J2_17), PTA3(SCL - J2_19), and (GND - J2_13)
	2. place pull up resistors between connections.
	3. Connect the grounds of both Master and Slave devices.
*
*******************************************************************************/

#include "ics.h"
#include "uart.h"
#include "gpio.h"
#include "i2c.h"
#include "printf.h"
#include "derivative.h"
/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define I2C_SLAVE_ADDRESS      0xA0
#define DATA_LENGTH 1
/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/
uint8_t data_send[1]={0};
uint8_t data_receive[1]={0};
uint32_t i;
uint32_t bytes_to_send=0;
uint32_t bytes_to_read=0;
uint8_t send_finish=1;

I2C_Type* pI2C0 = (I2C_Type*)I2C0;
/******************************************************************************
* Local functions
******************************************************************************/
int main (void);
void I2C0_ISR(void);
/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{
	uint8_t __attribute__ ((unused)) counter=0;

    /* Perform processor initialization */
	ICS_ConfigType ICS_set={0};				/* Declaration of ICS_setup structure */
	ICS_set.u8ClkMode=ICS_CLK_MODE_FEI;
	ICS_set.bdiv=0;
	ICS_Init(&ICS_set);             		/*Initialization of Core clock at 48 MHz, Bus Clock at 24 MHz*/


	UART_ConfigType UART_Config={{0}};

	UART_Config.sctrl1settings.bits.bM=0;  	/* 8 bit mode*/
	UART_Config.sctrl1settings.bits.bPe=0;	/* No hardware parity generation or checking*/
	UART_Config.bSbns=0;					/* One stop bit*/
	UART_Config.sctrl2settings.bits.bRe=1;	/* Enable Receiver*/
	UART_Config.sctrl2settings.bits.bTe=1;	/* Enable Transmitter*/
	UART_Config.u32SysClkHz = 24000000;   	/* Bus clock in Hz*/
	UART_Config.u32Baudrate = 115200;     	/* UART baud rate */

	UART_Init(UART2,&UART_Config);			/*Initialization of UART utilities*/

  	printf("\r\nRunning the FRDM_KEA64_I2C_MasterInt project.\r\n");

    I2C_ConfigType  sI2C_Config = {{0}};

    /* Initialize I2C module with interrupt mode */
    sI2C_Config.u16Slt = 0;
    sI2C_Config.u16F = 0xBC;  				/* Baud rate at 100 kbit/sec, MULT = 4 , ICR=60*/
    sI2C_Config.sSetting.bMSTEn=1;
    sI2C_Config.sSetting.bIntEn = 1;
    sI2C_Config.sSetting.bI2CEn = 1;

    I2C0_SetCallBack(I2C0_ISR);
    I2C_Init(pI2C0,&sI2C_Config );

    CONFIG_PIN_AS_GPIO(PTE,PTE7,OUTPUT);
    OUTPUT_SET(PTE,PTE7);

	while(1)
	{

		/* Master Send data*/
		if(send_finish==0)
		{
		 I2C_MasterSend(pI2C0, 0xA0);

		}

		/*Master Receive data*/
		else
		{

		    I2C_MasterRead(pI2C0, 0xA0);


         }

		for(i=0;i<0xfffff;i++);
		if(data_send[0]==data_receive[0])
		{
			printf("\r\n Master send:%d \r\n", data_send[0]);
			 printf("\r\n Master receive:%d \r\n", data_receive[0]);
				data_send[0]++;
		}else{
			/* Do nothing. */
		}



	}

}


/*****************************************************************************//*!
*
* @brief callback routine of I2C0.
*
* @param  none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/

void I2C0_ISR(void)
{
	I2C_ClearStatus(pI2C0,I2C_S_IICIF_MASK);

	/* Master send data to slave */

	if(I2C_IsTxMode(pI2C0))
	{
		if( bytes_to_send < DATA_LENGTH )
		{
			I2C_WriteDataReg(pI2C0,data_send[bytes_to_send]);
			bytes_to_send++;

		}
		else
		{
			I2C_Stop(pI2C0);
			bytes_to_send=0;
			send_finish=1;

		}
	}
	else
	{
		/* Master receive data from slave */
		if( bytes_to_read < (DATA_LENGTH-1) )
		{
			I2C_SendAck(pI2C0);
			data_receive[bytes_to_read] = I2C_ReadDataReg(pI2C0);
			bytes_to_read++;

		}
		else if( bytes_to_read == (DATA_LENGTH-1) )
		{
		   I2C_SendNack(pI2C0);
		   data_receive[bytes_to_read] = I2C_ReadDataReg(pI2C0);
			bytes_to_read++;

		}
		else
		{
			I2C_Stop(pI2C0);
			I2C_ReadDataReg(pI2C0);
			bytes_to_read=0;
			send_finish=0;

		}

	}


}
