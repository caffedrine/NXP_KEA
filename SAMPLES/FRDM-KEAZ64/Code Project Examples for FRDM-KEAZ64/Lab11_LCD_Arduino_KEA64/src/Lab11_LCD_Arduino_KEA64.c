/********************************************************************************
*    LAB11 - LCD Arduino Control					        					*
*    FRDM+KEA64 Board															*
*                                                                             	*
*    This lab demonstrates how to control an LCD screen using the KEA.			*
*    																			*
*    This code example uses a host of peripherals, such KBI, FTM, RTC, GPIO,	*
*    PMC, and ADC at the same time. The LCD used to develop this code example 	*
*    is the DFR0009 by DFRobot. On the DFR0009 are six push buttons. The "RST"	*
*    button resets the MCU, just like SW1 on the FRDM KEA.  "Select" switches	*
*    the program between five LCD demo functions.  Finally, "Up", "Down",		*
*    "Left", and "Right" have been programmed to move the LCD cursor in the two	*
*    applicable demo functions.													*
*    																			*
*    The five demo functions are Message, Echo, ADC, Arithmetic, and Fortune	*
*    Teller. Plug in the DFR0009 to FRDM KEA board.  Connect your board			*
*    to a UART hyperterminal such as Putty and run the demo.  The Message		*
*    function will be the first to run.  You will see a welcome message print	*
*    on the UART terminal and the LCD screen.									*
*    																			*
*    Pressing "Select" will take you to the Echo function. This function		*
*    prints whatever you type into the UART terminal onto the LCD.  Your 		*
*    text will also appear in the UART terminal.								*
*    																			*
*    Pushing "Select" again will take you to the ADC function, which prints on	*
*    the LCD screen the 12-bit ADC value of the onboard FRDM KEA potentiometer,	*
*    R13.																		*
*    																			*
*    Press "Select" yet again, and you will enter the Arithmetic function.		*
*    Type any arithmetic problem between two positive integers (e.g. 5*5)		*
*    and hit the Enter key. The solution will be printed on the LCD. This		*
*    function supports math between TWO integer operands whose values are		*
*    between 0 and 9999, inclusive.												*
*    																			*
*    The final function is the Fortune Teller.  When you switch to this			*
*    function, the LCD screen will print "Ask me!"  Type any question into		*
*    the UART terminal and hit Enter.  Make sure to end your question 			*
*    with a '?'.  Your answer will be printed onto the LCD screen.				*
*    																			*
*    Pressing "Select" after the Fortune Teller will take you back to the		*
*    Message function.  														*
********************************************************************************/


#include "derivative.h" /* include peripheral declarations SKEAZ128M4 */
#include <string.h>
#include "LiquidCrystal.h"
#include "LCD_DFR0009.h"
#include "drv_adc.h"
#include "adc_kea.h"
#include "rtc.h"
#include "ics.h"
#include "lcd_routines.h"
#include "kbi.h"
#include "gpio.h"
#include "msf_skea.h"
#include "ftm_kea.h"
#include "drv_ftm.h"

#if defined(MCU_SKEAZ1284)

#define RIGHT_SHIFT_VALUE 4 //right shift the value four bits because PTE4 and PTE5 are in bit[5:4]. Move to bit[1:0]

#elif defined(MCU_SKEAZN642)

#define RIGHT_SHIFT_VALUE 0 //No shift required for PTA0 and PTA1

#else

/* Depends on what GPIO pins your board uses. */

#endif

LiquidCrystal lcd={0};

extern Driver_LiquidCrystal driverlcd;

extern MSF_DRIVER_ADC Driver_ADC0;

//extern char char_transmitted;

extern uint8_t lcd_mode_old;

#if defined(MCU_SKEAZ1284)
extern MSF_DRIVER_FTM Driver_FTM1; //PTE7, which FTM controls, is hooked to FTM_CH1 when SIM_PINSEL_FTM1PS1=1

#elif defined (MCU_SKEAZN642)

extern MSF_DRIVER_FTM Driver_FTM1; //PTE7 also maps to FTM_CH1 in SKEAZN642

#else

#endif

char msg_instruction[]={"\r\nDisplaying KEA welcome message on LCD screen.\r\n"};

char arithmetic_instructions[]={"\n\rPlease enter simple arithmetic problem of two positive integers (e.g. 2+3), with a max of 4 digits per operand (no higher than 9999). "
		"Answer will print on LCD.\r\n"
		"\n\rType your problem and press Enter\r\n"};

