/*
 * lcd_routines.c
 *
 *  Created on: Nov 9, 2015
 *      Author: B48683
 */

#include "lcd_routines.h"
#include <stdlib.h>
#include <string.h>

#define BACKSPACE 0b01111111 //ASCII value of backspace
#define BLANK 0b00100000 //ASCII value of a blank space
#define MAX_ADC_DIGITS 4 //ADC up to 12 bit precision, so that is 4096, four digits
#define MAX_ARITHMETIC_LENGTH 10 //Specified simple arithmetic of two operands each of max 4 digits. 2 numbers of 4 digits plus an operator and a '\r' makes 10 characters
#define MAX_ANSWER_LENGTH 10
#define MAX_OPERAND_DIGITS 5 //Arithmetic function supports arithmetic between up to 9999, plus one digit for null terminator in itoa function
#define MAX_REMAINDER_LENGTH 3 //Three decimal places

extern Driver_LiquidCrystal driverlcd;

extern char char_received;

uint8_t ascii_adc[MAX_ADC_DIGITS]={0}; //Used to convert integer to ASCII representation. ADC is at most 12-bit precision, meaning max adc reading is 4096. One array element per digit

uint8_t ascii_math[MAX_ARITHMETIC_LENGTH]={0};

uint8_t first_operand[MAX_OPERAND_DIGITS]={0};

uint8_t second_operand[MAX_OPERAND_DIGITS]={0};

uint8_t ascii_result[MAX_ANSWER_LENGTH]={0}; //The maximum value of the arithmetic function is 9999*9999, which can be expressed in 8 digits in base 10. Giving 2 extra spaces just in case

uint8_t ascii_remainder[MAX_REMAINDER_LENGTH]={0};

uint8_t lcd_mode_old=0;

/* This function echos whatever is printed on the hyperterminal. */

void Echo(LiquidCrystal * plcd){
	static uint8_t ch=0;
#if defined(MCU_SKEAZ1284) || defined(MCU_SKEAZN642)
	Driver_UART2.Receive(&ch, 1); //Receive a character through the UART

	//Driver_UART2.Send(&ch, 1); //echo it on the hyperterminal
#else
	/* Capture the data from the appropriate UART for your device. */
#endif

	/* Only print if that character was valid and transmitted to the hyperterminal. */

	//if(UART2_S1 & UART_S1_TC_MASK){ //Only when transmission of echoed character has been completed, print on LCD

	/* If you're echoing whatever you type in the hyperterminal, all backspaces (0b0111_1111) should delete the previous character, not
	 * print a '<-'. This is the effect of the case statement.
	 */
	if(char_received==1){
		if(ch==BACKSPACE){//If a backspace is detected, do not print a '<-'. Instead, scroll left one space then print a blank space.
			driverlcd.MoveCursorLeft(plcd);
			driverlcd.Write(plcd, BLANK);

			/* Since the blank is technically a write, the cursor automatically increments so move the cursor left one space again. */

			driverlcd.MoveCursorLeft(plcd);

		}else{ //If not a backspace, just print a the value received
			driverlcd.Write(plcd, ch); //Print the character on the LCD
		}
		char_received=0;
#if defined(MCU_SKEAZ1284) || defined(MCU_SKEAZN642)
		Driver_UART2.Send(&ch, 1); //Send the character to the hyperterminal
#else
#endif
	}
}

/* This function prints the ADC value sensed from the onboard potentiometer. */

void ADC_Show(LiquidCrystal * plcd){
	int temp=0;
	/* Clear the ASCII array ascii_adc every time. */

	for(int i=0; i<MAX_ADC_DIGITS; i++){
		ascii_adc[i]=0;
	}
	driverlcd.Clear(plcd); //Clear the screen every time
#if defined(MCU_SKEAZ1284) || defined(MCU_SKEAZN642)
	Driver_ADC0.SetChannel(10); //Potentiometer connects to channel 10 (ADC0_SE10)
	temp=Driver_ADC0.Read(); //Read the ADC Value
#else
	/* Read the potentiometer from the appropriate ADC channel of your device. */
#endif
	/* Must convert integer read to ASCII values */
	itoa(temp, ascii_adc);

	/* Print the ASCII digits. */
	driverlcd.Print(plcd, "ADC Value: ");
	for(int i=3; i>=0; i--){ //From itoa, the digits are recorded backwards in ascii_adc such that ascii_adc[0]=ones, ascii_adc[1]=tens, and so on
		if(ascii_adc[i]!='\0'){
			driverlcd.Write(plcd,(uint8_t)ascii_adc[i]); //But we read highest digit first so print the array backwards
		}
	}

	delayMicroseconds(80000);
}


