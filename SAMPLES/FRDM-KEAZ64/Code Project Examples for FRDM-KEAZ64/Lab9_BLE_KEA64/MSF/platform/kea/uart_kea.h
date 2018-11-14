/****************************************************************************
 * @file     uart_kea.h
 * @brief    CMSIS-like UART driver for kinetis
 * @version  1
 * @date     20. May 2014
 *
 * @note     This is simplified imitation od CMSIS driver which does not 
 *          exist for Kinetis(?). Should be used as a standard for MSF drivers.  
 *
 *
 ******************************************************************************/
#ifndef MSF_UART_KEA_H
#define MSF_UART_KEA_H

/** @addtogroup group_uart  
 * @{
*/

/* Make sure the definitions of baudrate constants etc. are available */
#ifndef MSF_MSF_H
	#error Plese include msf.h before including this file. 
#endif

/* Common definitions for this platform and this driver class */
#include "drv_uart.h"


#ifdef __cplusplus
extern "C" {
#endif


/** UART Run-time information*/
typedef struct _UART_INFO {
  MSF_UART_Event_t cb_event;          // Event Callback
  uint32_t  status;              // Status flags 
  void*		txbuff;				// buffer for current Tx operation
  void*		rxbuff;				// buffer for current Tx operation
  uint32_t  tx_cnt;				// number of bytes already transmitted
  uint32_t  rx_cnt;				// number of bytes already transmitted
  uint32_t  tx_total;			// total number of bytes to receive or transmit
  uint32_t  rx_total;			// total number of bytes to receive or transmit
} UART_INFO;

/** UART pin info 
 */
typedef struct _UART_PIN {
	MCU_pin_t	pin_code;	/* the pin-code defined in msf_<device>.h*/
	uint8_t		alt_num; /* the number of alt function for the given pin, for example, 2 for ALT2, etc. */
} UART_PIN;

/** UART information about the pins used by one instance of UART driver
 * This is the Tx and Rx pin */
typedef struct _UART_PINS {
	UART_PIN rxpin;
	UART_PIN txpin;
} UART_PINS;

		
/** The data for one instance of the driver - the "resource"
*/
typedef struct {       
	    UART_Type	*reg;		// UART registers, CMSIS
        UART_PINS   *pins; 		// UART I/O pins
        UART_INFO   *info;   	// Run-Time information
} const UART_RESOURCES;

/** Obtain the NVIC interrupt number for given instance of the UART driver.
* */
#define	MSF_UART_GETNVIC_IRQn(uart_adr)	((UART_Type*)uart_adr == UART0) ? UART0_IRQn : \
										((UART_Type*)uart_adr == UART1) ? UART1_IRQn : \
										((UART_Type*)uart_adr == UART2) ? UART2_IRQn : -1

#ifdef __cplusplus
}
#endif
/** @}*/

/* ----------- end of file -------------- */
#endif /* MSF_UART_KEA_H */
