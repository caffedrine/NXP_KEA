/********************************************************************************
*    LAB10A - Local Interconnect Interface Communication (LIN), Master 			*
*    FRDM+KEA64 Board															*
*                                                                             	*
*    This lab demonstrates how to communicate between two FRDM boards			*
*    using the LIN communication protocol.  The FRDM KEA comes with a LIN		*
*    PHY that the MCU can control using its UART peripheral					  	*
*    										                                  	*
*  	 Two boards are required to run this demo, one running the master program	*
*  	 and one running the slave.  J11 is the LIN PHY, on which three pins 	 	*
*  	 provide the LIN, supply voltage (Vsup), and ground (GND) signals,			*
*  	 respectively.  Connect the two boards with GND to GND, Vsup to Vsup, and	*
*  	 LIN to LIn using three blue wires.											*
*
*  	 Once your boards are connected and running, turn the master board's		*
*  	 potentiometer to toggle the slave's red LED.  Also press the slave board's	*
*  	 push buttons to toggle the master's blue and green LEDs.					*
********************************************************************************/

#include "derivative.h" /* include peripheral declarations */
#include "LIN_Driver/lowlevel/lin.h"
#include "ADC.h"
#include "spi.h"
#include "ics.h"

/**********************************************************************************************
* Constants and macros
**********************************************************************************************/
#define LED0_TOGGLE		OUTPUT_TOGGLE(PORT_H,24)
#define LED1_TOGGLE		OUTPUT_TOGGLE(PORT_H,25)
#define LED2_TOGGLE		OUTPUT_TOGGLE(PORT_E,7)

#define LED0_OFF		OUTPUT_CLEAR(PORT_H,24);
#define LED1_OFF		OUTPUT_CLEAR(PORT_H,25);
#define LED2_OFF		OUTPUT_CLEAR(PORT_E,7);

#define LED0_ON			OUTPUT_SET(PORT_H,24);
#define LED1_ON			OUTPUT_SET(PORT_H,25);
#define LED2_ON			OUTPUT_SET(PORT_E,7);

#define OUTPUT  1
#define INPUT	0

#define PORT_A  A
#define PORT_B  A
#define PORT_C  A
#define PORT_D  A

#define PORT_E	B
#define PORT_F	B
#define PORT_G	B
#define PORT_H	B

#define CONFIG_PIN_AS_GPIO(port,register_number,mode)    XCONFIG_PIN_AS_GPIO(port,register_number,mode)
#define XCONFIG_PIN_AS_GPIO(port,register_number,mode)   (mode == 0) ? (GPIO##port##_PDDR |= 0 << register_number) : (GPIO##port##_PDDR |= 1 << register_number)

#define ENABLE_INPUT(port,pin_number) 					XENABLE_INPUT(port,pin_number)
#define XENABLE_INPUT(port,pin_number)					GPIO##port##_PIDR ^= 1<<pin_number

#define ENABLE_PULLUP(port,pin_number) 					XENABLE_PULLUP(port,pin_number)
#define XENABLE_PULLUP(port,pin_number) 				PORT_PUE0 |= PORT_PUE0_PT##port##PE##pin_number##_MASK

#define OUTPUT_SET(port,register_num)				XOUTPUT_SET(port,register_num)
#define XOUTPUT_SET(port,register_num)				GPIO##port##_PCOR |=1<<register_num

#define OUTPUT_CLEAR(port,register_num)				XOUTPUT_CLEAR(port,register_num)
#define XOUTPUT_CLEAR(port,register_num)			GPIO##port##_PSOR |=1<<register_num

#define OUTPUT_TOGGLE(port,register_num)			XOUTPUT_TOGGLE(port,register_num)
#define XOUTPUT_TOGGLE(port,register_num)			GPIO##port##_PTOR |=1<<register_num

//#define MOTOR_SELECTION_INCREASE  1
//#define MOTOR_SELECTION_DECREASE  2
//#define MOTOR_SELECTION_STOP      3

//#define MOTOR1_OVER_TEMP   200
//#define MOTOR1_MAX_TEMP   100
//#define MOTOR1_MIN_TEMP   30

/* Global variables */
l_u8 LIN_counter =0, LED_counter = 0;
l_u8 Motor1_temp;

