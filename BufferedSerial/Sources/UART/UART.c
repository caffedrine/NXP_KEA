/*! @file UART.c
 @brief Definition of the Uart driver module.
 */

#include "UART.h"

/***************************************************************************//**
 * @addtogroup Peripherals
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Uart
 * @brief The UART driver module
 * @{
 */

/*! \def UART_TOTAL_NUMBER_OF_CHANNELS
 \brief Defines the amount of channels the driver handles.
 */
#define UART_TOTAL_NUMBER_OF_CHANNELS           (3u)

/*! \def UART_TX_BUF_SIZE_MASK
 \brief Defines the TX data buffer mask.
 */
#define UART_TX_BUF_SIZE_MASK                   (UART_TX_BUF_SIZE - 1u)
#if (UART_TX_BUF_SIZE & UART_TX_BUF_SIZE_MASK)
#error UART_TX_BUF_SIZE is not power of 2
#endif

/*! \def UART_RX_BUF_SIZE_MASK
 \brief Defines the RX data buffer mask
 */
#define UART_RX_BUF_SIZE_MASK                   (UART_RX_BUF_SIZE - 1u)
#if (UART_RX_BUF_SIZE & UART_RX_BUF_SIZE_MASK)
#error UART_RX_BUF_SIZE is not power of 2
#endif

/* Local Prototypes */
void Un_RxISR(uint8_t Channel, uint8_t data);
void Un_TxISR(uint8_t Channel);

#if 0
void TheApp_Uart_Notification_Callback(uint8_t u8Channel, uint8_t u8Data);
#define UnRx_Notification_Calback(arg1, arg2)   TheApp_Uart_Notification_Callback( arg1, arg2 )
#endif

/*! \struct Un_DataType
 \brief The Uart data type instance definition.
 */
typedef struct
{
	uint8_t u8RxBuff[UART_RX_BUF_SIZE]; /**< The RX data buffer */
	uint8_t u8TxBuff[UART_TX_BUF_SIZE]; /**< The TX data buffer */
	uint8_t u8TxHead; /**< Holds the head within TX buffer */
	uint8_t u8TxTail; /**< Holds the tail within TX buffer */
	uint8_t u8RxHead; /**< Holds the head within RX buffer */
	uint8_t u8RxTail; /**< Holds the tail within RX buffer */
	uint8_t u8Wake; /**< Non zero if the Wakeup notification is to be sent to the EcuM */

	uint32_t u32PbClk; /**< Holds the peripheral bus clock */
	uint32_t u32BaudRate; /**< Holds the peripheral baud rate */
	BOOL bRts; /**< Holds the actual RTS status */
	void (*pfRTSCtl)(BOOL); /**< A pointer to function that control the RTS status */

	uint8_t u8BootByteLast; /**< Used for enter boot request bytes sequence identifier. Holds the latest received byte. */
	uint8_t u8BootByteCount; /**< Used for enter boot request bytes sequence identifier. Holds the number of bytes the sequence is expected. */
} Un_DataType;

/*! \var static Un_DataType Un_Inst[ UART_TOTAL_NUMBER_OF_CHANNELS ]
 \brief The Uart instances declaration.
 */

static Un_DataType Un_Inst[UART_TOTAL_NUMBER_OF_CHANNELS];

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
//extern AppBootIf_Type AppBootIf_Inst;
/** @endcond */

