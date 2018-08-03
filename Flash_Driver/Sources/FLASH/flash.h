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
* @file flash.h
*
* @author Freescale
*
* @brief header file for FLASH module utilities (FLASH). 
*
*******************************************************************************
*
* provide APIs for accessing FLASH module (FLASH)
******************************************************************************/

#ifndef FLASH_H_
#define FLASH_H_
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
#ifdef SKEAZN642

	/* ----------------------------------------------------------------------------
	   -- FTMRE Peripheral Access Layer
	   ---------------------------------------------------------------------------- */

	/*!
	 * @addtogroup FTMRE_Peripheral_Access_Layer FTMRE Peripheral Access Layer
	 * @{
	 */

	/** FTMRE - Register Layout Typedef */
	typedef struct {
	  __IO uint8_t FCLKDIV;                            /**< Flash Clock Divider Register, offset: 0x0 */
	  __I  uint8_t FSEC;                               /**< Flash Security Register, offset: 0x1 */
	  __IO uint8_t FCCOBIX;                            /**< Flash CCOB Index Register, offset: 0x2 */
	       uint8_t RESERVED_0[1];
	  __IO uint8_t FCNFG;                              /**< Flash Configuration Register, offset: 0x4 */
	  __IO uint8_t FERCNFG;                            /**< Flash Error Configuration Register, offset: 0x5 */
	  __IO uint8_t FSTAT;                              /**< Flash Status Register, offset: 0x6 */
	  __IO uint8_t FERSTAT;                            /**< Flash Error Status Register, offset: 0x7 */
	  __IO uint8_t FPROT;                              /**< Flash Protection Register, offset: 0x8 */
	  __IO uint8_t EEPROT;                             /**< EEPROM Protection Register, offset: 0x9 */
	  __IO uint8_t FCCOBHI;                            /**< Flash Common Command Object Register:High, offset: 0xA */
	  __IO uint8_t FCCOBLO;                            /**< Flash Common Command Object Register: Low, offset: 0xB */
	  __I  uint8_t FOPT;                               /**< Flash Option Register, offset: 0xC */
	} FTMRE_Type, *FTMRE_MemMapPtr;

	/* ----------------------------------------------------------------------------
	   -- FTMRE - Register accessor macros
	   ---------------------------------------------------------------------------- */

	/*!
	 * @addtogroup FTMRE_Register_Accessor_Macros FTMRE - Register accessor macros
	 * @{
	 */


	/* FTMRE - Register accessors */
	#define FTMRE_FCLKDIV_REG(base)                  ((base)->FCLKDIV)
	#define FTMRE_FSEC_REG(base)                     ((base)->FSEC)
	#define FTMRE_FCCOBIX_REG(base)                  ((base)->FCCOBIX)
	#define FTMRE_FCNFG_REG(base)                    ((base)->FCNFG)
	#define FTMRE_FERCNFG_REG(base)                  ((base)->FERCNFG)
	#define FTMRE_FSTAT_REG(base)                    ((base)->FSTAT)
	#define FTMRE_FERSTAT_REG(base)                  ((base)->FERSTAT)
	#define FTMRE_FPROT_REG(base)                    ((base)->FPROT)
	#define FTMRE_EEPROT_REG(base)                   ((base)->EEPROT)
	#define FTMRE_FCCOBHI_REG(base)                  ((base)->FCCOBHI)
	#define FTMRE_FCCOBLO_REG(base)                  ((base)->FCCOBLO)
	#define FTMRE_FOPT_REG(base)                     ((base)->FOPT)

	/*!
	 * @}
	 */ /* end of group FTMRE_Register_Accessor_Macros */


	/* ----------------------------------------------------------------------------
	   -- FTMRE Register Masks
	   ---------------------------------------------------------------------------- */

	/*!
	 * @addtogroup FTMRE_Register_Masks FTMRE Register Masks
	 * @{
	 */

	/* FCLKDIV Bit Fields */
	#define FTMRE_FCLKDIV_FDIV_MASK                  0x3Fu
	#define FTMRE_FCLKDIV_FDIV_SHIFT                 0
	#define FTMRE_FCLKDIV_FDIV(x)                    (((uint8_t)(((uint8_t)(x))<<FTMRE_FCLKDIV_FDIV_SHIFT))&FTMRE_FCLKDIV_FDIV_MASK)
	#define FTMRE_FCLKDIV_FDIVLCK_MASK               0x40u
	#define FTMRE_FCLKDIV_FDIVLCK_SHIFT              6
	#define FTMRE_FCLKDIV_FDIVLD_MASK                0x80u
	#define FTMRE_FCLKDIV_FDIVLD_SHIFT               7
	/* FSEC Bit Fields */
	#define FTMRE_FSEC_SEC_MASK                      0x3u
	#define FTMRE_FSEC_SEC_SHIFT                     0
	#define FTMRE_FSEC_SEC(x)                        (((uint8_t)(((uint8_t)(x))<<FTMRE_FSEC_SEC_SHIFT))&FTMRE_FSEC_SEC_MASK)
	#define FTMRE_FSEC_KEYEN_MASK                    0xC0u
	#define FTMRE_FSEC_KEYEN_SHIFT                   6
	#define FTMRE_FSEC_KEYEN(x)                      (((uint8_t)(((uint8_t)(x))<<FTMRE_FSEC_KEYEN_SHIFT))&FTMRE_FSEC_KEYEN_MASK)
	/* FCCOBIX Bit Fields */
	#define FTMRE_FCCOBIX_CCOBIX_MASK                0x7u
	#define FTMRE_FCCOBIX_CCOBIX_SHIFT               0
	#define FTMRE_FCCOBIX_CCOBIX(x)                  (((uint8_t)(((uint8_t)(x))<<FTMRE_FCCOBIX_CCOBIX_SHIFT))&FTMRE_FCCOBIX_CCOBIX_MASK)
	/* FCNFG Bit Fields */
	#define FTMRE_FCNFG_FSFD_MASK                    0x1u
	#define FTMRE_FCNFG_FSFD_SHIFT                   0
	#define FTMRE_FCNFG_FDFD_MASK                    0x2u
	#define FTMRE_FCNFG_FDFD_SHIFT                   1
	#define FTMRE_FCNFG_IGNSF_MASK                   0x10u
	#define FTMRE_FCNFG_IGNSF_SHIFT                  4
	#define FTMRE_FCNFG_CCIE_MASK                    0x80u
	#define FTMRE_FCNFG_CCIE_SHIFT                   7
	/* FERCNFG Bit Fields */
	#define FTMRE_FERCNFG_SFDIE_MASK                 0x1u
	#define FTMRE_FERCNFG_SFDIE_SHIFT                0
	#define FTMRE_FERCNFG_DFDIE_MASK                 0x2u
	#define FTMRE_FERCNFG_DFDIE_SHIFT                1
	/* FSTAT Bit Fields */
	#define FTMRE_FSTAT_MGSTAT_MASK                  0x3u
	#define FTMRE_FSTAT_MGSTAT_SHIFT                 0
	#define FTMRE_FSTAT_MGSTAT(x)                    (((uint8_t)(((uint8_t)(x))<<FTMRE_FSTAT_MGSTAT_SHIFT))&FTMRE_FSTAT_MGSTAT_MASK)
	#define FTMRE_FSTAT_MGBUSY_MASK                  0x8u
	#define FTMRE_FSTAT_MGBUSY_SHIFT                 3
	#define FTMRE_FSTAT_FPVIOL_MASK                  0x10u
	#define FTMRE_FSTAT_FPVIOL_SHIFT                 4
	#define FTMRE_FSTAT_ACCERR_MASK                  0x20u
	#define FTMRE_FSTAT_ACCERR_SHIFT                 5
	#define FTMRE_FSTAT_CCIF_MASK                    0x80u
	#define FTMRE_FSTAT_CCIF_SHIFT                   7
	/* FERSTAT Bit Fields */
	#define FTMRE_FERSTAT_SFDIF_MASK                 0x1u
	#define FTMRE_FERSTAT_SFDIF_SHIFT                0
	#define FTMRE_FERSTAT_DFDIF_MASK                 0x2u
	#define FTMRE_FERSTAT_DFDIF_SHIFT                1
	/* FPROT Bit Fields */
	#define FTMRE_FPROT_FPLS_MASK                    0x3u
	#define FTMRE_FPROT_FPLS_SHIFT                   0
	#define FTMRE_FPROT_FPLS(x)                      (((uint8_t)(((uint8_t)(x))<<FTMRE_FPROT_FPLS_SHIFT))&FTMRE_FPROT_FPLS_MASK)
	#define FTMRE_FPROT_FPLDIS_MASK                  0x4u
	#define FTMRE_FPROT_FPLDIS_SHIFT                 2
	#define FTMRE_FPROT_FPHS_MASK                    0x18u
	#define FTMRE_FPROT_FPHS_SHIFT                   3
	#define FTMRE_FPROT_FPHS(x)                      (((uint8_t)(((uint8_t)(x))<<FTMRE_FPROT_FPHS_SHIFT))&FTMRE_FPROT_FPHS_MASK)
	#define FTMRE_FPROT_FPHDIS_MASK                  0x20u
	#define FTMRE_FPROT_FPHDIS_SHIFT                 5
	#define FTMRE_FPROT_RNV6_MASK                    0x40u
	#define FTMRE_FPROT_RNV6_SHIFT                   6
	#define FTMRE_FPROT_FPOPEN_MASK                  0x80u
	#define FTMRE_FPROT_FPOPEN_SHIFT                 7
	/* EEPROT Bit Fields */
	#define FTMRE_EEPROT_DPS_MASK                    0x7u
	#define FTMRE_EEPROT_DPS_SHIFT                   0
	#define FTMRE_EEPROT_DPS(x)                      (((uint8_t)(((uint8_t)(x))<<FTMRE_EEPROT_DPS_SHIFT))&FTMRE_EEPROT_DPS_MASK)
	#define FTMRE_EEPROT_DPOPEN_MASK                 0x80u
	#define FTMRE_EEPROT_DPOPEN_SHIFT                7
	/* FCCOBHI Bit Fields */
	#define FTMRE_FCCOBHI_CCOB_MASK                  0xFFu
	#define FTMRE_FCCOBHI_CCOB_SHIFT                 0
	#define FTMRE_FCCOBHI_CCOB(x)                    (((uint8_t)(((uint8_t)(x))<<FTMRE_FCCOBHI_CCOB_SHIFT))&FTMRE_FCCOBHI_CCOB_MASK)
	/* FCCOBLO Bit Fields */
	#define FTMRE_FCCOBLO_CCOB_MASK                  0xFFu
	#define FTMRE_FCCOBLO_CCOB_SHIFT                 0
	#define FTMRE_FCCOBLO_CCOB(x)                    (((uint8_t)(((uint8_t)(x))<<FTMRE_FCCOBLO_CCOB_SHIFT))&FTMRE_FCCOBLO_CCOB_MASK)
	/* FOPT Bit Fields */
	#define FTMRE_FOPT_NV_MASK                       0xFFu
	#define FTMRE_FOPT_NV_SHIFT                      0
	#define FTMRE_FOPT_NV(x)                         (((uint8_t)(((uint8_t)(x))<<FTMRE_FOPT_NV_SHIFT))&FTMRE_FOPT_NV_MASK)

	/*!
	 * @}
	 */ /* end of group FTMRE_Register_Masks */


	/* FTMRE - Peripheral instance base addresses */
	/** Peripheral FTMRE base address */
	#define FTMRE_BASE                               (0x40020000u)
	/** Peripheral FTMRE base pointer */
	#define FTMRE                                    ((FTMRE_Type *)FTMRE_BASE)
	#define FTMRE_BASE_PTR                           (FTMRE)
	/** Array initializer of FTMRE peripheral base addresses */
	#define FTMRE_BASE_ADDRS                         { FTMRE_BASE }
	/** Array initializer of FTMRE peripheral base pointers */
	#define FTMRE_BASE_PTRS                          { FTMRE }

	/* ----------------------------------------------------------------------------
	   -- FTMRE - Register accessor macros
	   ---------------------------------------------------------------------------- */

	/*!
	 * @addtogroup FTMRE_Register_Accessor_Macros FTMRE - Register accessor macros
	 * @{
	 */


	/* FTMRE - Register instance definitions */
	/* FTMRE */
	#define FTMRE_FCLKDIV                            FTMRE_FCLKDIV_REG(FTMRE)
	#define FTMRE_FSEC                               FTMRE_FSEC_REG(FTMRE)
	#define FTMRE_FCCOBIX                            FTMRE_FCCOBIX_REG(FTMRE)
	#define FTMRE_FCNFG                              FTMRE_FCNFG_REG(FTMRE)
	#define FTMRE_FERCNFG                            FTMRE_FERCNFG_REG(FTMRE)
	#define FTMRE_FSTAT                              FTMRE_FSTAT_REG(FTMRE)
	#define FTMRE_FERSTAT                            FTMRE_FERSTAT_REG(FTMRE)
	#define FTMRE_FPROT                              FTMRE_FPROT_REG(FTMRE)
	#define FTMRE_EEPROT                             FTMRE_EEPROT_REG(FTMRE)
	#define FTMRE_FCCOBHI                            FTMRE_FCCOBHI_REG(FTMRE)
	#define FTMRE_FCCOBLO                            FTMRE_FCCOBLO_REG(FTMRE)
	#define FTMRE_FOPT                               FTMRE_FOPT_REG(FTMRE)

	/*!
	 * @}
	 */ /* end of group FTMRE_Register_Accessor_Macros */


	/*!
	 * @}
	 */ /* end of group FTMRE_Peripheral_Access_Layer */

