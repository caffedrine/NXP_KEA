/****************************************************************************
 * @file     msf_config.h
 * @brief    User configuration for this application 
 * @note     This is the file included by user application. It specifies 
 * the configuration for this project and should be created separately for
 * each project.
 * The options which can easily be configured are located in this file directly.
 * Other options which can be configured with care are located in the
 * msf_config_mkl25z.h file included from the frdm_kl25z.h     
 *
 ******************************************************************************/
#ifndef MSF_CONFIG_H
	#define MSF_CONFIG_H


/*********************************************
*    Define the CPU frequency and BUS frequency.
********************************************/
/* How? 
 * Look into Project_Settings\Startup_Code\system_SKEAZxxxx.c
 * and set the CLOCK_SETUP macro there to select one of the available CPU
 * frequencies. Then define the F_CPU below to match the selected frequency.
 *
 * Note: These CLOCK_SETUP values 0 thru 4 are supported 
 */

/* CLOCK_SETUP = 1;
 * CLOCK_SETUP = 4; */

#define F_CPU		(48000000)
#define	F_BUS		(24000000)


/* CLOCK_SETUP = 2; */
/*
#define F_CPU		(4000000)
#define	F_BUS		(800000)
*/

/* CLOCK_SETUP = 3; */
/*
#define F_CPU		(4000000)
#define	F_BUS		(1000000)
*/

/* CLOCK_SETUP = 0; default value in new project */
/*
#define F_CPU		(20970000)
#define	F_BUS		(20970000)
*/

// ------------------------
// Older values supported by MSF but not available in system_MKL25Z4.c anymore:
// #define F_CPU == 20900000   /* BUS = 13,98 MHz */
	/*The default clock if no clock init is performed. We can clock UART from PLLFLLCLK which is 20.9 MHz
	 * Note that we assume PLLFLLSEL = 0 (MCGFLLCLK clock)!
 	 * BUS clock 13,98 MHz
 	 */
//#define   F_CPU   (41943040) 	/* not available in CLOCK_SETUP in KDS 1.1.1 */
	/* The F_CPU 41943040 was available before and is supported by MSF,
	 * so I keep it here for possible use.
	 * The settings were:
	 * Multipurpose Clock Generator (MCG) in FLL Engaged Internal (FEI) mode
       Reference clock source for MCG module is the slow internal clock source 32.768kHz
       Core clock = 41.94MHz, BusClock = 13.98MHz
	 */
//#define #define   F_CPU   (8000000)	/* not available in CLOCK_SETUP in KDS 1.1.1 */
/* The F_CPU 8000000 was available before and is supported by MSF,
	 * so I keep it here for possible use.
	 * The settings were:
		Multipurpose Clock Generator (MCG) in Bypassed Low Power External (BLPE) mode
         Core clock/Bus clock derived directly from an external crystal 8MHz with no multiplication
         Core clock = 8MHz, BusClock = 8MHz
*/


/*********************************************
*    Define the standard I/O channel 
********************************************/
#define   MSF_USE_STDIO     1   /* Using conio driver (UART) for std output */
/* To disable initializing UART in msf_init use:
#define   MSF_USE_STDIO     0 
*/
/* Define the default baudrate used by console I/O (conio).
 * This must be one of the values from the enum defined in msf_<device>.h. */
#define	 MSF_STDIO_BAUDRATE		(BD19200)

/*********************************************
*    Define whether we want to use analog inputs
*    If nonzero; ADC is initialized. This means
*    little more power consumption, otherwise no
*    harm is done by enabling the analog inputs even
*    if not used 
********************************************/
#define MSF_USE_ANALOG      1

