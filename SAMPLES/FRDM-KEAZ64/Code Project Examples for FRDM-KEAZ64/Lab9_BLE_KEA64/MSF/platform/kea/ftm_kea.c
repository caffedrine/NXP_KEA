/****************************************************************************
 * @file     tpm_kl25.c
 * @brief    CMSIS-like driver for TPM timer in kinetis KL25
 * @version  1
 * @date     17. June 2014
 *
 * @note     Drivers: Driver_TPM0, Driver_TPM1, Driver_TPM2 
 *  see TPM_SetChannelMode comment for calculating the freq, duty etc. for various
 *  timer modes.
 *
 ******************************************************************************/
/** @addtogroup group_tpm  
 * @{ 
 */ 
/* MSF includes */
#include "msf_config.h"

/* Include the core definitions - CMSIS <device>.h file etc.*/
#include "coredef.h"
#include "msf.h"

#include "ftm_kea.h"


/* Define the resource for each FTM available on the MCU */
#if (MSF_DRIVER_FTM0)    
/* runtime info for FTM0 */
static FTM_INFO FTM0_Info;    

/** The pins for FTM0 
 * The pins are user-configurable through msf_config.h file.
 * Each is defined by pin-code (see msf_<device>.h) and the number of the alternate
 * function (ALTn) which is the timer channel function for this pin. 
 * IMPORTANT: there must always be 6 pins defined for each TMP module, even if the pin/channel is not used!
 * use {GPIO_INVALID_PIN, 0} for such pins/channels. 
 *  */
static FTM_PINS FTM0_Pins = {
		{	/* the array within the structure */
		{MSF_FTM0_CH0_PIN, MSF_FTM0_CH0_ALT},	/* pin for channel 0 */
		{MSF_FTM0_CH1_PIN, MSF_FTM0_CH1_ALT},	/* pin for channel 1 */		
		{GPIO_INVALID_PIN, 0},		/* pin for channel 2 */
		{GPIO_INVALID_PIN, 0},		/* pin for channel 3 */
		{GPIO_INVALID_PIN, 0},		/* pin for channel 4 */
		{GPIO_INVALID_PIN, 0},		/* pin for channel 5 */
		}
};
/* FTM0 Resources */
static FTM_RESOURCES FTM0_Resources = {
  FTM0,    /* ADC type object defined in CMSIS <device.h>*/
  &FTM0_Info,
  &FTM0_Pins,
  2,	/* number of channels */
};
#endif /* MSF_DRIVER_FTM0 */


#if (MSF_DRIVER_FTM1)    
/* runtime info for FTM1 */
static FTM_INFO FTM1_Info;    

/* The pins for FTM1 
 * IMPORTANT: there must always be 6 pins defined for each TMP module, even if the pin/channel is not used!
 * use {GPIO_INVALID_PIN, 0} for such pins/channels. 
 *  */
static FTM_PINS FTM1_Pins = {
		{	/* the array within the structure */
		{MSF_FTM1_CH0_PIN, MSF_FTM1_CH0_ALT},	/* pin for channel 0 */
		{MSF_FTM1_CH1_PIN, MSF_FTM1_CH1_ALT},	/* pin for channel 1 */
		{GPIO_INVALID_PIN, 0},		/* pin for channel 2 */
		{GPIO_INVALID_PIN, 0},		/* pin for channel 3 */
		{GPIO_INVALID_PIN, 0},		/* pin for channel 4 */
		{GPIO_INVALID_PIN, 0},		/* pin for channel 5 */
		}
};
/* FTM1 Resources */
static FTM_RESOURCES FTM1_Resources = {
  FTM1,    /* ADC type object defined in CMSIS <device.h>*/
  &FTM1_Info,
  &FTM1_Pins,
  2,
};
#endif /* MSF_DRIVER_FTM1 */

#if (MSF_DRIVER_FTM2)    
/* runtime info for FTM1 */
static FTM_INFO FTM2_Info;    

/* The pins for FTM1 
 * IMPORTANT: there must always be 6 pins defined for each TMP module, even if the pin/channel is not used!
 * use {GPIO_INVALID_PIN, 0} for such pins/channels. 
 *  */
