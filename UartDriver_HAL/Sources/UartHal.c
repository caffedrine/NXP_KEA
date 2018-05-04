/*! @file HalUart.c
    @brief Definition of the Uart driver module.
 */

#include "UartHal.h"
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

#define UART_TX_BUF_SIZE_MASK                   (UART_TX_BUF_SIZE - 1u)
#if (UART_TX_BUF_SIZE & UART_TX_BUF_SIZE_MASK)
#error UART_TX_BUF_SIZE is not power of 2
#endif

#define UART_RX_BUF_SIZE_MASK                   (UART_RX_BUF_SIZE - 1u)
#if (UART_RX_BUF_SIZE & UART_RX_BUF_SIZE_MASK)
#error UART_RX_BUF_SIZE is not power of 2
#endif

/* Local Prototypes */
static UInt8 Boot_GetNextByte(UInt8 a);

static void Un_TxISR (UInt8 Channel);
static void Un_RxISR (UInt8 Channel, UInt8 data);

void TheApp_Uart_Notification_Callback(UART_Type *pUART, UInt8 u8Data);
#define UnRx_Notification_Calback(arg1, arg2)   TheApp_Uart_Notification_Callback( arg1, arg2 )

/*! \struct Un_DataType
    \brief The Uart data type instance definition.
 */
typedef struct
{
    UInt8 u8RxBuff[UART_RX_BUF_SIZE];   /**< The RX data buffer */
    UInt8 u8TxBuff[UART_TX_BUF_SIZE];   /**< The TX data buffer */
    UInt16 u16TxHead;   /**< Holds the head within TX buffer */
    UInt16 u16TxTail;   /**< Holds the tail within TX buffer */
    UInt16 u16RxHead;   /**< Holds the head within RX buffer */
    UInt16 u16RxTail;   /**< Holds the tail within RX buffer */
    UInt8 u8Wake;       /**< Non zero if the Wakeup notification is to be sent to the EcuM */

    UInt32 u32PbClk;    /**< Holds the peripheral bus clock */
    UInt32 u32BaudRate; /**< Holds the peripheral baud rate */
    BOOL bRts;                  /**< Holds the actual RTS status */
    void (*pfRTSCtl)(BOOL);     /**< A pointer to function that control the RTS status */

    UInt8 u8BootByteLast;   /**< Used for enter boot request bytes sequence identifier. Holds the latest received byte. */
    UInt8 u8BootByteCount;  /**< Used for enter boot request bytes sequence identifier. Holds the number of bytes the sequence is expected. */
} Un_DataType;

/*! \var static Un_DataType Un_Inst[ UART_TOTAL_NUMBER_OF_CHANNELS ]
    \brief The Uart instances declaration.
 */
static Un_DataType Un_Inst[ UART_TOTAL_NUMBER_OF_CHANNELS ];

void Un0_RTSCtl(BOOL bStatus);

