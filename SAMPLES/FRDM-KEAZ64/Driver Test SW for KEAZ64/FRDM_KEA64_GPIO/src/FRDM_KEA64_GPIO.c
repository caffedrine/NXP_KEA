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
* @file FRDM_KEA64_GPIO.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief GPIO demo.
* 		Press button 0 (SW2) to turn on LED0
* 		Press button 1 (SW3) to turn on LED1
*
*******************************************************************************/
#include "gpio.h"

/******************************************************************************
* Global variables
******************************************************************************/
/******************************************************************************
* Constants and macros
******************************************************************************/

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/

/******************************************************************************
* Local functions
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

uint8_t a;
/********************************************************************/
int main (void)
{

    	CONFIG_PIN_AS_GPIO(PTH,PTH0,OUTPUT); /*Config LED0 as output */
    	CONFIG_PIN_AS_GPIO(PTH,PTH1,OUTPUT); /*Config LED1 as output */
    	OUTPUT_CLEAR(PTH,PTH0);
    	OUTPUT_CLEAR(PTH,PTH1);


    	CONFIG_PIN_AS_GPIO(PTE,PTE4,INPUT);  /* Config BTN0 as input*/
    	ENABLE_INPUT(PTE,PTE4);

    	CONFIG_PIN_AS_GPIO(PTE,PTE5,INPUT);	 /* Config BTN1 as input*/
    	ENABLE_INPUT(PTE,PTE5);

    	while(1)
    	{
    		if(!READ_INPUT(PTE,PTE4))  	/* If button 0 is pressed*/
    		{
    			OUTPUT_SET(PTH,PTH0); 	/* Turn on LED0*/

    		}
			else						/* If button 0 is unpressed*/
			{
				OUTPUT_CLEAR(PTH,PTH0); /* Turn off LED0*/
			}

			if(!READ_INPUT(PTE,PTE5))	 /* If button 1 is pressed*/
			{
				OUTPUT_SET(PTH,PTH1); 	/* Turn on LED1*/

			}
			else						/* If button 0 is unpressed*/
			{
				OUTPUT_CLEAR(PTH,PTH1); /* Turn off LED1*/
			}

		}

}
