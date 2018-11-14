/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2008-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************/
/**************************************************************************//**
* @addtogroup lowlevel_group
* @{
******************************************************************************/
/**************************************************************************//**
*
* @file      lin.c
*
* @author    FPT Software
*
* @brief     LIN low level functions
*
******************************************************************************/
/******************************************************************************
 *
 * History:
 *
 * 20090408		v1.0	First version
 *
 *****************************************************************************/
 
#include "lin.h"

#if (_LIN_SCI_ == 1)
#include "lin_lld_sci.h"
#include "lin_lld_timesrv.h"
#endif /* End (_LIN_SCI_ == 1) */

#if (_LIN_XGATE_ == 1)
#include "lin_lld_xgate.h"
#include "lin_lld_timesrv.h"
#endif /* End (_LIN_XGATE_ == 1) */

#if (_LIN_SLIC_== 1)
#include "lin_lld_slic.h"
#include "MC9S08EL32.h"
/* Check Multi frame transmission */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
#include "lin_lld_timesrv.h"
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
#endif /* End (_LIN_SLIC_== 1) */

#if (_LIN_GPIO_ == 1)
#include "lin_lld_gpio.h"
#endif /* End (_LIN_GPIO_ == 1) */

#if (_LIN_UART_ == 1)
#include "LIN_Driver/bsp/UART/lin_lld_uart.h"
#include "LIN_Driver/bsp/UART/lin_lld_timesrv.h"
#endif /* End (_LIN_SCI_ == 1) */
#if (LIN_MODE == _MASTER_MODE_)
/* Extern globle variable */

l_u8 lin_lld_init
  (
    /*[IN] interface identifier*/
    l_ifc_handle iii
  )
{
  lin_hardware_name lin_hw; /* Indicator of LIN HW */
  l_u8 ret = 0;

  /* Check for valid interface  */
  if(iii > LIN_NUM_OF_IFCS){
	  ret = LIN_LLD_INVALID_IFC;
  }else{

  #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
    /* Find LIN HW interface which is mounted to interface */
    lin_hw = lin_virtual_ifc[iii];
    /* Call lin_lld_xxx_init */
    #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
    if(lin_hw <= SCI5) {
      #if (_LIN_SCI_ == 1)
      lin_lld_sci_init(lin_hw,iii);
      #else
      lin_lld_xgate_init(lin_hw,iii);
      #endif /* End (_LIN_SCI_ == 1) */
    }
    #endif /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */

    #if (_LIN_SLIC_ == 1)
    if(lin_hw==SLIC) {
      lin_lld_slic_init();
    }
    #endif /* End (_LIN_SLIC_ == 1) */

  #endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

  #if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
    lin_lld_gpio_init();
  #endif /* End  (_LIN_GPIO_ == 1) */
    
#if (_LIN_UART_ == 1)  /* For Kinetis only */
  /* Find LIN HW interface which is mounted to interface */
  lin_hw = lin_virtual_ifc[iii];
  lin_lld_uart_init(lin_hw,iii);
#endif /* End  (_LIN_UART_ == 1)  */
  	ret = LIN_LLD_OK;
  }
  return ret;
}

l_u8 lin_lld_deinit
  (
    /*[IN] interface identifier*/
    l_ifc_handle iii
  )
{
  lin_hardware_name lin_hw; /* Indicator of LIN HW */
  l_u8 ret = 0;

  /* Check for valid interface  */
  if(iii > LIN_NUM_OF_IFCS){
	  ret = LIN_LLD_INVALID_IFC;
  }else{

  #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
    /* Find LIN HW interface which is mounted to interface */
    lin_hw = lin_virtual_ifc[iii];
    /* Call lin_lld_xxx_deinit */
    #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
    if(lin_hw <= SCI5) {
      #if (_LIN_SCI_ == 1)
      lin_lld_sci_deinit(lin_hw);
      #else
      lin_lld_xgate_deinit(lin_hw);
      #endif /* End (_LIN_SCI_ == 1) */
    }
    #endif /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */

    #if (_LIN_SLIC_ == 1)
    if(lin_hw==SLIC) {
      lin_lld_slic_deinit();
    }
    #endif /* End (_LIN_SLIC_ == 1) */

  #endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

  #if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
    lin_lld_gpio_deinit();
  #endif /* End (_LIN_GPIO_ == 1 */
    
#if (_LIN_UART_ == 1)  /* For Kinetis only */
    /* Find LIN HW interface which is mounted to interface */
    lin_hw = lin_virtual_ifc[iii];
    lin_lld_uart_deinit(lin_hw);
#endif /* End  (_LIN_UART_ == 1)  */
  	ret = LIN_LLD_OK;
  }
  return ret;
}