static FTM_PINS FTM2_Pins = {
		{	/* the array within the structure */
		{MSF_FTM2_CH0_PIN, MSF_FTM2_CH0_ALT},	/* pin for channel 0 */
		{MSF_FTM2_CH1_PIN, MSF_FTM2_CH1_ALT},	/* pin for channel 1 */
		{MSF_FTM2_CH0_PIN, MSF_FTM2_CH0_ALT},	/* pin for channel 2 */
		{MSF_FTM2_CH1_PIN, MSF_FTM2_CH1_ALT},	/* pin for channel 3 */
		{MSF_FTM2_CH0_PIN, MSF_FTM2_CH0_ALT},	/* pin for channel 4 */
		{MSF_FTM2_CH1_PIN, MSF_FTM2_CH1_ALT},	/* pin for channel 5 */					
		}
};
/* FTM2 Resources */
static FTM_RESOURCES FTM2_Resources = {
  FTM2,    /* ADC type object defined in CMSIS <device.h>*/
  &FTM2_Info,
  &FTM2_Pins,
  6,
};
#endif /* MSF_DRIVER_FTM1 */



/* prototypes */
static uint32_t FTM_Control(uint32_t control, uint32_t arg, FTM_RESOURCES* ftm);

/* internal functions */
static void wftm_enable_pin(uint32_t channel, FTM_RESOURCES* ftm);
static void wftm_enable_int(uint32_t irqno, uint32_t enable);    

/* The driver API functions */

/**
  \brief       Initialize timer  
  \param[in]   event  Pointer to FTM_Event function or 0 (null)
  \param[in]   ftm The resources for the driver (driver instance)
  \return      error code (0 = OK)
  \note         
  	  Initializes FTM timer: timer clocked from internal clock source (see MSF_FTM_CLKSEL
  	  in msf_<device>.h for the values for given F_CPU); prescaler = 1, TOF not signaled.
*/
static uint32_t  FTM_Initialize( MSF_FTM_Event_t event,  FTM_RESOURCES* ftm)
{
	uint32_t i;
	
	/* init given timer */	
	ftm->info->cb_event = event;	/* store pointer to user callback*/
	ftm->info->status = 0;	
	
	/* Set the clock for timers FTMx (shared by all instances) */
	//SIM->SOPT2 |= SIM_SOPT2_FTMSRC(MSF_TPM_CLKSEL);
	ftm->reg->SC &=  ~FTM_SC_CLKS_MASK; 		// clear SC[CLKS]
	ftm->reg->SC |=  1 << FTM_SC_CLKS_SHIFT; 	// set FTM clock is TIMER_CLK
	
	/* note that the caller needs to enable the clock for the specific FTMn module before calling this!
	 * if the clock for FTMn is disabled, attempt to write its register will cause an exception!
	 * in SIM->SCGC */	
	
	ftm->reg->SC = 0;	/* default values, counter disabled */
	ftm->reg->MOD = 0x0000ffff;
	/* channel configuration registers - there are 6 in the <device>.h declaration of FTM_Type but we cannot
	 * access those not available, e.g. if FTM1 only has 2 channels, we can only write to CONSTROLS[0] and [1]
	 * Note: FTM1 and FTM2 have only 2 channels */
	for ( i=0; i<ftm->nchannels; i++ )
		ftm->reg->CONTROLS[i].CnSC = 0;
	//?ftm->reg->CONF = 0;
	
	/* Now set internal clock (prescaler = 0) */
	FTM_Control(MSF_FTM_CLOCK_INTERNAL, 0, ftm);

    return MSF_ERROR_OK;
}

#if (MSF_DRIVER_FTM0)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t FTM0_Initialize (MSF_FTM_Event_t pEvent) 
{
	SIM->SCGC |= SIM_SCGC_FTM0_MASK;	/* Enable clock for FTM0 BEFORE calling FTM_Initialize! */
	return FTM_Initialize(pEvent, &FTM0_Resources); 		
}
#endif

#if (MSF_DRIVER_FTM1)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t FTM1_Initialize (MSF_FTM_Event_t pEvent) 
{
	SIM->SCGC |= SIM_SCGC_FTM1_MASK;	/* Enable clock for FTM1 BEFORE calling FTM_Initialize! */
	return FTM_Initialize(pEvent, &FTM1_Resources); 	
}
#endif

#if (MSF_DRIVER_FTM2)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t FTM2_Initialize (MSF_FTM_Event_t pEvent) 
{
	SIM->SCGC |= SIM_SCGC_FTM2_MASK;	/* Enable clock for FTM1 BEFORE calling FTM_Initialize! */
	return FTM_Initialize(pEvent, &FTM2_Resources); 	
}
#endif


/**  
  \brief       Uninitialize timer 
  \param[in]   ftm The resources for the driver (driver instance) 
  \return      error code (0 = OK)
  \note        Common function called by instance-specific function.
*/
static uint32_t  FTM_Uninitialize( FTM_RESOURCES* ftm)
{
	uint32_t irq;
    /* Reset internal state for this instance of the FTM driver */
    ftm->info->cb_event = null;    
    /* if interrupt is enabled in NVIC, disable it */
    if ( (ftm->info->status & (MSF_FTM_STATUS_ANY_CHANNEL | MSF_FTM_STATUS_SIGNAL_TOF)) != 0 )
    {
    	irq = MSF_FTM_GETNVIC_IRQn(ftm->reg);
    	wftm_enable_int(irq, 0);
    }
    
    ftm->info->status = 0;
        
    return MSF_ERROR_OK;
}

