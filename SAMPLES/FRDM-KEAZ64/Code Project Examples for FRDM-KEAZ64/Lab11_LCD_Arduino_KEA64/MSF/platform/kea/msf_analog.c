/****************************************************************************
 * @file     msf_analog.c
 * @brief    Analog read - MSF global functions 
 * @version  1
 * @date     28. May 2014
 *
 * @note Prototypes of these functions are in msf.h
 *      Uses msf ADC driver. The driver is initialized in msf_init() if
 *      proper option is specified in msf_config.h  
 *
 ******************************************************************************/

#include "msf_config.h"                    

/* Include hardware definitions */
#include "coredef.h"
              
#include "msf.h"
#include "drv_adc.h"  /* ADC driver */

/* Define which driver (instance of the ADC) is used by us */
#define	 MSF_ANALOG_DRIVER	Driver_ADC0


#if (MSF_USE_ANALOG)
    /* Read analog value from given analog pin */
 uint16_t msf_analog_read(Analog_pin_t apin)
 {
    /* future: use MSF_PIN2ADC() macro to obtain ADC module number (for MCUs 
    with more than 1 ADC module) and use the appropriate adc driver. */

	 /* Simple version which does not check if the channel is actually available */
#if defined(__FRDM_KEA__)
    /* Obtain ADC channel number from the Analog pin code*/
	 MSF_ANALOG_DRIVER.SetChannel(MSF_PIN2CHANNEL(apin)); 
    return MSF_ANALOG_DRIVER.Read();
#endif    
    

#if (MSF_DRIVER_ADC0) && defined(__FRDM_KL25Z__)    
    /* Version which sets the proper channel mux in the ADC driver as needed */
    if ( ((uint32_t)apin & MSF_ADC_BOTH) != MSF_ADC_BOTH )
    {
    	/* If the requested channel is not available in both A and B channels,
    	 * we have to select the A or B channels first. As the driver does not 
    	 * support querying its current configuration, we always have to set A or B */
    	if ( MSF_ADC_ISA_CHANNEL(apin) )
    		MSF_ANALOG_DRIVER.Control(MSF_ADC_ABSEL_A, 0);
    	else
    		MSF_ANALOG_DRIVER.Control(MSF_ADC_ABSEL_B, 0);    	    
    }
    
    MSF_ANALOG_DRIVER.SetChannel(MSF_PIN2CHANNEL(apin)); 	/* Select the channel */
    return MSF_ANALOG_DRIVER.Read();		/* Read the value from this channel */
#else
	//#warning	Analog input driver Driver_ADC0 is not enabled in msf_config.h; msf_analog_read will not work.
    return 0;
#endif
 }
 
#endif
