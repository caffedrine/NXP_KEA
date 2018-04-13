/********************************************************************************
*    LAB9 - Bluetooth Low Energy Communication (BLE)        					*
*    FRDM+KEA64 Board															*
*                                                                             	*
*    This lab demonstrates how to use an Arduino shield to expand the			*
*    capabilities of the FRDM board.										  	*
*    										                                  	*
*    To use this demo you must install the BLE Controller app from either	 	*
*    the Apple Appstore or the Google Playstore.  The app is developed			*
*    by a third party and is free of charge.						         	*
*                                                                             	*
*    Once the app is running, make sure that it is in BLE Controller mode		*
*    and that your phone's Bluetooth is on. Run the demo and connect your       *
*    app to your FRDM board.  You can observe and control onboard I/O through	*
*    the app.																	*
********************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <boards.h>
#include "arduino.h"
#include "freemaster.h"
#include "drv_uart.h"
#include "derivative.h"

#define PROTOCOL_MAJOR_VERSION   0 //
#define PROTOCOL_MINOR_VERSION   0 //
#define PROTOCOL_BUGFIX_VERSION  2 // bugfix

#define PIN_CAPABILITY_NONE      0x00
#define PIN_CAPABILITY_DIGITAL   0x01

#define PIN_CAPABILITY_ANALOG    0x02
#define PIN_CAPABILITY_PWM       0x04
#define PIN_CAPABILITY_SERVO     0x08
#define PIN_CAPABILITY_I2C       0x10

// pin modes
//#define INPUT                 0x00 // defined in wiring.h
//#define OUTPUT                0x01 // defined in wiring.h
#define ANALOG                  0x02 // analog pin in analogInput mode
#define PWM                     0x03 // digital pin in PWM output mode
#define SERVO                   0x04 // digital pin in Servo output mode

byte pin_mode[TOTAL_PINS];
byte pin_state[TOTAL_PINS];
byte pin_pwm[TOTAL_PINS];
byte pin_servo[TOTAL_PINS];

/* Referenced Functions */
extern unsigned char ble_available(void);
extern void ble_begin(void);
extern void ble_set_name(char * name);
extern unsigned char ble_connected(void);
extern void ble_do_events(void);
extern int ble_read(void);
extern void ble_write(unsigned char data);

void setup()
{
  /* Default all to digital input */
  for (int pin = 0; pin < TOTAL_PINS - TOTAL_ANALOG_PINS; pin++)
  {
    // Set pin to input with internal pull up
    //pinMode(pin, INPUT);
	  pinMode(pin, INPUT_PULLUP);
	  digitalWrite(pin, HIGH);

    // Save pin mode and state
    pin_mode[pin] = INPUT;
    pin_state[pin] = LOW;
  }

  /* Serial init here */
  Serial.begin(115200);
  Serial.println("BLE Arduino Slave - S32DS_FSL");

  // Default pins set to 9 and 8 for REQN and RDYN
  // Set your REQN and RDYN here before ble_begin() if you need
  //ble_set_pins(3, 2);

  // Set your BLE Shield name here, max. length 10
  ble_set_name("FSL_DS");

  // Init. and start BLE library.
  ble_begin();
}

static byte buf_len = 0;

void ble_write_string(byte *bytes, uint8_t len)
{
  if (buf_len + len > 20)
  {
    for (int j = 0; j < 15000; j++)
      ble_do_events();

    buf_len = 0;
  }

  for (int j = 0; j < len; j++)
  {
    ble_write(bytes[j]);
    buf_len++;
  }

  if (buf_len == 20)
  {
    for (int j = 0; j < 15000; j++)
      ble_do_events();

    buf_len = 0;
  }
}

byte reportDigitalInput()
{
  if (!ble_connected())
    return 0;

  static byte pin = 0;
  byte report = 0;

  if (!IS_PIN_DIGITAL(pin))
  {
    pin++;
    if (pin >= TOTAL_PINS)
      pin = 0;
    return 0;
  }

  if (pin_mode[pin] == INPUT)
  {
      byte current_state = digitalRead(pin);

      if (pin_state[pin] != current_state)
      {
        pin_state[pin] = current_state;
        byte buf[] = {'G', pin, INPUT, current_state};
        ble_write_string(buf, 4);

        report = 1;
      }
  }

  pin++;
  if (pin >= TOTAL_PINS)
    pin = 0;

  return report;
}

