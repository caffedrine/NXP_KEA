/*
 * GPIO.h
 *
 *  Created on: Sep 24, 2013
 *      Author: B46911
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "derivative.h"

#define A1  A
#define B1  A
#define C1  A
#define D1  A

#define E1	B
#define F1	B
#define G1	B
#define H1	B

#define I1	C

#define OUTPUT  1
#define INPUT	0

#define CONFIG_PIN_AS_GPIO(port,port_pin,mode)    XCONFIG_PIN_AS_GPIO(port,port_pin,mode)
#define XCONFIG_PIN_AS_GPIO(port,port_pin,mode)   (mode == 0) ? (GPIO##port##_PDDR |= 0 << port_pin) : (GPIO##port##_PDDR |= 1 << port_pin)

#define ENABLE_INPUT(port,port_pin) 				XENABLE_INPUT(port,port_pin)
#define XENABLE_INPUT(port,port_pin)				GPIO##port##_PIDR ^= 1<<port_pin

#define ENABLE_PULLUP(port,port_pin) 				XENABLE_PULLUP(port,port_pin) 	
#define XENABLE_PULLUP(port,port_pin) 				PORT_PUE0 |= PORT_PUE0_PT##port##PE##port_pin##_MASK

#define OUTPUT_SET(port,register_num)				XOUTPUT_SET(port,register_num)
#define XOUTPUT_SET(port,register_num)				GPIO##port##_PCOR |=1<<register_num

#define OUTPUT_CLEAR(port,register_num)				XOUTPUT_CLEAR(port,register_num)		
#define XOUTPUT_CLEAR(port,register_num)			GPIO##port##_PSOR |=1<<register_num

#define OUTPUT_TOGGLE(port,register_num)			XOUTPUT_TOGGLE(port,register_num)
#define XOUTPUT_TOGGLE(port,register_num)			GPIO##port##_PTOR |=1<<register_num
												
#define CONFIG_PIN_AS_FGPIO(port,port_pin,mode)    XCONFIG_PIN_AS_FGPIO(port,port_pin,mode)
#define XCONFIG_PIN_AS_FGPIO(port,port_pin,mode)   (mode == 0) ? (FGPIO##port##_PDDR |= 0 << port_pin) : (FGPIO##port##_PDDR |= 1 << port_pin)

#define READ_INPUT(port,port_pin)					XREAD_INPUT(port,port_pin)
#define XREAD_INPUT(port,port_pin)					(GPIO##port##_PDIR & GPIO_PDIR_PDI(1<<port_pin))

#endif /* GPIO_H_ */
