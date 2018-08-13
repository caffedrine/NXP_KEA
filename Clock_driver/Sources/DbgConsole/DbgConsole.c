/*
 * DbgConsole.c
 *
 *  Created on: May 16, 2018
 *      Author: Alex C.
 */


#include "DbgConsole.h"

#include <stdio.h>

/** UART unctions definitions */
void UART_Init(UART_Type *pUART, uint32_t u32SysClk, uint32_t u32Baud);
void UART_SendChar(UART_Type *pUART, uint8_t send);
uint8_t UART_GetChar(UART_Type *pUART);

/** Default uart channel */
UART_Type *pUART = UART2;

void DbgConsole_Init(UART_Type *pUartType, uint32_t PbClk, uint32_t u32Baud)
{
	pUART = pUartType;

	UART_Init( pUART, PbClk, u32Baud );
}

//	 ____  ____  ___ _   _ _____ _____    ____
//	|  _ \|  _ \|_ _| \ | |_   _|  ___|  / /\ \
//	| |_) | |_) || ||  \| | | | | |_    | |  | |
//	|  __/|  _ < | || |\  | | | |  _|   | |  | |
//	|_|   |_| \_\___|_| \_| |_| |_|     | |  | |
// 										 \_\/_/

///!< CLASSIC: (does not work on kds)
//struct __FILE
//{
//	int dummyVar; //Just for the sake of redefining __FILE, we won't we using it anyways ;)
//};
//
//FILE __stdout;
//FILE __stdin;
//
//int fputc(int c, FILE * stream)
//{
//	char s = c;
//	UART_SendChar( pUART, c );
//	return c; 					//return the character written to denote a successfully write
//}
//
//int fgetc(FILE * stream)
//{
//	char c = UART_GetChar(pUART);
//	return c;
//}

///!< KDS Version
int _read(int fd, const void *buf, size_t count)
{
	size_t CharCnt = 0x00;

	(void)fd; /* Parameter is not used, suppress unused argument warning */
	for ( ; count > 0x00; --count )
	{
		/* Wait for received buffer to be full*/
		while ( ( pUART->S1 & UART_S1_RDRF_MASK) == 0 )
				;

		 /* Increase char counter */
		CharCnt++;

		/* Save character received by UARTx device into the receive buffer */
		*(uint8_t*)buf = UART_GetChar(pUART);

		/* Stop reading if CR (Ox0D) character is received */
		if ( *(uint8_t*)buf == 0x0DU )
		{
			/* New line character (CR) received ? */
			*(uint8_t*)buf = '\n'; /* Yes, convert LF to '\n' char. */
			break; /* Stop loop and return received char(s) */
		}
		(uint8_t*)buf++; /* Increase buffer pointer */
	}
	return CharCnt;
}

int _write(int fd, const void *buf, size_t count)
{
	size_t CharCnt = 0x00;

	(void)fd; /* Parameter is not used, suppress unused argument warning */
	for ( ; count > 0x00; --count )
	{
		UART_SendChar(pUART, (unsigned char)*(uint8_t*)buf);

		(uint8_t*)buf++; /* Increase buffer pointer */
		CharCnt++; /* Increase char counter */
	}
	return CharCnt;
}


//	 _   _   _    ____ _____
//	| | | | / \  |  _ \_   _|
//	| | | |/ _ \ | |_) || |
//	| |_| / ___ \|  _ < | |
//	 \___/_/   \_\_| \_\|_|
//

void UART_Init(UART_Type *pUART, uint32_t PbClk, uint32_t u32Baud)
{
	uint16_t u16Sbr;
	uint8_t u8Temp;

	/* Enable the clock to the selected UART */
	(pUART == UART0) ? (SIM->SCGC |= SIM_SCGC_UART0_MASK) : ((pUART == UART1) ? (SIM->SCGC |= SIM_SCGC_UART1_MASK) : (SIM->SCGC |= SIM_SCGC_UART2_MASK));

	pUART->BDH = 0; /* One stop bit*/
	pUART->BDL = 128; /* Baud rate at 9600*/
	pUART->C1 = 0; /* No parity enable, 8 bit format*/
	pUART->C2 |= UART_C2_TE_MASK; /* Enable Transmitter*/
	pUART->C2 |= UART_C2_RE_MASK; /* Enable Receiver*/
	pUART->C2 |= UART_C2_RIE_MASK; /* Enable Receiver interrupts*/
}

void UART_SendChar(UART_Type *pUART, uint8_t u8Char)
{
	/* Wait until space is available in the FIFO */
	while ( !(pUART->S1 & UART_S1_TDRE_MASK) )
		;

	/* Read UART2_S1 register*/
	(void)pUART->S1;

	/* Send the character */
	pUART->D = (uint8_t)u8Char;
}

uint8_t UART_GetChar(UART_Type *pUART)
{
	/* Wait until character has been received */
	while ( !(pUART->S1 & UART_S1_RDRF_MASK) )
		;

	/* Read UART2_S1 register*/
	(void)pUART->S1;

	/* Return the 8-bit data from the receiver */
	return pUART->D;
}

