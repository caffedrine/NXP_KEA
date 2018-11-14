/****************************************************************************
 * @file     msf_config_<device>.h
 * @brief    User configuration for the MSF library 
 * @version  2
 * @date     22. Apr 2014
 *
 * @note     This file should contain settings which are fixed for given board
 *    
 *
 ******************************************************************************/
#ifndef MSF_CONFIG_MKEA_H
#define	MSF_CONFIG_MKEA_H

/*********************************************
*    Select your platform, e.g. KINETIS
********************************************/
#define     MSF_KINETIS     1


/*********************************************
*    Select your device from the platform 
********************************************/
//#define   MSF_KINETIS_MKL25Z      0 
#define   MSF_KINETIS_SKEA         1


/*********************************************
*    Define the compiler to be used 
********************************************/
#define     CW_10    0
#define     S32DS    1


/* TODO: moved to project-private file msf_config.h*/
/*********************************************
*    Define the standard I/O channel 
********************************************/
//#define   MSF_USE_STDIO     1   /* Using conio driver (UART) for std output */
/* To disable initializing UART in msf_init use:
#define   MSF_USE_STDIO     0 
*/

/*********************************************
*    Define whether we want to use analog inputs
*    If nonzero; ADC is initialized. This means 
*    little more power consumption, otherwise no
*    harm is done by enabling the analog inputs even
*    if not used 
********************************************/
//#define MSF_USE_ANALOG      1


/*********************************************
*    Define which drivers should be created (used) 
*    It may save some memory to define drivers which 
*    your applicaiton will not use with 0.
********************************************/
//#define MSF_DRIVER_UART0    1   /* Use UART0 driver (create UART0 driver instance) */
//#define MSF_DRIVER_ADC0     1	/* Use ADC driver */

/*********************************************
*    Define inline option for functions
********************************************/
#define     MSF_USE_INLINE  1
/* NOTE: it is not possible to define macro for pragma, e.g.
    #define     MSF_INLINE  #pragma INLINE
    except for C99 compilers, where is should be possible to use:
    _Pragma("argument") which is equivalent to #pragma argument 
*/

/*********************************************
*    Define the MSF API components
********************************************/
/* Note: this is defined by MSF implementor for given device.
    The supported group of fucntions should be defined as 1,
    not-supported as 0.
*/ 
#define     MSF_TIME_API               1
#define     MSF_SHORT_DELAY_API        1
#define     MSF_PRINT_API              1
#define     MSF_ANALOG_API             1 
#define     MSF_SWTIMER_API            0


/*********************************************
*    Default options for MSF drivers
********************************************/
/* Note: these are settings applied to the driver when it is initialized.
    For example: resolution of ADC.
    It can be changed with care. The values are not checked in the code and
    the driver may stop working.
    You can change some options of the drivers also in runtime, see the <driver>.Control(). 
*/

/******************** ADC driver options *************************/
/** ADC resolution bit field value
0 = 8-bit
1 = 10-bit
2 = 12-bit
@note this is default value; any value can be set by driver Control function.  
*/
#define WMSF_ADC_RESOLUTION     (2)
 

 
/******************** End ADC driver options *************************/

/* Check if there is valid F_CPU defined in msf-config.h */
#if !( (F_CPU == 41943040) ||  (F_CPU == 48000000) || (F_CPU == 8000000) || (F_CPU == 20900000) || (F_CPU == 4000000)  || (F_CPU == 20970000))
	#error Please define valid F_CPU in msf_config.h. It is possible that the clock speed is not supported.
#endif



#endif /* MSF_CONFIG_SKEA_H */
