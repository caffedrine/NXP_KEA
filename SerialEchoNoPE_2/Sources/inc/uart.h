#ifndef _UART_H_
#define _UART_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
* Includes
******************************************************************************/
#include "derivative.h"

/******************************************************************************
* Constants
******************************************************************************/


/******************************************************************************
* Macros
******************************************************************************/
#define MAX_UART_NO             3

/******************************************************************************
* Types
******************************************************************************/

/******************************************************************************
*define uart setting type
*
*//*! @addtogroup uart_setting_type
* @{
*******************************************************************************/  

/*!
* @brief UART setting type.
*
*/ 

typedef struct
{
    uint32_t bEnable      : 1;                /*!< 1: enable, 0: disable */
    uint32_t resvd        : 31;               /*!< 1: reserved bit field */
} UART_SettingType;
/*! @} End of uart_setting_type */



/******************************************************************************
*define uart config baudrate type
*
*//*! @addtogroup uart_config_baudrate_type
* @{
******************************************************************************/
 /*!
 * @brief UART baudrate type structure.
 *
 */   
typedef struct
{
    uint32_t    u32SysClkHz;        /*!< system clock */
    uint32_t    u32Baudrate;        /*!< UART baudrate */
} UART_ConfigBaudrateType;
/*! @} End of uart_config_baudrate_type */


/******************************************************************************
* Global variables
******************************************************************************/
enum UART_pinsel
{
	PTB0_PTB1 = 0,
	PTA2_PTA3 = 1,
	PTC6_PTC7 = 2,
	PTF2_PTF3 = 3,
	PTD6_PTD7 = 4,
	PTI0_PTI1 = 5
};

/******************************************************************************
* Inline functions
******************************************************************************/

/******************************************************************************
* define UART APIs
*
*//*! @addtogroup uart_api_list
* @{
*******************************************************************************/

/*****************************************************************************//*!
*
* @brief read receive buffer
*        
* @param[in] pUART       base of UART port
*
* @return unsign char received char
*
*****************************************************************************/
__STATIC_INLINE uint8_t UART_ReadDataReg(UART_Type *pUART)
{
    /* Return the 8-bit data from the receiver */
    return pUART->D;
}
/*****************************************************************************//*!
*
* @brief write transmit buffer
*        
* @param[in] pUART       base of UART port
* @param[in] u8Char      char to send
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_WriteDataReg(UART_Type *pUART, uint8_t u8Char)
{
    /* Send the character */
    pUART->D = (uint8_t)u8Char;
}

