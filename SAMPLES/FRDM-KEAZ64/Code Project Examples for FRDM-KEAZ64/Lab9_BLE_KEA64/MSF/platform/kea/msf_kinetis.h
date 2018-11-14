/****************************************************************************
 * @file     kinetis.h
 * @brief    Base header file for Kinetis MCUs 
 * @version  1
 * @date     29. Apr 2014
 *
 * @note    Defines the MSF data types etc. for Kinetis microcontrollers.
 *
 ******************************************************************************/
#ifndef MSF_KINETIS_H
#define MSF_KINETIS_H


#ifdef __cplusplus
extern "C" {
#endif


/* Convenience port letter-to-number defines */
#define PORT_FLAG  (0x80)   // PORT flag is andded with specific port  to allow A0 <> 0

#define	PORT_A		(0)
#define	PORT_B		(1)
#define	PORT_C		(2)
#define	PORT_D		(3)
#define	PORT_E		(4)
#define	PORT_F		(5)
#define	PORT_G		(6)
#define	PORT_H		(7)
#define	PORT_I		(8)


/* GPIO_MAKE_PINCODE
 @brief Helper macro to create pin code from port number and pin number
 @note Note that this is not used in runtime; just in compile time to create the
 enum with pin codes.
 Pin code is: 8-bit pin number (normally 0 - 31) in lower byte 1 and 8-bit port number in byte 0 
 It cannot be defined in gpio.h as this file is included only at the end of
 msf_<device>.h but needed at the beginning. And gpio.h cannot be at the beginning 
 of the msf_<device>.h because it uses macros defined in the msf_<device>.h.
*/
#define	GPIO_MAKE_PINCODE(port, pin)  ((((uint16_t)port & 0x00FF) | PORT_FLAG) | ((uint16_t)(pin) << 8))


//#define enable_irq NVIC_EnableIRQ
//#define disable_irq NVIC_DisableIRQ
#define EnableInterrupts __enable_irq()
#define DisableInterrupts __disable_irq()

/** Helper macros to create atomic block of code 
 TODO: restore status register?    (as in AVR version)
 */
#define	MSF_ATOMIC_BEGIN()		DisableInterrupts
#define MSF_ATOMIC_END()		EnableInterrupts


/** Command for resetting the watchdog.
 * TODO: Not implemented yet! 
 * Note that resetting watchdog when it is not enabled will cause a reset! */
#define     MSF_RESET_WATCHDOG()    


/* Include device header  - the device constant is defined by the user
 in msf_config.h 
*/
#if   MSF_KINETIS_SKEA
    #include "msf_skea.h"
#else
    #error Please select the device in msf_config.h 
#endif
 

#ifdef __cplusplus
}
#endif

#endif  /* MSF_KINETIS_H */
