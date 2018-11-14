/****************************************************************************
 * @file     coniob.c
 * @brief    Console Input/output functions; with buffer
 * @version  1
 * @date     10 June 2014
 *
 * @note	After initialisation, the coniob driver receives characters from
 * 			serial line into internal buffer. The user can read them using coniob_getch or gets.
 * 			This is very simple version which handles each received/sent char in UART driver event
 * 			(not efficient :( ), but sending more data at once proved to be very complicated.
 * 			For F_CPU = 48 MHz:
 * 			9600 BD:  1 byte is sent in about 5000 CPU clock cycles (instructions)
 * 			115200 BD: 1 byte is sent in about 420 CPU clock cycles   
 * 			Even with high overhead of this version it will save lot of CPU time which would
 * 			be vasted in waiting for Tx/Rx complete in polled mode. 
 * 			
 * These functions rely on UART driver.
 * 
 *      
 *
 ******************************************************************************/


/** @addtogroup group_coniob
 * @{
*/
/* Include user configuration */
#include "msf_config.h"

/* Include hardware definitions */
#include "coredef.h"
       
#include <stdio.h>   /* for sprintf */
#include <string.h>	/* for strlen */

#include "cbuf.h"	/* circular buffer */

           
#include "msf.h"

#include "coniob.h"


/*---------- Internal variables ---------------- */
/* The name of the size for the buffer must match this patterns */
/* The circular buffer library requires that the circular buffer size be a power of two, and the
*   size of the buffer needs to smaller than the index. So an 8 bit index
*   supports a circular buffer upto ( 1 << 7 ) = 128 entries, and a 16 bit index
*   supports a circular buffer upto ( 1 << 15 ) = 32768 entries.
*   */
#define coniob_txQ_SIZE    CONIOB_TXBUFFER_SIZE
#define coniob_rxQ_SIZE    CONIOB_RXBUFFER_SIZE
   
volatile struct
{
	uint8_t     m_getIdx;
    uint8_t     m_putIdx;
    uint8_t     m_entry[ coniob_txQ_SIZE ];
} coniob_txQ;

volatile struct
{
	uint8_t     m_getIdx;
    uint8_t     m_putIdx;
    uint8_t     m_entry[ coniob_rxQ_SIZE ];
} coniob_rxQ;


volatile uint32_t coniob_nowSending;


/* -------- Prototypes of internal functions   -------- */
static void coniob_UART_SignalEvent(uint32_t event, uint32_t arg);
//void wconiob_update_rxfifo(void);
//void wconiob_update_txfifo(void);

/* -------- Implementation of public functions   -------- */

/* initialize console I/O */
void coniob_init(UART_speed_t baudrate)               
{
    CONIOB_UART_DRIVER.Initialize(baudrate, coniob_UART_SignalEvent);
	/* go to interrupt driven mode */
	CONIOB_UART_DRIVER.Control(MSF_UART_INT_MODE, 0);
    /* Note: if you get compiler error that the speed constant is not defined, check if
    for given F_CPU this speed is available; in msf_<device>.h file included into <platform>.h,
    e.g. s08.h */ 
		
	/* Init the FIFOs */
	CBUF_Init(coniob_txQ);
	CBUF_Init(coniob_rxQ);	
	coniob_nowSending = 0;
	
	/* We automatically start to receive data from serial line */
	CONIOB_UART_DRIVER.Receive((void*)CBUF_GetPushEntryPtr(coniob_rxQ), 1);
}

/* read one character. return 0 if no character is available */
char coniob_getch(void)               
{	
	if ( CBUF_IsEmpty( coniob_rxQ ) )
		return 0;
	
	return CBUF_Pop( coniob_rxQ );
	
}

/* Return number of characters available in input buffer */
uint32_t coniob_kbhit(void)
{
	return CBUF_Len(coniob_rxQ);
}

/* send one character to console */
void coniob_putch(char c)        
{		
	//uint8_t* pStart = CBUF_GetLastEntryPtr(coniob_txQ);
	volatile uint8_t* pData;
	
	/* push to FIFO; if full, overwrites the oldest char 
	 * TODO: would it be better to block the caller and wait for ISR to transmit some bytes? */
    if(c == '\n')
    {	 	
    	CBUF_Push( coniob_txQ, CR );
    	CBUF_Push( coniob_txQ, LF ); 
	} 
    else 
    {
    	CBUF_Push( coniob_txQ, c ); 
	}
        
    if ( !coniob_nowSending )
    {    	
    	coniob_nowSending = 1;    	
    	/* Note: we always send only 1 char and leave it to the callback to send the rest of the FIFO;
    	 * otherwise we would have to handle the situation when the 1st char is at the end of the 
    	 * FIFO buffer and the next char is at the beginning, which the UART driver does not handle-
    	 * - it needs simple flat buffer.  */
    	//CONIOB_UART_DRIVER.Send(pStart, 1);
    	pData = &CBUF_Pop(coniob_txQ);
    	CONIOB_UART_DRIVER.Send( (const void*)pData, 1);
    }
}

