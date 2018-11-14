/****************************************************************************
 * @file     uart_kea.c
 * @brief    CMSIS-like UART driver for kinetis KEA
 * @version  1
 * @date     6. May 2015
 *
 * @note    This is simplified imitation of CMSIS driver (which does not 
 *          exist for Kinetis(?)).
 *          See the definition of the baudrate enum in msf_<device>.h.
 *          The UART_Initialize will return MSF_ERROR_ARGUMENT if called with unsupported 
 *          baudrate.              
 *
 ******************************************************************************/
/** @addtogroup group_uart  
 * @{
 * @note 
 * See the definition of the baudrate enum in msf_[device].h.
 * The UART_Initialize will return MSF_ERROR_ARGUMENT if called with unsupported 
 * baudrate.               
*/

/* MSF includes */
#include "msf_config.h"

/* Include the core definitions - CMSIS <device>.h file etc.*/
#include "coredef.h"
#include "msf.h"

#include "uart_kea.h"


#if (MSF_DRIVER_UART0)
/* Define the resource for each UART available on the MCU */
/* runtime info for UART0 */
static UART_INFO UART0_Info; 

/* The pins for UART0 
 * The pins are user-configurable through msf_config.h file.
 * Each is defined by pin-code (see msf_<device>.h) and the number of the alternate
 * function (ALTn) which is the UART function for this pin. 
 *  */
static UART_PINS UART0_Pins = {
		{MSF_UART0_RX_PIN, MSF_UART0_RX_ALT},	/* pin for Rx */
		{MSF_UART0_TX_PIN, MSF_UART0_TX_ALT},	/* pin for Tx */
};

/* UART0 Resources */
static UART_RESOURCES UART0_Resources = {
  UART0,    /* UART0 type object defined in CMSIS <device.h>*/
  &UART0_Pins,
  &UART0_Info
};
#endif /* MSF_DRIVER_UART0 */


#if (MSF_DRIVER_UART1)
/* Define the resource for each UART available on the MCU */
/* runtime info for UART1 */
static UART_INFO UART1_Info; 

/* The pins for UART1 
 * The pins are user-configurable through msf_config.h file.
 * Each is defined by pin-code (see msf_<device>.h) and the number of the alternate
 * function (ALTn) which is the UART function for this pin. 
 *  */
static UART_PINS UART1_Pins = {
		{MSF_UART1_RX_PIN, MSF_UART1_RX_ALT},	/* pin for Rx */
		{MSF_UART1_TX_PIN, MSF_UART1_TX_ALT},	/* pin for Tx */
};

/* UART1 Resources */
static UART_RESOURCES UART1_Resources = {
  UART1,
  &UART1_Pins,
  &UART1_Info
};

#endif /* MSF_DRIVER_UART1 */

#if (MSF_DRIVER_UART2)
/* Define the resource for each UART available on the MCU */
/* runtime info for UART2 */
static UART_INFO UART2_Info; 

/* The pins for UART1 
 * The pins are user-configurable through msf_config.h file.
 * Each is defined by pin-code (see msf_<device>.h) and the number of the alternate
 * function (ALTn) which is the UART function for this pin. 
 *  */
static UART_PINS UART2_Pins = {
		{MSF_UART2_RX_PIN, MSF_UART2_RX_ALT},	/* pin for Rx */
		{MSF_UART2_TX_PIN, MSF_UART2_TX_ALT},	/* pin for Tx */
};

/* UART2 Resources */
static UART_RESOURCES UART2_Resources = {
  UART2,
  &UART2_Pins,
  &UART2_Info
};

#endif /* MSF_DRIVER_UART2 */


/* Internal functions */
//static void uart0_setbaudrate(uint32_t baudrate, UART_RESOURCES* uart);
//static void uart0_intconfig(uint32_t enable, UART_RESOURCES* uart);
static uint32_t uart_setbaudrate(uint32_t baudrate, UART_RESOURCES* uart);
static void uart_intconfig(uint32_t enable, UART_RESOURCES* uart);

/* The driver API functions */

