/****************************************************************************
 * @file     coniob.h
 * @brief    Console I/O driver with internal BUFFER; generic definitions for all platforms 
 * @version  1
 * @date     10. June 2014
 *
 * @note        
 *
 ******************************************************************************/
#ifndef MSF_CONIOB_H
#define MSF_CONIOB_H

/* Platform-specific UART driver */
#include "drv_uart.h"	

#ifdef __cplusplus
extern "C" {
#endif


/** @defgroup group_coniob coniob - buffered console I/O driver
 * @{
 * @brief  Console I/O high-level driver. Version with buffers for send and receive.
 * @details Provides functions for reading from/writing to a serial interface (SCI, UART).
 * After initialization, the coniob driver receives characters from
 *	serial line into internal buffer. The user can read them using coniob_getch or gets.
 *	The driver is initialized by msf_init.
 *
 */
/** Define which driver (instance of the UART) is used by us */
// R200040
#define	 CONIOB_UART_DRIVER	Driver_UART2
//#define	 CONIOB_UART_DRIVER	Driver_UART0  /* uart 2 used for freemaster !!!*/

/** Define the size of the buffer in bytes.
 * NOTE:  The size must be a power of two
*   and it needs to fit in the get/put indicies. i.e. if you use an
*   8 bit index, then the maximum supported size would be 128. (see cbuf.h) 
*  NOTE: you cannot send longer string than CONIOB_TXBUFFER_SIZE in one call to coniob_puts! 
*/
#define	 CONIOB_TXBUFFER_SIZE		(64)
#define	 CONIOB_RXBUFFER_SIZE		(64)

/** Convenience definition for Line Feed ASCII code */
#define LF 0x0A				
/** Convenience definition for Carriage Return ASCII code */		 
#define CR 0x0D		


/**
 * @brief Initialize the console I/O driver
 * @param[in] baudrate the communication speed; one of
 * the enum values defined in msf_mkl25z.h!
 * @return none
 *
 */
void coniob_init(UART_speed_t baudrate);  


/**
 * @brief Send one character to SCI. If the char is "\n", send CR + LF
 * @param[in] c char to send
 */
void coniob_putch(char c);


/**
 * @brief Send null-terminated string to SCI.
 * @param[in] str pointer to string to send
 * @note If the string contains "\n", CR + LF are sent.
 */
void coniob_puts(const char* str);     

/**
 * @brief Read one character from SCI.
 * @return the character read or 0 if there is no character available.
 *
 */
char coniob_getch(void);               

/**
 * @brief Read string from console (from buffer). Does not block the caller.
 * @param str [out] buffer provided by the user to receive the string
 * @param max_chars [in] maximum character (not including terminating 0) to
 *  receive.
 * @param terminator [in] character which means the end of the string. Can be 0 if not needed.
 * @return number of characters actually written to the string str; not including
 * the ending 0.
 * @note The possible results are:
 * 1) string with max_chars valid characters .
 * 2) string up to terminator character
 * 3) string with all the chars from buffer (which may be empty string if buffer is empty)
 *
 * The resulting string is null-terminated (valid C-language string) in all cases.
 * The terminator character in not included in the resulting string.
 */
uint32_t coniob_gets(char* str, uint32_t max_chars, char terminator);		 

/**
 * @brief Check if data are available for reading
 * @return 0 if there are no data (call to getch would block the caller;
 *  or the number of characters available in input buffer.
*/
uint32_t coniob_kbhit(void);

/**
 * @brief Get the char from input buffer without removing it from the buffer.
 * @return 0 if no char is available */
char coniob_peek(void);

/* empty the receive buffer */
/*void coniob_flush(void);*/


/** @} */
#ifdef __cplusplus
}
#endif
/* ----------- end of file -------------- */
#endif /* MSF_CONIOB_H */