/* This function converts an integer value into an ASCII string representing that value. */

void itoa(int32_t temp, uint8_t array[]){
	/* If the ADC value is 0, turn the 0 into ASCII and return. */
	int i=0;
	bool negative_flag=false;
	if(temp==0){
		array[0]=temp+48;
	}else{
		/* Check whether the result is positive or negative. If negative, turn value to positive and set flag so that
		 * we can remember to add a '-' in front of most significant digit. Then parse the rest normally.
		 */
		if(temp<0){
			temp=temp*-1; //invert the negative number
			negative_flag=true;
		}
		while(temp!='\0'){
			array[i]=(temp%10)+48; //Get the lowest digit and add 48 to convert to ASCII
			temp=(uint32_t)(temp/10); //Get the next digit and truncate to make sure it stays an int
			i++;
		}
		/* Add a negative sign in front of the most significant digit if number is negative. */
		if(negative_flag==true){
			array[i]='-';
		}
	}
}

/* This is the first message that prints when you start up.  Prints the message then it floats on the LCD, since
 * message goes offscreen (longer than 16 characters).
 */

void msg(LiquidCrystal * plcd){
	LiquidCrystal temp=*plcd;
	uint8_t strlen=0; //local variable for how long the string is
	//driverlcd.Clear(&temp);
	driverlcd.Print(&temp, "NXP FRDM KEA! Press 'Select'");
	driverlcd.SetCursor(&temp, 0,1); //Print the rest of the message on the next line
	driverlcd.Print(&temp, "for other functions!");
	strlen=28; //This is the length of the longest segment of the message (first line)
	delayMicroseconds(50000); //Wait 50ms
	while(lcd_mode==message){
		DisplayBounce(&temp, strlen);
	}
	*plcd=temp;
}

/* This function moves the display such that a whole string is shown if it runs offscreen. Resets cursor to home position. */