void lin_lld_tx_header
  (
    /*[IN] interface identifier*/
    l_ifc_handle iii,
    /*[IN] ID of the header to be sent */
    l_u8 pid
  )
{
  lin_hardware_name lin_hw; /* Indicator of LIN HW */

  /* Check for valid interface */
  if(iii <= LIN_NUM_OF_IFCS){

#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
  /* Find LIN HW interface which is mounted to interface */
  lin_hw = lin_virtual_ifc[iii];
  /* Call lin_lld_xxx_tx_header */
  if(lin_hw <= SCI5) {
    #if (_LIN_SCI_ == 1)
    lin_lld_sci_tx_header(lin_hw,pid);
    #else
    lin_lld_xgate_tx_header(lin_hw,pid);
    #endif /* End (_LIN_SCI_ == 1) */
  }

#endif  /* defined(_LIN_SCI_) || defined(_LIN_XGATE_) */
  
#if (_LIN_UART_ == 1)  /* For Kinetis only */
  /* Find LIN HW interface which is mounted to interface */
  lin_hw = lin_virtual_ifc[iii];
  lin_lld_uart_tx_header(lin_hw,pid);
#endif /* End  (_LIN_UART_ == 1)  */
  }
  return;
}

void lin_lld_tx_wake_up
  (
    /*[IN] interface identifier*/
    l_ifc_handle iii
  )
{
  lin_hardware_name lin_hw;

  /* Check for valid interface  */
  if(iii<=LIN_NUM_OF_IFCS){

#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
  /* Find LIN HW interface which is mounted to interface */
  lin_hw = lin_virtual_ifc[iii];
  /* Call lin_lld_xxx_tx_wake_up */
  #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
  if(lin_hw <= SCI5) {
    #if (_LIN_SCI_ == 1)
    lin_lld_sci_tx_wake_up(lin_hw);
    #else
    lin_lld_xgate_tx_wake_up(lin_hw);
    #endif /* End (_LIN_SCI_ == 1) */
  }
  #endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */

  #if (_LIN_SLIC_ == 1)
  if(lin_hw==SLIC) {
    lin_lld_slic_tx_wake_up();
  }
  #endif /* End (_LIN_SLIC_ == 1) */

#endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

#if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
  lin_lld_gpio_tx_wake_up();
#endif /* End (_LIN_GPIO_ == 1)  */
  
#if (_LIN_UART_ == 1)  /* For Kinetis only */
  /* Find LIN HW interface which is mounted to interface */
  lin_hw = lin_virtual_ifc[iii];
  lin_lld_uart_tx_wake_up(lin_hw);
#endif /* End  (_LIN_UART_ == 1)  */  
  }
  return;
}