#endif


/******************************************************************************
* Uncomment the following line to support programming flash while running code from flash 
*
*//*! @addtogroup flash_stalling_control_enable
* @{
*******************************************************************************/
#define TRUE	1
#define	FALSE	0
#define FLASH_ENABLE_STALLING_FLASH_CONTROLLER	/* enable flash stalling control */

/*! @} End of flash_stalling_control_enable                                  */

/******************************************************************************
* Memory Controller Command Completion Status Flag
*
*//*! @addtogroup flash_MGSTAT_list
* @{
*******************************************************************************/

#define FTMRE_FSTAT_MGSTAT0_MASK  (1)						/* FTMRE FSTAT MGSTAT0 MASK */
#define FTMRE_FSTAT_MGSTAT1_MASK  (1<<1)					/* FTMRE FSTAT MGSTAT1 MASK */
/*! @} End of flash_MGSTAT_list                                                */

#define FLASH_SECTOR_SIZE			512						/* FTMRE sector size*/
#define FLASH_START_ADDRESS			0						/* FTMRE start address */
#define EEPROM_START_ADDRESS		0x10000000				/* EEPRON start address*/

/******************************************************************************
* flash and eeprom driver error status
*
*//*! @addtogroup flash_error_list
* @{
*******************************************************************************/
#define FLASH_ERR_BASE				0x3000										/* FTMRE error base */
#define FLASH_ERR_SUCCESS			0											/* FTMRE success */
#define FLASH_ERR_INVALID_PARAM		(FLASH_ERR_BASE+1)							/* Invalid parameter error code */
#define EEPROM_ERR_SINGLE_BIT_FAULT	(FLASH_ERR_BASE+2)							/* EEPROM single bit fault error code */
#define EEPROM_ERR_DOUBLE_BIT_FAULT	(FLASH_ERR_BASE+4)							/* EEPROM double bits fault error code */
#define FLASH_ERR_ACCESS			(FLASH_ERR_BASE+8)							/* Flash access error code */
#define FLASH_ERR_PROTECTION		(FLASH_ERR_BASE+0x10)						/* Flash protection error code */
#define FLASH_ERR_MGSTAT0			(FLASH_ERR_BASE+0x11)						/* Flash verification error code */
#define FLASH_ERR_MGSTAT1			(FLASH_ERR_BASE+0x12)						/* Flash non-correctable error code */
#define FLASH_ERR_INIT_CCIF			(FLASH_ERR_BASE+0x14)						/* Flash driver init error with CCIF = 1 */
#define FLASH_ERR_INIT_FDIV			(FLASH_ERR_BASE+0x18)						/* Flash driver init error with wrong FDIV */
/*! @} End of flash_error_list                                                */

