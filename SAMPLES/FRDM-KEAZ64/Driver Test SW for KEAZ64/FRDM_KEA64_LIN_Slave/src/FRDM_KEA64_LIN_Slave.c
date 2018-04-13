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
* @file FRDM_KEA64_LIN_Slave.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief providing framework of demo cases for MCU.
		This is the demo for communication between two FRDM+KEA64
*       boards. The Slave receives the ADC value sent by Master and
*       changes RED LED accordingly.
*       Sends the key pressed at slave board to the master.
******************************************************************************/

#include "derivative.h" /* include peripheral declarations */
#include "LIN_Driver/lowlevel/lin.h"
#include "spi.h"
#include "ics.h"
#include "gpio.h"
#include "nvic.h"

/* Constants and macros */
#define LED0_TOGGLE		OUTPUT_TOGGLE(PTH,PTH0)
#define LED1_TOGGLE		OUTPUT_TOGGLE(PTH,PTH1)
#define LED2_TOGGLE		OUTPUT_TOGGLE(PTE,PTE7)

#define LED0_OFF		OUTPUT_CLEAR(PTH,PTH0);
#define LED1_OFF		OUTPUT_CLEAR(PTH,PTH1);
#define LED2_OFF		OUTPUT_CLEAR(PTE,PTE7);

#define LED0_ON			OUTPUT_SET(PTH,PTH0);
#define LED1_ON			OUTPUT_SET(PTH,PTH1);
#define LED2_ON			OUTPUT_SET(PTE,PTE7);

/* Global variables */
l_u8 LIN_counter =0, LED_counter = 0;
l_u8 Motor1_temp;

/* For SBC */
void Enable_SBC();
uint8_t spi_SBC_INIT_SEQ_PART0[2]={0xDF, 0x80};		/* Read Vreg register H */
uint8_t spi_SBC_INIT_SEQ_PART1[2]={0x5A, 0x00};		/* Enter in Normal mode */
uint8_t spi_SBC_INIT_SEQ_PART2[2]={0x5E, 0x90};		/* Enable 5V-CAN and Vaux regulators */
uint8_t spi_SBC_INIT_SEQ_PART3[2]={0x60, 0xC0};		/* Set CAN in TxRx mode, fast slew rate */
uint8_t spi_SBC_INIT_SEQ_PART4[2]={0x66, 0xC4};		/* Set LIN1 in TxRx mode, slew rate 20kb/s, LIN term ON */
uint8_t spi_data_receive16bit[2]={0, 0};


/***********************************************************************************************
*
* @brief    GPIO_Init - Initialize the pins for input/output
* @param    none
* @return   none
*
************************************************************************************************/
void GPIO_Init()
{
		CONFIG_PIN_AS_GPIO(PTH,PTH0,OUTPUT); 	/* Configure RED LED (PTH0) as an output */
		CONFIG_PIN_AS_GPIO(PTH,PTH1,OUTPUT); 	/* Configure GREEN LED (PTH1) as an output */
		CONFIG_PIN_AS_GPIO(PTE,PTE7,OUTPUT); 	/* Configure BLUE LED (PTE7) as an output */

		CONFIG_PIN_AS_GPIO(PTE,PTE4,INPUT); 	/* Configure BTN0 (PTE4) as an input */
		CONFIG_PIN_AS_GPIO(PTE,PTE5,INPUT); 	/* Configure BTN1 (PTE5) as an input */
		ENABLE_INPUT(PTE,PTE4);			 		/* Enable input SW2 */
		ENABLE_INPUT(PTE,PTE5);					/* Enable input SW3 */

		LED0_OFF;								/* Turn off RED */
		LED1_OFF;								/* Turn off GREEN */
		LED2_OFF;								/* Turn off BLUE*/

}