/*********************************************
*    Define which drivers should be created (used) 
*    It may save some memory to define drivers which 
*    your application will not use with 0.
********************************************/
#define MSF_DRIVER_UART0    1   /* Use UART0 driver (create UART0 driver instance) */
#define	MSF_DRIVER_UART1	1	/* Use UART1 driver */
#define	MSF_DRIVER_UART2	1	/* Use UART2 driver */
#define	MSF_DRIVER_SPI0		1	/* Use SPI0 driver */
#define	MSF_DRIVER_SPI1		0	/* Use SPI1 driver */
#define MSF_DRIVER_ADC0     1	/* Use ADC driver */
#define	MSF_DRIVER_FTM0		1	/* Use FTM0 driver */
#define	MSF_DRIVER_FTM1		1	/* Use FTM1 driver */
#define	MSF_DRIVER_FTM2		1	/* Use FTM2 driver */

/*********************************************
* Here you can select which pin is used for each UART Rx and Tx from
* the available pins for that UART.
* You will need to look into the datasheet into the Pinout chapter
* (in Signal Multiplexing and Signal Descriptions) to find out which
* pins are available for each UART.
* Or check the comment for each pin here :)
* Define the name of the pin as defined in msf_<device>.h for you MCU
* and the number of the ALTx function as found in the pinout table in
* the datasheet.
**********************************************/

/* UART0 pins */
#define		MSF_UART0_RX_PIN	(GPIO_A2)	/* PTA2/2; PTB0/2 */
#define		MSF_UART0_RX_ALT	(0)         /* PINSEL0 (UART0PS) PTB = 0; PTA = 1 */
#define		MSF_UART0_TX_PIN	(GPIO_A3)	/* PTA3/2; PTB1/2 */
#define		MSF_UART0_TX_ALT	(0)         

/* UART1 pins */
#define		MSF_UART1_RX_PIN	(GPIO_C6)	/* PTC6/2; PTF2/2 */
#define		MSF_UART1_RX_ALT	(0)         /* PINSEL1 (UART1PS) PTC = 0; PTF = 1 */
#define		MSF_UART1_TX_PIN	(GPIO_C7)	/* PTC7/2; PTF3/2  */
#define		MSF_UART1_TX_ALT	(0)

/* UART2 pins */
#define		MSF_UART2_RX_PIN	(GPIO_D6)	/* PTI0/3; PTD6/2 */
#define		MSF_UART2_RX_ALT	(0)         /* PINSEL2 (UART2PS) PTD = 0; PTI = 1 */
#define		MSF_UART2_TX_PIN	(GPIO_D7)	/* PTI1/3; PTD7/2 */
#define		MSF_UART2_TX_ALT	(0)


/*********************************************
* Here you can select which pin is used for each SPI MISO/MOSI/CLK from
* the available pins for that SPI.
* You will need to look into the datasheet into the Pinout chapter
* (in Signal Multiplexing and Signal Descriptions) to find out which
* pins are available for each SPI.
* Or check the comment for each pin here :)
* Define the name of the pin as defined in msf_<device>.h for you MCU
* and the number of the ALTx function as found in the pinout table in
* the datasheet.
**********************************************/

/* SPI0 pins */
#define		MSF_SPI0_MISO_PIN	(GPIO_D3)	
#define		MSF_SPI0_MISO_ALT	(2)
#define		MSF_SPI0_MOSI_PIN	(GPIO_D2)	
#define		MSF_SPI0_MOSI_ALT	(2)
#define		MSF_SPI0_SCK_PIN	(GPIO_D1)	
#define		MSF_SPI0_SCK_ALT	(2)

/* SPI1 pins */
#define		MSF_SPI1_MISO_PIN	(GPIO_E3)	
#define		MSF_SPI1_MISO_ALT	(2)
#define		MSF_SPI1_MOSI_PIN	(GPIO_E1)	
#define		MSF_SPI1_MOSI_ALT	(2)
#define		MSF_SPI1_SCK_PIN	(GPIO_E2)	
#define		MSF_SPI1_SCK_ALT	(2)


