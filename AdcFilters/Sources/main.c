#include "derivative.h"
#include "CLK/CLK.h"
#include "ADC/ADC.h"
#include "SYSTICK/SysTick.h"
#include "CircularBuffer/CircularBuffer.h"
#include "DbgConsole/DbgConsole.h"

#define SAMPLES_NO	100

/* Ticks */
uint32_t currTicksCounter, prevTicksCounter;

/* Used on ADC events to have the store location for analog reads */
typedef struct adc_pair
{
	uint8_t adcChannel;
	uint16_t *storeLocation;
}adc_pair_t;

uint16_t analog_truck_12v_voltage;						///<! Store current read voltage of truck power supply from 12V pin
uint16_t analog_truck_24v_voltage;						///<! Store current read voltage of truck power supply from 24V pin
uint16_t analog_external_pwr_source_voltage;			///<! External power source voltage read
uint16_t analog_pwm_fb;									///<! LM5175 PWM feedback
uint16_t analog_v_battery_micro_voltage;				///<! Battery voltage read on uC
uint16_t analog_battery_current_amps;					///<! Battery current read
uint16_t analog_NTC_mos_temp;							///<! PCB Temperature read
uint16_t analog_NTC_batt1_temp;							///<! Battery temperature 1
uint16_t analog_NTC_batt2_temp;							///<! Battery temperature 2

/* Select where to store each adc value */
const static adc_pair_t adc_channels[] =
{
		{1, 			&analog_truck_12v_voltage},
		{2, 			&analog_truck_24v_voltage},
		{6, 			&analog_external_pwr_source_voltage},
		{13, 			&analog_v_battery_micro_voltage},
		{14, 			&analog_battery_current_amps},
		{15, 			&analog_NTC_mos_temp},
		{8, 			&analog_NTC_batt1_temp}
//		{9, 			&analog_NTC_batt2_temp}
};

static inline float filterAdc(float current_element, float input)
{
		float tmp;
		tmp = current_element;
		tmp *= 100.0f;
		tmp += input;
		tmp = (tmp/101.0f);
		return tmp;
}

void ADC_OnEnd(void)
{
	/// The values are handled on main loop
	// Channels counter
	static int chCount = 0, chNumber = (sizeof(adc_channels)/sizeof(adc_pair_t)-1);

	// Process adc result
	float adcResult = ADC_ReadResultReg(ADC);
	float currentElement = *(adc_channels[chCount].storeLocation);

	// Save adc value
	float filteredResult = filterAdc( currentElement, adcResult);

	*(adc_channels[chCount].storeLocation) = filteredResult;

	if(chCount == 4)
	{
		int breakp = 1;
		if(breakp == 1)
		{
			int b = 0;
		}
	}

	// Calculate next channel
	chCount = ((chCount == chNumber) ? (0) : (++chCount) );

	// Set current channel to be read
	ADC_SetChannel(ADC, adc_channels[chCount].adcChannel);

	// Enable reading on next channel
	ADC_ContinuousConversion(ADC);
}

int main(void)
{
	/** Init clock */
	Clk_Init(CLK_FREQUENCY_16_MHZ);
	/** Debug console init */
	DbgConsole_Init(UART0, CLK_FREQUENCY_16_MHZ, 115200);

	/* Init ADC with default params */
	ADC_Init_Default();
	/* Set ADC interruption callback */
	ADC_SetCallBack(ADC_OnEnd);
	/* ADC Set channel */
	ADC_SetChannel(ADC, ADC_CHANNEL_AD8);

	/* SysTick Init - Reload value is provided in uS - 1000uS  = 1mS */
	SysTick_Init( CLK_FREQUENCY_16_MHZ/* Clock freq *// (1000 / 2 /* uS */));

	printf("--start!--\r\n");
	static int i = 0;

	float a;

	for (;;)
	{
		//a = filter(4096, 4095);

		if( currTicksCounter % 100 == 0 ) /* Every 500ms */
		{
			printf("\e[1;1H\e[2J");
			fflush(stdout);
			printf("%d. ADC Conversions:\r\n", i++);
			printf("Truck 12V\t: %d\r\n", analog_truck_12v_voltage );
			printf("Truck 24V\t: %d\r\n", analog_truck_24v_voltage );
			printf("External PWR\t: %d\r\n", analog_external_pwr_source_voltage );
			printf("Battery voltage\t: %d\r\n", analog_v_battery_micro_voltage);
			printf("Battery amps\t: %d\r\n", analog_battery_current_amps );
			printf("NTC MOS Sens\t: %d\r\n", analog_NTC_mos_temp );
			printf("Batt1 temp\t: %d\r\n",  analog_NTC_batt1_temp);
			printf("Batt2 temp\t: %d\r\n", analog_NTC_batt2_temp );
			//printf("A=%.2f\r\n", a);
		}
	}
	return 0;
}
