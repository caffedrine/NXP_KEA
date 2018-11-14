/****************************************************************************
 * @file     waveio.c
 * @brief    Driver for measuring input and generating output signal
 * @version  1
 * @date     13. Feb. 2012
 *
 * @note
 *      
 ******************************************************************************/

/** @addtogroup waveio_driver
 * @{
 */


/* Include section
* Add all #includes here
*
***************************************************/
/* Include user configuration */
#include "msf_config.h"
/* Include hardware definitions */
#include "coredef.h"
/* Include any standard headers, such as string.h */

/* Include our main header*/           
#include "msf.h"
/* Include the header for this module */
#include "waveio.h"


/* Defines section
* Add all internally used #defines here
*
***************************************************/
//#define		WAVEIO_NUM_CHANNELS	(10)

/* Number of TPM drivers which can be used by waveio driver. */
//#define		WAVEIO_MAX_DRIVERS		(3)

/* Variables section
* Add all global variables here
*
***************************************************/
/*---------- Internal variables ---------------- */

/* The states for the output channel */
typedef enum wwaveio_channel_state
{
	waveout_idle,
	waveout_1st_half,	/* generating 1st half now */
	waveout_2nd_half,

	wavein_idle,
	wavein_1st_edge,	/* 1st edge WAS detected */
	wavein_2nd_edge,
	wavein_3rd_edge,

} waveio_channel_state;


//static MSF_DRIVER_TPM* waveout_drivers[WAVEIO_MAX_DRIVERS];
/* Status information for each of the TPM drivers */
//static uint8_t waveout_drv_status[WAVEIO_MAX_DRIVERS];


/* Structure with information about 1 channel */
typedef struct _waveio_channel_data
{
	uint32_t start;				/* time stamp for 1st detected edge (start of the period) */
	uint16_t half_wave[2];		/* half-wave 1 and 2 parts */
	waveio_channel_state status;	/* status of this channel/driver */

} waveio_channel_data;

static volatile waveio_channel_data gwaveio_data[WAVEIO_NUM_CHANNELS];

/* Mask of channels which were initialized (the TPM drivers the channels
 * belong to were initialized). */
static volatile uint32_t gwaveio_initialized_channels;

/* Mask of channels which are active (measure or generate signal).
 * bit 0 is channel 1; value 1 means measuring/generating now.
 * This is used in ISR to decide whether further processing on this
 * channel is needed.*/
static volatile uint32_t gwaveio_active_channels;

/* Mask of channels which are configured as inputs.
 * Input channel has 1 in its bit in the mask.
 * this does not need to be volatile; it is used only in the API functions*/
static volatile uint32_t gwaveio_input_channels;

/* Counters incremented when timer counter overflows.
 * ¨There is one for each TPM timer we use
 * It is used for timeout in waveio_in_pulse_wait() */
static volatile uint32_t gwaveio_overflow_cnt[WAVEIO_MAX_DRIVERS];

/* Function Prototype Section
* Add prototypes for all functions called by this
* module, with the exception of runtime routines.
*
***************************************************/   
/* -------- Prototypes of internal functions   -------- */
static void waveio_timer0_event(uint32_t event, uint32_t arg);
static void waveio_timer1_event(uint32_t event, uint32_t arg);
static void waveio_timer2_event(uint32_t event, uint32_t arg);
static void waveio_on_out_event(MSF_DRIVER_FTM* pdriver, uint8_t ftm_channel, uint8_t channel, uint16_t cntval);
static void waveio_on_in_event(MSF_DRIVER_FTM* pdriver, uint8_t ftm_channel, uint8_t channel, uint16_t cntval);
static MSF_DRIVER_FTM* waveio_get_ftm_driver(uint8_t channel, uint8_t* out_channel);

/* Code section 
* Add the code for this module.
*
***************************************************/  
/* -------- Implementation of public functions   -------- */

/*
 * initialize the waveout driver.
 *
 **/
