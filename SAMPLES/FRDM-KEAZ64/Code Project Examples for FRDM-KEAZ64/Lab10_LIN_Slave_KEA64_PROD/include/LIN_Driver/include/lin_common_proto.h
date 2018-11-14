/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2008-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************/
/**************************************************************************//**
* @addtogroup common_core_api_group
* @{
******************************************************************************/
/**************************************************************************//**
*
* @file      lin_common_proto.h
*
* @author    FPT Software
*
* @brief     Common LIN protocol functions
*
******************************************************************************/ 
/******************************************************************************
 *
 * History:
 *
 * 20090407		v1.0	First version
 *
 *****************************************************************************/
 
#ifndef _LIN_COMMON_PROTO_H
#define _LIN_COMMON_PROTO_H

#include "LIN_Driver/lowlevel/lin.h"

#define MAKE_UNCONDITIONAL_FRAME 0      /**< make unconditional frame */
#define UPDATE_UNCONDITIONAL_FRAME 1    /**< update unconditional frame */

#if LIN_MODE == _MASTER_MODE_

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_pid_response_callback_handler (l_ifc_handle iii, lin_lld_event_id event_id, l_u8 pid)
* @brief The upper slave task is in charge of  handling PID or Response complete events
*
* @param iii <B>[IN]</B> interface name
* @param event_id <B>[IN]</B> event id
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_264
* @endif
*
* @details
*   The upper slave task is in charge of  handling PID or Response complete events
*
* @see  #lin_process_pid
* @see  #lin_update_tx
* @see  #lin_update_rx
* @see  #lin_bus_activity_timeout
* @see  #lin_handle_error
*//*END*----------------------------------------------------------------------*/
void lin_pid_response_callback_handler (l_ifc_handle iii, lin_lld_event_id event_id, l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_tick_callback_handler (l_ifc_handle iii)
* @brief Tick handle function used for master task. This function is called periodically,
*
* @param iii <B>[IN]</B> interface name
*
* @return #l_u8
*
* @SDD_ID LIN_SDD_265
* @endif
*
* @local_var
*   -# <B>#lin_schedule_struct</B> <I>*sch</I>
*   -# <B>#l_u8</B> <I>*current_entry</I>
*   -# <B>#l_u8</B> <I>frame_index</I>
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#lin_configuration</B> <I>*conf</I>
*   -# <B>#lin_tl_queue</B> <I>*current_pdu</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*   -# <B>#schedule_tbl</B>
*
* @details
*   Tick handle function used for master task. This function is called periodically,
*
* @see  #lin_update_word_status_lin21
* @see  #lin_update_word_status_j2602
* @see  #lin_collision_resolve
* @see  #lin_switch_sch_table
* @see  #lin_check_sporadic_update
* @see  #lin_lld_tx_header
*//*END*----------------------------------------------------------------------*/
l_u8 lin_tick_callback_handler (l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_process_pid (l_ifc_handle iii, l_u8 pid)
* @brief Handle the received PID and decide the next action of driver
*
* @param iii <B>[IN]</B> interface name
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_266
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>action</I>
*   -# <B>#l_u8</B> <I>associate_frame</I>
*   -# <B>#lin_frame_struct</B> <I>*lin_frame_ptr</I>
*   -# <B>#l_u8</B> <I>frame_index</I>
*   -# <B>#lin_configuration</B> <I>*conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>*tl_conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*   -# <B>#lin_associate_frame_struct</B>
*
* @details
*   Handle the received PID and decide the next action of driver
*
* @see  #lin_get_frame_index
* @see  #lin_process_uncd_frame
* @see  #lin_make_res_evnt_frame
* @see  #lin_tl_make_mreq_pdu
* @see  #lin_make_res_diag_frame
* @see  #lin_lld_rx_response
* @see  #lin_lld_set_response
* @see  #lin_lld_ignore_response
*//*END*----------------------------------------------------------------------*/
void lin_process_pid (l_ifc_handle iii, l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_update_rx (l_ifc_handle iii, l_u8 pid)
* @brief Update signal, status and flags after received response
*
* @param iii <B>[IN]</B> interface name
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_267
* @endif
*
* @local_var
*   -# <B>#l_bool</B> <I>pid_found</I>
*   -# <B>#l_u8</B> <I>frame_index</I>
*   -# <B>#const lin_configuration</B> <I>*conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*   -# <B>#lin_flag_frame_handle_tbl</B>
*
* @details
*   Update signal, status and flags after received response
*
* @see  #lin_get_frame_index
* @see  #lin_process_uncd_frame
* @see  #lin_update_rx_evnt_frame
* @see  #lin_update_rx_diag_frame
* @see  #lin_update_word_status_lin21
* @see  #lin_update_word_status_j2602
*//*END*----------------------------------------------------------------------*/
void lin_update_rx (l_ifc_handle iii, l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_update_tx (l_ifc_handle iii, l_u8 pid)
* @brief Update status and flags after transmit response
*
* @param iii <B>[IN]</B> interface name
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_268
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>frame_index</I>
*   -# <B>#const lin_configuration</B> <I>*conf</I>
*   -# <B>#const lin_frame_struct</B> <I>*lin_frame_ptr</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*   -# <B>#frame_signal_error</B>
*   -# <B>#lin_flag_frame_handle_tbl</B>
*   -# <B>#lin_frame_flag_tbl</B>
*   -# <B>#lin_associate_frame_struct</B>
*
* @details
*   Update status and flags after transmit response
*
* @see  #lin_get_frame_index
* @see  #lin_update_tx_flags
* @see  #tl_process_mreq
* @see  #lin_update_err_signal
* @see  #lin_update_word_status_lin21
* @see  #lin_update_word_status_j2602
*//*END*----------------------------------------------------------------------*/
void lin_update_tx (l_ifc_handle iii, l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_handle_error (l_ifc_handle iii, lin_lld_event_id event_id, l_u8 pid)
* @brief Handle error raised by low-level
*
* @param iii <B>[IN]</B> interface name
* @param event_id <B>[IN]</B> event id
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_269
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>frame_index</I>
*   -# <B>#const lin_configuration</B> <I>*conf</I>
*   -# <B>#const lin_frame_struct</B> <I>*lin_frame_ptr</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*   -# <B>#frame_signal_error</B>
*   -# <B>#lin_flag_frame_handle_tbl</B>
*   -# <B>#lin_frame_flag_tbl</B>
*   -# <B>#lin_associate_frame_struct</B>
*
* @details
*   Handle error raised by low-level
*
* @see  #lin_get_frame_index
* @see  #lin_update_tx_flags
* @see  #tl_process_mreq
* @see  #lin_update_err_signal
* @see  #lin_update_word_status_lin21
* @see  #lin_update_word_status_j2602
*//*END*----------------------------------------------------------------------*/
void lin_handle_error (l_ifc_handle iii, lin_lld_event_id event_id, l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_bus_activity_timeout (l_ifc_handle iii, l_u8 pid)
* @brief Set bus activity timeout
*
* @param iii <B>[IN]</B> interface name
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_270
* @endif
*
* @local_var
*   -# <B>#const lin_configuration</B> <I>*conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   Set bus activity timeout
*
* @see  #lin_update_word_status_lin21
* @see  #lin_update_word_status_j2602
* @see  #lin_lld_set_low_power_mode
*//*END*----------------------------------------------------------------------*/
void lin_bus_activity_timeout (l_ifc_handle iii, l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_switch_sch_table (l_ifc_handle iii)
* @brief Check to see if the current schedule table is needed to change
*   or not
*
* @param iii <B>[IN]</B> interface name
*
* @return #void
*
* @SDD_ID LIN_SDD_271
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>tmp_sch_id</I>
*   -# <B>#const lin_configuration</B> <I>*conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   Check to see if the current schedule table is needed to change
*   or not. Used in tick callback function.
*
*//*END*----------------------------------------------------------------------*/
void lin_switch_sch_table (l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_check_sporadic_update (l_ifc_handle iii, l_frame_handle frm_id)
* @brief Find the frame associated with a specified sporadic frame which is updated
*
* @param iii <B>[IN]</B> interface name
* @param frm_id <B>[IN]</B> index of frame
*
* @return #l_u8
*
* @SDD_ID LIN_SDD_272
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>j</I>
*   -# <B>#lin_associate_frame_struct</B> <I>*ptr</I>
*   -# <B>#const lin_configuration</B> <I>*conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   Find the frame associated with a specified sporadic frame which is updated
*
*//*END*----------------------------------------------------------------------*/
l_u8 lin_check_sporadic_update (l_ifc_handle iii, l_frame_handle frm_id);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_update_tx_flags (l_ifc_handle iii, l_u8 frm_id)
* @brief Update transmit flags
*
* @param iii <B>[IN]</B> interface name
* @param frm_id <B>[IN]</B> index of frame
*
* @return #void
*
* @SDD_ID LIN_SDD_273
* @endif
*
* @local_var
*   -# <B>#const lin_configuration</B> <I>*conf</I>
*   -# <B>#const lin_frame_struct</B> <I>*frame_struct_ptr</I>
*   -# <B>#lin_frm2sig</B> <I>*ptr</I>
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#l_u8</B> <I>signal_id</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*   -# <B>#lin_flag_signal_handle_tbl</B>
*
* @details
*   Update transmit flags
*
*//*END*----------------------------------------------------------------------*/
void lin_update_tx_flags (l_ifc_handle iii, l_u8 frm_id);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_update_rx_diag_frame (l_ifc_handle iii, l_u8 pid)
* @brief The function is to update the receive flags associated with signals/frames
*   in case receive a diagnostic frame.
*
* @param iii <B>[IN]</B> interface name
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_274
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#l_u8</B> <I>j</I>
*   -# <B>#const lin_configuration</B> <I>*conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   The function is to update the receive flags associated with signals/frames
*   in case receive a diagnostic frame.
*
* @see  #lin_lld_set_low_power_mode (iii)
* @see  #lin_tl_handler (iii)
*//*END*----------------------------------------------------------------------*/
void lin_update_rx_diag_frame (l_ifc_handle iii, l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_process_uncd_frame (l_ifc_handle iii, l_u8 pid, l_u8 type)
* @brief This function to pack signals associated with unconditional frame into buffer
*
* @param iii <B>[IN]</B> interface name
* @param pid <B>[IN]</B> PID to process
* @param type <B>[IN]</B> make or update
*
* @return #void
*
* @SDD_ID LIN_SDD_278
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#l_u8</B> <I>tmp_offset</I>
*   -# <B>#l_u8</B> <I>index</I>
*   -# <B>#l_u8</B> <I>frame_index</I>
*   -# <B>#l_u8</B> <I>signal_id</I>
*   -# <B>#l_u8</B> <I>*response_buffer_ptr</I>
*   -# <B>#l_u8</B> <I>tmp_sig_size</I>
*   -# <B>#l_u8</B> <I>tmp_byte_data</I>
*   -# <B>#l_s8</B> <I>length</I>
*   -# <B>#l_s8</B> <I>to_fill</I>
*   -# <B>#l_s8</B> <I>byte_offset</I>
*   -# <B>#const lin_frame_struct</B> <I>*frame_struct_ptr</I>
*   -# <B>#const lin_signal_struct</B> <I>*lin_signal_str_ptr</I>
*   -# <B>#l_u16</B> <I>tmp16</I>
*   -# <B>#lin_frm2sig</B> <I>*ptr</I>
*   -# <B>#const lin_configuration</B> <I>*conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*   -# <B>#frame_signal_error</B>
*   -# <B>#lin_flag_signal_handle_tbl</B>
*
* @details
*   This function to pack signals associated with unconditional frame into buffer
*
* @see  #lin_update_signal_scalar
* @see  #l_bytes_rd
* @see  #l_bytes_wr
*//*END*----------------------------------------------------------------------*/
void lin_process_uncd_frame(l_ifc_handle iii, l_u8 pid, l_u8 type);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_make_res_diag_frame (l_ifc_handle iii, l_u8 pid)
* @brief This function is to copy diagnostic data into buffer
*
* @param iii <B>[IN]</B> interface name
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_275
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#const lin_configuration</B> <I>*conf</I>
*   -# <B>#lin_tl_pdu_data</B> <I>lin_tl_pdu</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   This function is to copy diagnostic data into buffer
*
*//*END*----------------------------------------------------------------------*/
void lin_make_res_diag_frame (l_ifc_handle iii, l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_get_frame_index (l_ifc_handle iii, l_u8 pid)
* @brief This function is return the index of a frame in frame list or 0xFF if not found.
*
* @param iii <B>[IN]</B> interface name
* @param pid <B>[IN]</B> PID to process
*
* @return #l_u8
*
* @SDD_ID LIN_SDD_276
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#const lin_configuration</B> <I>*conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   This function is return the index of a frame in frame list or 0xFF if not found.
*
*//*END*----------------------------------------------------------------------*/
l_u8 lin_get_frame_index (l_ifc_handle iii, l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_update_signal_scalar (const lin_configuration *conf_ptr, l_u16 value, l_signal_handle signal_name)
* @brief Check and update scalar signal
*
* @param conf_ptr <B>[IN]</B> current configuration
* @param value <B>[IN]</B> value to compare
* @param signal_name <B>[IN]</B> signal name
*
* @return #void
*
* @SDD_ID LIN_SDD_277
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>signal_size</I>
*   -# <B>#l_u16</B> <I>sig_value</I>
*
* @details
*   Check and update scalar signal
*
* @see  #l_u16_wr
* @see  #l_u8_wr
* @see  #l_bool_wr
*//*END*----------------------------------------------------------------------*/
void lin_update_signal_scalar(const lin_configuration *conf_ptr, l_u16 value, l_signal_handle signal_name);
#endif /* end of LIN_MODE == _MASTER_MODE_ */

/* ---------------------------- For 1 interface -----------------------------------*/
#if LIN_MODE == _SLAVE_MODE_

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_pid_response_callback_handler (lin_lld_event_id event_id, l_u8 pid)
* @brief The upper slave task is in charge of  handling PID or Response complete events
*
* @param event_id <B>[IN]</B> event id
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_287
* @endif
*
* @details
*   The upper slave task is in charge of  handling PID or Response complete events
*
* @see  #lin_process_pid
* @see  #lin_update_tx
* @see  #lin_update_rx
* @see  #lin_handle_error (event_id, pid)
*//*END*----------------------------------------------------------------------*/
void lin_pid_response_callback_handler (lin_lld_event_id event_id, l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_process_pid (l_u8 pid)
* @brief Handle the received PID and decide the next action of driver
*
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_288
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>action</I>
*   -# <B>#lin_frame_struct</B> <I>*lin_frame_ptr</I>
*
* @static_global_var
*   -# <B>#frame_index</B>
*   -# <B>#lin_frame_tbl</B>
*   -# <B>#lin_frame_flag_tbl</B>
*   -# <B>#lin_configuration_RAM</B>
*   -# <B>#tl_slaveresp_cnt</B>
*
* @details
*   Handle the received PID and decide the next action of driver
*
* @see  #lin_get_frame_index
* @see  #lin_make_res_evnt_frame
* @see  #lin_process_uncd_frame
* @see  #lin_make_res_diag_frame
* @see  #lin_lld_rx_response
* @see  #lin_lld_set_response
* @see  #lin_lld_ignore_response
*//*END*----------------------------------------------------------------------*/
void lin_process_pid (l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_update_rx (l_u8 pid)
* @brief Update signal, status and flags after received response
*
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_289
* @endif
*
* @local_var
*   -# <B>#l_bool</B> <I>pid_found</I>
*
* @static_global_var
*   -# <B>#lin_successful_transfer</B>
*   -# <B>#lin_frame_tbl</B>
*   -# <B>#lin_flag_frame_handle_tbl</B>
*
* @details
*   Update signal, status and flags after received response
*
* @see  #lin_process_uncd_frame
* @see  #lin_update_rx_diag_frame
* @see  #lin_update_word_status_lin21
* @see  #lin_update_word_status_j2602
*//*END*----------------------------------------------------------------------*/
void lin_update_rx (l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_update_tx (l_u8 pid)
* @brief Update status and flags after transmit response
*
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_290
* @endif
*
* @static_global_var
*   -# <B>#lin_successful_transfer</B>
*   -# <B>#lin_frame_flag_tbl</B>
*   -# <B>#frame_index</B>
*
* @details
*   Update status and flags after transmit response
*
* @see  #lin_process_uncd_frame
* @see  #lin_update_rx_diag_frame
* @see  #lin_update_word_status_lin21
* @see  #lin_update_word_status_j2602
*//*END*----------------------------------------------------------------------*/
void lin_update_tx (l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_handle_error (lin_lld_event_id event_id, l_u8 pid)
* @brief Handle error raised by low-level
*
* @param event_id <B>[IN]</B> event id
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_291
* @endif
*
* @static_global_var
*   -# <B>#frame_index</B>
*   -# <B>#lin_frame_tbl</B>
*   -# <B>#lin_error_in_response</B>
*   -# <B>#tl_slaveresp_cnt</B>
*
* @details
*   Handle error raised by low-level
*
* @see  #lin_get_frame_index
* @see  #lin_update_err_signal
* @see  #lin_update_word_status_lin21
* @see  #lin_update_status_byte
* @see  #lin_update_word_status_j2602
*//*END*----------------------------------------------------------------------*/
void lin_handle_error (lin_lld_event_id event_id, l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_bus_activity_timeout (l_u8 pid)
* @brief Set bus activity timeout
*
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_292
* @endif
*
* @details
*   Set bus activity timeout
*
* @see  #lin_update_word_status_lin21
* @see  #lin_update_word_status_j2602
* @see  #lin_lld_set_low_power_mode
*//*END*----------------------------------------------------------------------*/
void lin_bus_activity_timeout (l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_update_tx_flags (l_u8 frm_id)
* @brief Update transmit flags
*
* @param frm_id <B>[IN]</B> index of frame
*
* @return #void
*
* @SDD_ID LIN_SDD_293
* @endif
*
* @local_var
*   -# <B>#lin_frame_struct</B> <I>frame_struct_ptr</I>
*   -# <B>#lin_frm2sig</B> <I>*ptr</I>
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#l_u8</B> <I>signal_id</I>
*
* @static_global_var
*   -# <B>#lin_frame_tbl</B>
*   -# <B>#lin_flag_signal_handle_tbl</B>
*
* @details
*   Update transmit flags
*
*//*END*----------------------------------------------------------------------*/
void lin_update_tx_flags (l_u8 frm_id);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_update_rx_diag_frame ( void )
* @brief update the receive flags associated with signals/frames
*   in case receive a diagnostic frame.
*
* @return #void
*
* @SDD_ID LIN_SDD_294
* @endif
*
* @static_global_var
*   -# <B>#lin_lld_response_buffer</B>
*   -# <B>#lin_goto_sleep_flg</B>
*
* @details
*   The function is to update the receive flags associated with signals/frames
*   in case receive a diagnostic frame.
*
* @see  #lin_lld_set_low_power_mode
* @see  #lin_tl_put_pdu
* @see  #lin_tl_handler
*//*END*----------------------------------------------------------------------*/
void lin_update_rx_diag_frame (void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_process_uncd_frame (l_u8 pid, l_u8 type)
* @brief Update signal, status and flags after received response
*
* @param pid <B>[IN]</B> PID to process
* @param type <B>[IN]</B> make or update
*
* @return #void
*
* @SDD_ID LIN_SDD_297
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#l_u8</B> <I>tmp_offset</I>
*   -# <B>#l_u8</B> <I>index</I>
*   -# <B>#l_u8</B> <I>signal_id</I>
*   -# <B>#l_s8</B> <I>length</I>
*   -# <B>#l_s8</B> <I>to_fill</I>
*   -# <B>#l_s8</B> <I>byte_offset</I>
*   -# <B>#l_s8</B> <I>tmp_byte_data</I>
*   -# <B>#l_u16</B> <I>tmp16</I>
*   -# <B>#l_u16</B> <I>tmp_sig_size</I>
*   -# <B>#lin_frm2sig</B> <I>*ptr</I>
*
* @static_global_var
*   -# <B>#frame_index</B>
*   -# <B>#lin_frame_tbl</B>
*   -# <B>#lin_lld_response_buffer</B>
*   -# <B>#response_error</B>
*   -# <B>#frame_signal_error</B>
*   -# <B>#lin_flag_signal_handle_tbl</B>
*   -# <B>#lin_signal_flag_tbl</B>
*
* @details
*   Update signal, status and flags after received response
*
* @see  #lin_get_frame_index
* @see  #l_u16_wr
* @see  #l_u8_wr
* @see  #l_bool_wr
* @see  #l_bytes_rd
* @see  #l_bytes_wr
*//*END*----------------------------------------------------------------------*/
void lin_process_uncd_frame(l_u8 pid, l_u8 type);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_make_res_diag_frame ( void )
* @brief This function is to copy diagnostic data into buffer
*
* @return #void
*
* @SDD_ID LIN_SDD_295
* @endif
*
* @details
*   This function is to copy diagnostic data into buffer
*
* @see  #lin_tl_get_pdu
*//*END*----------------------------------------------------------------------*/
void lin_make_res_diag_frame (void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_get_frame_index (l_u8 pid)
* @brief This function is return the index of a frame in frame list or 0xFF if not found.
*
* @param pid <B>[IN]</B> PID to process
*
* @return #l_u8
*
* @SDD_ID LIN_SDD_296
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>i</I>
*
* @static_global_var
*   -# <B>#lin_configuration_RAM</B>
*
* @details
*   This function is return the index of a frame in frame list or 0xFF if not found.
*
*//*END*----------------------------------------------------------------------*/
l_u8 lin_get_frame_index (l_u8 pid);
#endif /* end of LIN_MODE == _SLAVE_MODE_*/
#endif  /* _LIN_COMMON_PROTO_H */
/** @} */
