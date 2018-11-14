/*
 Arduino-compatibility layer for MSF Lite.
 Include this file  in your application if you want to use the Arduino API.
 You do not need to include any other MSF files.
*/

#ifndef MSFL_ARDUINO_H
#define MSFL_ARDUINO_H

/* We include all the needed files here so that it is easy for the user - just include arduino.h in your source */

/* MSF configuration for this project */
#include "msf_config.h"

/* Include the core definitions - CMSIS <device>.h file etc.*/
#include "coredef.h"

#include "msf.h"
#include "drv_adc.h"  /* ADC driver needed for analogReference() */
//#include "drv_uart.h"	/* UART driver needed for Serial.begin() */
#include "coniob.h"		/* buffered serial line for Serial */
#include "drv_ftm.h"	/* timer for analogWrite */

#include "drv_spi.h" /* spi routines */

//Board used


/* Arduino pin definitions 
 * Digital pin n is named PDn etc.
 */
#define		PD0				(GPIO_B0)
#define		PD1				(GPIO_B1)
#define		PD2				(GPIO_D5)
#define		PD3				(GPIO_A0)
#define		PD4				(GPIO_D4)
#define		PD5				(GPIO_C3)
#define		PD6				(GPIO_B4)
#define		PD7				(GPIO_A7)

#define		PD8				(GPIO_C5)  //used for ACI (RDNY) nRF8001 (SPI)
#define		PD9				(GPIO_H2)  //used for ACI (REQN) nRF8001 (SPI)

#define		PD10			(GPIO_B5) 

/* Replaced by RGB LEDs */
//#define		PD11			(GPIO_B3)   // used for ACI nRF8001 (SPI)
//#define		PD12			(GPIO_B4)   // used for ACI nRF8001 (SPI) 
//#define		PD13			(GPIO_B2)   // used for ACI nRF8001 (SPI)
#define		PD11			(GPIO_H0)   // FRDM+RED
#define		PD12			(GPIO_H1)   // FRDM+GREEN 
#define		PD13			(GPIO_E7)   // FRDM+BLUE

//#define		PD14			(GPIO_A2)	/* Pins 14 and 15 are not available on older */
//#define		PD15			(GPIO_A3)	/* version of Arduino Uno (before R3 pinout) */

#define		PD14			(GPIO_E4)	//FRDM+SW2
#define		PD15			(GPIO_E5)	//FRDM+SW3


/* The pins above PD15 are analog inputs but can be also used as digital pins */
#define		PD16			(GPIO_C0)
#define		PD17			(GPIO_C1)
#define		PD18			(GPIO_F4)
#define		PD19			(GPIO_F5)
#define		PD20			(GPIO_F6)
#define		PD21			(GPIO_F7)
#define		PD22			(GPIO_C2)  // FRDM + Potentiometer

/* Names of the analog pins when used in analog mode */
#define		PA0				(AIN_C0)
#define		PA1				(AIN_C1)
#define		PA2				(AIN_F4)
#define		PA3				(AIN_F5)
#define		PA4				(AIN_F6)
#define		PA5				(AIN_F7)
#define		PA6				(AIN_C2)

/* Names of the SPI pins Arduino pins*/
#define  	MOSI			(11) //D2 = pin 11
#define  	MISO			(12)  //D3 = pin 12
#define		SCK				(13)   //D1  = pin 13



/* Number of pins on this board */
//#define		MSF_ARDUINO_PINS	(22)
#define		MSF_ARDUINO_PINS	(23)

/* Number of analog pins on this board */
//#define		MSF_ARDUINO_APINS	(6)
#define		MSF_ARDUINO_APINS	(7)

/* Note about Arduino compatibility on the FRMD-KL25Z board:
   * The Arduino pins A0 thru A5 are:
   * A0 = SE8 (PTB0)
   * A1 = SE9 (PTB1)
   * A2 = SE12 (PTB2)
   * A3 = SE13 (PTB3)
   * A4 = SE11 (PTC2)
   * A5 = SE15 (PTC1) 
   * On the FRDM board the pin names refer to the MCU port + pin, e.g. Arduino "A0"
   * is names "B0" on the board (Port B, pin 0 of the MCU) */