void waveio_init(uint32_t channel_mask )
{
	int i;
	gwaveio_initialized_channels = 0;
	gwaveio_active_channels	= 0;
	gwaveio_input_channels = 0;
	for ( i=0; i<WAVEIO_MAX_DRIVERS; i++)
		gwaveio_overflow_cnt[i] = 0;

	// need to initialize FTM0?
	if ( channel_mask & WAVEIO_RANGE_0_1 )
	{
		Driver_FTM0.Initialize(waveio_timer0_event);
		// The timer clock speed depends on F_CPU, see MSF_FTM_CLKSEL in msf_mkea.h
		// and in waveio.h we set the WMSF_WAVEOUT_PRESCALER to obtain 1 MHz.
		Driver_FTM0.Control(MSF_FTM_PRESCALER_SET | MSF_FTM_TOF_SIGNAL, WMSF_WAVEIO_PRESCALER);

		gwaveio_initialized_channels |= WAVEIO_RANGE_0_1;
	}

	if ( channel_mask & WAVEIO_RANGE_2_3 )
	{
		Driver_FTM1.Initialize(waveio_timer1_event);
		Driver_FTM1.Control(MSF_FTM_PRESCALER_SET | MSF_FTM_TOF_SIGNAL, WMSF_WAVEIO_PRESCALER);
		gwaveio_initialized_channels |= WAVEIO_RANGE_2_3;
	}

	if ( channel_mask & WAVEIO_RANGE_4_9 )
	{
		Driver_FTM2.Initialize(waveio_timer2_event);
		Driver_FTM2.Control(MSF_FTM_PRESCALER_SET | MSF_FTM_TOF_SIGNAL, WMSF_WAVEIO_PRESCALER);
		gwaveio_initialized_channels |= WAVEIO_RANGE_4_9;
	}

}

/*
 * un-initialize the driver.
 *
 **/
void waveio_uninit(void)
{

	gwaveio_active_channels	= 0;
	gwaveio_input_channels = 0;

	// need to initialize FTM0?
	if ( gwaveio_initialized_channels & WAVEIO_RANGE_0_1 )
	{
		Driver_FTM0.Uninitialize();
	}

	if ( gwaveio_initialized_channels & WAVEIO_RANGE_2_3 )
	{
		Driver_FTM1.Uninitialize();
	}

	if ( gwaveio_initialized_channels & WAVEIO_RANGE_4_9 )
	{
		Driver_FTM2.Uninitialize();
	}

	gwaveio_initialized_channels = 0;
}

/*
 * Set given channel to input mode
 */
uint8_t waveio_in_attach(WAVEIO_channel iochannel)
{
	uint8_t ftm_channel;
	MSF_DRIVER_FTM* pdrv;
	uint8_t channel = (uint8_t)iochannel;

	pdrv = waveio_get_ftm_driver(channel, &ftm_channel);
	if ( !pdrv )
		return WAVEIO_ERROR_INVALID_CHANNEL;

	/* Check if the channel is initialized (the FTM driver it
	belongs to was initialized) */
	if ( !(gwaveio_initialized_channels & (1 << channel)) )
			return WAVEIO_ERROR_UNINITIALIZED_CHANNEL;

	/* Set the channel as input in our internal mask */
	gwaveio_input_channels |= (1 << channel);

	/* Set the timer channel to input capture mode; detect both rising and
	 falling edge */
	pdrv->ChannelSetMode(ftm_channel, InCapture_both_edges, MSF_FTM_PARAM_CHANNEL_EVENT);

	return WAVEIO_NO_ERROR;
}

/**
 * @note This function may do nothing. The pin can be configured
 * to e.g. GPIO mode without the need to disconnect it from the timer first.
 *
 **/
void waveio_in_detach(WAVEIO_channel iochannel)
{
	// do nothing
}

/*
 * Start generating output signal
 *
 **/
