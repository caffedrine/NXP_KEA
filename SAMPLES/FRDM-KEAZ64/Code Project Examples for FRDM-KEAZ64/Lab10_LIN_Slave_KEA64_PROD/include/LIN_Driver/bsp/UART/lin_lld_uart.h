/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2008-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************/
/**************************************************************************//**
* @addtogroup UART_group
* @{
******************************************************************************/
/**************************************************************************//**
*
* @file      bsp/UART/lin_lld_uart.h
*
* @author    FPT Software
*
* @brief     UART for LIN network
*
******************************************************************************/  

/******************************************************************************
 *
 * History:
 *
 * 20140516		v1.0	First version
 *
 *****************************************************************************/
 
#ifndef __LIN_LLD_UART
#define __LIN_LLD_UART

/* MODULE LIN_LLD_UART. */

#include "LIN_Driver/bsp/UART/lin_reg.h"
#include "lin_cfg/lin_cfg.h"
#include "LIN_Driver/lowlevel/lin.h"


/*** MARCOS ***/

/* Lin status bit mask */
#define LIN_STA_SUCC_TRANSFER           1         /**< LIN status bit mask: success transfer */
#define LIN_STA_ERROR_RESP              2         /**< LIN status bit mask: error in response */
#define LIN_STA_BUS_ACTIVITY            4         /**< LIN status bit mask: bus activity */
#define LIN_STA_FRAME_ERR               8         /**< LIN status bit mask: frame error */
#define LIN_STA_CHECKSUM_ERR            16        /**< LIN status bit mask: checksum error */
#define LIN_STA_READBACK_ERR            32        /**< LIN status bit mask: readback error */
#define LIN_STA_PARITY_ERR              64        /**< LIN status bit mask: parity error */
#define LIN_STA_RESET                   128       /**< LIN status bit mask: reset */