/******************************************************************************
* Flash and EEPROM commands
*
*//*! @addtogroup flash_command_list
* @{
*******************************************************************************/
#define FTMRE_CMD_ERASE_VERIFY_ALL						0x01					/* FTMRE erase verify all command */
#define FTMRE_CMD_ERASE_VERIFY_BLOCK					0x02					/* FTMRE erase verify block command */
#define FTMRE_CMD_ERASE_ALL								0x08					/* FTMRE erase all command */
#define FTMRE_CMD_ERASE_BLOCK							0x09					/* FTMRE erase blockcommand */
#define FTMRE_CMD_UNSECURE								0x0B					/* FTMRE unsecure command */
#define FTMRE_CMD_SET_USER_MARGIN						0x0D					/* FTMRE set usermargin command */

#define FLASH_CMD_ERASE_VERIFY_SECTION					0x03					/* FTMRE erase verify section command */
#define FLASH_CMD_READONCE								0x04					/* FTMRE read once command */
#define FLASH_CMD_PROGRAM								0x06					/* FTMRE program command */
#define FLASH_CMD_PROGRAMONCE							0x07					/* FTMRE program once command */
#define FLASH_CMD_ERASE_SECTOR							0x0A					/* FTMRE erase sector command */
#define FLASH_CMD_BACKDOOR_ACCESS						0x0C					/* FTMRE backdoor key access command */
#define FLASH_CMD_SET_USER_MARGIN_LEVEL					0x0D					/* FTMRE set user margin level command */