/***************************************************************************//**
 * \fn void UART0_IRQHandler(void)
 * \brief   The UART0 ISR.
 * \details
 * \return  This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void UART0_IRQHandler(void)
{
	UART_Type *pUART = UART0;
	uint8_t lu8_Status = pUART->S1;
	uint8_t lu8_Data = pUART->D;

	/* UART parity error interrupt occurred ------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_PF_MASK)) && (0ul != (pUART->C3 & UART_C3_PEIE_MASK)) )
	{
		;
	}
	/* UART frame error interrupt occurred -------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_FE_MASK)) && (0ul != (pUART->C3 & UART_C3_FEIE_MASK)) )
	{
		;
	}
	/* UART noise error interrupt occurred -------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_NF_MASK)) && (0ul != (pUART->C3 & UART_C3_NEIE_MASK)) )
	{
		;
	}
	/* UART Over-Run interrupt occurred ----------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_OR_MASK)) && (0ul != (pUART->C3 & UART_C3_ORIE_MASK)) )
	{
		;
	}
	/* UART RX-NotEmpty interrupt occurred ----------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_RDRF_MASK)) && (0ul != (pUART->C2 & UART_C2_RIE_MASK)) )
	{
		Un_RxISR( Un0, lu8_Data );
	}
	/* ###################################################################### */
	/* ################################### */
	//
	// Based on number of bytes in TxFifo, Un_TxISR, shall enable the TXE or TXC interrupt.
	/* UART TX-Empty interrupt occurred ----------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_TDRE_MASK)) && (0ul != (pUART->C2 & UART_C2_TIE_MASK)) )
	{
		Un_TxISR( Un0 );
	}
	/* UART TX-Complete interrupt occurred ----------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_TC_MASK)) && (0ul != (pUART->C2 & UART_C2_TCIE_MASK)) )
	{
		pUART->C2 &= ~UART_C2_TIE_MASK; /* Clear interrupt enable flag */
		if ( FALSE != Un_Inst[Un0].bRts ) /* Clear RTS Status            */
		{
			Un_Inst[Un0].bRts = FALSE;
			if ( NULL_PTR != Un_Inst[Un0].pfRTSCtl )
			{
				Un_Inst[Un0].pfRTSCtl( FALSE ); /* Release transceiver TE line */
			}
		}
	}
}

/***************************************************************************//**
 * \fn void UART1_IRQHandler(void)
 * \brief   The UART1 ISR.
 * \details
 * \return  This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void UART1_IRQHandler(void)
{
	UART_Type *pUART = UART1;
	uint8_t lu8_Status = pUART->S1;
	uint8_t lu8_Data = pUART->D;

	/* UART parity error interrupt occurred ------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_PF_MASK)) && (0ul != (pUART->C3 & UART_C3_PEIE_MASK)) )
	{
		;
	}
	/* UART frame error interrupt occurred -------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_FE_MASK)) && (0ul != (pUART->C3 & UART_C3_FEIE_MASK)) )
	{
		;
	}
	/* UART noise error interrupt occurred -------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_NF_MASK)) && (0ul != (pUART->C3 & UART_C3_NEIE_MASK)) )
	{
		;
	}
	/* UART Over-Run interrupt occurred ----------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_OR_MASK)) && (0ul != (pUART->C3 & UART_C3_ORIE_MASK)) )
	{
		;
	}
	/* UART RX-NotEmpty interrupt occurred ----------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_RDRF_MASK)) && (0ul != (pUART->C2 & UART_C2_RIE_MASK)) )
	{
		Un_RxISR( Un1, lu8_Data );
	}
	/* ###################################################################### */
	/* ################################### */
	//
	// Based on number of bytes in TxFifo, Un_TxISR, shall enable the TXE or TXC interrupt.
	/* UART TX-Empty interrupt occurred ----------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_TDRE_MASK)) && (0ul != (pUART->C2 & UART_C2_TIE_MASK)) )
	{
		Un_TxISR( Un1 );
	}
	/* UART TX-Complete interrupt occurred ----------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_TC_MASK)) && (0ul != (pUART->C2 & UART_C2_TCIE_MASK)) )
	{
		pUART->C2 &= ~UART_C2_TIE_MASK; /* Clear interrupt enable flag   */
		if ( FALSE != Un_Inst[Un1].bRts ) /* Clear RTS Status            */
		{
			Un_Inst[Un1].bRts = FALSE;
			if ( NULL_PTR != Un_Inst[Un1].pfRTSCtl )
			{
				Un_Inst[Un1].pfRTSCtl( FALSE ); /* Release transceiver TE line */
			}
		}
	}
}

