/*
 Arduino-compatibility layer for MSF Lite.
 
*/
#include "arduino.h"

#include <stdio.h>	/* for sprintf */



/* Array which maps the pin numbers used in Arduino to
 * codes of pins used in this board. 
 * The Arduino pin number is used as index into this array. 
 * Value at given index N is the pin code for the Arduino pin N.
 * */
const uint32_t g_msf_pins_arduino[] =
{
	PD0, PD1, PD2, PD3, PD4,
	PD5, PD6, PD7, PD8, PD9,
	PD10, PD11, PD12, PD13, PD14,
	PD15, PD16, PD17, PD18, PD19,
	PD20, PD21, PD22,
};

/* Array which maps the analog pin numbers used in Arduino to
 * codes of analog pins used in this board. 
 * The Arduino pin number is used as index into this array. 
 * Value at given index N is the pin code for the Arduino pin N.
 * */ 
const uint32_t g_msf_analogpins_arduino[] =
{
	PA0, PA1, PA2, PA3, PA4, PA5, PA6,	 
};

/** In this variable we keep record of which timer channels were already set to
 * PWM mode (so we can only change the channel value in analogWrite) and do
 * not need to set the channel to PWM mode.
 * Each byte is reserved for 1 FTM module, byte 0 is FTM0, byte 1 is FTM2, etc.
 * Each bit within the byte is 1 channel of the timer, bit 0 in byte 0 is FTM0 channel 0,
 * bit 1 is FTM0 channel 1, etc. 
 */
uint32_t g_msf_awritechannels;

/** Prescaler value for timers according to user-defined PWM frequency 
 * in msf_config.h.
 * Prescaler values for frequency:
 * 128 - 244 Hz
 * 64 - 488 H
 * 32 - 977 Hz
 * 16 - 1953 Hz
 * 8 - 3906 Hz
 * 4 - 7.812 kHz
 * 2 - 15.625 kHz
 * 1 - 31.250 Khz
 * 
 * */
#if MSF_AWRITE_244HZ
	#define	MSF_AWRITE_PRESCALER	(MSF_FTM_PRESCALER_128)	/* freq. 244 Hz */
#elif MSF_AWRITE_500HZ
	#define	MSF_AWRITE_PRESCALER	(MSF_FTM_PRESCALER_64)	/* freq. 488 Hz */
#elif MSF_AWRITE_1KHZ
	#define	MSF_AWRITE_PRESCALER	(MSF_FTM_PRESCALER_32)	/* freq. 977 kHz */
#elif MSF_AWRITE_2KHZ
	#define	MSF_AWRITE_PRESCALER	(MSF_FTM_PRESCALER_16)	/* freq. 1.953 kHz */
#elif MSF_AWRITE_4KHZ
	#define	MSF_AWRITE_PRESCALER	(MSF_FTM_PRESCALER_8)	/* freq. 3.906 kHz */
#elif MSF_AWRITE_8KHZ
	#define	MSF_AWRITE_PRESCALER	(MSF_FTM_PRESCALER_4)	/* freq. 7.812 kHz */
#elif MSF_AWRITE_16KHZ
	#define	MSF_AWRITE_PRESCALER	(MSF_FTM_PRESCALER_2)	/* freq. 15.625 kHz */
#elif MSF_AWRITE_32KHZ
	#define	MSF_AWRITE_PRESCALER	(MSF_FTM_PRESCALER_1)	/* freq. 31.250 Hz */
#else
	#define	MSF_AWRITE_PRESCALER	(MSF_FTM_PRESCALER_64)	/* default freq. 488 Hz */
#endif
/** Internal function which must be called by main before calling setup().
 * It will initialize the Arduino-compatibility layer.
 * NOTE: this assumes the timer clock is 8 MHz, which is true for CLOCK_SETUP = 1
 * and  2 (see msf_config.h).
 * For the other clock setups the values must be re-calculated.
 * See msf_<device>.h, the "FTM timer definitions" section for the 
 * timer clocks for all clock options.
 * */