//Comments
uint8_t spi_SBC_INIT_SEQ_PART0[2]={0xDF, 0x80};		//Read Vreg register H
uint8_t spi_SBC_INIT_SEQ_PART1[2]={0x5A, 0x00};		//Enter in Normal mode
uint8_t spi_SBC_INIT_SEQ_PART2[2]={0x5E, 0x90};		//Enable 5V-CAN and Vaux regulators
uint8_t spi_SBC_INIT_SEQ_PART3[2]={0x60, 0xC0};		//Set CAN in TxRx mode, fast slew rate
uint8_t spi_SBC_INIT_SEQ_PART4[2]={0x66, 0xC4};		//Set LIN1 in TxRx mode, slew rate 20kb/s, LIN term ON

uint8_t spi_data_receive16bit[2]={0, 0};

/***********************************************************************************************
*
* @brief    CLK_Init - Initialize the clocks to run at 20 MHz from the 10Mhz external XTAL
* @param    none
* @return   none
*
************************************************************************************************/
void Clk_Init()
{
	/*core clock frequency using an external 8MHz crystal */
	OSC_CR = 0x96; // high-range, high-gain oscillator selected
	while ((OSC_CR & OSC_CR_OSCINIT_MASK) == 0); // wait until oscillator is ready
	ICS_C2 = 0x20; // BDIV=divide by 2 – use default until clock dividers configured
	ICS_C1 = 0x18; // 8MHz external reference clock/256 as source to FLL
	while ((ICS_S & ICS_S_IREFST_MASK) == 1); // wait for external source selected
	while ((ICS_S & ICS_S_LOCK_MASK) == 0); // wait for FLL to lock
	SIM_CLKDIV = 0x01000000; // core clock = ICSOUT/1 and bus clock = core clock/2
	ICS_C2 = 0x00; // BDIV=divide by 1 – allows max core and bus clock frequencies

//	ICS_C1 |=ICS_C1_IREFS_MASK;           /* Selection of intern clock to FLL */
//	ICS_C1|=ICS_C1_IRCLKEN_MASK; 		/* Enable the internal reference clock*/
//	ICS_C1 |=ICS_C1_CLKS(0b00);          /* Clock source select   Output FLL is selected */
//	ICS_C2 = (ICS_C2 & ~(ICS_C2_BDIV_MASK)) | ICS_C2_BDIV(2);
//	SIM_CLKDIV|=SIM_CLKDIV_OUTDIV2_MASK;  /*Core frequency divide by 2 for Bus freq*/
//	ICS_S |= ICS_S_LOCK_MASK ; 			/* Clear Loss of lock sticky bit */


}
/***********************************************************************************************
*
* @brief    GPIO_Init - Initialize the pins for input/output
* @param    none
* @return   none
*
************************************************************************************************/
void GPIO_Init()
{
		CONFIG_PIN_AS_GPIO(PORT_H,24,OUTPUT); /* Configure LED 0 (PTC0) as an output */
		CONFIG_PIN_AS_GPIO(PORT_H,25,OUTPUT); /* Configure LED 1 (PTC1) as an output */
		CONFIG_PIN_AS_GPIO(PORT_E,7,OUTPUT); /* Configure LED 2 (PTC2) as an output */

		CONFIG_PIN_AS_GPIO(PORT_E,4,INPUT); /* Configure BTN0 (PTD0) as an input */
		CONFIG_PIN_AS_GPIO(PORT_E,5,INPUT); /* Configure BTN1 (PTD1) as an input */
		ENABLE_INPUT(PORT_E,4);			 /* Enable input SW1*/
		ENABLE_INPUT(PORT_E,5);			/*  Enable input SW2*/

		LED0_OFF;							/* Turn off LED0 */
		LED1_OFF;							/* Turn off LED1 */
		LED2_OFF;							/* Turn off LED2 */

		//CONFIG_PIN_AS_GPIO(PORT_H,24,OUTPUT); /* Configure LED 0 (PTC0) as an output */
		//OUTPUT_SET(PORT_H,24);

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
	SIM_SCGC |= SIM_SCGC_FTM0_MASK; /* Enable Clock for FTM0 */
	FTM0_SC |= FTM_SC_PS(7);	/* Select Preescaler in this case 128. 20 Mhz /128 =156.25 Khz. */
									/* Counter increase by one every 6.4 us */
		/* Enable Channle 0*/
	FTM0_C0SC |= FTM_CnSC_CHIE_MASK; /* Enable channel 0 interrupt */
	FTM0_C0SC |= FTM_CnSC_MSA_MASK;  /* Channel as Output compare mode */
		/*Select interrupt frequency*/
	FTM0_C0V = FTM_CnV_VAL(391) ;	 	/* Interrupt every 2.5ms */

	FTM0_SC |= FTM_SC_CLKS(1); /*FTM0 use system clock*/

	/* Set the ICPR and ISER registers accordingly */

	//NVIC_ICPR |= 1 << ((INT_FTM0-16)%32);
	//NVIC_ISER |= 1 << ((INT_FTM0-16)%32);

	NVIC_ClearPendingIRQ(FTM0_IRQn);
	NVIC_EnableIRQ(FTM0_IRQn);
}