char adc_instruction[]={"\r\nWelcome to the ADC function! The LCD will display the 12-bit value of the FRDM-KEA on-board potentiometer R13.\r\n"};

char echo_instructions[]={"\n\rWelcome to the Echo function. Type anything onto this hyperterminal and the LCD will print the same characters.\r\n"};

char intro[]={"\n\rWelcome to the FRDM-KEA LCD Demo!\n\r"};

char fortune_instruction[]={"\n\rWelcome to the Fortune function. Type any question to the hyperterminal and press Enter. The LCD will display your fortune!\n\r"};

/* Function prototypes. */

void RTC_ISR(void);

void KBI_ISR(void);

void Brighter(void); //This function increases duty cycle of LED to increase brightness

void Dimmer(void); //This function decreases duty cycle of LED to decrease brightness

/* RTC interrupt service routine.  This is the callback function that is called by RTC_IRQHandler. */

void RTC_ISR(void){
	uint32_t temp=0;
	uint8_t button=none_pressed;
	/* Based on ADC value, determine which button has been pressed. */
	Driver_ADC0.SetChannel(8);
	temp=Driver_ADC0.Read();
	if((temp>=BTN_RIGHT_LOW)&&(temp<=BTN_RIGHT_HIGH)){
		button=right;
	}else if((temp>=BTN_UP_LOW)&&(temp<=BTN_UP_HIGH)){
		button=up;
	}else if((temp>=BTN_LEFT_LOW)&&(temp<=BTN_LEFT_HIGH)){
		button=left;
	}else if((temp>=BTN_DOWN_LOW)&&(temp<=BTN_DOWN_HIGH)){
		button=down;
	}else if((temp>=BTN_SELECT_LOW)&&(temp<=BTN_SELECT_HIGH)){
		button=select;
	}else{
		button=none_pressed;
	}
	switch(button){
	case right: //Move the cursor one to the right
		driverlcd.MoveCursorRight(&lcd);
		break;
	case up: //Move the cursor one line up, if possible
		if(lcd.currline>0){//Only move cursor up if there is another line to move up
			driverlcd.SetCursor(&lcd, lcd.currcol, lcd.currline-1);
		}
		break;
	case left: //Move the cursor one space to the left
		driverlcd.MoveCursorLeft(&lcd);
		break;
	case down: //Move the cursor one line down, if possible
		if(lcd.currline<(lcd.numlines-1)){ //Only move cursor down if there is room to move down. numlines-1 because let's say you have 3 lines, then that means they're lines 0-2. Only move down if you're in line 0 or line 1
			driverlcd.SetCursor(&lcd, lcd.currcol, lcd.currline+1);
		}
		break;
	case select: //If "Select" pressed, cycle between three programs: printing, arithmetic, and floating message
		driverlcd.Clear(&lcd); //Clear whatever is on the LCD first
		driverlcd.NoCursor(&lcd); //only want cursor in echo mode
		lcd_mode++; //increment the flag
		if(lcd_mode>fortune){ //If the flag increments past arithmetic, go back to message mode
				lcd_mode=message;
			}
		if(lcd_mode==message){
			int cnt=strlen(msg_instruction); //count the number of characters in the string
#if defined(MCU_SKEAZ1284) || defined(MCU_SKEAZN642)
			Driver_UART2.Send(msg_instruction,cnt); //Print the message instruction
#else
			/* Write your message to the proper UART for your device. */
#endif
		}else if(lcd_mode==echo){
			driverlcd.Cursor(&lcd);
			/* Count the number of characters in echo_instructions. */
			int cnt=strlen(echo_instructions);
			/* Print the Echo instruction message. */
#if defined(MCU_SKEAZ1284) || defined(MCU_SKEAZN642)
			Driver_UART2.Send(echo_instructions, cnt);
#else
			/* Write your message to the proper UART for your device. */
#endif
		}else if(lcd_mode==adc){
			int cnt=strlen(adc_instruction); //count the number of characters in the string
#if defined(MCU_SKEAZ1284) || defined(MCU_SKEAZN642)
			Driver_UART2.Send(adc_instruction, cnt); //Print the ADC instructions
#else
			/* Write your message to the proper UART for your device. */
#endif
		}else if(lcd_mode==arithmetic){//If you're going into arithmetic mode, print the instructions what to do onto the UART terminal
#if defined(MCU_SKEAZ1284) || defined(MCU_SKEAZN642)
			Driver_UART2.Send(arithmetic_instructions,strlen(arithmetic_instructions)); //Print the instructions
#else
			/* Write your message to the proper UART for your device. */
#endif
		}else{ //Or else it is the fortune function
			driverlcd.Print(&lcd, "Ask me!");
#if defined(MCU_SKEAZ1284) || defined(MCU_SKEAZN642)
			driverlcd.Cursor(&lcd);
			Driver_UART2.Send(fortune_instruction,strlen(fortune_instruction));
#else
			/* Write your message to the proper UART for your device. */
#endif
		}
		break;
	default:
		break;
	}
}

