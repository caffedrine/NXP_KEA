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
* @file FRDM_KEA64_LIN_Master.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief providing framework of demo cases for MCU.
		This is the demo for communication between two FRDM+KEA64
*       boards. The Master sends the ADC port value and
*       reads back the key pressed at slave board and sets the
*       corresponding LEDs.
******************************************************************************/

#include "derivative.h" /* include peripheral declarations */
#include "LIN_Driver/lowlevel/lin.h"
#include "adc.h"
#include "spi.h"
#include "ics.h"
#include "gpio.h"
#include "nvic.h"
#include "ftm.h"


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

/* Function Prototypes */
void FTM0_ISR(void);

/***********************************************************************************************
*
* @brief    GPIO_Init - Initialize the pins for input/output
* @param    none
* @return   none
*
************************************************************************************************/
void GPIO_Init()
{
		CONFIG_PIN_AS_GPIO(PTH,PTH0,OUTPUT); 		/* Configure RED LED (PTH0) as an output */
		CONFIG_PIN_AS_GPIO(PTH,PTH1,OUTPUT); 		/* Configure GREEN LED (PTH1) as an output */
		CONFIG_PIN_AS_GPIO(PTE,PTE7,OUTPUT); 		/* Configure BLUE (PTE7) as an output */

		CONFIG_PIN_AS_GPIO(PTE,PTE4,INPUT); 		/* Configure BTN0 (SW2-PTE4) as an input */
		CONFIG_PIN_AS_GPIO(PTE,PTE5,INPUT); 		/* Configure BTN1 (SW3-PTE5) as an input */
		ENABLE_INPUT(PTE,PTE4);			 			/* Enable input SW2 */
		ENABLE_INPUT(PTE,PTE5);						/* Enable input SW3 */

		LED0_OFF;									/* Turn off RED */
		LED1_OFF;									/* Turn off GREEN */
		LED2_OFF;									/* Turn off BLUE*/

}
/***********************************************************************************************
*
* @brief   lin_application_timer_FTM0 - Initialize the timer for LIN application
* @param    none
* @return   none
*
************************************************************************************************/
void lin_application_timer_FTM0()
{
	SIM_SCGC |= SIM_SCGC_FTM0_MASK; 			/* Enable Clock for FTM0 */
	FTM0_SC |= FTM_SC_PS(7);					/* Select Preescaler in this case 128. 20 Mhz /128 =156.25 Khz. */
												/* Counter increase by one every 6.4 us */
	/* Enable Channel 0*/
	FTM0_C0SC |= FTM_CnSC_CHIE_MASK; 			/* Enable channel 0 interrupt */
	FTM0_C0SC |= FTM_CnSC_MSA_MASK;  			/* Channel as Output compare mode */

	/* Select interrupt frequency*/
	FTM0_C0V = FTM_CnV_VAL(391) ;	 			/* Interrupt every 2.5ms */

	FTM0_SC |= FTM_SC_CLKS(1); 					/* FTM0 use system clock*/

	FTM_SetCallback(FTM0, FTM0_ISR);

	/* Set the ICPR and ISER registers accordingly */
	Enable_Interrupt(FTM0_IRQn);

}

int main(void)
{

	int counter = 0;
	l_u8 __attribute__ ((unused)) ret;
	uint8_t adc_value = 0;
	GPIO_Init();

	/* Initialization of clock in FLL Engaged external */
	ICS_ConfigType ics_config={0};
	ics_config.u8ClkMode=ICS_CLK_MODE_FEE;
	ics_config.bdiv=0;							/* Bdiv=1*/
	ics_config.oscConfig.bRange=1;				/* Oscillator high range */
	ics_config.oscConfig.bIsCryst=1;			/* Oscillator clock source selected */
	ics_config.oscConfig.bStopEnable=1;			/* Oscillator enable in stop */
	ics_config.oscConfig.u32OscFreq=8000;		/* 8 MHz oscillator */
	ics_config.oscConfig.bEnable=1;				/* Enable external oscillator */

	ICS_Init(&ics_config);						/* Initialize Core clock at 40 MHz and Bus clock at 20 MHz */

	/* Initialization of ADC module */
    ADC_ConfigType  ADC_Config={{0}};
	ADC_Config.u8ClockDiv = ADC_ADIV_DIVIDE_4;	/* Divide clock by 4 */
	ADC_Config.u8ClockSource = CLOCK_SOURCE_BUS_CLOCK;	/* Select bus as a clock source */
	ADC_Config.u8Mode = ADC_MODE_8BIT;			/* ADC in 8bit conversion mode */
	ADC_Config.sSetting.bContinuousEn=1;		/* Enable continuous conversion */
	ADC_Config.u16PinControl= 0x0400; 			/* Disable I/O control on ADC channel 10*/

	ADC_Init( ADC, &ADC_Config);				/* Initialize ADC, in 8-bit conversion mode */

    /* Initialize SPI1 as master to enable SBC for LIN */
	SPI_ConfigType sSPIConfig = {{0}};
    sSPIConfig.u32BitRate = 500000;				/* Comment - min frequency shall be 250kHZ	115200 */
    sSPIConfig.u32BusClkHz = 20000000;
    sSPIConfig.sSettings.bModuleEn             = 1;
    sSPIConfig.sSettings.bMasterMode           = 1;
    sSPIConfig.sSettings.bClkPhase1            = 1;
    sSPIConfig.sSettings.bClkPolarityLow       = 0;
    sSPIConfig.sSettings.bMasterAutoDriveSS    = 0;		/* Comment - to drive CS manually for 16-bit operation -> MODFEN = 0 */

    SPI_Init(SPI1, &sSPIConfig);				/* Initialize SPI1, as a master */

	/* Enable SBC for LIN */
	Enable_SBC();

	/* Initialization for LIN protocol */
	ret = l_sys_init();
	ret = l_ifc_init(LI0);
	lin_application_timer_FTM0();
	l_sch_set(LI0, LI0_NormalTable, 0);

	Enable_Interrupt(UART1_IRQn);

	while(1)
	{
		adc_value=ADC_PollRead(ADC,ADC_CHANNEL_AD10);	/* Read ADC value from channel 10 */

		l_u8_wr_LI0_ADC_signal(adc_value);				/* Send ADC value to slave*/

		if(1==l_u8_rd_LI0_Button1_signal())				/* Read BTN0 state from slave*/
		{
			LED1_ON;
		}
		else
		{
			LED1_OFF;
		}


		if(1==l_u8_rd_LI0_Button2_signal())				/* Read BTN1 state from slave*/
		{
			LED2_ON;
		}
		else
		{
			LED2_OFF;
		}

	   	counter++;
	}
	
	return 0;
}


void FTM0_ISR()
  {
    if (1==((FTM0_C0SC & FTM_CnSC_CHF_MASK)>>FTM_CnSC_CHF_SHIFT) )  /* If the CHF of the channel is equal to 0 */
  	{
  		(void)FTM0_C0SC;  											/* Read to clear flag */
  		FTM0_C0SC ^= FTM_CnSC_CHF_MASK;  							/* Clear flag */
  		FTM0_C0V = FTM0_C0V + 391 ; 								/* Refresh interrupt period */

  		if (LIN_counter>=6){
  		    l_sch_tick(LI0);  		    							/* Activate LIN frame transfer for every 15ms */
  		    LIN_counter = 0;			  		    				/* Reset counter */
  		  }

	  LIN_counter++;


  	}
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