/***************************************************************************//**
 * \fn void USART1_Handler(void)
 * \brief   The UART ISR.
 * \details
 * \return  This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void USART1_Handler(void)
{
//    BOOL lb_InvalidInvocation = TRUE;
//
//    /*  */
//    if ( 0u != Un_Inst[Un0].u8Wake ) {
//        EcuM_SetWakeupEvent( ECUM_WKSOURCE_Un0 );
//    }
//
//    /* UART parity error interrupt occurred ------------------------------------- */
//    if ( (0ul != (USART1->ISR & USART_ISR_PE)) && (0ul != (USART1->CR1 & USART_CR1_PEIE)) )
//    {
//        (void)USART1->RDR;              /* Discard received data */
//        USART1->ICR = USART_ICR_PECF;   /* Clear flag            */
//        lb_InvalidInvocation = FALSE;
//    }
//
//    /* UART frame error interrupt occurred -------------------------------------- */
//    if ( (0ul != (USART1->ISR & USART_ISR_FE)) && (0ul != (USART1->CR3 & USART_CR3_EIE)) )
//    {
//        (void)USART1->RDR;              /* Discard received data */
//        USART1->ICR = USART_ICR_FECF;   /* Clear flag            */
//        lb_InvalidInvocation = FALSE;
//    }
//
//    /* UART noise error interrupt occurred -------------------------------------- */
//    if ( (0ul != (USART1->ISR & USART_ISR_NE)) && (0ul != (USART1->CR3 & USART_CR3_EIE)) )
//    {
//        (void)USART1->RDR;              /* Discard received data */
//        USART1->ICR = USART_ICR_NCF;    /* Clear flag            */
//        lb_InvalidInvocation = FALSE;
//    }
//
//    /* UART Over-Run interrupt occurred ----------------------------------------- */
//    if ( (0ul != (USART1->ISR & USART_ISR_ORE)) && (0ul != (USART1->CR3 & USART_CR3_EIE)) )
//    {
//        (void)USART1->RDR;              /* Discard received data */
//        USART1->ICR = USART_ICR_ORECF;  /* Clear flag            */
//        lb_InvalidInvocation = FALSE;
//    }
//
//    /* UART RX-NotEmpty interrupt occurred ----------------------------------------- */
//    if ( (0ul != (USART1->ISR & USART_ISR_RXNE)) && (0ul != (USART1->CR1 & USART_CR1_RXNEIE)) )
//    {
//        Un_RxISR( Un0, (UInt8)USART1->RDR );
//        lb_InvalidInvocation = FALSE;
//    }
//
//
//    /* ###################################################################### */
//    /* ################################### */
//    //
//    // Based on number of bytes in TxFifo, Un_TxISR, shall enable the TXE or TXC interrupt.
//
//    /* UART TX-Empty interrupt occurred ----------------------------------------- */
//    if ( (0ul != (USART1->ISR & USART_ISR_TXE)) && (0ul != (USART1->CR1 & USART_CR1_TXEIE)) ) {
//        USART1->CR1 &= ~USART_CR1_TXEIE;
//        Un_TxISR( Un0 );
//        lb_InvalidInvocation = FALSE;
//    }
//
//    /* UART TX-Complete interrupt occurred ----------------------------------------- */
//    if ( (0ul != (USART1->ISR & USART_ISR_TC)) && (0ul != (USART1->CR1 & USART_CR1_TCIE)) )
//    {
//        USART1->CR1 &= ~USART_CR1_TCIE;             /* Clear interrupt enable flag */
//        USART1->ICR = USART_ICR_TCCF;               /* Clear flag                  */
//        if ( FALSE != Un_Inst[Un0].bRts )           /* Clear RTS Status            */
//        {
//            Un_Inst[Un0].bRts = FALSE;
//            if ( NULL_PTR != Un_Inst[Un0].pfRTSCtl ) {
//                Un_Inst[Un0].pfRTSCtl( FALSE );     /* Release transceiver TE line */
//            }
//        }
//        lb_InvalidInvocation = FALSE;
//    }
//
//    /* ********** */
//    /* Crazy Uart */
//    /* ********** */
//    if ( FALSE != lb_InvalidInvocation )
//    {
//        LL_APB1_GRP2_ForceReset( LL_APB1_GRP2_PERIPH_USART1 );
//        _nop(); _nop(); _nop(); _nop(); _nop(); _nop(); _nop(); _nop();
//        _nop(); _nop(); _nop(); _nop(); _nop(); _nop(); _nop(); _nop();
//        LL_APB1_GRP2_ReleaseReset( LL_APB1_GRP2_PERIPH_USART1 );
//
//        /* (Re)Initialize it accordingly to latest settings */
//        if ( 0ul != Un_Inst[Un0].u32BaudRate ) {
//            Uart_Init( Un0, Un_Inst[Un0].u32PbClk, Un_Inst[Un0].u32BaudRate );
//        }
//    }
}


/***************************************************************************//**
 * \fn void Uart_Init(UInt8 Channel, UInt32 PbClk, UInt32 BaudRate)
 * \brief
 *   Initialization routine of the module.
 * \details
 *   Upon system start-up, call this function to initialize the module before
 *   any other function call.
 * \param[in] Channel
 *   Identifies the peripheral instance. It is a pointer to UART_Type macro defined in CPU headers
 * \param[in] PbClk
 *   Peripheral bus clock.
 * \param[in] BaudRate
 *   Desired baud rate.
 * \return
 *   This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void Uart_Init(UART_Type *pUART, UInt32 u32SysClk, UInt32 u32Baud)
{
    UInt16 u16Sbr;
    UInt8 u8Temp;

	/* Enable the clock to the selected UART */
    if (pUART == UART0)
	{
		SIM->SCGC |= SIM_SCGC_UART0_MASK;
	}
	else if (pUART == UART1)
	{
        SIM->SCGC |= SIM_SCGC_UART1_MASK;
	}
    else
	{
        SIM->SCGC |= SIM_SCGC_UART2_MASK;
	}
    /* Make sure that the transmitter and receiver are disabled while we
     * change settings.
     */
    pUART->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );

    /* Configure the UART for 8-bit mode, no parity */
    pUART->C1 = 0;

    /* Calculate baud settings */
    u16Sbr = (((u32SysClk)>>4) + (u32Baud>>1))/u32Baud;

    /* Save off the current value of the UARTx_BDH except for the SBR field */
    u8Temp = pUART->BDH & ~(UART_BDH_SBR_MASK);

    pUART->BDH = u8Temp |  UART_BDH_SBR(u16Sbr >> 8);		//0

    // How to calculate baudrate: https://community.nxp.com/thread/387950
    pUART->BDL = 128;//(uint8_t)(u16Sbr & UART_BDL_SBR_MASK);	//128 (9600 baud)

    /* Enable receiver */
    pUART->C2 |= UART_C2_RE_MASK;

    /* Enable transmitter */
    pUART->C2 |= UART_C2_TE_MASK;

	/* Enable Receiver interrupts*/
	pUART->C2 |= UART_C2_RIE_MASK;
}