/* Array for converting digital pin number to pin-code
 * Defined in arduino.c */
extern const uint32_t g_msf_pins_arduino[];

/* Array for converting analog pin number to pin-code
 * Defined in arduino.c */
extern const uint32_t g_msf_analogpins_arduino[];

/** Arduino symbols for digital I/O */
#define		HIGH	(1)
#define		LOW		(0)
#define		INPUT	(0)
#define		OUTPUT	(1)
#define		INPUT_PULLUP	(2)

/** Arduino defines for analog reference */
#define		DEFAULT		(0)		/* about 3V */
#define		EXTERNAL	(1)		/* VREFH pin */

/* Arduino formats for printing to serial line */
#define		DEC			(0)
#define		HEX			(1)
#define		OCT			(2)
/*#define		BIN			(3) */

#define		ARDUINO_FORMAT_2_PRINTF(format)  ((format == DEC) ?  "%d" : \
											 (format == HEX) ?  "%X" : \
											 (format == OCT) ?  "%o" : \
											 "")  
#define interrupts() __enable_irq()
#define noInterrupts() __disable_irq()


/** SPI defines **/
#define LSBFIRST (0)
#define MSBFIRST (1)

#define SPI_MODE0 (0x00)
#define SPI_MODE1 (0x04)
#define SPI_MODE2 (0x08)
#define SPI_MODE3 (0x0C)

#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV32 0x06 
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03

/*Arduino typedefs*/
typedef bool boolean;
typedef uint8_t byte;

/** Internal function which must be called by main before calling setup().
 * It will initialize the Arduino-compatibility layer.
 * Implemented in arduino.c */
void arduino_init();


/* Arduino pin number to pin-code conversion */
/* Convert Ardiuno pin number into pin code used by MSF GPIO functions. 
 * @returns Pin code or GPIO_INVALID_PIN if the pin number is out of range */
static inline uint32_t pin2pincode(int pin)
{
	if ( pin < MSF_ARDUINO_PINS )
		return g_msf_pins_arduino[pin];
	else
		return GPIO_INVALID_PIN;
}

/* Arduino analog pin number (0-5) to pin-code conversion */
/* Convert Ardiuno pin number into pin code used by MSF analog read function. 
 * @returns Pin code or AIN_ADC_DISALED if the pin number is out of range */
static inline uint32_t analogpin2pincode(int pin)
{
	if ( pin < MSF_ARDUINO_APINS )
		return g_msf_analogpins_arduino[pin];
	else
		return AIN_ADC_DISALED;
}

/***********************
 *  Digital functions 
 *********************** */

/** Set pin mode to INPUT, INPUT_PULLUP or OUTPUT
 * @param pin Arduino pin number (e.g. 13)
 * @param mode INPUT, INPUT_PULLUP or OUTPUT
 * */
static inline void pinMode(int pin, int mode)
{
	uint32_t pin_code = pin2pincode(pin);
	switch ( mode) 
	{
	case INPUT:
		msf_pin_direction(pin_code, input);
		msf_pin_pullmode(pin_code, pull_none);
		break;
		
	case INPUT_PULLUP:
		msf_pin_direction(pin_code, input);
		msf_pin_pullmode(pin_code, pull_up);
		break;	
		
	case OUTPUT:
		msf_pin_direction(pin_code, output);
		break;
	}
		
}

/** Write digital output pin to HIGH or LOW 
 * @param pin Arduino pin number (e.g. 13)
 * @param val HIGH or LOW
 * */
static inline void digitalWrite(int pin, int val)
{
	msf_pin_write(pin2pincode(pin), (bool)val);	
}

/** Read state of digital input pin 
 * @param pin Arduino pin number (e.g. 13)
 * @return HIGH or LOW 
 * */