void lin_lld_int_enable
  (
    /*[IN] interface identifier*/
    l_ifc_handle iii
  )
{
  lin_hardware_name lin_hw;

  /* Check for valid interface  */
  if(iii <= LIN_NUM_OF_IFCS){

#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
  /* Find LIN HW interface which is mounted to interface */
  lin_hw = lin_virtual_ifc[iii];
  /* Call lin_lld_xxx_int_enable */
  #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
  if(lin_hw <= SCI5) {
    #if (_LIN_SCI_ == 1)
    lin_lld_sci_int_enable(lin_hw);
    #else
    lin_lld_xgate_int_enable(lin_hw);
    #endif /* End (_LIN_SCI_ == 1) */
  }
  #endif /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */ 

  #if (_LIN_SLIC_ == 1)
  if(lin_hw==SLIC) {
    lin_lld_slic_int_enable();
  }
  #endif /* End (_LIN_SLIC_ == 1) */

#endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

#if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
    lin_lld_gpio_int_enable();    
 #endif /* (_LIN_GPIO_ == 1)  */
    
#if (_LIN_UART_ == 1)  /* For Kinetis only */
  /* Find LIN HW interface which is mounted to interface */
  lin_hw = lin_virtual_ifc[iii];
  lin_lld_uart_int_enable(lin_hw);
#endif /* End  (_LIN_UART_ == 1)  */
  }
  
  return;
}

l_u8 lin_lld_int_disable
  (
    /*[IN] interface identifier*/
    l_ifc_handle iii
  )
{
  lin_hardware_name lin_hw;
  l_u8 ret = 0;

  /* Check for valid interface  */
  if(iii>LIN_NUM_OF_IFCS){
	  ret = LIN_LLD_INVALID_IFC;
  }else{

#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
    /* Find LIN HW interface which is mounted to interface */
    lin_hw = lin_virtual_ifc[iii];
    /* Call lin_lld_xxx_int_disable */
    #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
    if(lin_hw <= SCI5) {
      #if (_LIN_SCI_ == 1)
      lin_lld_sci_int_disable(lin_hw);
      #else
      lin_lld_xgate_int_disable(lin_hw);
      #endif /* End (_LIN_SCI_ == 1) */
    }
    #endif /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */

    #if (_LIN_SLIC_ == 1)
    if(lin_hw==SLIC) {
      lin_lld_slic_int_disable();
    }
    #endif /* End (_LIN_SLIC_ == 1) */

#endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

#if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
    lin_lld_gpio_int_disable();
#endif /* (_LIN_GPIO_ == 1) */
    
#if (_LIN_UART_ == 1)  /* For Kinetis only */
  /* Find LIN HW interface which is mounted to interface */
  lin_hw = lin_virtual_ifc[iii];
  lin_lld_uart_int_disable(lin_hw);
#endif /* End  (_LIN_UART_ == 1)  */
    ret = LIN_LLD_OK;
  }
  return ret;
}

void lin_lld_ignore_response
  (
    /*[IN] interface identifier*/
    l_ifc_handle iii
  )
{
  lin_hardware_name lin_hw; /* Indicator of LIN HW */

  /* Check for valid interface  */
  if(iii<=LIN_NUM_OF_IFCS){

#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
  /* Find LIN HW interface which is mounted to interface */
  lin_hw = lin_virtual_ifc[iii];
  /* Call lin_lld_xxx_ignore_response */
  #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
  if(lin_hw <= SCI5) {
    #if (_LIN_SCI_ == 1)
    lin_lld_sci_ignore_response(lin_hw);
    #else
    lin_lld_xgate_ignore_response(lin_hw);
    #endif /* End (_LIN_SCI_ == 1) */
  }
  #endif /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */

  #if (_LIN_SLIC_ == 1)
  if(lin_hw==SLIC) {
    lin_lld_slic_ignore_response();
  }
  #endif /* End (_LIN_SLIC_ == 1) */

#endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

#if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
  lin_lld_gpio_ignore_response();
#endif /* End (_LIN_GPIO_ == 1) */
  
#if (_LIN_UART_ == 1)  /* For Kinetis only */
  /* Find LIN HW interface which is mounted to interface */
  lin_hw = lin_virtual_ifc[iii];
  lin_lld_uart_ignore_response(lin_hw);
#endif /* End  (_LIN_UART_ == 1)  */
  }

  return;
}

