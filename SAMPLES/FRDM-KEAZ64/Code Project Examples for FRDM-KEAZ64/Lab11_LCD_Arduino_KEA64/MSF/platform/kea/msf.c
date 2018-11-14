/****************************************************************************
 * @file     msf.c
 * @brief    The "global" functions of the framework for Kinetis
 * @version  1
 * @date     29. Apr2014
 *
 * @note    About the CPU clock in Kinetis:
 *          The CPU clock is set by the program and depends on many
 *          varaibles. It is out of scope for MSF to support setting the clock;
 *          We assume user sets the clock and lets us know about the speed
 *          using the F_CPU constant defined in user config .h file.
 *
 ******************************************************************************/

/* Include user_config.h */
#include "msf_config.h" 
/* Include the core definitions - CMSIS <device>.h file etc.*/
#include "coredef.h"

#include "msf.h"

#include <stdlib.h>	/* abs */


#if MSF_USE_STDIO
    #include "coniob.h"  /* To initialize console in msf_init()*/
#endif

#if MSF_USE_ANALOG
    #include "drv_adc.h"    /* To initialize ADC for msf_analog_read */
#endif

/* Global variables
   Internal use only!
*/
/** 
 @brief hold the number of ms elapsed from start of the program.
 @note It is incremented in SysTick interrupt every ms. The SysTick is 
 initialized to interrupt every ms in msf_init()
*/
volatile uint32_t   gmsf_systime;
volatile uint32_t   gmsf_delaycnt;

/** @addtogroup group_globals  
* @{ 
*/

/* Initialize the MSF */
uint8_t msf_init(uint32_t param)
{
    uint8_t err = MSF_ERROR_OK;
    
    gmsf_systime = 0;
    gmsf_delaycnt = 0;
    
    /* Use SysTick as reference for the delay loops. 
    Configure the SysTick interrupt to occur every ms */
    if ( SysTick_Config (SystemCoreClock / 1000u ) != 0 )
    	err = MSF_ERROR_SYSTIMER;
    
    /* Kinetis (ARM) needs enabling clock to GPIO ports*/
    gpio_init();
    
	// initialize standard I/O if desired
#if MSF_USE_STDIO
	coniob_init(MSF_STDIO_BAUDRATE);	
#endif

	// initialize ADC if desired
#if MSF_USE_ANALOG
	Driver_ADC0.Initialize(null);
#endif 
	
	/* Enable interrupts */
	EnableInterrupts;
    
    return err;
  
}

void msf_delay_ms(uint32_t millis)
{
    gmsf_delaycnt = millis;

    /* Busy wait until the SysTick decrements the counter to zero */
    while (gmsf_delaycnt != 0u)
        MSF_RESET_WATCHDOG();   
}


uint32_t msf_millis()
{
    return gmsf_systime;
}


uint32_t msf_micros()
{   
	uint32_t fraction = (SysTick->LOAD - SysTick->VAL) / MSF_SYSTICK_VALINUS;
	/* The value in the SysTick->LOAD register is the number of clock ticks
	 * between SysTick interrupts. The VAL register is loaded with this value
	 * and then decremented. When it reaches 0, SysTick interrupt occurs.
	 * msf_init sets the interrupt to occur every ms, so the value in
	 * LOAD is (SystemCoreClock / 1000) and this is how many "ticks" there are
	 * in 1 ms. 
	 * msf_<device>.h defines constant we use to convert the count in LOAD to us  */
	
    return (msf_millis() * 1000u + fraction);
}

/**
 *  @note There are some limitations:
 * 	- The function works reliably from certain minimal value (depending on F_CPU) - about 6 us 
 * 	 for F_CPU 8 and 21 MHz, about 2 us for 48 MHz.
 * 	- The results are better for F_CPU in exact MHz, e.g. 48 MHz  is better than 41.9 MHz - see clock options. 
 *  - If interrupts occur while this delay is in progress the delay time will be longer
 *  (as this si simple busy loop it will execute given number of times, not knowing that 
 *  some time elapsed in ISR.
 *  - It is safe to call this function from ISR; it will function ok.
 *  - The maximum delay is about 2 ^32 / 6 so about 715 millions of us, but it does not make
 *  much sense to call this function for delays longer than few millisecond; use delay_ms instead
 *  combined with delay_us if desired.
 *  - When testing this function note the overhead in measuring the time with msf_micros() which is
 *  about 26 us! (as of May 2014 version). 
 *  
 */
