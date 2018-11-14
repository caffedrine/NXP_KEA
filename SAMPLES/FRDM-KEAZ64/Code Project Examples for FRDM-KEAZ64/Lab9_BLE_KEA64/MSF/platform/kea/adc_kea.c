/****************************************************************************
 * @file     adc_kea.c
 * @brief    CMSIS-like ADC driver for kinetis kea
 * @version  1
 * @date     23. May 2014
 *
 * @note     Drivers: Driver_ADC0 
 *
 ******************************************************************************/
/* MSF includes */
#include "msf_config.h"

/* Include the core definitions - CMSIS <device>.h file etc.*/
#include "coredef.h"
#include "msf.h"

#include "adc_kea.h"

/** @addtogroup group_adc  
 * @{ 
*/

#if (MSF_DRIVER_ADC0)    
/* Define the resource for each ADC available on the MCU */
/* runtime info for ADC0 */
static ADC_INFO ADC0_Info;    

/* UART0 Resources */
static ADC_RESOURCES ADC0_Resources = {
  ADC,    /* ADC type object defined in CMSIS <device.h>*/
  &ADC0_Info
};

#endif /* MSF_DRIVER_ADC0 */


/* Prototypes */
static void adc_init(ADC_RESOURCES* adc);	  

        
/* The driver API functions */

/**
  \brief       Initialize ADC  
  \param[in]   event  Pointer to ADC_Event function or null
  \param[in]   adc       Pointer to ADC resources
  \return      error code (0 = OK)
  \note         Common function called by instance-specific function.
  	  Initializes ADC for single-conversion mode, busy-wait for conversion 
      completion, channel 0 selected.
*/
static uint32_t  ADC_Initialize( MSF_ADC_Event_t event,  ADC_RESOURCES* adc)
{
    adc->info->cb_event = event;	/* store pointer to user callback; not used for now */
    adc->info->channel = 0;
    
    /* Enable clock for ADC0 */
    /* Note: if modifying for MCUs with ADC1, need to update this code! */
    SIM->SCGC |= (SIM_SCGC_ADC_MASK );	
      
    /* init the given ADC */
    adc_init(adc);
    
    return MSF_ERROR_OK;
}

#if (MSF_DRIVER_ADC0)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t ADC0_Initialize (MSF_ADC_Event_t pEvent) 
{
  return ADC_Initialize(pEvent, &ADC0_Resources);
}
#endif

/* example: function for another instance of the UART
static int32_t UART1_Initialize (UART_Event_t pEvent) {
  return UART_Initialize(pEvent, &UART1_Resources);
}
}
*/

/**  
  \brief       Uninitialize ADC 
  \param[in]   adc    Pointer to adc resources 
  \return      error code (0 = OK)
  \note        Common function called by instance-specific function.
*/
static uint32_t  ADC_Uninitialize( ADC_RESOURCES* adc)
{
    /* TODO: reset internal state for this instance of the ADC */
    adc->info->cb_event = null;
    adc->info->channel = 0;
    return MSF_ERROR_OK;
}

#if (MSF_DRIVER_ADC0)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t ADC0_Uninitialize (void) 
{
  return ADC_Uninitialize(&ADC0_Resources);
}
#endif


/**
  \brief       Initialize ADC Interface.
  \param[in]   state  The requested power state
  \param[in]   adc    Pointer to adc resources
  \return      error code (0 = OK)
  \note        Currently does nothing!
               Note that the ADC automatically enters low power mode after conversion, see datasheet.
*/
static uint32_t ADC_PowerControl(MSF_power_state state, ADC_RESOURCES* adc)
{
    return MSF_ERROR_NOTSUPPORTED;
} 

#if (MSF_DRIVER_ADC0)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t ADC0_PowerControl(MSF_power_state state) 
{
  return ADC_PowerControl(state, &ADC0_Resources);
}
#endif

