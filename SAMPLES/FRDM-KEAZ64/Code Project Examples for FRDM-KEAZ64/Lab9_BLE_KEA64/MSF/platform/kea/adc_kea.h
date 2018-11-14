/****************************************************************************
 * @file     adc_kea.h
 * @brief    CMSIS-like ADC driver for kinetis kea
 * @version  1
 * @date     22. May 2014
 *
 * @note     The driver does not set the pins for analog input. This it default
 * 			function for the pin so this is in general not needed. If it is needed,
 * 			the user should set the appropriate pin.  
 * 			There are 2 sets of input pins: "a" and "b" which are selected by the
 * 			Control function ( MSF_ADC_ABSEL_x). Pins without the "a" or "b" in their
 * 			name can be used in both ADC configurations, but pins with "a" or "b" can
 * 			be used only if the appropriate pin set is selected.
 * 			Note that this "a", "b" channels sets are not related to the SC1n (SC1A, SC1B,...)
 * 			and Rn (RA, RB, ..) result registers - the registers represent more "instances"
 * 			of conversions.
 * 			The ADC goes to low power mode after conversion automatically. 
 * 	TODO: PowerControl
 * 	TODO: Control option to run calibration
 * 	TODO: interrupt on COCO mode
 * 	TODO: Continuous conversion mode
 *
 ******************************************************************************/
#ifndef MSF_ADC_KEA_H
#define MSF_ADC_KEA_H

/* Make sure the definitions of baudrate constants etc. are available */
#ifndef MSF_MSF_H
	#error Plese include msf.h before including this file. 
#endif

/* Common definitions for this platform and this driver class */
#include "drv_adc.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup group_adc  
 * @{ 
*/

/* ADC Run-time information*/
typedef struct _ADC_INFO {
  MSF_ADC_Event_t cb_event;          // Event Callback
  uint32_t      status;               // Status flags - not used yet.
  uint8_t       channel;        /* Current channel */     
} ADC_INFO;



/* The data for one instance of the driver - the "resource" */
/* Note: for K25 the UART0 is different than UART1 and UART2 which have different 
overlay struct. There will always be only 1 instance of the UART0;
there could be 1 or 2 instances of UART driver for UART1 and UART2. */
typedef struct {
        ADC_Type  *reg;  // ADC peripheral register interface, CMSIS        
        ADC_INFO   *info;   // Run-Time information
} const ADC_RESOURCES;


/**@}*/

#ifdef __cplusplus
}
#endif
/* ----------- end of file -------------- */
#endif /* MSF_ADC_KEA_H */
