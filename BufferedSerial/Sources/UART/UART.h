/*! @file HalUart.h
	@brief Uart Driver Interfaces
 */

#ifndef _HALUART_H
#define _HALUART_H

/***************************************************************************//**
 * @addtogroup Peripherals
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Uart
 * @brief Uart Driver Module
 *
 * @{
 */

#include "stdio.h"

#include "derivative.h"

/***
 * necessary for using within a C++ module
 */
#ifdef __cplusplus
extern "C" {
#endif

/* _ */
/*! \define Channels
 * 	\brief Defines the values of every UART channel
 */
#define Un0		(0u)
#define Un1		(1u)
#define Un2		(2u)
/*! \def NULL_PTR
 *	\brief The null pointer.
 */
#define NULL_PTR		((void*)0)
/*! \typedef BOOL
	\brief The BOOL data type definition.
*/
typedef enum {
	FALSE = 0,	/**< Defines the false code. */
	TRUE		/**< Defines the true code. */
} BOOL;
/*! \enum PINSEL
 * 	\brief Enumerates the specific pins for every channel's Tx and Rx
 */
enum UART_pinsel
{
	PTB0_PTB1 = 0,
	PTA2_PTA3 = 1,
	PTC6_PTC7 = 2,
	PTF2_PTF3 = 3,
	PTD6_PTD7 = 4,
	PTI0_PTI1 = 5
};
/* _ */
/*! \define Buffers size\
 *  \brief 	Defines the Rx and Tx  buffer size
 */
#define UART_RX_BUF_SIZE		(128u)
#define UART_TX_BUF_SIZE		(128u)
void Uart_Init(uint8_t Channel, uint32_t PbClk, uint32_t u32Baud);
void Uart_DeInit(UART_Type *pUART);
void Uart_Write(uint8_t Channel, const uint8_t* lpBuffer, uint8_t nNumberOfBytesToWrite, uint8_t* lpNumberOfBytesWritten);
void Uart_Read(uint8_t Channel, uint8_t* lpBuffer, uint8_t nNumberOfBytesToRead, uint8_t* lpNumberOfBytesRead);

uint8_t Uart_GetRxQueueLevel(uint8_t Channel);
uint8_t Uart_GetTxQueueLevel(uint8_t Channel);
void Uart_EnableWakeup(uint8_t Channel);
void Uart_DisableWakeup(uint8_t Channel);


void UART_PutChar(UART_Type *pUART, uint8_t u8Char);
uint8_t UART_GetChar(UART_Type *pUART);

void UART_pin_settings(enum UART_pinsel uart_pins);
/***
 * necessary for using within a C++ module
 */
#ifdef __cplusplus
	}
#endif

/** @} (end addtogroup Uart) */
/** @} (end addtogroup Peripherals) */
#endif	/* End include guard */


#if 0
/***********************************************************************************************
*
* @brief    UART_Init - Initalizes the Uart 1 to run at 9600 bauds assuming bus clock of 20Mhz
* @param    none
* @return   none
*
/***********************************************************************************************
*
* @brief    UART_SendChar - Send a single byte on Uart1
* @param    byte to send
* @return   none
*
************************************************************************************************/  
void UART_SendChar(uint8_t_t send);

/***********************************************************************************************
*
* @brief    UART_GetChar - Retrieve the received char from the Uart1 buffer (should be called from Interrupt)
* @param    none
* @return   received byte
*
************************************************************************************************/
uint8_t_t UART_GetChar();

/***********************************************************************************************
*
* @brief    UART_SendStr - Print the String on Terminal Window
* @param    String to be sent
* @return   none
*
************************************************************************************************/
void UART_SendStr(char data_string[]);

#endif /* UART_H_ */