#if (MSF_DRIVER_FTM0)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t FTM0_Uninitialize (void) 
{
  return FTM_Uninitialize(&FTM0_Resources);
}
#endif

#if (MSF_DRIVER_FTM1)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t FTM1_Uninitialize (void) 
{
  return FTM_Uninitialize(&FTM1_Resources);
}
#endif

#if (MSF_DRIVER_FTM2)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t FTM2_Uninitialize (void) 
{
  return FTM_Uninitialize(&FTM2_Resources);
}
#endif


/**
  \brief       Change power mode for the timer
  \param[in]   state  The requested power state
  \param[in]   ftm The resources for the driver (driver instance)
  \return      error code (0 = OK)
  \note        Currently does nothing!               
*/
static uint32_t FTM_PowerControl(MSF_power_state state, FTM_RESOURCES* ftm)
{
    return MSF_ERROR_NOTSUPPORTED;
} 

#if (MSF_DRIVER_FTM0)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t FTM0_PowerControl(MSF_power_state state) 
{
  return FTM_PowerControl(state, &FTM0_Resources);
}
#endif    /* MSF_DRIVER_FTM0 */

#if (MSF_DRIVER_FTM1)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t FTM1_PowerControl(MSF_power_state state) 
{
  return FTM_PowerControl(state, &FTM1_Resources);
}
#endif    /* MSF_DRIVER_FTM1 */

#if (MSF_DRIVER_FTM2)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t FTM2_PowerControl(MSF_power_state state) 
{
  return FTM_PowerControl(state, &FTM2_Resources);
}
#endif    /* MSF_DRIVER_FTM2 */

/**
  \brief       Control various options of the FTM
  \param[in]   control  Flags indicating which parameter(s) to set, see \ref group_ftm_control_flags.
  \param[in]   arg Optional argument for the command  
  \param[in]   ftm The resources for the driver (driver instance)
  \return      error code (0 = OK)
  \note        Common function called by instance-specific function.
            
*/
static uint32_t FTM_Control(uint32_t control, uint32_t arg, FTM_RESOURCES* ftm)
{	
	uint32_t val, i;
	
	ftm->reg->CNT = 0;	/* reset counter */
	
	/* Changing clock option: disabled/internal/external */
	if ( control & MSF_FTM_CLOCK_MODE_Mask )
	{
		val = ((control & MSF_FTM_CLOCK_MODE_Mask) >> MSF_FTM_CLOCK_MODE_Pos);
		ftm->reg->SC &= ~FTM_SC_CLKS_MASK;
		/* wait until the counter is really disabled (change is "acknowledged in the timer clock domain" :) )*/
		while (ftm->reg->SC & FTM_SC_CLKS_MASK ) ;
		ftm->reg->SC |= FTM_SC_CLKS(val-1);				
	}
	
	/* changing prescaler settings; arg is the value */
	if ( control & MSF_FTM_PRESCALER_SET && arg != 0)
	{		
		/* disable the counter before changing values */
		val = (ftm->reg->SC & FTM_SC_CLKS_MASK);	/* save current clock mode */
		ftm->reg->SC &= ~FTM_SC_CLKS_MASK;
		/* wait until the counter is really disabled (change is "acknowledged in the timer clock domain" :) )*/
		while (ftm->reg->SC & FTM_SC_CLKS_MASK ) ;
					
		ftm->reg->SC &= ~FTM_SC_PS_MASK;
		ftm->reg->SC |= FTM_SC_PS(arg);
		/*ftm->reg->CNT = 0;*/	/* reset counter */
		
		/* restore counter clock mode */
		ftm->reg->SC |= val;	
	}
	
	/* changing TOF interrupt (event) settings */
	if ( control & MSF_FTM_TOF_Mask )
	{
		i = MSF_FTM_GETNVIC_IRQn(ftm->reg);
		if ( i > 0 )
		{
			/* disable the counter before changing values */
			val = (ftm->reg->SC & FTM_SC_CLKS_MASK);	/* save current clock mode */
			ftm->reg->SC &= ~FTM_SC_CLKS_MASK;
			/* wait until the counter is really disabled (change is "acknowledged in the timer clock domain" :) )*/
			while (ftm->reg->SC & FTM_SC_CLKS_MASK ) ;
			
			
			/* clear possibly pending TOF interrupt */
			ftm->reg->SC |= FTM_SC_TOF_MASK;
			
			if ( control & MSF_FTM_TOF_SIGNAL )
			{				
				/* Configure NVIC */
				wftm_enable_int(i, 1 );
				
				ftm->reg->SC |= FTM_SC_TOIE_MASK;
				ftm->info->status |=  MSF_FTM_STATUS_SIGNAL_TOF;	/* remember we report TOF now */
			}
			else
			{
				ftm->info->status &=  ~MSF_FTM_STATUS_SIGNAL_TOF;	/* remember we do not report TOF now */
				ftm->reg->SC &= ~FTM_SC_TOIE_MASK; 				
				/* Disable the interrupt in NVIC */
				wftm_enable_int(i, 0 );				
			}
			
			/* restore counter clock mode */
			ftm->reg->SC |= val;
		}
	}
	
	/* Changing the modulo value? */
	if ( control & MSF_FTM_MOD_Mask )
	{
		/* disable the counter before changing values */
		val = (ftm->reg->SC & FTM_SC_CLKS_MASK);	/* save current clock mode */
		ftm->reg->SC &= ~FTM_SC_CLKS_MASK;
		/* wait until the counter is really disabled (change is "acknowledged in the timer clock domain" :) )*/
		while (ftm->reg->SC & FTM_SC_CLKS_MASK ) ;
				
		ftm->reg->CNT = 0;	/* recommended in datasheet */
		ftm->reg->MOD = (arg & 0x0000ffff);		
				
		/* restore counter clock mode */
		ftm->reg->SC |= val;	
	}
	
	
    return MSF_ERROR_OK;
}

