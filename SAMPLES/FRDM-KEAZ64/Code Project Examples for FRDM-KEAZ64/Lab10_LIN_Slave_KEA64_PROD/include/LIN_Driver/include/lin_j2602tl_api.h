/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2008-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************/
/**************************************************************************//**
* @addtogroup j2602tl_api_group
* @{
******************************************************************************/
/**************************************************************************//**
*
* @file      lin_j2602tl_api.h
*
* @author    FPT Software
*
* @brief     J2602 LIN transport layer and configuration functions
*
******************************************************************************/
/******************************************************************************
 *
 * History:
 *
 * 20090407		v1.0	First version
 *
 *****************************************************************************/
 
/** @} */
#ifndef _LIN_J2602TL_API_H
#define _LIN_J2602TL_API_H

#include "LIN_Driver/lowlevel/lin.h"

#if LIN_MODE == _MASTER_MODE_
#if LIN_PROTOCOL == PROTOCOL_J2602
/** @addtogroup j2602tl_api_group
* @{ */
/* API functions */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_bool ld_is_ready(l_ifc_handle iii)
* @brief Verifies a state of node setting (using for J2602)
*
* @param iii <B>[IN]</B> lin interface handle
*
* @return #l_bool
*
* @SDD_ID LIN_SDD_338
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>*conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   Verifies a state of node setting (using for J2602)
*//*END*----------------------------------------------------------------------*/
l_bool ld_is_ready (l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 ld_check_response(l_ifc_handle iii, l_u8* const RSID, l_u8* const error_code)
* @brief Verifies the state of response (using for J2602)
*
* @param iii <B>[IN]</B> lin interface handle
* @param RSID <B>[OUT]</B> buffer for saving the response ID
* @param error_code <B>[OUT]</B> buffer for saving the error code
*
* @return #l_u8
*
* @SDD_ID LIN_SDD_339
* @endif
*
* @local_var

*   -# <B>#lin_last_cfg_result</B> <I>temp</I>
*   -# <B>#lin_configuration</B> <I>*conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   Verifies the state of response (using for J2602)
*//*END*----------------------------------------------------------------------*/
l_u8 ld_check_response (l_ifc_handle iii, l_u8* const RSID, l_u8* const error_code);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ld_assign_frame_id(l_ifc_handle iii, l_u8 initial_NAD, l_u16 supplier_id, l_u16 message_id, l_u8 PID)
* @brief
*   This call assigns the protected identifier of a frame in the slave node
*   with the address NAD and the specified supplier ID (using for J2602)
*
* @param iii <B>[IN]</B> lin interface handle
* @param initial_NAD <B>[IN]</B> initial node address  of the target node
* @param supplier_id <B>[IN]</B> supplier ID of the target node
* @param message_id <B>[IN]</B> message ID of the target node
* @param PID <B>[IN]</B> protected ID of the target node
*
* @return #void
*
* @SDD_ID LIN_SDD_340
* @endif
*
* @local_var

*   -# <B>#lin_tl_pdu_data</B> <I>*lin_tl_pdu</I>
*   -# <B>#lin_configuration</B> <I>*conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>*tl_conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   This call assigns the protected identifier of a frame in the slave node
*   with the address NAD and the specified supplier ID (using for J2602)
*//*END*----------------------------------------------------------------------*/
void ld_assign_frame_id (l_ifc_handle iii, l_u8 initial_NAD, l_u16 supplier_id, l_u16 message_id, l_u8 PID);
/** @} */
#endif /* End (LIN_PROTOCOL == PROTOCOL_J2602) */
#endif /* End (LIN_MODE == _MASTER_MODE_) */
#endif  /* _LIN_J2602TL_API_H */
