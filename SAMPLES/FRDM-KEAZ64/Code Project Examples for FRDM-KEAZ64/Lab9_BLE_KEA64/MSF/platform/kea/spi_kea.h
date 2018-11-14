/****************************************************************************
 * @file     spi_kea.h
 * @brief    CMSIS-like SPI driver for kinetis
 * @version  1
 * @date     13. April 2015
 *
 * @note     This is simplified imitation od CMSIS driver which does not 
 *          exist for Kinetis(?). Should be used as a standard for MSF drivers.  
 *
 *
 ******************************************************************************/

#ifndef MSF_SPI_KEA_H_
#define MSF_SPI_KEA_H_

/* Make sure the definitions of baudrate constants etc. are available */
#ifndef MSF_MSF_H
	#error Plese include msf.h before including this file. 
#endif

/* Common definitions for this platform and this driver class */
#include "drv_spi.h"

#ifdef __cplusplus
extern "C" {
#endif


/** UART Run-time information*/
typedef struct _SPI_INFO {
  MSF_SPI_Event_t cb_event;          // Event Callback
  uint32_t  status;             // Status flags 
  void*		tbuff;   			// buffer for current Transmition operation
  void*		rbuff;				// buffer for data received (size/cnt match with trans)
  uint32_t  trans_total;		// total number of bytes to transmit
  uint32_t  trans_cnt;  		// number of bytes already transmitted
 
} SPI_INFO;

/** SPI pin info 
 */
typedef struct _SPI_PIN {
	MCU_pin_t	pin_code;	/* the pin-code defined in msf_<device>.h*/
	uint8_t		alt_num; /* the number of alt function for the given pin, for example, 2 for ALT2, etc. */
} SPI_PIN;

/** SPI information about the pins used by one instance of SPI driver
 * This is the Tx and Rx pin */
typedef struct _SPI_PINS {
	SPI_PIN misopin;
	SPI_PIN mosipin;
	SPI_PIN sckpin;
} SPI_PINS;


/** The data for one instance of the driver - the "resource" */

typedef struct {
        SPI_Type    *reg;  		// SPI peripheral register interface, CMSIS
        SPI_PINS    *pins; 		// SPI I/O pins
        SPI_INFO    *info;   	// Run-Time information
} const SPI_RESOURCES;

/** Obtain the NVIC interrupt number for given instance of the SPI driver.
 *
 */
#define	MSF_SPI_GETNVIC_IRQn(spi_adr)	((SPI_Type*)spi_adr == SPI0) ? SPI0_IRQn : \
										((SPI_Type*)spi_adr == SPI1) ? SPI1_IRQn : -1

#ifdef __cplusplus
}
#endif
/** @}*/

/* ----------- end of file -------------- */
#endif /* MSF_SPI_KEA_H */