#if (MSF_DRIVER_FTM0)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t FTM0_Control(uint32_t control, uint32_t arg) 
{
  return FTM_Control(control, arg, &FTM0_Resources);
}
#endif   /* MSF_DRIVER_FTM0 */

#if (MSF_DRIVER_FTM1)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t FTM1_Control(uint32_t control, uint32_t arg) 
{
  return FTM_Control(control, arg, &FTM1_Resources);
}
#endif   /* MSF_DRIVER_FTM1 */

#if (MSF_DRIVER_FTM2)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t FTM2_Control(uint32_t control, uint32_t arg) 
{
  return FTM_Control(control, arg, &FTM2_Resources);
}
#endif   /* MSF_DRIVER_FTM2 */

/**
  \brief       Set the channel to PWM, input capture or output compare mode
  \param[in]   channel The number of the channel to set (0-5) but depends on FTMn module!
  \param[in]   mode The mode to set. One of the enum values defined in drv_ftm.h!
  \param[in]   args Optional other options for the mode;
  \param[in]   ftm The resources for the driver (driver instance)
  \return      error code (0 = OK)
  \note   
  Information for the modes:
  Fsrc - frequency of the clock source for the timer (before prescaler) 
  Ft - frequency of the timer clock (after prescaling)
  Fo - frwquency of the output (PWM) signal
  MOD is the TOP value at which the counter resets. Set by Control with MSF_FTM_TOP_VALUE flag.  
  
  PWM edge aligned: 
   PWM frequency: Fo = Ft / (MOD+1)
   Duty for channel n: CnV 
   To calculate the MOD for required Fo: 
   MOD = Ft/Fo - 1 = Fsrc / (Fo . PRESCALER) - 1     
  
  PWM center-aligned:
   PWM frequency: Fo = Ft / 2*MOD
   Duty for channel n: 2xCnV
   MOD must be within 1 and 0x7fff (32767)!
   MOD = Ft/(2.Fo) = Fsrc / (2 . Fo . PRESCALER)   
  
  Possible masked values for the "args" parameter:
   MSF_FTM_PARAM_CHANNEL_EVENT - signal the channel event (interrupt)   
           
*/
static uint32_t FTM_ChannelSetMode(uint32_t channel, TMP_channel_mode_t mode, uint32_t args, FTM_RESOURCES* ftm)
{
	uint32_t val, i;
	 	
	/* Check if the channel is valid for given instance of FTM (FTM1 and 2 only have 2 channels) */	 
	if ( channel >= ftm->nchannels )
		return MSF_ERROR_ARGUMENT;
	
	/* disable the counter before changing values */
	val = (ftm->reg->SC & FTM_SC_CLKS_MASK);	/* save current clock mode */
	//ftm->reg->MODE |= FTM_MODE_WPDIS_MASK;      /* write protection disabled */
	
	ftm->reg->SC &= ~FTM_SC_CLKS_MASK;
	/* wait until the counter is really disabled (change is "acknowledged in the timer clock domain" :) )*/
	while (ftm->reg->SC & FTM_SC_CLKS_MASK ) ;
	
	ftm->reg->CNT = 0;	/* reset counter */
	
	
	while ( ftm->reg->CONTROLS[channel].CnSC != 0 )	/* wait for the change to be "acknowledged in the timer clock domain" */
	/* disable the channel which will be changed*/
	{
		ftm->reg->CONTROLS[channel].CnSC = 0;			
	}
		
	ftm->reg->CONTROLS[channel].CnV = 0;
	
	/* Set mode to up counting - this is valid for all modes except center-aligned PWM,
	 * which sets the mode below. */	
	ftm->reg->SC &=  ~FTM_SC_CPWMS_MASK;	
				
	switch(mode)
	{
	case Disabled:	/* The channel is disabled */
		/* the channel is disabled above... */
		break;
		
	case SWcompare:	/* no input/output pins connected */
		ftm->reg->CONTROLS[channel].CnSC |= FTM_CnSC_MSB_MASK;	
		break;

		
	case PWM_edge_hightrue:	/* PWM; set output at the start of the period */
		wftm_enable_pin(channel, ftm);	
		ftm->reg->CONTROLS[channel].CnSC |= (FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK);	/* edge aligned PWM... */
		break;
		
	case PWM_edge_lowtrue:	/* PWM; clear output at the start of the period */		
		wftm_enable_pin(channel, ftm);	
		ftm->reg->CONTROLS[channel].CnSC |= (FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK);	/* edge aligned PWM... */
		break;
		
	case PWM_center_hightrue:
		ftm->reg->SC |=  FTM_SC_CPWMS_MASK;	// Set mode to up-down counting
		wftm_enable_pin(channel, ftm);	
		ftm->reg->CONTROLS[channel].CnSC |= (FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK);		
		break;
		
	case PWM_center_lowtrue:
		ftm->reg->SC |=  FTM_SC_CPWMS_MASK;	// Set mode to up-down counting
		wftm_enable_pin(channel, ftm);
		ftm->reg->CONTROLS[channel].CnSC |= (FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK);	
		break;
			
	case InCapture_rising_edge:
		ftm->reg->CONTROLS[channel].CnSC |= FTM_CnSC_ELSA_MASK;
		wftm_enable_pin(channel, ftm);
		break;
	case InCapture_falling_edge:
		ftm->reg->CONTROLS[channel].CnSC |= FTM_CnSC_ELSB_MASK;
		wftm_enable_pin(channel, ftm);
		break;	
	case InCapture_both_edges:
		ftm->reg->CONTROLS[channel].CnSC |= (FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK);
		wftm_enable_pin(channel, ftm);
		break;
		
	case OutCompare_toggle:
		ftm->reg->CONTROLS[channel].CnSC |= (FTM_CnSC_ELSA_MASK | FTM_CnSC_MSA_MASK);
		wftm_enable_pin(channel, ftm);
		break;
	case OutCompare_clear:
		ftm->reg->CONTROLS[channel].CnSC |= (FTM_CnSC_ELSB_MASK | FTM_CnSC_MSA_MASK);
		wftm_enable_pin(channel, ftm);
		break;
	case OutCompare_set:
		ftm->reg->CONTROLS[channel].CnSC |= (FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK | FTM_CnSC_MSA_MASK);
		wftm_enable_pin(channel, ftm);
		break;
		
	case OutCompare_pulselow:
		ftm->reg->CONTROLS[channel].CnSC |= (FTM_CnSC_ELSB_MASK  | FTM_CnSC_MSA_MASK | FTM_CnSC_MSB_MASK);
		wftm_enable_pin(channel, ftm);
		break;	
		
	case OutCompare_pulsehigh:
		ftm->reg->CONTROLS[channel].CnSC |= (FTM_CnSC_ELSA_MASK  | FTM_CnSC_MSA_MASK | FTM_CnSC_MSB_MASK);
		wftm_enable_pin(channel, ftm);
		break;		
	
			
	}	/* end switch */
	
	i = MSF_FTM_GETNVIC_IRQn(ftm->reg);	/* IRQ number in NVIC */
	if ( i > 0 )
	{
		/* Enable/disable the event for this channel */
		ftm->reg->CONTROLS[channel].CnSC |= FTM_CnSC_CHF_MASK;	/* clear possible pending interrupt */
		if ( args & MSF_FTM_PARAM_CHANNEL_EVENT)
		{
			/* If the FTM interrupt is not enabled yet in NVIC, we need to enable it */
			if ( (ftm->info->status & (MSF_FTM_STATUS_ANY_CHANNEL | MSF_FTM_STATUS_SIGNAL_TOF)) == 0 )
			{
				wftm_enable_int(i, 1);
			}
	
			/* enable the event */
			ftm->reg->CONTROLS[channel].CnSC |= FTM_CnSC_CHIE_MASK;	/* enable channel interrupt */
			ftm->info->status |=  MSF_FTM_STATUS_SIGNAL_CHN(channel);	/* remember we report TOF now */	
		}
		else
		{
			/* disable the event */
			ftm->reg->CONTROLS[channel].CnSC &= ~FTM_CnSC_CHIE_MASK;	/* disable channel interrupt */
			ftm->info->status &=  ~MSF_FTM_STATUS_SIGNAL_CHN(channel);	/* remember we do not report TOF now */
			
			/* Disable the interrupt in NVIC if not needed anymore */
			if ( (ftm->info->status & (MSF_FTM_STATUS_ANY_CHANNEL | MSF_FTM_STATUS_SIGNAL_TOF)) == 0 )
			{
				wftm_enable_int(i, 0);
			}
		}
	}
		
	/* restore counter clock mode */
	ftm->reg->SC |= val;
	
	return MSF_ERROR_OK;
}

