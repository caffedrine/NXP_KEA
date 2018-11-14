/****************************************************************************
 * @file     conio.c
 * @brief    Console Input/output functions; not buffered.
 * @version  1
 * @date     22. Apr 2014
 *
 * @note     These functions rely on UART hardware driver.
 * 			IMPORTANT: This driver is deprecated. Use coniob instead.
 *      
 *
 ******************************************************************************/

/** @defgroup group_conio conio - console I/O driver 
 * @{ 
 * @brief  Console I/O generic driver. Version without buffers for send and receive.
 * @details Provides functions for reading from/writing to a serial interface (SCI, UART).
 * 
 */

/* Include user configuration */
#include "msf_config.h"

/* Include hardware definitions */
#include "coredef.h"
       
#include <stdio.h>   /* for sprintf */
#include <string.h>	/* for strlen */

           
#include "msf.h"


/* Platform-specific UART driver */
#include "drv_uart.h"	

/** Define which low-level driver (instance of the UART) is used by the coniob */
#define	 CONIO_UART_DRIVER	Driver_UART2

/** Convenience definition for Line Feed ASCII code */
#define LF 0x0A				
/** Convenience definition for Carriage Return ASCII code */		 
#define CR 0x0D						

/* -------- Prototypes of internal functions   -------- */

/* -------- Implementation of public functions   -------- */

/** Initialize the console I/O driver
 **/
void conio_init(void)               
{
	CONIO_UART_DRIVER.Initialize(BD9600, null);
    /* Note: if you get compiler error that the speed constant is not defined, check if
    for given F_CPU this speed is available; in msf_<device>.h file included into <platform>.h,
    e.g. s08.h */  
}

/** Read one character from SCI.
 * @return the character read.
 * @note blocks the caller untill character is received! 
 **/
char conio_getch(void)               
{
	char buffer[1];
	CONIO_UART_DRIVER.Receive(buffer, 1);
	return buffer[0];
    //return (char)uart_read();   
}

/** Check if data are available for reading
 * @return 0 if there are no data (call to getch would block the caller;
 *  or the number of characters available in input buffer. 
 **/
uint8_t conio_kbhit(void)
{
    /* Note: for driver with internal buffer we could check out buffer;
    now we just check the hardware (1 byte) input data register */
    //return (uart_data_available() ? 1 : 0);
	return CONIO_UART_DRIVER.DataAvailable();
}

/** Send one character to SCI. If the char is '\n', send CR + LF
 * @param c char to send
 **/
void conio_putch(char c)        
{
	char buff[] = {CR, LF };
    if(c == '\n')
    {	 		    	
    	CONIO_UART_DRIVER.Send(buff, 2);
		//uart_write(CR);
		//uart_write(LF);
	} 
    else 
    {
    	CONIO_UART_DRIVER.Send(&c, 1);
		//uart_write(c);					
	}
}

/** Send null-terminated string to SCI. 
 * @param str pointer to string to send
 * @note If the string contains '\n', CR + LF are sent. 
 **/
void conio_puts(const char* str)     
{
	/* simple but does not translate "\n" to CR + LF */
	/* CONIO_UART_DRIVER.Send(str, strlen(str)); */
    while(*str) 
    {	    	
	   conio_putch(*str);
	   str++;
    }
}


/** Read string from console. 
 * @note Reading from console is terminated when given character is received.
 * This terminator character in not included in the resulting string.
 * The resulting string is null-terminated (valid C-language string) in any 
 * case.  
 * The function blocks the caller until string is received or max_chars reached.
 * @param str [out] buffer provided by the user to receive the string
 * @param max_chars [in] maximum character (not including terminating 0) to 
 *  receive.
 * @param terminator [in] character which means the end of the string. 
 * @return number of characters actually written to the string, not including 
 * the ending 0.      
 **/
uint8_t conio_gets(char* str, uint8_t max_chars, char terminator)
{
    char c;
    uint8_t i;
    
    for ( i = 0; i < max_chars; i++ )
    {
        //c = conio_getch();
    	CONIO_UART_DRIVER.Receive(&c, 1);
        if ( c != terminator )
            str[i] = c;        
    }
    str[i] = 0; 
    return i;
}		 

/** @}*/