#if (LIN_MODE == _SLAVE_MODE_)
/* Low-level API prototype */

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_uart_init(l_ifc_handle iii)
* @brief UART init
*
* @param iii <B>[IN]</B> lin interface handle
*
* @SDD_ID LIN_SDD_168
* @endif
*
* @return #void
*
* @local_var
*   -# <B>#l_u16</B> <I>temp</I> 
*
*
* @details Init UART for LIN network
*   
*
* @see #lin_goto_idle_state
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_init(l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_deinit()
* @brief UART deinit
*
* @SDD_ID LIN_SDD_169
* @endif
*
* @return #void
*
* @details
*   Deinit UART for LIN network
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_deinit(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_tx_wake_up()
* @brief This function requests to send wake up signal via UART channel
*
*
* @SDD_ID LIN_SDD_170
* @endif
*
* @return #void
*
*
* @details
*   This function requests to send wake up signal via UART channel
*   Send 4Tbit to 64 Tbit 
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_tx_wake_up(void); 

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_int_enable()
* @brief This function enables UART Interrupt
*
* @SDD_ID LIN_SDD_171
* @endif
*
* @return #void
*
* @details
*   This function enables UART Interrupt
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_int_enable(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_int_disable()
* @brief This function disables UART Interrupt
*
* @SDD_ID LIN_SDD_172
* @endif
*
* @return #void
*
* @details
*   This function disables UART Interrupt
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_int_disable(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_ignore_response()
* @brief Set low level ignore flag lin_lld_ignore_flag
*
* @SDD_ID LIN_SDD_173
* @endif
*
* @return #void
*
* @details
*   Set low level ignore flag lin_lld_ignore_flag
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_ignore_response(void); 

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_set_low_power_mode()
* @brief Request to enter low power mode in UART Driver
*
* @SDD_ID LIN_SDD_174
* @endif
*
* @return #void
*
* @details
*   Request to enter low power mode in UART Driver
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_set_low_power_mode(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_rx_response(l_u8  msg_length)
*
* @param msg_length <B>IN</B> length of response data expect to wait
*
* @brief This function requests to receive response data of LIN frame via UART channel
*
* @SDD_ID LIN_SDD_175
* @endif
*
* @return #void
*
* @details
*   This function requests to receive response data of LIN frame via UART channel
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_rx_response(l_u8  msg_length); 

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_tx_response()
*
* @brief This function requests to send first data and set lin_lld_state to SEND_DATA
*
* @SDD_ID LIN_SDD_179
* @endif
*
* @return #void
*
*
* @details
*   This function requests to send first data and set lin_lld_state to SEND_DATA
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_tx_response(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_lld_UART_get_status()
*
* @brief Get LIN status ( return lin_status )  
*
* @SDD_ID LIN_SDD_180
* @endif
*
* @return #l_u8
*
* @details
*   Get LIN status ( return lin_status )  
*//*END*----------------------------------------------------------------------*/
l_u8 lin_lld_uart_get_status(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_lld_UART_get_state()
*
* @brief This function is to get state of Low level layer   
*
* @SDD_ID LIN_SDD_181
* @endif
*
* @return #l_u8
*
* @details
*   This function is to get state of Low level layer
*//*END*----------------------------------------------------------------------*/
l_u8 lin_lld_uart_get_state(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_timeout()
*
* @brief This function is called in period Tbit in order to check timeout for IDLE and MAXFRAME 
*
* @SDD_ID LIN_SDD_
* @endif
*
* @return #void
*
* @details
*   This function is called in period Tbit in order to check timeout for IDLE and MAXFRAME 
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_timeout(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_err_isr ( )
* @brief UART Interrupt service routine that handles UART Error
*   
* @return #void
* 
* @SDD_ID LIN_SDD_187
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>UART_flag_sr1</I> 
*   -# <B>#l_u8</B> <I>tmp_byte</I> 
*
* @static_global_var
*   -# <B>#pUART</B>
*   -# <B>#l_status</B>
*   -# <B>#state</B>
*   -# <B>#l_ifc_handle</B> 
*   -# <B>#current_id</B> 
*   -# <B>#ifc</B> 
*
* @details
*   UART Interrupt service routine that handles UART Error
*
* @see  #lin_goto_idle_state 
*//*END*----------------------------------------------------------------------*/ 
void lin_lld_uart_err_isr(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_uart_rx_isr ( )
* @brief UART Interrupt service routine that handles RX LIN Communication 
*   
* @return #void
* 
* @SDD_ID LIN_SDD_188
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>UART_flag_sr1</I> 
*   -# <B>#l_u8</B> <I>UART_flag_sr2</I> 
*   -# <B>#l_u8</B> <I>tmp_byte</I> 
*
* @static_global_var
*   -# <B>#pUART</B>
*   -# <B>#l_status</B>
*   -# <B>#frame_timeout_cnt</B>
*   -# <B>#lin_max_frame_res_timeout_val</B> 
*   -# <B>#current_id</B> 
*   -# <B>#ifc</B> 
*   -# <B>#l_ifc_handle</B> 
*   -# <B>#response_buffer</B> 
*
* @details
*   UART Interrupt service routine that handles RX LIN Communication 
*
* @see  #lin_checksum 
*//*END*----------------------------------------------------------------------*/ 
void lin_lld_uart_rx_isr(void);

/*** INTERNAL FUNCTIONS ***/

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_goto_idle_state()
*
* @brief Enter IDLE state  
*
* @SDD_ID LIN_SDD_182
* @endif
*
* @return #void
*
* @details
*   Enter IDLE state  
*//*END*----------------------------------------------------------------------*/
void lin_goto_idle_state(void);


#endif /* End (LIN_MODE == _SLAVE_MODE_) */

#if LIN_MODE == _MASTER_MODE_

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_init(UART_channel_name  channel, l_ifc_handle iii)
*
* @param channel <B>IN</B> UART channel name
* @param iii <B>IN</B> LIN interface name
*
* @brief UART Init
*
* @SDD_ID LIN_SDD_97
* @endif
*
* @return #void
*
* @local_var
*   -# <B>#l_u16</B> <I>*tmp</I>
*   -# <B>#lin_node</B> <I>*lnode_p</I>
*   -# <B>#lin_configuration</B> <I>*lconf_p</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @see lin_goto_idle_state
*
* @details
*   This function is inited UART for LIN network
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_init(uart_channel_name  channel, l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_deinit(UART_channel_name  channel)
*
* @param channel <B>IN</B> UART channel name
*
* @brief UART deinit
*
* @SDD_ID LIN_SDD_98
* @endif
*
* @return #void
*
*
* @static_global_var
*   -# <B>#lin_node</B>
*   -# <B>#lnode_p</B>
*
* @see lin_lld_UART_int_disable
* @see lin_node_descrs
*
* @details
*   This function is deinited UART
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_deinit(uart_channel_name  channel);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_tx_header(UART_channel_name  channel, l_u8  pid_id)
*
* @param channel <B>IN</B> UART channel name
* @param pid_id <B>IN</B> PID to be send
*
* @brief This function requests to send header of LIN frame via UART channel
*
* @SDD_ID LIN_SDD_99
* @endif
*
* @return #void
*
* @local_var
*   -# <B>#lin_node</B> <I>*lnode_p</I>
*
*
* @see lin_process_parity
* @see lin_node_descrs
*
* @details
*   This function requests to send header of LIN frame via UART channel
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_tx_header(uart_channel_name  channel, l_u8  pid_id);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_tx_wake_up(UART_channel_name  channel)
*
* @param channel <B>IN</B> UART channel name
*
* @brief This function requests to send wake up signal via UART channel.
*
* @SDD_ID LIN_SDD_100
* @endif
*
* @return #void
*
* @local_var
*   -# <B>#lin_node</B> <I>*lnode_p</I>
*
* @static_global_var
*   -# <B>#lin_node_descrs</B>
*
* @see lin_goto_idle_state
*
* @details
*   This function requests to send wake up signal via UART channel.
*   Send 4Tbit to 64 Tbit 
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_tx_wake_up(uart_channel_name  channel); 

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_int_enable(UART_channel_name  channel)
*
* @param channel <B>IN</B> UART channel name
*
* @brief This function enables UART Interrupt
*
* @SDD_ID LIN_SDD_101
* @endif
*
* @return #void
*
* @local_var
*   -# <B>#lin_node</B> <I>*lnode_p</I>
*
* @static_global_var
*   -# <B>#lin_node_descrs</B>
*
*
* @details
*   This function enables UART Interrupt
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_int_enable(uart_channel_name  channel);              

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_int_disable(UART_channel_name  channel)
*
* @param channel <B>IN</B> UART channel name
*
* @brief This function disables UART Interrupt
*
* @SDD_ID LIN_SDD_102
* @endif
*
* @return #void
*
* @local_var
*   -# <B>#lin_node</B> <I>*lnode_p</I>
*
* @static_global_var
*   -# <B>#lin_node_descrs</B>
*
*
* @details
*   This function disables UART Interrupt
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_int_disable(uart_channel_name  channel);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_ignore_response(UART_channel_name  channel)
*
* @param channel <B>IN</B> UART channel name
*
* @brief Set low level ignore flag lin_lld_ignore_flag
*
* @SDD_ID LIN_SDD_103
* @endif
*
* @return #void
*
* @see lin_goto_idle_state
*
* @details
*   Set low level ignore flag lin_lld_ignore_flag
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_ignore_response(uart_channel_name  channel); 

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_set_low_power_mode(UART_channel_name  channel)
*
* @param channel <B>IN</B> UART channel name
*
* @brief SRequest to enter low power mode in UART Driver
*
* @SDD_ID LIN_SDD_104
* @endif
*
* @return #void
*
* @local_var
*   -# <B>#lin_node</B> <I>*lnode_p</I>
*
* @static_global_var
*   -# <B>#lin_node_descrs</B>
*
* @details
*   Request to enter low power mode in UART Driver
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_set_low_power_mode(uart_channel_name  channel);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_rx_response(UART_channel_name  channel, l_u8  msg_length)
*
* @param channel <B>IN</B> UART channel name
* @param msg_length <B>IN</B> length of response data expect to wait
*
* @brief This function requests to receive response data of LIN frame via UART channel
*
* @SDD_ID LIN_SDD_105
* @endif
*
* @return #void
*
* @local_var
*   -# <B>#lin_node</B> <I>*lnode_p</I>
*
* @static_global_var
*   -# <B>#lin_node_descrs</B>
*
* @details
*   This function requests to receive response data of LIN frame via UART channel
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_rx_response(uart_channel_name  channel,l_u8  msg_length); 

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_tx_response(UART_channel_name  channel)
*
* @param channel <B>IN</B> UART channel name
*
* @brief This function requests to send first data and set lin_lld_state to SEND_DATA
*
* @SDD_ID LIN_SDD_109
* @endif
*
* @return #void
*
* @local_var
*   -# <B>#lin_node</B> <I>*lnode_p</I>
*
* @static_global_var
*   -# <B>#lin_node_descrs</B>
*
* @details
*   This function requests to send first data and set lin_lld_state to SEND_DATA
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_tx_response(uart_channel_name  channel);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_lld_UART_get_status(UART_channel_name  channel)
*
* @param channel <B>IN</B> UART channel name
*
* @brief Get LIN status ( return lin_status )  
*
* @SDD_ID LIN_SDD_110
* @endif
*
* @return #l_u8
*
* @local_var
*   -# <B>#lin_node</B> <I>*lnode_p</I>
*
* @static_global_var
*   -# <B>#lin_node_descrs</B>
*
* @details
*   Get LIN status ( return lin_status )  
*//*END*----------------------------------------------------------------------*/
l_u8 lin_lld_uart_get_status(uart_channel_name  channel);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_lld_UART_get_state(UART_channel_name  channel)
*
* @param channel <B>IN</B> UART channel name
*
* @brief Get LIN state ( return state ) 
*
* @SDD_ID LIN_SDD_
* @endif
*
* @return #l_u8
*
* @local_var
*   -# <B>#lin_node</B> <I>*lnode_p</I>
*
* @static_global_var
*   -# <B>#lin_node_descrs</B>
*
* @details
*   Get LIN state ( return state )  
*//*END*----------------------------------------------------------------------*/
l_u8 lin_lld_uart_get_state(uart_channel_name channel);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_timeout(UART_channel_name  channel)
*
* @param channel <B>IN</B> UART channel name
*
* @brief This function is called in period Tbit in order to check timeout for IDLE and MAXFRAME  
*
* @SDD_ID LIN_SDD_111
* @endif
*
* @return #void
*
* @local_var
*   -# <B>#register lin_node</B> <I>*lnode_p</I>
*
* @static_global_var
*   -# <B>#lin_node_descrs</B>
*
* @details
*   This function is called in period Tbit in order to check timeout for IDLE and MAXFRAME  
*//*END*----------------------------------------------------------------------*/
void lin_lld_uart_timeout(uart_channel_name channel);


/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_err_isr ( UART_channel_name channel)
* @brief UART Interrupt service routine that handles UART Error
*  
* @param channel <B>[IN]</B> UART channel name 
*
* @return #void
* 
* @SDD_ID LIN_SDD_115
* @endif
*
* @local_var
*   -# <B>#lin_node</B> <I>*lnode_p</I> 
*   -# <B>#l_u8</B> <I>UART_flag_sr1</I> 
*   -# <B>#l_u8</B> <I>tmp_byte</I> 
*
* @static_global_var
*   -# <B>#lin_node_descrs</B> 
*
* @details
*  UART Interrupt service routine that handles UART Error
* 
* @see #lin_goto_idle_state
*//*END*----------------------------------------------------------------------*/ 
void lin_lld_uart_err_isr(uart_channel_name channel);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_UART_rx_isr ( UART_channel_name channel)
* @brief UART Interrupt service routine that handles RX LIN Communication  
*  
* @param channel <B>[IN]</B> UART channel name 
*
* @return #void
* 
* @SDD_ID LIN_SDD_116
* @endif
*
* @local_var
*   -# <B>#register lin_node</B> <I>*lnode_p</I> 
*   -# <B>#l_u8</B> <I>UART_flag_sr1</I> 
*   -# <B>#l_u8</B> <I>UART_flag_sr2</I> 
*   -# <B>#l_u8</B> <I>tmp_byte</I> 
*
* @static_global_var
*   -# <B>#lin_node_descrs</B> 
*   -# <B>#lin_max_frame_res_timeout_val</B> 
*
* @details
*   UART Interrupt service routine that handles RX LIN Communication  
* 
* @see #lin_goto_idle_state
* @see #lin_process_parity
*//*END*----------------------------------------------------------------------*/ 
void lin_lld_uart_rx_isr(uart_channel_name channel);


/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_goto_idle_state(UART_channel_name  channel)
*
* @param channel <B>IN</B> UART channel name
*
* @brief Enter IDLE state  
*
* @SDD_ID LIN_SDD_112
* @endif
*
* @return #void
*
* @local_var
*   -# <B>#register lin_node</B> <I>*lnode_p</I>
*
* @static_global_var
*   -# <B>#lin_node_descrs</B>
*
* @details
*   Enter IDLE state  
*//*END*----------------------------------------------------------------------*/
void lin_goto_idle_state(uart_channel_name channel);
#endif /* End (LIN_MODE == _MASTER_MODE_) */

#endif /* ifndef _LIN_LLD_UART */

/**
* @}
*/