/**
  \brief       Initialize UART Interface.
  \param[in]   baudrate  baudrate constant as defined in msf_<device>.h
  \param[in]   event  Pointer to UART_Event function or null
  \param[in]   uart       Pointer to UART resources
  \return      error code (0 = OK). May return MSF_ERROR_ARGUMENT if baudrate is not supported.
  \note			            	  
  	  Initializes UART for 8N1 operation, no parity, interrupts disabled, and
  	  no hardware flow-control.
  	  Pins used for Rx and Tx are configured in msf_config.h.
  	  Note that the clock for the port used by the Rx and Tx pins must be enabled by the 
  	  caller before calling this function.
  	  
  	  Common function called by instance-specific function.
*/
static uint32_t  UART_Initialize( UART_speed_t baudrate, MSF_UART_Event_t event,  UART_RESOURCES* uart)
{	
    /* init given UART */	
	uart->info->cb_event = event;	/* store pointer to user callback*/
	uart->info->status = MSF_UART_STATUS_POLLED_MODE;
		
	/* Enable clock for PORTA needed for Tx, Rx pins */
	//SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;			
	/* Enable the UART_RXD function on PTA1 */	
	//GPIO_PORT_OBJECT(GPIO_A1)->PCR[GPIO_PIN_NUM(GPIO_A1)] = PORT_PCR_MUX(2);
	/* Enable the UART_TXD function on PTA2 */	
	//GPIO_PORT_OBJECT(GPIO_A2)->PCR[GPIO_PIN_NUM(GPIO_A2)] = PORT_PCR_MUX(2);
	
	/* Enable the UART Rx and Tx functions on the user-defined pins */	
	if ( uart->pins->rxpin.pin_code == GPIO_INVALID_PIN
			|| uart->pins->txpin.pin_code == GPIO_INVALID_PIN)
		return MSF_ERROR_CONFIG;
	// PCR registers not available on KEA!!! 
	
	if (uart->reg == UART0)
	{
#if defined(MCU_SKEAZ1284)
		SIM_PINSEL0 = uart->pins->rxpin.alt_num ? SIM_PINSEL0 | SIM_PINSEL_UART0PS_MASK : SIM_PINSEL0 & ~SIM_PINSEL_UART0PS_MASK;
#elif defined(MCU_SKEAZN642)
		/*Migrated from KEAZ128 to KEAZN64. KEAZ128 has 3 uarts; KEAZN64 only has one. Comment out conditions for UART1 and UART2*/
		SIM_PINSEL = uart->pins->rxpin.alt_num ? SIM_PINSEL | SIM_PINSEL_UART0PS_MASK : SIM_PINSEL & ~SIM_PINSEL_UART0PS_MASK;
#else
#endif
	}
#if defined(MCU_SKEAZ1284) || defined(MCU_SKEAZN642) //SKEAZN642 have UART1 and UART2 but no alt pins for them
	else if (uart->reg == UART1)
	{
#if defined(MCU_SKEAZ1284)
		SIM_PINSEL1 = uart->pins->rxpin.alt_num ? SIM_PINSEL1 | SIM_PINSEL1_UART1PS_MASK : SIM_PINSEL1 & ~SIM_PINSEL1_UART1PS_MASK;
#else
		/* No PINSEL option for SKEAZN642 UART1. */
#endif
	}
	else /* UART 2*/ 
	{
#if defined(MCU_SKEAZ1284)
		SIM_PINSEL1 = uart->pins->rxpin.alt_num ? SIM_PINSEL1 | SIM_PINSEL1_UART2PS_MASK : SIM_PINSEL1 & ~SIM_PINSEL1_UART2PS_MASK;
#else
		/* No PINSEL option for SKEAZN642 UART2. */
#endif
	}
#else
	/* If the board you are using has more than 1 UART, add them here. */
#endif

	
    if ( uart->reg == 0 )
			return MSF_ERROR_CONFIG;	/* UART_RESOURCES not defined properly */
				
	uart->reg->C2 = 0;	/* default values */	  
	uart->reg->C1 = 0;	/* default values */	
	uart->reg->C3 = 0;	/* default values */
	uart->reg->BDH = 0;	/* default value including 1 stop bit */
				
	if ( !uart_setbaudrate((uint32_t)baudrate, uart) )
			return MSF_ERROR_ARGUMENT;		/* It will return zero if the baudrate is not supported */
		
	/* Enable receiver and transmitter */
	uart->reg->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK );
    return MSF_ERROR_OK;
}

#if (MSF_DRIVER_UART0)
/* Instance specific function pointed-to from the driver access struct */
static uint32_t UART0_Initialize(UART_speed_t baudrate, MSF_UART_Event_t pEvent) 
{
  SIM->SCGC |= SIM_SCGC_UART0_MASK;
  return UART_Initialize(baudrate, pEvent, &UART0_Resources);
}
#endif

#if (MSF_DRIVER_UART1)
static uint32_t UART1_Initialize(UART_speed_t baudrate, MSF_UART_Event_t pEvent) 
{
	/* Enable the clock for UART1 */
	SIM->SCGC |= SIM_SCGC_UART1_MASK;	
	return UART_Initialize(baudrate, pEvent, &UART1_Resources);
}
#endif

#if (MSF_DRIVER_UART2)
static uint32_t UART2_Initialize(UART_speed_t baudrate, MSF_UART_Event_t pEvent) 
{
	/* Enable the clock for UART2 */
	SIM->SCGC |= SIM_SCGC_UART2_MASK;	
	return UART_Initialize(baudrate, pEvent, &UART2_Resources);
}
#endif

/**
  \brief       Uninitialize UART Interface. 
  \param[in]   uart    Pointer to UART resources 
  \return      error code (0 = OK)
  \note        Common function called by instance-specific function.
  	  	  Disables the UART.
*/
static uint32_t  UART_Uninitialize( UART_RESOURCES* uart)
{
    /* Reset internal state for this instance of the UART driver */
    uart->info->cb_event = null;
    uart->info->status = 0;
    
    /* Disable the UART */
   	uart->reg->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
    return MSF_ERROR_OK;
}
/* Instance specific function pointed-to from the driver access struct */

#if (MSF_DRIVER_UART0)
static uint32_t UART0_Uninitialize (void) 
{
  return UART_Uninitialize(&UART0_Resources);
}
#endif

#if (MSF_DRIVER_UART1)
static uint32_t UART1_Uninitialize (void) 
{
  return UART_Uninitialize(&UART1_Resources);
}
#endif