void DisplayBounce(LiquidCrystal * plcd, uint8_t strlen){
	LiquidCrystal temp=*plcd;
	uint8_t window_width=0;

	/* If the string is longer than the number of columns (i.e. it has to be displayed on 2 lines, then set strlen to the max
	 * column count because we can only scroll that much.
	 */
	if(strlen>temp.numcol){
		strlen=temp.numcol;
	}

	/* Depending on what direction the message printed in, Set cursor and display to home. */

	driverlcd.Home(&temp);

	/* At the same time, you don't need to scroll the display the entire strlen number of times.  The display needs to scroll
	 * just enough such that all character show.  So if you have a string of 20 characters on a display that is 16 characters
	 * wide, you only need to scroll 4 times, not all 16.
	 */

	/* How window_width is calculated depends on whether the cursor is in the wrap-around region. In the wrap-around region
	 * leftedge>rightedge
	 */

	if(temp.leftedge>temp.rightedge){
		/* In the wrap around region, the window width is the distance from leftedge to numcol (i.e. end of DDRAM Addresses)
		 *  plus the distance from 0x00 to rightedge. */
		window_width=temp.rightedge+(temp.numcol-temp.leftedge);
	}else{ //In normal region, window width is imply rightedge-leftedge
		window_width=temp.rightedge-temp.leftedge;
	}


	/* If in rightward mode, start at home address.  Scroll right strlen-window_width counts, then scroll back the same.  strlen-window_width
	 * for reason illustrated in comment about with example.
	 */
	if(strlen>window_width){
		if(temp.direction==rightward){
			/* Scroll characters right. */
			for(int i=0; i<strlen-window_width; i++){
				driverlcd.ScrollDisplayLeft(&temp);
				delayMicroseconds(150000); //Wait 50ms between each scroll

				/* If at any point the user pressed Select and switched modes, clear the screen,
				 * go home, and exit this routine.
				 */
				if(lcd_mode_old!=lcd_mode){
					driverlcd.Clear(&temp);
					driverlcd.Home(&temp);
					*plcd=temp;
					return;
				}
				/* If a character has been received through the UART, stop scrolling, but don't clear display. */
				if(char_received==1){
					*plcd=temp;
					return;
				}
			}

			/* Scroll characters left. */
			for(int i=0; i<strlen-window_width; i++){
				driverlcd.ScrollDisplayRight(&temp);
				delayMicroseconds(150000);

				if(lcd_mode_old!=lcd_mode){
					driverlcd.Clear(&temp);
					driverlcd.Home(&temp);
					*plcd=temp;
					return;
				}

				if(char_received==1){
					*plcd=temp;
					return;
				}
			}
		}else{ //If not rightward, it is printing leftwards, so do the same thing as rightward condition but in opposite direction
			for(int i=0; i<strlen-window_width; i++){
				driverlcd.ScrollDisplayRight(&temp);
				delayMicroseconds(150000);
				if(lcd_mode_old!=lcd_mode){
					driverlcd.Clear(&temp);
					driverlcd.Home(&temp);
					*plcd=temp;
					return;
				}

				if(char_received==1){
					*plcd=temp;
					return;
				}
			}
			for(int i=0; i<strlen-window_width; i++){
				driverlcd.ScrollDisplayLeft(&temp);
				delayMicroseconds(150000);
				if(lcd_mode_old!=lcd_mode){
					driverlcd.Clear(&temp);
					driverlcd.Home(&temp);
					*plcd=temp;
					return;
				}

				if(char_received==1){
					*plcd=temp;
					return;
				}
			}
		}
	}
	driverlcd.Home(&temp); //This function is to reset to Home if there is a mode switch while BounceDisplay is happening
	*plcd=temp;
}

/* This function does simple arithmetic on two terms (e.g. 2+3) from the UART and prints the answer on the LCD. */

