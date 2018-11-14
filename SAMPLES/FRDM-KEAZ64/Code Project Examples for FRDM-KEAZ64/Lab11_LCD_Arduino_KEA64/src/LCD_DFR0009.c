/*
 * DFR0009.c
 *
 *  Created on: Oct 30, 2015
 *      Author: B48683
 */

#include "derivative.h"
#include "LCD_DFR0009.h"
#include "LiquidCrystal.h"

/* This function populates lcd type with DFR0009 parameters. Consult DFR0009 schematic. */

Driver_LiquidCrystal driverlcd = { //These are the actual function prototypes for DFR0009 Driver
	LCDInit,
	LCDSPIInit,
	LCDBegin,
	LCDClear,
	LCDHome,
	LCDNoDisplay,
	LCDDisplay,
	LCDNoBlink,
	LCDBlink,
	LCDNoCursor,
	LCDCursor,
	LCDScrollDisplayLeft,
	LCDScrollDisplayRight,
	LCDMoveCursorLeft,
	LCDMoveCursorRight,
	LCDLeftToRight,
	LCDRightToLeft,
	LCDAutoScroll,
	LCDNoAutoScroll,
	LCDCreateChar,
	LCDSetCursor,
	LCDWrite,
	LCDCommand,
	LCDSend,
	LCDSPISendOut,
	LCDWrite4Bits,
	LCDWrite8Bits,
	LCDPulseEnable,
	LCDPrint
};

void DFR0009_InitParams(LiquidCrystal * plcd){
	LiquidCrystal templcd=*plcd; //declare a temporary local variable and initialize to exactly what input LiquidCrystal has
	templcd.BM=fourbitmode; //DFR0009 only has 4 digital data pins connected
	templcd.rs.num=RS; //RS is hooked to D8
	templcd.enable.num=E; //E is hooked to D9
	templcd.rw.num=RW; //RW is always wired to GND
	templcd.dpins[0].num=D0; //hooked to pin D4
	templcd.dpins[1].num=D1; //Pin D5
	templcd.dpins[2].num=D2; //Pin D6
	templcd.dpins[3].num=D3; //Pin D7
	*plcd=templcd; //Change the LiquidCrystal that we actually care about by deferenceing plcd and setting it to templcd+
}

/* Below is a custom function for initializing the LCD of DFR0009. */

void LCDInit(LiquidCrystal * plcd){
	LiquidCrystal temp=*plcd;
	temp=LCD_Init(*plcd); //Call the general function in LiquidCrystal.c
	*plcd=temp; //Update whatever LCD_Initialize changes to the LCD parameters
	/* Call the LCDBegin function. */
	driverlcd.Begin(plcd, NUM_COL, NUM_ROW, LCD_5x8DOTS);
}

/* Just calls the general LCD Begin function. */

void LCDBegin(LiquidCrystal* plcd, uint8_t col, uint8_t row, uint8_t dotsize){
	LCD_Begin(plcd, col, row, dotsize);
}

void LCDClear(LiquidCrystal * plcd){
	/* Call the general funciton. */
	LCD_Clear(plcd);
}

void LCDHome(LiquidCrystal * plcd){
	LCD_Home(plcd);
}

void LCDSetCursor(LiquidCrystal * plcd, uint8_t col, uint8_t row){
	LCD_SetCursor(plcd, col, row);
}

void LCDNoDisplay(LiquidCrystal * plcd){
	LCD_NoDisplay(plcd);
}

void LCDDisplay(LiquidCrystal * plcd){
	LCD_Display(plcd);
}

void LCDNoCursor(LiquidCrystal * plcd){
	LCD_NoCursor(plcd);
}

void LCDCursor(LiquidCrystal *plcd){
	LCD_Cursor(plcd);
}

void LCDNoBlink(LiquidCrystal * plcd){
	LCD_NoBlink(plcd);
}

void LCDBlink(LiquidCrystal * plcd){
	LCD_Blink(plcd);
}

void LCDScrollDisplayLeft(LiquidCrystal * plcd){
	LCD_ScrollDisplayLeft(plcd);
}

void LCDScrollDisplayRight(LiquidCrystal * plcd){
	LCD_ScrollDisplayRight(plcd);
}

void LCDAutoScroll(LiquidCrystal *plcd){
	LCD_AutoScroll(plcd);
}

void LCDNoAutoScroll(LiquidCrystal * plcd){
	LCD_NoAutoScroll(plcd);
}

void LCDLeftToRight(LiquidCrystal *plcd){
	LCD_LeftToRight(plcd);
}

void LCDRightToLeft(LiquidCrystal *plcd){
	LCD_RightToLeft(plcd);
}

void LCDCreateChar(LiquidCrystal * plcd, uint8_t location, uint8_t array[]){
	LCD_CreateChar(plcd, location, array);
}

void LCDCommand(LiquidCrystal* plcd, uint8_t value){
	LCD_Command(plcd, value);
}

uint8_t LCDWrite(LiquidCrystal * plcd, uint8_t value){
	return LCD_Write(plcd, value); //returns 1 means success
}

void LCDSend(LiquidCrystal * plcd, uint8_t value, uint8_t mode){
	LCD_Send(plcd, value, mode);
}

void LCDPulseEnable(LiquidCrystal * plcd){
	LCD_PulseEnable(plcd);
}

void LCDWrite4Bits(LiquidCrystal * plcd, uint8_t value){
	LCD_Write4Bits(plcd,value);
}

void LCDWrite8Bits(LiquidCrystal * plcd, uint8_t value){
	LCD_Write8Bits(plcd,value);
}

void LCDSPISendOut(LiquidCrystal * plcd){
	LCD_SPISendOut(plcd);
}

void LCDSPIInit(LiquidCrystal* plcd, uint8_t ssPin){
	LCD_InitSPI(plcd, ssPin);
}

void LCDPrint(LiquidCrystal* plcd, char * str){
	LCD_Print(plcd, str);
}

void LCDMoveCursorLeft(LiquidCrystal* plcd){
	LCD_MoveCursorLeft(plcd);
}

void LCDMoveCursorRight(LiquidCrystal * plcd){
	LCD_MoveCursorRight(plcd);
}
