/****************************************************************************
 * @file     spi_kl25.c
 * @brief    CMSIS-like SPI driver for kinetis KL25
 * @version  1
 * @date     4. April 2015
 *
 * @note    This is simplified imitation of CMSIS driver (which does not 
 *          exist for Kinetis(?)).
 *          
 ******************************************************************************/

/* MSF includes */
#include "msf_config.h"

/* Include the core definitions - CMSIS <device>.h file etc.*/
#include "coredef.h"
#include "msf.h"

#include "spi_kea.h"


#if (MSF_DRIVER_SPI0)
/* Define the resource for each SPI available on the MCU */
/* runtime info for SPI0 */
static SPI_INFO SPI0_Info; 

/* The pins for SPI0 
 * The pins are user-configurable through msf_config.h file.
 * Each is defined by pin-code (see msf_<device>.h) and the number of the alternate
 * function (ALTn) which is the UART function for this pin. 
 *  */
static SPI_PINS SPI0_Pins = {
		{MSF_SPI0_MISO_PIN, MSF_SPI0_MISO_ALT},	/* pin for MISO */
		{MSF_SPI0_MOSI_PIN, MSF_SPI0_MOSI_ALT},	/* pin for MOSI */
		{MSF_SPI0_SCK_PIN, MSF_SPI0_SCK_ALT},	/* pin for SCK */
};

/* SPI0 Resources */

static SPI_RESOURCES SPI0_Resources = {
  SPI0,    /* UART0 type object defined in CMSIS <device.h>*/  
  &SPI0_Pins,
  &SPI0_Info
};
#endif /* MSF_DRIVER_UART0 */


#if (MSF_DRIVER_SPI1)
/* Define the resource for each SPI available on the MCU */
/* runtime info for SPI1 */
static SPI_INFO SPI1_Info; 

/* The pins for SPI1 
 * The pins are user-configurable through msf_config.h file.
 * Each is defined by pin-code (see msf_<device>.h) and the number of the alternate
 * function (ALTn) which is the SPI function for this pin. 
 *  */
static SPI_PINS SPI1_Pins = {
		{MSF_SPI1_MISO_PIN, MSF_SPI1_MISO_ALT},	/* pin for MISO */
		{MSF_SPI1_MOSI_PIN, MSF_SPI1_MOSI_ALT},	/* pin for MOSI */
		{MSF_SPI1_SCK_PIN, MSF_SPI1_SCK_ALT},	/* pin for SCK */
};

/* UART1 Resources */
static SPI_RESOURCES SPI1_Resources = {
  SPI1,
  &SPI1_Pins,
  &SPI1_Info
};

#endif /* MSF_DRIVER_UART1 */



/* Internal functions */
static void spi_setbaudrate(uint32_t baudrate, SPI_RESOURCES* spi);
static void spi0_intconfig(uint32_t enable, SPI_RESOURCES* spi);

/* The driver API functions */