/***************************************************************************//**
 * \fn void Usart_DeInit(void)
 * \brief
 *   De-initialization routine of the module.
 * \details
 *
 * \return
 *   This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void Usart_DeInit(UART_Type *pUART)
{
    /* Set to after reset state an disable UART Tx/Rx */
    pUART->C2 = 0x00;
    pUART->BDH = 0x00;
    pUART->BDL = 0x04;
    pUART->C1 = 0;

    /* Disable clock to UART */
    SIM->SCGC &= (0xFFFFFFFF) & (~SIM_SCGC_UART0_MASK);
    SIM->SCGC &= (0xFFFFFFFF) & (~SIM_SCGC_UART1_MASK);
    SIM->SCGC &= (0xFFFFFFFF) & (~SIM_SCGC_UART2_MASK);
}


/***************************************************************************//**
 * \fn void Usart_Write(UInt8 Channel, UInt8* lpBuffer, UInt8 nNumberOfBytesToWrite, UInt8* lpNumberOfBytesWritten)
 * \brief
 * \details
 * \param[in] Channel
 * \param[in] lpBuffer
 * \param[in] nNumberOfBytesToWrite
 * \param[out] lpNumberOfBytesWritten
 * \return  This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void Uart_Write(UInt8 Channel, const UInt8* lpBuffer, UInt8 nNumberOfBytesToWrite, UInt8* lpNumberOfBytesWritten)
{
    UInt8  lb_Cond = 1u;
    UInt16 lu16_Head = 0u;
    UInt8  *pBuff = NULL_PTR;
    UInt16 *pHead = NULL_PTR;
    UInt16 *pTail = NULL_PTR;
    UInt32 *pStatusReg = NULL_PTR;
    UInt32 *pTxDataReg = NULL_PTR;
    UInt32 lu32_Un_STATUS_TXBL_MASK = 0ul;
    volatile UInt32 lu32_SimpleDelay = 0ul;

    /*  */
    if ( UART_TOTAL_NUMBER_OF_CHANNELS > Channel )
    {
        if ( NULL_PTR != lpNumberOfBytesWritten )
        {
            *lpNumberOfBytesWritten = 0u;
            if ( 0u == nNumberOfBytesToWrite ) {
                return;
            }

            pHead = &Un_Inst[Channel].u16TxHead;
            pTail = &Un_Inst[Channel].u16TxTail;
            pBuff = Un_Inst[Channel].u8TxBuff;

            do {
                lu16_Head = (UInt16)((*pHead + 1u) & UART_TX_BUF_SIZE_MASK);
                if ( *pTail != lu16_Head ) {
                    *pHead = lu16_Head;         /* Update It */
                    pBuff[lu16_Head] = *lpBuffer++;
                    (*lpNumberOfBytesWritten)++;
                    nNumberOfBytesToWrite--;
                }
                else {
                    lb_Cond = 0u;
                }
            } while ( (0u != lb_Cond) && (0u < nNumberOfBytesToWrite) );

            switch ( Channel )
            {
                case Un0:
//                    pStatusReg = (UInt32*)&USART1->ISR;
//                    pTxDataReg = (UInt32*)&USART1->TDR;
//                    lu32_Un_STATUS_TXBL_MASK = USART_ISR_TXE;
                    break;

                default:
                    // Can not use it here as we got ....
                    //Det_ReportErrorL( ModId_Usart, __LINE__ );
                    return;
                    break;
            }

            /* ********* */
            /* Drive RTS */
            /* ********* */
            if ( FALSE == Un_Inst[Channel].bRts )
            {
                Un_Inst[Channel].bRts = TRUE;
                if ( NULL_PTR != Un_Inst[Channel].pfRTSCtl )
                {
                    Un_Inst[Channel].pfRTSCtl( TRUE );

                    /* Output enable time to high/low level tPZH = 120 ns   */
                    /* Simple delay, MAX 120 ns                             */
                    /* Fcpu = 16MHz, Tcpu = 20.8 ns                         */
                    lu32_SimpleDelay = 10u;
                    while( 0u < lu32_SimpleDelay ) {
                        lu32_SimpleDelay--;
                    }
                }
            }

            /*  */
            while ( (*pHead != *pTail) && (0ul != (*pStatusReg & lu32_Un_STATUS_TXBL_MASK)) ) {
                *pTail = (UInt16)((*pTail + 1u) & UART_TX_BUF_SIZE_MASK);
                *pTxDataReg = pBuff[*pTail];
            }

            /* ******************************************* */
            /* Activate TX Empty or TX Complete interrupt? */
            /* ******************************************* */
//            if ( *pHead != *pTail ) {
//                USART1->CR1 |= USART_CR1_TXEIE;
//            }
//            else {
//                USART1->CR1 |= USART_CR1_TCIE;
//            }
        }
    }
}