void arduino_init()
{
	g_msf_awritechannels = 0;
	
	// Initialize the timers for analogWrite
	// MOD = Ft/Fo - 1 = Fsrc / (Fo . PRESCALER) - 1
	// If we want Fo = 500 Hz
	// For timer clock Fsrc = 8 MHz:
	// PRSC = 128: Fo = 62500; MOD = 124	(too little resolution)
	// PRSC = 64: Fo = 125000; MOD = 250	(ok if we change the frequency a little)
	// Fo = Ft/(MOD + 1) = 125000/256 = 488.2 Hz
	// Fo = Fsrc/(MOD+1)*PRESCALER

#if (MSF_DRIVER_FTM2)    		
	Driver_FTM2.Initialize(null);	
	Driver_FTM2.Control(MSF_FTM_PRESCALER_SET, MSF_AWRITE_PRESCALER);
	Driver_FTM2.Control(MSF_FTM_MOD_VALUE, 130);	
#endif

#if (MSF_DRIVER_FTM1)    	
	Driver_FTM1.Initialize(null);	
	Driver_FTM1.Control(MSF_FTM_PRESCALER_SET, MSF_AWRITE_PRESCALER);
	Driver_FTM1.Control(MSF_FTM_MOD_VALUE, 130);	
#endif

#if (MSF_DRIVER_FTM0)    	
	Driver_FTM0.Initialize(null);	
	Driver_FTM0.Control(MSF_FTM_PRESCALER_SET, MSF_AWRITE_PRESCALER);
	Driver_FTM0.Control(MSF_FTM_MOD_VALUE, 130);	
#endif

	

	
}


/* ---------------------- Analog write ------------------------ */
/* internal function for analogWrite
 * Convert value 0 - 255 used in Arduino for analogWrite to
 * the value which is written to FTM register CnV (depends on modulo)
 * */
static inline uint32_t val_to_cnval(int value)
{
	// See arduino_init(); We set up the timer so that there 
	// is no conversion needed.
	return value;
}


/** Start generating PWM signal on given pin.  
 * @param Arduino pin number 0 to 21 (not all pins supported, see note below!). You do not need to send
 * 	the pin to output mode using pinMode.
 * @param value between 0 and 255; 0 means full time low, 255 means full time high on the pin.
 * @return none
 * @note The PWM frequency is 488 Hz by default but can be changed in msf_config.h 
 * Not all pins can be used to really generate PWM (only those connected to timer can).
 * For pins 14 and 15 the function does nothing. This is different behaviour than on Arduino, where
 * the pin which is not available on timer will be all time low for values 0 - 127 and all time high
 * for values 128 - 255. This makes the code more complicated (handle pin mode and direction) and
 * makes little sense to do. On the other hand, almost all pins can be used for PWM here while on Arduino
 * there are only few. :)  
 * 
 * 
 * Pins with PWM support: 
 * 0  (PTB0) - N/A
 * 1  (PTB1) - N/A
 * 2  (PTD5) - N/A
 * 3  (PTA0) - FTM0_CH0
 * 4  (PTD4) - N/A
 * 5  (PTC3) - FTM2_CH3
 * 6  (PTB4) - n/a (FTM2_CH4) shared with SPI0_MISO PIN (pin 12 BLE shield) 
 * 7  (PTA7) - N/A
 * 8  (PTC5) - n/a (FTM1_CH1)
 * 9  (PTH2) - N/A
 * 10 (PTB5) - FTM2_CH5 
 
 * 11 (PTH0) - FTM2_CH0 (shared with 16)
 * 12 (PTH1) - FTM2_CH1 (shared with 17)
 * 13 (PTE7) - FTM1_CH1 (shared with 8)
 
 * 14 (PTA2) - N/A
 * 15 (PTA3) - N/A
 * 16 (PTC0) - n/a (FTM2_CH0)
 * 17 (PTC1) - n/a (FTM2_CH1)
 * 18 (PTF4) - N/A
 * 19 (PTF5) - N/A
 * 20 (PTF6) - N/A
 * 21 (PTF7) - N/A
 
 * 
 * Pins which are defined as default for analogWrite. No need to change msf_config.h to use these pins with 
 * analogWrite:
 * 2,3,4,5,7,8,10,12,18,19. 
 * 
 * Note about pins which use the same timer channel (e.g. pin 6 and pin 2):
 * Only one of the pins can be used at a time. This is defined in msf_config.h, see the 
 * "Pin configuration for the timer TPMn drivers." section. The pins with lower numbers are
 * generally pre-defined, except for pins 0 and 1. The alternate pin definitions are given in comment. 
 * IMPORTANT: if you do not configure the pin in msf_config.h, analogWrite on this pin will not work!
 * 
 * Note on using the same pin for analogWrite and digital read/write:
 * This should be avoided...
 * If you use a pin for analogWrite and then want to use it in digital mode, set it to input/output 
 * mode using pinMode and then you can use digital Read/Write. But note that the pin will not work
 * with analogWrite again, because the analogWrite will not attach the pin to the timer channel, because 
 * it will still think it is already attached (see g_msf_awritechannels).
 * 
 * Note about implementation: Arduino initializes all the timers to fixed frequency and analogWrite just connects
 * timer pins to the timer as required. 
 * Our implementation also initializes the timers to freq. about 488 Hz and 
 * does not set channels to PWM mode (this would attach the pin).
 * 
 * 
 * 
 * */
