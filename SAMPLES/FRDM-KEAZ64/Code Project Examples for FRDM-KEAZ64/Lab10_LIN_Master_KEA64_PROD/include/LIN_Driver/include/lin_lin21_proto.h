/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2008-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************/
/**************************************************************************//**
* @addtogroup LIN21_core_api_group
* @{
******************************************************************************/
/**************************************************************************//**
*
* @file      lin_lin21_proto.h 
*
* @author    FPT Software
*
* @brief     Common LIN 2.1 protocol functions
*
******************************************************************************/
/******************************************************************************
 *
 * History:
 *
 * 20090408		v1.0	First version
 *
 *****************************************************************************/
 
#ifndef _LIN_LIN21_PROTO_H
#define _LIN_LIN21_PROTO_H

#include "LIN_Driver/lowlevel/lin.h"

#if LIN_MODE == _MASTER_MODE_

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_collision_resolve (l_ifc_handle iii, l_u8 pid)
* @brief Switch to collision resolve table
*
* @param iii <B>[IN]</B> interface name
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_279
* @endif
*
* @local_var
*   -# <B>#lin_associate_frame_struct</B> <I>*ptr</I>
*   -# <B>#l_u8</B> <I>frame_index</I>
*   -# <B>#const lin_configuration</B> <I>*conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   Switch to collision resolve table
*
* @see  #lin_get_frame_index
*//*END*----------------------------------------------------------------------*/
void lin_collision_resolve (l_ifc_handle iii, l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_update_word_status_lin21 (l_ifc_handle iii, lin_lld_event_id event_id, l_u8 pid)
* @brief Update date word status
*
* @param iii <B>[IN]</B> interface name
* @param event_id <B>[IN]</B> event id
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_280
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
*   Update date word status
*
* @see  #lin_process_uncd_frame
* @see  #lin_update_rx_diag_frame
* @see  #lin_update_word_status_lin21
* @see  #lin_update_word_status_j2602
*//*END*----------------------------------------------------------------------*/
void lin_update_word_status_lin21 (l_ifc_handle iii, lin_lld_event_id event_id, l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_update_err_signal (l_ifc_handle iii)
* @brief Update error signal
*
* @param iii <B>[IN]</B> interface name
*
* @return #void
*
* @SDD_ID LIN_SDD_281
* @endif
*
* @local_var
*   -# <B>#const lin_configuration</B> <I>*conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   Update error signal
*
*//*END*----------------------------------------------------------------------*/
void lin_update_err_signal (l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_update_rx_evnt_frame (l_ifc_handle iii, l_u8 pid)
* @brief The function is to update the receive flags associated with signals/frames
*   in case receive an event trigger frame.
*
* @param iii <B>[IN]</B> interface name
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_282
* @endif
*
* @local_var
*   -# <B>#lin_associate_frame_struct</B> <I>*ptr</I>
*   -# <B>#const lin_configuration</B> <I>*conf</I>
*   -# <B>#l_u8</B> <I>frame_index</I>
*   -# <B>#l_u8</B> <I>ass_pid</I>
*   -# <B>#l_u8</B> <I>i</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   The function is to update the receive flags associated with signals/frames
*   in case receive an event trigger frame.
*
* @see  #lin_get_frame_index
* @see  #lin_process_parity
* @see  #lin_process_uncd_frame
*//*END*----------------------------------------------------------------------*/
void lin_update_rx_evnt_frame (l_ifc_handle iii, l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_make_res_evnt_frame (l_ifc_handle iii,  l_u8 pid)
* @brief This function is to pack signals associated with event trigger frame into buffer
*
* @param iii <B>[IN]</B> interface name
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_278
* @endif
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   This function is to pack signals associated with event trigger frame into buffer
*
* @see  #lin_process_uncd_frame
* @see  #lin_process_parity
*//*END*----------------------------------------------------------------------*/
void lin_make_res_evnt_frame (l_ifc_handle iii, l_u8 pid);
#endif /* End (LIN_MODE == _MASTER_MODE_) */

/* ---------------------------- For 1 interface -----------------------------------*/
#if LIN_MODE == _SLAVE_MODE_

#if LIN_PROTOCOL == PROTOCOL_21

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_update_word_status_lin21 (l_u8 pid)
* @brief Update date word status
*
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_298
* @endif
*
* @static_global_var
*   -# <B>#lin_word_status</B>
*   -# <B>#lin_error_in_response</B>
*   -# <B>#lin_successful_transfer</B>
*   -# <B>#lin_goto_sleep_flg</B>
*
* @details
*   Update date word status
*
*//*END*----------------------------------------------------------------------*/
void lin_update_word_status_lin21 (l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_update_err_signal ( void )
* @brief Update error signal
*
* @return #void
*
* @SDD_ID LIN_SDD_299
* @endif
*
* @local_var
*   -# <B>#l_bool</B> <I>pid_found</I>
*
* @static_global_var
*   -# <B>#response_error</B>
*   -# <B>#lin_error_in_response</B>
*
* @details
*   Update error signal
*
*//*END*----------------------------------------------------------------------*/
void lin_update_err_signal (void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_make_res_evnt_frame (l_u8 pid)
* @brief This function is to pack signals associated with event trigger frame into buffer
*
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_300
* @endif
*
* @details
*   This function is to pack signals associated with event trigger frame into buffer
*
* @see  #lin_process_uncd_frame
* @see  #lin_lld_response_buffer
*//*END*----------------------------------------------------------------------*/
void lin_make_res_evnt_frame (l_u8 pid);
#endif /* End (LIN_PROTOCOL == PROTOCOL_21) */

#endif /* End ( LIN_MODE == _SLAVE_MODE_) */
 
#endif  /* _LIN_LIN21_PROTO_H */
/** @} */