uint8_t waveio_out_start(WAVEIO_channel iochannel, uint16_t half1, uint16_t half2)
{
	uint8_t ftm_channel;
	MSF_DRIVER_FTM* pdrv;
	uint8_t channel = (uint8_t)iochannel;

	pdrv = waveio_get_ftm_driver(channel, &ftm_channel);
	if ( !pdrv )
		return WAVEIO_ERROR_INVALID_CHANNEL;

	// Check if the channel is initialized (the FTM driver it belongs to was initialized)
	if ( !(gwaveio_initialized_channels & (1 << channel)) )
			return WAVEIO_ERROR_UNINITIALIZED_CHANNEL;

	/* Clear the input flag for this channel: user can call this on a
	   channel which was previously used as input. This should cause no problem,
	   we configure the channel as output below; just mark our internal
	   variable that this channel is input.
	*/
	gwaveio_input_channels &= ~(1 << channel);


	// stop generating the signal before updating values
	gwaveio_active_channels &= ~(1 << channel);

	// save the requested values to our buffer for updates in ISR
	// note that we need to account for possible different speed of the timer for some
	// CPU clock options
	gwaveio_data[channel].half_wave[0] = (((uint32_t)half1 * WAVEIO_DIV_FACTOR)/WAVEIO_MULT_FACTOR);
	gwaveio_data[channel].half_wave[1] = (((uint32_t)half2 * WAVEIO_DIV_FACTOR)/WAVEIO_MULT_FACTOR);
	gwaveio_data[channel].status = waveout_idle;
	// The 1st interrupt will occur when the timer counter value == channel value (which is half1).
	// Then in the ISR we start generating the 1st part of the wave

	// We always configure the channel because this will force the pin low on 1st match
	// and also enable the channel event, which may be disabled after stop().
	// Note that setting the channel mode itself will not change the pin state!
	pdrv->ChannelSetMode(ftm_channel, OutCompare_toggle, MSF_FTM_PARAM_CHANNEL_EVENT);
	// write the first half to timer register - we can write anything actually, just need the
	// 1st interrupt to occur to start generating the signal
	pdrv->ChannelWrite(ftm_channel, gwaveio_data[channel].half_wave[0]);

	// enable updating the signal in timer interrupt
	gwaveio_active_channels |= (1 << channel);
	return WAVEIO_NO_ERROR;

}

/*
 *
 **/
uint8_t waveio_out_stop(WAVEIO_channel iochannel)
{
	uint8_t ftm_channel;
	MSF_DRIVER_FTM* drv;
	uint8_t channel = (uint8_t)iochannel;

	drv = waveio_get_ftm_driver(channel, &ftm_channel);
	if ( !drv )
		return WAVEIO_ERROR_INVALID_CHANNEL;

	// No need to check if the channel is initialized (the FTM driver it belongs to was initilized)
	// such cannel cannot be active and will not pass the next test.

	// if the channel is not active, do nothing
	if ( (gwaveio_active_channels & (1 << channel))  == 0 )
		return WAVEIO_ERROR_INACTIVE_CHANNEL;

	// Howto be sure the pin is low when we stop?
	// Version a) wait for the output pin to go low
	// Version b) disconnect and again connect the channel?
	// Version b) does not work - the disconnecting itself will not change the pin state.
	// Ver. a): waiting
	while (gwaveio_data[channel].status != waveout_2nd_half )
		;


	gwaveio_active_channels &= ~(1 << channel);
	// We disconnect the pin so that it is forced low after the 1st match when connected again.
	// The datasheet says "When a channel is initially configured to output compare mode,
	// the channel output updates with its negated value (logic 0 for set/toggle/pulse high..."
	// In fact the pin updates on the 1st compare match after connecting the pin and in our case,
	// toggle, the pin goes low.
	drv->ChannelSetMode(ftm_channel, Disabled, 0);

	// Ver. b) - re-connect the channel to force the output pin low
	// Does not work - the disconnecting itself will not change the pin state.
	//drv->ChannelSetMode(ftm_channel, OutCompare_toggle, 0);

	// Reset the data for this channel - not needed, but to be sure...
	gwaveio_data[channel].status = waveout_idle;
	gwaveio_data[channel].half_wave[0] = 0;
	gwaveio_data[channel].half_wave[1] = 0;

	return 0;
}

/*
 * Start generating signal on given channel for RC servo
 * with desired value in degrees (0 - 180)
 *
 */
uint8_t waveio_out_servo(WAVEIO_channel iochannel, uint8_t angle)
{
	uint16_t us;

	if (angle > 180)
		return MSF_ERROR_ARGUMENT;
	// convert the angle to microseconds:
	// 0 = 1000; 180 = 2000
	// us = 5.55 * angle + 1000
	// For integers:
	// us = (555 * angle)/100 + 1000
	us = (uint16_t)(((uint32_t)angle * 555) / 100 + 1000);
	return waveio_out_servo_us(iochannel, us);

}