void lin_lld_set_low_power_mode
  (
    /*[IN] interface identifier*/
    l_ifc_handle iii
  )
{
  lin_hardware_name lin_hw; /* Indicator of LIN HW */

  /* Check for valid interface  */
  if(iii<=LIN_NUM_OF_IFCS){

#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
  /* Find LIN HW interface which is mounted to interface */
  lin_hw = lin_virtual_ifc[iii];
  /* Call lin_lld_xxx_ignore_response */
  #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
  if(lin_hw <= SCI5) {
    #if (_LIN_SCI_ == 1)
    lin_lld_sci_set_low_power_mode(lin_hw);
    #else
    lin_lld_xgate_set_low_power_mode(lin_hw);
    #endif /* End (_LIN_SCI_ == 1) */
  }
  #endif /* End  (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */

  #if (_LIN_SLIC_ == 1)
  if(lin_hw==SLIC) {
    lin_lld_slic_set_low_power_mode();
  }
  #endif /* End (_LIN_SLIC_ == 1) */

#endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

#if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
  lin_lld_gpio_set_low_power_mode();
#endif /* End (_LIN_GPIO_ == 1) */
  
#if (_LIN_UART_ == 1)  /* For Kinetis only */
  /* Find LIN HW interface which is mounted to interface */
  lin_hw = lin_virtual_ifc[iii];
  lin_lld_uart_set_low_power_mode(lin_hw);
#endif /* End  (_LIN_UART_ == 1)  */
  }
  return;
}

l_u8 lin_lld_set_response
  (
    /* [IN] interface identifier */
    l_ifc_handle iii,
    /* [IN] response length */
    l_u8 response_length
  )
{
  lin_hardware_name lin_hw; /* Indicator of LIN HW */
  l_u8 ret = 0;

  /* Check for valid interface */
  if(iii>LIN_NUM_OF_IFCS){
	  ret = LIN_LLD_INVALID_IFC;
  }else{
	  /* Check for valid response length */
	  if(response_length>8){
		  ret = LIN_LLD_INVALID_PARA;
	  }else{
  	  /* Put response length into buffer */
  	  *(lin_ifc_configuration[iii].response_buffer) = response_length;

  	#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
  	  /* Find LIN HW interface which is mounted to interface */
  	  lin_hw = lin_virtual_ifc[iii];
  	  /* Call lin_lld_xxx_set_response */
  	  #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
  	  if(lin_hw <= SCI5) {
  		#if (_LIN_SCI_ == 1)
  		lin_lld_sci_tx_response(lin_hw);
  		#else
  		lin_lld_xgate_tx_response(lin_hw);
  		#endif /* End (_LIN_SCI_ == 1) */
  	  }
  	 #endif /*End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */

  	  #if (_LIN_SLIC_ == 1)
  	  if(lin_hw==SLIC) {
  		/* Add code here */
  		lin_lld_slic_tx_response(&lin_ifc_configuration[iii].response_buffer[0]);
  	  }
  	  #endif /* End (_LIN_SLIC_ == 1) */

  	#endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

  	#if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
  	  /* Add code here */
  	  lin_lld_gpio_tx_response();
  	#endif /* End (_LIN_GPIO_ == 1) */
  	  
#if (_LIN_UART_ == 1)  /* For Kinetis only */
  /* Find LIN HW interface which is mounted to interface */
  lin_hw = lin_virtual_ifc[iii];
  lin_lld_uart_tx_response(lin_hw);
#endif /* End  (_LIN_UART_ == 1)  */
      ret = LIN_LLD_OK;
    }
  }
  
  return ret;
}