#define EEPROM_CMD_ERASE_VERIFY_SECTION					0x10					/* EEPROM erase berify section command */
#define EEPROM_CMD_PROGRAM								0x11					/* EEPROM program command */
#define EEPROM_CMD_ERASE_SECTOR 						0x12					/* EEPROM erase sector command */
/*! @} End of flash_command_list                                              */


/******************************************************************************
*
*//*! @addtogroup nvm_api_list
* @{
*******************************************************************************/
/******************************************************************************
* inline function
******************************************************************************/

/*****************************************************************************//*!
  *
  * @brief Command Complete Interrupt Enable.
  *        
  * @param none.
  *
  * @return none.
  *
  * @ Pass/ Fail criteria: none.
*****************************************************************************/
__STATIC_INLINE void FLASH_IntEnable( void )
{
	FTMRE_FCNFG |= FTMRE_FCNFG_CCIE_MASK;
}
/*****************************************************************************//*!
  *
  * @brief Command Complete Interrupt Disable.
  *        
  * @param none.
  *
  * @return none.
  *
  * @ Pass/ Fail criteria: none.
*****************************************************************************/
__STATIC_INLINE void FLASH_IntDisable( void )
{
	FTMRE_FCNFG &= ~FTMRE_FCNFG_CCIE_MASK;
}