void math(LiquidCrystal* plcd){
	LiquidCrystal temp=*plcd;
	uint8_t i=0, counter=0;
	char ch=0;
	int32_t operand1=0, operand2=0;
	int32_t result=0, remainder=0;
	char newline[]={"\r\n"};

	/* Clear the ASCII array. */

	for(int j=0; j<MAX_ARITHMETIC_LENGTH; j++){
		ascii_math[j]=0;
	}

	/* Clear the result array as well. */

	for(int j=0; j<MAX_ANSWER_LENGTH; j++){
		ascii_result[j]=0;
	}

	/* Clear the remainder array. */

	for(int j=0; j<MAX_REMAINDER_LENGTH; j++){
		ascii_remainder[j]=0;
	}

	/* And clear the operand arrays. */

	for(int j=0; j<MAX_OPERAND_DIGITS; j++){
		first_operand[j]=0;
		second_operand[j]=0;
	}

	/* Collect the characters. Keep scanning until you see an enter or until you run out of room in the ASCII array. */
	while((ch!=CR) && (i<MAX_ARITHMETIC_LENGTH) && (ch!='.')){//CR stands for Carriage Return aka Enter key. And no decimals. Only integers
#if defined(MCU_SKEAZ1284) || defined(MCU_SKEAZN642)
		Driver_UART2.Receive(&ch, 1);
		//Driver_UART2.Send(&ch, 1); //Echo on UART so user can see what they typed on the terminal
#else
		/* Read from/send to the appropriate UART for your device. */
#endif
		/* Take only valid characters 0-9 and +, -, *, /. */
		if(char_received==1){
			if((ch=='0') ||(ch=='1') || (ch=='2') || (ch=='3') || (ch=='4') || (ch=='5') || (ch=='6') || (ch=='7') || (ch=='8') || (ch=='9')
					|| (ch=='-') || (ch=='+') || (ch=='*') || (ch=='/')){

				ascii_math[i]=ch;
				i++;
			}
			char_received=0;
#if defined(MCU_SKEAZ1284) || defined(MCU_SKEAZN642)
			Driver_UART2.Send(&ch, 1);
#else
#endif
		}
		/* If there is a mode switch before the data gathering is finished, just break out of the function. */
		if(lcd_mode!=arithmetic){
			*plcd=temp; //Record whatever might have changed so far into the function
			return;
		}
	}

	/* If we exited the loop because it got a '.', then print error message. No decimals allowed! */

	if(ch=='.'){
		math_fail(&temp);
		*plcd=temp;
		return;
	}


	/* Reset i. */

	i=0;

	/* A valid equation requires that a user enter his/her equation and then press enter. So you would expect the last character to be a CR.
	 * Therefore, you would expect ch=CR right after the while loop above.
	 */

	if(ch!=CR){ //If the final character is not an Enter then it is not a valid equation
		math_fail(&temp);
		*plcd=temp; //update the LCD struct pointer
		return;
	}

	/* Now that you have the array of numbers, scan to see if it is a valid equation.
	 * First scan for the operator: '+, '-', '*', or '/' */

	while(i<MAX_ARITHMETIC_LENGTH){
		if((ascii_math[i]=='+') || (ascii_math[i]=='-') || (ascii_math[i]=='*') || (ascii_math[i]=='/')){
			break;
		} //If there is one of the these operators in the equation, the index i of the first operator found will be the dividing line between the two operands
		i++;
	}

	/* If index i goes through the whole array without finding an operator, then it is not a valid equation. Or if the index of the operator is greater than 4
	 * which means the first operand is greater than 4 digits, it is not a valid equation. Or if the operator index is 0, that means
	 * the equation is something like "+2", which doesn't make sense.  Or if there is nothing after the operator, meaning an equation
	 * like a "2+"
	 */

	if((i==MAX_ARITHMETIC_LENGTH) ||(i>MAX_OPERAND_DIGITS-1) || (i==0) || (ascii_math[i+1]=='\0')){
		math_fail(&temp);
		*plcd=temp;
		return;
	}

	/* Now that we have found the first operator, we must make sure there are no other operators following.
	 * We use the first operator found as the operation to be performed.  If there are subsequent operators then they are invalid.
	 * Starting on character after first operator.  If another operator found, print error message.
	 */

	for(int j=i+1; j<MAX_ARITHMETIC_LENGTH; j++){
		if((ascii_math[j]=='+') || (ascii_math[j]=='-') || (ascii_math[j]=='*') || (ascii_math[j]=='/')){
			math_fail(&temp);
			*plcd=temp; //update the struct
			return;
		}
	}

	/* Check the second operand to make sure it's not too long. */

	for(int j=i+1; j<MAX_ARITHMETIC_LENGTH; j++){
		if(ascii_math[j] != '\0'){ //count the number of non-null digits are in the second operand
			counter++;
		}
	}

	/* After going through the second operand, if the counter is greater than 4 that means the second operand is longer than 4 digits, so
	 * invalid. Print error message.
	 */

	if(counter>(MAX_OPERAND_DIGITS-1)){ //MAX_OPERAND_DIGITS-1 because first_operand and second_operand arrays, which use this macro as a length indicator require a null terminator for itoa
		math_fail(&temp);
		*plcd=temp;
		return;
	}

	/* If the equation is valid, separate the two operands. */

	for(int j=0; j<i; j++){ //Start with the first operand. Its last character is right before the first operator at index i
		first_operand[j]=(ascii_math[j]); //Put the digits of the first operand into the first_operand array
	}

	/* Next get the second operand. Second character starts at the index after the
	 * operator and can run up to the end of the array, but not necessarily. Spots that are not populated
	 * have a null. When the array starts to be nulls, that means it's the end of the second operand
	 */

	for(int j=i+1; (j<MAX_ARITHMETIC_LENGTH) && (ascii_math[j]!='\0'); j++){
		second_operand[j-i-1]=(ascii_math[j]); //The j-i-1 is to negate the fact that j starts at i+1 while the second operand index should start at 0
	}

	/* Now that we have the two operands separated, convert them to single integers. */

	operand1=atoi((const char*)first_operand); //atoi function exists in cstlib in S32DS, but not itoa, so don't have to write atoi myself
	operand2=atoi((const char*)second_operand); //do the same for the second operand

	/* Now perform the appropriate function based on operator found */

	if(ascii_math[i]=='+'){
		result=operand1+operand2;
		Print_Result(&temp, result, remainder);
	}else if(ascii_math[i]=='-'){
		result=operand1-operand2;
		Print_Result(&temp, result, remainder);
	}else if(ascii_math[i]=='*'){
		result=operand1*operand2;
		Print_Result(&temp, result, remainder);
	}else{ //or else it is a division, which is trickier because you may have to deal with decimals

		/* Print error message if division by 0. */

		if(operand2==0){
			math_fail(&temp);
			*plcd=temp;
			return;
		}
		result=(int32_t)operand1/operand2; //This gets the truncated version of the quotient

		/* If there is a remainder, get it up to 2 decimal places. */

		if(operand1%operand2!=0){ //If the modulo does not equal 0 then there is a remainder
			remainder=(int32_t)(operand1*1000)/operand2; //The thousandths place is not printing; only for rounding of hundredths place
		}
		Print_Result(&temp, result, remainder);
	}
#if defined(MCU_SKEAZ1284) || defined(MCU_SKEAZN642)
	Driver_UART2.Send(newline,2); //Create a new line
#else
	/* Send out to appropriate UART for your device. */
#endif

	*plcd=temp;

}