l_u8 lin_lld_rx_response
  (
    /* [IN] interface identifier */
    l_ifc_handle iii,
    /* [IN] response length */
    l_u8 response_length
  )
{
  lin_hardware_name lin_hw; /* Indicator of LIN HW */
  l_u8 ret = 0;

  /* Check for valid interface  */
  if(iii > LIN_NUM_OF_IFCS){
	  ret = LIN_LLD_INVALID_IFC;
  }else{

#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
    /* Find LIN HW interface which is mounted to interface */
    lin_hw = lin_virtual_ifc[iii];
    /* Call lin_lld_xxx_rx_response */
#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
    if(lin_hw <= SCI5) {
    #if (_LIN_SCI_ == 1)
      lin_lld_sci_rx_response(lin_hw,response_length);
    #else
      lin_lld_xgate_rx_response(lin_hw,response_length);
    #endif /* End (_LIN_SCI_ == 1) */
    }
  #endif /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */

  #if (_LIN_SLIC_ == 1)
    if(lin_hw==SLIC) {
      lin_lld_slic_rx_response(response_length);
    }
  #endif /* End (_LIN_SLIC_ == 1) */

#endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

#if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
    /* Add code here */
    lin_lld_gpio_rx_response(response_length);
#endif /* End (_LIN_GPIO_ == 1) */

#if (_LIN_UART_ == 1)  /* For Kinetis only */
  /* Find LIN HW interface which is mounted to interface */
  lin_hw = lin_virtual_ifc[iii];
  lin_lld_uart_rx_response(lin_hw,response_length);
#endif /* End  (_LIN_UART_ == 1)  */
    ret = LIN_LLD_OK;
  }
  return ret;
}

#endif /* end of LIN_MODE == _MASTER_MODE_ */


#if LIN_MODE == _SLAVE_MODE_
/* Extern globle variable */

l_u8 lin_lld_init
  (
  )
{
#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
  /* Call lin_lld_xxx_init */
  #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
  if(lin_virtual_ifc <= SCI5) {
    #if (_LIN_SCI_ == 1)
    lin_lld_sci_init((l_ifc_handle)0);
    #else
    lin_lld_xgate_init((l_ifc_handle)0);
    #endif /* End (_LIN_SCI_ == 1) */
  }
  #endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */

  #if (_LIN_SLIC_ == 1)
  if(lin_virtual_ifc == SLIC) {
    lin_lld_slic_init();
  }
  #endif /* End (_LIN_SLIC_ == 1) */

#endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

#if (_LIN_UART_ == 1)  /* For Kinetis platform */
  lin_lld_uart_init((l_ifc_handle)0);
#endif /* End (_LIN_UART_ == 1) */  
  
#if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
  lin_lld_gpio_init();
#endif /* End (_LIN_GPIO_ == 1) */
  return LIN_LLD_OK;
}

l_u8 lin_lld_deinit
  (
  )
{
  /* Check for valid interface  */
#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
  /* Call lin_lld_xxx_deinit */
  #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
  if(lin_virtual_ifc <= SCI5) {
    #if (_LIN_SCI_ == 1)
    lin_lld_sci_deinit();
    #else
    lin_lld_xgate_deinit();
    #endif /* End (_LIN_SCI_ == 1) */
  }
  #endif /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */

  #if (_LIN_SLIC_ == 1)
  if(lin_virtual_ifc == SLIC) {
    lin_lld_slic_deinit();
  }
  #endif /* End (_LIN_SLIC_ == 1) */

#endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

#if (_LIN_UART_ == 1)  /* For Kinetis platform */
  lin_lld_uart_deinit();
#endif /* End (_LIN_UART_ == 1) */  
  
#if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
  lin_lld_gpio_deinit();
#endif /* End (_LIN_GPIO_ == 1) */
  return LIN_LLD_OK;
}

l_u8 lin_lld_get_status
  (
  )
{
  l_u8 ret = 0;

  /* Check for valid interface  */
#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
  /* Call lin_lld_xxx_get_status */
  #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
  if(lin_virtual_ifc <= SCI5) {
    #if (_LIN_SCI_ == 1)
    ret = lin_lld_sci_get_status();
    #else
    ret = lin_lld_xgate_get_status();
    #endif /* End (_LIN_SCI_ == 1) */
  }
  #endif /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */

  #if (_LIN_SLIC_ == 1)
  if(lin_virtual_ifc==SLIC) {
    /* Add code here */
  }
  #endif /* End (_LIN_SLIC_ == 1) */

#endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

#if (_LIN_UART_ == 1)  /* For Kinetis platform */
  lin_lld_uart_get_status();
#endif /* End (_LIN_UART_ == 1) */  
  
#if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
  /* Add code here */
#endif /* End (_LIN_GPIO_ == 1) */
  return ret;
}