int main(void)
{
	int i = 10000;
	int counter = 0;
	uint8_t adc_value=0;
	//Clk_Init();
	GPIO_Init();
	ADC_Init(10,0);

	ICS_ConfigType ics_config={0};
	/*FLL Engaged external*/
	ics_config.u8ClkMode=ICS_CLK_MODE_FEE;
	/* Bdiv=1*/
	ics_config.bdiv=0;

	/*Oscillator high range*/
	ics_config.oscConfig.bRange=1;
	/*Oscillator clock source selected*/
	ics_config.oscConfig.bIsCryst=1;
	/* Oscillator enable in stop*/
	ics_config.oscConfig.bStopEnable=1;
	/*8 MHz oscillator*/
	ics_config.oscConfig.u32OscFreq=8000;
	/*Enable external oscillator*/
	ics_config.oscConfig.bEnable=1;

	ICS_Init(&ics_config);


	SPI_ConfigType sSPIConfig = {{0}};
    /* initialize SPI1 as master    */
    sSPIConfig.u32BitRate = 500000;		//Comment - min frequency shall be 250kHZ	115200;
    sSPIConfig.u32BusClkHz = 20000000;
    sSPIConfig.sSettings.bModuleEn             = 1;
    sSPIConfig.sSettings.bMasterMode           = 1;
    sSPIConfig.sSettings.bClkPhase1            = 1;
    sSPIConfig.sSettings.bClkPolarityLow       = 0;
    sSPIConfig.sSettings.bMasterAutoDriveSS    = 0;		//Comment - to drive CS manually for 16-bit operation -> MODFEN = 0

    SPI_Init(SPI1, &sSPIConfig);



    //Comment - configure PTD3 as a digital output -> will be driven as a chip select manually
    //CS = High
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


	l_sys_init();
	l_ifc_init(LI0);
	lin_application_timer_FTM0();
	l_sch_set(LI0, LI0_NormalTable, 0);

//	NVIC_ICPR |= 1 << ((INT_UART1-16)%32);
//	NVIC_ISER |= 1 << ((INT_UART1-16)%32);

	NVIC_ClearPendingIRQ(UART1_IRQn);
	NVIC_EnableIRQ(UART1_IRQn);


	for(;;) {	   


		adc_value=ADC_Read(10); /* Read ADC value from channel 10 */

		/* Send ADC value to slave*/
		l_u8_wr_LI0_ADC_signal(adc_value);

		/* Read button 1 state from slave*/
		if(1==l_u8_rd_LI0_Button1_signal())
		{
			LED1_ON;
		}
		else
		{
			LED1_OFF;
		}

		/* Read button 2 state from slave*/
		if(1==l_u8_rd_LI0_Button2_signal())
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


void FTM0_IRQHandler()
  {
    if (1==((FTM0_C0SC & FTM_CnSC_CHF_MASK)>>FTM_CnSC_CHF_SHIFT) )  /* If the CHF of the channel is equal to 0 */
  	{
  		(void)FTM0_C0SC;  							/* Read to clear flag */
  		FTM0_C0SC ^= FTM_CnSC_CHF_MASK;  			/* Clear flag */
  		FTM0_C0V = FTM0_C0V + 391 ; /* Refresh interrupt period */

  		if (LIN_counter>=6){
  		    /* Activate LIN frame transfer for every 15ms */
  		    l_sch_tick(LI0);
  		    /* Reset counter */
  		    LIN_counter = 0;
  		  }

	  LIN_counter++;


  	}
  }