#if (MSF_DRIVER_UART2)
static uint32_t UART2_Uninitialize (void) 
{
  return UART_Uninitialize(&UART2_Resources);
}
#endif
/**
  \brief       Change the power mode of the UART.
  \param[in]   state  The requested power state
  \param[in]   uart    Pointer to UART resources
  \return      error code (0 = OK)
  \note        Common function called by instance-specific function.
                Currently does nothing!
*/
static uint32_t UART_PowerControl(MSF_power_state state, UART_RESOURCES* uart)
{
    return MSF_ERROR_NOTSUPPORTED;
} 

/* Instance specific function pointed-to from the driver access struct */

#if (MSF_DRIVER_UART0)
static uint32_t UART0_PowerControl(MSF_power_state state) 
{
  return UART_PowerControl(state, &UART0_Resources);
}
#endif

#if (MSF_DRIVER_UART1)
static uint32_t UART1_PowerControl(MSF_power_state state) 
{
  return UART_PowerControl(state, &UART1_Resources);
}
#endif

#if (MSF_DRIVER_UART2)
static uint32_t UART2_PowerControl(MSF_power_state state) 
{
  return UART_PowerControl(state, &UART2_Resources);
}
#endif

/**
  \brief       Control the UART driver.
  \param[in]   control  Flags indicating what parameter(s) to set. See \ref group_uart_control_flags
  \param[in]   arg Optional argument for the command
  \param[in]   uart    Pointer to UART resources
  \return      error code (0 = OK);	
  \note        Call to Control always stops any pending receive and transmit in interrupt mode.
  	  	  	  When changing baudrate, returns MSF_ERROR_ARGUMENT if baudrate is not available 
  	  	  	  for current F_CPU.
  	  Common function called by instance-specific function.      
*/
static uint32_t UART_Control(uint32_t control, uint32_t arg, UART_RESOURCES* uart)
{
	uint32_t result;
	
	/* stop any transfer in progress */
	uart->info->status &= ~(MSF_UART_STATUS_TXNOW | MSF_UART_STATUS_RXNOW);	
	
	/* Disable Tx and Rx interrupts */
	uart->reg->C2 &= ~(UART_C2_TIE_MASK | UART_C2_RIE_MASK);
		
	
	/* Changing baudrate? */
	if ( (control & MSF_UART_BAUD_Mask) && (arg != 0) )
	{	/* arg must be is valid baudrate value from the enum UART_speed_t */
		uart->reg->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
		result = uart_setbaudrate((uint32_t)(UART_speed_t)arg, uart);	  
		/* Enable receiver and transmitter */
		uart->reg->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK );
		if ( result == 0 )
			return MSF_ERROR_ARGUMENT;	/* baudrate not supported for current F_CPU */
	}
	
	/* Changing mode: interrupt vs polled */
	if ( (control & MSF_UART_INTMODE_Mask) != 0 )
	{		
		if ( (control & MSF_UART_INTMODE_Mask) == MSF_UART_INT_MODE )
		{
			/* interrupt mode */
			
			/* internal status to interrupt mode */
			uart->info->status &= ~MSF_UART_STATUS_POLLED_MODE;
			uart->info->status |= MSF_UART_STATUS_INT_MODE;
			uart_intconfig(1, uart);	/* enable interrupt */
		}
		else
		{
			/* pollede mode */
			uart_intconfig(0, uart);	/* disable interrupt */
			
			/* internal status to polled mode */
			uart->info->status &= ~MSF_UART_STATUS_INT_MODE;
			uart->info->status |= MSF_UART_STATUS_POLLED_MODE;			
		}
	}
	
	/* Abort current Send or Receive command */
	if ( control & MSF_UART_ABORTTX_Mask)
	{
		/* stop any transmit in progress */
		uart->info->status &= ~MSF_UART_STATUS_TXNOW;	 	
		/* Disable Tx Interrupt */
		uart->reg->C2 &= ~UART_C2_TIE_MASK;
	}
	
	if ( control & MSF_UART_ABORTRX_Mask)
	{
		/* stop any receive in progress */
		uart->info->status &= ~MSF_UART_STATUS_RXNOW;	
		/* Disable Tx and Rx interrupts */
		uart->reg->C2 &= ~UART_C2_RIE_MASK;
	}
	
	/* Change parity */
	if ( control & MSF_UART_PARITY_Mask )
	{	
		/* Disable UART0 before changing registers */
		uart->reg->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
					
		if ( (control & MSF_UART_PARITY_Mask) == MSF_UART_PARITY_NONE )
		{
			/* no parity */
			uart->reg->C1 &= ~(UART_C1_PT_MASK | UART_C1_PE_MASK);
		}
		else if ( (control & MSF_UART_PARITY_Mask) == MSF_UART_PARITY_EVEN )
		{
			/* even parity */
			uart->reg->C1 |= UART_C1_PE_MASK;	/* PE = 1 > parity check enabled */
			uart->reg->C1 &= ~UART_C1_PT_MASK;	/* PT = 0 */				
		}
		else
		{	/* odd parity */
			uart->reg->C1 |= (UART_C1_PE_MASK | UART_C1_PT_MASK);	/* PE = 1 > parity check enabled */
		}
			
		/* Enable receiver and transmitter */
		uart->reg->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK );
	}
	
	/* Change number of stop bits */
	if ( control & MSF_UART_STOPBIT_Mask )
	{
		if ( (control & MSF_UART_STOPBIT_Mask) == MSF_UART_STOP_BITS_2 )
		{
			uart->reg->BDH |= UART_BDH_SBNS_MASK;	/* 2 stop bits */
		}
		else
		{
			uart->reg->BDH &= ~UART_BDH_SBNS_MASK; /* 1 stop bit */
		}
	}
	
	/* Change number of data bits 
	 * NOTE that we do not support sending 9 bits; the 9-bit option can only be used
	 * to send 8 data bits and parity bit if parity is enabled. */
	if ( control & MSF_UART_DATA_BITS_Mask )
	{
		if ( (control & MSF_UART_DATA_BITS_Mask) == MSF_UART_DATA_BITS_9 )
		{
			uart->reg->C1 |= UART_C1_M_MASK;	/* 9 data bits */
		}
		else
		{
			uart->reg->C1 &= ~UART_C1_M_MASK; /* 8 data bits */
				
		}
	}
		
	
	
    return MSF_ERROR_OK;
}
/* Instance specific function pointed-to from the driver access struct */
#if (MSF_DRIVER_UART0)
static uint32_t UART0_Control(uint32_t control, uint32_t arg) 
{
  return UART_Control(control, arg, &UART0_Resources);
}
#endif
#if (MSF_DRIVER_UART1)
static uint32_t UART1_Control(uint32_t control, uint32_t arg) 
{
  return UART_Control(control, arg, &UART1_Resources);
}
#endif