int main(void)
{
		l_u8 __attribute__((unused)) ret;
	    uint8_t adc_value=0;
	    GPIO_Init();

		/*Initialization of clock in FLL Engaged external*/
		ICS_ConfigType ics_config={0};
		ics_config.u8ClkMode=ICS_CLK_MODE_FEE;
		ics_config.bdiv=0;						/* Bdiv=1*/
		ics_config.oscConfig.bRange=1;			/* Oscillator high range*/
		ics_config.oscConfig.bIsCryst=1;		/* Oscillator clock source selected*/
		ics_config.oscConfig.bStopEnable=1;		/* Oscillator enable in stop*/
		ics_config.oscConfig.u32OscFreq=8000;	/* 8 MHz oscillator*/
		ics_config.oscConfig.bEnable=1;			/* Enable external oscillator*/

		ICS_Init(&ics_config);					/* Initialize Core clock at 40 MHz, Bus clock at 20 MHz */


		SPI_ConfigType sSPIConfig = {{0}};

	    /* Initialize SPI1 as master to enable SBC for LIN */
	    sSPIConfig.u32BitRate = 500000;			/* Comment - min frequency shall be 250kHZ	115200 */
	    sSPIConfig.u32BusClkHz = 20000000;
	    sSPIConfig.sSettings.bModuleEn             = 1;
	    sSPIConfig.sSettings.bMasterMode           = 1;
	    sSPIConfig.sSettings.bClkPhase1            = 1;
	    sSPIConfig.sSettings.bClkPolarityLow       = 0;
	    sSPIConfig.sSettings.bMasterAutoDriveSS    = 0;		/*Comment - to drive CS manually for 16-bit operation -> MODFEN = 0 */

	    SPI_Init(SPI1, &sSPIConfig);

		/* Enable SBC for LIN */
		Enable_SBC();

		/* Initialization for LIN protocol */
	    ret = l_sys_init();
	    ret = l_ifc_init(LI0);

	    Enable_Interrupt(UART1_IRQn);

	while(1)
	{
		/* When SW2 pressed */
	   	if((GPIOB_PDIR&GPIO_PDIR_PDI(0x0000010))>>4==0)
	   	{
	   		l_u8_wr_LI0_Button1_signal(0x01);

	   	}

	   	else
	   	{
	   		l_u8_wr_LI0_Button1_signal(0x00);
	   	}

		/* When SW3 pressed */
	   	if((GPIOB_PDIR&GPIO_PDIR_PDI(0x0000020))>>5==0)
	   	{
	   		l_u8_wr_LI0_Button2_signal(0x01);
	   	}
	   	else
	   	{
	   		l_u8_wr_LI0_Button2_signal(0x00);
	   	}

	   	/* Receive ADC value from master and turn off the RED LED on crossing threshold value of 100 and above */
	    adc_value= l_u8_rd_LI0_ADC_signal();

	   	if(adc_value < 100)
	   	{
	   		LED0_ON;
	   	}
	   	else
	   	{
	   		LED0_OFF;
	   	}


	}
	
	return 0;
}

/***********************************************************************************************
*
* @brief    Configure PTD3 as a digital output -> will be driven as a chip select manually
	        CS = High
* @param    none
* @return   none
*
************************************************************************************************/
void Enable_SBC()
{

		int i = 0;
	    GPIOA_PDOR = 0x08000000;
	    GPIOA_PDDR = 0x08000000;

	    GPIOA_PDOR = 0x00000000;
	    SPI_TransferWait(SPI1,spi_data_receive16bit,spi_SBC_INIT_SEQ_PART0,2);
	    GPIOA_PDOR = 0x08000000;
	    for(i=0;i<100;i++);
	    GPIOA_PDOR = 0x00000000;
	    SPI_TransferWait(SPI1,spi_data_receive16bit,spi_SBC_INIT_SEQ_PART1,2);
	    GPIOA_PDOR = 0x08000000;
	    for(i=0;i<100;i++);
	    GPIOA_PDOR = 0x00000000;
	    SPI_TransferWait(SPI1,spi_data_receive16bit,spi_SBC_INIT_SEQ_PART2,2);
	    GPIOA_PDOR = 0x08000000;
	    for(i=0;i<100;i++);
	    GPIOA_PDOR = 0x00000000;
	    SPI_TransferWait(SPI1,spi_data_receive16bit,spi_SBC_INIT_SEQ_PART3,2);
	    GPIOA_PDOR = 0x08000000;
	    for(i=0;i<100;i++);
	    GPIOA_PDOR = 0x00000000;
		SPI_TransferWait(SPI1,spi_data_receive16bit,spi_SBC_INIT_SEQ_PART4,2);
		GPIOA_PDOR = 0x08000000;
		for(i=0;i<100;i++);
}