/* This function is to print a message notifying user if there is an invalid equation inputed. */

void math_fail(LiquidCrystal * plcd){
	if(lcd_mode==arithmetic){ //Only fret about equations if you're in the right mode
		LiquidCrystal temp=*plcd;
		char fail_msg[]={"\n\rThe equation you entered is invalid! Please try again.\n\r"};
		int cnt=strlen(fail_msg); //Get the length of the failure message

		/* Print a message on the LCD. */
		driverlcd.Clear(&temp);
		driverlcd.Print(&temp, "Not Valid");

		/* Print a message on the hyperterminal. */
#if defined(MCU_SKEAZ1284) || defined(MCU_SKEAZN642)
		Driver_UART2.Send(fail_msg, cnt);
#else
		/* Send to appropriate UART for your device. */
#endif

		*plcd=temp;
	}

}

/* This function prints the result of the arithmetic function onto the LCD screen. */

void Print_Result(LiquidCrystal* plcd, int32_t result, int32_t remainder){
	LiquidCrystal temp=*plcd;
	driverlcd.Clear(&temp);

	/* Calculate the remainder first in case there is any carrying to do. */

	if(remainder!=0){

		itoa(remainder, ascii_remainder);

		/* With decimals you include all leading zeroes.  For example if the variable remainder is 234, then the decimal is .234.
		 * If remainder is 340, then decimal is .34; don't have to print the '0' in the thousandths place.
		 * But if the variable remainder is just 34, then the decimal is .034, not .34, so the zero must be printed
		 * In the ascii_remainder, turn all nulls before the most significant digit to character zeroes.
		 */
		/* Start at the end of the array ascii_remainder (i.e. the tenths place and scan so long as the
		 * contents are nulls and replace them with character zeroes.
		 */
		for(int i=MAX_REMAINDER_LENGTH-1; (ascii_remainder[i]=='\0') && (i>=0); i--){
			ascii_remainder[i]='0';
		}

		/* Use value of thousandths place to determine whether to round the hundredths place up. */

		if(ascii_remainder[0]>='5'){ //Round up if thousandths place is 5 or greater
			/* If the hundredths place is a '9' carry the one to the tenths. If the decimal is .99, carry the to the integer result. */
			if((ascii_remainder[1]=='9') && (ascii_remainder[2]=='9')){
				result++;
				ascii_remainder[1]='0';
				ascii_remainder[2]='0';
				remainder=0;
			}else if(ascii_remainder[1]=='9'){
				ascii_remainder[2]++;
				ascii_remainder[1]='0';
			}else{
				ascii_remainder[1]++;
			}
		}
	}

	/* Now get the result. */

	itoa(result, ascii_result); //convert the result back to ASCII
	/* If there is a remainder, convert that too. */
	for(int i=MAX_ANSWER_LENGTH-1; i>=0; i--){ //From itoa, the digits are recorded backwards in ascii_result such that ascii_result[0]=ones, ascii_result[1]=tens, and so on
		if(ascii_result[i]!='\0'){
			driverlcd.Write(plcd,(uint8_t)ascii_result[i]); //But we read highest digit first so print the array backwards
		}
	}

	/* Print the remainder if necessary. */
	if(remainder!=0){
		/* Print the decimal point. */
		driverlcd.Print(&temp, ".");
		/* Excluding thousandths, hence array does not print element 0*/
		for(int i=MAX_REMAINDER_LENGTH-1; i>=1; i--){
			if(ascii_remainder[i]!='\0'){
				driverlcd.Write(plcd,(uint8_t)ascii_remainder[i]); //But we read highest digit first so print the array backwards
			}
		}
	}
	*plcd=temp;
}