#if (MSF_DRIVER_UART2)
static uint32_t UART2_Control(uint32_t control, uint32_t arg) 
{
  return UART_Control(control, arg, &UART2_Resources);
}
#endif
/**
  \brief       Send data over UART.
  \param[in]   data pointer to buffer to send
  \param[in]   cnt number of bytes to send
  \param[in]   uart    Pointer to UART resources
  \return      error code (0 = OK)
  \note        
        Send array of bytes to serial line. Note that it blocks the caller
        until the data are sent in polled mode.
        If interrupt mode is enabled, the function sends 1st byte and then returns to caller.        
        The next bytes are sent "in the background" using interrupt handler. 
        NOTE that the memory pointed to by "data" must be still available (do not use local variable in caller)!
        The caller is notified by MSF_UART_EVENT_SEND_COMPLETE event when send is complete.
            
        Common function called by instance-specific function.
*/
uint32_t UART_Send(const void* data, uint32_t cnt, UART_RESOURCES* uart)
{
	uint32_t i;
	if ( uart->info->status & MSF_UART_STATUS_POLLED_MODE )
	{	/* blocking (polled) mode */
		for ( i =0; i< cnt; i++ )	
		{			
			while(!(uart->reg->S1 & UART_S1_TDRE_MASK));
			uart->reg->D = ((const uint8_t*)data)[i];		
		}
		
	}
	else
	{
		/* non-blocking (interrupt) mode */
		//if ( cnt > 1 )	/* If there is only 1 byte, we just send it below... */
		{
			/* Disable any pending transmit - this would be error to call us while in progress. */			
			uart->info->status &= ~MSF_UART_STATUS_TXNOW;
			/* Disable Tx  interrupt. If user calls (by error) Send or Receive before previous 
			 * operation is completed, the interrupt would never be disabled. */
		    uart->reg->C2 &= ~UART_C2_TIE_MASK;
			
			/* Setup the internal data to start sending */
			uart->info->txbuff = (void*)data;
			uart->info->tx_total = cnt;
			uart->info->tx_cnt = 0;
			uart->info->status |= MSF_UART_STATUS_TXNOW;	/* now sending... */
			uart->reg->C2 |= UART_C2_TIE_MASK;
		}
#if 0
		/* Send first character now; the next one will be sent by ISR */ 
		/* Wait until space is available in the FIFO */
		while(!(uart->reg->S1 & UART0_S1_TDRE_MASK))
			 ;
		 /* Send the character */
		uart->reg->D = ((const uint8_t*)data)[0];
		/* Enable interrupt for Tx buffer empty */
		if ( cnt > 1 )
			uart->reg->C2 |= UART0_C2_TIE_MASK;
#endif
	}
	
    return MSF_ERROR_OK;
} 
/* Instance specific function pointed-to from the driver access struct */

#if (MSF_DRIVER_UART0)
static uint32_t UART0_Send(const void* data, uint32_t cnt) 
{
  return UART_Send(data, cnt, &UART0_Resources);
}
#endif

#if (MSF_DRIVER_UART1)
static uint32_t UART1_Send(const void* data, uint32_t cnt) 
{
  return UART_Send(data, cnt, &UART1_Resources);
}
#endif

