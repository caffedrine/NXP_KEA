#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "derivative.h"

/**!< !!!: The registers are maped only for SKEAZN642 */

/** SysTick - Peripheral register structure */
typedef struct SysTick_MemMap 
{
  uint32_t CSR;                                    /**< SysTick Control and Status Register, offset: 0x0 */
  uint32_t RVR;                                    /**< SysTick Reload Value Register, offset: 0x4 */
  uint32_t CVR;                                    /**< SysTick Current Value Register, offset: 0x8 */
  uint32_t CALIB;                                  /**< SysTick Calibration Value Register, offset: 0xC */
} volatile *SysTick_MemMapPtr;

/* SysTick - Register accessors */
#define SysTick_CSR_REG(base)                    ((base)->CSR)
#define SysTick_RVR_REG(base)                    ((base)->RVR)
#define SysTick_CVR_REG(base)                    ((base)->CVR)
#define SysTick_CALIB_REG(base)                  ((base)->CALIB)

/* CSR Bit Fields */
#define SysTick_CSR_ENABLE_MASK                  0x1u
#define SysTick_CSR_ENABLE_SHIFT                 0
#define SysTick_CSR_ENABLE_WIDTH                 1
#define SysTick_CSR_ENABLE(x)                    (((uint32_t)(((uint32_t)(x))<<SysTick_CSR_ENABLE_SHIFT))&SysTick_CSR_ENABLE_MASK)
#define SysTick_CSR_TICKINT_MASK                 0x2u
#define SysTick_CSR_TICKINT_SHIFT                1
#define SysTick_CSR_TICKINT_WIDTH                1
#define SysTick_CSR_TICKINT(x)                   (((uint32_t)(((uint32_t)(x))<<SysTick_CSR_TICKINT_SHIFT))&SysTick_CSR_TICKINT_MASK)
#define SysTick_CSR_CLKSOURCE_MASK               0x4u
#define SysTick_CSR_CLKSOURCE_SHIFT              2
#define SysTick_CSR_CLKSOURCE_WIDTH              1
#define SysTick_CSR_CLKSOURCE(x)                 (((uint32_t)(((uint32_t)(x))<<SysTick_CSR_CLKSOURCE_SHIFT))&SysTick_CSR_CLKSOURCE_MASK)
#define SysTick_CSR_COUNTFLAG_MASK               0x10000u
#define SysTick_CSR_COUNTFLAG_SHIFT              16
#define SysTick_CSR_COUNTFLAG_WIDTH              1
#define SysTick_CSR_COUNTFLAG(x)                 (((uint32_t)(((uint32_t)(x))<<SysTick_CSR_COUNTFLAG_SHIFT))&SysTick_CSR_COUNTFLAG_MASK)
/* RVR Bit Fields */
#define SysTick_RVR_RELOAD_MASK                  0xFFFFFFu
#define SysTick_RVR_RELOAD_SHIFT                 0
#define SysTick_RVR_RELOAD_WIDTH                 24
#define SysTick_RVR_RELOAD(x)                    (((uint32_t)(((uint32_t)(x))<<SysTick_RVR_RELOAD_SHIFT))&SysTick_RVR_RELOAD_MASK)
/* CVR Bit Fields */
#define SysTick_CVR_CURRENT_MASK                 0xFFFFFFu
#define SysTick_CVR_CURRENT_SHIFT                0
#define SysTick_CVR_CURRENT_WIDTH                24
#define SysTick_CVR_CURRENT(x)                   (((uint32_t)(((uint32_t)(x))<<SysTick_CVR_CURRENT_SHIFT))&SysTick_CVR_CURRENT_MASK)
/* CALIB Bit Fields */
#define SysTick_CALIB_TENMS_MASK                 0xFFFFFFu
#define SysTick_CALIB_TENMS_SHIFT                0
#define SysTick_CALIB_TENMS_WIDTH                24
#define SysTick_CALIB_TENMS(x)                   (((uint32_t)(((uint32_t)(x))<<SysTick_CALIB_TENMS_SHIFT))&SysTick_CALIB_TENMS_MASK)
#define SysTick_CALIB_SKEW_MASK                  0x40000000u
#define SysTick_CALIB_SKEW_SHIFT                 30
#define SysTick_CALIB_SKEW_WIDTH                 1
#define SysTick_CALIB_SKEW(x)                    (((uint32_t)(((uint32_t)(x))<<SysTick_CALIB_SKEW_SHIFT))&SysTick_CALIB_SKEW_MASK)
#define SysTick_CALIB_NOREF_MASK                 0x80000000u
#define SysTick_CALIB_NOREF_SHIFT                31
#define SysTick_CALIB_NOREF_WIDTH                1
#define SysTick_CALIB_NOREF(x)                   (((uint32_t)(((uint32_t)(x))<<SysTick_CALIB_NOREF_SHIFT))&SysTick_CALIB_NOREF_MASK)

/* SysTick - Peripheral instance base addresses */

/** Peripheral SysTick base pointer */
#define SysTick_BASE_PTR                         ((SysTick_MemMapPtr)0xE000E010u)

/** Array initializer of SysTick peripheral base pointers */
#define SysTick_BASE_PTRS                        { SysTick_BASE_PTR }

/* SysTick - Register instance definitions */
/* SysTick */
#define SYST_CSR                                 SysTick_CSR_REG(SysTick_BASE_PTR)
#define SYST_RVR                                 SysTick_RVR_REG(SysTick_BASE_PTR)
#define SYST_CVR                                 SysTick_CVR_REG(SysTick_BASE_PTR)
#define SYST_CALIB                               SysTick_CALIB_REG(SysTick_BASE_PTR)

/* Peripheral base address parameter */
#define SysTick_DEVICE SysTick_BASE_PTR

/////////////////////////////
// Functions and stuff
/////////////////////////////

void SysTick_Init(uint32_t reload);

#endif // __SYSTICK_H__