#if (MSF_DRIVER_FTM0)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t FTM0_ChannelSetMode(uint32_t channel, TMP_channel_mode_t mode, uint32_t args) 
{
  return FTM_ChannelSetMode(channel, mode, args, &FTM0_Resources);
}
#endif   /* MSF_DRIVER_FTM0 */

#if (MSF_DRIVER_FTM1)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t FTM1_ChannelSetMode(uint32_t channel, TMP_channel_mode_t mode, uint32_t args) 
{
  return FTM_ChannelSetMode(channel, mode, args, &FTM1_Resources);
}
#endif   /* MSF_DRIVER_FTM1 */

#if (MSF_DRIVER_FTM2)    
/* Instance specific function pointed-to from the driver access struct */
static uint32_t FTM2_ChannelSetMode(uint32_t channel, TMP_channel_mode_t mode, uint32_t args) 
{
  return FTM_ChannelSetMode(channel, mode, args, &FTM2_Resources);
}
#endif   /* MSF_DRIVER_FTM2 */

/**
  \brief       Write value to the channel register (e.g. duty in pwm mode)
  \param[in]   channel The number of the channel to set (0-5) but depends on FTMn module!
  \param[in]   value The value to write. Only 16-bit value is supported by FTM timers.
  \param[in]   ftm The resources for the driver (driver instance)
  \return      error code (0 = OK)
  \note                      
*/
static uint32_t FTM_ChannelWrite(uint32_t channel, uint16_t value, FTM_RESOURCES* ftm) 
{
	/* Check if the channel is valid for given instance of FTM (FTM1 and 2 only have 2 channels) */
	if ( channel >= ftm->nchannels )
		return MSF_ERROR_ARGUMENT;
	ftm->reg->CONTROLS[channel].CnV = value;
	return MSF_ERROR_OK;
}

