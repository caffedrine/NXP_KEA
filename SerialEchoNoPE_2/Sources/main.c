/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "SKEAZN642.h"
#include "uart.h"

enum UART_pinsel
{
	PTB0_PTB1 = 0,
	PTA2_PTA3 = 1,
	PTC6_PTC7 = 2,
	PTF2_PTF3 = 3,
	PTD6_PTD7 = 4,
	PTI0_PTI1 = 5
};

#define UART_PORT	UART2
#define SYSCLK		(DEFAULT_SYSTEM_CLOCK / 2)
#define UART_BAUD	19200
#define UART_PINS	PTD6_PTD7

void UART_pin_settings(enum UART_pinsel uart_pins)
{
	if ( (uart_pins == 0) | (uart_pins == 1) )
	{
		SIM_PINSEL |= (uart_pins << SIM_PINSEL_UART0PS_SHIFT);
	}
	else if ( (uart_pins == 2) | (uart_pins == 3) )
	{
		SIM_PINSEL |= ((uart_pins % 2) << SIM_PINSEL_UART0PS_SHIFT);
	}
	else if ( (uart_pins == 4) | (uart_pins == 5) )
	{
		SIM_PINSEL |= ((uart_pins % 2) << SIM_PINSEL_UART0PS_SHIFT);
	}

	//SIM_PINSEL |= (uart_pins << SIM_PINSEL_UART0PS_SHIFT);
}

int main(void)
{
	/* Initialize pin settings for UART communication */
	UART_pin_settings(UART_PINS);

	/* Initialize UART communication */
	UART_Init(UART_PORT, SYSCLK, UART_BAUD);

	UART_EnableTx(UART_PORT);
	UART_EnableRx(UART_PORT);

	for ( ;; )
	{

		uint8_t c = UART_GetChar(UART_PORT);
		UART_PutChar(UART_PORT, c);
	}
	/* Never leave main */
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
