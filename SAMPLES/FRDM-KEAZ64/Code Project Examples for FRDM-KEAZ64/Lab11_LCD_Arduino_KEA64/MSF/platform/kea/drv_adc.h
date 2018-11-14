/****************************************************************************
 * @file     drv_adc.h
 * @brief    CMSIS-like ADC driver for kinetis
 * @version  1
 * @date     22. May 2014
 *
 * @note     This is driver in CMSIS style, but CMSIS does not define ADC driver.
 *   Should be used as a standard for MSF drivers.    
 *   The manual recommend putting CPU into sleep during conversion for best accuracy
 *   but this driver does not support it. 
 *
 ******************************************************************************/
#ifndef MSF_DRIVER_ADC_H
#define MSF_DRIVER_ADC_H

/* Make sure the definitions of baudrate constants etc. are available */
#ifndef MSF_MSF_H
	#error Plese include msf.h before including this file. 
#endif


#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup group_adc ADC (Analog-to-digital converter) device driver 
 * @{ 
 * @brief CMSIS-like ADC driver for Kinetis  
 * @details    This is driver for the ADC created in style 
 * similar to ARM CMSIS. The CMSIS does not define ADC driver.
 * @note The manual recommend putting CPU into sleep during conversion for best accuracy
 *   but this driver does not support it.  
 * 
 * <b>Driver objects available in your program</b>
 * - Driver_ADC0
 * 
 * There is only one ADC (ADC0) on KL25Z. 
 */

/** Version of this drivers API */
#define     MSF_ADC_VERSION    (1)

/** Pointer to call back function for reporting events from the driver to
 * client application. Set in Initialize function */
typedef void (*MSF_ADC_Event_t) (uint32_t event);   

/* Flags (operations and parameters) for the Control function */
/* Positions and meaning of the bit-fields:
 Bit(s)  Meaning
 0:1	0=do not change settings; Single (1) or continuous (2) conversion mode
 2:3	0=do not change settings; Polled (1) or interrupt (2) mode
 4:6	0=do not change settings; Resolution 16-bit (4), 12-bit (2), 10-bit (3), 8-bit (1)
 7		Reserved 
 8:10	0=do not change settings; Average samples (1)= no averaging, (2)=4 samples; (3)=8 samples; (4)=16 samples, (5)=32 samples
 11:12	0=do not change settings; Select ADC A (1) or ADC B (2) channels .
 13		Reserved
 14:15	ADC reference: 0=do not change; (1)=default (VREFH, VREFL pins); (2)=Alternate pins (VALTH, VALTL).
  	
 */
/* Defines for these positions*/
#define		MSF_ADC_CONV_Pos	(0)
#define		MSF_ADC_CONV_Mask	(0x03)
#define		MSF_ADC_MODE_Pos	(2)
#define		MSF_ADC_MODE_Mask	(0xC)
#define		MSF_ADC_RES_Pos		(4)
#define		MSF_ADC_RES_Mask	(0x70)
#define		MSF_ADC_AVERAGE_Pos		(8)
#define		MSF_ADC_AVERAGE_Mask	(0x700)
#define		MSF_ADC_ABSEL_Pos		(11)
#define		MSF_ADC_ABSEL_Mask		(0x1800)
#define		MSF_ADC_REFSEL_Pos		(14)
#define		MSF_ADC_REFSEL_Mask		(0xC000)

/* Definitions of the flags */
/** @defgroup group_adc_control_flags Flags for the ADC driver Control function 
 @{*/
#define     MSF_ADC_CONV_SINGLE      (1UL << MSF_ADC_CONV_Pos)  /**< single-shot conversion */
#define     MSF_ADC_CONV_CONTINUOUS  (2UL << MSF_ADC_CONV_Pos)  /**< continuous conversions */
#define     MSF_ADC_MODE_POLLED      (1UL << MSF_ADC_MODE_Pos)  /**< no interrupt on conversion complete  */
#define     MSF_ADC_MODE_INT         (2UL << MSF_ADC_MODE_Pos)  /**< interrupt on conversion complete  */
#define     MSF_ADC_RES_12_BITS      (3UL << MSF_ADC_RES_Pos)  /**< default resolution 12-bits */
#define     MSF_ADC_RES_10_BITS      (2UL << MSF_ADC_RES_Pos)  /**< default resolution 10-bits */
#define     MSF_ADC_RES_8_BITS       (1UL << MSF_ADC_RES_Pos)  /**< default resolution 8-bits */
#define     MSF_ADC_AVERAGE_NONE    (1UL << MSF_ADC_AVERAGE_Pos)  /**< do not average ADC results, */
#define     MSF_ADC_AVERAGE_4      	(2UL << MSF_ADC_AVERAGE_Pos)  /**< average 4 samples */
#define     MSF_ADC_AVERAGE_8     	(3UL << MSF_ADC_AVERAGE_Pos)  /**< average 8 samples */
#define     MSF_ADC_AVERAGE_16     	(4UL << MSF_ADC_AVERAGE_Pos)  /**< average 16 samples */
#define     MSF_ADC_AVERAGE_32     	(5UL << MSF_ADC_AVERAGE_Pos)  /**< average 32 samples */
#define     MSF_ADC_ABSEL_A     	(1UL << MSF_ADC_ABSEL_Pos)    /**< Select ADC A */
#define     MSF_ADC_ABSEL_B     	(2UL << MSF_ADC_ABSEL_Pos)    /**< Select ADC B */
#define     MSF_ADC_REFSEL_DEFAULT  (1UL << MSF_ADC_REFSEL_Pos)    /**< Select VREF */
#define     MSF_ADC_REFSEL_ALT     	(2UL << MSF_ADC_REFSEL_Pos)    /**< Select VALT */
/**@}*/

/**
\brief Access structure of the ADC Driver.
*/
typedef struct _MSF_DRIVER_ADC {  
  uint32_t      (*Initialize)   (MSF_ADC_Event_t cb_event);  
  uint32_t      (*Uninitialize) (void);                       
  uint32_t      (*PowerControl) (MSF_power_state state); 
  uint32_t      (*Control)      (uint32_t control, uint32_t arg);
  uint32_t      (*Read)         (void);   /* channel selected by SetChannel */
  uint32_t      (*SetChannel)   (uint32_t channel);   
              
} const MSF_DRIVER_ADC;

/* Declare the instance(s) of the ADC driver(s). 
 * There is only one ADC (ADC0) on KL25Z
 * This is placed here rather than in the device-specific implementation
 * file (adc_k25.h) because then the client program would need to include 
 * different files for different derivatives... 
 * ... which is not good for generic drivers such as Conio. 
 */        
#if (MSF_DRIVER_ADC0)
	extern MSF_DRIVER_ADC Driver_ADC0;
#endif	/* MSF_DRIVER_ADC0 */

/**@}*/

#ifdef __cplusplus
}
#endif
/* ----------- end of file -------------- */
#endif /* MSF_DRIVER_ADC_H */
