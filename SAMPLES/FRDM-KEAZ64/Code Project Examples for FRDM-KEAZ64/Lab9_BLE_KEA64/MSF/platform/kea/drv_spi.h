/****************************************************************************
 * @file     drv_spi.h
 * @brief    CMSIS-like SPI driver for kinetis
 * @version  1
 * @date     13. April 2015
 *
 *
 ******************************************************************************/
 
 /** @defgroup group_spi SPI device driver 
 * @{ 
 * @brief CMSIS-like SPI driver for Kinetis  
 * @details    This is simplified imitation of SPI driver as defined by ARM CMSIS. 
 *  Official implementation for Kinetis does not exist.   
 *  
 *  
 * <b>Driver objects available in your program</b>
 *  - Driver_SPI0
 *  - Driver_SPI1   
 * 
 */
 
#ifndef MSF_DRIVER_SPI_H
#define MSF_DRIVER_SPI_H

/* Make sure the definitions of baudrate constants etc. are available */
#ifndef MSF_MSF_H
	#error Plese include msf.h before including this file. 
#endif


#ifdef __cplusplus
extern "C" {
#endif

/** Version of this drivers API */
#define     MSF_SPI_VERSION    (1)

/** Pointer to call back function for reporting events from the driver to
 * client application. Set in Initialize function */
typedef void (*MSF_SPI_Event_t) (uint32_t event, uint32_t arg);   


/* The priority of the SPI interrupts; lower number means higher priority.
 * For KL25Z valid value is 0 thru 3; 3 is lowest */
#define		MSF_SPI0_INT_PRIORITY	(2)	/* Priority for UART0 */
#define		MSF_SPI1_INT_PRIORITY	(2) /* Priority for UART1 and UART2 */


/* Flags (operations and parameters) for the Control function */
/* To allow changing only desired values in call to Control(), most flags use
 * at least 2 bits (i.e. possible values 0,1,2 meaning do not set, set to 0, set to 1). 
 * Positions and meaning of the bit-fields:
 * 
 Bit(s)  Meaning
 0		Set baudrate; (0) = not set; (1) = set, arg = baudrate (one of the UART_speed_t values only!)	
 1:2	Set polled or interrupt mode; 0 = not set; 1 = polled; 2 = interrupt
  		Note: If you plan using interrupt mode the callback (MSF_UART_Event_t) function must be provided in call
  		to Initialize. This function will be called when character is received, etc.
 3		Abort current send in interrupt mode
 4 		Abort current receive in interrupt mode
 5:6	Set parity; (0) = no change; (1) = none; (2) = even; (3) = odd
 7		Reserved
 8:9	Set 1 or 2 stop bits; (0) = no change; (1) = 1 stop bit; (2) = 2 stop bits;
 10:12  Number of data bits; (0) = no change; (1) = 8 bits; (2) = 9 bits
*/

/* Defines for these positions*/
#define		MSF_SPI_BAUD_Pos		(0)
#define		MSF_SPI_BAUD_Mask		(0x01)
#define		MSF_SPI_INTMODE_Pos	    (1)
#define		MSF_SPI_INTMODE_Mask	(0x06)
#define		MSF_SPI_DATAMODE_Pos	(3)
#define		MSF_SPI_DATAMODE_Mask	(0x38)

#define		MSF_SPI_DATAORDER_Pos	(6)
#define		MSF_SPI_DATAORDER_Mask	(0xC0)


/* Definitions of the flags for the Control function */
/** @defgroup group_uart_control_flags Flags for the UART Control function 
 @{*/
#define     MSF_SPI_BAUD_SET    	(1UL << MSF_SPI_BAUD_Pos)  /**< set the baudrate; arg = baudrate (one of the SPI_speed_t values only!) */
#define     MSF_SPI_POLLED_MODE 	(1UL << MSF_SPI_INTMODE_Pos)  /**< wait for each char to be sent/received in busy loop */
#define     MSF_SPI_INT_MODE      	(2UL << MSF_SPI_INTMODE_Pos)   /**< use interrupts */
#define     MSF_SPI_DATA_MODE0      (1UL << MSF_SPI_DATAMODE_Pos)  /**< spi mode 0 (default) */
#define     MSF_SPI_DATA_MODE1      (2UL << MSF_SPI_DATAMODE_Pos)  /**< spi mode 1 */
#define     MSF_SPI_DATA_MODE2      (3UL << MSF_SPI_DATAMODE_Pos)  /**< spi mode 2 */
#define     MSF_SPI_DATA_MODE3      (4UL << MSF_SPI_DATAMODE_Pos)  /**< spi mode 3 */
#define     MSF_SPI_ORDER_MSB       (1UL << MSF_SPI_DATAORDER_Pos)  /**< MSB FIRST (default) */
#define     MSF_SPI_ORDER_LSB       (2UL << MSF_SPI_DATAORDER_Pos)  /**< LSB FIRST */

/**@}*/


/** SPI events (masks) 
 * The driver will generate these events in interrupt mode.
 * The user defined function MSF_SPI_Event_t will get the mask in event parameter */
/* Events like in CMSIS driver specification */
#define		MSF_SPI_EVENT_TRANSFER_COMPLETE		(1UL << 0)	/**< Send completed; however USART may still transmit data. */


/* MSF unused version of events */
//#define		MSF_UART_EVENT_TX_EMPTY		(1UL<<0)	/**< Tx buffer is empty; arg not used; transmitter may still transmit data! */
//#define		MSF_UART_EVENT_TX_COMPLETE	(1UL<<1)	/**< Tx transfer complete; line is idle, safe to turn off transmitter */
//#define		MSF_UART_EVENT_RX_FULL		(1UL<<2)	/**< Rx buffer is full; arg = the data received */

/** SPI driver status flags (configuration), stored in SPI_INFO for each instance */
#define		MSF_SPI_STATUS_INT_MODE		    MSF_SPI_INT_MODE		/**< interrupt mode (not polled) */
#define		MSF_SPI_STATUS_POLLED_MODE		MSF_SPI_POLLED_MODE	    /**<  polled mode */

#define		MSF_SPI_STATUS_TRANSNOW			(1UL<<16)			/**< now transmitting */
//#define		MSF_UART_STATUS_RXNOW			(1UL<<17)			/**< now receiving */

/** SPI_speed_t 
 @brief The data type for baud rate for SPI. 
*/
typedef SPI_baudrate_type  SPI_speed_t;

/**
\brief Access structure of the SPI Driver.
*/
typedef struct _MSF_DRIVER_SPI {  
  uint32_t      (*Initialize)   (SPI_speed_t speed, MSF_SPI_Event_t cb_event);  
  uint32_t      (*Uninitialize) (void);                       
  
  //uint32_t      (*SPISetClockDivider) (SPI_clockdiv_t divider);
  
  uint32_t      (*Control)      (uint32_t control, uint32_t arg);
  uint8_t       (*Transfer)     (const uint8_t data);
  //uint32_t      (*Receive)      (void* data, uint32_t cnt);
  //uint32_t      (*GetRxCount)   (void);
  //uint32_t      (*GetTxCount)   (void);
  //uint32_t      (*DataAvailable)    (void);            
  
} const MSF_DRIVER_SPI;

/* Declare the instance(s) of the SPI driver(s). 
 * This is placed here rather than in uart_kea.h because then
 * the client program would need to include the (possible) MCU-specific
 * file such us uart_kea.h, which is not good for generic drivers such as Conio. */

#if (MSF_DRIVER_SPI0)
	extern MSF_DRIVER_SPI Driver_SPI0;
#endif
#if (MSF_DRIVER_SPI1)
	extern MSF_DRIVER_SPI Driver_SPI1;
#endif

	
#ifdef __cplusplus
}
#endif
/** @}*/

/* ----------- end of file -------------- */
#endif /* MSF_DRIVER_UART_H */