/* This function takes a question from the hyperterminal and returns a generic "fortune" message on LCD screen. */

void fortune_teller(LiquidCrystal* plcd){
	LiquidCrystal temp=*plcd;
	static uint8_t fortune_length; //this is a flag to determine whether display needs to shift to accomodate fortune length
	static uint8_t ch_previous;
	char fortune_fail[]={"\n\rThat is not a valid question! Try again.\n\r"};
	char fortune_success[] = {"\n\rLook at the LCD screen for your fortune!\n\r"};
	static uint8_t ch; //For receiving/sending char from/to UART terminal
#if defined(MCU_SKEAZ1284) || defined(MCU_SKEAZN642)
	Driver_UART2.Receive(&ch, 1);
#else
#endif
	if(char_received==1){
		char_received=0; //reset the flag for next time
#if defined(MCU_SKEAZ1284) || defined(MCU_SKEAZN642)
		Driver_UART2.Send(&ch, 1); //Send out character once it has been received
#else
#endif
		/* Only respond if Enter key has been pressed. */
		if(ch==CR){
			/* If the last character in the sentence is not a '?' then it's not a valid question. */
			if(ch_previous!='?'){
#if defined(MCU_SKEAZ1284) || defined(MCU_SKEAZN642)
				Driver_UART2.Send(fortune_fail, strlen(fortune_fail));
#else
#endif
				driverlcd.Clear(&temp);
				driverlcd.Print(&temp, "Sorry. :(");
				fortune_length=strlen("Sorry. :(");
			}else{ //Or else it is a true question
				/* Print on the UART terminal an instruction telling user to look at the LCD. */
#if defined(MCU_SKEAZ1284) || defined(MCU_SKEAZN642)
				Driver_UART2.Send(fortune_success, strlen(fortune_success));
#else
#endif
				fortune_length=print_fortune(&temp);
			}
		}
		ch_previous=ch; //ch_previous for purpose of seeing if the question is actually a question
	}
	if(fortune_length>15){
		DisplayBounce(&temp, fortune_length);
	}
	*plcd=temp;
}

/* This function prints a random fortune on the LCD screen. */

uint8_t print_fortune(LiquidCrystal* plcd){
	LiquidCrystal temp=*plcd;
	char fortune0[]={"The stars are with you."};
	char fortune1[]={"Maybe someday."};
	char fortune2[]={"Definitely."};
	char fortune3[]={"It is certain."};
	char fortune4[]={"Without a doubt."};
	char fortune5[]={"Ask me later."};
	char * fortuneptr=0;
	/* Generate a random number to pick a fortune message. Currently there are 6.
	 * rand stdlib function randomly generates a number between 0 and RAND_MAX.
	 * RAND_MAX is library dependent but is guaranteed to be at least 32767.
	 */
	uint32_t number=((uint32_t)rand()%6); //Mod the random number by 5 (0-5 totals 6 numbers) so it picks one of the 6 random messages
	switch(number){
	case 0:
		fortuneptr=fortune0;
		break;
	case 1:
		fortuneptr=fortune1;
		break;
	case 2:
		fortuneptr=fortune2;
		break;
	case 3:
		fortuneptr=fortune3;
		break;
	case 4:
		fortuneptr=fortune4;
		break;
	case 5:
		fortuneptr=fortune5;
		break;
	default:
		break;
	}
	driverlcd.Clear(&temp);
	driverlcd.Print(&temp, fortuneptr);

	*plcd=temp;
	return strlen(fortuneptr); //return the length of the fortune message
}