/***************************************************************************//**
 * \fn void Usart_Read(UInt8 Channel, UInt8* lpBuffer, UInt8 nNumberOfBytesToRead, UInt8* lpNumberOfBytesRead)
 * \brief
 *
 * \details
 *
 * \param[in] Channel
 *
 * \param[in] lpBuffer
 *
 * \param[in] nNumberOfBytesToRead
 *
 * \param[out] lpNumberOfBytesRead
 *
 * \return
 *   This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void Uart_Read(UInt8 Channel, UInt8* lpBuffer, UInt8 nNumberOfBytesToRead, UInt8* lpNumberOfBytesRead)
{
    UInt8  *pBuff = NULL_PTR;
    UInt16 *pHead = NULL_PTR;
    UInt16 *pTail = NULL_PTR;

    /*  */
    if ( UART_TOTAL_NUMBER_OF_CHANNELS > Channel )
    {
        if ( (NULL_PTR != lpBuffer) && (NULL_PTR != lpNumberOfBytesRead) )
        {
            pHead = &Un_Inst[Channel].u16RxHead;
            pTail = &Un_Inst[Channel].u16RxTail;
            pBuff = Un_Inst[Channel].u8RxBuff;

            *lpNumberOfBytesRead = 0u;
            while ( (*pHead != *pTail) && (0u < nNumberOfBytesToRead) )
            {
                *pTail = (UInt16)((*pTail + 1u) & UART_RX_BUF_SIZE_MASK);
                *lpBuffer++ = pBuff[*pTail];
                nNumberOfBytesToRead--;
                (*lpNumberOfBytesRead)++;
            }
        }
    }


/***************************************************************************//**
 * \fn void Un_RxISR(UInt8 Channel, UInt8 Data)
 * \brief
 *
 * \details
 *
 * \param[in] Channel
 *
 * \param[in] Data
 *
 * \return
 *   This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void Un_RxISR(UInt8 Channel, UInt8 Data)
{
    UInt8  *pBuff = NULL_PTR;
    UInt16 *pHead = NULL_PTR;
    UInt16 *pTail = NULL_PTR;
    UInt16 lu16_Head = 0u;

    if ( UART_TOTAL_NUMBER_OF_CHANNELS > Channel )
    {
        UnRx_Notification_Calback( Channel, Data );

        pHead = &Un_Inst[Channel].u16RxHead;
        pTail = &Un_Inst[Channel].u16RxTail;
        pBuff = Un_Inst[Channel].u8RxBuff;

        lu16_Head = (UInt16)((*pHead + 1u) & UART_RX_BUF_SIZE_MASK);

        if ( *pTail != lu16_Head )
        {
            *pHead = lu16_Head;
            pBuff[lu16_Head] = Data;
        }
        else {
            Det_ReportError( 0 );
        }


        /* ################################################ */
        /* ################################################ */
        /* Check boot enter sequence */
        /* ************************* */
        if ( Data == Boot_GetNextByte(Un_Inst[Channel].u8BootByteLast) )
        {
            if ( 0u < Un_Inst[Channel].u8BootByteCount ) {
                Un_Inst[Channel].u8BootByteCount--;
            }

            if ( 0u == Un_Inst[Channel].u8BootByteCount )
            {
//                AppBootIf_Inst.u32IfVersion        = 1ul;
//                AppBootIf_Inst.u32BootEnterKey1    = BOOT_ENTER_KEY1;
//                AppBootIf_Inst.u32BootEnterKey2    = BOOT_ENTER_KEY2;
//                AppBootIf_Inst.u32BootSysTickVal   = Mcu_SysTickRead();
//                AppBootIf_Inst.u32BootSysTickLoad  = Mcu_SysTickPeriodGet();
//                AppBootIf_Inst.u32BootEnterChannel = COMIF_UART(Un0);
//
//                Mcu_PerformReset( MCU_RESET_REASON_SWR_ENTER_BOOT );
            }
        }
        else {
            Un_Inst[Channel].u8BootByteCount = 50u;
        }

        Un_Inst[Channel].u8BootByteLast = Data;
    }
}