void reportPinCapability(byte pin)
{
  byte buf[] = {'P', pin, 0x00};
  byte pin_cap = 0;

  if (IS_PIN_DIGITAL(pin))
    pin_cap |= PIN_CAPABILITY_DIGITAL;

  if (IS_PIN_ANALOG(pin))
    pin_cap |= PIN_CAPABILITY_ANALOG;

  if (IS_PIN_PWM(pin))
    pin_cap |= PIN_CAPABILITY_PWM;

  if (IS_PIN_SERVO(pin))
    pin_cap |= PIN_CAPABILITY_SERVO;

  buf[2] = pin_cap;
  ble_write_string(buf, 3);
}

void reportPinServoData(byte pin)
{
//  if (IS_PIN_SERVO(pin))
//    servos[PIN_TO_SERVO(pin)].write(value);
//  pin_servo[pin] = value;

  byte value = pin_servo[pin];
  byte mode = pin_mode[pin];
  byte buf[] = {'G', pin, mode, value};
  ble_write_string(buf, 4);
}

byte reportPinAnalogData()
{
  if (!ble_connected())
    return 0;

  static byte pin = 0;
  byte report = 0;

  if (!IS_PIN_DIGITAL(pin))
  {
    pin++;
    if (pin >= TOTAL_PINS)
      pin = 0;
    return 0;
  }

  if (pin_mode[pin] == ANALOG)
  {
    uint16_t value = analogRead(pin-TOTAL_PINS+TOTAL_ANALOG_PINS); //12Bit precision
    byte value_lo = value;
    byte value_hi = value>>8;

    byte mode = pin_mode[pin];
    mode = (value_hi << 4) | mode;

    byte buf[] = {'G', pin, mode, value_lo};
    ble_write_string(buf, 4);
  }

  pin++;
  if (pin >= TOTAL_PINS)
    pin = 0;

  return report;
}

void reportPinDigitalData(byte pin)
{
  byte state = digitalRead(pin);
  byte mode = pin_mode[pin];
  byte buf[] = {'G', pin, mode, state};
  ble_write_string(buf, 4);
}

void reportPinPWMData(byte pin)
{
  byte value = pin_pwm[pin];
  byte mode = pin_mode[pin];
  byte buf[] = {'G', pin, mode, value};
  ble_write_string(buf, 4);
}

void sendCustomData(uint8_t *buf, uint8_t len)
{
  uint8_t data[20] = "Z";
  memcpy(&data[1], buf, len);
  ble_write_string(data, len+1);
}

byte queryDone = false;

