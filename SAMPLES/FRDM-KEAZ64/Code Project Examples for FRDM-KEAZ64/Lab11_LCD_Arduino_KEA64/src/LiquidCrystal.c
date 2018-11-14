/*
 * LiquidCrystal.c
 *
 *  Created on: Oct 30, 2015
 *      Author: B48683
 */

/* Writing custom LiquidCrystal library in C because could only find incomplete ones in C++ online. */

#include "derivative.h"
#include "LiquidCrystal.h"

LiquidCrystal LCD_Init(LiquidCrystal plcd){
	/* Set the RS pin as output and specify which pin it is via board-specific Arduino config file. */
	pinMode((int)plcd.rs.num, OUTPUT);
	/* we can save 1 pin by not using RW. Indicate by passing 255 instead of pin */
	if (plcd.rw.num != 255) {
	  pinMode((int)plcd.rw.num, OUTPUT);
	}
	/* Set the Enable Pin to an output */
	pinMode((int)plcd.enable.num, OUTPUT);

	/* Check whether LCD running four bit mode or eight bit mode. */
	if(plcd.BM==fourbitmode){
	    plcd.displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	}else{
	    plcd.displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
	}
	//since in initSPI constructor we set _usingSPI to true and we run it first
	//from SPI constructor, we do nothing here otherwise we set it to false
	if(plcd.SM==spi_off){
		plcd.SM=spi_off;
	}else{
		plcd.SM=spi_on;
	}
	return plcd;
}

void LCD_Begin(LiquidCrystal* plcd, uint8_t col, uint8_t row, uint8_t dotsize){

	LiquidCrystal temp=*plcd; //Set a temporary variable and then any changes we make to temp we'll put back to plcd at the end
	temp.numcol=LCD_NUM_COL_1LINE; //Will reassign numcol if in 2 line mode
	/* Check whether user specified 1 or 2 rows to start with */
	if(row>1){
		temp.displayfunction |= LCD_2LINE;
		temp.numcol=LCD_NUM_COL_2LINES; //Columns can run off the LCD so there can be more than 16 columns
	}
	temp.numlines=row;
	temp.currline=0;
	temp.currcol=0;
	temp.leftedge=0; //The left edge of the screen starts at 0
	temp.rightedge=(col-1); //The last column starts out in column[col-1]
	/* For some 1 line displays you can select a 10 pixel high font. */
	if ((dotsize != 0) && (row == 1)) {
	  temp.displayfunction |= LCD_5x10DOTS;
	}
	/* SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	** According to datasheet, we need at least 40ms after power rises above 2.7V
	** Before sending commands. Arduino can turn on way before 4.5V so we'll wait 50 */
	delayMicroseconds(50000);
	/* Now we pull both RS and R/W low to begin commands. */
	digitalWrite(temp.rs.num, LOW);
	temp.rs.state=LOW; //Record the pin state
	digitalWrite(temp.enable.num, LOW);
	temp.enable.state=LOW;
	if (temp.rw.num != 255) {
	  digitalWrite(temp.rw.num, LOW);
	  temp.rw.state=LOW;
	}
	/* Put LCD in 4 or 8 bit mode. */
	if (! (temp.displayfunction & LCD_8BITMODE)){
	    // this is according to the hitachi HD44780 datasheet
	    // figure 24, pg 46

	    // we start in 8bit mode, try to set 4 bit mode
	    LCD_Write4Bits(&temp, 0x03);
	    delayMicroseconds(4500); // wait min 4.1ms

	    // second try
	    LCD_Write4Bits(&temp, 0x03);
	    delayMicroseconds(4500); // wait min 4.1ms

	    // third go!
	    LCD_Write4Bits(&temp, 0x03);
	    delayMicroseconds(150);

	    // finally, set to 4-bit interface
	    LCD_Write4Bits(&temp, 0x02);
	}else {
	    // this is according to the hitachi HD44780 datasheet
	    // page 45 figure 23

	    // Send function set command sequence
	    LCD_Command(&temp, LCD_FUNCTIONSET | temp.displayfunction);
	    delayMicroseconds(4500);  // wait more than 4.1ms

	    // second try
	    LCD_Command(&temp, LCD_FUNCTIONSET | temp.displayfunction);
	    delayMicroseconds(150);

	    // third go
	    LCD_Command(&temp, LCD_FUNCTIONSET | temp.displayfunction);
	  }
	// finally, set # lines, font size, etc.
	  LCD_Command(&temp, LCD_FUNCTIONSET | temp.displayfunction);

	  // turn the display on with no cursor or blinking default
	  temp.displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	  LCD_Display(&temp);

	  // clear it off
	  LCD_Clear(&temp);

	  // Initialize to default text direction (for romance languages)
	  temp.displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	  // set the entry mode
	  LCD_Command(&temp, LCD_ENTRYMODESET | temp.displaymode);

	  temp.direction=rightward; //Update the status flag for LCD going rightward
	  temp.alignment=leftaligned; //Because entry mode
	  temp.initialized=1; //This flag shows that LCD has been initialized

	*plcd=temp; //Update any changes you made to the LCD state after you passed it into this function
}

