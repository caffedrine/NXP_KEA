/****************************************************************************
 * @file     waveio.h
 * @brief    Driver for measuring input signal and generating output signal
 * @version  1
 * @date     13. Feb. 2015
 *
 * @note        
 *
 ******************************************************************************/
#ifndef MSF_WAVEIO_DRV_H
    #define MSF_WAVEIO_DRV_H

/** @defgroup waveio_driver waveio Driver
 * @{
 * @brief High-level driver for measuring input and generating output signals
 * @details
 * The driver has several output channels. You have to specify which channel(s)
 * you will use in your application in call to waveio_init, so that this
 * driver can initialize the underlying timer (FTM) driver(s).
 * The MCU pins corresponding to each channel depends on configuration of
 * the underlying timer driver(s), see msf_config.h.
 * Map of this driver's channels to FTM timer channels:
 * waveout channel  timer channel   default pin (msf_config.h)
 * <b>FTM0:</b><br>
 * WAVEIO_C0			FTM0 channel 0	A0  (Arduino 3)<br>
 * WAVEIO_C1			FTM0 channel 1	A1	(n/a)<br>
 * 
 * <b>FTM1:</b><br>
 * WAVEIO_C2			FTM1 channel 0	C4	(n/a)<br>
 * WAVEIO_C3			FTM1 channel 1	C5	(Arduino 8)<br>
 *
 ** <b>FTM2:</b><br>
 * WAVEIO_C4			FTM2 channel 0	C0	(Arduino 16)<br>
 * WAVEIO_C5			FTM2 channel 1	C1	(Arduino 17)<br>
 * WAVEIO_C6			FTM2 channel 2  C2	(n/a)<br>
 * WAVEIO_C7			FTM2 channel 3  C3	(Arduino 5)<br>
 * WAVEIO_C8			FTM2 channel 4  B4	(Arduino 6)<br>
 * WAVEIO_C9			FTM2 channel 5  B6	(n/a)<br>
 *
 * You can use simple channel numbers 0 thru 9, but for future compatibility it
 * is recommended to use the channel names as seen in the table above, for example,
 * WAVEIO_C0. These are defined in msf_mkea.h.<br>
 * <br>
 * <b>Howto use the driver</b><br>
 * To generate output:<br>
 * 1) Initialize: waveio_init(range of used inputs);<br>
 * 2) Start generating on given channel(s): waveio_out_start(channel, half-wave1, halfwave2);<br>
 * You specify the lengths of the half-waves in microseconds.<br>
 * <br>
 * To measure input signal<br>
 * 1) Initialize: waveio_init(range of used inputs);<br>
 * 2) Attach channel(s) you want to measure: waveio_in_attach(channel);<br>
 * 3) Read the values of the half-waves detected on the input(s): <br>
 * waveio_in_read(channel, half-wave1, half-vave2);<br>
 * OR<br>
 * Wait for a pulse on the input: waveio_in_pulse_wait(channel, timeout);<br>
 * You specify the lengths of the half-waves in microseconds.<br>

 *
 * <b>Useful equations</b><br>
 * Frequency_in_kHz = 1 000 / period_in_us<br>
 * Period_in_us = 1 000 / frequency_in_kHz<br><br>
 *
 * <b>Important limits</b><br>
 * Maximum frequency of generated signal if only 1 channel is used for 48 MHz CPU clock:<br>
 * Fmax = 35 kHz for channels 0 thru 5 (TPM0). <br>
 * Fmax = 45 kHz for channels 6 thru 9 (TPM1 and TPM2)<br>
 * If more channels are used, the maximum frequency is proportionally lower.<br>
 * For example, if 2 channels are used, the Fmax is half of the Fmax for single channel.
 * If 3 channels are used, Fmax is 1/3rd, etc.<br>
 * If lower CPU clock is used, the values are proportionally lower.<br><br>
 * <b>Notes</b><br>
 * If multiple channels are used, it seems that the real Fmax can be little higher than
 * Fmax for single channel / number of channels.<br>
 * For example when using 5 channels, 0 thru 4, at the same frequency, the maximum frequency
 * obtained was about 10 kHz while theoretically it should be only 7 kHz (35/5).
 *
 * The lower Fmax for TPM0 is due to more channels available in TPM0 (6) which requires
 * more processing time in the ISR.<br>
 *
 * For generating the output signal, waveio driver uses the timer in output compare mode
 * with pin toggle on compare match.
 * It would be possible to achieve higher maximum frequencies using PWM mode of the timer,
 * but the PWM mode requires that the period of the signal is tied to the value at
 * which the timer counter overflows. So all the channels of one physical
 * timer (TPM driver) have to have the same frequency.
 * <br>
 * Longest half-wave length is:<br>
 *	65535 for the CPU clock options which allow obtaining 1 MHz timer clock.
 *	About 48500 for the other CPU clock options.
 *	See the clock conditions in waveio.h (this file). If the WAVEIO_MULT_FACTOR is not 1,
 *	the clock for timer is higher than 1 MHz and the longest half-vave value is
 *	lower than 65535. This is because the timer registers are 16 bits and the value must
 *	fit into the registers even after adjusting it from us to actual counter "ticks".
 * <br>
 * Discussion about the performance<br>
 *
 * Limit by the resolution of the timer:<br>
 * We enter the lengths of the half-waves in timer counter units, which are microseconds.
 * It probably makes little sense to use half-wave lengths smaller than say, 2. Lets assume
 * the shortest period of the signal is 5 us which means the frequency is 200 kHz, BUT this
 * is not reachable due to CPU performance. So the timer resolution is not limiting.
 * <br>
 * Limit by CPU computing power:<br>
 * Generating the signal requires 2 interrupts per period for each channel.
 * Assuming the ISR takes about 500 CPU cycles to execute (just an estimate), if the
 * CPU runs at 1 MHz, it can handle 2000 ISRs per second - leaving no time for the main program to execute.
 * We need 2 ISR executions per period of the signal, so we can obtain 1 KHz output for each MHz
 * of CPU speed. <br>
 * For the max. CPU clock in KL25Z, 48 MHz, the max frequency we can generate is about 48 kHz.<br>
 * BUT THERE IS ALSO ANOTHER LIMIT:<br>
 * The ISR must execute in shorter time than it takes for the timer counter to reach
 * the new value (new output pin toggle). For example, if the requested half-wave lengths are 50 and 50,
 * the ISR needs to execute in less than 50 us.
 * Assuming ISR needs 500 CPU clocks, CPU running at 1 MHz will execute the ISR in 500 us.
 * For 48 MHz clock the ISR will execute in about 11 us, so the lowest half-wave value is 11.
 * <br>
 * In general, to obtain shortest possible half-wave for 1 channel:<br>
 * half_wave_MIN = ISR_clocks/CPU_clock_MHz  [result in us]<br>
 * Experimental results show that the ISR takes in about 500 clock cycles, so <br>
 * half_wave_MIN = 500/CPU_clock_MHz. <br>
 * Thus the shortest pulse we can generate with the maximum CPU speed 48 MHz is about 11 us.
 * <br>
 *
 *
 */
 
