/*
 * DbgConsole.c
 *
 *  Created on: May 16, 2018
 *      Author: Alex C.
 */

#include "DbgConsole/DbgConsole.h"

#include <stdio.h>

#ifndef USE_UART_INTERNAL_IMPLEMENTATION_BLOCKING
#include "UART/UART.h"
#endif

/** UART unctions definitions */
static void Dbg_UART_Init(UART_Type *pUART, uint32_t u32SysClk, uint32_t u32Baud);
static void Dbg_UART_SendChar(UART_Type *pUART, uint8_t send);
static uint8_t Dbg_UART_GetChar(UART_Type *pUART);

/** Default uart channel */
UART_Type *pUART = UART2;

void DbgConsole_Init(UART_Type *pUartType, uint32_t ClkBus, uint32_t u32Baud)
{
	pUART = pUartType;

	Dbg_UART_Init( pUART, ClkBus, u32Baud );
}
///!< KDS Versions
//	 ____   ____    _    _   _ _____    ____
//	/ ___| / ___|  / \  | \ | |  ___|  / /\ \
//	\___ \| |     / _ \ |  \| | |_    | |  | |
//	 ___) | |___ / ___ \| |\  |  _|   | |  | |
//	|____/ \____/_/   \_\_| \_|_|     | |  | |
//	 								   \_\/_/
int _read(int fd, const void *buf, size_t count)
{
	size_t CharCnt = 0x00;

	(void)fd; /* Parameter is not used, suppress unused argument warning */
	for ( ; count > 0x00; --count )
	{
		/* Wait for received buffer to be full*/
		while ( (pUART->S1 & UART_S1_RDRF_MASK) == 0 )
			;

		/* Increase char counter */
		CharCnt++;

		/* Save character received by UARTx device into the receive buffer */
		*(uint8_t*)buf = Dbg_UART_GetChar( pUART );

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

//	 ____  ____  ___ _   _ _____ _____    ____
//	|  _ \|  _ \|_ _| \ | |_   _|  ___|  / /\ \
//	| |_) | |_) || ||  \| | | | | |_    | |  | |
//	|  __/|  _ < | || |\  | | | |  _|   | |  | |
//	|_|   |_| \_\___|_| \_| |_| |_|     | |  | |
// 										 \_\/_/
int _write(int fd, const void *buf, size_t count)
{
	size_t CharCnt = 0x00;

	(void)fd; /* Parameter is not used, suppress unused argument warning */
	for ( ; count > 0x00; --count )
	{
		Dbg_UART_SendChar( pUART, (unsigned char)*(uint8_t*)buf );

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

#ifdef USE_UART_INTERNAL_IMPLEMENTATION_BLOCKING

// Own UART implementation
static void Dbg_UART_Init(UART_Type *pUART, uint32_t BusClock, uint32_t u32Baud)
{
    uint16_t u16Sbr;
    uint8_t u8Temp;

	/* Enable the clock to the selected UART */
    if (pUART == UART0)
	{
		SIM->SCGC |= SIM_SCGC_UART0_MASK;
	}
	else if (pUART == UART1)
	{
        SIM->SCGC |= SIM_SCGC_UART1_MASK;
	}
    else
	{
        SIM->SCGC |= SIM_SCGC_UART2_MASK;
	}
    /* Make sure that the transmitter and receiver are disabled while we
     * change settings.
     */
    pUART->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );

    /* Configure the UART for 8-bit mode, no parity */
    pUART->C1 = 0;

    /* Calculate baud settings */
    u16Sbr = (((BusClock)>>4) + (u32Baud>>1))/u32Baud;

    /* Save off the current value of the UARTx_BDH except for the SBR field */
    u8Temp = pUART->BDH & ~(UART_BDH_SBR_MASK);

    pUART->BDH = u8Temp |  UART_BDH_SBR(u16Sbr >> 8);
    pUART->BDL = (uint8_t)(u16Sbr & UART_BDL_SBR_MASK);

    /* Enable receiver and transmitter */
    pUART->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK );
}

static void Dbg_UART_SendChar(UART_Type *pUART, uint8_t u8Char)
{
	/* Wait until space is available in the FIFO */
	while ( !(pUART->S1 & UART_S1_TDRE_MASK) )
	;

	/* Read UART_S1 register*/
	(void)pUART->S1;

	/* Send the character */
	pUART->D = (uint8_t)u8Char;
}

static uint8_t UART_GetChar(UART_Type *pUART)
{
	/* Wait until character has been received */
	while ( !(pUART->S1 & UART_S1_RDRF_MASK) )
	;

	/* Read UART_S1 register*/
	(void)pUART->S1;

	/* Return the 8-bit data from the receiver */
	return pUART->D;
}

#else

/* Wrapping all UART functions from driver */

static void Dbg_UART_Init(UART_Type *_pUART, uint32_t PbClk, uint32_t u32Baud)
{
	pUART = _pUART;
#ifdef USE_UART_INTERNAL_IMPLEMENTATION_BLOCKING
	Uart_Init( (pUART == UART0) ? 0 :( (pUART == UART1) ? 1 : 2), PbClk, u32Baud );
#else
    UART_ConfigType Uart_Config={{0}};

    Uart_Config.sctrl1settings.bits.bM=0; 	/* 8 bit Mode */
    Uart_Config.sctrl1settings.bits.bPe=0; 	/* Parity disable */
    Uart_Config.bSbns=0;					/* One stop bit*/
    Uart_Config.sctrl2settings.bits.bRe=1;	/* Receiver enable*/
    Uart_Config.sctrl2settings.bits.bTe=1;	/* Transmitter enable*/
    Uart_Config.sctrl2settings.bits.bTie=1;	/* Transmit buffer empty interrupt enable*/
    Uart_Config.u32SysClkHz = PbClk;   		/* Bus clock in Hz*/
    Uart_Config.u32Baudrate = u32Baud;     	/* Baud rate*/

    /*Initialization of UART module*/
    if(_pUART == UART2)
    {
    	//UART_SetCallback(_pUART, UART2_ISR, &Uart_Config);
    }
    else if (_pUART == UART1)
    {
    	//UART_SetCallback(_pUART, UART1_ISR, &Uart_Config);
    }
    else if (_pUART == UART0)
    {
    	//UART_SetCallback(_pUART, UART0_ISR, &Uart_Config);
    }
    UART_Init(_pUART, &Uart_Config);
#endif
}

static void Dbg_UART_SendChar(UART_Type *_pUART, uint8_t u8Char)
{
//	uint8_t tmp;
//	Uart_Write((pUART == UART0) ? 0 : ((pUART == UART1) ? 1 : 2), &u8Char, 1, &tmp);
	UART_PutChar(_pUART, u8Char);
}

static uint8_t Dbg_UART_GetChar(UART_Type *_pUART)
{
	return Dbg_UART_GetChar(_pUART);

	//TODO: !!!
//	uint8_t tmp, c;
//	Uart_Read((pUART == UART0) ? 0 : ((pUART == UART1) ? 1 : 2), &c, 1, &tmp);
//	return c;
}

#endif