#if (MSF_DRIVER_UART2)
static uint32_t UART2_Send(const void* data, uint32_t cnt) 
{
  return UART_Send(data, cnt, &UART2_Resources);
}
#endif
/**
  \brief       Receive data from UART.
  \param[in]   data pointer to buffer which receives the data
  \param[in]   cnt number of bytes to receive
  \param[in]   uart    Pointer to UART resources
  \return      error code (0 = OK)
  \note        
        Receive one or mode bytes from serial line. 
        In polled mode (MSF_UART_POLLED_MODE) it blocks the caller until specified number 
        of characters is received.
        In interrupt mode it returns to caller immediately and receives in the background (in ISR).
        NOTE that the memory pointed to by "data" must be still available (do not use local variable in caller)!
        When the "cnt" number of bytes it received the caller is notified by MSF_UART_EVENT_RECEIVE_COMPLETE event.

        Common function called by instance-specific function.
*/
uint32_t UART_Receive(void* data, uint32_t cnt, UART_RESOURCES* uart)
{
	uint32_t i = 0;
	if ( uart->info->status & MSF_UART_STATUS_POLLED_MODE )
	{	/* blocking (polled) mode */
		
		/* Clear overrun flag 
		 It is possible Receive was not called for some time and we missed some 
		 characters. When Overrun is set, we do not get the receiver data full flag? */
		//uart->reg->S1 |= UART_S1_OR_MASK;
		for ( i=0; i<cnt; i++ )
		{		
			/* Wait until character has been received */
			while (!(uart->reg->S1 & UART_S1_RDRF_MASK))
				;
				 
			/* store the 8-bit data from the receiver */
			((uint8_t*)data)[i] = uart->reg->D;
		}			
	}
		
	else
	{	/* non-blocking (interrupt) mode */
	 			
		/* Disable any pending receive - this would be error to call us while in progress. */
		uart->info->status &= ~MSF_UART_STATUS_RXNOW;	
		/* Disable Rx interrupt. If user calls (by error) Send or Receive before previous 
		* operation is complete, the interrupt would never be disabled. */
		uart->reg->C2 &= ~UART_C2_RIE_MASK;
		
		/* Setup the internal data to start receiving */
		uart->info->rxbuff = data;
		uart->info->rx_total = cnt;
		uart->info->rx_cnt = 0;
		uart->info->status |= MSF_UART_STATUS_RXNOW;	/* now receiving... */
		uart->reg->C2 |= UART_C2_RIE_MASK; /* Enable interrupt for Rx buffer full */		 
	}
	
    return MSF_ERROR_OK;
} 
/* Instance specific function pointed-to from the driver access struct */
#if (MSF_DRIVER_UART0)
static uint32_t UART0_Receive(void* data, uint32_t cnt) 
{
  return UART_Receive(data, cnt, &UART0_Resources);
}
#endif

#if (MSF_DRIVER_UART1)
static uint32_t UART1_Receive(void* data, uint32_t cnt) 
{
  return UART_Receive(data, cnt, &UART1_Resources);
}
#endif

#if (MSF_DRIVER_UART2)
static uint32_t UART2_Receive(void* data, uint32_t cnt) 
{
  return UART_Receive(data, cnt, &UART2_Resources);
}
#endif

/**
  \brief       Get number of bytes received so far during Receive operation in interrupt mode
  \param[in]   uart    Pointer to UART resources
  \return      number of bytes received so far
  \note        
          Common function called by instance-specific function.
*/
static uint32_t UART_GetRxCount(UART_RESOURCES* uart)
{
	if ( uart->info->status & MSF_UART_STATUS_POLLED_MODE )
		return 0;
	return uart->info->rx_cnt;	
}

#if (MSF_DRIVER_UART0)
static uint32_t UART0_GetRxCount(void)
{
	return UART_GetRxCount(&UART0_Resources);
}
#endif

#if (MSF_DRIVER_UART1)
static uint32_t UART1_GetRxCount(void)
{
	return UART_GetRxCount(&UART1_Resources);
}
#endif

#if (MSF_DRIVER_UART2)
static uint32_t UART2_GetRxCount(void)
{
	return UART_GetRxCount(&UART2_Resources);
}
#endif
/**
  \brief       Get number of bytes sent so far during Send operation in interrupt mode
  \param[in]   uart    Pointer to UART resources
  \return      number of bytes sent so far
  \note        
          Common function called by instance-specific function.
*/
static uint32_t UART_GetTxCount(UART_RESOURCES* uart)
{
	if ( uart->info->status & MSF_UART_STATUS_POLLED_MODE )
		return 0;
	return uart->info->tx_cnt;
}

#if (MSF_DRIVER_UART0)
static uint32_t UART0_GetTxCount(void)
{
	return UART_GetTxCount(&UART0_Resources);
}
#endif
#if (MSF_DRIVER_UART1)
static uint32_t UART1_GetTxCount(void)
{
	return UART_GetTxCount(&UART1_Resources);
}
#endif
#if (MSF_DRIVER_UART2)
static uint32_t UART2_GetTxCount(void)
{
	return UART_GetTxCount(&UART2_Resources);
}
#endif
/**
  \brief       Check is there is some byte received
  \param[in]   uart    Pointer to UART resources 
  \return      number of bytes available (0 or 1 as we do not have any buffer)
  \note        Common function called by instance-specific function.
*/
static uint32_t  UART_DataAvailable( UART_RESOURCES* uart)
{
	if ( (uart->reg->S1 & UART_S1_RDRF_MASK) != 0 )
			return 1;
	return 0;
}
/* Instance specific function pointed-to from the driver access struct */
#if (MSF_DRIVER_UART0)
static uint32_t UART0_DataAvailable(void) 
{
  return UART_DataAvailable(&UART0_Resources);
}	
#endif
#if (MSF_DRIVER_UART1)
static uint32_t UART1_DataAvailable(void) 
{
  return UART_DataAvailable(&UART1_Resources);
}	
#endif