l_u8 lin_lld_get_state()
{
  l_u8 ret = 0;
  /* Check for valid interface  */
#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
  /* Call lin_lld_xxx_get_status */
  #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
  if(lin_virtual_ifc <= SCI5) {
    #if (_LIN_SCI_ == 1)
    ret = lin_lld_sci_get_state();
    #else
    ret = lin_lld_xgate_get_state();
    #endif /* End (_LIN_SCI_ == 1) */
  }
  #endif /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */

  #if (_LIN_SLIC_ == 1)
  if(lin_virtual_ifc==SLIC) {
    ret = lin_lld_slic_get_state();
  }
  #endif /* End (_LIN_SLIC_ == 1) */

#endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

#if (_LIN_UART_ == 1)  /* For Kinetis platform */
  lin_lld_uart_get_state();
#endif /* End (_LIN_UART_ == 1) */  
  
#if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
  ret = lin_lld_gpio_get_status();
#endif /* End (_LIN_GPIO_ == 1) */
  return ret;

}

void lin_lld_tx_wake_up
  (
  )
{
  /* Check for valid interface  */
#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
  /* Call lin_lld_xxx_tx_wake_up */
  #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
  if(lin_virtual_ifc <= SCI5) {
    #if (_LIN_SCI_ == 1)
    lin_lld_sci_tx_wake_up();
    #else
    lin_lld_xgate_tx_wake_up();
    #endif /* End (_LIN_SCI_ == 1) */
  }
  #endif /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */

  #if (_LIN_SLIC_ == 1)
  if(lin_virtual_ifc == SLIC) {
    lin_lld_slic_tx_wake_up();
  }
  #endif /* End (_LIN_SLIC_ == 1) */

#endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

#if (_LIN_UART_ == 1)  /* For Kinetis platform */
  lin_lld_uart_tx_wake_up();
#endif /* End (_LIN_UART_ == 1) */   
  
#if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
  lin_lld_gpio_tx_wake_up();
#endif /* End (_LIN_GPIO_ == 1) */
  return;
}

void lin_lld_int_enable
  (
  )
{
  /* Check for valid interface  */
#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
  /* Call lin_lld_xxx_int_enable */
  #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
  if(lin_virtual_ifc <= SCI5) {
    #if (_LIN_SCI_ == 1)
    lin_lld_sci_int_enable();
    #else
    lin_lld_xgate_int_enable();
    #endif /* End (_LIN_SCI_ == 1) */
  }
  #endif /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */

  #if (_LIN_SLIC_ == 1)
  if(lin_virtual_ifc == SLIC) {
    lin_lld_slic_int_enable();
  }
  #endif /* End (_LIN_SLIC_ == 1) */ 

#endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

#if (_LIN_UART_ == 1)  /* For Kinetis platform */
  lin_lld_uart_int_enable();
#endif /* End (_LIN_UART_ == 1) */     
  
    #if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
      lin_lld_gpio_int_enable();    
    #endif /* End #if (_LIN_GPIO_ == 1) */
  return;
}

l_u8 lin_lld_int_disable
  (
  )
{
  /* Check for valid interface  */
#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
  /* Call lin_lld_xxx_int_disable */
  #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
  if(lin_virtual_ifc <= SCI5) {
    #if (_LIN_SCI_ == 1)
    lin_lld_sci_int_disable();
    #else
    lin_lld_xgate_int_disable();
    #endif /* End (_LIN_SCI_ == 1) */
  }
  #endif /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */

  #if (_LIN_SLIC_ == 1)
  if(lin_virtual_ifc==SLIC) {
    lin_lld_slic_int_disable();
  }
  #endif /* End (_LIN_SLIC_ == 1) */

#endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

#if (_LIN_UART_ == 1)  /* For Kinetis platform */
  lin_lld_uart_int_disable();
#endif /* End (_LIN_UART_ == 1) */     
  
#if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
  lin_lld_gpio_int_disable();
#endif  /* End (_LIN_GPIO_ == 1)  */
  return LIN_LLD_OK;
}