/**
  \brief       Initialize SPI Interface.
  
  
  \param[in]   baudrate  baudrate constant as defined in msf_<device>.h
  \param[in]   event  Pointer to SPI_Event function or null
  \param[in]   spi    Pointer to SPI resources
  \return      error code (0 = OK). May return MSF_ERROR_ARGUMENT if baudrate is not supported.
  \note			            	  
  	  Initializes SPI for MasterMode operation, SPI mode 0, interrupts disabled
  	  
  	  Pins used for MISO/MOSI/SCK are configured in msf_config.h.
  	  
  	  Common function called by instance-specific function.
*/
static uint32_t  SPI_Initialize( SPI_speed_t baudrate, MSF_SPI_Event_t event,  SPI_RESOURCES* spi)
{	
    /* init given SPI */	
	spi->info->cb_event = event;	/* store pointer to user callback*/
	spi->info->status = MSF_SPI_STATUS_POLLED_MODE;
		
	/* Enable the SPI MISO/MOSI/SCK functions on the user-defined pins */	
	
	//if ( (spi->pins->misopin.pin_code == GPIO_INVALID_PIN) || (spi->pins->mosipin.pin_code == GPIO_INVALID_PIN) || (spi->pins->sckpin.pin_code == GPIO_INVALID_PIN))
	if ( (spi->pins->misopin.pin_code == GPIO_INVALID_PIN) || (spi->pins->mosipin.pin_code == GPIO_INVALID_PIN) || (spi->pins->sckpin.pin_code == GPIO_INVALID_PIN))	
	  return MSF_ERROR_CONFIG;
	// Hard-coded no PCR available on KEA, No PCR registers, set SPI0 to MOSI=PTB3  MISO=PTB4 SCK=PTB2
		
	SIM_PINSEL0 &=  ~SIM_PINSEL_SPI0PS_MASK; //0 = SPI0_SCK, SPI0_MOSI, SPI0_MISO, and SPI0_PCS are mapped on PTB2, PTB3, PTB4, and PTB5.
	
	//GPIO_PORT_OBJECT(spi->pins->misopin.pin_code)->PCR[GPIO_PIN_NUM(spi->pins->misopin.pin_code)] = PORT_PCR_MUX(spi->pins->misopin.alt_num);
	//GPIO_PORT_OBJECT(spi->pins->mosipin.pin_code)->PCR[GPIO_PIN_NUM(spi->pins->mosipin.pin_code)] = PORT_PCR_MUX(spi->pins->mosipin.alt_num);
	//GPIO_PORT_OBJECT(spi->pins->sckpin.pin_code)->PCR[GPIO_PIN_NUM(spi->pins->sckpin.pin_code)] = PORT_PCR_MUX(spi->pins->sckpin.alt_num);
		
	
  		/* Disable SPI before changing registers */	
		/* TODO: default data mode setup*/
	    spi->reg->C1 |= ( SPI_C1_MSTR_MASK );	/* default values */	  
		spi->reg->C2 = 0;	    /* default values */	
		spi->reg->BR = 0;	    /* default values */	
						
		
		/* set baudrate */
		spi->reg->BR = (baudrate & 0x7F);
				
		//spi0_setbaudrate((uint32_t)baudrate, spi);		
				
		/* Enable SPI */
		spi->reg->C1 |= (SPI_C1_SPE_MASK);
		
   return MSF_ERROR_OK;
}

#if (MSF_DRIVER_SPI0)
/* Instance specific function pointed-to from the driver access struct */
static uint32_t SPI0_Initialize(SPI_speed_t baudrate, MSF_SPI_Event_t pEvent) 
{
    SIM_SCGC |= SIM_SCGC_SPI0_MASK;  /* Enable clock for SPI0 */		
	if (!baudrate) baudrate = SBD4000KHZ; /* default is 4Mhz*/
    return SPI_Initialize(baudrate, pEvent, &SPI0_Resources);
}
#endif

#if (MSF_DRIVER_SPI1)
static uint32_t SPI1_Initialize(SPI_speed_t baudrate, MSF_SPI_Event_t pEvent) 
{
    SIM_SCGC |= SIM_SCGC_SPI1_MASK;  /* Enable clock for SPI1 */		
    if (!baudrate) baudrate = SBD4000KHZ; /* default is 4Mhz*/
    return SPI_Initialize(baudrate, pEvent, &SPI1_Resources);
}
#endif

/**
  \brief       Uninitialize SPI Interface. 
  \param[in]   spi    Pointer to SPI resources 
  \return      error code (0 = OK)
  \note        Common function called by instance-specific function.
  	  	  Disables the SPI.
*/
static uint32_t  SPI_Uninitialize( SPI_RESOURCES* spi)
{
    /* Reset internal state for this instance of the UART driver */
    spi->info->cb_event = null;
    spi->info->status = 0;
    
    /* Disable the SPI */
    spi->reg->C1 &= ~(SPI_C1_SPE_MASK);
    
    return MSF_ERROR_OK;
}
/* Instance specific function pointed-to from the driver access struct */
#if (MSF_DRIVER_SPI0)
static uint32_t SPI0_Uninitialize (void) 
{
  return SPI_Uninitialize(&SPI0_Resources);
}
#endif

