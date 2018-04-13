/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2013 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
***************************************************************************//*!
*
* @file uart.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date June-6-2013
*
* @brief providing common UART API. 
*
******************************************************************************/
#include "uart.h"

/******************************************************************************
* Local variables
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local functions
*****************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

/******************************************************************************
* define UART APIs
*
*//*! @addtogroup uart_api_list
* @{
*******************************************************************************/

/*****************************************************************************//*!
*
* @brief initialize the UART, interrupts disabled, and no hardware flow-control.
*        
* @param[in] pUART       base of UART port
* @param[in] pConfig     pointer to UART configuration structure
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void UART_Init(UART_Type *pUART, uint32_t u32SysClk, uint32_t u32Baud)
{
    uint16_t u16Sbr;
    uint8_t u8Temp;
  
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
    
    pUART->BDH = u8Temp |  UART_BDH_SBR(u16Sbr >> 8);
    pUART->BDL = (uint8_t)(u16Sbr & UART_BDL_SBR_MASK);

    /* Enable receiver and transmitter */
    pUART->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK );
}

void UART_Reset(UART_Type *pUART)
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

/*****************************************************************************//*!
*
* @brief receive a character.
*        
* @param[in] pUART       base of UART port
*
* @return unsigned char
*
*****************************************************************************/
uint8_t UART_GetChar(UART_Type *pUART)
{
 
    /* Wait until character has been received */
    while (!(pUART->S1 & UART_S1_RDRF_MASK));
    
    /* Return the 8-bit data from the receiver */
    return pUART->D;
}
/*****************************************************************************//*!
*
* @brief send a character.
*        
* @param[in] pUART       base of UART port
* @param[in] u8Char      char to send
*
* @return none
*
*****************************************************************************/
void UART_PutChar(UART_Type *pUART, uint8_t u8Char)
{    
    /* Wait until space is available in the FIFO */
    while (!(pUART->S1 & UART_S1_TDRE_MASK));
    
    /* Send the character */
    pUART->D = (uint8_t)u8Char;
}


/*****************************************************************************//*!
*
* @brief send a series of characters using polling mode.
*        
* @param[in] pUART      base of UART port
* @param[in] pSendBuff  pointer of characters to send
* @param[in] u32Length  number of characters
*
* @return       none
*
* @ Pass/ Fail criteria:
*****************************************************************************/
void UART_SendWait(UART_Type *pUART, uint8_t *pSendBuff, uint32_t u32Length)
{
    uint8_t u8TxChar;
    uint32_t  i;
    
    for (i = 0; i < u32Length; i++)
    {
        u8TxChar = pSendBuff[i];
        while (!UART_IsTxBuffEmpty(pUART))
        {
            #if defined(ENABLE_WDOG)
                WDOG_Feed();
            #endif        
        }
        UART_WriteDataReg(pUART, u8TxChar);        
    }
}

/*****************************************************************************//*!
*
* @brief receive a series of charecters using polling mode.
*        
* @param[in] pUART          base of UART port
* @param[in] pReceiveBuff   pointer of charecters to receive
* @param[in] u32Length      number of charecters
*
* @return       none
*
* @ Pass/ Fail criteria:
*****************************************************************************/
void UART_ReceiveWait(UART_Type *pUART, uint8_t *pReceiveBuff, uint32_t u32Length)
{
    uint8_t u8RxChar;
    uint32_t i;
    
    for (i = 0; i < u32Length; i++)
    {
        while (!UART_IsRxBuffFull(pUART))
        {
            #if defined(ENABLE_WDOG)
                WDOG_Feed();
            #endif       
        }    
        u8RxChar = UART_ReadDataReg(pUART);
        pReceiveBuff[i] = u8RxChar;
    }
}

/*****************************************************************************//*!
*
* @brief wait tx complete.
*        
* @param[in] pUART      base of UART port
*
* @return       none
*
* @ Pass/ Fail criteria: none*****************************************************************************/
void UART_WaitTxComplete(UART_Type *pUART)
{
    while (!UART_IsTxComplete(pUART));
}



/*! @} End of uart_api_list */