void lin_lld_ignore_response
  (
  )
{
#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
  /* Call lin_lld_xxx_ignore_response */
  #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
  if(lin_virtual_ifc <= SCI5) {
    #if (_LIN_SCI_ == 1)
    lin_lld_sci_ignore_response();
    #else
    lin_lld_xgate_ignore_response();
    #endif /* End (_LIN_SCI_ == 1) */
  }
  #endif /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */

  #if (_LIN_SLIC_ == 1)
  if(lin_virtual_ifc == SLIC) {
    lin_lld_slic_ignore_response();
  }
  #endif /* End (_LIN_SLIC_ == 1) */

#endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

#if (_LIN_UART_ == 1)  /* For Kinetis platform */
  lin_lld_uart_ignore_response();
#endif /* End (_LIN_UART_ == 1) */    
  
#if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
  lin_lld_gpio_ignore_response();
#endif /* End (_LIN_GPIO_ == 1) */
  return;
}

void lin_lld_set_low_power_mode
  (
  )
{
#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
  /* Call lin_lld_xxx_ignore_response */
  #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
  if(lin_virtual_ifc <= SCI5) {
    #if (_LIN_SCI_ == 1)
    lin_lld_sci_set_low_power_mode();
    #else
    lin_lld_xgate_set_low_power_mode();
    #endif /* End (_LIN_SCI_ == 1) */
  }
  #endif /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */

  #if (_LIN_SLIC_ == 1)
  if(lin_virtual_ifc == SLIC) {
    lin_lld_slic_set_low_power_mode();
  }
  #endif /* End (_LIN_SLIC_ == 1) */

#endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

#if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
  lin_lld_gpio_set_low_power_mode();
#endif  /* End (_LIN_GPIO_ == 1)  */
  return;
}

l_u8 lin_lld_set_response
  (
    /* [IN] response length */
    l_u8 response_length
  )
{
  l_u8 ret = 0;

  /* Check for valid response length */
  if(response_length>8){
    ret = LIN_LLD_INVALID_PARA;
  }else{
	  /* Put response length into buffer */
	  lin_lld_response_buffer[0] = response_length;

	#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
	  /* Call lin_lld_xxx_set_response */
	  #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
	  if(lin_virtual_ifc <= SCI5) {
		  #if (_LIN_SCI_ == 1)
		    lin_lld_sci_tx_response();
		  #else
		    lin_lld_xgate_tx_response();
		  #endif /* End (_LIN_SCI_ == 1) */
	  }
	  #endif /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */

	  #if (_LIN_SLIC_ == 1)
	  if(lin_virtual_ifc==SLIC) {
		/* Add code here */
		  lin_lld_slic_tx_response(&lin_lld_response_buffer[0]);
	  }
	  #endif /* End (_LIN_SLIC_ == 1) */

	#endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

	  
	#if (_LIN_UART_ == 1)  /* For Kinetis platform */
	  lin_lld_uart_tx_response();
    #endif /* End (_LIN_UART_ == 1) */
	  	  
	#if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
	  /* Add code here */
	  lin_lld_gpio_tx_response();
	#endif /* End (_LIN_GPIO_ == 1) */
    ret = LIN_LLD_OK;
  }
  return ret;
}

l_u8 lin_lld_rx_response
  (
    /* [IN] response length */
    l_u8 response_length
  )
{
#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
  /* Call lin_lld_xxx_rx_response */
  #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
  if(lin_virtual_ifc <= SCI5) {
    #if (_LIN_SCI_ == 1)
    lin_lld_sci_rx_response(response_length);
    #else
    lin_lld_xgate_rx_response(response_length);
    #endif /* End (_LIN_SCI_ == 1) */
  }
  #endif /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */

  #if (_LIN_SLIC_ == 1)
  if(lin_virtual_ifc == SLIC) {
    /* Add code here */
    lin_lld_slic_rx_response(response_length);
  }
  #endif /* End (_LIN_SLIC_ == 1) */

#endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

#if (_LIN_UART_ == 1)  /* For Kinetis platform */
  lin_lld_uart_rx_response(response_length);
#endif /* End (_LIN_UART_ == 1) */  
  
#if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
  /* Add code here */
  lin_lld_gpio_rx_response(response_length);
#endif /* End (_LIN_GPIO_ == 1) */
  return LIN_LLD_OK;
}