#if (MSF_DRIVER_SPI1)
static uint32_t SPI1_Uninitialize (void) 
{
  return SPI_Uninitialize(&SPI1_Resources);
}
#endif
/**
  \brief       Control the SPI driver.
  \param[in]   control  Flags indicating what parameter(s) to set. See \ref group_uart_control_flags
  \param[in]   arg Optional argument for the command
  \param[in]   uart    Pointer to SPI resources
  \return      error code (0 = OK);	
  \note        Call to Control always stops any pending receive and transmit in interrupt mode.
  	  	  	  When changing baudrate, returns MSF_ERROR_ARGUMENT if baudrate is not available 
  	  	  	  for current F_CPU.
  	  Common function called by instance-specific function.      
*/
static uint32_t SPI_Control(uint32_t control, uint32_t arg, SPI_RESOURCES* spi)
{
	//uint32_t result;
	
	/* stop any transfer in progress */
	spi->info->status &= ~(MSF_SPI_STATUS_TRANSNOW);	
	
	/* Disable SPI interrupts */
	spi->reg->C1 &= ~(SPI_C1_SPTIE_MASK | SPI_C1_SPIE_MASK);
			
	/* Changing baudrate? */
	if ( (control & MSF_SPI_BAUD_Mask) && (arg != 0) )
	{	/* arg must be is valid baudrate value from the enum SPI_speed_t */
		/* Disable SPI before changing registers */
		spi->reg->C1 &= ~(SPI_C1_SPE_MASK);
		spi_setbaudrate((uint32_t)(SPI_speed_t)arg, spi);	  
		/* Enable SPI */
		spi->reg->C1 |= SPI_C1_SPE_MASK; 
	}
	
	
	/* Changing mode: interrupt vs polled */
	if ( (control & MSF_SPI_INTMODE_Mask) != 0 )
	{		
		if ( (control & MSF_SPI_INTMODE_Mask) == MSF_SPI_INT_MODE )
		{
			/* interrupt mode */
			/* internal status to interrupt mode */
			spi->info->status &= ~MSF_SPI_STATUS_POLLED_MODE;
			spi->info->status |= MSF_SPI_STATUS_INT_MODE;	
		
		    // ToDo: switch SPI0 SPI1 ... 
			spi0_intconfig(1, spi);	/* enable interrupt */
		}
		else
		{
			/* pollede mode */
			spi0_intconfig(0, spi);	/* disable interrupt */
			
			/* internal status to polled mode */
			spi->info->status &= ~MSF_SPI_STATUS_INT_MODE;
			spi->info->status |= MSF_SPI_STATUS_POLLED_MODE;			
		}
	}
	
	/* change data SPI mode */
	if ( control & MSF_SPI_DATAMODE_Mask)
	{
		/* Disable SPI before changing registers */
		spi->reg->C1 &= ~(SPI_C1_SPE_MASK);	
		
		if ( (control & MSF_SPI_DATAMODE_Mask) == MSF_SPI_DATA_MODE0 ) // SPI_MODE0
		{
			spi->reg->C1 &= ~(SPI_C1_CPOL_MASK | SPI_C1_CPHA_MASK);	 //CPOL,CPHA = 0						
			
		} else if  ( (control & MSF_SPI_DATAMODE_Mask) == MSF_SPI_DATA_MODE1 ) // SPI_MODE1
		  {
			spi->reg->C1 &= ~(SPI_C1_CPOL_MASK);	 //CPOL = 0
			spi->reg->C1 |= (SPI_C1_CPHA_MASK);		 //CPHA = 1	
						
		  }	 else if ( (control & MSF_SPI_DATAMODE_Mask) == MSF_SPI_DATA_MODE2 ) // SPI_MODE2
		    {
			   spi->reg->C1 |= (SPI_C1_CPOL_MASK);	 //CPOL = 1, CPHA = 0
			   spi->reg->C1 &= ~(SPI_C1_CPHA_MASK);
		    } else 
		    {
		       spi->reg->C1 |= (SPI_C1_CPOL_MASK | SPI_C1_CPHA_MASK);		 //CPOL,CPHA = 1	  
		    		   	
		    }
		/* Enable SPI */
		spi->reg->C1 |= SPI_C1_SPE_MASK; 
	}
	
	/* Change data transfer start LSB vs. MSB */
	if ( control & MSF_SPI_DATAORDER_Mask)
	{
		/* Disable SPI before changing registers */
	    spi->reg->C1 &= ~(SPI_C1_SPE_MASK);	
	    
		if ( (control & MSF_SPI_DATAORDER_Mask) == MSF_SPI_ORDER_MSB )
		{
			spi->reg->C1 &= ~(SPI_C1_LSBFE_MASK);
		} else 
		{
			spi->reg->C1 |= (SPI_C1_LSBFE_MASK);
		}
		/* Enable SPI */
		spi->reg->C1 |= SPI_C1_SPE_MASK; 
	}
		
    return MSF_ERROR_OK;
}
/* Instance specific function pointed-to from the driver access struct */
#if (MSF_DRIVER_SPI0)
static uint32_t SPI0_Control(uint32_t control, uint32_t arg) 
{
  return SPI_Control(control, arg, &SPI0_Resources);
}
#endif