void msf_delay_us(uint32_t micros)
{
	/* convert us to clock cycles and divide by 3 as the loop below
	 * takes 3 cycles */
	
	
#if F_CPU > 20000000
	/* There is some overhead for the function call which we have defined in the
	 * msf_<device>.h  for each F_CPU*/
	if ( micros <= WMSF_DELAYUS_OVERHEAD )
			return;
	micros -= WMSF_DELAYUS_OVERHEAD;
		
	/* For F_CPU > 20 MHz we assume we can calculate the micro to clocks without
	 * bringing big error into the delay 
	 * Divide by 7 because the ASM loop takes 7 cycles and 7 cycles because
	 * this makes (MSF_CLOCKS_PER_US / 7) nice number for the common F_CPU */
	 micros = micros * MSF_CLOCKS_PER_US / 7;
	__asm__ __volatile__ (				
				"1: SUB %0, #1" "\n\t"	// 1 clock
				"nop" "\n\t"			// 1 clock 
				"nop" "\n\t"			// 1 clock 
				"nop" "\n\t"			// 1 clock 
				"nop" "\n\t"			// 1 clock 
				"BNE 1b"  				// 2 clocks if branches, 1 if not
			 : /* no outputs */ 
			 : "r" (micros)  
			);
	
	
#elif F_CPU == 8000000
	/* There is about 3 us overhead at 8 MHz F_CPU for the function call */
	if ( micros <= 3 )
		return;
	micros -= 3;
	 /* r0 register holds the input argument (micros) but
	  * after any manipulation in C the result is in r3 */
	/* ASM loop which takes 8 clocks, that is 1 us */
	__asm__ __volatile__ (
			//"a: nop" "\n\t" 	// 1 clock
			"1: SUB %0, #1" "\n\t"	// 1 clock
			"nop" "\n\t"
			"nop" "\n\t"
			"nop" "\n\t"
			"nop" "\n\t"
			"nop" "\n\t"
			"BNE 1b"  				// 2 clocks if branches, 1 if not
			: /* no outputs */ 
			: "r" (micros)
		);

#elif F_CPU == 4000000

	/* There is about 6 us (?) overhead at 4 MHz F_CPU for the function call */
	if ( micros <= 6 )
		return;
	micros -= 6;
	 /* r0 register holds the input argument (micros) but
	  * after any manipulation in C the result is in r3 */
	/* ASM loop which takes 8 clocks, that is 1 us */
	__asm__ __volatile__ (
			//"a: nop" "\n\t" 	// 1 clock
			"1: SUB %0, #1" "\n\t"	// 1 clock
			"nop" "\n\t"
			"nop" "\n\t"
			"nop" "\n\t"
			"nop" "\n\t"
			"nop" "\n\t"
			"BNE 1b"  				// 2 clocks if branches, 1 if not
			: /* no outputs */
			: "r" (micros)
		);
#else
	#error msf_delay_us is not implemented for this F_CPU. 
#endif
	
	
		
	
	
	/* Abandoned version with polling of SysTick timer. 
	 * It has big overhead which depends on F_CPU and thus no advantage
	 * compared to simple busy loop. */
#if 0
	uint32_t end_val;
	/* First wait the whole ms, if any */
	uint32_t ms = micros / 1000u;
	if ( ms > 0 )
		msf_delay_ms(ms);
	
	/* the remaining us to wait */
	micros = micros % 1000u;
	if ( micros == 0 )
		return;
	
	/* convert us to "tick" of the SysTick counter. The counter runs from LOAD val to zero
	 * in 1 ms and we will always have wait-time < than ms. */
	micros *= MSF_SYSTICK_VALINUS;	
	/* Now wait the number of ticks */
	if ( micros <= SysTick->VAL )
	{
		/* If there is no under-flow of the VAL during our wait:
		 * e.g. wait 10 ticks when VAL is 50; so wait till it reaches 40 */
		end_val = SysTick->VAL - micros;
		while ( SysTick->VAL > end_val  )
			MSF_RESET_WATCHDOG();   
	}
	else
	{
		/* If there is an underflow:
		 * e.g. wait 10 ticks when VAL is 6; so wait till it goest to 0, reloads with
		 * full value (LOAD) and reaches LOAD - (wait-VAL), e.g. 
		 * LOAD = 100; end = 100 - 10 + 6 = 96*/
		end_val = SysTick->LOAD - micros + SysTick->VAL;
		while ( SysTick->VAL > 0 )
			MSF_RESET_WATCHDOG();
		while ( SysTick->VAL > end_val )
			MSF_RESET_WATCHDOG();			
	}
#endif
	
}

/** @}*/

/* Handler for the SysTick interrupt.
The name of the function is pre-defined by CMSIS */
void SysTick_Handler (void)
{
    /* global system time */
    gmsf_systime++; 
    
    /* Decrement to zero the counter used by the msf_delay_ms */
    if (gmsf_delaycnt != 0u)
    {
        gmsf_delaycnt--;
    }
}

  



