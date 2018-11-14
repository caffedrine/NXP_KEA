/*
 * lcd_routines.h
 *
 *  Created on: Nov 9, 2015
 *      Author: B48683
 */

#ifndef LCD_ROUTINES_H_
#define LCD_ROUTINES_H_

#include "LiquidCrystal.h"
#include "LCD_DFR0009.h"
#include "uart_kea.h"
#include "drv_uart.h"
#include "drv_adc.h"
#include "adc_kea.h"
#include "derivative.h"

enum {
	message=0,
	echo=1,
	adc=2,
	arithmetic=3,
	fortune=4
}lcd_mode;

void Echo(LiquidCrystal* plcd);

void ADC_Show(LiquidCrystal * plcd);

void itoa(int32_t value, uint8_t array[]);

void msg(LiquidCrystal* plcd);

void DisplayBounce(LiquidCrystal * plcd, uint8_t strlen);

void math(LiquidCrystal* plcd);

void math_fail(LiquidCrystal* plcd);

void Print_Result(LiquidCrystal * plcd, int32_t result, int32_t remainder);

void fortune_teller(LiquidCrystal* plcd);

uint8_t print_fortune(LiquidCrystal* plcd);

#endif /* LCD_ROUTINES_H_ */