/***************************************************************************//**
 * \fn void Un_TxISR(UInt8 Channel)
 * \brief
 *
 * \details
 *
 * \param[in] Channel
 *
 * \return
 *   This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void Un_TxISR(UInt8 Channel)
{
    UInt8  *pBuff = NULL_PTR;
    UInt16 *pHead = NULL_PTR;
    UInt16 *pTail = NULL_PTR;
    UInt32 *pStatusReg = NULL_PTR;
    UInt32 *pTxDataReg = NULL_PTR;
    UInt32 lu32_Un_STATUS_TXBL_MASK = 0ul;

    if ( UART_TOTAL_NUMBER_OF_CHANNELS > Channel )
    {
        pHead = &Un_Inst[Channel].u16TxHead;
        pTail = &Un_Inst[Channel].u16TxTail;
        pBuff = Un_Inst[Channel].u8TxBuff;

        /* ********************************************** */
        /* Get The Address Of STATUS And TXDATA Registers */
        /* ********************************************** */
        switch ( Channel )
        {
            case Un0:
//                pStatusReg = (UInt32*)&USART1->ISR;
//                pTxDataReg = (UInt32*)&USART1->TDR;
//                lu32_Un_STATUS_TXBL_MASK = USART_ISR_TXE;
                break;

            default:
                Det_ReportError( 0 );
                break;
        }

        /* *********************************************************************** */
        /* Arm hardware TX Fifo until it is full or no more bytes are to be Xmited */
        /* *********************************************************************** */
        while ( (*pHead != *pTail) && (0ul != (*pStatusReg & lu32_Un_STATUS_TXBL_MASK)) )
        {
            *pTail = (UInt16)((*pTail + 1u) & UART_TX_BUF_SIZE_MASK);
            *pTxDataReg = pBuff[*pTail];
        }

        /* ***************************************************************************** */
        /* If there are more bytes to be Xmited - enable TX Empty, otherwise TX Complete */
        /* ***************************************************************************** */
//        if ( *pHead != *pTail ) {
//            USART1->CR1 |= USART_CR1_TXEIE;
//        }
//        else {
//            USART1->CR1 |= USART_CR1_TCIE;
//        }
    }
}


/***************************************************************************//**
 * \fn void Usart_EnableWakeup(UInt8 Channel)
 * \brief
 * \details
 * \param[in] Channel
 * \return  This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void Uart_EnableWakeup(UInt8 Channel)
{
    if ( UART_TOTAL_NUMBER_OF_CHANNELS > Channel )
    {
        Un_Inst[Channel].u8Wake = 1u;

        switch ( Channel )
        {
        case Un0:
            ;
            break;

        default:
            Det_ReportError( 0 );
            break;
        }
    }
    else {
        Det_ReportError( 0 );
    }
}


/***************************************************************************//**
 * \fn void Usart_DisableWakeup(UInt8 Channel)
 * \brief
 * \details
 * \param[in] Channel
 * \return  This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void Uart_DisableWakeup(UInt8 Channel)
{
    if ( UART_TOTAL_NUMBER_OF_CHANNELS > Channel )
    {
        Un_Inst[Channel].u8Wake = 0u;

        switch ( Channel )
        {
        case Un0:
            ;
            break;

        default:
            Det_ReportError( 0 );
            break;
        }
    }
    else {
        Det_ReportError( 0 );
    }
}


/***************************************************************************//**
 * \fn void Uart_GetRxQueueLevel(UInt8 Channel, UInt8* lpLevel)
 * \brief
 * \details
 * \param[in] Channel
 * \param[out] lpLevel
 * \return  This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void Uart_GetRxQueueLevel(UInt8 Channel, UInt8* lpLevel)
{
    UInt16 *pHead = NULL_PTR;
    UInt16 *pTail = NULL_PTR;

    /*  */
    if ( UART_TOTAL_NUMBER_OF_CHANNELS > Channel )
    {
        pHead = &Un_Inst[Channel].u16RxHead;
        pTail = &Un_Inst[Channel].u16RxTail;

        *lpLevel = (UInt8)((*pHead - *pTail) & (UInt16)UART_RX_BUF_SIZE_MASK);
    }
    else {
        *lpLevel = (UInt16)0u;
        Det_ReportError( 0 );
    }
}


