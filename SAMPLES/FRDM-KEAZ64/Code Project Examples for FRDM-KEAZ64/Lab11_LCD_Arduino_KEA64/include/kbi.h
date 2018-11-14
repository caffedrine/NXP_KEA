/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2014 Freescale Semiconductor, Inc.
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
* @file kbi.h
*
* @author Freescale
*
* 
* @brief header file for KBI. 
*
*******************************************************************************
*
* provide APIs for accessing KBI
******************************************************************************/
#ifndef _KBI_H_
#define _KBI_H_
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
* Includes
******************************************************************************/
#include "derivative.h"
#include "nvic.h"

#define KBI_MAX_PINS_PER_PORT   32                  /*!< max number of pins */
//#define KBI_Type KBI_MemMapPtr
/******************************************************************************
* Constants
******************************************************************************/

/*!
* @brief KBI MODE select enum.
*
*/
typedef enum
{
    KBI_MODE_EDGE_ONLY = 0,             /*!< select  edge only mode  */         
    KBI_MODE_EDGE_LEVEL                 /*!< select  both edge and level  mode */
}KBI_ModeType;


 /*!
 * @brief KBI Edge select enum.
 *
 */
typedef enum
{
    KBI_FALLING_EDGE_LOW_LEVEL = 0,     /*!< select falling edge and/or low level  */
    KBI_RISING_EDGE_HIGH_LEVEL          /*!< select rising edge and/or high level  */
}KBI_EdgeType;


/******************************************************************************
* Macros
******************************************************************************/

/******************************************************************************
* KBI module max number and port pins definition
*
*//*! @addtogroup kbi_macro
* @{
*******************************************************************************/
#define KBI_MAX_NO              2                  /*!< max number of modules */

/*! @} End of kbi_macro                                                    */


/******************************************************************************
* Types
******************************************************************************/

/*! @brief KBI_CALLBACK function declaration                                  */
typedef void (*KBI_CallbackType)(void);                     
/*! @} End of kbi_callback                                                    */



/******************************************************************************
* KBI pin config struct
*
*//*! @addtogroup kbi_pinconfigstruct
* @{
*******************************************************************************/
/*!
* @brief KBI pin enable and edge select struct.
*
*/

typedef struct
{
    uint8_t     bEdge   : 1;                                /*!< edge/level select bit */
    uint8_t     bEn     : 1;                                /*!< pin enable bit */
    uint8_t     bRsvd   : 6;                                /*!< reserved */
} KBI_PinConfigType;
/*! @} End of kbi_pinconfigstruct                                             */


/******************************************************************************
* KBI config struct
*
*//*! @addtogroup kbi_configstruct
* @{
*******************************************************************************/
/*!
 * @brief KBI status and control struct.
 *
 */

typedef struct
{

    struct
    {
        uint32_t     bMode   : 1;                            /*!< KBI detection mode select */
        uint32_t     bIntEn  : 1;                            /*!< KBI interrupt enable bit */
        uint32_t     bRsvd2  : 2;                           /*!< reserved */
        uint32_t     bKbspEn : 1;							  /*!<Real KBI_SP register enable*/
		uint32_t     bRstKbsp: 1;                            /*!<Reset KBI_SP register*/
        uint32_t     bRsvd26 : 26;                           /*!< reserved */
    } sBits;
    KBI_PinConfigType   sPin[KBI_MAX_PINS_PER_PORT];
} KBI_ConfigType, *KBI_ConfigTypePtr;
/*! @} End of kbi_configstruct                                                */

/******************************************************************************
* Global variables
******************************************************************************/
/*!
 * inline functions
 */
/******************************************************************************
* KBI api list
*
*//*! @addtogroup kbi_api_list
* @{
*******************************************************************************/
/*****************************************************************************//*!
*
* @brief set detect falling edge only.
*        
* @param[in]   pKBI          pointer to KBI module.
* @param[in]   PinMasks      indicate pin numbers.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
* @see KBI_DetectRisingEdge.
*
*****************************************************************************/


static inline  void KBI_DetectFallingEdge(KBI_Type* pKBI, uint32_t PinMasks)

{
    pKBI->SC &= ~KBI_SC_KBMOD_MASK;
    pKBI->ES &= ~(PinMasks);
}

/*****************************************************************************//*!
*
* @brief set detect falling edge only.
*        
* @param[in]   pKBI         pointer to KBI module.
* @param[in]   PinMasks     indicate pin numbers.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
* @see KBI_DetectFallingEdge.
*
*****************************************************************************/

static inline  void KBI_DetectRisingEdge(KBI_Type* pKBI, uint32_t PinMasks)

{
    pKBI->SC &= ~KBI_SC_KBMOD_MASK;
    pKBI->ES |= (PinMasks);    
}

/*****************************************************************************//*!
*
* @brief set detect falling edge only.
*        
* @param[in]   pKBI         pointer to KBI module.
* @param[in]   PinMasks     indicate pin number/mask.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
* @see KBI_DetectFallingEdgeLowLevel.
*
*****************************************************************************/