#if (MSF_DRIVER_UART2)
static uint32_t UART2_DataAvailable(void) 
{
  return UART_DataAvailable(&UART2_Resources);
}	
#endif

/* Access structure for UART0 */
#if (MSF_DRIVER_UART0)
	MSF_DRIVER_USART Driver_UART0 = {
	  UART0_Initialize,
	  UART0_Uninitialize,
	  UART0_PowerControl,
	  UART0_Control,  
	  UART0_Send,
	  UART0_Receive,  
	  UART0_GetRxCount,
	  UART0_GetTxCount,
	  UART0_DataAvailable,
	};
#endif /* MSF_DRIVER_UART0 */
	
/* Access structure for UART1 */
#if (MSF_DRIVER_UART1)
	MSF_DRIVER_USART Driver_UART1 = {
		  UART1_Initialize,
		  UART1_Uninitialize,
		  UART1_PowerControl,
		  UART1_Control,  
		  UART1_Send,
		  UART1_Receive,  
		  UART1_GetRxCount,
		  UART1_GetTxCount,
		  UART1_DataAvailable,
		};
#endif /* MSF_DRIVER_UART1 */	

/* Access structure for UART2 */
#if (MSF_DRIVER_UART2)
	MSF_DRIVER_USART Driver_UART2 = {
		  UART2_Initialize,
		  UART2_Uninitialize,
		  UART2_PowerControl,
	  	  UART2_Control,  
		  UART2_Send,
		  UART2_Receive,  
		  UART2_GetRxCount,
		  UART2_GetTxCount,
		  UART2_DataAvailable,
	};
#endif /* MSF_DRIVER_UART2 */	

/* Software flags for easier programming. */

char char_received=0;
char char_transmitted=0;

void UART_handleIRQ( UART_RESOURCES* uart)
{	
	volatile uint8_t dummy;
	/* sanity check - are we in interrupt mode? we should not be called if not. */
	if ( (uart->info->status & MSF_UART_STATUS_INT_MODE) == 0 )
		return;
	
	/* nothing to do if callback was not provided in Initialize() */
	/*if ( uart->info->cb_event == null )
		return;*/
	
	
	/* Tx buffer empty int. */
	/* If sending now and the Tx buffer is empty
	 * Note that it is empty all the time except when sending! */
	if ( (uart->info->status & MSF_UART_STATUS_TXNOW) && (uart->reg->S1 & UART_S1_TDRE_MASK) )
	{
		/* Send next char */			
		uart->reg->D = ((const uint8_t*)uart->info->txbuff)[uart->info->tx_cnt++];
		/* Check if sent all we wanted */
		if ( uart->info->tx_cnt >= uart->info->tx_total )
		{
			/* stop sending */
			uart->info->status &= ~MSF_UART_STATUS_TXNOW;	
			/* Disable this interrupt; the Send() will re-enable it */
			uart->reg->C2 &= ~UART_C2_TIE_MASK;
			/* generate user event */
			if ( uart->info->cb_event )
				uart->info->cb_event(MSF_UART_EVENT_SEND_COMPLETE, 0);
			/* Reset the Tx count */
			/* If we reset here, the caller will not know we have sent all!
			 * Will be reset in Send()
			 * uart->info->tx_cnt = 0; */

			/* Enable Transmit complete interrupt  - to generate event when line is idle */
			uart->reg->C2 |= UART_C2_TCIE_MASK;	
		}	
		char_transmitted=1;
	}
	

	/* Transmit complete int.  */
	/* If Tx complete int is enabled and the flag is set.
	 * Note that the flag is set all the time if th eline is idle, so we use the int enable bit
	 * to chech if we are interested in this event now */
	if ( (uart->reg->C2 & UART_C2_TCIE_MASK) && (uart->reg->S1 & UART_S1_TC_MASK) )
	{
		/* disable the TC interrupt */
		uart->reg->C2 &= ~UART_C2_TCIE_MASK;	
		if ( uart->info->cb_event )
			uart->info->cb_event(MSF_UART_EVENT_TRANSFER_COMPLETE, 0);
	}

	
	/* Rx buffer full flag is set AND we are receiving now  */
	if ( (uart->reg->S1 & UART_S1_RDRF_MASK) && (uart->info->status & MSF_UART_STATUS_RXNOW) )
	{		
		/* Save next byte */
		((uint8_t*)uart->info->rxbuff)[uart->info->rx_cnt++] = uart->reg->D;
		/* Check if received all we wanted */
		if ( uart->info->rx_cnt >= uart->info->rx_total )
		{
			/* stop receiving */
			uart->info->status &= ~MSF_UART_STATUS_RXNOW;
			/* Disable this interrupt; the Receive() will re-enable it when needed */
			uart->reg->C2 &= ~UART_C2_RIE_MASK;
			/* generate user event */
			if ( uart->info->cb_event )
				uart->info->cb_event(MSF_UART_EVENT_RECEIVE_COMPLETE, 0);
			/* reset the Rx count */
			uart->info->rx_cnt = 0;
		}				
		char_received=1;
	}
	

	/* Rx overflow occurred? */
	if ( uart->reg->S1 & UART_S1_OR_MASK )
	{	
		/* Clear the overflow flag */
		dummy = uart->reg->S1;
		dummy = uart->reg->D;	
		if(dummy){}
		
		if ( uart->info->cb_event )
			uart->info->cb_event(MSF_UART_EVENT_RX_OVERFLOW, 0);		
	}
	
}

