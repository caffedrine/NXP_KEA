/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2008-2009 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 ******************************************************************************/
/**************************************************************************//**
 * @addtogroup lin21tl_api_group
 * @{
 ******************************************************************************/
/**************************************************************************//**
 *
 * @file      lin_lin21tl_api.h
 *
 * @author    FPT Software
 *
 * @brief     LIN21 LIN transport layer and configuration functions
 *
 ******************************************************************************/
/******************************************************************************
 *
 * History:
 *
 * 20090408		v1.0	First version
 *
 *****************************************************************************/
 
 /** @} */
#ifndef _LIN_LIN21TL_API_H
#define _LIN_LIN21TL_API_H

#include "LIN_Driver/lowlevel/lin.h"
#define MAX_LENGTH_SERVICE 60			/**< max length service */
#if LIN_MODE == _MASTER_MODE_
#if LIN_PROTOCOL == PROTOCOL_21

/* API functions */
/** @addtogroup node_configuration_group
* @{ */
/*FUNCTION*--------------------------------------------------------------*//**
 * @fn l_u8 ld_is_ready(l_ifc_handle iii)
 * @brief Verifies a state of node setting (using for LIN2.1)
 *
 * @param iii <B>[IN]</B> lin interface handle
 *
 * @return #l_u8
 *
 * @SDD_ID LIN_SDD_329
* @endif
 *
 *
 * @static_global_var
 *   -# <B>#lin_ifc_configuration</B>
 *
 * @details
 *   Verifies a state of node setting (using for LIN2.1)
 *//*END*----------------------------------------------------------------------*/