void Brighter(){
	/* Increase the duty cycle (i.e. the amount of time in a period that the LED is on).
	 * This demo uses low-true mode, meaning LED is ON up to FTMx_CnV.
	 */

	int16_t ch_val=0;

	/* Read the current value of the channel CnV, add 20 counts, and write it back to the CnV register. */

#if defined(MCU_SKEAZ1284)
	ch_val=Driver_FTM1.ChannelRead(1);

	ch_val+=20; //Increase the duty cycle by 20 counts

	/* If the CnV will rises above FTMx_MOD, set the CnV to FTMx_MOD; don't exceed. */

	if(ch_val>FTM1_MOD){
		ch_val=FTM1_MOD;
	}

	Driver_FTM1.ChannelWrite(1, ch_val);
#elif defined(MCU_SKEAZN642)
	ch_val=Driver_FTM1.ChannelRead(1);

	ch_val+=20; //Increase the duty cycle by 20 counts

	/* If the CnV will rises above FTMx_MOD, set the CnV to FTMx_MOD; don't exceed. */

	if(ch_val>FTM1_MOD){
		ch_val=FTM1_MOD;
	}

	Driver_FTM1.ChannelWrite(1, ch_val);
#else
#endif
}

void Dimmer(){
	/* Decrease the duty cycle (i.e. the amount of time in a period that the LED is on).
	 * This demo uses low-true mode, meaning LED is ON up to FTMx_CnV.
	 */

	int16_t ch_val=0;

	/* Read the current value of the channel CnV, subtract 20 counts, and write it back to the CnV register. */

#if defined(MCU_SKEAZ1284)
	ch_val=Driver_FTM1.ChannelRead(1);

	ch_val-=20; //Decrease the duty cycle by 20 counts

	/* If the CnV will fall below 0, set the CnV to 0; don't go negative. */

	if(ch_val<0){
		ch_val=0;
	}

	Driver_FTM1.ChannelWrite(1, ch_val);

#elif defined(MCU_SKEAZN642)
	ch_val=Driver_FTM1.ChannelRead(1);

	ch_val-=20; //Decrease the duty cycle by 20 counts

	/* If the CnV will fall below 0, set the CnV to 0; don't go negative. */

	if(ch_val<0){
		ch_val=0;
	}

	Driver_FTM1.ChannelWrite(1, ch_val);
#else
#endif

}

void KBI_ISR(void){

#if defined(MCU_SKEAZ1284)
	KBI_MemMapPtr pKBI = KBI1;
#elif defined(MCU_SKEAZN642)
	KBI_MemMapPtr pKBI = KBI0;
#else

#endif

	/* Grab the value of the source pin register to see which pin(s) were pressed. */

	uint32_t value=pKBI->SP;
	value=value>>RIGHT_SHIFT_VALUE; //Right shift your value by however much is needed to bring the pins to bits[1:0]

	switch(value){
	case 0b01:
		Brighter();
		break;
	case 0b10:
		Dimmer();
		break;
	default:
		break;
	}

	/* Clear the source pin register. */
	pKBI->SC |= KBI_SC_RSTKBSP_MASK;

}

