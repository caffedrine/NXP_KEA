/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2008-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************/
/**************************************************************************//**
* @addtogroup UART_group
* @{
******************************************************************************/
/**************************************************************************//**
*
* @file      bsp/UART/lin_lld_timesrv.c
*
* @author    FPT Software
*
* @brief     Timer for KEA
*
******************************************************************************/  

/******************************************************************************
 *
 * History:
 * 2014   v1.0  First version for Kinetis timer handler
 *
 *****************************************************************************/
 
#include "LIN_Driver/bsp/UART/lin_lld_timesrv.h"
#include "LIN_Driver/bsp/UART/lin_lld_uart.h"
#include "LIN_Driver/include/lin_common_proto.h"
#include "lin_cfg/lin_hw_cfg.h"


void lin_lld_timer_K_init
  (
    void
  ) 
{
		SIM_SCGC |= SIM_SCGC_FTM2_MASK; /* Enable Clock for FTM2 */			
		FTM2_SC |= FTM_SC_PS(7);	/* Select Preescaler in this case 128. 20 Mhz /128 =156.25 Khz. */ 
									/* Counter increase by one every 6.4 us */		
		/* Enable Channle 0*/
		FTM2_C0SC |= FTM_CnSC_CHIE_MASK; /* Enable channel 0 interrupt */
		FTM2_C0SC |= FTM_CnSC_MSA_MASK;  /* Channel as Output compare mode */			
		/*Select interrupt frequency*/
		FTM2_C0V = FTM_CnV_VAL(((MCU_BUS_FREQ << 1)/1000000.0) * (TIME_BASE_PERIOD / 128.0));	 	/* Interrupt every TIME_BASE_PERIOD/6.4 counts,  equal to every TIME_BASE_PERIOD */

		FTM2_SC |= FTM_SC_CLKS(1); /*FTM2 use system clock*/
				
		/* Set the ICPR and ISER registers accordingly */

		//NVIC_ICPR |= 1 << ((INT_FTM2-16)%32);
		//NVIC_ISER |= 1 << ((INT_FTM2-16)%32);

		NVIC_ClearPendingIRQ(FTM2_IRQn);
		NVIC_EnableIRQ(FTM2_IRQn);

}
#ifdef MULTI_TIMER_MODE
   void lin_lld_timer_isr(l_u8 index)
   {
      /* Timeout for UART channel */    
      lin_lld_uart_timeout(lin_virtual_ifc[index]);    
      /*** Master task firing ***/  
   }
#else /* Else MULTI_TIMER_MODE */
   void lin_lld_timer_isr(void)
   {
   #if LIN_MODE == _MASTER_MODE_
     l_u8 i;
     /* Timeout for UART channel */    
     for(i=0; i<LIN_NUM_OF_IFCS; i++) { 
       lin_lld_uart_timeout(lin_virtual_ifc[i]);
     }    
     /*** Master task firing ***/  
   #else
     lin_lld_uart_timeout();
   #endif /* End LIN_MODE == _MASTER_MODE_ */
   }
#endif /* End MULTI_TIMER_MODE */

/**
* @}
*/
