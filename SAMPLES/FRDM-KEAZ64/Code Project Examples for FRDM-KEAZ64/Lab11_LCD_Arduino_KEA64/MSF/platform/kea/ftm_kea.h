/****************************************************************************
 * @file     ftm_kea.h
 * @brief    CMSIS-like driver for FTM timer in kinetis KEA
 * @version  1
 * @date     17. June 2014
 *
 * @note     R200040 adjusted for Kinetis KEA
 *
 ******************************************************************************/
#ifndef MSF_FTM_KEA_H
#define MSF_FTM_KEA_H

/* Make sure basic definitions are available */
#ifndef MSF_MSF_H
	#error Plese include msf.h before including this file. 
#endif

/* Common definitions for this platform and this driver class */
#include "drv_ftm.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup group_tpm  
 * @{ 
*/ 

/* TPM Run-time information*/
typedef struct _FTM_INFO {
  MSF_FTM_Event_t cb_event;          // Event Callback
  uint32_t      status;               // Status flags 
       
} FTM_INFO;


/** TPM information about 1 pin for 1 channel 
 * Note that there may be mode than 1 pin available for one channel of the timer, for example,
 * for TPM0 channel 0 there are about 4 possible pins. But only one can be selected at a time.
 * To select the pin, enter the proper values in the tpm_kl25.c where the instance of the TPM_PINS
 * structure is defined for each timer module. */
typedef struct _FTM_PIN {
	MCU_pin_t	pin_code;	/* the pin-code defined in msf_<device>.h*/
	uint8_t		alt_num; /* the number of alt function for the given pin, e.g. 0 for ALT0, etc. */
} FTM_PIN;

/* TPM information about channel pins 
 * These are the pin codes of the pins used by the timer channels. There are
 * up to 6 channels in each timer, so we define 6 pins for each timer instance*/
typedef struct _FTM_PINS {
	FTM_PIN pin[6];
} FTM_PINS;



/* The data for one instance of the driver - the "resource" */
typedef struct {
	    FTM_Type  	*reg;  		/* TPM peripheral register interface, CMSIS */        
        FTM_INFO	*info;   	/* Run-Time information */
        FTM_PINS	*pins;		/* Pins used for our channels*/
        uint8_t		nchannels;	/* number of channels available in this module */
} const FTM_RESOURCES;


/** Obtain the NVIC interrupt number for given instance of the TPMn driver. 
 * tpm_adr is the address of the register overlay structure for the TPMn module. */
#define	MSF_FTM_GETNVIC_IRQn(ftm_adr)	((FTM_Type*)ftm_adr == FTM0) ? FTM0_IRQn : \
										((FTM_Type*)ftm_adr == FTM1) ? FTM1_IRQn : \
										((FTM_Type*)ftm_adr == FTM2) ? FTM2_IRQn : -1

/**@}*/

#ifdef __cplusplus
}
#endif
/* ----------- end of file -------------- */
#endif /* MSF_FTM_KEA_H */