#if (MSF_DRIVER_SPI1)
static uint32_t SPI1_Control(uint32_t control, uint32_t arg) 
{
  return SPI_Control(control, arg, &SPI1_Resources);
}
#endif
/**
  \brief       Send data over SPI.
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
uint8_t SPI_Transfer(uint8_t data, SPI_RESOURCES* spi)
{
	
	//uint32_t i;
	if ( spi->info->status & MSF_SPI_STATUS_POLLED_MODE )
	{
		/* Wait until transmit buffer is ready */
		while(!(spi->reg->S & SPI_S_SPTEF_MASK));
		/* Send the character */
		spi->reg->D = (data);
		while(!(spi->reg->S & SPI_S_SPRF_MASK)); //wait until Receive buffer is full
		return (spi->reg->D);	
	}
	
	else
	{
		/* non-blocking (interrupt) mode */
		/* Disable any pending transmit - this would be error to call us while in progress. */			
		spi->info->status &= ~MSF_SPI_STATUS_TRANSNOW;
		/* Disable Transmit  interrupt. If user calls (by error) Send or Receive before previous 
		 * operation is completed, the interrupt would never be disabled. */
		spi->reg->C1 &= ~ SPI_C1_SPTIE_MASK;
			
		/* Setup the internal data to start sending */
		/*
		spi->info->transbuff = (void*)data;
		spi->info->trans_total = cnt;
		spi->info->trans_cnt = 0;
		spi->info->status |= MSF_SPI_STATUS_TRANSNOW;	// now sending...
		**/		
		spi->reg->C1 |= SPI_C1_SPTIE_MASK;
		return MSF_ERROR_OK;
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
	
    
 
/* Instance specific function pointed-to from the driver access struct */
#if (MSF_DRIVER_SPI0)
static uint8_t SPI0_Transfer(const uint8_t data) 
{
  return SPI_Transfer(data, &SPI0_Resources);
}
#endif

#if (MSF_DRIVER_SPI1)
static uint32_t SPI1_Transfer(const uint8_t data)
{
  return SPI_Transfer(data, &SPI1_Resources);
}
#endif


/* Access structure for SPI0 */
#if (MSF_DRIVER_SPI0)
	MSF_DRIVER_SPI Driver_SPI0 = {
	  SPI0_Initialize,
	  SPI0_Uninitialize,
	  SPI0_Control,
	  SPI0_Transfer,
//	  UART0_GetRxCount,
//	  UART0_GetTxCount,
//	  UART0_DataAvailable,
	};
#endif /* MSF_DRIVER_UART0 */
	
/* Access structure for UART1 */
#if (MSF_DRIVER_SPI1)
	MSF_DRIVER_SPI Driver_SPI1 = {
	  SPI1_Initialize,
	  SPI1_Uninitialize,
	  SPI1_Control,
	  SPI1_Transfer,
//	  UART0_GetRxCount,
//	  UART0_GetTxCount,
//	  UART0_DataAvailable,
	};
#endif /* MSF_DRIVER_UART1 */	


	
/* Interrupt handler for SPI 0  */
void SPI_handleIRQ( SPI_RESOURCES* spi)
{	
	
	/* sanity check - are we in interrupt mode? we should not be called if not. */
	if ( (spi->info->status & MSF_SPI_STATUS_INT_MODE) == 0 )
		return;
	
	/* nothing to do if callback was not provided in Initialize() */
	/*if ( uart->info->cb_event == null )
		return;*/
	
	
	/* Tx buffer empty int. */
	/* If sending now and the Tx buffer is empty
	 * Note that it is empty all the time except when sending! */
	
	if ( (spi->info->status & MSF_SPI_STATUS_TRANSNOW) && (spi->reg->S & SPI_S_SPTEF_MASK) )
	{
		/* Send next char */			
		spi->reg->D = ((const uint8_t*)spi->info->tbuff)[spi->info->trans_cnt++];
		/* Save received char */
		((uint8_t*)(spi->info->rbuff))[spi->info->trans_cnt-1] = spi->reg->D;				
	
		/* Check if sent all we wanted */
		if ( spi->info->trans_cnt >= spi->info->trans_total )
		{
			/* stop sending */
			spi->info->status &= ~MSF_SPI_STATUS_TRANSNOW;	
			/* Disable this interrupt; the Transfer() will re-enable it */
			spi->reg->C1 &= ~ SPI_C1_SPTIE_MASK;
			
			/* generate user event */
			if ( spi->info->cb_event )
				spi->info->cb_event(MSF_SPI_EVENT_TRANSFER_COMPLETE, 0);
			/* Reset the Tx count */
			/* If we reset here, the caller will not know we have sent all!
			 * Will be reset in Send()
			 * uart->info->tx_cnt = 0; */			
		}	
		
	}
	
#if 0 
	/* Rx buffer full flag is set AND we are receiving now  */
	if ( (uart->reg->S1 & UART0_S1_RDRF_MASK) && (uart->info->status & MSF_UART_STATUS_RXNOW) )
	{		
		/* Save next byte */
		((uint8_t*)uart->info->rxbuff)[uart->info->rx_cnt++] = uart->reg->D;
		/* Check if received all we wanted */
		if ( uart->info->rx_cnt >= uart->info->rx_total )
		{
			/* stop receiving */
			uart->info->status &= ~MSF_UART_STATUS_RXNOW;
			/* Disable this interrupt; the Receive() will re-enable it when needed */
			uart->reg->C2 &= ~UART0_C2_RIE_MASK;
			/* generate user event */
			if ( uart->info->cb_event )
				uart->info->cb_event(MSF_UART_EVENT_RECEIVE_COMPLETE, 0);
			/* reset the Rx count */
			uart->info->rx_cnt = 0;
		}				
		
	}
	

	/* Rx overflow occurred? */
	if ( uart->reg->S1 & UART0_S1_OR_MASK )
	{	
		/* Clear the overflow flag */
		uart->reg->S1 |= UART0_S1_OR_MASK;		
		if ( uart->info->cb_event )
			uart->info->cb_event(MSF_UART_EVENT_RX_OVERFLOW, 0);		
	}
#endif
	
}


/* Interrupt handler for the UART0 */
#if (MSF_DRIVER_SPI0)
	void SPI0_IRQHandler()
	{
		SPI_handleIRQ( &SPI0_Resources);
		
	}
#endif

#if (MSF_DRIVER_SPI1)
	void SPI1_IRQHandler()
	{
		SPI_handleIRQ(&SPI1_Resources);	
	}
#endif


/* Internal workers */
static void spi_setbaudrate(uint32_t baudrate, SPI_RESOURCES* spi)
{
	/* write new value */  
	spi->reg->BR = (baudrate & 0x7F);
}

/* Configure interrupt for SPI0
 * enable = 0 > disable interrupt; anything else >  enablke int*/
static void spi0_intconfig(uint32_t enable, SPI_RESOURCES* spi)
{	
	if ( enable )
	{
		/* Configure SPI0 */
		
		/* Enable int when transmit data buffer empty; TDRE flag and Receiver buffer full (RDRF) */ 				
		
		/* We cannot enable the interrupts for all time, only when really Transmit-ing, otherwise the
		 * ISR is called all the time */
		
		spi->reg->C1 |=SPI_C1_SPTIE_MASK;
		spi->reg->C1 |=SPI_C1_SPIE_MASK; /*Request a hardware interrupt when SPRF or MODF is 1 ??*/
		
		/* Configure NVIC */
		NVIC_ClearPendingIRQ(SPI0_IRQn);	/* Clear possibly pending interrupt */
		NVIC_EnableIRQ(SPI0_IRQn);			/* and enable it */	
		/* Set priority for the interrupt; 0 is highest, 3 is lowest */
		NVIC_SetPriority(SPI0_IRQn, MSF_SPI0_INT_PRIORITY);
	}
	else
	{
		
		spi->reg->C1 &= ~SPI_C1_SPTIE_MASK;	/* interrupt disabled */
		spi->reg->C1 &= ~SPI_C1_SPIE_MASK;    /*  SPRF interrupt disabled*/
				
		/* Disable the interrupt in NVIC */
		NVIC_DisableIRQ(SPI0_IRQn);	
	}
}

#if 0 //This function is redundant with spi0_intconfig as the function is not SPI-specific.
/* Configure interrupt for SPI1
 * enable = 0 > disable interrupt; anything else >  enablke int*/
static void spi1_intconfig(uint32_t enable, SPI_RESOURCES* spi)
{	
	if ( enable )
	{
		/* Configure SPI1 */
		
		/* We cannot enable the interrupts for all time, only when really Transmit-ing, otherwise the
		/ * ISR is called all the time */
		
		spi->reg->C1 |=SPI_C1_SPTIE_MASK;
		spi->reg->C1 |=SPI_C1_SPIE_MASK; /*Request a hardware interrupt when SPRF or MODF is 1 ??*/
		
		/* Configure NVIC */
		NVIC_ClearPendingIRQ(SPI1_IRQn);	/* Clear possibly pending interrupt */
		NVIC_EnableIRQ(SPI1_IRQn);			/* and enable it */	
		/* Set priority for the interrupt; 0 is highest, 3 is lowest */
		NVIC_SetPriority(SPI1_IRQn, MSF_SPI0_INT_PRIORITY);
	}
	else
	{
		
		spi->reg->C1 &= ~SPI_C1_SPTIE_MASK;	/* interrupt disabled */
		spi->reg->C1 &= ~SPI_C1_SPIE_MASK;    /*  SPRF interrupt disabled*/
				
		/* Disable the interrupt in NVIC */
		NVIC_DisableIRQ(SPI1_IRQn);	
	}
}
#endif

 
/** @}*/          
/* ----------- end of file -------------- */