l_u8 ld_is_ready(l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
 * @fn void ld_check_response(l_ifc_handle iii, l_u8* const RSID, l_u8* const error_code)
 * @brief Verifies the state of response (using for LIN2.1)
 *
 * @param iii <B>[IN]</B> lin interface handle
 * @param RSID <B>[OUT]</B> buffer for saving the response ID
 * @param error_code <B>[OUT]</B> buffer for saving the error code
 *
 * @return #void
 *
 * @local_var
 *   -# <B>#lin_configuration</B> <I>*conf</I>
 *
 * @SDD_ID LIN_SDD_330
* @endif
 *
 *
 * @static_global_var
 *   -# <B>#lin_ifc_configuration</B>
 *
 * @details
 *   Verifies the state of response (using for LIN2.1)
 *//*END*----------------------------------------------------------------------*/
void ld_check_response(
    l_ifc_handle iii, 
    l_u8* const RSID,
		l_u8* const error_code);

/*FUNCTION*--------------------------------------------------------------*//**
 * @fn void ld_assign_frame_id_range(l_ifc_handle iii, l_u8 NAD, l_u8 start_index, const l_u8* const PIDs)
 * @brief
 *  This call assigns the protected identifier of up to four frames in the slave node with the
 *  addressed NAD (using for LIN2.1)
 *
 * @param iii <B>[IN]</B> lin interface handle
 * @param NAD <B>[IN]</B> NAD value of the target node
 * @param start_index <B>[IN]</B> specifies which is the first frame to assign a PID
 * @param PIDs <B>[IN]</B> list of protected identifier
 *
 * @return #void
 *
 * @local_var
 *   -# <B>#lin_configuration</B> <I>*conf</I>
 *   -# <B>#l_u8</B> <I>Buff</I>
 *
 * @SDD_ID LIN_SDD_331
* @endif
 *
 *
 * @static_global_var
 *   -# <B>#lin_ifc_configuration</B>
 *
 * @details
 *  This call assigns the protected identifier of up to four frames in the slave node with the
 *  addressed NAD (using for LIN2.1)
 *//*END*----------------------------------------------------------------------*/
void ld_assign_frame_id_range(l_ifc_handle iii, 
    l_u8 NAD, 
    l_u8 start_index,
		const l_u8* const PIDs);
/** @} */

/* Multi frame support */
/** @addtogroup lin21tl_api_group
* @{ */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
/* get diagnostic flag */
/*FUNCTION*--------------------------------------------------------------*//**
 * @fn l_u8 diag_get_flag(l_ifc_handle iii, l_u8 flag_order)
 * @brief Get flag status of diagnostic service
 *
 * @param iii <B>[IN]</B> lin interface handle
 * @param flag_order <B>[IN]</B> Order of service flag
 *
 * @return #l_u8
 *
 * @local_var 
 *   -# <B>#lin_configuration</B> <I>*conf</I>
 *
 * @SDD_ID LIN_SDD_
* @endif
 *
 *
 * @static_global_var
 *   -# <B>#lin_ifc_configuration</B> 
 *
 * @details
 * This function will return flag of diagnostic service, if lin node receive master request or slave response
 *//*END*----------------------------------------------------------------------*/
l_u8 diag_get_flag(l_ifc_handle iii,l_u8 flag_order);  
/*FUNCTION*--------------------------------------------------------------*//**
 * @fn void diag_clear_flag(l_ifc_handle iii, l_u8 flag_order)
 * @brief Clear flag status of diagnostic service
 *
 * @param iii <B>[IN]</B> lin interface handle
 * @param flag_order <B>[IN]</B> Order of service flag
 *
 * @return #void
 *
 * @local_var 
 *   -# <B>#lin_configuration</B> <I>*conf</I>
 *
 * @SDD_ID LIN_SDD_
* @endif
 *
 *
 * @static_global_var
 *   -# <B>#lin_ifc_configuration</B> 
 *
 * @details
 * This function will clear flag of diagnostic service, if lin node receive master request or slave response
 *//*END*----------------------------------------------------------------------*/
void diag_clear_flag(l_ifc_handle iii,l_u8 flag_order);

/* Diagnostic class II services, they can be used in class III */

/*FUNCTION*--------------------------------------------------------------*//**
 * @fn void diag_read_data_by_identifier(l_ifc_handle iii, const l_u8 NAD, l_u8 *const data)
 * @brief This function is read data by identifier.
 *
 * @param iii <B>[IN]</B> lin interface handle
 * @param NAD <B>[IN]</B> NAD value of the destination node for the transmission
 * @param data <B>[IN]</B> buffer for the data to be transmitted
 *
 * @return #void
 *
 * @local_var
 *   -# <B>#l_u16</B> <I>temp</I>
 *   -# <B>#l_u8</B> <I>Buff</I>
 *   -# <B>#lin_configuration</B> <I>*conf</I>
 *
 * @SDD_ID LIN_SDD_332
* @endif
 *
 *
 * @static_global_var
 *   -# <B>#lin_ifc_configuration</B>
 * @see ld_send_message
 *
 * @details
 * This function is read data by identifier. <br/>
 *	iii: name of interface										<br/>
 *	NAD: NAD value of the destination node for the transmission		 <br/>
 *	data: buffer for the data to be transmitted										 <br/>
 *		0x0202 - Read - A2D Values/Measurement											 <br/>
 *		0x0206 - Read - LIN Driver Information											 <br/>
 *		0xF18C - Read - ECU Barcode Number (Delphi)		<br/>
 *//*END*----------------------------------------------------------------------*/
void diag_read_data_by_identifier(l_ifc_handle iii, const l_u8 NAD, l_u8 * const data);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void diag_write_data_by_identifier(l_ifc_handle iii, l_u8 NAD,l_u16 data_length, l_u8 *const data)
* @brief This function is write data by identifier service (0x2E)
*
* @param iii <B>[IN]</B> lin interface handle
* @param NAD <B>[IN]</B> NAD value of the destination node for the transmission
* @param data_length <B>[IN]</B> data length of frame
* @param data <B>[IN]</B> buffer for the data to be transmitted
*
* @return #void
*
* @local_var
*   -# <B>#l_u16</B> <I>temp = 0</I>
*   -# <B>#l_u8</B> <I>Buff</I>
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#lin_configuration</B> <I>conf</I>
*
* @SDD_ID LIN_SDD_333
* @endif
*
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
* @see ld_send_message
*
* @details
* Write_Data_By_Identifier service (0x2E)  <br/>
*	iii: name of interface										<br/>
*	NAD: NAD value of the destination node for the transmission	<br/>
*	data: buffer for the data to be transmitted
*//*END*----------------------------------------------------------------------*/
void diag_write_data_by_identifier(l_ifc_handle iii, const l_u8 NAD, l_u16 data_length, l_u8 * const data);
void lin_diag_master_set_state(l_ifc_handle iii, lin_diagnostic_state state, l_u8 error);
/* END diagnostic class II services */
/* Diagnostic class III services, these services only belong to diagnostic class III */
#if (_DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_III_)
/* Session control */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void diag_session_control(l_ifc_handle iii, const l_u8 NAD,l_u16 data_length, l_u8 *const data)
* @brief This function is session control service (service id: 0x10)
*
* @param iii <B>[IN]</B> lin interface handle
* @param NAD <B>[IN]</B> NAD value of the destination node for the transmission
* @param data_length <B>[IN]</B> data length of frame
* @param data <B>[IN]</B> buffer for the data to be transmitted
*
* @return #void
*
* @local_var
*   -# <B>#l_u8</B> <I>Buff</I>
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#lin_configuration</B> <I>conf</I>
*
* @SDD_ID LIN_SDD_334
* @endif
*
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
* @see ld_send_message
*
* @details
*   This function is used for master node only. It will packing data and send request to slave node with service ID = 0x10: Session control 
*//*END*----------------------------------------------------------------------*/
void diag_session_control(l_ifc_handle iii, const l_u8 NAD,l_u16 data_length, l_u8 *const data);
/* Fault memory */

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void diag_fault_memory_read(l_ifc_handle iii, const l_u8 NAD,l_u16 data_length, l_u8 *const data)
* @brief This function is fault management read service (SID = 0x19)
*
* @param iii <B>[IN]</B> lin interface handle
* @param NAD <B>[IN]</B> NAD value of the destination node for the transmission
* @param data_length <B>[IN]</B> data length of frame
* @param data <B>[IN]</B> buffer for the data to be transmitted
*
* @return #void
*
* @local_var
*   -# <B>#l_u8</B> <I>Buff</I>
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#lin_configuration</B> <I>conf</I>
*
* @SDD_ID LIN_SDD_335
* @endif
*
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
* @see ld_send_message
*
* @details
*   This function is used for master node only. It will packing data and send request to slave node with service ID = 0x19: Fault memory read 
*//*END*----------------------------------------------------------------------*/
void diag_fault_memory_read (l_ifc_handle iii, const l_u8 NAD,l_u16 data_length, l_u8 *const data);
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void diag_fault_memory_clear(l_ifc_handle iii, const l_u8 NAD,l_u16 data_length, l_u8 *const data)
* @brief This function is fault management clear service (SID = 0x14)
*
* @param iii <B>[IN]</B> lin interface handle
* @param NAD <B>[IN]</B> NAD value of the destination node for the transmission
* @param data_length <B>[IN]</B> data length of frame
* @param data <B>[IN]</B> buffer for the data to be transmitted
*
* @return #void
*
* @local_var
*   -# <B>#l_u8</B> <I>Buff</I>
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#lin_configuration</B> <I>conf</I>
*
* @SDD_ID LIN_SDD_336
* @endif
*
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
* @see ld_send_message
*
* @details
*   This function is used for master node only. It will packing data and send request to slave node with service ID = 0x14: Fault memory clear
*//*END*----------------------------------------------------------------------*/
void diag_fault_memory_clear(l_ifc_handle iii, const l_u8 NAD,l_u16 data_length, l_u8 *const data);
/* IO control */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void diag_IO_control(l_ifc_handle iii, const l_u8 NAD,l_u16 data_length, l_u8 *const data)
* @brief This function is Input/Output control service (SID = 0x2F)
*
* @param iii <B>[IN]</B> lin interface handle
* @param NAD <B>[IN]</B> NAD value of the destination node for the transmission
* @param data_length <B>[IN]</B> data length of frame
* @param data <B>[IN]</B> buffer for the data to be transmitted
*
* @return #void
*
* @local_var
*   -# <B>#l_u8</B> <I>Buff</I>
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#lin_configuration</B> <I>conf</I>
*
* @SDD_ID LIN_SDD_337
* @endif
*
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
* @see ld_send_message
*
* @details
*   This function is used for master node only. It will packing data and send request to slave node with service ID = 0x2F: Input/Output control service
*//*END*----------------------------------------------------------------------*/
void diag_IO_control (l_ifc_handle iii, const l_u8 NAD,l_u16 data_length, l_u8 *const data);
#endif /* End (_DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_III_) */
/* End diagnostic class III services */
		
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
#endif /* End of (LIN_PROTOCOL == PROTOCOL_21) */
#endif /* End (LIN_MODE == _MASTER_MODE_) */ 
#if LIN_MODE == _SLAVE_MODE_
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
/* get diagnostic flag */
/*FUNCTION*--------------------------------------------------------------*//**
 * @fn l_u8 diag_get_flag(l_u8 flag_order)
 * @brief Get flag status of diagnostic service
 *
 * @param flag_order <B>[IN]</B> Order of service flag
 *
 * @return #l_u8
 *
 * @local_var 
 *   -# <B>#lin_configuration</B> <I>*conf</I>
 *
 * @SDD_ID LIN_SDD_
* @endif
 *
 *
 * @static_global_var
 *   -# <B>#lin_ifc_configuration</B> 
 *
 * @details
 * This function will return flag of diagnostic service, if lin node receive master request or slave response
 *//*END*----------------------------------------------------------------------*/
l_u8 diag_get_flag(l_u8 flag_order);  
/*FUNCTION*--------------------------------------------------------------*//**
 * @fn void diag_clear_flag(l_u8 flag_order)
 * @brief Clear flag status of diagnostic service
 *
 * @param flag_order <B>[IN]</B> Order of service flag
 *
 * @return #void
 *
 * @local_var 
 *   -# <B>#lin_configuration</B> <I>*conf</I>
 *
 * @SDD_ID LIN_SDD_
* @endif
 *
 *
 * @static_global_var
 *   -# <B>#lin_ifc_configuration</B> 
 *
 * @details
 * This function will clear flag of diagnostic service, if lin node receive master request or slave response
 *//*END*----------------------------------------------------------------------*/
void diag_clear_flag(l_u8 flag_order);
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)*/
#endif/* End LIN_MODE == _SLAVE_MODE_ */
/** @} */
#endif  /* _LIN_LIN21TL_API_H */
