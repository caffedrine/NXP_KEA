/** ###################################################################
**      Filename    : Vectors_Config.h
**      Processor   : SKEAZ64MLH4
**      Version     : 1.00
** 
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file Vectors_Config.h                                                  
** @version 01.00
*/         
/*!
**  @addtogroup Vectors_Config_module Vectors_Config module documentation
**  @{
*/         

#ifndef __Vectors_Config_H
#define __Vectors_Config_H

/* MODULE Vectors_Config.h */
  
#include "Cpu.h"
#include "Pins1.h"
#include "PWM1.h"
#include "PwmLdd1.h"
#include "TU1.h"
#include "Events.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PEX_VECTOR_TABLE                                   0x01U     /* Vector table is managed by PEx */ 

/* Vector                 Address                                  No.  Pri  Name                          Description */
#define VECTOR_SP_MAIN    &__SP_INIT                            /* 0x00 -    ivINT_Initial_Stack_Pointer   used by PE */
#define VECTOR_1          (tIsrFunc)&__thumb_startup            /* 0x01 -    ivINT_Initial_Program_Counter used by PE */
#define VECTOR_2          (tIsrFunc)&Cpu_INT_NMIInterrupt       /* 0x02 -2   ivINT_NMI                     used by PE */
#define VECTOR_3          (tIsrFunc)&UnhandledInterrupt         /* 0x03 -1   ivINT_Hard_Fault              unused by PE */
#define VECTOR_4          (tIsrFunc)&UnhandledInterrupt         /* 0x04 -    ivINT_Reserved4               unused by PE */
#define VECTOR_5          (tIsrFunc)&UnhandledInterrupt         /* 0x05 -    ivINT_Reserved5               unused by PE */
#define VECTOR_6          (tIsrFunc)&UnhandledInterrupt         /* 0x06 -    ivINT_Reserved6               unused by PE */
#define VECTOR_7          (tIsrFunc)&UnhandledInterrupt         /* 0x07 -    ivINT_Reserved7               unused by PE */
#define VECTOR_8          (tIsrFunc)&UnhandledInterrupt         /* 0x08 -    ivINT_Reserved8               unused by PE */
#define VECTOR_9          (tIsrFunc)&UnhandledInterrupt         /* 0x09 -    ivINT_Reserved9               unused by PE */
#define VECTOR_10         (tIsrFunc)&UnhandledInterrupt         /* 0x0A -    ivINT_Reserved10              unused by PE */
#define VECTOR_11         (tIsrFunc)&UnhandledInterrupt         /* 0x0B -    ivINT_SVCall                  unused by PE */
#define VECTOR_12         (tIsrFunc)&UnhandledInterrupt         /* 0x0C -    ivINT_Reserved12              unused by PE */
#define VECTOR_13         (tIsrFunc)&UnhandledInterrupt         /* 0x0D -    ivINT_Reserved13              unused by PE */
#define VECTOR_14         (tIsrFunc)&UnhandledInterrupt         /* 0x0E -    ivINT_PendableSrvReq          unused by PE */
#define VECTOR_15         (tIsrFunc)&UnhandledInterrupt         /* 0x0F -    ivINT_SysTick                 unused by PE */
#define VECTOR_16         (tIsrFunc)&UnhandledInterrupt         /* 0x10 -    ivINT_Reserved16              unused by PE */
#define VECTOR_17         (tIsrFunc)&UnhandledInterrupt         /* 0x11 -    ivINT_Reserved17              unused by PE */
#define VECTOR_18         (tIsrFunc)&UnhandledInterrupt         /* 0x12 -    ivINT_Reserved18              unused by PE */
#define VECTOR_19         (tIsrFunc)&UnhandledInterrupt         /* 0x13 -    ivINT_Reserved19              unused by PE */
#define VECTOR_20         (tIsrFunc)&UnhandledInterrupt         /* 0x14 -    ivINT_Reserved20              unused by PE */
#define VECTOR_21         (tIsrFunc)&UnhandledInterrupt         /* 0x15 -    ivINT_FTMRE                   unused by PE */
#define VECTOR_22         (tIsrFunc)&UnhandledInterrupt         /* 0x16 -    ivINT_LVD_LLW                 unused by PE */
#define VECTOR_23         (tIsrFunc)&UnhandledInterrupt         /* 0x17 -    ivINT_IRQ                     unused by PE */
#define VECTOR_24         (tIsrFunc)&UnhandledInterrupt         /* 0x18 -    ivINT_I2C0                    unused by PE */
#define VECTOR_25         (tIsrFunc)&UnhandledInterrupt         /* 0x19 -    ivINT_I2C1                    unused by PE */
#define VECTOR_26         (tIsrFunc)&UnhandledInterrupt         /* 0x1A -    ivINT_SPI0                    unused by PE */
#define VECTOR_27         (tIsrFunc)&UnhandledInterrupt         /* 0x1B -    ivINT_SPI1                    unused by PE */
#define VECTOR_28         (tIsrFunc)&UnhandledInterrupt         /* 0x1C -    ivINT_UART0                   unused by PE */
#define VECTOR_29         (tIsrFunc)&UnhandledInterrupt         /* 0x1D -    ivINT_UART1                   unused by PE */
#define VECTOR_30         (tIsrFunc)&UnhandledInterrupt         /* 0x1E -    ivINT_UART2                   unused by PE */
#define VECTOR_31         (tIsrFunc)&UnhandledInterrupt         /* 0x1F -    ivINT_ADC0                    unused by PE */
#define VECTOR_32         (tIsrFunc)&UnhandledInterrupt         /* 0x20 -    ivINT_ACMP0                   unused by PE */
#define VECTOR_33         (tIsrFunc)&UnhandledInterrupt         /* 0x21 -    ivINT_FTM0                    unused by PE */
#define VECTOR_34         (tIsrFunc)&UnhandledInterrupt         /* 0x22 -    ivINT_FTM1                    unused by PE */
#define VECTOR_35         (tIsrFunc)&UnhandledInterrupt         /* 0x23 -    ivINT_FTM2                    unused by PE */
#define VECTOR_36         (tIsrFunc)&UnhandledInterrupt         /* 0x24 -    ivINT_RTC                     unused by PE */
#define VECTOR_37         (tIsrFunc)&UnhandledInterrupt         /* 0x25 -    ivINT_ACMP1                   unused by PE */
#define VECTOR_38         (tIsrFunc)&UnhandledInterrupt         /* 0x26 -    ivINT_PIT_CH0                 unused by PE */
#define VECTOR_39         (tIsrFunc)&UnhandledInterrupt         /* 0x27 -    ivINT_PIT_CH1                 unused by PE */
#define VECTOR_40         (tIsrFunc)&UnhandledInterrupt         /* 0x28 -    ivINT_KBI0                    unused by PE */
#define VECTOR_41         (tIsrFunc)&UnhandledInterrupt         /* 0x29 -    ivINT_KBI1                    unused by PE */
#define VECTOR_42         (tIsrFunc)&UnhandledInterrupt         /* 0x2A -    ivINT_Reserved42              unused by PE */
#define VECTOR_43         (tIsrFunc)&UnhandledInterrupt         /* 0x2B -    ivINT_ICS                     unused by PE */
#define VECTOR_44         (tIsrFunc)&UnhandledInterrupt         /* 0x2C -    ivINT_WDOG_EWM                unused by PE */
#define VECTOR_45         (tIsrFunc)&UnhandledInterrupt         /* 0x2D -    ivINT_PWT                     unused by PE */
#define VECTOR_46         (tIsrFunc)&UnhandledInterrupt         /* 0x2E -    ivINT_MSCAN_RX                unused by PE */
#define VECTOR_47         (tIsrFunc)&UnhandledInterrupt         /* 0x2F -    ivINT_MSCAN_TX                unused by PE */
  
#ifdef __cplusplus
}
#endif

/* END Vectors_Config.h */

#endif /* __Vectors_Config_H */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file is a part of Processor Expert static initialization
**     library for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
