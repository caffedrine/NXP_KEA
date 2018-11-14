/****************************************************************************
 * @file     msf_spi.c
 * @brief    SPI transfer - MSF global functions 
 * @version  1
 * @date     14. April 2015
 *
 * @note Prototypes of these functions are in msf.h
 *      Uses msf ADC driver. The driver is initialized in msf_init() if
 *      proper option is specified in msf_config.h  
 *
 ******************************************************************************/

#include "msf_config.h"                    

/* Include hardware definitions */
#include "coredef.h"
              
#include "msf.h"
#include "drv_spi.h"  /* SPI driver */

#if MSF_DRIVER_SPI0
/* Define which driver (instance of the ADC) is used by us */
#define	 MSF_SPI_DRIVER	Driver_SPI0

 uint8_t msf_spi_init(SPI_baudrate_type SPIbaud, uint32_t control)
 {
   MSF_SPI_DRIVER.Initialize(SPIbaud, 0);
   MSF_SPI_DRIVER.Control(control, MSF_SPI_DATA_MODE0 | MSF_SPI_STATUS_POLLED_MODE);
   return 0;
 }
 
 uint8_t msf_spi_transfer(uint8_t data)
 {
    return MSF_SPI_DRIVER.Transfer(data);
		    		
 }
#endif /*MSF_DRIVER_SPI0*/ 


