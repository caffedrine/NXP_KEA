/*
 * PWM.h
 *
 *  Created on: May 24, 2018
 *      Author: Alex C.
 *
 *  PWM Driver!
 *
 *  Known bugs: It does not work on FTM1
 */

#ifndef _PWM_PWM_H_
#define _PWM_PWM_H_

#include <stdint.h>
#include "derivative.h"

void PWM_Init(FTM_Type *pFTM, uint32_t u32clock, uint16_t u16periodUS);
void PWM_Enable(FTM_Type *pFTM, uint8_t channel);
void PWM_SetRatio16(FTM_Type *pFTM, uint8_t ftmChannel, uint16_t ratio);

#endif /* SOURCES_PWM_PWM_H_ */