/********** high level commands, for the user! */

void LCD_Clear(LiquidCrystal * plcd){
	LiquidCrystal temp=*plcd;
	LCD_Command(plcd, LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
	delayMicroseconds(2000);  // this command takes a long time!
	/* Reset all the row/column states. */
	temp.currcol=0;
	temp.currline=0;
	temp.leftedge=0;
	temp.rightedge=0xF;
	*plcd=temp; //Update the changes

}

void LCD_Home(LiquidCrystal * plcd){
	LiquidCrystal temp=*plcd;
	LCD_Command(plcd, LCD_RETURNHOME);  // set cursor position to zero
	delayMicroseconds(2000);  // this command takes a long time!
	/* Reset all the row/column states. */
	temp.currcol=0;
	temp.currline=0;
	temp.leftedge=0;
	temp.rightedge=0xF;
	*plcd=temp; //Update the changes
}

void LCD_SetCursor(LiquidCrystal * plcd, uint8_t col, uint8_t row){
	LiquidCrystal temp=*plcd;
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	  if ( row > temp.numlines ) {
	    row = temp.numlines-1;    // we count rows starting w/0
	  }

	  LCD_Command(&temp, LCD_SETDDRAMADDR | (col + row_offsets[row]));
	  temp.currline=row; //Update what the current line the LCD is in
	  temp.currcol=col;

	  /* If the scroll goes off the screen, reorient. First check where left/rightedge are. Are they in the normal range or
	   * in the wrap-around area (recall: DDRAM address wraps around at 0x28, where leftedge>rightedge? */

	  if(temp.leftedge>temp.rightedge){ //Check if the screen is in the wrap around area
		  /* If the scrren is in the wrap around area, then the cursor is offscreen if the currcol
		   * is greater than the rightedge address but less than the leftedge address.
		   */

		  if((temp.currcol<temp.leftedge) && (temp.currcol>temp.rightedge)){
			  /* If it is offscreen, reorient the LCD screen to fit according to whatever alignment is chosen. */
			  LCD_DisplayReorient(&temp);
		  }

	  }else{ //Or else it is in the non-wrap-around range, where leftedge<rightedge
		  /* If cursor is in normal range, the cursor is offscreen if its Dcurrcol is less than leftedge or greater than right edge. */
		  if((temp.currcol<temp.leftedge)||(temp.currcol>temp.rightedge)){
		  	  LCD_DisplayReorient(&temp);
	  	  }
	  }

	  *plcd = temp; //Propogate that to the pointer
}

/* This function reorients the display after LCD_SetCursor such that the cursor is on the leftedge again. */

void LCD_DisplayReorient(LiquidCrystal * plcd){
	LiquidCrystal temp=*plcd;

	/* First check which alignment mode the LCD is in. */

	switch(temp.alignment){
	case leftaligned:
		/* Scroll right until left edge is the same as currcol. */
		while(temp.leftedge != temp.currcol){
			LCD_ScrollDisplayRight(&temp);
		}
		break;
	case centeraligned:
		/* Scroll right until right edge hits currcol, then scroll right a further ((rightedge-leftedge)%numcol)/2 times. */
		while(temp.rightedge != temp.currcol){
			LCD_ScrollDisplayRight(&temp);
		}
		/* This is the part where the display scrolls a further half-a-display's worth of times. */
		for(int i=0; i<(((temp.rightedge-temp.leftedge)%temp.numcol)/2) ; i++){
			LCD_ScrollDisplayRight(&temp);
		}
		break;
	case rightaligned:
		/* Scroll left until the right edge is the same as currcol. */
		while(temp.rightedge != temp.currcol){
			LCD_ScrollDisplayLeft(&temp);
		}
		break;
	default:
		break;
	}

	*plcd=temp; //Propagate the changes

}

// Turn the display on/off (quickly)

void LCD_NoDisplay(LiquidCrystal * plcd){
	LiquidCrystal temp=*plcd;
	temp.displaycontrol &= ~LCD_DISPLAYON;
	LCD_Command(&temp, LCD_DISPLAYCONTROL | temp.displaycontrol);
	*plcd=temp; //Update the actual LiquidCrystal struct
}

void LCD_Display(LiquidCrystal * plcd){
	LiquidCrystal temp=*plcd;
	temp.displaycontrol |= LCD_DISPLAYON;
	LCD_Command(&temp, LCD_DISPLAYCONTROL | temp.displaycontrol);
	*plcd=temp;
}

void LCD_NoCursor(LiquidCrystal * plcd){
	LiquidCrystal temp=*plcd;
	temp.displaycontrol &= ~LCD_CURSORON;
	LCD_Command(&temp, LCD_DISPLAYCONTROL | temp.displaycontrol);
	*plcd=temp;
}

void LCD_Cursor(LiquidCrystal *plcd){
	LiquidCrystal temp=*plcd;
	temp.displaycontrol |= LCD_CURSORON;
	LCD_Command(&temp, LCD_DISPLAYCONTROL | temp.displaycontrol);
	*plcd=temp;
}

void LCD_NoBlink(LiquidCrystal * plcd){
	LiquidCrystal temp=*plcd;
	temp.displaycontrol &= ~LCD_BLINKON;
	LCD_Command(&temp, LCD_DISPLAYCONTROL | temp.displaycontrol);
	*plcd=temp;
}

void LCD_Blink(LiquidCrystal * plcd){
	LiquidCrystal temp=*plcd;
	temp.displaycontrol |= LCD_BLINKON;
	LCD_Command(&temp, LCD_DISPLAYCONTROL | temp.displaycontrol);
	*plcd=temp;
}

// These commands scroll the display without changing the RAM


void LCD_PosCheck(LiquidCrystal * plcd){
	LiquidCrystal temp=*plcd;
	if(temp.direction==rightward){//increment currcol if LCD is in rightward mode (writes left to right)
		if(temp.currcol==(temp.numcol-1)){//column wraps around at address 0x28 and goes to second line
			temp.currcol=0;
			if(temp.currline==(temp.numlines-1)){ //at last column of last line, wraps back to home
				temp.currline=0;
			}else{ //otherwise increment the line
				temp.currline++;
			}
		}else{
			temp.currcol++; //If none of these corner cases, just increment the column number
		}
		/* If the cursor runs off the right edge, scoll the display accordingly. Enter this condition if currcol exceeds the
		 * rightedge index OR if currcol has exceeded to the point where the DDRAM Address rolls over, which means that
		 * rightedge=numcol (i.e. LCD_NUM_COL_2LINES or LCD_NUM_COL_1LINE, depending on the mode) AND currcol=0 (rollover). */

		if((temp.currcol-temp.rightedge==1)||((temp.rightedge==temp.numcol-1)&&(temp.currcol==0))){
			uint8_t alignment_old=temp.alignment; //save the old alignment setting
			temp.alignment=rightaligned; //temporarily change the alignment to right aligned because we want the cursor to keep showing on the right
			LCD_DisplayReorient(&temp);
			temp.alignment=alignment_old; //Reset the alignment setting to what it was
		}

	}else{ //otherwise it is in leftward mode
		if(temp.currcol==0){ //columns wraps around at what would be currcol=-1
			temp.currcol=(temp.numcol-1); //Set column counter to last column
			if(temp.currline==0){
				temp.currline=(temp.numlines-1); //if row counter already at top line, wrap around to last line
			}else{
				temp.currline--; //otherwise just move up a line
			}
		}else{
			temp.currcol--; //if none of these corner cases, just decrement the column counter
		}

		/* Implement the same display shift logic as in rightward mode, but in the other direction. */

		if((temp.currcol-temp.leftedge==-1) || ((temp.leftedge==0) && (temp.currcol==(temp.numcol-1)))){
			uint8_t alignment_old=temp.alignment;
			temp.alignment=leftaligned; //Temporarily change to leftaligned because we just want the cursor to keep showing on the left
			LCD_DisplayReorient(&temp);
			temp.alignment=alignment_old;
		}
	}
	*plcd=temp; //propagate these changes
}

void LCD_ScrollDisplayLeft(LiquidCrystal * plcd){
	LiquidCrystal temp=*plcd;
	LCD_Command(plcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT); //Shift the display
	temp.rightedge=((temp.rightedge+1)%temp.numcol); //update the display boundaries. Do right edge first
	temp.leftedge=((temp.leftedge+1)%temp.numcol); //then do left edge
	*plcd=temp; //Propagate the change
}

void LCD_ScrollDisplayRight(LiquidCrystal * plcd){
	LiquidCrystal temp=*plcd;
	LCD_Command(plcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
	//temp.rightedge=((temp.rightedge-1)%temp.numcol);
	//temp.leftedge=((temp.leftedge-1)%temp.numcol);
	temp.rightedge--; //0=1 in 8 bits is 255, which is 15d mod 0x28 so must implement the negative mod with a case statement
	if(temp.rightedge==255){
		temp.rightedge=(temp.numcol-1);
	}
	temp.leftedge--; //same for leftedge
	if(temp.leftedge==255){
		temp.leftedge=(temp.numcol-1);
	}
	*plcd=temp;
}


void LCD_MoveCursorLeft(LiquidCrystal * plcd){
	LiquidCrystal temp=*plcd;
	LCD_Command(plcd, LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVELEFT);
	if(temp.direction==rightward){ //if LCD in rightward mode, the cursor move function does not change direction of print
		temp.direction=leftward; //change it temporarily so PosCheck knows to decrement position
		LCD_PosCheck(&temp); //update the new position
		temp.direction=rightward; //change direction back to what it was
	}else{//If LCD prints going left and you moved cursor left, you don't need to temporarily switch directions
		LCD_PosCheck(&temp); //just update the new position
	}

	*plcd=temp; //propagate the change
}


void LCD_MoveCursorRight(LiquidCrystal * plcd){
	LiquidCrystal temp=*plcd;
	LCD_Command(plcd, LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVERIGHT);
	if(temp.direction==leftward){ //same logic as the if statement in LCD_MoveCursorLeft
		temp.direction=rightward;
		LCD_PosCheck(&temp); //update the new position
		temp.direction=leftward;
	}else{
		LCD_PosCheck(&temp); //update the new position
	}
	*plcd=temp; //propagate the change
}

// This will 'right justify' text from the cursor
void LCD_AutoScroll(LiquidCrystal *plcd){
	LiquidCrystal temp=*plcd;
	temp.displaymode |= LCD_ENTRYSHIFTINCREMENT;
	LCD_Command(&temp, LCD_ENTRYMODESET | temp.displaymode);
	temp.alignment=rightaligned; //Update the alignment status
	*plcd=temp;
}

// This will 'left justify' text from the cursor
void LCD_NoAutoScroll(LiquidCrystal * plcd){
	LiquidCrystal temp=*plcd;
	temp.displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	LCD_Command(&temp, LCD_ENTRYMODESET | temp.displaymode);
	temp.alignment=leftaligned; //update the alignment status
	*plcd=temp;
}


// This is for text that flows Left to Right
void LCD_LeftToRight(LiquidCrystal * plcd){
	LiquidCrystal temp=*plcd;
	temp.displaymode |= LCD_ENTRYLEFT;
	LCD_Command(&temp, LCD_ENTRYMODESET | temp.displaymode);
	temp.direction=rightward; //update the direction for LCD to go rightward
	temp.alignment=leftaligned; //change the alignment flag
	*plcd=temp;
}

// This is for text that flows Right to Left
void LCD_RightToLeft(LiquidCrystal * plcd){
	LiquidCrystal temp=*plcd;
	temp.displaymode &= ~LCD_ENTRYLEFT;
	LCD_Command(&temp, LCD_ENTRYMODESET | temp.displaymode);
	temp.direction=leftward; //update the direction for it to go leftward
	temp.alignment=rightaligned; //change the alignment
	*plcd=temp;
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters

void LCD_CreateChar(LiquidCrystal * plcd, uint8_t location, uint8_t charmap[]){
	location &= 0x7; // we only have 8 locations 0-7
	LCD_Command(plcd, LCD_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++) {
	  LCD_Write(plcd, charmap[i]);
	}
}

/*********** mid level commands, for sending data/cmds */

inline void LCD_Command(LiquidCrystal* plcd, uint8_t value){
	LCD_Send(plcd, value, LOW);
}

inline uint8_t LCD_Write(LiquidCrystal * plcd, uint8_t value){
	LiquidCrystal temp=*plcd;
	LCD_Send(plcd, value, HIGH);

	/* Everytime the LCD prints, the cursor position updates in the LCD RAM. Record this change in the status flag temp.direction. */
	LCD_PosCheck(&temp);
	*plcd=temp; //update these changes
	return 1; //Assume success
}

/* This function prints a string. */

void LCD_Print(LiquidCrystal* plcd, char * str){
	uint32_t length=0; //Gets the length of the string

	/* If you get an empty string, print an empty message on LCD to that effect. Recursive. */
	if(*str=='\0'){
		LCD_Print(plcd, "There is nothing to print!");
		return;
	}

	/* Look for the null to set the length. */

	for(uint32_t i=0; *(str+i)!='\0'; i++){
		length++;
	}
	/* Now print it. */
	for(uint32_t i=0; i<length; i++){
		LCD_Write(plcd, *(str+i));
	}
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection

void LCD_Send(LiquidCrystal * plcd, uint8_t value, uint8_t mode){
	LiquidCrystal temp=*plcd;
	if (temp.SM == spi_off)
	{
	  digitalWrite(temp.rs.num, mode);
	  //Then update that pin state
	  temp.rs.state=mode;

	    // if there is a RW pin indicated, set it low to Write
	  if (temp.rw.num != 255) {
	    digitalWrite(temp.rw.num, LOW);
	    temp.rw.state=LOW;
	  }

	  if (temp.displayfunction & LCD_8BITMODE) {
	    LCD_Write8Bits(&temp, value);
	  } else {
	    LCD_Write4Bits(&temp, value>>4);
	    LCD_Write4Bits(&temp, value);
	  }
	}
	else //we use SPI  ##########################################
	{
	  bitWrite(&temp, 1, mode); //set RS to mode. bit1=RS
	  LCD_SPISendOut(&temp);
	  //update the rs pin state
	  temp.rs.state=mode;

	//we are not using RW with SPI so we are not even bothering
	//or 8BITMODE so we go straight to write4bits
	  LCD_Write4Bits(&temp, value>>4);
	  LCD_Write4Bits(&temp, value);
	}
	*plcd=temp; //Give the actual pointer the new state
}

void LCD_PulseEnable(LiquidCrystal * plcd){
	LiquidCrystal temp=*plcd;
	if (temp.SM == spi_off)
	{
	  digitalWrite(temp.enable.num, LOW);
	  temp.enable.state=LOW; //Keep track of the states
	  delayMicroseconds(1);
	  digitalWrite(temp.enable.num, HIGH);
	  temp.enable.state=HIGH;
	  delayMicroseconds(1);    // enable pulse must be >450ns
	  digitalWrite(temp.enable.num, LOW);
	  delayMicroseconds(100);   // commands need > 37us to settle
	  temp.enable.state=LOW;
	}
	else //we use SPI #############################################
	{
	  bitWrite(&temp, 3, LOW); //bit 3=enable
	  temp.enable.state=LOW; //Keep track of the states
	  LCD_SPISendOut(&temp);
	  delayMicroseconds(1);
	  bitWrite(&temp, 3, HIGH);
	  temp.enable.state=HIGH;
	  LCD_SPISendOut(&temp);
	  delayMicroseconds(1);    // enable pulse must be >450ns
	  bitWrite(&temp, 3, LOW);
	  temp.enable.state=LOW;
	  LCD_SPISendOut(&temp);
	  delayMicroseconds(40);   // commands need > 37us to settle
	}
	*plcd=temp; //Update the changes
}

void LCD_Write4Bits(LiquidCrystal * plcd, uint8_t value){
	LiquidCrystal temp=*plcd;
	if (temp.SM == spi_off)
	{
	  for (int i = 0; i < 4; i++) {
	    pinMode(temp.dpins[i].num, OUTPUT);
	    digitalWrite(temp.dpins[i].num, (value >> i) & 0x01);
	    temp.dpins[i].state=((value>>i)&0x01); //update the value of the pin
	  }
	}
	else //we use SPI ##############################################
	{
	  for (int i = 4; i < 8; i++)
	  {
		//we put the four bits in the _bit_string
		bitWrite(&temp, i, ((value >> (i - 4)) & 0x01));
		temp.dpins[i-4].state=((value>>(i-4))&0x01); //update the value of each pin
	  }
		//and send it out
	  LCD_SPISendOut(&temp);
	}
	LCD_PulseEnable(&temp);
	*plcd=temp; //save the changes
}

void LCD_Write8Bits(LiquidCrystal * plcd, uint8_t value){
	LiquidCrystal temp=*plcd;
	for (int i = 0; i < 8; i++) {
	  pinMode(temp.dpins[i].num, OUTPUT);
	  digitalWrite(temp.dpins[i].num, (value >> i) & 0x01);
	  temp.dpins[i].state=((value>>i)&0x01); //update the pin state
	}

	LCD_PulseEnable(&temp);
	*plcd=temp; //save the changes
}

void LCD_SPISendOut(LiquidCrystal * plcd){
	//just in case you are using SPI for more then one device
	//set bitOrder, clockDivider and dataMode each time
	LiquidCrystal temp=*plcd;
	SPI.setClockDivider(temp.clockDivider);
	SPI.setBitOrder(temp.bitOrder);
	SPI.setDataMode(temp.dataMode);

	digitalWrite(temp.latchPin.num, LOW);
	temp.latchPin.state=LOW;
	SPI.transfer(temp.bitString);
	digitalWrite(temp.latchPin.num, HIGH);
	temp.latchPin.state=LOW;
	*plcd=temp;
}

void LCD_InitSPI(LiquidCrystal * plcd, uint8_t ssPin){
    // initialize SPI:
	LiquidCrystal temp=*plcd;
	temp.SM = spi_on;
	temp.latchPin.num = ssPin;
	pinMode (temp.latchPin.num, OUTPUT); //just in case _latchPin is not 10 or 53 set it to output
								 //otherwise SPI.begin() will set it to output but just in case

	SPI.begin();

	//set clockDivider to SPI_CLOCK_DIV2 by default which is 8MHz
	temp.clockDivider = SPI_CLOCK_DIV2;
	SPI.setClockDivider(temp.clockDivider);

	//set data mode to SPI_MODE0 by default
	temp.dataMode = SPI_MODE0;
	SPI.setDataMode(temp.dataMode);

	//set bitOrder to MSBFIRST by default
	temp.bitOrder = MSBFIRST;
	SPI.setBitOrder(temp.bitOrder);
	*plcd=temp;
}

void bitWrite(LiquidCrystal * plcd, uint8_t bitpos, uint8_t value){
	LiquidCrystal temp=*plcd; //use a temporary struct to work with
	if(value==0){ //if the value is 0, then clear the corresponding bit
		temp.bitString &= ~(1<<bitpos);
	}else{ //if the value of that bit is 1, set the corresponding bit
		temp.bitString |= 1<<bitpos;
	}
	*plcd=temp; //propogate the change to bitString
}
