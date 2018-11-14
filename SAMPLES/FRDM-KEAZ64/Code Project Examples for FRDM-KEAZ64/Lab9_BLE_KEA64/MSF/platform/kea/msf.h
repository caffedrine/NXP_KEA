/****************************************************************************
 * @file     msf.h
 * @brief    root definitions for the framework 
 * @version  1
 * @date     31. Jan 2014
 *
 * @note     Main header file for MSF.  
 *  The implementation should be mostly in platform\...\msf.c but can be also 
 *  in other files.
 *
 ******************************************************************************/
 /**
   \mainpage MSF Lite Software Library
   *
   * <b>Introduction</b>
   *
   * This describes the MSF Lite software library.
   * 
   *  
   * <b>Examples</b>
   *
   * The library ships with a number of examples which demonstrate how to use the library functions.
   * You can find these examples in the msf_lite/examples/[ide]/[board] directory.
   *
   * <b>Toolchain Support</b>
   *
   * The library has been developed and tested with Freescale Kinetis Design Studio 1.1.1.
   * It should be possible to use it with CodeWarrior also, although some changes may be required.
   * 
   * This software is released under the GNU LESSER GENERAL PUBLIC LICENSE version 3 or newer.
   * Author: Jan Dolinay, dolinay(AT)fai.utb.cz   
   *    
   */
#ifndef MSF_MSF_H
#define MSF_MSF_H

#ifndef MSF_COREDEF_H
	#error Please include the coredef.h file before including msf.h
#endif