/***************************************************************************//**
 * \fn void UART2_IRQHandler(void)
 * \brief   The UART2 ISR.
 * \details
 * \return  This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void UART2_IRQHandler(void)
{
	UART_Type *pUART = UART2;
	uint8_t lu8_Status = pUART->S1;
	uint8_t lu8_Data = pUART->D;

	/* UART parity error interrupt occurred ------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_PF_MASK)) && (0ul != (pUART->C3 & UART_C3_PEIE_MASK)) )
	{
		;
	}
	/* UART frame error interrupt occurred -------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_FE_MASK)) && (0ul != (pUART->C3 & UART_C3_FEIE_MASK)) )
	{
		;
	}
	/* UART noise error interrupt occurred -------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_NF_MASK)) && (0ul != (pUART->C3 & UART_C3_NEIE_MASK)) )
	{
		;
	}
	/* UART Over-Run interrupt occurred ----------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_OR_MASK)) && (0ul != (pUART->C3 & UART_C3_ORIE_MASK)) )
	{
		;
	}
	/* UART RX-NotEmpty interrupt occurred ----------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_RDRF_MASK)) && (0ul != (pUART->C2 & UART_C2_RIE_MASK)) )
	{
		Un_RxISR( Un2, lu8_Data );
	}

	/* ###################################################################### */
	/* ################################### */
	//
	// Based on number of bytes in TxFifo, Un_TxISR, shall enable the TXE or TXC interrupt.
	/* UART TX-Empty interrupt occurred ----------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_TDRE_MASK)) && (0ul != (pUART->C2 & UART_C2_TIE_MASK)) )
	{
		Un_TxISR( Un2 );
	}

	/* UART TX-Complete interrupt occurred ----------------------------------------- */
	if ( (0ul != (lu8_Status & UART_S1_TC_MASK)) && (0ul != (pUART->C2 & UART_C2_TCIE_MASK)) )
	{
		pUART->C2 &= ~UART_C2_TIE_MASK; /* Clear interrupt enable flag   */
		pUART->C2 &= ~UART_C2_TCIE_MASK; //0b10111111;

//		if ( FALSE != Un_Inst[Un2].bRts ) /* Clear RTS Status            */
//		{
//			Un_Inst[Un2].bRts = FALSE;
//			if ( NULL_PTR != Un_Inst[Un2].pfRTSCtl )
//			{
//				Un_Inst[Un2].pfRTSCtl( FALSE ); /* Release transceiver TE line */
//			}
//		}
	}
}