/* Platform-specific timer driver */
#include "drv_ftm.h"


#ifdef __cplusplus
extern "C" {
#endif


/* Defines section
* Add all public #defines here
*
***************************************************/

/** @defgroup group_waveio_error_codes Error codes for the waveout driver
 @{*/
#define	WAVEIO_NO_ERROR						MSF_ERROR_OK		/**< no error, success. */
#define	WAVEIO_ERROR_INVALID_CHANNEL		(MSF_ERROR_LAST+1)	/**< given channel number is not valid (too high) */
#define	WAVEIO_ERROR_NOINPUT				(MSF_ERROR_LAST+2)	/**< no input on this channel detected so far */
#define	WAVEIO_ERROR_UNINITIALIZED_CHANNEL	(MSF_ERROR_LAST+3)	/**< given channel number was not initialized in call to waveio_init() */
#define	WAVEIO_ERROR_INACTIVE_CHANNEL		(MSF_ERROR_LAST+4)	/**< given channel number is not active (used when stopping a channel)*/
#define	WAVEIO_TIMEOUT						MSF_ERROR_TIMEOUT	/**< timeout occurred while waiting for edge */

/** @}*/



/* WMSF_WAVEIO_PRESCALER
 * Auto-select the prescaler for wavein_init to achieve 1 MHz timer clock
 * The clock setup for TPM drivers depends on MSF_TPM_CLKSEL in msf_mkl25z.h
 * Timer clock for given F_CPU is:
 * F_CPU				TPM clock
 * 48 MHz 		 		8 MHz
 * 20.9 MHz				20.9 MHz
 * 8 MHz 				8 MHz
 * 4 MHz,F_BUS=0.8 MHz	4 MHz
 * 4 MHZ,F_BUS=1 MHz	8 MHz
 * 41.9 MHz				41943040 Hz
 *
 * WAVEIO_OVERFLOW_UNITMS
 * Define how many milliseconds it takes before the counter overflows.
 * The counter is 16-bit.
 * For timer clock 1 MHz it overflows about 15 times per second.
 * The time between overflows is about 65.5 ms
 * For timer clock 1.311 MHz overflows 20times per second; time
 * between overflows is 50 ms.
 *
 * WAVEIO_MULT_FACTOR
 * WAVEIO_DIV_FACTOR
 * Also define the factor to account for different clock speed than 1 MHz.
 * The resulting time is: time = counter * WAVEIN_MULT_FACTOR/WAVEIN_DIV_FACTOR;
 * */
#if F_CPU == 48000000
    #define WMSF_WAVEIO_PRESCALER      MSF_FTM_PRESCALER_8
	#define	WAVEIO_OVERFLOW_UNITMS	(66)
	#define	WAVEIO_MULT_FACTOR		(1)
	#define	WAVEIO_DIV_FACTOR		(1)

#elif ((F_CPU == 20900000) || (F_CPU == 20970000))
    #define WMSF_WAVEIO_PRESCALER      MSF_TPM_PRESCALER_16		/* timer clock 1310625 Hz! */
	#define	WAVEIO_OVERFLOW_UNITMS	(50)
	#define	WAVEIO_MULT_FACTOR		(76)
	#define	WAVEIO_DIV_FACTOR		(100)

#elif  F_CPU == 8000000
    #define WMSF_WAVEIO_PRESCALER      MSF_TPM_PRESCALER_8
	#define	WAVEIO_OVERFLOW_UNITMS	(66)
	#define	WAVEIO_MULT_FACTOR		(1)
	#define	WAVEIO_DIV_FACTOR		(1)

#elif  F_CPU == 4000000
	#if F_BUS == 800000
    	#define WMSF_WAVEIO_PRESCALER      MSF_TPM_PRESCALER_4
	#else
		#define WMSF_WAVEIO_PRESCALER      MSF_TPM_PRESCALER_8
	#endif

	#define	WAVEIO_OVERFLOW_UNITMS	(66)
	#define	WAVEIO_MULT_FACTOR		(1)
	#define	WAVEIO_DIV_FACTOR		(1)

#elif F_CPU == 41943040
    #define WMSF_WAVEIO_PRESCALER     MSF_TPM_PRESCALER_32	/* timer clock 1310720 */
	#define	WAVEIO_OVERFLOW_UNITMS	(50)
	#define	WAVEIO_MULT_FACTOR		(76)
	#define	WAVEIO_DIV_FACTOR		(100)
#else
	#error The CPU clock defined by F_CPU is not supported by waveout driver.
#endif



/* Function Prototype Section
* Add prototypes for all public functions.
* Write doxygen comments here!
*
***************************************************/ 
/**
 * @brief initialize the driver.
 * @param channel_mask mask indicating which channels will be used.
 * For convenience, use the macros WAVEIO_RANGE_0_5 etc.
 * You can also create the mask yourself: 1 in bit number N in the mask indicates that
 * the channel N will be used. For example, mask 0x01 means channel 0 will be used, 0x03 means
 * channels 0 and 1 will be used.
 * @note
 *
 **/
void waveio_init(uint32_t channel_mask);

/**
 * @brief un-initialize the driver.
 * @note
 *
 **/
void waveio_uninit(void);


/**
 * @brief Start generating signal on given channel (pin).
 * @param iochannel the channel to generate the signal on.
 * @param half1 length of the 1st part of the wave in microseconds (us)
 * @param half2 length of the 2nd part of the wave in microseconds (us)
 * @return WAVEIO_NO_ERROR (0) if OK, or WAVEIO_ERROR_INVALID_CHANNEL if the channel
 * number is not valid.
 * @note  The values given to WAVEIO_channel_start() are length of the half-waves
 *	of the signal we want to generate in microseconds (us).
 *	For example: 100 and 100 means wave length 200 us (signal period = 200 us)
 *	frequency_in_Hz = 1/period_in_second = 1 000 000 / period_in_us
 *	frequency_in_kHz = 1 000 / period_in_us
 *	period_in_us = 1 000 / frequency_in_kHz
 *	Example:
 *	We want to obtain 1 kHz signal: period = 1000/1 = 1000
 *	For square wave signal the half-wave values are 500 and 500.
 *
 *	The shortest half-wave length is approximately:
 *	half_wave_MIN = 500/CPU_clock_MHz
 *	Examples:
 *	For 48 MHz CPU clock it is 11 (us)
 *	For 4 MHz CPU clock it is 125 (us)
 *
 *	NOTE:
 *	Besides the shortest half-waves (max. frequency) described above, there is also
 *	limit to the shortest length of the half-wave in the signal without regard to the frequncy.
 *	For example, it is OK to generate signal with f = 1 KHz
 *	with 50% duty (half-wave values 500 and 500). BUT it may not be OK to ask for
 *	1% duty in this signal (half-wave values 10 and 990)! The value 10 is to low and cannot be
 *	achieved!
 *
 *	Longest half-wave length is:
 *	65535 for the CPU clock options which allow obtaining 1 MHz timer clock.
 *	About 48500 for the other CPU clock options.
 *	See the clock "#if" in waveio.h (this file). If the WAVEIO_MULT_FACTOR is not 1,
 *	the clock for timer is higher than 1 MHz and the longest half-vave value is
 *	lower than 65535. This is because the timer registers are 16 bits and the value must
 *	fit into the registers even after adjusting it from us to actual counter "ticks".
 *
 *
 */
uint8_t waveio_out_start(WAVEIO_channel iochannel, uint16_t half1, uint16_t half2);

/**
 * @brief Start generating signal on given channel for RC servo.
 * @param iochannel the channel to generate the signal on.
 * @param angle the angle between 0 and 180
 * @note this is convenience function for controlling the commonly used servo motor
 * used in Radio controlled models.
 *
 */
uint8_t waveio_out_servo(WAVEIO_channel iochannel, uint8_t angle);

/**
 * @brief Start generating signal on given channel for RC servo. Pulse length given in microseconds.
 * @param iochannel The channel to generate the signal on.
 * @param us the pulse width which should be between 1000 and 2000.
 * The function does not check for validity except for us < 19 ms; the
 * period of the signal will always be 20 ms.
 * @note this is convenience function for controlling the commonly used servo motor
 * used in Radio controlled models.
 */
uint8_t waveio_out_servo_us(WAVEIO_channel iochannel, uint16_t us);

/**
 * @brief Stop generating the output for given channel.
 * @param iochannel the channel to stop generating output to.
 * @return WAVEIO_NO_ERROR (0) if OK; error code otherwise, see waveout.h
 * @note The function will wait for the pin to become low before it returns so
 * it may block the caller up to the half2 microseconds of the current signal.
 * At most this could be about 65 ms.
 *
 */
uint8_t waveio_out_stop(WAVEIO_channel iochannel);

/**
 * @brief Connect given channel to the driver as input.
 * @param iochannel the channel to attach.
 * @return WAVEIO_NO_ERROR (0) if OK; error code otherwise, see waveio.h
 * @note  This configures the pin for underlying timer channel into timer mode.
 */
uint8_t waveio_in_attach(WAVEIO_channel iochannel);

/**
 * @brief Disconnect given input channel from the driver.
 * @param iochannel the channel to detach.
 * @note This disconnects the pin from the timer.
 *
 */
void waveio_in_detach(WAVEIO_channel iochannel);

/**
 * @brief Start capturing the input for given channel.
 * @param iochannel the channel to capture.
 * @note  This starts updating the values for the given channel.
 *
 */
uint8_t waveio_in_start(WAVEIO_channel iochannel);

/**
 * @brief Stop capturing the input for given channel.
 * @param iochannel the channel to stop capturing.
 * @return WAVEIN_NO_ERROR (0) if OK; error code otherwise, see wavein.h
 * @note  This stops updating the values for the given channel. Use this when the data
 *  from the channel are needed for some time (but will be needed later, so it is not
 *  suitable to detach the channel.
 *  It will reset the internal data for this channel.
 *
 */
uint8_t waveio_in_stop(WAVEIO_channel iochannel);

/**
 * @brief Read the values captured for given channel
 * @param iochannel the channel for which we want to read
 * @param pulseA pointer to user-provided variable which receives the length
 * of the 1st part of the wave (the "pulse"). In microseconds (us).
 * @param pulseB pointer to user-provided variable which receives the length
 * of the 2nd part of the wave (space after the pulse)
 * @return WAVEIN_NO_ERROR (0) if OK; error code otherwise, see wavein.h
 * @note Do not assume that both parts A and B must be valid. Invalid (unavailable) parts
 * will be set to 0.
 * Also do not assume any particular order of the parts. For example, if measuring RC PPM signal,
 * the pulse may be in part A or part B depending on when the measurement started (or
 * when the input signal was connected).
 *
 */
uint8_t waveio_in_read(WAVEIO_channel iochannel, uint16_t* pulseA, uint16_t* pulseB  );

/**
 * @brief Wait for a pulse (2 edges) on given channel.
 * @param iochannel the channel for which we want to wait.
 * @param timeout maximum time in milliseconds to wait. If 0 is specified, wait infinitely.
 *  Note that the timeout measurement is "rough", about 66 ms for 1 MHZ timer clock,
 *  see WAVEIN_OVERFLOW_UNITMS.
 * @return the length of the pulse or 0 if the timeout occurs.
 * @note This function can be used to measure single pulse on input, for example, in case of the
 * Ping ultrasonic sensor we sent a pulse and then wait for a pulse on the same pin.
 * It will reset the internal data buffers and then wait for 2 edges.
 * Must be called before the 1st edge occurs on the input pin!
 *
 **/
uint16_t waveio_in_pulse_wait(WAVEIO_channel iochannel, uint32_t timeout );

/**
 * @brief Read input from an RC receiver on given channel.
 * @param iochannel the channel for which we want to read the input.
 * @return the length of the pulse in microseconds or 0 on error.
 * @note This is convenient wrapper for waveio_in_read.
 * The function will handle checking the correct length of the pulse
 * and return only values between 500 and 2500 us. This is broader range than
 * typically needed. Typical RC signal is 1000 to 2000 us.
*/
uint16_t waveio_in_servo_read_us(WAVEIO_channel iochannel);



#ifdef __cplusplus
}
#endif

/**@}*/
/* ----------- end of file -------------- */
#endif /* MSF_WAVEIO_DRV_H */