/* Align bits down */
__STATIC_INLINE uint32_t align_down(uint32_t data, uint32_t base)
{
	return (data & ~(base - 1));
}

/* align bits up */
__STATIC_INLINE uint32_t align_up(uint32_t data, uint32_t base)
{
	return align_down( data + base - 1, base );
}
/******************************************************************************
* Types
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/
uint16_t FLASH_Program1LongWord(uint32_t wNVMTargetAddress, uint32_t dwData);
uint16_t FLASH_Program2LongWords(uint32_t wNVMTargetAddress, uint32_t dwData0, uint32_t dwData1);
uint16_t FLASH_ProgramOnce(uint8_t bPhraseIndex, uint8_t *pData8Bytes);
uint16_t FLASH_ReadOnce(uint8_t bPhraseIndex, uint8_t *pData8Bytes);
uint16_t FLASH_EraseSector(uint32_t wNVMTargetAddress);
uint16_t FLASH_EraseVerifySection(uint32_t wNVMTargetAddress, uint16_t uiLongWordCount);
uint16_t FLASH_VerifyBackdoorKey(uint8_t *pbKey);
uint16_t FLASH_SetUserMarginLevel(uint32_t wNVMTargetAddress, uint16_t wMarginLevel, uint8_t bIsEEPROM);
uint16_t FLASH_Program(uint32_t wNVMTargetAddress, uint8_t *pData, uint16_t sizeBytes);

uint16_t FLASH_EraseBlock(uint32_t wNVMTargetAddress, uint8_t bIsEEPROM);
uint16_t FLASH_EraseVerifyBlock(uint32_t wNVMTargetAddress,uint8_t bIsEEPROM);
uint16_t FLASH_EraseAll(void);
uint16_t FLASH_EraseVerifyAll(void);
uint16_t FLASH_Unsecure(void);
uint16_t FLASH_EraseBlock_NoWait(uint32_t wNVMTargetAddress, uint8_t bIsEEPROM);
uint16_t FLASH_CheckErrStatus(void);

void FLASH_LaunchCMD(uint8_t bWaitComplete);

uint16_t EEPROM_Program1Byte(uint32_t wNVMTargetAddress, uint8_t bData);
uint16_t EEPROM_ProgramUpto4Bytes(uint32_t wNVMTargetAddress, uint8_t *pbData, uint8_t bByteCount);
uint16_t EEPROM_Program(uint32_t wNVMTargetAddress, uint8_t *pData, uint16_t sizeBytes);
uint16_t EEPROM_EraseVerifySection(uint32_t wNVMTargetAddress, uint16_t uiByteCount);
uint16_t EEPROM_Program1Byte_NoWait(uint32_t wNVMTargetAddress, uint8_t bData);
uint16_t EEPROM_ProgramUpto4Bytes_NoWait(uint32_t wNVMTargetAddress, uint8_t *pbData, uint8_t bByteCount);
uint16_t EEPROM_EraseSector_NoWait(uint32_t wNVMTargetAddress);
uint16_t EEPROM_EraseSector(uint32_t wNVMTargetAddress);

uint16_t FLASH_Init(uint32_t u32BusClock);

/*! @} End of nvm_api_list                                              */
/********************************************************************/
#ifdef __cplusplus
}
#endif
#endif /* FLASH_H_ */