/***************************************************************************//**
 * \fn void Uart_Init(uint8_t Channel, uint32_t PbClk, uint32_t u32Baud)
 * \brief
 *   Initialization routine of the module.
 * \details
 *   Upon system start-up, call this function to initialize the module before
 *   any other function call.
 * \param[in] Channel - Identifies the peripheral instance.
 *
 * \param[in] PbClk - Defines the Peripheral bus clock.
 *
 * \param[in] u32Baud - Desired baud rate.
 *
 * \return
 *   This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void Uart_Init(uint8_t Channel, uint32_t PbClk, uint32_t u32Baud)
{
	UART_Type *pUART = NULL_PTR;
	uint16_t u16Sbr;
	uint8_t u8Temp;

	/* Check if given channel is correct.*/
	if ( UART_TOTAL_NUMBER_OF_CHANNELS > Channel )
	{
		Un_Inst[Channel].u32PbClk = PbClk;
		Un_Inst[Channel].u32BaudRate = u32Baud;
		Un_Inst[Channel].u8BootByteCount = 50u;
	}
	switch ( Channel )
	{
		case Un0:
			pUART = UART0;
			/* Enable the clock to the selected UART */
			SIM->SCGC |= SIM_SCGC_UART0_MASK;

			/* Make sure that the transmitter and receiver are disabled while we
			 * change settings.
			 */
			pUART->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);

			/* Configure the UART for 8-bit mode, no parity */
			pUART->C1 = 0;

			/* Calculate baud settings */
			u16Sbr = (((PbClk) >> 4) + (u32Baud >> 1)) / u32Baud;

			/* Save off the current value of the UARTx_BDH except for the SBR field */
			u8Temp = pUART->BDH & ~(UART_BDH_SBR_MASK);

			pUART->BDH = u8Temp | UART_BDH_SBR( u16Sbr >> 8 );		//0

			// How to calculate baudrate: https://community.nxp.com/thread/387950
			pUART->BDL = 128;		//(uint8_t)(u16Sbr & UART_BDL_SBR_MASK);	//128 (9600 baud)

			/* Enable receiver */
			pUART->C2 |= UART_C2_RE_MASK;

			/* Enable transmitter */
			pUART->C2 |= UART_C2_TE_MASK;

			/* Enable Receiver interrupts*/
			pUART->C2 |= UART_C2_RIE_MASK;

			/* NVIC for USART */
			NVIC_SetPriority( UART0_IRQn, 1 );
			NVIC_EnableIRQ( UART0_IRQn );
			NVIC_ClearPendingIRQ( UART0_IRQn );
			break;

		case Un1:
			pUART = UART1;

			/* Enable the clock to the selected UART */
			SIM->SCGC |= SIM_SCGC_UART1_MASK;

			/* Make sure that the transmitter and receiver are disabled while we
			 * change settings.
			 */
			pUART->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);

			/* Configure the UART for 8-bit mode, no parity */
			pUART->C1 = 0;

			/* Calculate baud settings */
			u16Sbr = (((PbClk) >> 4) + (u32Baud >> 1)) / u32Baud;

			/* Save off the current value of the UARTx_BDH except for the SBR field */
			u8Temp = pUART->BDH & ~(UART_BDH_SBR_MASK);

			pUART->BDH = u8Temp | UART_BDH_SBR( u16Sbr >> 8 );		//0

			// How to calculate baudrate: https://community.nxp.com/thread/387950
			pUART->BDL = 128;		//(uint8_t)(u16Sbr & UART_BDL_SBR_MASK);	//128 (9600 baud)

			/* Enable receiver */
			pUART->C2 |= UART_C2_RE_MASK;

			/* Enable transmitter */
			pUART->C2 |= UART_C2_TE_MASK;

			/* Enable Receiver interrupts*/
			pUART->C2 |= UART_C2_RIE_MASK;

			/* NVIC for USART */
			NVIC_SetPriority( UART1_IRQn, 1 );
			NVIC_EnableIRQ( UART1_IRQn );
			NVIC_ClearPendingIRQ( UART1_IRQn );
			break;

		case Un2:

			pUART = UART2;

			/* Enable the clock to the selected UART */
			SIM->SCGC |= SIM_SCGC_UART2_MASK;

			/* Make sure that the transmitter and receiver are disabled while we
			 * change settings.
			 */
			pUART->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);

			/* Configure the UART for 8-bit mode, no parity */
			pUART->C1 = 0;

			/* Calculate baud settings */
			u16Sbr = (((PbClk) >> 4) + (u32Baud >> 1)) / u32Baud;

			/* Save off the current value of the UARTx_BDH except for the SBR field */
			u8Temp = pUART->BDH & ~(UART_BDH_SBR_MASK);

			pUART->BDH = 0; //u8Temp | UART_BDH_SBR( u16Sbr >> 8 );		//0

			// How to calculate baudrate: https://community.nxp.com/thread/387950
			pUART->BDL = 128;		//(uint8_t)(u16Sbr & UART_BDL_SBR_MASK);	//128 (9600 baud)

			/* Enable receiver */
			pUART->C2 |= UART_C2_RE_MASK;

			/* Enable transmitter */
			pUART->C2 |= UART_C2_TE_MASK;


			/* Enable Receiver interrupts*/
			pUART->C2 |= UART_C2_RIE_MASK;
			/* Enable Transmitter interrupts*/
			//pUART->C2 |= UART_C2_TIE_MASK;

			NVIC_EnableIRQ( UART2_IRQn );

			break;
		default:
			//Det_ReportError( 0 );
			break;
	}

	Un_Inst[Channel].u8RxTail = Un_Inst[Channel].u8TxTail = 0;
	Un_Inst[Channel].u8RxHead = Un_Inst[Channel].u8TxHead = 0;

}

