/*
 *	ADC.h
 *
 *  Created on: May 16, 2018
 *      Author: Alex C.
 */


#ifndef ADC_H_
#define ADC_H_

#include "derivative.h"

#define ADC_EIGHT_BIT 	0
#define ADC_TEN_BIT 	1
#define ADC_TWELVE_BIT 	2
                

/***********************************************************************************************
*
* @brief    ADC_Init - Initiates the ChannelN to read the value of the ADC channel
* 			 
* @param    Channel to init and resolution
* @return   none
*
************************************************************************************************/  
void ADC_Init(uint8_t channel, uint8_t mode);

/***********************************************************************************************
*
* @brief    ADC_Read - Read the selected ADC channel
* @param    ch - channel to read
* @return   result
*
************************************************************************************************/
uint16_t ADC_Read(uint8_t channel);

#endif /* ADC_H_ */