/* send null-terminated string */
void coniob_puts(const char* str)     
{		
	//uint8_t* pStart = CBUF_GetLastEntryPtr(coniob_txQ);
	volatile uint8_t* pData;
	
	while(*str) 
    {	    	
    	if( *str == '\n')
    	{	 	
    	 	CBUF_Push( coniob_txQ, CR );
    	  	CBUF_Push( coniob_txQ, LF );    
    	} 
    	else 
    	{
    	  	CBUF_Push( coniob_txQ, *str );    	  	
    	}	  
    	
    	str++;
    }
        
    if ( !coniob_nowSending )
    {
      	/* Only if we are not sending, start sending */
    	coniob_nowSending = 1;
    	/* Note: we always send only 1 char and leave it to the callback to send the rest of the FIFO;
    	* otherwise we would have to handle the situation when the 1st char is at the end of the 
    	* FIFO buffer and the next char is at the beginning, which the UART driver does not handle-
    	* - it needs simple flat buffer.  */
    	//CONIOB_UART_DRIVER.Send(pStart, 1);
    	pData = &CBUF_Pop(coniob_txQ);
    	CONIOB_UART_DRIVER.Send( (const void*)pData, 1);
    }
}



/* read string from console. */
uint32_t coniob_gets(char* str, uint32_t max_chars, char terminator)
{
    char c;
    uint8_t i;
    
    for ( i = 0; i < max_chars && !CBUF_IsEmpty(coniob_rxQ); i++ )
    { 
    	c = CBUF_Pop( coniob_rxQ );
    	if ( c != terminator )
            str[i] = c;        
    }
    str[i] = 0; 
    return i;
}

char coniob_peek(void)
{
	if ( CBUF_IsEmpty(coniob_rxQ) )
		return 0;
	return CBUF_Get(coniob_rxQ, 0);
}

#if 0	/* not working properly */
/** Empty the receive buffer.
 *  Note that if data keep on coming from the serial line, some data may appear in the
 *  buffer even before this function returns. 
 *  It is useful in processing commands in 
 *  command-line-interface if we start processing e.g. when 3 chars are received, but 
 *  the user might have typed some extra chars in the time before we process the 
 *  command and wait for another command - so there would some chars left in the
 *  Rx buffer.   
 * */
void coniob_flush(void)
{
	CBUF_Init(coniob_rxQ);	
}
#endif

/** @}*/

/* ---------------------- Internal functions ------------------------------------------- */
/* Internal use only!
 * This function will be called by the UART driver in interrupt mode to report events,
 * such as sending completed, etc.
 * NOTE: this is called from ISR, so 
 * - do not spend much time here! 
 * - preferably do not send to UART from here!
 * - beware of loops, e.g. by sending text to UART in response to send complete event!
 */
static void coniob_UART_SignalEvent(uint32_t event, uint32_t arg)
{
	volatile uint8_t* pData;
	
	switch( event) 
	{
	case MSF_UART_EVENT_SEND_COMPLETE:
		
		/* sending just completed; if there is something more to send, start sending again... */		
		if ( !CBUF_IsEmpty(coniob_txQ))
		{
			coniob_nowSending = 1;
			pData = &CBUF_Pop(coniob_txQ);
			CONIOB_UART_DRIVER.Send( (const void*)pData, 1);
		}
		else
			coniob_nowSending = 0;
		break;
		
	case MSF_UART_EVENT_RECEIVE_COMPLETE:
		/* just put the new char to FIFO and start new receive */
		CBUF_AdvancePushIdx(coniob_rxQ);	/* the new char is now available in FIFO (UART driver 
			copied it there already, we just now update the FIFO index) */
		/* Get next char */
		CONIOB_UART_DRIVER.Receive((void*)CBUF_GetPushEntryPtr(coniob_rxQ), 1);
		break;
		
		
		
	/*case MSF_UART_EVENT_TRANSFER_COMPLETE:
		break;
		
	case MSF_UART_EVENT_RX_OVERFLOW:
		break;*/
	}

}

