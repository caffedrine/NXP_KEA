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
* @file      lin_j2602tl_api.c
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
 * 20090409		v1.0	First version
 *
 *****************************************************************************/
 
/**
* @}
*/

#include "LIN_Driver/include/lin_j2602tl_api.h"

#if LIN_MODE == _MASTER_MODE_
#if LIN_PROTOCOL == PROTOCOL_J2602

/** @addtogroup j2602tl_api_group
* @{ */
l_bool ld_is_ready 
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
  lin_configuration const *conf;
  
  /* Get the current configuration */
  conf = &lin_ifc_configuration[iii];
  /* Check the service status */
  if (LD_SERVICE_BUSY == conf->tl_desc->tl_service_status){
    return 0;
  }
  return 1;
}


l_u8 ld_check_response 
  (
    /* [IN] interface name */
    l_ifc_handle iii, 
    /* [OUT] buffer for saving the response ID */
    l_u8* const RSID, 
    /* [OUT] buffer for saving the error code */
    l_u8* const error_code
  )
{
  lin_last_cfg_result temp;
  lin_configuration const *conf;
  lin_tl_descriptor *_tl_desc;         

  
  /* Get the current configuration */
  conf = &lin_ifc_configuration[iii];
  /* Get the TL configuration */
  _tl_desc = conf->tl_desc;         

  /* Get the status of the last service */
  temp = (lin_last_cfg_result)(_tl_desc->tl_last_cfg_result);
  /* Check status of last configuration */
  switch (temp)
  {
    case  LD_SUCCESS:
      *RSID = _tl_desc->tl_last_RSID;
    break;
    case LD_NEGATIVE:
      *RSID =  _tl_desc->tl_last_RSID;
      *error_code = _tl_desc->tl_ld_error_code;
    break;
    
    default:
    break;
  } /* end of switch */
  return (l_u8)temp;
}


void ld_assign_frame_id 
  (
    /* [IN] interface name */
    l_ifc_handle iii, 
    /* [IN] initial node address  of the target node */
    l_u8 initial_NAD, 
    /* [IN] supplier ID of the target node */
    l_u16 supplier_id, 
    /* [IN] message ID of the target node */
    l_u16 message_id, 
    /* [IN] protected ID of the target node */
    l_u8 PID
  )
{
  lin_tl_pdu_data *lin_tl_pdu; 

  lin_configuration *conf;                               
  lin_tl_descriptor *tl_conf;
  
  /* Get current configuration */  
  conf = (lin_configuration *)&lin_ifc_configuration[iii];
  /* Get TL configuration */
  tl_conf = conf->tl_desc;
  /* Get pointer to TX single frame PDU */
  lin_tl_pdu = (lin_tl_pdu_data *)tl_conf->tl_tx_single_pdu;
  /* check service is busy? */  
  if (LD_SERVICE_BUSY != tl_conf->tl_service_status) {
    /* Create data for PDU */
    (*lin_tl_pdu)[0] = initial_NAD;
    (*lin_tl_pdu)[1] = 0x06;
    (*lin_tl_pdu)[2] = 0xB1;
    (*lin_tl_pdu)[3] = supplier_id &0x00FF;
    (*lin_tl_pdu)[4] = (supplier_id >> 8) & 0x00FF;
    (*lin_tl_pdu)[5] = message_id &0x00FF;
    (*lin_tl_pdu)[6] = (message_id >> 8) & 0x00FF;
    (*lin_tl_pdu)[7] = PID;
    /* Set state of service is BUSY */
    tl_conf->tl_service_status = LD_SERVICE_BUSY;
    /* Notify to Master task sending frame */
    tl_conf->tl_cnt_to_send = 1;
  }  /* End of checking service status */
}
/** @} */
#endif /* End (LIN_PROTOCOL == PROTOCOL_J2602) */
#endif /* End LIN_MODE == _MASTER_MODE_ */
