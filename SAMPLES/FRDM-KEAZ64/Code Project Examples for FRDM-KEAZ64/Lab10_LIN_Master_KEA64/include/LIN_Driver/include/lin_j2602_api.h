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
* @file      lin_j2602_api.h 
*
* @author    FPT Software
*
* @brief     J2602 LIN core API functions
*
******************************************************************************/
/******************************************************************************
 *
 * History:
 *
 * 20090407		v1.0	First version
 *
 *****************************************************************************/
 
#ifndef _LIN_J2602_API_H
#define _LIN_J2602_API_H

#include "LIN_Driver/lowlevel/lin.h"

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_bool l_ifc_connect (l_ifc_handle iii)
* @brief Connect the interface iii to the LIN cluster and enable the transmission
*
* @param iii <B>[IN]</B> interface name
*
* @return #l_bool
*
* @SDD_ID LIN_SDD_260
* @endif
*
* @details
*   Connect the interface iii to the LIN cluster and enable the transmission
*   of headers and data to the bus. (For J2602 and LIN 2.0 only)
*
*//*END*----------------------------------------------------------------------*/
l_bool l_ifc_connect (l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_bool l_ifc_disconnect (l_ifc_handle iii)
* @brief Disconnect the interface iii from the LIN cluster and thus disable the
*   interaction with the other nodes in the cluster.
*
* @param iii <B>[IN]</B> interface name
*
* @return #l_bool
*
* @SDD_ID LIN_SDD_261
* @endif
*
* @details
*   Disconnect the interface iii from the LIN cluster and thus disable the
*   interaction with the other nodes in the cluster. (For J2602 and LIN 2.0 only)
*
*//*END*----------------------------------------------------------------------*/
l_bool l_ifc_disconnect (l_ifc_handle iii);
 
#endif  /* _LIN_J2602_API_H */
/** @} */