/*
 * Start generating signal on given channel for RC servo
 * with desired value in microseconds (typically 1000 - 2000)
*/
uint8_t waveio_out_servo_us(WAVEIO_channel iochannel, uint16_t us)
{
	uint16_t space;

	if ( us > 19000 )
		return MSF_ERROR_ARGUMENT;

	space = 20000 - us;
	return waveio_out_start(iochannel, us, space);
}

/*
 * This starts updating the values for the given channel
 *
 **/
uint8_t waveio_in_start(WAVEIO_channel iochannel)
{
	uint8_t channel = (uint8_t)iochannel;
	if ( channel >= WAVEIO_NUM_CHANNELS)
		return WAVEIO_ERROR_INVALID_CHANNEL;


	// Check if the channel is initialized (the FTM driver it belongs to was initialized)
	if ( !(gwaveio_initialized_channels & (1 << channel)) )
		return WAVEIO_ERROR_UNINITIALIZED_CHANNEL;

	gwaveio_active_channels |= (1 << channel);
	return WAVEIO_NO_ERROR;
}

/*
 *
 **/
uint8_t waveio_in_stop(WAVEIO_channel iochannel)
{
	uint8_t channel = (uint8_t)iochannel;

	if ( channel >= WAVEIO_NUM_CHANNELS)
		return WAVEIO_ERROR_INVALID_CHANNEL;

	gwaveio_active_channels &= ~(1 << channel);
	// Reset the data for this channel
	gwaveio_data[channel].status = wavein_idle;
	gwaveio_data[channel].half_wave[0] = 0;
	gwaveio_data[channel].half_wave[1] = 0;
	return WAVEIO_NO_ERROR;

}

/*
 * Read the values captured for given channel
 *
 **/
uint8_t waveio_in_read(WAVEIO_channel iochannel, uint16_t* pulseA, uint16_t* pulseB  )
{
	uint8_t channel = (uint8_t)iochannel;

	*pulseB = 0;
	*pulseA = 0;

	// valid channel number?
	if ( channel >= WAVEIO_NUM_CHANNELS )
		return WAVEIO_ERROR_INVALID_CHANNEL;	// invalid channel

	// is the channel active (measuring)?
	if ( (gwaveio_active_channels & (1 << channel)) == 0 )
	{
		return WAVEIO_ERROR_INACTIVE_CHANNEL;
	}

	// have we detected any edges on this channel yet?
	if ( gwaveio_data[channel].status == wavein_idle )
		return WAVEIO_ERROR_NOINPUT;



	// deactivate the channel to be sure the read of the value is
	// not interfered with by new channel event (interrupt)
	gwaveio_active_channels &= ~(1 << channel);

	// Note: cannot use the status is this way:
	// if gwaveio_data[channel].status == wavein_2nd_edge
	// because with normal signal on input, we do not care that the 1st part
	// is for new period and the 2nd part from older period, this is better
	// than getting just one part of the wave many times because we did not
	// call this function in the right time to "catch" both the values for the
	// same period.
	if ( gwaveio_data[channel].half_wave[0] != 0)
	{
		*pulseA = gwaveio_data[channel].half_wave[0];
		// Reset the value we have just returned otherwise the user would get
		// still the same values even after the input is disconnected.
		gwaveio_data[channel].half_wave[0] = 0;
	}

	if ( gwaveio_data[channel].half_wave[1] != 0)
	{
		*pulseB = gwaveio_data[channel].half_wave[1];
		gwaveio_data[channel].half_wave[1] = 0;
	}

	gwaveio_data[channel].status = wavein_idle;

	// re-enable the channel
	gwaveio_active_channels |= (1 << channel);

	// If both the values are 0, we have no input
	if ( (*pulseA == 0) && (*pulseB == 0) )
		return WAVEIO_ERROR_NOINPUT;

	// Account for different clock than 1 MHz
	*pulseA = (uint16_t)(((uint32_t)*pulseA * WAVEIO_MULT_FACTOR)/WAVEIO_DIV_FACTOR);
	*pulseB = (uint16_t)(((uint32_t)*pulseB * WAVEIO_MULT_FACTOR)/WAVEIO_DIV_FACTOR);

	return WAVEIO_NO_ERROR;

}