void loop()
{

while(ble_available())
  {
    byte cmd;
    cmd = ble_read();
    Serial.write(cmd);

    // Parse data here
    switch (cmd)
    {
      case 'V': // query protocol version
        {
          byte buf[] = {'V', 0x00, 0x00, 0x01};
          ble_write_string(buf, 4);
        }
        break;

      case 'C': // query board total pin count
        {
          byte buf[2];
          buf[0] = 'C';
          buf[1] = TOTAL_PINS;
          ble_write_string(buf, 2);
        }
        break;

      case 'M': // query pin mode
        {
          byte pin = ble_read();
          byte buf[] = {'M', pin, pin_mode[pin]}; // report pin mode
          ble_write_string(buf, 3);
        }
        break;

      case 'S': // set pin mode
        {
          byte pin = ble_read();
          byte mode = ble_read();
          byte mode_old = 0;

          //not implemented if (IS_PIN_SERVO(pin) && mode != SERVO && servos[PIN_TO_SERVO(pin)].attached())
          //not implemented   servos[PIN_TO_SERVO(pin)].detach();

          /* ToDo: check the mode is in its capability or not */
          /* assume always ok */
          if (mode != pin_mode[pin])
          {
            //R200040 check previous pin mode to disable
        	mode_old = pin_mode[pin]; //remember old mode
            switch (mode_old){
            case ANALOG:
            	//disable analog channel
            	break;
            case PWM:
            	//disable FTM channel
            	analogWriteDisable(pin);
            	break;
            case OUTPUT:
            case INPUT:
            	// do nothing
            	break;
            }

        	pinMode(pin, mode);
            pin_mode[pin] = mode;


            if (mode == OUTPUT)
            {
              digitalWrite(pin, LOW);
              pin_state[pin] = LOW;
            }
            else if (mode == INPUT)
            {
              digitalWrite(pin, HIGH);
              pin_state[pin] = HIGH;
            }
            else if (mode == ANALOG)
            {
              if (IS_PIN_ANALOG(pin)) {
                if (IS_PIN_DIGITAL(pin)) {
                  pinMode(PIN_TO_DIGITAL(pin), LOW);
                }
              }
            }
            else if (mode == PWM)
            {
              if (IS_PIN_PWM(pin))
              {
                pinMode(PIN_TO_PWM(pin), OUTPUT);
                analogWrite(PIN_TO_PWM(pin), 0);
                pin_pwm[pin] = 0;
                pin_mode[pin] = PWM;
              }
            }
            else if (mode == SERVO)
            {
             /*
             if (IS_PIN_SERVO(pin))
              {
                pin_servo[pin] = 0;
                pin_mode[pin] = SERVO;
                if (!servos[PIN_TO_SERVO(pin)].attached())
                  servos[PIN_TO_SERVO(pin)].attach(PIN_TO_DIGITAL(pin));
              }
             */
            }

          }

          if (mode == ANALOG)
            reportPinAnalogData(pin);
          if ( (mode == INPUT) || (mode == OUTPUT) )
            reportPinDigitalData(pin);
          else if (mode == PWM)
            reportPinPWMData(pin);
          else if (mode == SERVO)
            reportPinServoData(pin);
        }
        break;

      case 'G': // query pin data
        {
          byte pin = ble_read();
          reportPinDigitalData(pin);
        }
        break;

      case 'T': // set pin digital state
        {
          byte pin = ble_read();
          byte state = ble_read();

          digitalWrite(pin, state);
          reportPinDigitalData(pin);
        }
        break;

      case 'N': // set PWM
        {
          byte pin = ble_read();
          byte value = ble_read();

          analogWrite(PIN_TO_PWM(pin), value);
          pin_pwm[pin] = value;
          reportPinPWMData(pin);
        }
        break;

      case 'O': // set Servo
        {
          /* Not currently being implemented.
          byte pin = ble_read();
          byte value = ble_read();
          */

        /*  if (IS_PIN_SERVO(pin))
            servos[PIN_TO_SERVO(pin)].write(value);
          pin_servo[pin] = value;
          reportPinServoData(pin);
          */
        }
        break;

      case 'A': // query all pin status
        for (int pin = 0; pin < TOTAL_PINS; pin++)
        {
          reportPinCapability(pin);
          if ( (pin_mode[pin] == INPUT) || (pin_mode[pin] == OUTPUT) )
            reportPinDigitalData(pin);
          else if (pin_mode[pin] == PWM)
            reportPinPWMData(pin);
          else if (pin_mode[pin] == SERVO)
            reportPinServoData(pin);
        }

        queryDone = true;
        {
          uint8_t str[] = "ABC";
          sendCustomData(str, 3);
        }

        break;

      case 'P': // query pin capability
        {
          byte pin = ble_read();
          reportPinCapability(pin);
        }
        break;

      case 'Z':
        {
          byte len = ble_read();
          byte buf[len];
          for (int i=0;i<len;i++)
            buf[i] = ble_read();
          Serial.println("->");
          Serial.print("Received: ");
          Serial.printInt(len, DEC);
          Serial.println(" byte(s)");
          Serial.print(" Hex: ");
          for (int i=0;i<len;i++)
            Serial.printInt(buf[i], HEX);
          Serial.println("");
        }
    }

    // send out any outstanding data
    ble_do_events();
    buf_len = 0;

    return; // only do this task in this loop
  }

  // process text data
  if (Serial.available())
  {
    byte d = 'Z';
    ble_write(d);

    delay(5);
    while(Serial.available())
    {
      d = Serial.read();
      ble_write(d);
    }

    ble_do_events();
    buf_len = 0;

    return;
  }

  // No input data, no commands, process analog data
  if (!ble_connected())
    queryDone = false; // reset query state

  if (queryDone) // only report data after the query state
  {
    byte input_data_pending = reportDigitalInput();
    if (input_data_pending)
    {
      ble_do_events();
      buf_len = 0;

      return; // only do this task in this loop
    }

    reportPinAnalogData();

    ble_do_events();
    buf_len = 0;

    return;
  }

  ble_do_events();
  buf_len = 0;
}

int main(void)
{
	int counter=0;

	  msf_init(0);    // msf hw drivers init.
	  arduino_init(); //Internal function which must be called by main before calling setup().
	  setup(); // Arduino setup()
	  Driver_UART2.Initialize(BD115200,null);
	  FMSTR_Init();

	for(;;) {
	   	counter++;
	   	loop();  //Arduino loop()
	   	FMSTR_Poll();
	}

	return 0;
}

void NMI_Handler(void)
{
	__asm("bkpt");
}

void HardFault_Handler(void)
{
	__asm("bkpt");
}

void SVC_Handler(void)
{
	__asm("bkpt");
}

void PendSV_Handler(void)
{
	__asm("bkpt");
}