#endif /* End of LIN_MODE == _SLAVE_MODE_ */

/*** Common function used to all low-level driver ***/

l_u8 lin_process_parity
(
    /* [IN] PID need to be send*/
    l_u8 pid,
    /* [IN] Check or Make parity*/
    l_u8 type
)
{
  l_u8 parity;
  l_u8 ret;

  parity =(((BIT(pid,0)^BIT(pid,1)^BIT(pid,2)^BIT(pid,4))<<6)|
          ((~(BIT(pid,1)^BIT(pid,3)^BIT(pid,4)^BIT(pid,5)))<<7));  
  if (CHECK_PARITY == type){
    if((pid&0xC0) != parity){
  	  ret = 0xFF;
    }else{
    	ret = (l_u8)(pid&0x3F);
    }
  } else {
    ret = (l_u8)(pid|parity);
  }

  return (ret);  
}

l_u8 lin_checksum
  (
    /* [IN] pointer to buffer */
    l_u8 *buffer,
    /* [IN] pid */
    l_u8 raw_pid
  )
{
  l_u8 length;
  l_u16 check_sum;

  /* 1. PID correspond to Master request and Slave response, their checksum cal is classic
  the non-diagnostic frame is calculated in Enhanced */
  if((0x3C!=raw_pid) && (0x7D!=raw_pid)){
    check_sum = raw_pid;
  }else{
	check_sum = 0;
  }

  for (length = *buffer; 0<length; length--) {
	buffer++;
    check_sum += *(buffer);
    /* 2. to deal with the carry  */
    if (check_sum > 0xFF){
		check_sum -= 0xFF;
	}
  }

  /* 3. to reverse */
  return (l_u8)(~check_sum);
}

void lin_lld_timer_init
  (
	  void
  )
{
  #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)

  #if (_MCU_ == _S12X_)
  /* if use PIT for time service */
  lin_lld_timer_S12X_init();
  #endif /* End (_MCU_ == _S12X_) */

  #if (_MCU_ == _S12_)
  lin_lld_timer_S12_init();
  #endif  /* End (_MCU_ == _S12_) */

  #if (_MCU_ == _S08_)
  lin_lld_timer_S08_init();
  #endif /* End (_MCU_ == _S08_) */

  #endif
  #if (_LIN_SLIC_ == 1)
  /* Init timer for SLIC interface, which using for check transport layer timeout */
  /* Use for multi frames transmission only. */
  #if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
  lin_lld_timer_S08_init();
  #endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
  
  #endif /* End (_LIN_SLIC_ == 1) */
  
#if (_MCU_ == _K_)
lin_lld_timer_K_init();
#endif  /* End (_MCU_ == _S12_) */
}


#if (LIN_MODE == _SLAVE_MODE_)
void lin_lld_mcu_reset()  
{
  
#if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1)
  /* Call lin_lld_xxx_rx_response */
  #if (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1)
  if(lin_virtual_ifc <= SCI5) {
    #if (_LIN_SCI_ == 1)
       
    #else
    
    #endif /* End (_LIN_SCI_ == 1) */
  }
  #endif /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) */

  #if (_LIN_SLIC_ == 1)
  if(lin_virtual_ifc==SLIC) {
    /* Add code here */
    
  }
  #endif /* End (_LIN_SLIC_ == 1) */

#endif  /* End (_LIN_SCI_ == 1) || (_LIN_XGATE_ == 1) || (_LIN_SLIC_ == 1) */

#if (_LIN_GPIO_ == 1)  /* For S09QD4 only */
  /* Add code here */
  
#endif /* End (_LIN_GPIO_ == 1) */
}
#endif /* End (LIN_MODE == _SLAVE_MODE_) */

/**
* @}
*/