/***************************************************************************//**
 * \fn void Uart_DeInit(UART_Type *pUART)
 * \brief
 *   De-initialization routine of the module.
 * \details
 *
 * \param[in] pUART - Pointer to the desired peripheral instance.
 *
 * \return
 *   This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void Uart_DeInit(UART_Type *pUART)
{
	pUART->C2 &= ~((UART_C2_TE_MASK >> UART_C2_TE_SHIFT) | (UART_C2_RE_MASK >> UART_C2_RE_SHIFT));
}

/***************************************************************************//**
 * \fn void Uart_Read(uint8_t Channel, uint8_t* lpBuffer, uint8_t nNumberOfBytesToRead, uint8_t* lpNumberOfBytesRead)
 * \brief
 *	 Reading Data
 * \details
 *
 * \param[in] Channel - Identifies the peripheral instance.
 *
 * \param[in] lpBuffer - The buffer where data shall be read.
 *
 * \param[in] nNumberOfBytesToRead - The number of bytes to read.
 *
 * \param[out] lpNumberOfBytesRead - The number of bytes successfully read.
 *
 * \return
 * 	 This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void Uart_Read(uint8_t Channel, uint8_t* lpBuffer, uint8_t nNumberOfBytesToRead, uint8_t* lpNumberOfBytesRead)
{
	uint8_t *pBuff = NULL_PTR;
	uint8_t *pHead = NULL_PTR;
	uint8_t *pTail = NULL_PTR;

	/* Check if given channel is correct. Each channel have one structure assigned so pick the correct one  */
	if ( UART_TOTAL_NUMBER_OF_CHANNELS > Channel )
	{	// Check whether invalid data was requested or not
		if ( (NULL_PTR != lpBuffer) && (NULL_PTR != lpNumberOfBytesRead) )
		{
			pHead = &Un_Inst[Channel].u8RxHead;
			pTail = &Un_Inst[Channel].u8RxTail;
			pBuff = Un_Inst[Channel].u8RxBuff;
			// Init number of elements extracted from LIFO structure
			*lpNumberOfBytesRead = 0u;
			// Extract elements from storage buffer and write them to lpBuffer
			while ( (*pHead != *pTail) && (0u < nNumberOfBytesToRead) )
			{
				*pTail = (uint8_t)((*pTail + 1u) & UART_RX_BUF_SIZE_MASK);	//new tail
				*lpBuffer++ = pBuff[*pTail];	//update buffer
				nNumberOfBytesToRead--;
				(*lpNumberOfBytesRead)++;
			}
		}
	}
}