/* include the core definitions for this platform */
#include "msf_kinetis.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup group_errors MSF Error Codes 
* @details MSF error codes
* @{ 
*/
#define     MSF_ERROR_OK            (0)     /**< success; no error! */
#define     MSF_ERROR_UNKNOWN       (1)     /**< error without further details */
#define     MSF_ERROR_NOTFOUND      (2)     /**< the object does not exit (is not valid) */
#define     MSF_ERROR_NOTSUPPORTED  (3)     /**< required option is not supported  */
#define		MSF_ERROR_SYSTIMER		(4)		/**< failed to initialize system timer  */
#define     MSF_ERROR_ARGUMENT      (6)     /**< invalid input argument for a function */
#define     MSF_ERROR_NOINIT        (7)     /**< device was not properly initialized */
#define     MSF_ERROR_CONFIG        (8)     /**< driver not properly configured; e.g. invalid pins defined in msf_config.h */
#define     MSF_ERROR_TIMEOUT       (9)     /**< the operation timed out */

#define		MSF_ERROR_LAST			(100)	/**< helper constant; the last error code used by MSF globally. */

#define     MSF_ERROR_MAXDWORD      (0xffffffff)    /**< special error indication used by ADC_Read and TPM_ReadChannel */

/** @}*/



/**********************************************
* Basic functions available in MSF API 1
**********************************************/
#include "gpio.h"

/** @defgroup group_globals MSF global functions  
* @{ 
*/
/** 
 * @brief Initialize the MSF. 
 * @details It may do nothing on some platforms for API level 1 but, for example,
 * kinetis needs enabling clock to GPIO ports.
 * @param param not used for now.
 * @return 0 if init was ok; error code otherwise. For now it always returns OK. 
 * @note It uses CMSIS SysTick. The CMSIS implementation must be provided by
 * the silicone vendor. Init the "system" timer which will be used by delay, 
 * millis etc. functions   
 **/
uint8_t     msf_init(uint32_t param);
/** @}*/
    
/** @defgroup group_msf_digitalIO Digital I/O 
* @brief MSF Digital I/O functions
* 
 Note 1: 
The GPIO funcitons are defined static inline in gpio.h
We include gpio.h here for convenience; this way the user does not need to include it
to be able to use the I/O functions.  

Note 2:
We use macros rather than inline functions for the MSF version here, because 
INLINE definition is compiler-specific, so this code would need "#if compiler"
*
* @{ 
*/
 

#define msf_pin_direction(pin, dir)       gpio_direction(pin, dir) 
#define msf_pin_pullmode(pin, mode)   gpio_pullmode(pin, mode) 
#define msf_pin_write(pin, value)         gpio_write(pin, value)
#define msf_pin_read(pin)                   gpio_read(pin)
#define msf_pin_toggle(pin)                gpio_toggle(pin)
/** @}*/


/*****************************************************
* Functions for obtaining system time and variable delays 
*****************************************************/
#if MSF_TIME_API

/** @addtogroup group_globals  
* @{ 
*/

/** @brief Get the number of milliseconds that elapsed since the CPU started.
 * @return number of milliseconds as 32-bit unsigned integer.
 * @note The value overflows in about 49.7 days. (2^32 milliseconds)
 **/
uint32_t    msf_millis(void);

/** @brief Get the number of microseconds that elapsed since the CPU started
 * @return number of microseconds as 32-bit unsigned integer.
 * @note  The value overflows in about 71.5 minutes.
 */
uint32_t    msf_micros(void);

/** @brief Stop the execution for given number of microseconds using busy-wait loop.
 * @param  micros number of microseconds for which the program should wait
 * @return none
 */
void        msf_delay_us(uint32_t micros);
/** @brief Stop the execution for given number of milliseconds using busy-wait loop.
 * @param  millis number of milliseconds for which the program should wait
 * @return none
 * @note We rely on the CMSIS SysTick service 
 */ 
void        msf_delay_ms(uint32_t millis);

/** @}*/

#endif /* MSF_TIME_API */


/*****************************************************
* Functions for very short delays (e.g. 1 us) 
*****************************************************/
#if MSF_SHORT_DELAY_API
/* Very short delay functions - implemented as inline functions in delay_util.h.  */
/* MSF_DELAY_xUS	- Wait x microseconds. X may be 1, 5, 10, etc.
The available values of X depend on F_CPU. See the included file for the 
actually available functions for your device (F_CU in fact) */
#include "delay_util.h"

#endif  /* MSF_SHORT_DELAY_API */



/*****************************************************
* Functions for reading and writing to SCI 
*****************************************************/
#if MSF_PRINT_API
/** @defgroup group_msf_spi MSF text functions 
* @brief MSF functions for transfering data over SPI.
* @details Implemented in msf_print.c.
* The Conio driver decides to which UART the output goes 
* @{ 
*/

/** @brief print text string.
 * @note Supports printing new line is "\n" is in the string.
 *   msf_init should be called before using any of the msf_print 
 *   functions. 
 **/
void msf_print(const char* str); 

/** @brief print single character
 * @param c char to print
*/    
void msf_print_char(char c);	// print single character

/** @brief Print simple integer in decimal format (as with sprintf %d) 
*/
void msf_printnum(uint32_t number); 

/** @brief Print integer in hex format (as with sprintf %x) 
*/
void msf_printhex(uint32_t number);  

/* More flexible functions */

/** @brief print string with one formatted 16-bit number. 
* @param str the string to print
* @param format format string which define how the number should be formated.
* this is the same as for printf function. For example, use %%d for integer.
* @param data 16-bit number to print (formatted according to format) 
* @note The number converted to string must not exceed 8 characters!
*  For example "1234" is OK; "123456789" is NOT OK.
*/
void msf_printf16(const char* str, const char* format, uint16_t data);

/** @brief print string with one formatted 32-bit number. 
* @param str the string to print
* @param format format string which define how the number should be formated.
* this is the same as for printf function. For example, use %%ld for integer.
* @param data 32-bit number to print (formatted according to format) 
* @note The number converted to string must not exceed 11 characters!
*/
void msf_printf32(const char* str, const char* format, uint32_t data);

/** @brief print string with one real number (float) 
* @param str the string to print
* @param format format string which define how the number should be formated.
* this is the same as for printf function. 
* @param data the real number to print (formatted according to format) 
* @note The number converted to string must not exceed 11 characters!
**/
void msf_printf_real(const char* str, const char* format, double data);

/* Reading characters */

/** @brief Read character from serial interface. 
 * @return 0 if no char is available */
char msf_read_char(void); 

/** @brief Check if character was received through serial line 
 * @return true if there is some character available.
 */
bool msf_char_available(void);

/** @}*/
#endif /* MSF_PRINT_API */

/*****************************************************
* Functions for reading analog values 
*****************************************************/
#if MSF_SPI_API
/** @defgroup group_msf_analog MSF analog I/O functions 
* @details MSF functions for reading analog values 
* @{ 
**/
    /** @brief Read analog value from given analog pin
    * @param apin analog pin
     * @details The analog pins are defined in msf_<device>.h. In future this definition
     * may allow mapping the pin to ADC module number but currently we support only
     * one ADC module for analog inputs.
     * @note Not all analog input pins are available in every configuration of the ADC.
     * The function will use the ADC driver Control function to switch between ADC "a" and "b" channels
     * to get the reading from the requested pin. This will slow down the reading a little if
     * you use pins which require this switching (SExa or SExb only). All SEx pins (without a or b at the end)
     * do not require any switching. 
     * See the Analog_pin_t enum in msf_<device>.h for the analog pin definitions   
     */
	uint8_t msf_spi_transfer(uint8_t data);	
	uint8_t msf_spi_init(SPI_baudrate_type SPIbaud, uint32_t control);
/** @}*/
#endif /* MSF_SPI_API */

/*****************************************************
* Functions for reading analog values 
*****************************************************/
#if MSF_ANALOG_API
/** @defgroup group_msf_analog MSF analog I/O functions 
* @details MSF functions for reading analog values 
* @{ 
**/
    /** @brief Read analog value from given analog pin
    * @param apin analog pin
     * @details The analog pins are defined in msf_<device>.h. In future this definition
     * may allow mapping the pin to ADC module number but currently we support only
     * one ADC module for analog inputs.
     * @note Not all analog input pins are available in every configuration of the ADC.
     * The function will use the ADC driver Control function to switch between ADC "a" and "b" channels
     * to get the reading from the requested pin. This will slow down the reading a little if
     * you use pins which require this switching (SExa or SExb only). All SEx pins (without a or b at the end)
     * do not require any switching. 
     * See the Analog_pin_t enum in msf_<device>.h for the analog pin definitions   
     */
	uint16_t msf_analog_read(Analog_pin_t apin);
/** @}*/
#endif /* MSF_ANALOG_API */



/*****************************************************
* Functions for software timer
* NOTE: not implemented for Kinetis yet!
*****************************************************/
#if MSF_SWTIMER_API
/** @defgroup group_msf_swtimer MSF software timer  
* @details MSF software timer functions
* @{
*/
/* Software timer support 
 Implemented in sw_timer.c */
/** User-defined function which is called when timer fires.
The prototype is: void some_func(void);   
*/
typedef void (*MSF_TIMER_FUNC)(void);

/** @brief Software timer data */
typedef struct msf_sw_timer 
{
   uint16_t         period;    /**< time in ms for the timer to fire */
   uint16_t         cnt;        /**< helper variable */
   MSF_TIMER_FUNC   handler;    /**< user routine to call when timer fires */
   struct msf_sw_timer* next; /**< pointer to next element in list */ 
} MSF_TIMER;

uint8_t     msf_timer_set(MSF_TIMER* tdata, uint16_t period, MSF_TIMER_FUNC callback);
void        msf_timer_kill(MSF_TIMER* tdata);
void        msf_timer_stop(MSF_TIMER* tdata);
uint8_t     msf_timer_restart(MSF_TIMER* tdata);

/** @}*/
#endif    /* MSF_SWTIMER_API */

/* Power states used for drivers */
typedef enum 
{
    msf_power_off,  /**< power off */
    msf_power_low,  /**< minimal power, wake-up with external event */
    msf_power_full, /**< power on; normal operation */
    
} MSF_power_state;


#ifdef __cplusplus
}
#endif
/* ----------- end of file -------------- */
#endif /* MSF_MSF_H */