void analogWrite(int pin, int value)
{
	MSF_DRIVER_FTM* pDrv = null;
	int channel = -1;
	uint32_t channel_mask, module_pos;
	
	// We use direct mapping of pin-number to timer "object" and channel using this switch...
	// This should allow fast code for const pins
	switch ( pin )
	{
	case 3:
		pDrv = &Driver_FTM0;
		channel = 0;
		module_pos = 0;
		break;
	case 5:
		pDrv = &Driver_FTM2;
		channel = 3;
		module_pos = 2;			
		break;
	case 6:
		pDrv = &Driver_FTM2;
		channel = 4;
		module_pos = 2;			
		break;
	case 8:
		pDrv = &Driver_FTM1;
		channel = 1;
		module_pos = 1;			
		break;
	case 10:
		pDrv = &Driver_FTM2;
		channel = 5;
		module_pos = 2;			
		break;
	
	case 11:
		pDrv = &Driver_FTM2;
		channel = 0;
		module_pos = 2;			
		break;
	case 12:
		pDrv = &Driver_FTM2;
		channel = 1;
		module_pos = 2;			
		break;
	case 13:
		pDrv = &Driver_FTM1;
		channel = 1;
		module_pos = 1;			
		break;
		
		
	case 16:
		pDrv = &Driver_FTM2;
		channel = 0;
		module_pos = 2;			
		break;
	case 17:
		pDrv = &Driver_FTM2;
		channel = 1;
		module_pos = 2;			
		break;
	default:			
		channel = -1;
	
	}
	if ( channel >= 0 )
	{
		channel_mask = (1UL << (8*module_pos+channel));	// see g_msf_awritechannels comment
		if ( (channel_mask &  g_msf_awritechannels) == 0 )
		{
			g_msf_awritechannels |= channel_mask;
			pDrv->ChannelSetMode(channel, PWM_edge_hightrue, 0);
		}
		// write the new value
		pDrv->ChannelWrite(channel, val_to_cnval(value));
	}
}


/* ---------------------- end Analog write ------------------------ */
/* not arduino API - workaround to switch PWM to GPIO*/
void analogWriteDisable(int pin)
{
	MSF_DRIVER_FTM* pDrv = null;
	int channel = -1;
	uint32_t channel_mask, module_pos;
	
	// We use direct mapping of pin-number to timer "object" and channel using this switch...
	// This should allow fast code for const pins
	switch ( pin )
	{
	case 3:
		pDrv = &Driver_FTM0;
		channel = 0;
		module_pos = 0;
		break;
	case 5:
		pDrv = &Driver_FTM2;
		channel = 3;
		module_pos = 2;			
		break;
	case 6:
		pDrv = &Driver_FTM2;
		channel = 4;
		module_pos = 2;			
		break;
	case 8:
		pDrv = &Driver_FTM1;
		channel = 1;
		module_pos = 1;			
		break;
	case 10:
		pDrv = &Driver_FTM2;
		channel = 5;
		module_pos = 2;			
		break;
		
	case 11:
		pDrv = &Driver_FTM2;
		channel = 0;
		module_pos = 2;			
		break;
	case 12:
		pDrv = &Driver_FTM2;
		channel = 1;
		module_pos = 2;			
		break;
	case 13:
		pDrv = &Driver_FTM1;
		channel = 1;
		module_pos = 1;			
		break;
	
	case 16:
		pDrv = &Driver_FTM2;
		channel = 0;
		module_pos = 2;			
		break;
	case 17:
		pDrv = &Driver_FTM2;
		channel = 1;
		module_pos = 2;			
		break;
	default:			
		channel = -1;
	
	}
	pDrv->ChannelSetMode(channel, Disabled, 0);
	channel_mask = (1UL << (8*module_pos+channel));	// see g_msf_awritechannels comment
	g_msf_awritechannels &= ~channel_mask;   //when disabled, next analogWrite() needs set mode
	
}