#if (MSF_DRIVER_FTM0)   
static uint32_t FTM0_ChannelWrite(uint32_t channel, uint16_t value) 
{
	return FTM_ChannelWrite(channel, value,  &FTM0_Resources); 
}
#endif   /* MSF_DRIVER_FTM0 */

#if (MSF_DRIVER_FTM1)   
static uint32_t FTM1_ChannelWrite(uint32_t channel, uint16_t value) 
{
	return FTM_ChannelWrite(channel, value,  &FTM1_Resources); 
}
#endif   /* MSF_DRIVER_FTM0 */

#if (MSF_DRIVER_FTM2)   
static uint32_t FTM2_ChannelWrite(uint32_t channel, uint16_t value) 
{
	return FTM_ChannelWrite(channel, value,  &FTM2_Resources); 
}
#endif   /* MSF_DRIVER_FTM0 */

/**
  \brief       Read value from the channel register (used in input capture mode)
  \param[in]   channel The number of the channel to set (0-5) but depends on FTMn module!
  \param[in]   ftm The resources for the driver (driver instance) 
  \return      The 16-bit value from the CnV register or error code 0xffffffff.
  \note                      
*/
static uint32_t FTM_ChannelRead(uint32_t channel, FTM_RESOURCES* ftm) 
{
	if ( channel >= ftm->nchannels )
		return MSF_ERROR_MAXDWORD;
	return ftm->reg->CONTROLS[channel].CnV;	
}

#if (MSF_DRIVER_FTM0)   
static uint32_t FTM0_ChannelRead(uint32_t channel) 
{
	return FTM_ChannelRead(channel, &FTM0_Resources); 
}
#endif   /* MSF_DRIVER_FTM0 */

#if (MSF_DRIVER_FTM1)   
static uint32_t FTM1_ChannelRead(uint32_t channel) 
{
	return FTM_ChannelRead(channel, &FTM1_Resources); 
}
#endif   /* MSF_DRIVER_FTM1 */