/***************************************************************************//**
 * \fn void Uart_Write(uint8_t Channel, const uint8_t* lpBuffer, uint8_t nNumberOfBytesToWrite, uint8_t* lpNumberOfBytesWritten)
 * \brief
 *	 Writing Data
 * \details
 *
 * \param[in] Channel - Identifies the peripheral instance.
 *
 * \param[in] lpBuffer - The buffer where data shall be written.
 *
 * \param[in] nNumberOfBytesToWrite - The number of bytes to write.
 *
 * \param[out] lpNumberOfBytesWritten - The number of bytes successfully written.
 *
 * \return
 * 	 This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void Uart_Write(uint8_t Channel, const uint8_t* lpBuffer, uint8_t nNumberOfBytesToWrite, uint8_t* lpNumberOfBytesWritten)
{
	UART_Type *pUART = NULL_PTR;
	uint8_t lb_Cond = 1u;
	uint8_t lu8_Head = 0u;
	uint8_t *pBuff = NULL_PTR;
	uint8_t *pHead = NULL_PTR;
	uint8_t *pTail = NULL_PTR;
	uint8_t *pStatusReg = NULL_PTR;
	uint8_t *pTxDataReg = NULL_PTR;
	uint8_t lu8_Un_STATUS_TXBL_MASK = 0ul;
	volatile uint8_t lu8_SimpleDelay = 0ul;
	/*Check if given channel is correct. Each channel have one structure assigned so pick the correct one*/
	if ( UART_TOTAL_NUMBER_OF_CHANNELS > Channel )
	{
		if ( NULL_PTR != lpNumberOfBytesWritten )
		{
			*lpNumberOfBytesWritten = 0u;
			if ( 0u == nNumberOfBytesToWrite )
			{
				return;
			}
			pHead = &Un_Inst[Channel].u8TxHead;
			pTail = &Un_Inst[Channel].u8TxTail;
			pBuff = Un_Inst[Channel].u8TxBuff;

			do
			{
				lu8_Head = (uint8_t)((*pHead + 1u) & UART_TX_BUF_SIZE_MASK);
				if ( *pTail != lu8_Head )
				{
					*pHead = lu8_Head; /* Update It */
					pBuff[lu8_Head] = *lpBuffer++;
					(*lpNumberOfBytesWritten)++;
					nNumberOfBytesToWrite--;
				}
				else
				{
					lb_Cond = 0u;
				}
			} while ( (0u != lb_Cond) && (0u < nNumberOfBytesToWrite) );

			switch ( Channel )
			{
				/*check registers*/
				case Un0:
					pUART = UART0;
					pStatusReg = (uint8_t*)&pUART->S1;
					pTxDataReg = (uint8_t*)&pUART->D;
					lu8_Un_STATUS_TXBL_MASK = UART_S1_TDRE_MASK;	//flag
					break;

				case Un1:
					pUART = UART1;
					pStatusReg = (uint8_t*)&pUART->S1;
					pTxDataReg = (uint8_t*)&pUART->D;
					lu8_Un_STATUS_TXBL_MASK = UART_S1_TDRE_MASK;	//flag
					break;

				case Un2:
					pUART = UART2;
					pStatusReg = (uint8_t*)&pUART->S1;
					pTxDataReg = (uint8_t*)&pUART->D;
					lu8_Un_STATUS_TXBL_MASK = UART_S1_TDRE_MASK;	//flag
					break;

				default:
					//Can not use it here as we got ....
					//Det_ReportErrorL( ModId_Usart, __LINE__ );
					return;
					break;
			}
			/*************************************************************************************************/
			/*************/
			/* Drive RTS */
			/* ********* */
			if ( Un_Inst[Channel].bRts == FALSE )
			{
				Un_Inst[Channel].bRts = TRUE;
				if ( NULL_PTR != Un_Inst[Channel].pfRTSCtl )
				{
					Un_Inst[Channel].pfRTSCtl( TRUE );

					/* Output enable time to high/low level tPZH = 120 ns   */
					/* Simple delay, MAX 120 ns                             */
					/* Fcpu = 16MHz, Tcpu = 20.8 ns                         */
					lu8_SimpleDelay = 10u;
					while ( 0u < lu8_SimpleDelay )
					{
						lu8_SimpleDelay--;
					}
				}
			}

			/*see if any data is available then grb a byte from the buffer*/
			while ( (*pHead != *pTail) && (0ul != (*pStatusReg & lu8_Un_STATUS_TXBL_MASK)) )
			{
				*pTail = (uint8_t)((*pTail + 1u) & UART_TX_BUF_SIZE_MASK);
				*pTxDataReg = pBuff[*pTail];
			}

			/* ******************************************* */
			/* Activate TX Empty or TX Complete interrupt? */
			/* ******************************************* */
			/*checks if data is available*/

			if ( *pHead != *pTail )
			{
				pUART->C2 |= UART_S1_TDRE_MASK;
			}
			else
			{
				pUART->C2 |= UART_C2_TCIE_MASK;
			}
		}
	}
}

