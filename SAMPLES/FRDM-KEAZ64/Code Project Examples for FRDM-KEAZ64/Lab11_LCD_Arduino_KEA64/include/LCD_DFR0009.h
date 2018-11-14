/*
 * DFR0009.h
 *
 *  Created on: Oct 30, 2015
 *      Author: B48683
 */

#ifndef LCD_DFR0009_H_
#define LCD_DFR0009_H_

#include "derivative.h"
#include "LiquidCrystal.h"

/* Refer to the DFR0009 schematic for these pinouts. */

#define RS 8
#define E 9
#define RW SAVE_PIN //Wired to GND
#define D0 4
#define D1 5
#define D2 6
#define D3 7
#define D4 0
#define D5 0
#define D6 0
#define D7 0

/* These macros define the ADC limits of the DFR0009 push buttons. Each push button changes the ADC value to a certain range. 12-bit precision of ADC */

#define BTN_RIGHT_LOW 0
#define BTN_RIGHT_HIGH 0
#define BTN_DOWN_LOW 1320
#define BTN_DOWN_HIGH 1350
#define BTN_UP_LOW 550
#define BTN_UP_HIGH 600
#define BTN_LEFT_LOW 2020
#define BTN_LEFT_HIGH 2060
#define BTN_SELECT_LOW 2960
#define BTN_SELECT_HIGH 3000
/* When starting the LCD, start with 16 columns and 2 rows. */

#define NUM_COL 16
#define NUM_ROW 2

#define SAVE_PIN 255

enum {
	right,
	left,
	up,
	down,
	select,
	none_pressed
}button_state;

void LCDInit(LiquidCrystal * A);

void LCDSPIInit(LiquidCrystal* plcd, uint8_t ssPin);

void LCDBegin(LiquidCrystal* plcd, uint8_t cols, uint8_t rows, uint8_t charsize);

void LCDClear(LiquidCrystal * plcd);

void LCDHome(LiquidCrystal * plcd);

void LCDNoDisplay(LiquidCrystal * plcd);

void LCDDisplay(LiquidCrystal * plcd);

void LCDNoBlink(LiquidCrystal * plcd);

void LCDBlink(LiquidCrystal * plcd);

void LCDNoCursor(LiquidCrystal * plcd);

void LCDCursor(LiquidCrystal * plcd);

void LCDScrollDisplayLeft(LiquidCrystal * plcd);

void LCDScrollDisplayRight(LiquidCrystal * plcd);

void LCDLeftToRight(LiquidCrystal * plcd);

void LCDRightToLeft(LiquidCrystal * plcd);

void LCDAutoScroll(LiquidCrystal * plcd);

void LCDNoAutoScroll(LiquidCrystal * plcd);

void LCDCreateChar(LiquidCrystal * plcd, uint8_t location, uint8_t charmap[]);

void LCDSetCursor(LiquidCrystal * plcd, uint8_t col, uint8_t row);

uint8_t LCDWrite(LiquidCrystal * plcd, uint8_t value);

void LCDCommand(LiquidCrystal * plcd, uint8_t value);

void LCDSend(LiquidCrystal * plcd, uint8_t value, uint8_t mode);

void LCDSPISendOut(LiquidCrystal * plcd);

void LCDWrite4Bits(LiquidCrystal * plcd, uint8_t value);

void LCDWrite8Bits(LiquidCrystal * plcd, uint8_t value);

void LCDPulseEnable(LiquidCrystal * plcd);

void DFR0009_InitParams(LiquidCrystal * A);

void LCDInit(LiquidCrystal * plcd);

void LCDPrint(LiquidCrystal * plcd, char * str);

void LCDMoveCursorLeft(LiquidCrystal * plcd);

void LCDMoveCursorRight(LiquidCrystal * plcd);

#endif /* LCD_DFR0009_H_ */
