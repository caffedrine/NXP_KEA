/*
 * LiquidCrystal.h
 *
 *  Created on: Oct 30, 2015
 *      Author: B48683
 */

#ifndef LIQUIDCRYSTAL_H_
#define LIQUIDCRYSTAL_H_

/* Could not find a C version of the LiquidCrystal library so making a custom one. */

#include "arduino.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for how many columns based on LCD display mode
#define LCD_NUM_COL_1LINE 0x50
#define LCD_NUM_COL_2LINES 0x28

/* This struct details where number a data pin is hooked and its state. */

struct lcd_pin{
	uint8_t num;
	uint8_t state;
};

typedef struct lcd_pin lcd_pin; //So don't have to write the "struct" every time

/* This struct declares information on LCD connections. */

struct LiquidCrystal{
	enum{ //bit mode
		eightbitmode=0,
		fourbitmode=1
	}BM;
	lcd_pin rs; //For register select bit. RS=0: Command, RS=1: Data
	lcd_pin rw; //Read/Write bit. RW=0: Write to LCD, RW=1: Read from LCD
	lcd_pin enable; //Enable bit. Falling edge-triggered so enable=0, you can write to LCD
	lcd_pin dpins[8]; //8 digital data pins
	lcd_pin apins[6]; //6 analog data pins
	enum { //SPI mode
		spi_off=0,
		spi_on=1
	}SM;
	enum {
		rightward,
		leftward
	}direction;
	uint8_t bitString; //for SPI  bit0=not used, bit1=RS, bit2=RW, bit3=Enable, bits4-7 = DB4-7
	lcd_pin latchPin;
	uint8_t clockDivider;
	uint8_t dataMode;
	uint8_t bitOrder;//SPI ####################################################

	uint8_t displayfunction;
	uint8_t displaycontrol;
	uint8_t displaymode;

	uint8_t initialized;
	enum {
		leftaligned,
		centeraligned,
		rightaligned
	}alignment;
	uint8_t numlines, numcol, currline, currcol;
	uint8_t leftedge, rightedge;
};

typedef struct LiquidCrystal LiquidCrystal;

/* Driver for LCD. A struct of function pointers. */

struct Driver_LiquidCrystal{
	void (*Init)(LiquidCrystal * plcd); //Specifies all the parameters such as pin outs
	void (*InitSPI)(LiquidCrystal * plcd, uint8_t ssPin); //Inits SPI if you choose to communicate to LCD using SPI instead of bitbanging the pins
	void (*Begin)(LiquidCrystal* plcd, uint8_t cols, uint8_t rows, uint8_t charsize); //Turns on the display, defaults to no cursor and no blink
	void (*Clear)(LiquidCrystal * plcd); //Clears the display of all characters
	void (*Home)(LiquidCrystal * plcd); //Resets display back to original position and cursor to DDRAM Address x00
	void (*NoDisplay)(LiquidCrystal * plcd); //Turns off display
	void (*Display)(LiquidCrystal * plcd); //Turns on display
	void (*NoBlink)(LiquidCrystal * plcd); //Turns off blink
	void (*Blink)(LiquidCrystal * plcd); //Turns on blink
	void (*NoCursor)(LiquidCrystal * plcd); //Turns off cursor
	void (*Cursor)(LiquidCrystal * plcd); //Turns on cursor
	void (*ScrollDisplayLeft)(LiquidCrystal * plcd); //Scrolls display to the left so that if you start out with a "Hello" with 'H' in DDRAM Address x00 (starting position) and then scroll left, you will see "ello"
	void (*ScrollDisplayRight)(LiquidCrystal * plcd); //Scrolls display to the right so that if you start out with a "Hello with 'H' in DDRAM Address x00 (starting position) and then scroll right, you will see " Hello"
	void (*MoveCursorLeft)(LiquidCrystal * plcd);
	void (*MoveCursorRight)(LiquidCrystal* plcd);
	void (*LeftToRight)(LiquidCrystal * plcd); //Prints left to right. Starting DDRAM Address x00
	void (*RightToLeft)(LiquidCrystal * plcd); //Prints right to left. Starting DDRAM Address x00.  Will print "Hello" as "olleH". Beware: if you start at DDRAM Address x00, your next character will be printed in last DDRAM Address, 0x67, i.e. last space of SECOND row
	void (*AutoScroll)(LiquidCrystal *plcd); //Right justifies text from cursor
	void (*NoAutoScroll)(LiquidCrystal * plcd); //Left justifies text from cursor
	void (*CreateChar)(LiquidCrystal * plcd, uint8_t location, uint8_t ch_array[]); //See explanation paragraph below
	void (*SetCursor)(LiquidCrystal * plcd, uint8_t col, uint8_t row); //Sets the cursor position. Starting DDRAM Address x00: col=0, row=0
	uint8_t (*Write)(LiquidCrystal* plcd, uint8_t data); //Sends an 8-bit value to LCD with RS=1 so it is understood as data
	void (*Command)(LiquidCrystal * plcd, uint8_t comm); //Sends an 8-bit value with RS=0 so it is a command
	void (*Send)(LiquidCrystal* plcd, uint8_t value, uint8_t mode); //Does the actual writing to LCD
	void (*SPISendOut)(LiquidCrystal * plcd); //Writes to LCD through SPI
	void (*Write4Bits)(LiquidCrystal * plcd, uint8_t value); //The bitbanging function that sets HIGH/LOW to the data pins in 4 bit mode. Writes to DB7:DB4
	void (*Write8Bits)(LiquidCrystal * plcd, uint8_t value); //In 8 bit mode. Same as Write4Bits but writes a whole byte. Some implementations do not allow 8-bit writes
	void (*PulseEnable)(LiquidCrystal * plcd); //A wait function for LCD to respond
	void (*Print)(LiquidCrystal * plcd, char * str); //Prints a string such as "Hello World!"
};

