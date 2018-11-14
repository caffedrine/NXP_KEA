#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "derivative.h"

/* Peripheral base address parameter */
#define SysTick_DEVICE SysTick_BASE_PTR

void SysTick_Init(void);

#endif // __SYSTICK_H__
