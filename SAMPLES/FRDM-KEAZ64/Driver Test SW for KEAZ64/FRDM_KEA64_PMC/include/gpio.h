/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2013 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
***************************************************************************//*!
*
* @file GPIO.h
*
* @author Freescale
*
* @brief provide common GPIO utilities. 
*
*******************************************************************************/

#ifndef GPIO_H_
#define GPIO_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "derivative.h"


/*******************************************************************************
* Constants
********************************************************************************/


/*******************************************************************************
* Macros
********************************************************************************/

/***********************************************************
 * 
 * Ports mapped to GPIOA,GPIOB and GPIOC registers
 * 
 **********************************************************/
#define PTA     GPIOA_BASE_PTR
#define PTB     GPIOA_BASE_PTR
#define PTC     GPIOA_BASE_PTR
#define PTD     GPIOA_BASE_PTR

#define PTE     GPIOB_BASE_PTR
#define PTF		GPIOB_BASE_PTR
#define PTG		GPIOB_BASE_PTR
#define PTH		GPIOB_BASE_PTR

#define PTI		GPIOC_BASE_PTR

/***********************************************************
 * 
 * Input/Output macro
 * 
 **********************************************************/
#define OUTPUT  1
#define INPUT	0

/***********************************************************
 * 
 * GPIO configuration macro
 * 
 **********************************************************/
#define CONFIG_PIN_AS_GPIO(port,port_pin,mode)    	XCONFIG_PIN_AS_GPIO(port,port_pin,mode)
#define XCONFIG_PIN_AS_GPIO(port,port_pin,mode)   	(mode == 0) ? (port->PDDR &=~(1<< port_pin)) : (port->PDDR |= (1 << port_pin)) 

/***********************************************************
 * 
 * Enable and Disable input macro
 * 
 **********************************************************/
#define ENABLE_INPUT(port,port_pin) 				XENABLE_INPUT(port,port_pin)
#define XENABLE_INPUT(port,port_pin)				port->PIDR &=~ (1<<port_pin)

#define DISABLE_INPUT(port,port_pin) 				XDISABLE_INPUT(port,port_pin)
#define XDISABLE_INPUT(port,port_pin)				port->PIDR |= (1<<port_pin)

/***********************************************************
 * 
 * Enable and Disable pullup resistor macro
 * 
 **********************************************************/
#define ENABLE_PULLUP(port,port_pin) 				XENABLE_PULLUP(port,port_pin) 	
#define XENABLE_PULLUP(port,port_pin) 				if(port==GPIOA_BASE_PTR){PORT_BASE_PTR->PUE0 |= (1<<port_pin);} else if(port==GPIOB_BASE_PTR){PORT_BASE_PTR->PUE1|=(1<<port_pin);} else {PORT_BASE_PTR->PUE2|=(1<<port_pin);}

#define DISABLE_PULLUP(port,port_pin) 				XDISABLE_PULLUP(port,port_pin) 	
#define XDISABLE_PULLUP(port,port_pin) 				if(port==GPIOA_BASE_PTR){PORT_BASE_PTR->PUE0 &=~ (1<<port_pin);} else if(port==GPIOB_BASE_PTR){PORT_BASE_PTR->PUE1 &=~ (1<<port_pin);} else {PORT_BASE_PTR->PUE2 &=~(1<<port_pin);}

/***********************************************************
 * 
 * Output set macro
 * 
 **********************************************************/
#define OUTPUT_SET(port,port_pin)					XOUTPUT_SET(port,port_pin)
#define XOUTPUT_SET(port,port_pin)					port->PCOR |=(1<<port_pin)

/***********************************************************
 * 
 * Output clear macro
 * 
 **********************************************************/
#define OUTPUT_CLEAR(port,port_pin)					XOUTPUT_CLEAR(port,port_pin)		
#define XOUTPUT_CLEAR(port,port_pin)				port->PSOR |=(1<<port_pin)

/***********************************************************
 * 
 * Output toggle macro
 * 
 **********************************************************/
#define OUTPUT_TOGGLE(port,port_pin)				XOUTPUT_TOGGLE(port,port_pin)
#define XOUTPUT_TOGGLE(port,port_pin)				port->PTOR |=(1<<port_pin)

/***********************************************************
 * 
 * Read input macro
 * 
 **********************************************************/
#define READ_INPUT(port,port_pin)					XREAD_INPUT(port,port_pin)
#define XREAD_INPUT(port,port_pin)					(port->PDIR & GPIO_PDIR_PDI(1<<port_pin))
                
/*******************************************************************************
* Types
********************************************************************************/


/*******************************************************************************
* Variables
********************************************************************************/


/*******************************************************************************
* Global Variables
********************************************************************************/
/***********************************************************
 * 
 * Pins mapped to GPIOA,GPIOB and GPIOC register numbers
 * 
 **********************************************************/
enum GPIOA_Register {
PTA0,	
PTA1,
PTA2,
PTA3,
PTA4,
PTA5,
PTA6,
PTA7,
PTB0,	
PTB1,
PTB2,
PTB3,
PTB4,
PTB5,
PTB6,
PTB7,
PTC0,	
PTC1,
PTC2,
PTC3,
PTC4,
PTC5,
PTC6,
PTC7,
PTD0,	
PTD1,
PTD2,
PTD3,
PTD4,
PTD5,
PTD6,
PTD7,
};

enum GPIOB_Register {
PTE0,	
PTE1,
PTE2,
PTE3,
PTE4,
PTE5,
PTE6,
PTE7,
PTF0,	
PTF1,
PTF2,
PTF3,
PTF4,
PTF5,
PTF6,
PTF7,
PTG0,	
PTG1,
PTG2,
PTG3,
PTG4,
PTG5,
PTG6,
PTG7,
PTH0,	
PTH1,
PTH2,
PTH3,
PTH4,
PTH5,
PTH6,
PTH7,
};

enum GPIOC_Register {
PTI0,	
PTI1,
PTI2,
PTI3,
PTI4,
PTI5,
PTI6,
};


/*******************************************************************************
* Global Functions
********************************************************************************/


#endif /* GPIO_H_ */