int main(void)
{
	int cnt=0;
	/* Enable the clock source FEI mode where core clock is 48 MHz and bus clock is 24MHz. */
	ICS_Init_FEI();

	/* Initialize the UART to talk to the hyper terminal. */
#if defined(MCU_SKEAZ1284) || defined(MCU_SKEAZN642)
	Driver_UART2.Initialize(BD115200, 0); //initialize with baudrate of 115200. Currently in polled mode
#else
#endif
	/* Change UART to interrupt driven mode. */

#if defined(MCU_SKEAZ1284) || defined(MCU_SKEAZN642)
	Driver_UART2.Control(MSF_UART_INT_MODE, 0);
#else
#endif

	/* Enable the RTC. */
	RTC_ConfigType rtc_set={0};
	rtc_set.bClockSource=RTC_CLKSRC_1KHZ; //Config RTC to us LPOCLK (1kHz)
	rtc_set.bClockPrescaler=RTC_CLK_PRESCALER_100; //Prescale of 100 so RTC freq is 1kHz/100=10Hz. 10 ticks per second
	rtc_set.u16ModuloValue=2; //RTC counter RTC_CNT overflows every 2 ticks, meaning every .2s (200ms)
	rtc_set.bFlag=1; //w1c to RTC_SC_RTIF in case the interrupt flag has been set for some reason
	rtc_set.bInterruptEn=1; //Enable RTC Interrupts

	RTC_SetCallback(RTC_ISR); //Set the ISR

	RTC_Init(&rtc_set);

	/* Onboard KEA switches for KBI to control RGB LED. Set them to output. Different boards have different pinouts for gpio buttons*/

#if defined(MCU_SKEAZ1284)	//On SKEAZ1284 family (KEAZ128 and KEAZ64) SW2 and SW3 are PTE4 and PTE5
	gpio_direction(GPIO_E4, input);

	gpio_direction(GPIO_E5, input);

	/* Also configure PTE7, the blue LED to an output and turn it off. */

	gpio_direction(GPIO_E7, output);

	gpio_write(GPIO_E7, 0); //Turns it off

#elif defined(MCU_SKEAZN642) //On SKEAZN642 family (KEAZN32) SW2 and SW3 are PTA0 and PTA1
	gpio_direction(GPIO_A0, input);

	gpio_direction(GPIO_A1, input);

	/* SKEAZN642 blue LED is also PTE7. */

	gpio_direction(GPIO_E7, output);

	gpio_write(GPIO_E7, 0);

#else
/* If you have a board other than those above, config the push buttons based on the pinout of the board you are using. */
#endif

	/* Initialize FTM. Used to control PWM feature of LED. FTM pinout to LED depends on board. */

#if defined(MCU_SKEAZ1284)
	/* On SKEAZ1284 family, blue LED is PTE7, which hooks to FTM1_CH1. */

	Driver_FTM1.Initialize(0); //No callback event necessary.  FTM Initialize sets the internal clock (ICSFFCLK) as the FTM clock and FTM1_MOD to xFFFF

	/* Set the clock to be the system clock of FTM, which is the TIMER_CLK at 48 MHz. Also, set prescaler to 7, meaning divide by 128
	 * At 48 MHz system clock divided by 128 creates 375 kHz. */
	Driver_FTM1.Control(MSF_FTM_CLOCK_SYSTEM|MSF_FTM_PRESCALER_SET, MSF_FTM_PRESCALER_128);

	/* Change MOD value to 1000. */

	Driver_FTM1.Control(MSF_FTM_MOD_VALUE, 1000);

	/* Setting FTM1_CH1 to Edge-Aligned PWM mode, Low True (sets output on start of period). */
	/* Altnum argument for FTM1_CH1 is set to 1 but MSF driver so SIM_PINSEL0_FTM1PS1 is set by ChannelSetMode function. */

	Driver_FTM1.ChannelSetMode(1,PWM_edge_lowtrue, 0); //no additional arguments

	Driver_FTM1.ChannelWrite(1, 500); //Initialize the duty cycle to half the period of FTM1

#elif defined(MCU_SKEAZN642)
	/* On SKEAZ1284 family, blue LED is PTE7, which hooks to FTM1_CH1. */

	Driver_FTM1.Initialize(0); //No callback event necessary.  FTM Initialize sets the internal clock (ICSFFCLK) as the FTM clock and FTM1_MOD to xFFFF

	/* Set the clock to be the system clock of FTM, which is the TIMER_CLK at 48 MHz. Also, set prescaler to 7, meaning divide by 128
	 * At 48 MHz system clock divided by 128 creates 375 kHz. */
	Driver_FTM1.Control(MSF_FTM_CLOCK_SYSTEM|MSF_FTM_PRESCALER_SET, MSF_FTM_PRESCALER_128);

	/* Change MOD value to 1000. */

	Driver_FTM1.Control(MSF_FTM_MOD_VALUE, 1000);

	/* Setting FTM1_CH1 to Edge-Aligned PWM mode, Low True (sets output on start of period). */
	/* Altnum argument for FTM1_CH1 is set to 1 but MSF driver so SIM_PINSEL0_FTM1PS1 is set by ChannelSetMode function. */

	Driver_FTM1.ChannelSetMode(1,PWM_edge_lowtrue, 0); //no additional arguments

	Driver_FTM1.ChannelWrite(1, 500); //Initialize the duty cycle to half the period of FTM1
#else

#endif

	/* Next, enable KBI on those two pins. We want edge detection only, Falling edge triggered, we want interrupt enabled.
	 * We also want latched value in KBI source pin register when interrupt fires.
	 */

	KBI_ConfigType KBI_set={{0}};
	KBI_set.sBits.bIntEn=1; //Enable KBI Interrupts
	KBI_set.sBits.bKbspEn=1; //Sets to latched value mode
	KBI_set.sBits.bRstKbsp=1; //Resets the whole KBI source pin register.  Good for initializing in case anything is stuck at 1 upon reset

	/* Set the pin initializtion.  Which pin is configured depends on the device. */
#if defined(MCU_SKEAZ1284)
#if FRDM_KEAZ128_PROTOTYPE //Push buttons on prototype are active low.  Polarity flipped on production
	KBI_set.sPin[4].bEdge=KBI_FALLING_EDGE_LOW_LEVEL;
#else
	KBI_set.sPin[4].bEdge=KBI_RISING_EDGE_HIGH_LEVEL;
#endif
	KBI_set.sPin[4].bEn=1;

#if FRDM_KEAZ128_PROTOTYPE
	KBI_set.sPin[5].bEdge=KBI_FALLING_EDGE_LOW_LEVEL;
#else
	KBI_set.sPin[5].bEdge=KBI_RISING_EDGE_HIGH_LEVEL;
#endif

	KBI_set.sPin[5].bEn=1;

	/* Initialize the KBI peripheral.  PTE4 and PTE5 correspond to KBI1. */

	KBI_SetCallback(KBI1, KBI_ISR);

	KBI_Init(KBI1, &KBI_set);

#elif defined(MCU_SKEAZN642)
	KBI_set.sPin[0].bEdge=KBI_RISING_EDGE_HIGH_LEVEL;
	KBI_set.sPin[0].bEn=1;

	KBI_set.sPin[1].bEdge=KBI_RISING_EDGE_HIGH_LEVEL;
	KBI_set.sPin[1].bEn=1;

	KBI_SetCallback(KBI0, KBI_ISR);

	/* PTA0 and PTA1 correspond to KBI0. */

	KBI_Init(KBI0, &KBI_set);
#else

#endif

	/* Enable read of push buttons on LCD shield. */

#if defined(MCU_SKEAZ1284) || defined(MCU_SKEAZN642)
	Driver_ADC0.Initialize(0);
	Driver_ADC0.Control(MSF_ADC_RES_12_BITS,0); //Change to 12 bit resolution
	Driver_ADC0.SetChannel(8);
#else
	/* Initialize the correct ADC channel for your device. */
#endif

	/* Bit banging GPIO ports, not using SPI so don't initialize it. */
	DFR0009_InitParams(&lcd); //Set the parameters of this particular shield
	driverlcd.Init(&lcd); //Initialize the LCD.  LCDBegin called within LCDInit
	/* LCD text follows ASCII standard */
	driverlcd.Print(&lcd, "Welcome!");
	cnt=strlen(intro); //Get length of intro message
	Driver_UART2.Send(intro,cnt); //Print intro message on UART hyperterminal
	for(int i=0; i<0xFFFFF; i++){} //Wait 0xFFFFF counts so user can enjoy this welcome message
	lcd_mode=message; //Initialize the LCD to message mode
	driverlcd.Clear(&lcd);

	/* Print character received through UART. */
	for(;;) {
		lcd_mode_old=lcd_mode; //Update the LCD mode flag
		switch(lcd_mode){ //This switches between message, echo, and arithmetic lcd modes
		case message:
			msg(&lcd);
			break;
		case echo:
			Echo(&lcd);
			break;
		case adc:
			ADC_Show(&lcd);
			break;
		case arithmetic:
			math(&lcd);
			break;
		case fortune:
			fortune_teller(&lcd);
			break;
		default:
			break;
		}
		delayMicroseconds(20000); //Delay 200ms
	}
	
	return 0;
}