#if (MSF_DRIVER_FTM2)   
static uint32_t FTM2_ChannelRead(uint32_t channel) 
{
	return FTM_ChannelRead(channel, &FTM2_Resources); 
}
#endif   /* MSF_DRIVER_FTM2 */


/** Interrupt handler for all TMPn instances */
void FTM_IRQHandler(FTM_RESOURCES* ftm)
{
	uint32_t i;
	
	if ( ftm->reg->SC & FTM_SC_TOF_MASK)
	{
		ftm->reg->SC |= FTM_SC_TOF_MASK;	/* clear TOF flag */
		if ( ftm->info->cb_event && (ftm->info->status & MSF_FTM_STATUS_SIGNAL_TOF) )
			ftm->info->cb_event(MSF_FTM_EVENT_TOF, 0);
	}
	
	/* should we report any channel interrupt? */
	if ( ftm->info->status & MSF_FTM_STATUS_ANY_CHANNEL )
	{
		for ( i=0; i<ftm->nchannels; i++)
		{
			if (ftm->reg->CONTROLS[i].CnSC & FTM_CnSC_CHF_MASK)
			{
				ftm->reg->CONTROLS[i].CnSC |= FTM_CnSC_CHF_MASK;	/* clear the interrupt flag */
				/* call user handler with the channel value in arg parameter */
				ftm->info->cb_event(MSF_FTM_EVENT_CHN(i), ftm->reg->CONTROLS[i].CnV);				
			}
		}
	}
}


/* Interrupt handler for FTM0 */
#if (MSF_DRIVER_FTM0) 
void FTM0_IRQHandler(void)
{
	FTM_IRQHandler(&FTM0_Resources);
}
#endif /* MSF_DRIVER_FTM1 */

#if (MSF_DRIVER_FTM1) 
void FTM1_IRQHandler(void)
{
	FTM_IRQHandler(&FTM1_Resources);
}
#endif /* MSF_DRIVER_FTM1 */

#if (MSF_DRIVER_FTM2) 
void FTM2_IRQHandler(void)
{
	FTM_IRQHandler(&FTM2_Resources);
}
#endif /* MSF_DRIVER_FTM2 */

/* Access structure for FTM0 */
#if (MSF_DRIVER_FTM0)
/* Only if the user-configuration in msf_config_<device>.h specifies that
 * it needs this driver we will create the instance */
MSF_DRIVER_FTM Driver_FTM0 = {
  FTM0_Initialize,
  FTM0_Uninitialize,
  FTM0_PowerControl,
  FTM0_Control,  
  FTM0_ChannelSetMode,
  FTM0_ChannelWrite,
  FTM0_ChannelRead,
  
};
#endif	/* MSF_DRIVER_FTM0 */

/* Access structure for FTM1 */
#if (MSF_DRIVER_FTM1)
/* Only if the user-configuration in msf_config_<device>.h specifies that
 * it needs this driver we will create the instance */
MSF_DRIVER_FTM Driver_FTM1 = {
  FTM1_Initialize,
  FTM1_Uninitialize,
  FTM1_PowerControl,
  FTM1_Control,  
  FTM1_ChannelSetMode,
  FTM1_ChannelWrite,
  FTM1_ChannelRead,
  
};
#endif	/* MSF_DRIVER_FTM1 */

/* Access structure for FTM2 */
#if (MSF_DRIVER_FTM2)
/* Only if the user-configuration in msf_config_<device>.h specifies that
 * it needs this driver we will create the instance */
MSF_DRIVER_FTM Driver_FTM2 = {
  FTM2_Initialize,
  FTM2_Uninitialize,
  FTM2_PowerControl,
  FTM2_Control,  
  FTM2_ChannelSetMode,
  FTM2_ChannelWrite,
  FTM2_ChannelRead,  
};
#endif	/* MSF_DRIVER_FTM2 */


