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
* @file      bsp/UART/lin_lld_timesrv.h
*
* @author    FPT Software
*
* @brief     Timer and time interrupt for S08, S12, S12X
*
******************************************************************************/

/******************************************************************************
 *
 * History:
 *
 * 20140516		v1.0	First version
 *
 *****************************************************************************/  

#ifndef __LIN_LLD_TIMESRV
#define __LIN_LLD_TIMESRV

/* MODULE LIN_LLD_TIMESRV. */
#include "lin_cfg/lin_cfg.h"
#include "LIN_Driver/lowlevel/lin.h"


/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_timer_K_init()
* @brief Initialize TIM1 timer
*
*
* @SDD_ID LIN_SDD_114
* @endif
*
* @return #void
*
* @details
*   Initialize TIM1 timer
*
*//*END*----------------------------------------------------------------------*/
	void lin_lld_timer_K_init(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_timer_isr()
* @brief Timer interrupt service routine
*
* @return #void
* 
* @SDD_ID LIN_SDD_113
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>i</I>
*
* @see #lin_lld_uart_timeout
*
* @details
*   Timer interrupt service routine
*//*END*----------------------------------------------------------------------*/ 
#ifdef MULTI_TIMER_MODE
void lin_lld_timer_isr(l_u8 index);
#else
void lin_lld_timer_isr(void);
#endif /* End MULTI_TIMER_MODE */

#endif /* __LIN_LLD_TIMESRV */

/**
* @}
*/