static inline int digitalRead(int pin)
{
	return (msf_pin_read(pin2pincode(pin))) ? HIGH : LOW;		
}

/***********************
 *  Analog functions 
 *********************** */


/** Set the reference for ADC.
 * @param type the reference type to set; DEFAULT or EXTERNAL. Both are 3V on FRDM-KL25Z
 * @note Arduino supports:
 * DEFAULT - e.g. 5V on Uno
 * INTERNAL - depends on MCU, e.g. 1.1 V for UNO (atmega328)
 * EXTERNAL - AREF pin.
 * and some other for Arduino Mega only. 
 * 
 * We support only DEFAULT and EXTERNAL.
 * Note that FRDM-KL25Z has the VREFH (AREF) pin connected to power supply (3V).
 * If you want to use external voltage, you have to cut trace SH1
 * and put wire (0R resistor) in place of R3.
 * The VALT is hard-wired to analog power VDDA (3V).
 * So in fact without touching the hardware, both the options have
 * the same effect. On altered hw, the default should be the power supply,
 * which is VALT
 * */
static inline void analogReference(int type)
{
	/* If any (invalid) value other than EXTERNAL is received,
	 * we use DEFAULT */
	if ( type == EXTERNAL ) 
	{
//R200040		Driver_ADC0.Control(MSF_ADC_REFSEL_DEFAULT, 0);
	}
	else 
	{
//R200040		Driver_ADC0.Control(MSF_ADC_REFSEL_ALT, 0);
	}
}

/** Read digital value from analog pin using ADC. 
 * @param Arduino pin number (0 to 5)
 * @return 16-bit ADC value (0 thru 65535)
 * */
static inline int analogRead(int pin)
{
	return msf_analog_read(analogpin2pincode(pin));
}



/** Start generatin PWM signal on given pin.  
 * @param Arduino pin number 0 to 21 (not all pins supported, see note below!)
 * @param value between 0 and 255; 0 means full time low, 255 means full time log 1 on the pin.
 * @return none
 * @note Only some of the pins can be used to really generate PWM (those connected to timer).
 * For all other pins the function does nothing. This is different behaviour than on Arduino, where
 * the pin which is not available on timer will be all time low for values 0 - 127 and all time high
 * for values 128 - 255. This makes the code more complicated (handle pin mode and direction) and
 * makes little sense to do.  
 * 
 * Implemented in arduino.c
 * */
void analogWrite(int pin, int value); 
void analogWriteDisable(int pin); 

/***********************
 *  Time functions 
 *********************** */

/** 
 * @return number of milliseconds since the board began running the current program 
 * */
static inline uint32_t millis()
{
	return msf_millis();
}

/** 
 * @return number of microseconds since the board began running the current program
 * The value overflows in about 72 minutes and starts over from 0.
 * The resolution is 1 us. 
 * */
static inline uint32_t micros()
{
	return msf_micros();
}


/** Pauses the program for the amount of time (in milliseconds) specified as parameter.
 * @note this function must not be called from interrupt handler (ISR), it will not work correctly.
 * */
static inline void delay(uint32_t millis)
{
	msf_delay_ms(millis);
}

/** Stop the execution for given number of microseconds using busy-wait loop
 * @param micros number of us to wait
 * @note  It is safe to call this function in interrupt handlers (ISR); it will function ok.
 * The function works reliably from certain minimal value (depending on F_CPU):
 *  about 6 us for F_CPU =  8 MHz and 21 MHz; about 2 us for 48 MHz.
 * 	For very short delays use MSF_DELAY_1US or MSF_DELAY_5US macros. 
 * 	The results are better for F_CPU in exact MHz, e.g. 48 MHz  is better than 41.9 MHz - see clock options. 
 *  If interrupt(s) occur while the delay is in progress, the delay time will be longer
 *  (as this is simple busy loop it will execute given number of times, not knowing that 
 *  some time elapsed in ISR.
 *  The maximum delay is about 715 millions of us, but it does not make sense to call this 
 *  function for delays longer than few milliseconds; use delay() instead; combined with delay_us if desired.
 *  
 *  When testing this function note the overhead in measuring the time with micros() which may be
 *  about 26 us! 
 * */