/**
  \brief       Control various options of the ADC
  \param[in]   control  Flags indicating which parameter(s) to set, see \ref group_adc_control_flags
  \param[in]   arg Optional argument for the command
  \param[in]   adc    Pointer to ADC resources
  \return      error code (0 = OK)
  \note        Common function called by instance-specific function.
            
*/
static uint32_t ADC_Control(uint32_t control, uint32_t arg, ADC_RESOURCES* adc)
{
	uint32_t tmp;
	
	/* Disable the ADC before changing parameters by writing all 1s to channel
	 * It will be re-enabled when Read is called.
	 */
	adc->reg->SC1 =  ADC_SC1_ADCH(AIN_ADC_DISALED);
		
	/* Single or continuous conversion  */
	if ( (control & MSF_ADC_CONV_Mask) != 0 )
	{
		if ( (control & MSF_ADC_CONV_Mask) == MSF_ADC_CONV_SINGLE )
		{
			/* single-shot mode */
			adc->reg->SC1 &= ~ADC_SC1_ADCO_MASK;		
		}
		else
		{	
			/* continuous mode */
			adc->reg->SC1 |= ADC_SC1_ADCO_MASK;
		}
	}
	
	/* Polled or interrupt mode */
	/* TODO: implement */
	
	
	/* Change resolution */	
	if ( (control & MSF_ADC_RES_Mask) != 0 )
	{
		tmp = ((control & MSF_ADC_RES_Mask) >> MSF_ADC_RES_Pos) - 1;	/* the value which means resolution */
		adc->reg->SC3 &= ~ADC_SC3_MODE_MASK;	/* clear the bits in register */
		adc->reg->SC3 = ADC_SC3_MODE(tmp);	/* now set the new resolution */
	}
	
	
	/* Set averaging option? */
	/* N/A on KEA */
#if 0 
	if ( (control & MSF_ADC_AVERAGE_Mask) != 0)
	{
		if ( (control & MSF_ADC_AVERAGE_Mask) != MSF_ADC_AVERAGE_NONE )
		{
			/* the bit field in control contains the value for register + 2 */
			tmp = ((control & MSF_ADC_AVERAGE_Mask) >> MSF_ADC_AVERAGE_Pos) - 2;
			/* Clear the current AVG value */
			adc->reg->SC3 &= ~ADC_SC3_AVGS_MASK;
			/* write new value and enable averaging */
			adc->reg->SC3 |= ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(tmp);
		}
		else
		{
			/* disable averaging */ 
			adc->reg->SC3 &= ~ADC_SC3_AVGE_MASK;
		}
	}
	
	/* Select ADC A or ADC B channels . 
	 * Some AIN pins are available only for A and some only for B. */
	if ( (control & MSF_ADC_ABSEL_Mask) != 0)
	{
		if ( (control & MSF_ADC_ABSEL_Mask) == MSF_ADC_ABSEL_A )
		{
			/* select ADC A */
			adc->reg->CFG2 &= ~ADC_CFG2_MUXSEL_MASK;
		}
		else
		{
			/* select ADC B */
			adc->reg->CFG2 |= ADC_CFG2_MUXSEL_MASK;
		}
	}	
#endif
	
	/* Select ADC reference 
	 * KL25Z uses VREFH (external pin) as default reference
	 * Alternate reference Valt is VDDA (analog power supply)*/
	if ( (control & MSF_ADC_REFSEL_Mask) != 0)
	{
		if ( (control & MSF_ADC_REFSEL_Mask) == MSF_ADC_REFSEL_DEFAULT )
		{
			/* select VREFH + VREFL pins */
			adc->reg->SC2 &= ~ADC_SC2_REFSEL_MASK;
		}
		else
		{
			/* select VALTH + VALTL pins */
			adc->reg->SC2 |= ADC_SC2_REFSEL(1);
		}
	}	
	
    return MSF_ERROR_OK;
}

#if (MSF_DRIVER_ADC0)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t ADC0_Control(uint32_t control, uint32_t arg) 
{
  return ADC_Control(control, arg, &ADC0_Resources);
}
#endif