#if 0
/* Common interrupt handler for UART1 and 2 */
void UART_handleIRQ( UART_RESOURCES* uart)
{		
	/* sanity check - are we in interrupt mode? we should not be called if not. */
	if ( (uart->info->status & MSF_UART_STATUS_INT_MODE) == 0 )
		return;
	
	/* Tx buffer empty int. */
	/* If sending now and the Tx buffer is empty
	 * Note that it is empty all the time except when sending! */
	if ( (uart->info->status & MSF_UART_STATUS_TXNOW) && (uart->reg1->S1 & UART_S1_TDRE_MASK) )
	{
		/* Send next char */			
		uart->reg1->D = ((const uint8_t*)uart->info->txbuff)[uart->info->tx_cnt++];
		/* Check if sent all we wanted */
		if ( uart->info->tx_cnt >= uart->info->tx_total )
		{
			/* stop sending */
			uart->info->status &= ~MSF_UART_STATUS_TXNOW;	
			/* Disable this interrupt; the Send() will re-enable it */
			uart->reg1->C2 &= ~UART_C2_TIE_MASK;
			/* generate user event */
			if ( uart->info->cb_event )
				uart->info->cb_event(MSF_UART_EVENT_SEND_COMPLETE, 0);
			/* Reset the Tx count */
			uart->info->tx_cnt = 0;
			/* Enable Transmit complete interrupt  - to generate event when line is idle */
			uart->reg1->C2 |= UART_C2_TCIE_MASK;	
		}			
	}
	

	/* Transmit complete int.  */
	/* If Tx complete int is enabled and the flag is set.
	 * Note that the flag is set all the time if th eline is idle, so we use the int enable bit
	 * to chech if we are interested in this event now */
	if ( (uart->reg1->C2 & UART_C2_TCIE_MASK) && (uart->reg1->S1 & UART_S1_TC_MASK) )
	{
		/* disable the TC interrupt */
		uart->reg1->C2 &= ~UART_C2_TCIE_MASK;	
		if ( uart->info->cb_event )
			uart->info->cb_event(MSF_UART_EVENT_TRANSFER_COMPLETE, 0);
	}

	
	/* Rx buffer full flag is set AND we are receiving now  */
	if ( (uart->reg1->S1 & UART_S1_RDRF_MASK) && (uart->info->status & MSF_UART_STATUS_RXNOW) )
	{		
		/* Save next byte */
		((uint8_t*)uart->info->rxbuff)[uart->info->rx_cnt++] = uart->reg1->D;
		/* Check if received all we wanted */
		if ( uart->info->rx_cnt >= uart->info->rx_total )
		{
			/* stop receiving */
			uart->info->status &= ~MSF_UART_STATUS_RXNOW;
			/* Disable this interrupt; the Receive() will re-enable it when needed */
			uart->reg1->C2 &= ~UART_C2_RIE_MASK;
			/* generate user event */
			if ( uart->info->cb_event )
				uart->info->cb_event(MSF_UART_EVENT_RECEIVE_COMPLETE, 0);
			/* reset the Rx count */
			uart->info->rx_cnt = 0;
		}						
	}
	

	/* Rx overflow occurred? */
	if ( uart->reg1->S1 & UART_S1_OR_MASK )
	{	
		/* Clear the overflow flag. For UART1/2 this is done by reading the data register */
		if ( uart->info->cb_event )
			uart->info->cb_event(MSF_UART_EVENT_RX_OVERFLOW, uart->reg1->D);		
	}
	
}
#endif 

/* Interrupt handler for the UART0 */
#if (MSF_DRIVER_UART0)
	void UART0_IRQHandler()
	{
		UART_handleIRQ( &UART0_Resources);
		
	}
#endif

#if (MSF_DRIVER_UART1)
	void UART1_IRQHandler()
	{
		UART_handleIRQ( &UART1_Resources);	
	}
#endif

#if (MSF_DRIVER_UART2)

	void UART2_IRQHandler()
	{
		UART_handleIRQ( &UART2_Resources);
	}
#endif


/* Internal workers */
#if 0
static void uart0_setbaudrate(uint32_t baudrate, UART_RESOURCES* uart)
{
	uint32_t osr_val;
	uint32_t sbr_val;
	uint32_t reg_temp = 0;

	osr_val = UART_GET_OSR(baudrate);
	sbr_val = UART_GET_BR(baudrate);
	/*uart->reg->C5 = 0;*/
	// If the OSR is between 4x and 8x then both
	// edge sampling MUST be turned on.  
	if ((osr_val >3) && (osr_val < 9))
		uart->reg->C5 |= UART0_C5_BOTHEDGE_MASK;
	else
		uart->reg->C5 &= ~UART0_C5_BOTHEDGE_MASK;
			
	// Setup OSR value 
	reg_temp = uart->reg->C4;
	reg_temp &= ~UART0_C4_OSR_MASK;
	reg_temp |= UART0_C4_OSR(osr_val-1);		
	// Write reg_temp to C4 register
	uart->reg->C4 = reg_temp;
	
	/* just set C4 to defaults and write our OSR value */
	/*	uart->reg->C4 = 0;	
		uart->reg->C4 |= UART0_C4_OSR(osr_val-1);	
	This does not work; maybe cannot write 0s to OSR field? */
	
	
	/* Save current value of uartx_BDH except for the SBR field */
	reg_temp = uart->reg->BDH & ~(UART0_BDH_SBR(0x1F));
	/* write new value */  
	uart->reg->BDH = reg_temp |  UART0_BDH_SBR(((sbr_val & 0x1F00) >> 8));
	uart->reg->BDL = (uint8_t)(sbr_val & UART0_BDL_SBR_MASK);
}