/*
 * Wait for pulse on given input channel (pin)
 */
uint16_t waveio_in_pulse_wait(WAVEIO_channel iochannel, uint32_t timeout )
{
	uint32_t endtime;
	uint32_t curtime;
	//R200040 uint_fast8_t ftm_index = 0;
	uint32_t ftm_index = 0;	
	
	uint8_t channel = (uint8_t)iochannel;

	if ( timeout > 0 )
	{
		// obtain the current overflow count for one of the active timers
		if ( gwaveio_initialized_channels & WAVEIO_RANGE_0_1 )
			ftm_index = 0;
		else if ( gwaveio_initialized_channels & WAVEIO_RANGE_2_3 )
			ftm_index = 1;
		else
			ftm_index = 2;

		curtime = gwaveio_overflow_cnt[ftm_index];

		if ( timeout <= WAVEIO_OVERFLOW_UNITMS)
			endtime = curtime + 1;
		else
		{
			endtime = curtime + (timeout / WAVEIO_OVERFLOW_UNITMS) + 1;	// how many overflows to wait
			// add 1 so that, for example timeout 100 results in 2 overflows = 132 ms rather than
			// 1 overflow = 66 ms. (for WAVEIN_OVERFLOW_UNITMS = 66).
			if ( endtime < curtime )	// overflow of the gwavein_overflow_cnt
			{
				// just reset the counter of overflows
				gwaveio_overflow_cnt[ftm_index] = 0;
				endtime = (timeout / WAVEIO_OVERFLOW_UNITMS) + 1;
			}


		}
	}
	else
		endtime = 0;

	// disable the channel (synchronization with ISR)
	gwaveio_active_channels &= ~(1 << channel);
	// Reset the data for this channel
	gwaveio_data[channel].status = wavein_idle;
	gwaveio_data[channel].half_wave[0] = 0;
	gwaveio_data[channel].half_wave[1] = 0;
	// enable the channel
	gwaveio_active_channels |= (1 << channel);
	// wait for 2 edges
	while ( gwaveio_data[channel].status != wavein_2nd_edge )
	{
		if ( (endtime > 0) && (gwaveio_overflow_cnt[ftm_index] >= endtime) )
			return 0;	// timeout occurred
	}

	// Account for different clock than 1 MHz
	// using endtime for temporary storage!
	endtime = (((uint32_t)gwaveio_data[channel].half_wave[0] * WAVEIO_MULT_FACTOR)/WAVEIO_DIV_FACTOR);

	return (uint16_t)endtime;
}

/*
 * @return the length of the pulse in microseconds or 0 on error.
 * @note The function will handle checking the correct length of the pulse
 * and return only values between 500 and 2500 us. This is broader range than
 * typically needed. Typical RC signal is 1000 to 2000 us.
*/
uint16_t waveio_in_servo_read_us(WAVEIO_channel iochannel)
{
	uint16_t a, b, pulse;

	waveio_in_read(iochannel, &a, &b);

	if ( a != 0 && b != 0 )
	{
		// Note that we do not know which part of the wave is the
		// pulse and which is the "space". So take the shorter part...
		if ( a < b )
			pulse = a;
		else
			pulse = b;
		// Check if the pulse length is valid - we allow rather broad range.
		if ( pulse > 500 && pulse < 2500 )
		{
			return pulse;
		}
	}

	return 0;

}


/*
 * --------------------------------------------
 * Internal functions
 * --------------------------------------------
 */

/* Return pointer to the FTM driver for given channel
 * @return pointer to the FTM driver or null
 * */
static MSF_DRIVER_FTM* waveio_get_ftm_driver(uint8_t channel, uint8_t* out_channel)
{
	*out_channel = 0;

	if (channel >= 0 && channel <= 5 )
	{
#if (MSF_DRIVER_FTM0)
		*out_channel = channel;
		return &Driver_FTM0;
#endif
	}
	else if ( channel >= 6 && channel <= 7 )
	{
#if (MSF_DRIVER_FTM1)
		*out_channel = channel - 6;
		return  &Driver_FTM1;
#endif
	}
	else if ( channel >= 8 && channel <= 9 )
	{
#if (MSF_DRIVER_FTM2)
		*out_channel = channel - 8;
		return &Driver_FTM2;
#endif
	}

	return null;
}