/**
  \brief       Read data from ADC; from current channel.  
  \param[in]   adc    Pointer to ADC resources
  \return      The value from ADC converter or 0xFFFFFFFF on error.
  \note        Common function called by instance-specific function.
            Only 16-bits in the return value or less are used depending on ADC configuration
*/
static uint32_t ADC_Read(ADC_RESOURCES* adc)
{	
	uint32_t tmp;
	/* Write the channel to SC1A (SC1[0] register to start a conversion 
	 * Note that SC1B cannot be used for SW triggered operation and write to 
	 * SC1B does not start new conversion! */
	if ( adc->info->channel < 31 )
	{
		tmp = adc->reg->SC1;
		tmp &= ~(ADC_SC1_ADCH_MASK << ADC_SC1_ADCH_SHIFT);	// set channel to 0, keep other values
		tmp |= ADC_SC1_ADCH(adc->info->channel);	// select the channel
		adc->reg->SC1 =  tmp;
		
		/* wait for conversion to complete */
		while ( !WMSF_ADC_COMPLETE(adc->reg) ) ; 
		/* return the result */
		return adc->reg->R;	
	}
	
	/* Invalid channel (should never happen) */
	return MSF_ERROR_MAXDWORD;	
} 

#if (MSF_DRIVER_ADC0)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t ADC0_Read(void) 
{
  return ADC_Read(&ADC0_Resources);
}
#endif

/**  
  \brief       Select the channel used for conversions by ADC_Read
  \param[in]   channel Number of the chanel to select. 
  \param[in]   adc    Pointer to ADC resources 
  \return      Error code; 0 = OK
  \note        Common function called by instance-specific function.
  	  	  	  The channel number must be the direct value which will be used in the ADC register.
  	  	  	  The caller must make sure the channel is valid, see enum in msf_<device>.h for 
  	  	  	  valid channel definitions.
*/
static uint32_t  ADC_SetChannel( uint32_t channel, ADC_RESOURCES* adc)
{     
	/* We check if the channel is valid but we do not check if the channel really exist..
	 * Not all of the 0 thru 30 channels are really available on the KL25Z. */
	/* The driver does not set the pins for analog input. This it default
  		function for the pin so this is in general not needed. If it is needed,
  		the user should set the appropriate pin.   */
	if ( channel < 31 )
	{
		/* just store the channel into our resources; */
		adc->info->channel = (uint8_t)channel;
		//R200040
		adc->reg->APCTL1 |= ADC_APCTL1_ADPC(channel);
		return MSF_ERROR_OK; 
	}
	
    return MSF_ERROR_ARGUMENT;
}

#if (MSF_DRIVER_ADC0)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t ADC0_SetChannel(uint32_t channel) 
{
  return ADC_SetChannel(channel, &ADC0_Resources);
}	
#endif


/* Access structure for ADC0 */
#if (MSF_DRIVER_ADC0)
/* Only if the user-configuration in msf_config_<device>.h specifies that
 * it needs this driver we will create the instance */
MSF_DRIVER_ADC Driver_ADC0 = {
  ADC0_Initialize,
  ADC0_Uninitialize,
  ADC0_PowerControl,
  ADC0_Control,  
  ADC0_Read,
  ADC0_SetChannel,    
};

#endif	/* MSF_DRIVER_ADC0 */

/*========================  Internal functions ========================== */
static void adc_init(ADC_RESOURCES* adc)
{	
	
	/* Disable interrupt and select channel 31 = ADC disabled, select single-ended mode */
	adc->reg->SC1 =  ADC_SC1_ADCH(AIN_ADC_DISALED);
	/*adc->reg->SC1 |= (ADC_SC1_AIEN_MASK << ADC_SC1_AIEN_SHIFT); */ /* enable interrupt */
	
	/* Select ADC clock, prescaler and resolution, short sample time  */
	adc->reg->SC3 = ADC_SC3_ADICLK(WMSF_ADC_CLOCK) | ADC_SC3_ADIV(WMSF_ADC_PRESCALER) 
			| ADC_SC3_MODE(WMSF_ADC_RESOLUTION);
	
	/* Select reference, triggering, compare mode */
	adc->reg->SC2 = 0;	/* default values: sw trigger, default reference=VREFH,VREFL pins, ... */
	
}
  
    	
/**@}*/


/* ----------- end of file -------------- */