/* ---------------------- Serial ------------------------ */

/* the worked for Serial.begin()
 * We allow baudrate as any number but only certain values are supported:
 * Supported baudrate values: 
 * 2400, 4800, 9600, 19200, 38400, 57600, 115200
 * If invalid value is given, the baudrate will be set to 9600.
 * */
static void  SerialBegin( uint32_t baudrate)
{     
	UART_speed_t baud = BD9600;	/* default value */
	
	switch(baudrate) {
	case 2400:
		baud = BD2400;
		break;
	case 4800:
		baud = BD4800;
		break;
	case 9600:
		baud = BD9600;
		break;
	case 19200:
		baud = BD19200;
		break;
	case 38400:
		baud = BD38400;
		break;
	case 57600:
		baud = BD57600;
		break;
	case 115200:
		baud = BD115200;
		break;
	
	}
	
	coniob_init(baud);
	
}

/* Implements Serial.end 
 * Disables the UART module. Does not touch the Rx, Tx pins - not needed.
 * To use the Tx, Rx pins in GPIO mode simply set the direction and use it as needed. */
static void  SerialEnd(void)
{   
	// do nothing for now
	//Driver_UART0.Uninitialize();	
}

/** Print text to serial line */
static void SerialPrint(const char* str)
{
	coniob_puts(str);	
}

/* print a string and move to new line.
 * Note that you can also write "\n" in the string and use Serial.print
 * example: Serial.print("hello\n"); == Serial.println("hello\n"); */
static void SerialPrintLn(const char* str)
{
	coniob_puts(str);
	coniob_putch('\n');
}

/** implements Serial.available
 * Get the number of bytes (characters) available for reading from the serial port. 
 * This is data that's already arrived and stored in the serial receive buffer (which holds 64 bytes).
 *  */
static int SerialAvailable(void)
{
	return coniob_kbhit();
}

/* Waits for the transmission of outgoing serial data to complete
 * */
static void SerialFlush(void)
{
	/* This is not available in coniob API, so we have to use the
	 * UART driver directly; but use the symbolic name defined in coniob.h
	 * so that we access the same driver which conio uses */
	while (CONIOB_UART_DRIVER.GetTxCount() > 0) 
		msf_delay_ms(1);
}

/* Returns the next byte (character) of incoming serial data without removing it from the internal serial buffer. 
 * That is, successive calls to peek() will return the same character, as will the next call to read().
 * @return the first byte of incoming serial data available (or -1 if no data is available) 
 * */
static int SerialPeek(void)
{
	int n = (int)coniob_peek();
	if (n == 0 )
		return -1;
	else
		return n;	
}

/** Reads incoming serial data. 
 * @return the first byte of incoming serial data available (or -1 if no data is available) */
static int SerialRead(void)
{
	int n = (int)coniob_getch();
	if (n == 0)
		return -1;
	else
		return n;
}

/** Reads characters from the internal buffer of serial driver into given buffer. 
 * The function terminates if the determined length has been read.
 * @return  number of characters placed in the buffer. A 0 means no valid data was found.
 * */
static int SerialReadBytes(char* buffer, int length)
{
	return (int)coniob_gets(buffer, length, 0);
}

/** Reads characters from the internal buffer of serial driver into given buffer. 
 * The function terminates if given terminator character is found or
 * if the given length has been read.
 * @return  number of characters placed in the buffer. A 0 means no valid data was found.
 * */
static int SerialReadBytesUntil(char terminator, char* buffer, int length)
{
	return (int)coniob_gets(buffer, length, 0);
}

/** Write byte to serial line.
 * This is limited version of the Arduino function; it just sends 1 byte.
 * If you want to send a string, use Serial.print(). 
 * */
static void SerialWrite(int val)
{
	coniob_putch((char)val);
}

/** Prints integer number in given format: DEC, HEX, OCT 
 * Note that without C++ features we cannot have function of the same name (print)
 * We do not support binary format as standard C print does not have it.
 */
static void SerialPrintInt(int val, int format)
{
	/* We have to use a temporary buffer for the string. 
	 * If the buffer is local variable (on the stack) this function could be 
	 * re-entrant. If it is global, it would not.
	 * But the UART driver itself is not reentrant... */
	char tmp[12];	
	sprintf(tmp, ARDUINO_FORMAT_2_PRINTF(format), val);
	coniob_puts(tmp);	
}