/* The code is in #if because if the driver FTMx does not exists (user does not
 * need to use it, this would generate compile errors. */
#if (MSF_DRIVER_FTM0)
/* Handler for the timer low-level driver events */
static void waveio_timer0_event(uint32_t event, uint32_t arg)
{
	switch ( event )
	{
	// This event is signaled when timer FTM0 counter overflows
	case MSF_FTM_EVENT_TOF:
		gwaveio_overflow_cnt[0]++;
		break;

	case MSF_FTM_EVENT_CH0:
		if ( gwaveio_active_channels & (1 << 0) )
		{
			if ( gwaveio_input_channels & (1 << 0) )
				waveio_on_in_event(&Driver_FTM0, 0, 0, (uint16_t)arg);
			else
				waveio_on_out_event(&Driver_FTM0, 0, 0, (uint16_t)arg);
		}
		break;

	case MSF_FTM_EVENT_CH1:
		if ( gwaveio_active_channels & (1 << 1) )
		{
			if ( gwaveio_input_channels & (1 << 1) )
				waveio_on_in_event(&Driver_FTM0, 1, 1, (uint16_t)arg);
			else
				waveio_on_out_event(&Driver_FTM0, 1, 1, (uint16_t)arg);
		}
		break;

	case MSF_FTM_EVENT_CH2:
		if ( gwaveio_active_channels & (1 << 2) )
		{
			if ( gwaveio_input_channels & (1 << 2) )
				waveio_on_in_event(&Driver_FTM0, 2, 2, (uint16_t)arg);
			else
				waveio_on_out_event(&Driver_FTM0, 2, 2, (uint16_t)arg);
		}
		break;

	case MSF_FTM_EVENT_CH3:
		if ( gwaveio_active_channels & (1 << 3) )
		{
			if ( gwaveio_input_channels & (1 << 3) )
				waveio_on_in_event(&Driver_FTM0, 3, 3, (uint16_t)arg);
			else
				waveio_on_out_event(&Driver_FTM0, 3, 3, (uint16_t)arg);
		}
		break;

	case MSF_FTM_EVENT_CH4:
		if ( gwaveio_active_channels & (1 << 4) )
		{
			if ( gwaveio_input_channels & (1 << 4) )
				waveio_on_in_event(&Driver_FTM0, 4, 4, (uint16_t)arg);
			else
				waveio_on_out_event(&Driver_FTM0, 4, 4, (uint16_t)arg);
		}

		break;

	case MSF_FTM_EVENT_CH5:
		if ( gwaveio_active_channels & (1 << 5) )
		{
			if ( gwaveio_input_channels & (1 << 5) )
				waveio_on_in_event(&Driver_FTM0, 5, 5, (uint16_t)arg);
			else
				waveio_on_out_event(&Driver_FTM0, 5, 5, (uint16_t)arg);
		}

		break;

	}

}
#endif	/* MSF_DRIVER_FTM0 */

#if (MSF_DRIVER_FTM1)
static void waveio_timer1_event(uint32_t event, uint32_t arg)
{
	switch ( event )
	{
	case MSF_FTM_EVENT_TOF:
		gwaveio_overflow_cnt[1]++;
		break;

	case MSF_FTM_EVENT_CH0:
		if ( gwaveio_active_channels & (1 << 6) )
		{
			if ( gwaveio_input_channels & (1 << 6) )
				waveio_on_in_event(&Driver_FTM0, 0, 6, (uint16_t)arg);
			else
				waveio_on_out_event(&Driver_FTM1, 0, 6, (uint16_t)arg);
		}
		break;

	case MSF_FTM_EVENT_CH1:
		if ( gwaveio_active_channels & (1 << 7) )
		{
			if ( gwaveio_input_channels & (1 << 7) )
				waveio_on_in_event(&Driver_FTM0, 1, 7, (uint16_t)arg);
			else
				waveio_on_out_event(&Driver_FTM1, 1, 7, (uint16_t)arg);
		}
		break;
	}
}
#endif	/* MSF_DRIVER_FTM1 */

