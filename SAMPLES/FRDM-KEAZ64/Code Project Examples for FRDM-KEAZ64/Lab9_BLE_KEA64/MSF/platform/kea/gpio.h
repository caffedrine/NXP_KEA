/****************************************************************************
 * @file     gpio.h
 * @brief    Define GPIO interface for Freescale Kinetis MCUs 
 * @version  1
 * @date     29. Apr 2014
 *
 * @note     
 *
 ******************************************************************************/
#ifndef MSF_GPIO_H
#define MSF_GPIO_H


/* The inline functions in this file disable interrupts and therefore need
 the core definitions for the MCU (coredef.h). BUT it should not be included here; the
 source file (C/Cpp) should always include coredef.h before including msf.h */
/* Make sure the definitions of baudrate constants etc. are available */
#ifndef MSF_MSF_H
	#error Plese include msf.h before including this file.
#endif

/* Note: we assume user includes msf.h before including this file.
the platform header includes the device specific files as selected in msf_config.h */
//#include "kinetis.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup group_msf_digitalIO  
* @{ 
*/

/** MCU_pin_t 
 @brief The data type for MCU pin.
 @note This is the type expected by the GPIO functions for the pin paremeter.
 The MCU_pin_type is defined in msf_lite/platform/kinetis/mkl25z/msf_mkl25z.h.
*/
typedef MCU_pin_type  MCU_pin_t;

/* -------------- Basic I/O function  ---------------- */ 
/** Macro to obtain pin number from pin code */
// R200040 returns pin number 0..7
#define		GPIO_PIN_NUM(pin)		((uint8_t)((uint16_t)pin >> 8)) 

// R200040 returns port number A=0, B=1, C=2

#define		GPIO_PORT_NUM(pin)		((uint8_t)((uint16_t)pin & 0x007F))  // 0x7F to eliminate port flag 

/** Macro to obtain bit mask of a pin from its code */
// R200040 returns 0x01 0x02 0x04 ... 0x80000000 (32 bit)
// #define		GPIO_PIN_MASK(pin)		(1 << GPIO_PIN_NUM(pin) )
#define		GPIO_PIN_MASK(pin)		(unsigned int)((1 << (GPIO_PIN_NUM(pin))) << ((GPIO_PORT_NUM(pin) % 4) * 8))

// Macros to obtain the addresses of various I/O registers from pin code
/** Obtain the address of the GPIO_x structure from pin code. Port number (A=0, etc. is encoded
 * as lowest byte in pin code */
//#define		GPIO_GPIO_OBJECT(pin)		((GPIO_Type *)(PTA_BASE + (PTB_BASE-PTA_BASE)*(pin & 0x007F)))
#define		GPIO_GPIO_OBJECT(pin)		((GPIO_Type *)(GPIOA_BASE + (GPIOB_BASE-GPIOA_BASE)*((pin & 0x007F)>>2)))
// GPIOA_BASE

/** Obtain the address of the PORT_x structure from pin code. Port number (A=0, etc. is encoded
 * as lowest byte in pin code */
//R200040 #define		GPIO_PORT_OBJECT(pin)		((PORT_Type *)(PORTA_BASE + (PORTB_BASE-PORTA_BASE)*(pin & 0x00FF)))
//#define		GPIO_PORT_OBJECT(pin)		((PORT_Type *)(PORTA_BASE + (PORTB_BASE-PORTA_BASE)*(pin & 0x00FF)))

#define GPIO_PORT_PULLUP(pin) *((&PORT_PUE0 + (&PORT_PUE1 - &PORT_PUE0)*((pin & 0x007F)>>2)))

// Macros which allow us to refer to the I/O registers directly
#define		GPIO_DDR_REG(pin)    	GPIO_GPIO_OBJECT(pin)->PDDR
#define		GPIO_INPUTDIS_REG(pin)  GPIO_GPIO_OBJECT(pin)->PIDR
#define		GPIO_TOGGLE_REG(pin)	GPIO_GPIO_OBJECT(pin)->PTOR
#define		GPIO_SET_REG(pin)		GPIO_GPIO_OBJECT(pin)->PSOR
#define		GPIO_CLEAR_REG(pin)		GPIO_GPIO_OBJECT(pin)->PCOR
#define		GPIO_DATAIN_REG(pin)	GPIO_GPIO_OBJECT(pin)->PDIR

/** @brief Directions for GPIO pins 
 @note only supported modes should be defined. 
 */
typedef enum {
  input = 0,
  output = 1,  
} GPIO_pin_direction;

/** @brief Options for pull-up/pull-down resistors 
*/
typedef enum {
  pull_none,
  pull_up,
  pull_down,  
} GPIO_pin_pullup;


/** @brief Initialises the GPIO ports.
 * It enables the clock to GPIO ports.
 */
static inline void gpio_init()
{
	/* R200040 No clock gating for GPIO on KEA
     * need to disable NMI pin B4  for GPIO functionality
     */
	SIM->SOPT0 &= ~SIM_SOPT0_NMIE_MASK;
	
	/*SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK
			| SIM_SCGC5_PORTB_MASK
			| SIM_SCGC5_PORTC_MASK
			| SIM_SCGC5_PORTD_MASK
			| SIM_SCGC5_PORTE_MASK );	
*/
}

/**
 * @brief Set the direction of given pin
 * @param pin the pin to configure.
 * @param dir the direction to set.
 * @return none.
 * Set the direction of a given pin to input or output.
 * Does not change pull-up settings.
*/
#if MSF_USE_INLINE
    inline
#endif
static void gpio_direction(MCU_pin_t pin, GPIO_pin_direction dir) 
{
  	// Set the pin multiplexer to GPIO mode
    // GPIO_PORT_OBJECT(pin)->PCR[GPIO_PIN_NUM(pin)] = PORT_PCR_MUX(1);
    // R200040 No multiplex PCR register on      
    
	switch ( dir )
	{
	case input:
		GPIO_DDR_REG(pin) &= ~(GPIO_PIN_MASK(pin));
		GPIO_INPUTDIS_REG(pin) &= ~(GPIO_PIN_MASK(pin));
	break;
	
	case output:		
		GPIO_DDR_REG(pin) |= GPIO_PIN_MASK(pin);    
	break;
	
	default:
	/* TODO: howto handle invalid inputs? 
	maybe error handler function? probably too complicated for simple and fast function;
	maybe define "debug" which enables this? */
	break; 
	}
}

    

/**
 * @brief Enable/disable pull-up register
 * @param pin the pin to configure.
 * @param mode the pull up enabled or disabled.
 * @return none.
 * Set the state of pull-up register for GPIO pin. 
 * The pin should be input! 
 * Does not check or change the pin direction. 
 * Note: pins which do not support pull-up/down selection or enable/disable 
 * the resistor should not be written? (data sheets says they are read only).
 * Note: No pins on MKL25Z support pull up/down select; all are hard-wired!
 * Special pins: 
 *  for PTA0 PS = 0 (read only)
 *  for PTA0 and PTA2 PE = 1 but is configurable?
 *  PTB0, PTB1, PTD6, PTD7 - high current drive pins.
 *   
*/
#if MSF_USE_INLINE
    inline
#endif
static void gpio_pullmode(MCU_pin_t pin, GPIO_pin_pullup mode)
{ 
    switch ( mode )
    {
    case pull_none:
    	/* disable pull resistor for given pin */
    	GPIO_PORT_PULLUP(pin) &= ~GPIO_PIN_MASK(pin);    	
    	break;
    
    case pull_up:        
    	/* enable pull resistor for given pin */
    	GPIO_PORT_PULLUP(pin) |= GPIO_PIN_MASK(pin);
    	
        break;        
    
    case pull_down:
    	//R200040 pull down is not available on KEA
    	    	
    	break;
    default:
        break; 
    }
}
    



/**
 * @brief set pin to high or low state
 * @param pin the pin to set
 * @param set true to set high, false to clear (set low) the pin
 * @return none. 
 * The pin should be output!
 * Does not check or change the pin direction.    
 */ 
#if MSF_USE_INLINE
    inline
#endif
static void gpio_write(MCU_pin_t pin, bool set)
{
  if ( set )
	  GPIO_SET_REG(pin) |= GPIO_PIN_MASK(pin);
  else
	  GPIO_CLEAR_REG(pin) |= GPIO_PIN_MASK(pin);
} 

/**
 * @brief read logical level at given pin
 * @param pin the pin to read
 * @return true if the pin is high (log. 1), false if it is low (log. 0). 
 * @note The pin should be input!
 * Does not check or change the pin direction.    
 */ 
#if MSF_USE_INLINE
    inline
#endif
static bool gpio_read(MCU_pin_t pin)
{
  if ( (GPIO_DATAIN_REG(pin) & GPIO_PIN_MASK(pin)) == 0 ) 
    return false;
  else
    return true;  
}

/**
 * @brief toggle the value of given output pin 
 * @param pin the pin to toggle
 * @return none. 
 * @note The pin should be output!
 * Does not check or change the pin direction.    
 */ 
#if MSF_USE_INLINE
    inline
#endif
static void gpio_toggle(MCU_pin_t pin)
{    
    GPIO_TOGGLE_REG(pin) |= GPIO_PIN_MASK(pin);
}

/** @}*/

#ifdef __cplusplus
}
#endif

#endif  /* MSF_GPIO_H */
