/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2008-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************/
/**************************************************************************//**
* @addtogroup J2602_core_api_group
* @{
******************************************************************************/
/**************************************************************************//**
*
* @file      lin_j2602_proto.h 
*
* @author    FPT Software
*
* @brief     J2602 LIN protocol functions
*
******************************************************************************/
/******************************************************************************
 *
 * History:
 *
 * 20090407		v1.0	First version
 *
 *****************************************************************************/
 
#ifndef _LIN_J2602_PROTO_H
#define _LIN_J2602_PROTO_H

#include "LIN_Driver/lowlevel/lin.h"

#if LIN_MODE == _MASTER_MODE_

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_update_word_status_j2602 (l_ifc_handle iii, lin_lld_event_id event_id, l_u8 pid)
* @brief Update date word status
*
* @param iii <B>[IN]</B> interface name
* @param event_id <B>[IN]</B> event id
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_285
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>err_in_res</I>
*   -# <B>#l_u8</B> <I>suc_in_tras</I>
*   -# <B>#lin_word_status_str</B> <I>*word_status</I>
*   -# <B>#lin_configuration</B> <I>const *conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   Update date word status
*
*//*END*----------------------------------------------------------------------*/
void lin_update_word_status_j2602 (l_ifc_handle iii, lin_lld_event_id event_id, l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_update_status_byte (l_ifc_handle iii, lin_lld_event_id event_id)
* @brief The function is to perform the update of error signal in J2602 system
*
* @param iii <B>[IN]</B> interface name
* @param event_id <B>[IN]</B> event id
*
* @return #void
*
* @SDD_ID LIN_SDD_284
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>*signal_data_ptr</I>
*   -# <B>#lin_configuration</B> <I>const *conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   The function is to perform the update of error signal in J2602 system
*
*//*END*----------------------------------------------------------------------*/
void lin_update_status_byte (l_ifc_handle iii, lin_lld_event_id event_id);
#endif /* End (LIN_MODE == _MASTER_MODE_) */

/* ---------------------------- For 1 interface -----------------------------------*/
#if LIN_MODE == _SLAVE_MODE_
#if LIN_PROTOCOL != PROTOCOL_21

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_update_word_status_j2602 (lin_lld_event_id event_id, l_u8 pid)
* @brief Update date word status
*
* @param event_id <B>[IN]</B> event id
* @param pid <B>[IN]</B> PID to process
*
* @return #void
*
* @SDD_ID LIN_SDD_301
* @endif
*
* @static_global_var
*   -# <B>#lin_word_status</B>
*   -# <B>#lin_error_in_response</B>
*   -# <B>#lin_successful_transfer</B>
*
* @details
*   Update date word status
*
*//*END*----------------------------------------------------------------------*/
void lin_update_word_status_j2602 (lin_lld_event_id event_id, l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_update_status_byte (lin_lld_event_id event_id)
* @brief The function is to perform the update of error signal in J2602 system
*
* @param event_id <B>[IN]</B> event id
*
* @return #void
*
* @SDD_ID LIN_SDD_302
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>*signal_data_ptr</I>
*
* @static_global_var
*   -# <B>#response_error</B>
* @details
*   The function is to perform the update of error signal in J2602 system
*
*//*END*----------------------------------------------------------------------*/
void lin_update_status_byte (lin_lld_event_id event_id);
#endif /* End (LIN_PROTOCOL != PROTOCOL_21) */

#endif  /* End (LIN_MODE == _SLAVE_MODE_) */

 
#endif  /* _LIN_J2602_PROTO_H */
/** @} */
