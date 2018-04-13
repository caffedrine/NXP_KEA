/*
 * lin_isr.h
 *
 *  Created on: Nov 30, 2015
 *      Author: B48683
 */

#ifndef LIN_ISR_H_
#define LIN_ISR_H_

#include "lin_hw_cfg.h"
#include "lin_lld_timesrv.h"
#include "lin_lld_uart.h"

void FTM2_IRQHandler_LIN(void);

#if _UART1_ == 1

void UART1_IRQHandler_LIN(void);

#elif _UART0_ == 1

void UART0_IRQHandler_LIN(void);

#endif

#endif /* LIN_ISR_H_ */