/* How to create custom char:
 * CreateChar let's you create a custom character. Each character block is either 5x8 or 5x10 so the character
 * array is uint8_t ch[8] and uint8_t ch[10], respectively where ch[i][7:5] always 0. Best leave ch[n-1] all 0's
 * b/c that's where cursor goes.  You can have up to 8 custom chars in 5x8 mode and 4 in 5x10 mode.
 * To create one in 5x8 mode, create the matrix for the custom char.  For example,
 * uint8_t ch[8]={0b00011111,
 * 					0b00010101
 * 					0b00001010,
 * 					0b00011100,
 * 					0b00000011,
 * 					0b00011000,
 * 					0b00000111,
 * 					0b00000000
 *					};
 *	1's represent that a dot is lit and 0 means it is not lit. Next, run the CreateChar function.
 *	Pass to it the LiquidCrystal *, the CGRAM Address, and the array ch.
 *	Once created, the characters are printed by writing data 0b0000_*000-0b0000*111. Let's say you want to create a character for
 *	DDRAM Data 0b0000*011, then your CGRAM Address that you would pass would be 0b011000.  The least significant 3 bits correspond
 *	to the lines of your character block where the highest line is address xxx000, and the eight line is xxx111.  Then write the value of
 *	each line of the character block ch as if you were writing data (i.e. LCD_Write(plcd, ch[i]) in a loop until all 8 lines filled.
 *	You do not need to respecify the CGRAM address every time you write a line of the character block ch, the CGRAM address increments
 *	automatically after every write so that once you LCD_Write(plcd, ch[0]) to 0b011000, then the next LCD_Write(plcd, ch[1]) writes to 0b011001
 *
 *	Once you have written information for the entire character block, run the function SetCursor and set to a DDRAM Address so that the LCD knows
 *	that your next write is to print characters again, instead of creating a new character in CGRAM.
 *
 *	In 5x10 mode, works the same except you can have up to 4 custom characters. DDRAM Data 0b0000*00* to 0b0000*11* and CGRAM address
 *	is 0b00xxxx to 0b11xxxx, where xxxx is your 4 bit line number identifier.  xxxx=0b1011 to xxxx=0b1111 are not used because 5x10 mode
 *	only requires 10 lines.
 */

typedef struct Driver_LiquidCrystal Driver_LiquidCrystal;

LiquidCrystal LCD_Init(LiquidCrystal plcd);

void LCD_Begin(LiquidCrystal* plcd, uint8_t col, uint8_t row, uint8_t dotsize);

void LCD_Clear(LiquidCrystal * plcd);

void LCD_Home(LiquidCrystal * plcd);

void LCD_SetCursor(LiquidCrystal * plcd, uint8_t col, uint8_t row);

void LCD_NoDisplay(LiquidCrystal * plcd);

void LCD_Display(LiquidCrystal * plcd);

void LCD_NoCursor(LiquidCrystal * plcd);

void LCD_Cursor(LiquidCrystal * plcd);

void LCD_NoBlink(LiquidCrystal * plcd);

void LCD_Blink(LiquidCrystal * plcd);

void LCD_ScrollDisplayLeft(LiquidCrystal * plcd);

void LCD_ScrollDisplayRight(LiquidCrystal * plcd);

void LCD_AutoScroll(LiquidCrystal * plcd);

void LCD_NoAutoScroll(LiquidCrystal *plcd);

void LCD_LeftToRight(LiquidCrystal *plcd);

void LCD_RightToLeft(LiquidCrystal * plcd);

void LCD_CreateChar(LiquidCrystal * plcd, uint8_t location, uint8_t charmap[]);

void LCD_Command(LiquidCrystal* plcd, uint8_t value);

uint8_t LCD_Write(LiquidCrystal* plcd, uint8_t value);

void LCD_Send(LiquidCrystal * plcd, uint8_t value, uint8_t mode);

void LCD_PulseEnable(LiquidCrystal * plcd);

void LCD_Write4Bits(LiquidCrystal * plcd, uint8_t value);

void LCD_Write8Bits(LiquidCrystal * plcd, uint8_t value);

void LCD_SPISendOut(LiquidCrystal * plcd);

void LCD_InitSPI(LiquidCrystal * plcd, uint8_t ssPin);

void bitWrite(LiquidCrystal * plcd, uint8_t bitpos, uint8_t value);

void LCD_Print(LiquidCrystal * plcd, char *str);

void LCD_MoveCursorLeft(LiquidCrystal * plcd);

void LCD_MoveCursorRight(LiquidCrystal * plcd);

void LCD_PosCheck(LiquidCrystal * plcd);

void LCD_DisplayReorient(LiquidCrystal * plcd);

#endif /* LIQUIDCRYSTAL_H_ */