#if (MSF_DRIVER_FTM2)
static void waveio_timer2_event(uint32_t event, uint32_t arg)
{
	switch ( event )
	{
	case MSF_FTM_EVENT_TOF:
		gwaveio_overflow_cnt[2]++;
		break;

	case MSF_FTM_EVENT_CH0:
		if ( gwaveio_active_channels & (1 << 8) )
		{
			if ( gwaveio_input_channels & (1 << 8) )
				waveio_on_in_event(&Driver_FTM0, 0, 8, (uint16_t)arg);
			else
				waveio_on_out_event(&Driver_FTM2, 0, 8, (uint16_t)arg);
		}
		break;

	case MSF_FTM_EVENT_CH1:
		if ( gwaveio_active_channels & (1 << 9) )
		{
			if ( gwaveio_input_channels & (1 << 9) )
				waveio_on_in_event(&Driver_FTM0, 1, 9, (uint16_t)arg);
			else
				waveio_on_out_event(&Driver_FTM2, 1, 9, (uint16_t)arg);
		}
	}
}
#endif /* MSF_DRIVER_FTM2 */


/* Called from timer event handler to process the event.
 * @param ftm_channel the channel in the FTM driver
 * @param channel the channel in waveout driver
 * @param cntval current value of the timer counter.

 * */
static void waveio_on_out_event(MSF_DRIVER_FTM* pdriver, uint8_t ftm_channel, uint8_t channel, uint16_t cntval)
{
	uint16_t next_match;

	switch( gwaveio_data[channel].status )
	{
	case waveout_idle:
		// On the 1st compare match the pin is forced low, so we set the
		// status as if we were just in the 2nd pulse now, and the next match
		// will really start to generate the signal.
		next_match = cntval + gwaveio_data[channel].half_wave[1];
		pdriver->ChannelWrite(ftm_channel, next_match);
		gwaveio_data[channel].status = waveout_2nd_half;	// now waiting for next match
		break;

	case waveout_1st_half:	// match after 1st match, now half 2nd part of the wave
		// calculate the value when next match should occur
		next_match = cntval + gwaveio_data[channel].half_wave[1];
		pdriver->ChannelWrite(ftm_channel, next_match);
		gwaveio_data[channel].status = waveout_2nd_half;	// now generating 2st half
		break;

	case waveout_2nd_half:	// match after 2nd half; now start the 1st half again
		// calculate the value when next match should occur
		next_match = cntval + gwaveio_data[channel].half_wave[0];
		pdriver->ChannelWrite(ftm_channel, next_match);
		gwaveio_data[channel].status = waveout_1st_half;	// now generating 1st half
		break;
		default:
			break;

	}


}

/*
 * param pdriver FTM driver which handles the channel
 * param channel waveio channel (logical channel as known by waveio driver)
 * param ftm_channel number of the channel in the FTM driver
 * param cntval the value of the timer counter (timestamp for the edge on input) */
static void waveio_on_in_event(MSF_DRIVER_FTM* pdriver, uint8_t ftm_channel, uint8_t channel, uint16_t cntval)
{
	switch( gwaveio_data[channel].status )
	{
	case wavein_idle:	// 1st edge detected, just save the time stamp
		gwaveio_data[channel].start = cntval;
		gwaveio_data[channel].status = wavein_1st_edge;
		break;

	case wavein_1st_edge:	// 2nd edge detected, calculate the length of 1st wave part
		if ( gwaveio_data[channel].start > cntval )
			gwaveio_data[channel].half_wave[0] = 0xffff - gwaveio_data[channel].start + cntval;
		else
			gwaveio_data[channel].half_wave[0] = cntval - gwaveio_data[channel].start;

		gwaveio_data[channel].start = cntval;
		gwaveio_data[channel].status = wavein_2nd_edge;
		break;

	case wavein_2nd_edge:	// 3rd edge detected: it is the end of part 2 and start of next period part 1
		if ( gwaveio_data[channel].start > cntval )
			gwaveio_data[channel].half_wave[1] = 0xffff - gwaveio_data[channel].start + cntval;
		else
			gwaveio_data[channel].half_wave[1] = cntval - gwaveio_data[channel].start;

		gwaveio_data[channel].start = cntval;
		gwaveio_data[channel].status = wavein_1st_edge;
		break;
		default:
			break;
	}

}

/** @}*/ 
 