/*****************************************************************************//*!
*
* @brief check if a character has been received
*
* @param[in] pUART  base of UART port
*
* @return 0, No character received; no-zero, Character has been received
*
* @ Pass/ Fail criteria:
*****************************************************************************/
__STATIC_INLINE uint8_t UART_CharPresent(UART_Type *pUART)
{  
    return (pUART->S1 & UART_S1_RDRF_MASK);
}
/*****************************************************************************//*!
*
* @brief enable transmit
*        
* @param[in] pUART       base of UART port
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_EnableTx(UART_Type *pUART)
{
    
    pUART->C2 |= UART_C2_TE_MASK;
}
/*****************************************************************************//*!
*
* @brief disable transmit
*        
* @param[in] pUART       base of UART port
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_DisableTx(UART_Type *pUART)
{    
    pUART->C2 &= (~UART_C2_TE_MASK);
}
/*****************************************************************************//*!
*
* @brief enable receive
*        
* @param[in] pUART       base of UART port
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_EnableRx(UART_Type *pUART)
{    
    pUART->C2 |= UART_C2_RE_MASK;
}
/*****************************************************************************//*!
*
* @brief disable receive
*        
* @param[in] pUART       base of UART port
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_DisableRx(UART_Type *pUART)
{    
    pUART->C2 &= (~UART_C2_RE_MASK);
}
/*****************************************************************************//*!
*
* @brief Enable loopback mode
*        
* @param[in] pUART       base of UART port
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_EnableLoopback(UART_Type *pUART)
{
    pUART->C1 |= UART_C1_LOOPS_MASK;
    pUART->C1 &= (~UART_C1_RSRC_MASK);
}
/*****************************************************************************//*!
*
* @brief enable single wire mode
*        
* @param[in] pUART       base of UART port
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_EnableSingleWire(UART_Type *pUART)
{
    pUART->C1 |= UART_C1_LOOPS_MASK;
    pUART->C1 |= UART_C1_RSRC_MASK;
}
/*****************************************************************************//*!
*
* @brief set 8-bit mode
*        
* @param[in] pUART       base of UART port
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_Set8BitMode(UART_Type *pUART)
{
    pUART->C1 &= (~UART_C1_M_MASK);
}
/*****************************************************************************//*!
*
* @brief set 9-bit mode
*        
* @param[in] pUART       base of UART port
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_Set9BitMode(UART_Type *pUART)
{
    pUART->C1 |= UART_C1_M_MASK;
}
/*****************************************************************************//*!
*
* @brief enable transmit buffer empty interrupt
*        
* @param[in] pUART       base of UART port
*
* @return none
*
* @ Pass/ Fail criteria:
*****************************************************************************/
__STATIC_INLINE void UART_EnableTxBuffEmptyInt(UART_Type *pUART)
{
    pUART->C2 |= UART_C2_TIE_MASK;
}
/*****************************************************************************//*!
*
* @brief enable transmit complete interrupt
*        
* @param[in] pUART       base of UART port
*
* @return none
*
* @ Pass/ Fail criteria:
*****************************************************************************/
__STATIC_INLINE void UART_EnableTxCompleteInt(UART_Type *pUART)
{
    pUART->C2 |= UART_C2_TCIE_MASK;
}
/*****************************************************************************//*!
*
* @brief enable receive buffer full interrupt
*        
* @param[in] pUART       base of UART port
*
* @return none
*
* @ Pass/ Fail criteria:
*****************************************************************************/
__STATIC_INLINE void UART_EnableRxBuffFullInt(UART_Type *pUART)
{
    pUART->C2 |= UART_C2_RIE_MASK;
}
/*****************************************************************************//*!
*
* @brief disable transmit buffer empty interrupt
*        
* @param[in] pUART       base of UART port
*
* @return none
*
* @ Pass/ Fail criteria:
*****************************************************************************/
__STATIC_INLINE void UART_DisableTxBuffEmptyInt(UART_Type *pUART)
{
        pUART->C2 &= (~UART_C2_TIE_MASK);    
}
/*****************************************************************************//*!
*
* @brief disable transmit complete interrupt
*        
* @param[in] pUART       base of UART port
*
* @return none
*
* @ Pass/ Fail criteria:
*****************************************************************************/
__STATIC_INLINE void UART_DisableTxCompleteInt(UART_Type *pUART)
{
    pUART->C2 &= (~UART_C2_TCIE_MASK);   
}
/*****************************************************************************//*!
*
* @brief disable receive buffer full interrupt
*        
* @param[in] pUART       base of UART port
*
* @return none
*
* @ Pass/ Fail criteria:
*****************************************************************************/
__STATIC_INLINE void UART_DisableRxBuffFullInt(UART_Type *pUART)
{
    pUART->C2 &= (~UART_C2_RIE_MASK);  
}
/*****************************************************************************//*!
*
* @brief print out break character
*        
* @param[in] pUART  base of UART port
*
* @return       none
*
* @ Pass/ Fail criteria:
*****************************************************************************/
__STATIC_INLINE void UART_PutBreak(UART_Type *pUART)
{
    /* Write 1 then write 0 to UART_C2[SBK] bit, will put break character */
    pUART->C2 |= UART_C2_SBK_MASK; 
    pUART->C2 &= (~UART_C2_SBK_MASK);
}

/*****************************************************************************//*!
*
* @brief check whether tx is complete,i.e. data has been sent out.
*        
* @param[in] pUART  base of UART port
*
* @return       
*               1, Tx complete flag is set
*               0, Tx complete flag is clear
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
__STATIC_INLINE uint8_t UART_IsTxComplete(UART_Type *pUART)
{
    return (pUART->S1 & UART_S1_TC_MASK);
}
/*****************************************************************************//*!
*
* @brief check whether Tx buffer is empty
*        
* @param[in] pUART  base of UART port
*
* @return       
*               1, Tx buffer is empty
*               0, Tx buffer is not empty
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
__STATIC_INLINE uint8_t UART_IsTxBuffEmpty(UART_Type *pUART)
{
    return (pUART->S1 & UART_S1_TDRE_MASK);
}
/*****************************************************************************//*!
*
* @brief check whether Rx buffer is full, i.e. receive a character
*        
* @param[in] pUART  base of UART port
*
* @return       
*               1, Rx buffer is full
*               0, Rx buffer is not full
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
__STATIC_INLINE uint8_t UART_IsRxBuffFull(UART_Type *pUART)
{
    return (pUART->S1 & UART_S1_RDRF_MASK);
}
/*! @} End of uart_api_list */


/******************************************************************************
* Global functions declaration
******************************************************************************/
void UART_pin_settings(enum UART_pinsel uart_pins);
void UART_Init(UART_Type *pUART, uint32_t u32SysClk, uint32_t u32Baud);
void UART_Reset(UART_Type *pUART);
uint8_t UART_GetChar(UART_Type *pUART);
void UART_PutChar(UART_Type *pUART, uint8_t u8Char);
void UART_SendWait(UART_Type *pUART, uint8_t *pSendBuff, uint32_t u32Length);
void UART_ReceiveWait(UART_Type *pUART, uint8_t *pReceiveBuff, uint32_t u32Length);
void UART_WaitTxComplete(UART_Type *pUART);



#ifdef __cplusplus
}
#endif
#endif /* #ifndef _UART_H_ */
