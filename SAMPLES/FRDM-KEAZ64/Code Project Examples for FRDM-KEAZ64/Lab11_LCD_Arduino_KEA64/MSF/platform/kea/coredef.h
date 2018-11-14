/****************************************************************************
 * @file     coredef.h
 * @brief    Includes the core definitions for this platform
 *              
 * @version  1
 * @date     10. May 2014
 *
 * @note    This file includes the file(s) which define the hardware registers
 *          etc. for given platform. 
 *          This may be <device>.h file in case of ARM CMSIS or "derivative.h"
 *          file in CodeWarrior for HCS08.
 *          This may be toolchain-dependent. 
 *          If more toolchains are supported, there may be #if here          
 *
 ******************************************************************************/
#ifndef MSF_COREDEF_H
#define MSF_COREDEF_H

#ifndef MSF_CONFIG_H
	#error Please include the msf_config.h file before including coredef.h
#endif

#if (CW_10) || (S32DS)
	/* The HW registers of the MCU, etc. 
    For Kinetis we use our own "derivative.h" file located in each project 
    which includes the CMSIS <device>.h for the actual MCU */
    #include "derivative.h"
	
    /* The  CMSIS <device>.h file includes "core" components such as stdint.h  in itself */	
	#include "msf_stdbool.h"	/* Include ouw definition of bool; not supported in EWL */

	/* define the null */
	#define	null	(0)
	
#else
    #error Please define the toolchain - include msf_config.h before including coredef.h 
#endif
/* ----------- end of file -------------- */
#endif /* MSF_COREDEF_H */
