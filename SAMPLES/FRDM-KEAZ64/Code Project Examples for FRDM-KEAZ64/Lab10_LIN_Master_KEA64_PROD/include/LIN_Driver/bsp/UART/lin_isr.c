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
* @file      bsp/UART/lin_isr.c 
*
* @author    FPT Software
*
* @brief     Interrupt service routine used in LIN driver
*
******************************************************************************/  

/******************************************************************************
 *
 * History:
 *
 * 20140422		v1.0	First version for KEA family support	
 *
 *****************************************************************************/
 
#include "lin_cfg/lin_hw_cfg.h"
#include "lin_lld_timesrv.h"
#include "lin_lld_uart.h"


  /**********************************************************************************************
* Local functions
**********************************************************************************************/
void FTM2_IRQHandler();

#if _UART1_==1
void UART1_IRQHandler();
  /***********************************************************************************************
  *
  * @brief    UART2_SCI2_IRQHandler - Uart1 reception interrupt, calls the user defined callback function 
  * @param    none
  * @return   none
  *
  ************************************************************************************************/
  void UART1_IRQHandler()
  {
#if (LIN_MODE == _MASTER_MODE_)
	  lin_lld_uart_rx_isr(uart1);
#else
	  lin_lld_uart_rx_isr();
#endif /* (LIN_MODE == _MASTER_MODE_) */

  }
#endif /*end _UART1_==1*/
  /***********************************************************************************************
  *
  * @brief    FTM2_IRQHandler - Interrupt for FTM channels, clear flag, set new tiemout and execute callback 
  * @param    none
  * @return   none
  *
  ************************************************************************************************/
  void FTM2_IRQHandler()
  {
  	
    if (1==((FTM2_C0SC & FTM_CnSC_CHF_MASK)>>FTM_CnSC_CHF_SHIFT) )  /* If the CHF of the channel is equal to 0 */
  	{
  		(void)FTM2_C0SC;  							/* Read to clear flag */
  		FTM2_C0SC ^= FTM_CnSC_CHF_MASK;  			/* Clear flag */
  		FTM2_C0V += (((MCU_BUS_FREQ << 1) / 1000000.0) * (TIME_BASE_PERIOD / 128.0)); /* Refresh interrupt period */
  		lin_lld_timer_isr();
  	}
  }