/***************************************************************************//**
 * \fn void Uart_GetTxQueueLevel(UInt8 Channel, UInt8* lpLevel)
 * \brief
 * \details
 * \param[in] Channel
 * \param[out] lpLevel
 * \return  This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void Uart_GetTxQueueLevel(UInt8 Channel, UInt8* lpLevel)
{
    UInt16 *pHead = NULL_PTR;
    UInt16 *pTail = NULL_PTR;

    /*  */
    if ( UART_TOTAL_NUMBER_OF_CHANNELS > Channel )
    {
        pHead = &Un_Inst[Channel].u16TxHead;
        pTail = &Un_Inst[Channel].u16TxTail;

        *lpLevel = (UInt8)((*pHead - *pTail) & (UInt16)UART_TX_BUF_SIZE_MASK);
    }
    else {
        *lpLevel = (UInt16)0u;
        Det_ReportError( 0 );
    }
}


/***************************************************************************//**
 * \fn void Un0_RTSCtl(BOOL bStatus)
 * \brief 
 *   
 * \details
 *   TX_EN
 * \param[in] bStatus
 *   The logical level to be set on the corresponding pin.
 * \return
 *   This function has no return value.
 * \callgraph
 * \callergraph
 ******************************************************************************/
void Un0_RTSCtl(BOOL bStatus) {
    //HalGpio_Write_TX_EN( bStatus );
}


/***************************************************************************//**
 * \fn static UInt8 Boot_GetNextByte(UInt8 a)
 * \brief 
 *   
 * \details
 *   
 * \param[in] a
 *   
 * \return
 *   
 * \callgraph
 * \callergraph
 ******************************************************************************/
static UInt8 Boot_GetNextByte(UInt8 a)
{
    UInt8 carry = 0u, i = 0u;

    /*  */
    for ( i = 0u; i < 3u; i++ )
    {
        carry = 0u;
        if ( 0x00u != (a & 0x01u) ) {
            carry = 1u;
        }
        a = (UInt8)(a >> 1);
        if ( 0x00u != carry ) {
            a = (UInt8)(a | 0x80u);
        }
    }
    return (UInt8)((UInt8)(a ^ 0x6Du) + 0x0Du);
}

/** @} (end addtogroup Uart) */
/** @} (end addtogroup Peripherals) */


// Later added
// This function handles every char received from given UART

void TheApp_Uart_Notification_Callback(UART_Type *pUART, UInt8 u8Data)
{
	UART_PutChar(pUART, u8Data);
}


void UART_PutChar(UART_Type *pUART, uint8_t u8Char)
{
    /* Wait until space is available in the FIFO */
    while (!(pUART->S1 & UART_S1_TDRE_MASK));

    /* Send the character */
    pUART->D = (uint8_t)u8Char;
}

uint8_t UART_GetChar(UART_Type *pUART)
{
    /* Wait until character has been received */
    while (!(pUART->S1 & UART_S1_RDRF_MASK));

    /* Return the 8-bit data from the receiver */
    return pUART->D;
}

//// Handle interrupt

void UART2_IRQHandler()
{
	(void)UART2_S1; /* Clear reception flag mechanism*/
	TheApp_Uart_Notification_Callback(UART2, UART_GetChar(UART2) );
}

//void UART1_IRQHandler()
//{
//	(void)UART1_S1; /* Clear reception flag mechanism*/
//	TheApp_Uart_Notification_Callback(UART1, UART_GetChar(UART1) );
//}
//
//void UART0_IRQHandler()
//{
//	(void)UART0_S1; /* Clear reception flag mechanism*/
//	TheApp_Uart_Notification_Callback(UART0, UART_GetChar(UART0) );
//}