/*********************************************
*   Define the PWM frequency for Arduino analogWrite
*	Define one of the values as 1, all others as 0
*	Note that the values in the macros are approximate (rounded), the
*	exact values are given in comment to analogWrite in Arduino.c 
********************************************/
#define	MSF_AWRITE_244HZ	0
#define	MSF_AWRITE_500HZ	0//1
#define	MSF_AWRITE_1KHZ		0
#define	MSF_AWRITE_2KHZ		0
#define	MSF_AWRITE_4KHZ		0
#define	MSF_AWRITE_8KHZ		0
#define	MSF_AWRITE_16KHZ	1
#define	MSF_AWRITE_32KHZ	0


/*********************************************
* Pin configuration for the timer FTMn drivers.
* Here you can select which pin is used for each channel from
* the available pins for this channel.
* You will need to look into the datasheet into the Pinout chapter
* (in Signal Multiplexing and Signal Descriptions) to find out which
* pins are available for each channel.
* Define the name of the pin as defined in msf_<device>.h for you MCU
* and the number of the ALTx function as found in the pinout table in 
* the datasheet.
**********************************************/

/* Arduino note:
 * For the channels which are available on multiple Arduino-pinout pins,
 * the lower pin numbers are pre-defined here. 
 * Definitions for the other pins are given in comments.
 * Exceptions are pins 0 and 1 which are used also for UART so we pre-define
 * the alternate pins 18 and 19 as PWM pins. If you want to use pin 0 and 1 
 * for PWM, un-comment the definitions for these pins below.
 * */


/* Timer FTM0 pins (2 channels are available) */ 
/* Choose the pin multiplex by comment/uncomment *_PIN *_ALT pair */
// Timer 0, channel 0
#define		MSF_FTM0_CH0_PIN	(GPIO_A0)		// Arduino pin 3
#define		MSF_FTM0_CH0_ALT	(0)         	// SIM_PINSEL0[FTM0PS0] = 0 (A0)
//#define		MSF_FTM0_CH0_PIN	(GPIO_B2)	// Arduino pin X 
//#define		MSF_FTM0_CH0_ALT	(1)         // SIM_PINSEL0[FTM0PS0] = 1 (B2) 

// Timer 0, channel 1
#define		MSF_FTM0_CH1_PIN	(GPIO_A1)		// Arduino pin X
#define		MSF_FTM0_CH1_ALT	(0)         	// SIM_PINSEL0[FTM0PS1] = 0 (A1)
//#define		MSF_FTM0_CH1_PIN	(GPIO_B3)	// Arduino pin X 
//#define		MSF_FTM0_CH1_ALT	(1)         // SIM_PINSEL0[FTM0PS1] = 1 (B3)

/* Timer FTM1 pins (2 channels are available) */
// Timer 1, channel 0
#define		MSF_FTM1_CH0_PIN	(GPIO_C4)		// Arduino pin X 
#define		MSF_FTM1_CH0_ALT	(0)         	// SIM_PINSEL0[FTM1PS0] = 0 (C4)
//#define		MSF_FTM1_CH0_PIN	(GPIO_H2)	// Arduino pin X 
//#define		MSF_FTM1_CH0_ALT	(1)         // SIM_PINSEL0[FTM1PS0] = 1 (H2)

// Timer 1, channel 1
//#define		MSF_FTM1_CH1_PIN	(GPIO_C5)		// Arduino pin 8
//#define		MSF_FTM1_CH1_ALT	(0)         	// SIM_PINSEL0[FTM1PS1] = 0 (C5)
#define		MSF_FTM1_CH1_PIN	(GPIO_E7)	// Arduino pin X
#define		MSF_FTM1_CH1_ALT	(1)         // SIM_PINSEL0[FTM1PS1] = 1 (E7)


