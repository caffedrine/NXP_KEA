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
 * @{
 */

#include "Std_Types.h"
#include "derivative.h"

/***
 * necessary for using within a C++ module
 */
#ifdef __cplusplus
extern "C" {
#endif

/* _ */
#define Un0		(0u)
#define Un1		(1u)
#define Un2		(2u)

/* _ */
#define UART_RX_BUF_SIZE		(128u)
#define UART_TX_BUF_SIZE		(128u)

/* _ */
void Uart_Init(UART_Type *pUART, UInt32 PbClk, UInt32 BaudRate);
void Uart_DeInit(UART_Type *pUART);
void Uart_Write(UInt8 Channel, const UInt8* lpBuffer, UInt8 nNumberOfBytesToWrite, UInt8* lpNumberOfBytesWritten);
void Uart_Read(UInt8 Channel, UInt8* lpBuffer, UInt8 nNumberOfBytesToRead, UInt8* lpNumberOfBytesRead);
void Uart_GetRxQueueLevel(UInt8 Channel, UInt8* lpLevel);
void Uart_GetTxQueueLevel(UInt8 Channel, UInt8* lpLevel);
void Uart_EnableWakeup(UInt8 Channel);
void Uart_DisableWakeup(UInt8 Channel);

// Debug
void UART_PutChar(UART_Type *pUART, uint8_t u8Char);
uint8_t UART_GetChar(UART_Type *pUART);

/***
 * necessary for using within a C++ module
 */
#ifdef __cplusplus
	}
#endif

/** @} (end addtogroup Uart) */
/** @} (end addtogroup Peripherals) */
#endif	/* End include guard */