static inline  void KBI_DetectRisingEdgeHighLevel(KBI_Type* pKBI, uint32_t PinMasks)

{
    pKBI->SC |= KBI_SC_KBMOD_MASK;
    pKBI->ES |= (PinMasks);    
}

/*****************************************************************************//*!
*
* @brief set detect falling edge only.
*        
* @param[in]   pKBI         pointer to KBI module.
* @param[in]   PinMasks     indicate pin number/mask.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
* @see KBI_DetectRisingEdgeHighLevel.
*
*****************************************************************************/

static inline  void KBI_DetectFallingEdgeLowLevel(KBI_Type* pKBI, uint32_t PinMasks)

{
    pKBI->SC |= KBI_SC_KBMOD_MASK;
    pKBI->ES &= ~(PinMasks);        
}

/*****************************************************************************//*!
*
* @brief enable the pin specified.
*        
* @param[in]   pKBI         pointer to KBI module.
* @param[in]   PinMasks     indicate pin number/mask.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
* @see KBI_Disable.
*
*****************************************************************************/

static inline  void KBI_Enable(KBI_Type* pKBI, uint32_t PinMasks)

{
    pKBI->PE |= (PinMasks);        
}

/*****************************************************************************//*!
*
* @brief disable the pin specified.
*        
* @param[in]   pKBI         pointer to KBI module.
* @param[in]   PinMasks     indicate pin number/mask.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
* @see KBI_Enable.
*
*****************************************************************************/

static inline  void KBI_Disable(KBI_Type* pKBI, uint32_t PinMasks)

{
    pKBI->PE &= ~(PinMasks);        
}

/*****************************************************************************//*!
*
* @brief enable the corresponding interrupt.
*        
* @param[in]   pKBI         pointer to KBI module.
*          
* @return none.
*
* @ Pass/ Fail criteria: none.
*
* @see KBI_DisableInt.
*
*****************************************************************************/
static inline  void KBI_EnableInt(KBI_Type* pKBI)
{
    pKBI->SC |= KBI_SC_KBIE_MASK;        
}


/*****************************************************************************//*!
*
* @brief disable the corresponding interrupt.
*        
* @param[in]  pKBI         pointer to KBI module.
*          
* @return none.
*
* @ Pass/ Fail criteria: none
*
* @see KBI_EnableInt.
*
*****************************************************************************/
static inline  void KBI_DisableInt(KBI_Type* pKBI)
{
    pKBI->SC &= ~KBI_SC_KBIE_MASK;        
}

/*****************************************************************************//*!
*
* @brief Get the corresponding status flag bits.
*        
* @param[in]   pKBI         pointer to KBI module.
*          
* @return uint8_t.
*
* @ Pass/ Fail criteria: none.
*
* @see KBI_ClrFlags.
*
*****************************************************************************/

static inline  uint32_t KBI_GetFlags(KBI_Type* pKBI)

{
    return (pKBI->SC & KBI_SC_KBF_MASK);        
}

/*****************************************************************************//*!
*
* @brief clear the corresponding status flag bits.
*        
* @param[in]   pKBI         pointer to KBI module
*          
* @return none.
*
* @ Pass/ Fail criteria: none
*
* @see KBI_GetFlags.
*
*****************************************************************************/
static inline  void KBI_ClrFlags(KBI_Type* pKBI)
{
    pKBI->SC |= KBI_SC_KBACK_MASK;        
}

/*****************************************************************************//*!
*
* @brief Real KBI_SP register enable.
*        
* @param[in]   pKBI         pointer to KBI module
*          
* @return none.
*
* @ Pass/ Fail criteria: none
*
* @see The real time value of Keyboard source pin to be read.
*
*****************************************************************************/
static inline  void KBI_SPEnable(KBI_Type* pKBI)
{
    pKBI->SC |= KBI_SC_KBSPEN_MASK;        
}

/*****************************************************************************//*!
*
* @brief Get KBI source pin register fields.
*        
* @param[in]   pKBI         pointer to KBI module.
*          
* @return uint32_t.
*
* @ Pass/ Fail criteria: none.
*
* @see KBI_GetSP.
*
*****************************************************************************/
static inline  uint32_t KBI_GetSP(KBI_Type* pKBI)
{
    return (pKBI->SP & KBI_SP_SP_MASK);        
}

/*****************************************************************************//*!
*
* @brief Reset KBI_SP register.
*        
* @param[in]   pKBI         pointer to KBI module
*          
* @return none.
*
* @ Pass/ Fail criteria: none
*
* @see KBI_RstSP.
*
*****************************************************************************/
static inline  void KBI_RstSP(KBI_Type* pKBI)
{
    pKBI->SC |= KBI_SC_RSTKBSP_MASK;        
}


/*! @} End of kbi_api_list                                                  */

/******************************************************************************
* Global functions
******************************************************************************/

void KBI_Init(KBI_Type* pKBI, KBI_ConfigType *pConfig);
void KBI_SetCallback(KBI_Type* pKBI, KBI_CallbackType pfnCallback);
void KBI_DeInit(KBI_Type* pKBI);


#endif 