/* Timer FTM2 pins (2 channels are available) */
// Timer 2, channel 0
//#define		MSF_FTM2_CH0_PIN	(GPIO_C0)		// Arduino pin 16 
//#define		MSF_FTM2_CH0_ALT	(0)         	// SIM_PINSEL1[FTM2PS0] = 0 (C0)
#define		MSF_FTM2_CH0_PIN	(GPIO_H0)	// Arduino pin X
#define		MSF_FTM2_CH0_ALT	(1)         // SIM_PINSEL1[FTM2PS0] = 1 (H0)

//#define		MSF_FTM2_CH0_PIN	(GPIO_F0)	// Arduino pin X
//#define		MSF_FTM2_CH1_ALT	(x)         // SIM_PINSEL1[FTM2PS0] = 2 (F0)


// Timer 2, channel 1
//#define		MSF_FTM2_CH1_PIN	(GPIO_C1)		// Arduino pin 17
//#define		MSF_FTM2_CH1_ALT	(0)         	// SIM_PINSEL1[FTM2PS1] = 0 (C1)

#define		MSF_FTM2_CH1_PIN	(GPIO_H1)	// Arduino pin X
#define		MSF_FTM2_CH1_ALT	(1)         // SIM_PINSEL1[FTM2PS1] = 1 (H1)

//#define		MSF_FTM2_CH1_PIN	(GPIO_F1)	// Arduino pin X
//#define		MSF_FTM2_CH1_ALT	(2)         // SIM_PINSEL1[FTM2PS1] = 2 (F1)

// Timer 2, channel 2
#define		MSF_FTM2_CH2_PIN	(GPIO_C2)		// Arduino pin X
#define		MSF_FTM2_CH2_ALT	(0)         	// SIM_PINSEL1[FTM2PS2] = 0 (C2)

//#define		MSF_FTM2_CH2_PIN	(GPIO_D0)	// Arduino pin X
//#define		MSF_FTM2_CH2_ALT	(1)         // SIM_PINSEL1[FTM2PS2] = 1 (D3)

//#define		MSF_FTM2_CH2_PIN	(GPIO_G4)	// Arduino pin X
//#define		MSF_FTM2_CH2_ALT	(2)         // SIM_PINSEL1[FTM2PS2] = 2 (G4)


// Timer 2, channel 3
#define		MSF_FTM2_CH3_PIN	(GPIO_C3)		// Arduino pin 5
#define		MSF_FTM2_CH3_ALT	(0)         	// SIM_PINSEL1[FTM2PS3] = 0 (C3)

//#define		MSF_FTM2_CH3_PIN	(GPIO_D1)	// Arduino pin X
//#define		MSF_FTM2_CH3_ALT	(1)         // SIM_PINSEL1[FTM2PS3] = 1 (D1)

//#define		MSF_FTM2_CH3_PIN	(GPIO_G5)	// Arduino pin X
//#define		MSF_FTM2_CH3_ALT	(2)         // SIM_PINSEL1[FTM2PS3] = 2 (G5)

// Timer 2, channel 4
#define		MSF_FTM2_CH4_PIN	(GPIO_B4)		// Arduino pin 6
#define		MSF_FTM2_CH4_ALT	(0)         	// SIM_PINSEL1[FTM2PS4] = 0 (B4)

//#define		MSF_FTM2_CH4_PIN	(GPIO_G6)	// Arduino pin x
//#define		MSF_FTM2_CH4_ALT	(1)         // SIM_PINSEL1[FTM2PS4] = 1 (G6)

// Timer 2, channel 5
#define		MSF_FTM2_CH5_PIN	(GPIO_B5)		// Arduino pin x
#define		MSF_FTM2_CH5_ALT	(0)         	// SIM_PINSEL1[FTM2PS5] = 0 (B5)

//#define		MSF_FTM2_CH5_PIN	(GPIO_G7)	// Arduino pin x
//#define		MSF_FTM2_CH5_ALT	(1)         // SIM_PINSEL1[FTM2PS5] = 1 (G7)


 /* Include the header file for our board */
#include "frdm_kea.h" 

#endif  /* MSF_CONFIG_H */
