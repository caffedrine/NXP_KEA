
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
* @file FRDM_KEA64_ADC_FIFO.c
*
* @author Freescale
*
* @version 0.0.1
*
*
* @brief providing a ADC demo with FIFO operation.
*
*This demo reads the Potentiometer, VREFH and VREFL and saving the conversion
*in an array that is shown on the screen when the user press a key.
*******************************************************************************/
#include "ics.h"
#include "uart.h"
#include "adc.h"
#include "printf.h"
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
volatile uint16_t u16ADC_ConversionBuff[16];
volatile uint16_t u16ADC_ConversionCount = 0;
volatile uint8_t  u8ADC_ConversionFlag = 0;
/******************************************************************************
* Local functions
******************************************************************************/

void ADC_CallBack( void );
/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{
	uint8_t __attribute__ ((unused)) u8Ch;

	/* Perform processor initialization */
  	ICS_ConfigType ICS_set={0};				/* Declaration of ICS_setup structure */
  	ICS_set.u8ClkMode=ICS_CLK_MODE_FEI;
    ICS_set.bdiv=0;
  	ICS_Init(&ICS_set);             			/* Initialization of Core clock at 48 MHz, Bus clock at 24 MHz */


    UART_ConfigType UART_Config={{0}};

    UART_Config.sctrl1settings.bits.bM=0;  		/* 8 bit mode*/
    UART_Config.sctrl1settings.bits.bPe=0;		/* No hardware parity generation or checking*/
    UART_Config.bSbns=0;						/* One stop bit*/
    UART_Config.sctrl2settings.bits.bRe=1;		/* Enable Receiver*/
    UART_Config.sctrl2settings.bits.bTe=1;		/* Enable Transmitter*/
    UART_Config.u32SysClkHz = 24000000;   		/* Bus clock in Hz*/
    UART_Config.u32Baudrate = 115200;     		/* UART baud rate */

    UART_Init(UART2,&UART_Config);				/*Initialization of UART utilities*/

    printf("\n\nRunning the FRDM_KEA64_ADC_FIFO project.\n\n");


    ADC_ConfigType  ADC_Config={{0}};

    /* Initialization of ADC module */
    ADC_Config.u8ClockDiv = ADC_ADIV_DIVIDE_4;
    ADC_Config.u8ClockSource = CLOCK_SOURCE_BUS_CLOCK;
    ADC_Config.u8Mode = ADC_MODE_12BIT;
    ADC_Config.sSetting.bIntEn = 1;
    ADC_Config.u8FiFoLevel = ADC_FIFO_LEVEL3;
    ADC_Config.u16PinControl= 0x0400; 			/* Disable I/O control on ADC channel 10*/

    ADC_SetCallBack(ADC_CallBack);
    ADC_Init(ADC, &ADC_Config);

    /* Echo chars received from terminal */
	while(1)
	{
	    /* Set channel to start a new conversion */
	    u8ADC_ConversionFlag = 0;
        ADC_SetChannel(ADC,ADC_CHANNEL_AD10);
        ADC_SetChannel(ADC,ADC_CHANNEL_AD29_VREFH);
        ADC_SetChannel(ADC,ADC_CHANNEL_AD30_VREFL);
        /* wait conversion complete */
        while( !u8ADC_ConversionFlag);

        printf("\nADC conversion result as below\n");

        printf("\nPOTENTIOMETER conversion value:0x%x\n",u16ADC_ConversionBuff[0]);
		printf("VREFH conversion value:0x%x\n",u16ADC_ConversionBuff[1]);
		printf("VREFL conversion value:0x%x\n",u16ADC_ConversionBuff[2]);


        printf("Input any character to start a new conversion!\n\n");
        u8Ch = UART_GetChar(UART2);
        u16ADC_ConversionCount = 0;
	}

}


/***************************************************************************
+FUNCTION----------------------------------------------------------------
* @function name: ADC_CallBack
*
* @brief callback routine of ADC driver which does what you want to do at
*        every ADC interrupt.
*
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void ADC_CallBack( void )
{
    while( !ADC_IsFIFOEmptyFlag(ADC) )
    {
        if( u16ADC_ConversionCount < 16 )
        {
            u16ADC_ConversionBuff[u16ADC_ConversionCount++] = ADC_ReadResultReg(ADC);
        }
        else
        {
            ADC_ReadResultReg(ADC);
        }
    }

    u8ADC_ConversionFlag = 1;
}
/********************************************************************/