/*========================  Internal functions ========================== */
/* Configure the pin for given channel of FTM to be used by the FTM (for PWM, input/output capture)*/
static void wftm_enable_pin(uint32_t channel, FTM_RESOURCES* ftm)
{
	/* Enable the FTMn function on given pin */
	if ( ftm->pins->pin[channel].pin_code != GPIO_INVALID_PIN)
	{
		//R200040 KEA has no PCR registers!!!		
		//GPIO_PORT_OBJECT(ftm->pins->pin[channel].pin_code)->PCR[GPIO_PIN_NUM(ftm->pins->pin[channel].pin_code)] = PORT_PCR_MUX(ftm->pins->pin[channel].alt_num);
		if (ftm->reg == FTM0)  // just 2 channels in FTM0
		{
			switch (channel)
			{
			case 0: // Set or clear FTM0PS0 flag in PINSEL0 
				SIM_PINSEL0 = ftm->pins->pin[channel].alt_num ? SIM_PINSEL0 | SIM_PINSEL_FTM0PS0_MASK : SIM_PINSEL0 & ~SIM_PINSEL_FTM0PS0_MASK;				 	
				break;
			case 1: // Set or clear FTM0PS1 flag in PINSEL0 
				SIM_PINSEL0 = ftm->pins->pin[channel].alt_num ? SIM_PINSEL0 | SIM_PINSEL_FTM0PS1_MASK : SIM_PINSEL0 & ~SIM_PINSEL_FTM0PS1_MASK;				 							
				break;
			}	
		}
		else if (ftm->reg == FTM1)
		{
			switch (channel)
			{
			case 0: // Set or clear FTM1PS0 flag in PINSEL0 
				SIM_PINSEL0 = ftm->pins->pin[channel].alt_num ? SIM_PINSEL0 | SIM_PINSEL_FTM1PS0_MASK : SIM_PINSEL0 & ~SIM_PINSEL_FTM1PS0_MASK;				 	
				break;
			case 1: // Set or clear FTM1PS1 flag in PINSEL0 
				SIM_PINSEL0 = ftm->pins->pin[channel].alt_num ? SIM_PINSEL0 | SIM_PINSEL_FTM1PS1_MASK : SIM_PINSEL0 & ~SIM_PINSEL_FTM1PS1_MASK;				 							
				break;
			}
		}
		else /* FTM2*/ 
		{
			switch (channel)
			{
			case 0: // Set or clear FTM2PS0 flag in PINSEL1 
				SIM_PINSEL1 &=  ~ SIM_PINSEL1_FTM2PS0_MASK ;			// clear fields
				SIM_PINSEL1 |= (ftm->pins->pin[channel].alt_num) << SIM_PINSEL1_FTM2PS0_SHIFT ; 	// set fields according to the alt_num 										 	
				break;
			case 1: // Set or clear FTM2PS1 flag in PINSEL1 
				SIM_PINSEL1 &=  ~ SIM_PINSEL1_FTM2PS1_MASK; 			// clear fields
				SIM_PINSEL1 |= (ftm->pins->pin[channel].alt_num) << SIM_PINSEL1_FTM2PS1_SHIFT; 	// set fields according to the alt_num 		 						
				break;
			case 2: // Set or clear FTM2PS2 flag in PINSEL1 
				SIM_PINSEL1 &=  ~ SIM_PINSEL1_FTM2PS2_MASK; 		// clear fields
				SIM_PINSEL1 |= (ftm->pins->pin[channel].alt_num) << SIM_PINSEL1_FTM2PS2_SHIFT; 	// set fields according to the alt_num 								 											
				break;
			case 3: // Set or clear FTM2PS3 flag in PINSEL1 
				SIM_PINSEL1 &=  ~ SIM_PINSEL1_FTM2PS3_MASK; 		// clear fields
				SIM_PINSEL1 |= (ftm->pins->pin[channel].alt_num) << SIM_PINSEL1_FTM2PS3_SHIFT; 	// set fields according to the alt_num 				
				break;
			case 4: // Set or clear FTM2PS4 flag in PINSEL1 
				SIM_PINSEL1 = ftm->pins->pin[channel].alt_num ? SIM_PINSEL1 | SIM_PINSEL1_FTM2PS4_MASK : SIM_PINSEL1 & ~SIM_PINSEL1_FTM2PS4_MASK;				 							
				break;
			case 5: // Set or clear FTM2PS5 flag in PINSEL1 
				SIM_PINSEL1 = ftm->pins->pin[channel].alt_num ? SIM_PINSEL1 | SIM_PINSEL1_FTM2PS5_MASK : SIM_PINSEL1 & ~SIM_PINSEL1_FTM2PS5_MASK;				 							
				break;
			}

		}

		
	}
}

/* Enable/disable interrupt for the timer in NVIC
 * @param  irqno number of the interrupt (e.g. FTM0_IRQn, see <device>.h CMSIS file)
 * @param enable If enable = 0 > disable interrupt; any other value > enable interrupt */
static void wftm_enable_int(uint32_t irqno, uint32_t enable)
{
	if ( enable )
	{
		NVIC_ClearPendingIRQ(irqno);	/* Clear possibly pending interrupt */
		NVIC_EnableIRQ(irqno);			/* and enable it */	
		/* Set priority for the interrupt; 0 is highest, 3 is lowest */
		NVIC_SetPriority(irqno, MSF_FTM_INT_PRIORITY);					
	}
	else
	{
		NVIC_DisableIRQ(irqno);	
	}
}
/**@}*/
/* ----------- end of file -------------- */