/***************************************************************************//**
 * \fn void Un_RxISR(uint8_t Channel, uint8_t Data)
 * \brief
 * This function shall be called from interrupt.
 * It reads from serial port 1 byte and then push it back to the buffer within data structure.
 * \details
 *
 * \param[in] Channel - Identifies the peripheral instance.
 *
 * \param[in] Data - Defines the data character passed from interrupt routine.
 *
 * \return
 *   This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void Un_RxISR(uint8_t Channel, uint8_t Data)
{
	uint8_t *pBuff = NULL_PTR;
	uint8_t *pHead = NULL_PTR;
	uint8_t *pTail = NULL_PTR;
	uint8_t lu8_Head = 0u;

	// Check if given channel is correct. Each channel have one structure assigned so pick the correct one
	if ( UART_TOTAL_NUMBER_OF_CHANNELS > Channel )
	{
		//Assign initial values
		pHead = &Un_Inst[Channel].u8RxHead;
		pTail = &Un_Inst[Channel].u8RxTail;
		pBuff = Un_Inst[Channel].u8RxBuff;

		//Head Ptr updated
		lu8_Head = (uint8_t)((*pHead + 1u) & UART_RX_BUF_SIZE_MASK);

		//if Tail != Head ,update head with new value and update value in the Buffer
		if ( *pTail != lu8_Head )
		{
			*pHead = lu8_Head;
			pBuff[lu8_Head] = Data;
		}
	}

}

/***************************************************************************//**
 * \fn void Un_TxISR(uint8_t Channel)
 * \brief
 *	 This is the function used to send data via UART.
 * \details
 *
 * \param[in] Channel - Identifies the peripheral instance.
 *
 * \return
 *   This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void Un_TxISR(uint8_t Channel)
{
	UART_Type *pUART = NULL_PTR;
	uint8_t *pBuff = NULL_PTR;
	uint8_t *pHead = NULL_PTR;
	uint8_t *pTail = NULL_PTR;
	uint8_t *pStatusReg = NULL_PTR;
	uint8_t *pTxDataReg = NULL_PTR;
	uint8_t lu8_Un_STATUS_TXBL_MASK = 0ul;

	if ( UART_TOTAL_NUMBER_OF_CHANNELS > Channel )
	{
		pHead = &Un_Inst[Channel].u8TxHead;
		pTail = &Un_Inst[Channel].u8TxTail;
		pBuff = Un_Inst[Channel].u8TxBuff;
		/* ********************************************** */
		/* Get The Address Of STATUS And TXDATA Registers */
		/* ********************************************** */
		switch ( Channel )
		{
			case Un0:
				pUART = UART0;
				pStatusReg = (uint8_t*)&pUART->S1; //used extract TDRE field from status register
				pTxDataReg = (uint8_t*)&pUART->D; //tx data write register
				lu8_Un_STATUS_TXBL_MASK = UART_S1_TDRE_MASK;
				break;
			case Un1:
				pUART = UART1;
				pStatusReg = (uint8_t*)&pUART->S1; //used extract TDRE field from status register
				pTxDataReg = (uint8_t*)&pUART->D; //tx data write register
				lu8_Un_STATUS_TXBL_MASK = UART_S1_TDRE_MASK;
				break;
			case Un2:
				pUART = UART2;
				pStatusReg = (uint8_t*)&pUART->S1; //used extract TDRE field from status register
				pTxDataReg = (uint8_t*)&pUART->D; //tx data write register
				lu8_Un_STATUS_TXBL_MASK = UART_S1_TDRE_MASK;
				break;
			default:
				return;
				break;
		}

		/* *********************************************************************** */
		/* Arm hardware TX Fifo until it is full or no more bytes are to be TXmited */
		/* *********************************************************************** */
		while ( (*pHead != *pTail) && (0ul != (*pStatusReg & lu8_Un_STATUS_TXBL_MASK)) ) // transmit data
		{
			*pTail = (uint8_t)((*pTail + 1u) & UART_TX_BUF_SIZE_MASK);
			*pTxDataReg = pBuff[*pTail];
		}

		/* ***************************************************************************** */
		/* If there are more bytes to be TXmited - enable TX Empty, otherwise TX Complete */
		/* ***************************************************************************** */
		if ( *pHead != *pTail )
		{
			pUART->C2 |= UART_C2_TE_MASK;
		}
		else
		{
			pUART->C2 |= UART_C2_TCIE_MASK;
		}
	}
}

uint8_t Uart_GetRxQueueLevel(uint8_t Channel)
{
	return  (Un_Inst[Channel].u8RxHead -  Un_Inst[Channel].u8RxTail);
}

uint8_t Uart_GetTxQueueLevel(uint8_t Channel)
{
	return  (Un_Inst[Channel].u8TxHead -  Un_Inst[Channel].u8TxTail);
}

// Only for debug
#if 0
void UART_PutChar(UART_Type *pUART, uint8_t u8Char)
{
	(void)UART2_S1; /* Clear reception flag mechanism*/

	/* Wait until space is available in the FIFO */
	while (!(pUART->S1 & UART_S1_TDRE_MASK));

	/* Send the character */
	pUART->D = (uint8_t)u8Char;
}

// Only for debug
uint8_t UART_GetChar(UART_Type *pUART)
{
	(void)UART2_S1; /* Clear reception flag mechanism*/

	/* Wait until character has been received */
	while (!(pUART->S1 & UART_S1_RDRF_MASK));

	/* Return the 8-bit data from the receiver */
	return pUART->D;
}

#endif
