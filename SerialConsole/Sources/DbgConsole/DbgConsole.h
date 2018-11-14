/*
 * DbgConsole.h
 *
 *  Created on: May 16, 2018
 *      Author: Alex C.
 */

//!< Compilation flags required:
//!< specs=nano.specs -specs=nosys.specs -u _printf_float -u _scanf_float

#ifndef _DBGCONSOLE_H_
#define _DBGCONSOLE_H_

#include <stdint.h>
#include <stdio.h>
#include "derivative.h"

//#define USE_UART_INTERNAL_IMPLEMENTATION

void DbgConsole_Init(UART_Type *pUART, uint32_t u32SysClk, uint32_t u32Baud);

#endif /* _DBGCONSOLE_H_ */
