/****************************************************************************
 * @file     drv_uart.h
 * @brief    CMSIS-like UART driver for kinetis
 * @version  1
 * @date     20. May 2014
 *
 *
 ******************************************************************************/
 
 /** @defgroup group_uart UART device driver 
 * @{ 
 * @brief CMSIS-like UART driver for Kinetis  
 * @details    This is simplified imitation of UART driver as defined by ARM CMSIS. 
 *  Official implementation for Kinetis does not exist.   
 *  The driver does not implement any software buffer. It uses the Rx,Tx buffer
 *  of the MCU, if available. For KL25Z the buffer is just 1 B in size.  
 *  
 * <b>Driver objects available in your program</b>
 *  - Driver_UART0
 *  - Driver_UART1
 *  - Driver_UART2   
 * 
 */
 
#ifndef MSF_DRIVER_UART_H
#define MSF_DRIVER_UART_H

/* Make sure the definitions of baudrate constants etc. are available */
#ifndef MSF_MSF_H
	#error Plese include msf.h before including this file. 
#endif


#ifdef __cplusplus
extern "C" {
#endif

/** Version of this drivers API */
#define     MSF_UART_VERSION    (1)

/** Pointer to call back function for reporting events from the driver to
 * client application. Set in Initialize function */
typedef void (*MSF_UART_Event_t) (uint32_t event, uint32_t arg);   


/* The priority of the UART interrupts; lower number means higher priority.
 * For KL25Z valid value is 0 thru 3; 3 is lowest */
#define		MSF_UART0_INT_PRIORITY	(2)	/* Priority for UART0 */
#define		MSF_UART12_INT_PRIORITY	(2) /* Priority for UART1 and UART2 */


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
#define		MSF_UART_BAUD_Pos		(0)
#define		MSF_UART_BAUD_Mask		(0x01)
#define		MSF_UART_INTMODE_Pos	(1)
#define		MSF_UART_INTMODE_Mask	(0x06)
#define		MSF_UART_ABORTTX_Pos	(3)
#define		MSF_UART_ABORTTX_Mask	(0x08)
#define		MSF_UART_ABORTRX_Pos	(4)
#define		MSF_UART_ABORTRX_Mask	(0x10)
#define		MSF_UART_PARITY_Pos		(5)
#define		MSF_UART_PARITY_Mask	(0x60)
#define		MSF_UART_STOPBIT_Pos	(8)
#define		MSF_UART_STOPBIT_Mask	(0x100)
#define		MSF_UART_DATA_BITS_Pos	(10)
#define		MSF_UART_DATA_BITS_Mask	(0x1C00)


/* Definitions of the flags for the Control function */
/** @defgroup group_uart_control_flags Flags for the UART Control function 
 @{*/
#define     MSF_UART_BAUD_SET    	(1UL << MSF_UART_BAUD_Pos)  /**< set the baudrate; arg = baudrate (one of the UART_speed_t values only!) */
#define     MSF_UART_POLLED_MODE 	(1UL << MSF_UART_INTMODE_Pos)  /**< wait for each char to be sent/received in busy loop */
#define     MSF_UART_INT_MODE      	(2UL << MSF_UART_INTMODE_Pos)  /**< use interrupts */
#define     MSF_UART_ABORTTX     	(1UL << MSF_UART_ABORTTX_Pos)  /**< abort current transmit */
#define     MSF_UART_ABORTRX     	(1UL << MSF_UART_ABORTRX_Pos)  /**< abort current receive */
#define 	MSF_UART_PARITY_NONE    (1UL << MSF_UART_PARITY_Pos)	/**< no parity checking (default) */
#define 	MSF_UART_PARITY_EVEN    (2UL << MSF_UART_PARITY_Pos)	/**< even parity */	
#define 	MSF_UART_PARITY_ODD    	(3UL << MSF_UART_PARITY_Pos)	/**< odd parity */	
#define		MSF_UART_STOP_BITS_1	(1UL << MSF_UART_STOPBIT_Pos)	/**< 1 stop bit (default) */
#define		MSF_UART_STOP_BITS_2	(2UL << MSF_UART_STOPBIT_Pos)	/**< 2 stop bits */
#define 	MSF_UART_DATA_BITS_8    (1UL << MSF_UART_DATA_BITS_Pos)	/**< 8 data bits (default) */
#define 	MSF_UART_DATA_BITS_9    (2UL << MSF_UART_DATA_BITS_Pos) /**< 9 data bits. NOTE: not supported by the driver except for parity + 8 data bits. */
/**@}*/


/** UART events (masks) 
 * The driver will generate these events in interrupt mode.
 * The user defined function MSF_UART_Event_t will get the mask in event parameter */
/* Events like in CMSIS driver specification */
#define		MSF_UART_EVENT_SEND_COMPLETE		(1UL << 0)	/**< Send completed; however USART may still transmit data. */
#define 	MSF_UART_EVENT_RECEIVE_COMPLETE   	(1UL << 1) 	/**< Receive completed. Occurs when number of bytes given to Receive is received.  */
#define 	MSF_UART_EVENT_TRANSFER_COMPLETE	(1UL << 2)	/**< Transmitter is idle; safe to turn it off */
#define 	MSF_UART_EVENT_RX_OVERFLOW   		(1UL << 5)  /** Receive data overflow. Occurs if data are received before Receive is called */ 
/* MSF unused version of events */
//#define		MSF_UART_EVENT_TX_EMPTY		(1UL<<0)	/**< Tx buffer is empty; arg not used; transmitter may still transmit data! */
//#define		MSF_UART_EVENT_TX_COMPLETE	(1UL<<1)	/**< Tx transfer complete; line is idle, safe to turn off transmitter */
//#define		MSF_UART_EVENT_RX_FULL		(1UL<<2)	/**< Rx buffer is full; arg = the data received */

/** UART driver status flags (configuration), stored in UART_INFO for each instance */
#define		MSF_UART_STATUS_INT_MODE		MSF_UART_INT_MODE		/**< interrupt mode (not polled) */
#define		MSF_UART_STATUS_POLLED_MODE		MSF_UART_POLLED_MODE	/**< interrupt mode (not polled) */
#define		MSF_UART_STATUS_TXNOW			(1UL<<16)			/**< now transmitting */
#define		MSF_UART_STATUS_RXNOW			(1UL<<17)			/**< now receiving */

/** UART_speed_t 
 @brief The data type for baud rate for UART (SCI). 
*/
typedef UART_baudrate_type  UART_speed_t;


/**
\brief Access structure of the UART Driver.
*/
typedef struct _MSF_DRIVER_USART {  
  uint32_t      (*Initialize)   (UART_speed_t baudrate, MSF_UART_Event_t cb_event);  
  uint32_t      (*Uninitialize) (void);                       
  uint32_t      (*PowerControl) (MSF_power_state state); 
  uint32_t      (*Control)      (uint32_t control, uint32_t arg);
  uint32_t      (*Send)         (const void* data, uint32_t cnt);
  uint32_t      (*Receive)      (void* data, uint32_t cnt);
  uint32_t      (*GetRxCount)   (void);
  uint32_t      (*GetTxCount)   (void);
  uint32_t      (*DataAvailable)    (void);            
  
} const MSF_DRIVER_USART;

/* Declare the instance(s) of the UART driver(s). 
 * This is placed here rather than in uart_k25.h because then
 * the client program would need to include the (possible) MCU-specific
 * file such us uart_k25.h, which is not good for generic drivers such as Conio. */
#if (MSF_DRIVER_UART0)
	extern MSF_DRIVER_USART Driver_UART0;
#endif
#if (MSF_DRIVER_UART1)
	extern MSF_DRIVER_USART Driver_UART1;
#endif
#if (MSF_DRIVER_UART2)
	extern MSF_DRIVER_USART Driver_UART2;
#endif

#ifdef __cplusplus
}
#endif
/** @}*/

/* ----------- end of file -------------- */
#endif /* MSF_DRIVER_UART_H */