static inline void delayMicroseconds(uint32_t micros)
{
	msf_delay_us(micros);
}

/** Re-maps a number from one range to another. 
 * Does not constrain values to within the range. 
 * */
static inline uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max)
{	
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/** Constrains a number to be within a range.
 * @return val if the val is within the range, low if val < low, high if val > high
 *  */
static inline uint32_t constrain(uint32_t val, uint32_t low, uint32_t high) 
{
	return ((val<low) ? low : (val>high) ? high : val);
}


/***********************
 *  Serial communication functions 
 *********************** */
/** Note that we do not implement the whole functionality of the Serial class in Arduino.
 * This seems too complicated and would require using C++ language in the project.
  * For printing to serial line we just implement the basic functions.
  * We support printing int and float with functions printInt and printFloat as we cannot have
  * more functions with the same name (print) without C++.
  * 
  * For more advanced purposes, please use the MSF native functions:
  *  void msf_print(const char* str);    -  print string  
  *  void msf_printnum(uint32_t number) - print number (integer) 
  *  void msf_printhex(uint32_t number) - print number in hexadecimal notation
  *  
  *  Print string and one formatted number (like printf in C): 
  *  Example: 
  *  To print "X = 5" write: msf_printf16("X = ", "%d", 5);
  *  void msf_printf16(const char* str, const char* format, uint16_t data); - 16-bit integer
  *  void msf_printf32(const char* str, const char* format, uint32_t data); - 32-bit integer
  *  void msf_printf_real(const char* str, const char* format, double data); - floating-point
  *  
  *   Reading characters:
  *   char msf_read_char(void);	- read 1 character 
  *   bool msf_char_available(void);  - return true if a character was received thru serial line.
  *   
  *   Note that the baudrate is 19200 by default. It can be changed in msf_config.h, see MSF_STDIO_BAUDRATE.
 * */
/**
 Access structure which emulates the C++ Serial class in Arduino
 Functions implemented and instance created in arduino.c
*/
typedef struct _MSF_SERIAL_ARDUINO {  
  void      (*begin)   (uint32_t baudrate);  
  void      (*end) (void); 
  void      (*print) (const char* str); 
  void 		(*printInt)(int val, int format);
  void 		(*printFloat)(float val, int decplaces);
  void      (*println) (const char* str); 
  int 		(*available)(void);
  int 		(*read)(void);
  int		(*readBytes)(char* buffer, int length);
  int 		(*readBytesUntil)(char terminator, char* buffer, int length);
  void		(*flush)(void);
  int		(*peek)(void);
  void 		(*write)(int val);
} const MSF_SERIAL_ARDUINO;


/* The instance of Serial defined in arduino.c */
extern MSF_SERIAL_ARDUINO Serial;

/***********************
 *  SPI communication functions 
 *********************** */
/** Note that we do not implement the whole functionality of the Serial class in Arduino.
 * This seems too complicated and would require using C++ language in the project.
  * 
  * For more advanced purposes, please use the MSF native functions:
 * */
/**
 Access structure which emulates the C++ Serial class in Arduino
 Functions implemented and instance created in arduino.c
*/
typedef struct _MSF_SPI_ARDUINO {  
  void      (*begin) (void);  
  void      (*end) (void); 
  char      (*transfer) (const unsigned char data); 
  void 		(*setDataMode)(unsigned char mode);
  void      (*setBitOrder)(unsigned char order); 
  void      (*setClockDivider) (unsigned char divider);
} const MSF_SPI_ARDUINO;

/* The instance of Serial defined in arduino.c */
extern MSF_SPI_ARDUINO SPI;

#endif /* MSFL_ARDUINO_H */