/* Configure interrupt for UART0 
 * enable = 0 > disable interrupt; anything else >  enablke int*/
static void uart0_intconfig(uint32_t enable, UART_RESOURCES* uart)
{	
	if ( enable )
	{
		/* Configure UART0 */
		/* Enable int when transmit data buffer empty; TDRE flag and Receiver buffer full (RDRF) */ 				
		/* uart->reg->C2 |= UART0_C2_TIE_MASK | UART0_C2_RIE_MASK; */
		/* We cannot enable the interrupts for all time, only when really Tx-ing or Rx-ing, otherwise the
		 * ISR is called all the time */
		uart->reg->C3 |= UART0_C3_ORIE_MASK;	/* Rx overflow interrupt enabled */
		
		/* Configure NVIC */
		NVIC_ClearPendingIRQ(UART0_IRQn);	/* Clear possibly pending interrupt */
		NVIC_EnableIRQ(UART0_IRQn);			/* and enable it */	
		/* Set priority for the interrupt; 0 is highest, 3 is lowest */
		NVIC_SetPriority(UART0_IRQn, MSF_UART0_INT_PRIORITY);
	}
	else
	{
		uart->reg->C3 &= ~UART0_C3_ORIE_MASK;	/* Rx overflow interrupt enabled */
		
		/* Disable the interrupt in NVIC */
		NVIC_DisableIRQ(UART0_IRQn);	
	}
}
#endif



static uint32_t uart_setbaudrate(uint32_t baudrate, UART_RESOURCES* uart)
{
	uint32_t sbr_val, reg_temp;
	
	/* assert(uart->reg == 0) calling us for uart0 is error */
	sbr_val = UART_GET_BR_UART1(baudrate);	
	if (sbr_val == 0 )
		return 0;
	/* Save current value of uartx_BDH except for the SBR field */
	reg_temp = uart->reg->BDH & ~(UART_BDH_SBR(0x1F));
	/* write new value */  
	uart->reg->BDH = reg_temp |  UART_BDH_SBR(((sbr_val & 0x1F00) >> 8));
	uart->reg->BDL = (uint8_t)(sbr_val & UART_BDL_SBR_MASK);
	return 1;
}

/* enable = 0 > disable interrupt; anything else >  enablke int*/
static void uart_intconfig(uint32_t enable, UART_RESOURCES* uart)
{	
	/* assert(uart->reg == 0) */
	uint32_t irqn;
	irqn = MSF_UART_GETNVIC_IRQn(uart->reg);	
	
	if ( irqn < 0 )
		return;		/* Invalid IRQ number; invalid uart->reg1 pointer? */
	
	if ( enable )
	{
		/* Configure UART1 or 2*/
		/* Enable int when transmit data buffer empty; TDRE flag and Receiver buffer full (RDRF) */ 						
		/* We cannot enable the interrupts for all time, only when really Tx-ing or Rx-ing, otherwise the
		 * ISR is called all the time */
		uart->reg->C3 |= UART_C3_ORIE_MASK;	/* Rx overflow interrupt enabled */
		
		/* Configure NVIC */
		NVIC_ClearPendingIRQ(irqn);	/* Clear possibly pending interrupt */
		NVIC_EnableIRQ(irqn);			/* and enable it */	
		/* Set priority for the interrupt; 0 is highest, 3 is lowest */
		NVIC_SetPriority(irqn, MSF_UART12_INT_PRIORITY);
	}
	else
	{
		uart->reg->C3 &= ~UART_C3_ORIE_MASK;	/* Rx overflow interrupt enabled */
		
		/* Disable the interrupt in NVIC */
		NVIC_DisableIRQ(irqn);	
	}
}






/* ------- Simple functions (a C-like driver for the UART) --------------- */
#if 0
/* not used for performance reasons */

/** Write single byte to SCI
  * @param byte of data to write
  **/
 static void uart_write(uint8_t data)
 {
	 /* Wait until space is available in the FIFO */
	 while(!(UART0->S1 & UART0_S1_TDRE_MASK))
		 ;
	    
	 /* Send the character */
	 UART0->D = data;
 }
 
 /** Read byte form SCI; wait for data to arrive!
  * @return byte received from SCI.
  **/
 static uint8_t uart_read(void)
 {
	 /* Wait until character has been received */
	 while (!(UART0->S1 & UART0_S1_RDRF_MASK))
		 ;
	     
	 /* Return the 8-bit data from the receiver */
	 return UART0->D;
 }
 
 /** Check if data are available for reading
  * @return true if there are data 
 **/
 static bool uart_data_available(void)
 {
	 return ((UART0->S1 & UART0_S1_RDRF_MASK) != 0);
 }
#endif
 
/** @}*/          
/* ----------- end of file -------------- */