/** Prints float number with given number of decimal places.
 * Note that the floating point numbers must be enabled for (s)printf in 
 * C/C++ Build > Settings > Librarian set model to: "ewl" 
 * and select print formats: "int_FP" 
 *  */
static void SerialPrintFloat(float val, int decplaces)
{	
	char tmp[12];
	char format[5] = "%.nf";
	if ( decplaces > 9 )
		decplaces = 9;
	if (decplaces < 0 )
		decplaces = 0;
	format[2] = decplaces + '0';
	
	sprintf(tmp, format, val);
	coniob_puts(tmp);	
}

/* Access structure for the Serial "class" */
MSF_SERIAL_ARDUINO Serial = {
  SerialBegin,
  SerialEnd,
  SerialPrint,
  SerialPrintInt,
  SerialPrintFloat,    
  SerialPrintLn,
  SerialAvailable,
  SerialRead,
  SerialReadBytes,
  SerialReadBytesUntil,
  SerialFlush,
  SerialPeek,
  SerialWrite,
};

/* ---------------------- end Serial ------------------------ */

/* ---------------------- SPI ------------------------ */

/* SPI.begin()
 
 * */
static void SPIBegin(void)
{
	Driver_SPI0.Initialize(0,0);
}

/* Implements SPI.begin 
 * Disables the SPI.end
*/ 
static void SPIEnd(void)
{
	Driver_SPI0.Uninitialize();		
}

/* Implements SPI.transfer 
*/ 
static char SPITransfer(const unsigned char data)
{
	return Driver_SPI0.Transfer(data);
}

/* Implements SPI.setDataMode 
*/ 
static void SPIDataMode(uint8_t mode)
{
  switch (mode)
  {
   case SPI_MODE1:
	   Driver_SPI0.Control(MSF_SPI_DATA_MODE1 ,0); 
	 break;
   case SPI_MODE2:
	   Driver_SPI0.Control(MSF_SPI_DATA_MODE2 ,0); 
   	 break;
   case SPI_MODE3:
	   Driver_SPI0.Control(MSF_SPI_DATA_MODE3 ,0); 
   	 break;
   default:
	   Driver_SPI0.Control(MSF_SPI_DATA_MODE0 ,0);  /** default mode = SPI_MODE0 **/ 	   
  }	 
}

/* Implements SPI.BitOrder 
*/ 
static void SPIBitOrder(uint8_t order)
{
 if (order == LSBFIRST) 
	 Driver_SPI0.Control(MSF_SPI_ORDER_LSB ,0); 
 else  	 
	 Driver_SPI0.Control(MSF_SPI_ORDER_MSB ,0);
}


void SPIsetClockDivider(uint8_t divider)
{
	  switch (divider)
	  {
	   case SPI_CLOCK_DIV2:
		 Driver_SPI0.Control(MSF_SPI_BAUD_SET ,SBD6000KHZ); 
		 break;
	   case SPI_CLOCK_DIV4: /*default 4MHz for 16MHz boards*/
		 Driver_SPI0.Control( MSF_SPI_BAUD_SET , SBD4000KHZ); 
	   	 break;
	   case SPI_CLOCK_DIV8:
		 Driver_SPI0.Control(MSF_SPI_BAUD_SET ,SBD2000KHZ); 
	   	 break;
	   case SPI_CLOCK_DIV16:
		 Driver_SPI0.Control(MSF_SPI_BAUD_SET ,SBD1000KHZ); 
	   	 break;
	   case SPI_CLOCK_DIV32:
		 Driver_SPI0.Control(MSF_SPI_BAUD_SET ,SBD500KHZ); 
	   	 break;
	   case SPI_CLOCK_DIV64:
		 Driver_SPI0.Control(MSF_SPI_BAUD_SET ,SBD250KHZ); 
	   	 break;	 
	   	 
	   default:
	   Driver_SPI0.Control(MSF_SPI_BAUD_SET ,SBD4000KHZ);  /** default mode = SPI_MODE0 **/ 	   
	  }	 	
}
/* Access structure for the SPI "class" */

MSF_SPI_ARDUINO SPI = {
  SPIBegin,
  SPIEnd,
  SPITransfer,
  SPIDataMode,
  SPIBitOrder,
  SPIsetClockDivider,
};

