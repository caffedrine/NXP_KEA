/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2008-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************/
/**************************************************************************//**
* @addtogroup transport_group
* @{
******************************************************************************/
/**************************************************************************//**
*
* @file      lin_commontl_proto.c
*
* @author    FPT Software
*
* @brief     Common Handle LIN transport layer and configuration functions
*
******************************************************************************/
/******************************************************************************
 *
 * History:
 *
 * 20090409		v1.0	First version
 * 20111005		v1.1	Added code for save configuration request and response
 *
 *****************************************************************************/
 
#include "LIN_Driver/include/lin_commontl_proto.h"
#include "LIN_Driver/include/lin_diagnostic_service.h"

/* multi frame support */      
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
#include "lin_commontl_api.h"
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
/* Define functionality */
#ifndef MULTI_PDU_SUPPORT
#define MULTI_PDU_SUPPORT 0   			/**< multi PDU support */
#endif  /* MULTI_PDU_SUPPORT */

/********------------- Code supports SINGLE interface ----------------**********/

#if LIN_MODE == _SLAVE_MODE_
/* Unuse for GPIO */
#if (_LIN_GPIO_ == 0) && !defined(_MC9S08SC4_H) && !defined(MCU_SKEAZN84)
/*************************** FUNCTIONS *******************/


void lin_tl_make_slaveres_pdu  
  (
    /* [IN] service identifier */
    l_u8 sid,
    /* [IN] type of response */
    l_u8 res_type,
    /* [IN] Error code in case of negative response, if positive = 0 */
    l_u8 error_code
    
  )
{
  lin_tl_pdu_data lin_tl_pdu;
  l_u8 i;
  
  lin_tl_pdu[0] = lin_configured_NAD; 				 /* NAD */
  lin_tl_pdu[1] = 0x03;                              /* PCI */
  lin_tl_pdu[2] = RES_NEGATIVE;                      /* SID */
  lin_tl_pdu[3] = sid;                               /* D0  */
  lin_tl_pdu[4] = error_code;                        /* D1  */
  lin_tl_pdu[5] = 0xFF;                              /* D2  */
  lin_tl_pdu[6] = 0xFF;                              /* D3  */
  lin_tl_pdu[7] = 0xFF;                              /* D4  */

  
  switch (sid) {
#if (LIN_PROTOCOL == PROTOCOL_J2602)
    case SERVICE_TARGET_RESET: 
      /* PCI type */    
      lin_tl_pdu[1] = 0x06;
      /* SID */
      if (NEGATIVE == res_type){
         lin_tl_pdu[2] = RES_NEGATIVE;
      }else{
         lin_tl_pdu[2] = RES_POSITIVE+ sid;  
      }
      /* Get Identifier infor */
      lin_tl_pdu[3] = (l_u8)(product_id.supplier_id & 0xFF);
      lin_tl_pdu[4] = (l_u8)(product_id.supplier_id >>8);
      lin_tl_pdu[5] = (l_u8)(product_id.function_id & 0xFF);
      lin_tl_pdu[6] = (l_u8)(product_id.function_id >>8);
      lin_tl_pdu[7] = product_id.variant;
      break;
#endif /* End (LIN_PROTOCOL == PROTOCOL_J2602) */  

#if (LIN_PROTOCOL == PROTOCOL_21)
   
    case SERVICE_READ_BY_IDENTIFY:  
      if (POSITIVE == res_type){
        /* PCI type */
        lin_tl_pdu[1] = PCI_RES_READ_BY_IDENTIFY;
        /* SID */
        lin_tl_pdu[2] = RES_POSITIVE + sid;
        
        if(error_code == LIN_PRODUCT_IDENT){
          /* Get Identifier infor */
          lin_tl_pdu[3] = (l_u8)(product_id.supplier_id & 0xFF);
          lin_tl_pdu[4] = (l_u8)(product_id.supplier_id >>8);
          lin_tl_pdu[5] = (l_u8)(product_id.function_id & 0xFF);
          lin_tl_pdu[6] = (l_u8)(product_id.function_id >>8);
          lin_tl_pdu[7] = product_id.variant;
        }else if(error_code >= LIN_READ_USR_DEF_MIN && error_code <= LIN_READ_USR_DEF_MAX){
          l_u8 data_callout[5];
          l_u8 retval = ld_read_by_id_callout(error_code, data_callout);
        
          if(retval == LD_POSITIVE_RESPONSE){
            /* packing user defined pdu */
            lin_tl_pdu[3] = data_callout[0];
            lin_tl_pdu[4] = data_callout[1];
            lin_tl_pdu[5] = data_callout[2];
            lin_tl_pdu[6] = data_callout[3];
            lin_tl_pdu[7] = data_callout[4];
          }
        }
      }
      break;
	  
    case SERVICE_ASSIGN_FRAME_ID_RANGE:  /* Mandatory for TL LIN 2.1 */
      if (POSITIVE == res_type) {
        lin_tl_pdu[1] = PCI_RES_ASSIGN_FRAME_ID_RANGE;
        lin_tl_pdu[2] = RES_POSITIVE + sid; 
        lin_tl_pdu[3] = 0xFF;
        lin_tl_pdu[4] = 0xFF; 
      }
      break;
	  
	case SERVICE_SAVE_CONFIGURATION:  
      if (POSITIVE == res_type){
        /* PCI type */
        lin_tl_pdu[1] = PCI_RES_SAVE_CONFIGURATION;
        /* SID */
        lin_tl_pdu[2] = RES_POSITIVE + sid;
        /* Data unused */
        lin_tl_pdu[3] = 0xFF;
        lin_tl_pdu[4] = 0xFF;
      }
      break;
      
	case SERVICE_ASSIGN_NAD:
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
		lin_configured_NAD = lin_tl_rx_queue.tl_pdu[lin_tl_rx_queue.queue_header][7];
#else  /* Single frame support */
		lin_configured_NAD = (*tl_current_rx_pdu_ptr)[7];
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)*/
		lin_tl_pdu[1] = 0x01;                              /* PCI */
		lin_tl_pdu[2] = 0xF0;                              /* RSID */
		lin_tl_pdu[3] = 0xFF;
		lin_tl_pdu[4] = 0xFF;
		
	break;
	case SERVICE_CONDITIONAL_CHANGE_NAD:
		lin_tl_pdu[1] = 0x01;                              /* PCI */
		lin_tl_pdu[2] = 0xF3;                              /* RSID */
		lin_tl_pdu[3] = 0xFF;
		lin_tl_pdu[4] = 0xFF;
	break;
	  
#endif /* End (LIN_PROTOCOL == PROTOCOL_21) */  
    default:
      break;
  }/* end of switch statement */
/* Multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
  lin_tl_tx_queue.queue_status = LD_QUEUE_FULL;
  lin_tl_tx_queue.queue_current_size = 1;
  
  /* Put to transmit queue */
  for (i=0; i<8; i++){
    lin_tl_tx_queue.tl_pdu[lin_tl_tx_queue.queue_header][i] = lin_tl_pdu[i];
  }
  /* Set check N_As Timeout  */
  tl_tx_msg_index = lin_tl_tx_queue.queue_tail;
  tl_tx_msg_size = 1;
  tl_tx_msg_status = LD_IN_PROGRESS;
#else /* Single frame support */
  for (i=0; i<8; i++){
    tx_single_pdu_data[i] = lin_tl_pdu[i];
  }
  /* set current TX PDU to send out */      
  tl_current_tx_pdu_ptr = (lin_tl_pdu_data*)tx_single_pdu_data;
#endif /*End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)*/  
  
  /* set number of SlaveResp response data */
  tl_slaveresp_cnt = 1;
}

void lin_tl_get_pdu()
{
  l_u8 i;
/* Multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
  lin_tl_pdu_data lin_tl_pdu;
  
  tl_get_raw(lin_tl_pdu, &lin_tl_tx_queue, TRANSMISSION);
  /* Copy PDU to response buffer */  
  for (i=1; i<9; i++) {
    lin_lld_response_buffer[i] = lin_tl_pdu[i - 1];
  }
#else /* Single frame support */
  /* Copy PDU to response buffer */  
  for (i=1; i<9; i++) {
    lin_lld_response_buffer[i] = (*tl_current_tx_pdu_ptr)[i - 1];
  }
#endif /*End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)*/
}

/* Single frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_SINGLE_FRAME_)
void lin_tl_put_pdu ()
{
  l_u8 i;
  
  /* Set PDU for RX buffer */
  tl_current_rx_pdu_ptr = &rx_single_pdu_data;
    
  for (i=0; i<8; i++) {
    (*tl_current_rx_pdu_ptr)[i] = lin_lld_response_buffer[i+1];
  }
}
#endif /*End (_TL_FRAME_SUPPORT_ == _TL_SINGLE_FRAME_)*/


/* Multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_process_pdu(lin_tl_pdu_data *pdu)
* @brief LIN process packet data unit
*
* @param pdu <B>[IN]</B> packet data unit
*
* @return #void
*
* @SDD_ID LIN_SDD_354
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>pci_type</I>
*   -# <B>#lin_configuration</B> <I>*conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>*tl_conf</I>
*   -# <B>#l_u16</B> <I>length</I>
*   -# <B>#l_u8</B> <I>tmp_frame_counter</I>
*   -# <B>#lin_transport_layer_queue</B> <I>*tl_queue</I>
*   -# <B>#l_u8</B> <I>i</I>
*
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @see tl_get_raw
* @see tl_put_raw
* @see lin_tl_get_response
*
* @details
*   LIN process packet data unit
*//*END*----------------------------------------------------------------------*/

static void lin_process_pdu(lin_tl_pdu_data *pdu) {
  l_u8 pci_type;
  l_u16 length;
  l_u8 tmp_frame_counter;

  /* get PCI type */
  pci_type = ((*pdu)[1] & 0xF0) >> 4;
  switch (pci_type) {
  case PCI_SF:
    length = ((*pdu)[1]) & 0x0F;
    /* check length of SF. If not valid, ignore this PDU */
    if (length <= 6) {
      tl_receive_msg_status = LD_COMPLETED;
      tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
      tl_service_status = LD_REQUEST_FINISHED;
      /* put PDU to rx queue */
      lin_tl_rx_queue.queue_header =0;
      lin_tl_rx_queue.queue_tail =0;
      lin_tl_rx_queue.queue_current_size = 0;
      lin_tl_rx_queue.queue_status = LD_NO_DATA;
      tl_put_raw(&(lin_lld_response_buffer[1]), &lin_tl_rx_queue,	RECEIVING);      
      tl_frame_counter = 1;
      tl_no_of_pdu = 1;
      if(tl_diag_state != LD_DIAG_RX_FUNCTIONAL)
        tl_diag_state = LD_DIAG_TX_PHY;
      lin_tl_attach_service();
    } else {
      tl_diag_state = LD_DIAG_IDLE;
    }
    break;
  case PCI_FF:
    length = ((*pdu)[1] & 0x0F) * 256 + ((*pdu)[2]);
    /* check length of FF. If not valid, ignore this PDU */
    if (length >= 7 && length <= (MAX_QUEUE_SIZE*6 - 1)) {
      /* Set check N_Cr timeout */
      tl_check_timeout = N_MAX_TIMEOUT_CNT;
      tl_check_timeout_type = LD_CHECK_N_CR_TIMEOUT;
      /* put PDU to rx queue */
      lin_tl_rx_queue.queue_header =0;
      lin_tl_rx_queue.queue_tail =0;
      lin_tl_rx_queue.queue_current_size = 0;
      lin_tl_rx_queue.queue_status = LD_NO_DATA;
      tl_put_raw(lin_lld_response_buffer + 1, &lin_tl_rx_queue, RECEIVING);

      /* canculate number of PDU for this message */
      if((length-5)%6==0)
        tl_no_of_pdu = (l_u8)((length - 5) / 6);
      else
        tl_no_of_pdu = (l_u8)((length - 5) / 6) + 1;
      /* set frame counter = 1 */
      tl_frame_counter = 1;
      tl_service_status = LD_SERVICE_BUSY;
      if(tl_diag_state != LD_DIAG_RX_FUNCTIONAL)
        tl_diag_state = LD_DIAG_RX_PHY;
    } else {
      tl_diag_state = LD_DIAG_IDLE;
    }
    break;
  case PCI_CF:
    /* Set check N_Cr timeout */
    tl_check_timeout = N_MAX_TIMEOUT_CNT;
    tl_check_timeout_type = LD_CHECK_N_CR_TIMEOUT;
    /* get frame counter of this PDU */
    tmp_frame_counter = ((*pdu)[1] & 0x0F);
    /* Check valid frame counter */
    if (tmp_frame_counter == tl_frame_counter) {
      /* increase frame counter */
      tl_frame_counter++;
      if(tl_frame_counter > 15)
        tl_frame_counter = 0;
      /* decrease number of PDU to check message is complete */
      tl_no_of_pdu--;
      /* put PDU to rx queue */
      tl_put_raw(&(lin_lld_response_buffer[1]), &lin_tl_rx_queue,	RECEIVING);
      if(tl_diag_state != LD_DIAG_RX_FUNCTIONAL)
        tl_diag_state = LD_DIAG_RX_PHY;
    } else {
      /* abort this message */
      tl_receive_msg_status = LD_WRONG_SN;
      tl_rx_msg_status = LD_WRONG_SN;
      tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
    }
    if (0 == tl_no_of_pdu) {
      /* message is received completely */
      /* set status is IDLE to receive new message */
      
      tl_receive_msg_status = LD_COMPLETED;
      tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
      
      tl_service_status = LD_REQUEST_FINISHED;
      if(tl_diag_state != LD_DIAG_RX_FUNCTIONAL)
        tl_diag_state = LD_DIAG_TX_PHY;
      lin_tl_attach_service();
    }
    break;
  default:
    /* ignore this PDU */
    break;
  }
}
#endif /*End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)*/

void lin_tl_handler()
{
  l_u8 NAD, SID;
/* Multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
  l_u8 frame_type;
  NAD = lin_lld_response_buffer[1];
  SID = lin_lld_response_buffer[3];
  frame_type = (l_u8)((lin_lld_response_buffer[2] & 0xF0)>>4);
  /* check NAD whether or not belongs to this slave node */
  /* receive both broadcast NAD and functional NAD */
	if ((LD_BROADCAST != NAD) &&
	    (LD_FUNCTIONAL_NAD != NAD) &&
	    (((lin_configured_NAD != NAD) 
	       && (SERVICE_ASSIGN_NAD != SID))
	     || ((lin_initial_NAD != NAD) 
	       && (SERVICE_ASSIGN_NAD == SID)))) {
		/* ignore any response */
    tl_slaveresp_cnt = 0;
		return;
	}
	/* check if  functional request is received while transmitting response */
	if((tl_diag_state == LD_DIAG_TX_PHY || tl_diag_state == LD_DIAG_RX_PHY) && NAD==LD_FUNCTIONAL_NAD && tl_service_status == LD_SERVICE_BUSY){
	  tl_check_timeout = N_MAX_TIMEOUT_CNT;
    tl_check_timeout_type = LD_CHECK_N_CR_TIMEOUT;
	  /* ignore request */
	  NAD=0x00;
	  return;
	}
	/* check if slave node is transmitting response while receive functional request */
	if(tl_diag_state == LD_DIAG_TX_PHY && NAD!=LD_FUNCTIONAL_NAD){
	  /* clear received request & response */
	  tl_service_status = LD_SERVICE_BUSY;
	  /* clear queue */
	  lin_tl_tx_queue.queue_tail = 0;
	  lin_tl_tx_queue.queue_header = 0;
	  lin_tl_tx_queue.queue_current_size = 0;
	  lin_tl_tx_queue.queue_status = LD_QUEUE_EMPTY;
	  
	  lin_tl_rx_queue.queue_tail = 0;
	  lin_tl_rx_queue.queue_header = 0;
	  lin_tl_rx_queue.queue_current_size = 0;
	  lin_tl_rx_queue.queue_status = LD_NO_DATA;
	  /* transmit status*/
	  tl_tx_msg_status = LD_COMPLETED;
	  /* receive status*/
	  tl_receive_msg_status = LD_NO_MSG;
	  tl_rx_msg_status = LD_COMPLETED;
	  
	  tl_slaveresp_cnt = 0;
	  /* then receive and process new request */
	}
	
	if(tl_service_status == LD_SERVICE_ERROR && frame_type == PCI_CF)
	  return;
	
	if(NAD != LD_FUNCTIONAL_NAD)
	  tl_diag_state = LD_DIAG_RX_PHY;
	else
	  tl_diag_state = LD_DIAG_RX_FUNCTIONAL;
	/* call process pdu */
	lin_process_pdu((lin_tl_pdu_data *)&(lin_lld_response_buffer[1]));

#else /* Single frame support */

  l_u8 pci_type, length;
  
  NAD = (*tl_current_rx_pdu_ptr)[0];
  SID = (*tl_current_rx_pdu_ptr)[2];
  /* check NAD whether or not belongs to this slave node */
  /* receive both broadcast NAD and functional NAD */
  
  if (NAD ==0x05){
	  length = 1;
  }
  if ((LD_BROADCAST != NAD)
      && (LD_FUNCTIONAL_NAD != NAD)
      &&
      (((lin_configured_NAD != NAD) && (SERVICE_ASSIGN_NAD != SID))
           || ((lin_initial_NAD != NAD) && (SERVICE_ASSIGN_NAD == SID)))
  ){
    tl_slaveresp_cnt = 0;
    return;
  }
  
  /* Get PCI type */
  pci_type = ((*tl_current_rx_pdu_ptr)[1] & 0xF0)>>4;
  switch (pci_type) 
  {
    case PCI_SF: /* Single frame */
      /* error handling: check length of message */
      length = (*tl_current_rx_pdu_ptr)[1] & 0x0F;
      if (6 < length){
        return;
      }
      lin_tl_attach_service();
      break;
    default:
     break;
  } /* End of ckeck PCI_type */
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
}

void lin_tl_attach_service()
{
  l_u8 sid;

/* Multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
  l_u8 frame_type, NAD;
   
  /* get frame type */
  frame_type = (lin_tl_rx_queue.tl_pdu[lin_tl_rx_queue.queue_header][1] & 0xF0)>>4;
  NAD = lin_tl_rx_queue.tl_pdu[lin_tl_rx_queue.queue_header][0];
  /* check if request is functional request */
  if(NAD == LD_FUNCTIONAL_NAD){
    #if (LIN_PROTOCOL == PROTOCOL_21) 
    lin_diagsrv_functional_service();
    #endif /* End (LIN_PROTOCOL == PROTOCOL_21) */
    /* update service state */
    tl_diag_state = LD_DIAG_IDLE;
    tl_service_status = LD_SERVICE_IDLE;
    /* ignore response */
    tl_no_of_pdu = 0;
    tl_frame_counter = 0;
    return;
  }
  
  /* Get SID information  */
  if(frame_type == PCI_FF)
    sid = lin_tl_rx_queue.tl_pdu[lin_tl_rx_queue.queue_header][3];
  else
    sid = lin_tl_rx_queue.tl_pdu[lin_tl_rx_queue.queue_header][2];
#else /* Single frame support */
  l_u8 NAD;
  
  NAD = (*tl_current_rx_pdu_ptr)[0];
  /* check functional request */
  if(NAD == LD_FUNCTIONAL_NAD){
    
    return;
  }
  /* Get SID information  */
  sid = (*tl_current_rx_pdu_ptr)[2];
#endif /*End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)*/
  
  switch (sid)
  {
#if LIN_PROTOCOL == PROTOCOL_21
    case SERVICE_READ_BY_IDENTIFY:                       /* Mandatory for TL LIN 2.1 & 2.0 */
		lin_diagservice_read_by_identifier();             
    break;
	
	case SERVICE_ASSIGN_FRAME_ID_RANGE:                  /* Mandatory for TL LIN 2.1 */
		lin_diagservice_assign_frame_id_range();   
      break;    
    
	case SERVICE_SAVE_CONFIGURATION:
		/* Set save configuration flag */
		lin_save_configuration_flg = 1;
		/* Response to master - RSID */
		lin_tl_make_slaveres_pdu(SERVICE_SAVE_CONFIGURATION, POSITIVE, 0);
	break;
	
	case SERVICE_ASSIGN_NAD:
		lin_tl_make_slaveres_pdu(SERVICE_ASSIGN_NAD, POSITIVE, 0);
	break;
	
	case SERVICE_CONDITIONAL_CHANGE_NAD:
		lin_condittional_change_nad();
		
	break;
		
#endif /* (End LIN_PROTOCOL == LIN_PROTOCOL_21) */   

#if LIN_PROTOCOL == PROTOCOL_J2602
    case SERVICE_TARGET_RESET:
      lin_diagservice_target_reset();
    break;
#endif /* (End LIN_PROTOCOL == LIN_PROTOCOL_J2602) */     
    
/* check diagnostic service is belong to class II or class III */
#if (_DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_II_ || _DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_III_)
    /* set diagnostic service flag */
    case SERVICE_READ_DATA_BY_IDENTIFY:
      lin_diag_services_flag[DIAGSRV_READ_DATA_BY_IDENTIFIER_ORDER]=1;
      break;
    case SERVICE_WRITE_DATA_BY_IDENTIFY:
      lin_diag_services_flag[DIAGSRV_WRITE_DATA_BY_IDENTIFIER_ORDER] = 1;
      break;
	#if (_DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_III_)
    case SERVICE_SESSION_CONTROL:
      lin_diag_services_flag[DIAGSRV_SESSION_CONTROL_ORDER] = 1;
      break;
    case SERVICE_IO_CONTROL_BY_IDENTIFY:
      lin_diag_services_flag[DIAGSRV_IO_CONTROL_BY_IDENTIFIER_ORDER]=1;
      break;
    case SERVICE_FAULT_MEMORY_READ:
      lin_diag_services_flag[DIAGSRV_FAULT_MEMORY_READ_ORDER] = 1;
      break;
    case SERIVCE_FAULT_MEMORY_CLEAR:
      lin_diag_services_flag[DIAGSRV_FAULT_MEMORY_CLEAR_ORDER] = 1;
      break;
	#endif /* End (_DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_III_) */
#endif /* End (_DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_II_ || _DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_III_) */    

    default:
      lin_tl_make_slaveres_pdu(sid, NEGATIVE, SERVICE_NOT_SUPPORTED); 
    break;
  } /* end of switch */
}


#endif /* End (_LIN_GPIO_ == 0) && !defined(_MC9S08SC4_H) */
#endif  /* End (LIN_MODE == _SLAVE_MODE_)  */

/********------------ End of code for SINGLE Interface --------------**********/


/********------------- Code supports Multi interface ----------------**********/
#if LIN_MODE == _MASTER_MODE_
#ifdef MULTI_TIMER_MODE
extern const l_u16 max_tl_timeout_counter[LIN_NUM_OF_IFCS];
#endif /* End MULTI_TIMER_MODE */

/*************************** FUNCTIONS *******************/

/* multi frame support */      
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
void tl_process_mreq
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
  const lin_configuration *conf;   
  lin_tl_descriptor *tl_conf;
  
  /* Get current configuration */  
  conf = &lin_ifc_configuration[iii];
  /* Get TL configuration */
  tl_conf = conf->tl_desc;

  /* decrease number of PDU for sending message*/
  tl_conf->tl_tx_msg_size--;
  /* Check message is sent completely */
  if (0 == tl_conf->tl_tx_msg_size){
    tl_conf->tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
    tl_conf->tl_tx_msg_status = LD_COMPLETED;
    *conf->tl_service_status = LD_REQUEST_FINISHED;
    /* back to normal table */
    *conf->active_schedule_id = *conf->previous_schedule_id;
    conf->schedule_start_entry[*(conf->active_schedule_id)] = 0;
  } else {
#ifdef MULTI_TIMER_MODE
    tl_conf->tl_check_timeout = max_tl_timeout_counter[iii];
#else
    tl_conf->tl_check_timeout = N_MAX_TIMEOUT_CNT;
#endif /* End MULTI_TIMER_MODE */
    tl_conf->tl_check_timeout_type = LD_CHECK_N_AS_TIMEOUT; 
  }
}


/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_process_pdu(l_ifc_handle iii, lin_tl_pdu_data *pdu)
* @brief LIN process packet data unit
*
* @param iii <B>[IN]</B> interface name
* @param pdu <B>[IN]</B> packet data unit
*
* @return #void
*
* @SDD_ID LIN_SDD_327
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>pci_type</I>
*   -# <B>#lin_configuration</B> <I>*conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>*tl_conf</I>
*   -# <B>#l_u16</B> <I>length</I>
*   -# <B>#l_u8</B> <I>tmp_frame_counter</I>
*   -# <B>#lin_transport_layer_queue</B> <I>*tl_queue</I>
*   -# <B>#l_u8</B> <I>i</I>
*
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @see tl_get_raw
* @see tl_put_raw
* @see lin_tl_get_response
*
* @details
*   LIN process packet data unit
*//*END*----------------------------------------------------------------------*/

static void lin_process_pdu(l_ifc_handle iii, lin_tl_pdu_data *pdu)
{
  l_u8 pci_type;
  l_u16 length;
  l_u8 tmp_frame_counter;
  const lin_configuration * conf;
  lin_tl_descriptor *tl_conf;
  
    /* Get the current configuration */
  conf = (lin_configuration *)&lin_ifc_configuration[iii];
  tl_conf = conf->tl_desc;

  /* get PCI type */
  pci_type = ((*pdu)[1] & 0xF0)>>4;
  switch(pci_type){
    case PCI_SF:
      length = ((*pdu)[1]) & 0x0F;
      /* check length of SF. If not valid, ignore this PDU */
      if (length <= 6){
        if (_MASTER_ == conf->function){
          /* reset queue header and queue tail*/
          tl_conf->tl_rx_queue->queue_header =0;
          tl_conf->tl_rx_queue->queue_tail =0;
          tl_conf->tl_rx_queue->queue_current_size = 0;
          tl_conf->tl_rx_queue->queue_status = LD_NO_DATA;
          /* put to rx queue */
          tl_put_raw(&(conf->response_buffer[1]),tl_conf->tl_rx_queue,RECEIVING);
          
          /* process diagnostic interleaved mode */
          *conf->tl_diag_interleave_state = DIAG_NOT_START;
          
          /* set status is IDLE to receive new message */
          tl_conf->tl_receive_msg_status = LD_COMPLETED;
          
          *conf->diagnostic_mode = DIAG_NONE;
          *conf->tl_service_status = LD_SERVICE_IDLE;
          tl_conf->tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
          
          /* process response */
          lin_tl_get_response(iii);
          
          /* switch to normal table */
          *conf->active_schedule_id = *conf->previous_schedule_id;
          conf->schedule_start_entry[*conf->active_schedule_id] = 0;
        } else { /* Slave interface */
          /* put PDU to rx queue */
          tl_conf->tl_receive_msg_status = LD_COMPLETED;
          tl_conf->tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
          *conf->tl_service_status = LD_REQUEST_FINISHED;
          /* reset queue header and queue tail*/
          tl_conf->tl_rx_queue->queue_header =0;
          tl_conf->tl_rx_queue->queue_tail =0;
          tl_conf->tl_rx_queue->queue_current_size = 0;
          tl_conf->tl_rx_queue->queue_status = LD_NO_DATA;
          /* put to rx queue */        
          tl_put_raw(&(conf->response_buffer[1]),tl_conf->tl_rx_queue,RECEIVING);
          tl_conf->tl_frame_counter = 1;
          tl_conf->tl_no_of_pdu = 1;
          lin_tl_attach_service(iii);
        }
      }
    break;
    case PCI_FF:
      /* for both MASTER and SLAVE */
      length = ((*pdu)[1] & 0x0F) * 256 + ((*pdu)[2]);
      /* check length of FF. If not valid, ignore this PDU */
      if (length >= 7 && length <= (MAX_QUEUE_SIZE*6 - 1)){
        /* Set check N_Cr timeout */
        
#ifdef MULTI_TIMER_MODE
    tl_conf->tl_check_timeout = max_tl_timeout_counter[iii];
#else
    tl_conf->tl_check_timeout = N_MAX_TIMEOUT_CNT;
#endif /* End MULTI_TIMER_MODE */

        tl_conf->tl_check_timeout_type = LD_CHECK_N_CR_TIMEOUT; 
        /* put PDU to rx queue */  
        /* reset queue header and queue tail*/
        tl_conf->tl_rx_queue->queue_header =0;
        tl_conf->tl_rx_queue->queue_tail =0;
        tl_conf->tl_rx_queue->queue_current_size = 0;
        tl_conf->tl_rx_queue->queue_status = LD_NO_DATA;
        tl_put_raw(&(conf->response_buffer[1]),tl_conf->tl_rx_queue,RECEIVING);
         
        /* calculate number of PDU for this message */
        if((length-5)%6==0)
          tl_conf->tl_no_of_pdu = (l_u8)((length - 5) / 6);
        else
          tl_conf->tl_no_of_pdu = (l_u8)((length - 5) / 6) + 1;
        /* set frame counter = 1 */
        tl_conf->tl_frame_counter = 1;
        *conf->tl_service_status = LD_SERVICE_BUSY;
      }
    break;
    case PCI_CF:
      /* Only for MASTER */
      if (_MASTER_ == conf->function){
        /* Set check N_Cr timeout */
#ifdef MULTI_TIMER_MODE
    tl_conf->tl_check_timeout = max_tl_timeout_counter[iii];
#else
    tl_conf->tl_check_timeout = N_MAX_TIMEOUT_CNT;
#endif /* End MULTI_TIMER_MODE */
        tl_conf->tl_check_timeout_type = LD_CHECK_N_CR_TIMEOUT; 
        /* get frame counter of this PDU */
        tmp_frame_counter = ((*pdu)[1]&0x0F);
        /* Check valid frame counter */
        if (tmp_frame_counter == tl_conf->tl_frame_counter){
          /* increase frame counter */
          tl_conf->tl_frame_counter++;
          if(tl_conf->tl_frame_counter > 15)
            tl_conf->tl_frame_counter = 0;
          /* decrease number of PDU to check message is complete */
          tl_conf->tl_no_of_pdu--;
          /* put PDU to rx queue */          
          tl_put_raw(&(conf->response_buffer[1]),tl_conf->tl_rx_queue,RECEIVING);
        } else {
          /* abort this message */
          tl_conf->tl_rx_msg_status = LD_WRONG_SN;
          tl_conf->tl_receive_msg_status = LD_WRONG_SN;
          tl_conf->tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
          *conf->diagnostic_mode = DIAG_NONE;
          /* switch to normal table */
          *conf->active_schedule_id = *conf->previous_schedule_id;
          conf->schedule_start_entry[*conf->active_schedule_id] = 0;
        }
        if(0 == tl_conf->tl_no_of_pdu){
          /* message is received completely */
          
          /* set status is IDLE to receive new message */
          tl_conf->tl_receive_msg_status = LD_COMPLETED;
          tl_conf->tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
          *conf->tl_service_status = LD_SERVICE_IDLE;
          /* process diagnostic interleaved mode */
          if(*conf->current_pid == 0x3D && *conf->diagnostic_mode==DIAG_INTER_LEAVE_MODE){
            *conf->tl_diag_interleave_state = DIAG_NOT_START;
            *conf->diagnostic_mode = DIAG_NONE;
          }
          
          /* call function to process this response */
          lin_tl_get_response(iii);
          /* switch to normal table */
          *conf->active_schedule_id = *conf->previous_schedule_id;
          conf->schedule_start_entry[*conf->active_schedule_id] = 0;
          
        }   
      } else { /* for SLAVE */
        /* Set check N_Cr timeout */
#ifdef MULTI_TIMER_MODE
    tl_conf->tl_check_timeout = max_tl_timeout_counter[iii];
#else
    tl_conf->tl_check_timeout = N_MAX_TIMEOUT_CNT;
#endif /* End MULTI_TIMER_MODE*/
        tl_conf->tl_check_timeout_type = LD_CHECK_N_CR_TIMEOUT; 
        /* get frame counter of this PDU */
        tmp_frame_counter = ((*pdu)[1]&0x0F);
        /* Check valid frame counter */
        if (tmp_frame_counter == tl_conf->tl_frame_counter){
          /* increase frame counter */
          tl_conf->tl_frame_counter++;
          if(tl_conf->tl_frame_counter > 15)
            tl_conf->tl_frame_counter = 0;
          /* decrease number of PDU to check message is complete */
          tl_conf->tl_no_of_pdu--;
          /* put PDU to rx queue */          
          tl_put_raw(&(conf->response_buffer[1]),tl_conf->tl_rx_queue,RECEIVING);
        } else {
          /* abort this message */
          tl_conf->tl_receive_msg_status = LD_WRONG_SN;
          tl_conf->tl_rx_msg_status = LD_WRONG_SN;
          tl_conf->tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
        }
        if(0 == tl_conf->tl_no_of_pdu){
          /* message is received completely */          
          /* set status is IDLE to receive new message */
          tl_conf->tl_receive_msg_status = LD_COMPLETED;
          tl_conf->tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
          *conf->tl_service_status = LD_REQUEST_FINISHED;
          
          lin_tl_attach_service(iii);
        }   
      }
    break;
    default:
      /* ignore this PDU */
    break;
  }
}


void lin_tl_handler_error
  (
    /* [IN] interface name */
    l_ifc_handle iii,
    /* [IN] interface name */
    l_u8 pid
  )
{
  const lin_configuration* conf;
  lin_tl_descriptor *tl_conf;

  /* Get current configuration */
  conf = &lin_ifc_configuration[iii];
  /* Get TL description */
  tl_conf = conf->tl_desc;

  /* for master */ 
  if((_MASTER_ == conf->function) && (DIAG_INTER_LEAVE_MODE == *conf->diagnostic_mode)){
    /* Master request */
    if(0x3C == pid){
      /* update status of message and queue */
      tl_conf->tl_tx_msg_status = LD_FAILED;
      tl_conf->tl_tx_queue->queue_status = LD_TRANSMIT_ERROR;
    } else { /* Slave Response */
      /* update status of message and queue */
      tl_conf->tl_receive_msg_status = LD_FAILED;
      tl_conf->tl_rx_msg_status = LD_FAILED;
      tl_conf->tl_rx_queue->queue_status = LD_RECEIVE_ERROR;
    } /* end of (0x3C == pid) */
    tl_conf->tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
    *conf->tl_service_status = LD_SERVICE_ERROR;
    *conf->diagnostic_mode= DIAG_NONE;
    /* switch to normal table */
    *conf->active_schedule_id = *conf->previous_schedule_id;
    conf->schedule_start_entry[*conf->active_schedule_id] = 0;
  } else {
    /*for slave */
    /* Master request */
    if(0x3C == pid){
      /* update status of message and queue */
      tl_conf->tl_receive_msg_status = LD_FAILED;
      tl_conf->tl_rx_msg_status = LD_FAILED;
      tl_conf->tl_rx_queue->queue_status = LD_RECEIVE_ERROR;
      tl_conf->tl_slaveresp_cnt = 0;
    } else { /* Slave Response */
      /* update status of message and queue */
      tl_conf->tl_tx_msg_status = LD_FAILED;
      tl_conf->tl_tx_queue->queue_status = LD_TRANSMIT_ERROR;
    } /* end of (0x3C == pid) */
    *conf->tl_service_status = LD_SERVICE_ERROR;
    tl_conf->tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
  }
}
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */


void lin_tl_make_mreq_pdu
  (
    /* [IN] interface name */
    l_ifc_handle iii
/* single frame support */      
#if (_TL_FRAME_SUPPORT_ == _TL_SINGLE_FRAME_)
    ,/* [IN] PDU pointer */
    lin_tl_pdu_data *pdu_ptr
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_SINGLE_FRAME_) */
  )
{
  const lin_configuration *conf;   
  lin_tl_descriptor *tl_conf;
/* Single frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_SINGLE_FRAME_)
  l_u8 i;
  lin_tl_pdu_data *tl_tx_pdu;
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_SINGLE_FRAME_) */
  /* Get current configuration */  
  conf = &lin_ifc_configuration[iii];
  /* Get TL configuration */
  tl_conf = conf->tl_desc;  
/* multi frame support */      
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
  tl_get_raw(&conf->response_buffer[1],tl_conf->tl_tx_queue,TRANSMISSION);
  conf->response_buffer[0] = 8;
#else /* single frame support */
  /* check if Service Mode and set current PDU pointer to PDU data based on Diag Service */
  
  tl_conf->tl_current_tx_pdu_ptr = (lin_tl_pdu_data*)tl_conf->tl_tx_single_pdu;
  tl_tx_pdu = (lin_tl_pdu_data*)tl_conf->tl_tx_single_pdu;
    
  for(i=0; i<8; ++i){
    conf->response_buffer[i+1] = (*tl_tx_pdu)[i];
  }
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */  
}

void lin_tl_make_slaveres_pdu  
  (
    /* [IN] interface name */
    l_ifc_handle iii,
    /* [IN] service identifier */
    l_u8 sid,
    /* [IN] type of response */
    l_u8 res_type,
    /* [IN] Error code in case of negative response, if positive = 0*/
    l_u8 error_code
    
  )
{
  const lin_configuration * conf;
  lin_tl_descriptor *tl_conf;
  const lin_node_attribute *node_conf;
  lin_product_id *ident;
  /* multi frame support */      
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
  lin_tl_pdu_data lin_tl_pdu_buff, *lin_tl_pdu;
  l_u8 i;
  lin_tl_pdu = (lin_tl_pdu_data *)&lin_tl_pdu_buff;
#else /* single frame support */
  lin_tl_pdu_data *lin_tl_pdu;
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
  
  /* Get the current configuration */
  conf = &lin_ifc_configuration[iii];
  
  /* Get TL configuratio */
  tl_conf = conf->tl_desc;

/* Single frame suport */  
#if (_TL_FRAME_SUPPORT_ == _TL_SINGLE_FRAME_)
  lin_tl_pdu = (lin_tl_pdu_data *)tl_conf->tl_tx_single_pdu;
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_SINGLE_FRAME_) */
  
  /* Get the node attribute */
  node_conf = conf->node_attribute;
  
  /* Pack data to response PDU */  
  (*lin_tl_pdu)[0] = node_conf->configured_NAD;
  (*lin_tl_pdu)[1] = 0x03;                              /* PCI */
  (*lin_tl_pdu)[2] = RES_NEGATIVE;                      /* SID */
  (*lin_tl_pdu)[3] = sid;                               /* D0  */
  (*lin_tl_pdu)[4] = error_code;                        /* D1  */
  (*lin_tl_pdu)[5] = 0xFF;                              /* D2  */
  (*lin_tl_pdu)[6] = 0xFF;                              /* D3  */
  (*lin_tl_pdu)[7] = 0xFF;                              /* D4  */
  
   switch (sid) {
#if (LIN_PROTOCOL == PROTOCOL_J2602)
      case SERVICE_TARGET_RESET:     
        (*lin_tl_pdu)[1] = 0x06;
        
        if (NEGATIVE == res_type){
           (*lin_tl_pdu)[2] = RES_NEGATIVE;
        }else{
          
           (*lin_tl_pdu)[2] = RES_POSITIVE+ sid;  
        }
        
        ident = (lin_product_id*)&node_conf->product_id;
        (*lin_tl_pdu)[3] = (l_u8)(ident->supplier_id & 0xFF);
        (*lin_tl_pdu)[4] = (l_u8)(ident->supplier_id >>8);
        (*lin_tl_pdu)[5] = (l_u8)(ident->function_id & 0xFF);
        (*lin_tl_pdu)[6] = (l_u8)(ident->function_id >>8);
        (*lin_tl_pdu)[7] = ident->variant;
      break;
#endif /* End (LIN_PROTOCOL == PROTOCOL_J2602) */

#if (LIN_PROTOCOL == PROTOCOL_21)      
      case SERVICE_READ_BY_IDENTIFY:  
        if (NEGATIVE == res_type){
          (*lin_tl_pdu)[1] = 0x03;
          (*lin_tl_pdu)[2] = RES_NEGATIVE;
          (*lin_tl_pdu)[3] = sid;
          (*lin_tl_pdu)[4] = error_code;
          (*lin_tl_pdu)[5] = 0xFF;
          (*lin_tl_pdu)[6] = 0xFF;
          (*lin_tl_pdu)[7] = 0xFF;
        }else{
          (*lin_tl_pdu)[1] = PCI_RES_READ_BY_IDENTIFY;
          (*lin_tl_pdu)[2] = RES_POSITIVE + sid;
          
            if (error_code == LIN_PRODUCT_IDENT){
                /* Get Identifier infor */
                ident = (lin_product_id*)&node_conf->product_id;
                (*lin_tl_pdu)[3] = (l_u8)(ident->supplier_id & 0xFF);
                (*lin_tl_pdu)[4] = (l_u8)(ident->supplier_id >>8);
                (*lin_tl_pdu)[5] = (l_u8)(ident->function_id & 0xFF);
                (*lin_tl_pdu)[6] = (l_u8)(ident->function_id >>8);
                (*lin_tl_pdu)[7] = ident->variant;
            }else if(error_code >= LIN_READ_USR_DEF_MIN && error_code <= LIN_READ_USR_DEF_MAX){
                l_u8 data_callout[5];
                l_u8 retval = ld_read_by_id_callout(iii, error_code, data_callout);
                if(retval == LD_POSITIVE_RESPONSE){
                    /* packing user defined pdu */
                    (*lin_tl_pdu)[3] = data_callout[0];
                    (*lin_tl_pdu)[4] = data_callout[1];
                    (*lin_tl_pdu)[5] = data_callout[2];
                    (*lin_tl_pdu)[6] = data_callout[3];
                    (*lin_tl_pdu)[7] = data_callout[4];
                }
            }
        }
      break;
      
      case SERVICE_ASSIGN_FRAME_ID_RANGE:  /* Mandatory for TL LIN 2.1 */
         if (POSITIVE == res_type) {
            (*lin_tl_pdu)[1] = PCI_RES_ASSIGN_FRAME_ID_RANGE;
            (*lin_tl_pdu)[2] = RES_POSITIVE + sid; 
            (*lin_tl_pdu)[3] = 0xFF;
            (*lin_tl_pdu)[4] = 0xFF; 
          }
      break;
      
      case SERVICE_SAVE_CONFIGURATION:  
      if (POSITIVE == res_type){
        /* PCI type */
        (*lin_tl_pdu)[1] = PCI_RES_SAVE_CONFIGURATION;
        /* SID */
        (*lin_tl_pdu)[2] = RES_POSITIVE + sid;
        /* Data unused */
        (*lin_tl_pdu)[3] = 0xFF;
        (*lin_tl_pdu)[4] = 0xFF;
      }
      break;
      
      
      
#endif /* End (LIN_PROTOCOL == PROTOCOL_21) */
      default:
        break;
   } /* end of switch statement */
/* multi frame support */      
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
   tl_conf->tl_tx_queue->queue_status = LD_QUEUE_FULL;
   tl_conf->tl_tx_queue->queue_current_size = 1;
   /* Put to transmit queue */
   for (i=0; i<8; i++){
     tl_conf->tl_tx_queue->tl_pdu[0][i] = (*lin_tl_pdu)[i];
   }
   tl_conf->tl_tx_msg_size = 1;
#else /* single frame support */
   /* set current TX PDU to send out */      
    tl_conf->tl_current_tx_pdu_ptr = lin_tl_pdu;
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
 
  /* set number of SlaveResp response data */
  tl_conf->tl_slaveresp_cnt = 1;
}


void lin_tl_get_pdu  
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
  l_u8 i;
  const lin_configuration *conf;
  lin_tl_descriptor *tl_conf;
  lin_tl_pdu_data lin_tl_pdu_buff;
  lin_tl_pdu_data *lin_tl_pdu = (lin_tl_pdu_data *)&lin_tl_pdu_buff;
       
  /* Get current configuration */  
  conf = &lin_ifc_configuration[iii];
  
  /* Get TL configuration */
  tl_conf = conf->tl_desc;    
/* multi frame support */      
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
  /* Get data from tx queue */
  tl_get_raw(*lin_tl_pdu,tl_conf->tl_tx_queue,TRANSMISSION);
#else /* single frame support */
   /* Get current PDU poiter of TX buffer */
  lin_tl_pdu = tl_conf->tl_current_tx_pdu_ptr;
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */    
    
  /* Copy PDU to response buffer */  
  for (i=1; i<9; i++) {
    conf->response_buffer[i] = (*lin_tl_pdu)[i - 1];
  }
}


void lin_tl_handler
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
  const lin_configuration * conf;
  lin_tl_descriptor *tl_conf;
  lin_tl_pdu_data *lin_tl_pdu;
  lin_node_attribute *node_conf;
  l_u8 pci_type;

/* single frame support */      
#if (_TL_FRAME_SUPPORT_ == _TL_SINGLE_FRAME_)
  l_u8 length;
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
  
  /* Get the current configuration */
  conf = (lin_configuration *)&lin_ifc_configuration[iii];
  
  /* Get TL configuration */
  tl_conf = conf->tl_desc;
   
  /* Get the node attribute */
  node_conf = (lin_node_attribute *)conf->node_attribute; 
  
/* multi frame support */      
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
  lin_tl_pdu= (lin_tl_pdu_data *)&(conf->response_buffer[1]);
  pci_type = ((*lin_tl_pdu)[1] & 0xF0)>>4;
#else /* single frame support */
  lin_tl_pdu = tl_conf->tl_current_rx_pdu_ptr;
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */    
  /* check NAD whether or not belongs to this slave node */
  if (_SLAVE_ == conf->function){
    if ((LD_BROADCAST != (*lin_tl_pdu)[0]) &&
        (LD_FUNCTIONAL_NAD != (*lin_tl_pdu)[0]) &&
         (((node_conf->configured_NAD != (*lin_tl_pdu)[0]) && (SERVICE_ASSIGN_NAD != (*lin_tl_pdu)[2]))
                || ((node_conf->initial_NAD != (*lin_tl_pdu)[0]) && (SERVICE_ASSIGN_NAD == (*lin_tl_pdu)[2])))
    ){
      tl_conf->tl_slaveresp_cnt = 0;
      return;
    }
  }
/* ingore Functional request */
if((*lin_tl_pdu)[0] == LD_FUNCTIONAL_NAD){
  return;
}
/* multi frame support */      
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
  /* check error */
  if(*conf->tl_service_status == LD_SERVICE_ERROR && pci_type == PCI_CF)
    return;
  /* process received PDU */
  lin_process_pdu(iii, lin_tl_pdu);
#else /* single frame support */
  /* Get PCI type */
  pci_type = ((*lin_tl_pdu)[1] & 0xF0)>>4;
  switch (pci_type) 
  {
    case PCI_SF: /* Single frame */
      /* error handling: check length of message */
      length = (*lin_tl_pdu)[1] & 0x0F;
      if (6 < length){
        return;
      }
      if (_SLAVE_ == conf->function){
        lin_tl_attach_service(iii);
      }else{
        lin_tl_get_response(iii);
      }
      break;
    case PCI_FF: /* First frame */
      break;
    case PCI_CF: /* Consecutive frame */
      break;
    default:
      break;
  } /* end of ckeck PCI_type */
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */  
}


void lin_tl_attach_service
  (
     /* [IN] interface name */
    l_ifc_handle iii
  ) 
{
  const lin_configuration* conf;
  lin_tl_descriptor *tl_conf;
  l_u8 sid;
  	  
 /* multi frame support */      
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
  l_u8 frame_type;
  lin_transport_layer_queue *rx_queue, *tx_queue;
  l_u8              *service_flag;
  l_u8              NAD;
  
  /* Get current configuration */
  conf = &lin_ifc_configuration[iii];
  /* Get TL description */
  tl_conf = conf->tl_desc;
  service_flag = tl_conf->tl_service_flags;
  rx_queue = tl_conf->tl_rx_queue;
  tx_queue = tl_conf->tl_tx_queue;
  NAD = rx_queue->tl_pdu[rx_queue->queue_header][0];
  /* check if request is functional request */
  if(NAD == LD_FUNCTIONAL_NAD){
    /* reset receive queue */
    rx_queue->queue_current_size = 0;
    rx_queue->queue_header = 0;
    rx_queue->queue_tail = 0;
    rx_queue->queue_status = LD_NO_DATA;
    /* update service state */
    *conf->tl_diag_state = LD_DIAG_IDLE;
    *conf->tl_service_status = LD_SERVICE_IDLE;
    /* ignore response */
    tl_conf->tl_no_of_pdu = 0;
    tl_conf->tl_frame_counter = 0;
    return;
  }
  /* prepare tx queue */
  tx_queue->queue_current_size = 0;
  tx_queue->queue_header = 0;
  tx_queue->queue_tail = 0;
  tx_queue->queue_status = LD_QUEUE_EMPTY;
  /* Check whether RX Queue contains data */
  if((LD_DATA_AVAILABLE == ld_raw_rx_status(iii))){
    /* get frame type */  
    frame_type = (rx_queue->tl_pdu[rx_queue->queue_header][1] & 0xF0)>>4;
    /* Get SID information  */
    if(frame_type == PCI_FF)
      sid = rx_queue->tl_pdu[rx_queue->queue_header][3];
    else
      sid = rx_queue->tl_pdu[rx_queue->queue_header][2];
    
    /* Check whether or not the Service is supported by the Slave node */
    switch (sid)
    {
    #if LIN_PROTOCOL == PROTOCOL_21
      case SERVICE_ASSIGN_FRAME_ID_RANGE:                  /* Mandatory for TL LIN 2.1 */
        lin_diagservice_assign_frame_id_range(iii);   
        break;

      case SERVICE_READ_BY_IDENTIFY:                       /* Mandatory for TL LIN 2.1 & 2.0, Reject by J2602 */
        lin_diagservice_read_by_identifier(iii);             /* Mandatory LIN TL 2.1 */
        break;
        
      case SERVICE_SAVE_CONFIGURATION:
		/* Set save configuration flag */
		if (_MASTER_ == conf->function)
		    lin_save_configuration_flg[iii] = 0;
		else
		    lin_save_configuration_flg[iii] = 1;
		/* Response to master - RSID */
		lin_tl_make_slaveres_pdu(iii, SERVICE_SAVE_CONFIGURATION, POSITIVE, 0);
	    break;
	  
    #endif /* (End LIN_PROTOCOL == LIN_PROTOCOL_21) */ 

    #if LIN_PROTOCOL == PROTOCOL_J2602
      case SERVICE_TARGET_RESET:
        if (LIN_PROTOCOL_J2602 == conf->lin_protocol_version){
          lin_diagservice_target_reset(iii);
        }
        break;
    #endif /* (End LIN_PROTOCOL == LIN_PROTOCOL_J2602) */
    
/* check diagnostic service is belong to class II or class III */
#if (_DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_II_ || _DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_III_)
      /* set diagnostic service flag */
      case SERVICE_READ_DATA_BY_IDENTIFY:
        service_flag[DIAGSRV_READ_DATA_BY_IDENTIFIER_ORDER]=1;
        break;
      case SERVICE_WRITE_DATA_BY_IDENTIFY:
        service_flag[DIAGSRV_WRITE_DATA_BY_IDENTIFIER_ORDER] = 1;
        break;
#if (_DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_III_)
      case SERVICE_SESSION_CONTROL:
        service_flag[DIAGSRV_SESSION_CONTROL_ORDER] = 1;
        break;
      case SERVICE_IO_CONTROL_BY_IDENTIFY:
        service_flag[DIAGSRV_IO_CONTROL_BY_IDENTIFIER_ORDER]=1;
        break;
      case SERVICE_FAULT_MEMORY_READ:
        service_flag[DIAGSRV_FAULT_MEMORY_READ_ORDER] = 1;
        break;
      case SERIVCE_FAULT_MEMORY_CLEAR:
        service_flag[DIAGSRV_FAULT_MEMORY_CLEAR_ORDER] = 1;
        break;
#endif /* End (_DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_III_) */
#endif /* End (_DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_II_ || _DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_III_) */

      default:
        lin_tl_make_slaveres_pdu(iii, sid, NEGATIVE, SERVICE_NOT_SUPPORTED);
        /* clear queue */
        tl_conf->tl_receive_msg_status = LD_NO_MSG;
        tl_conf->tl_rx_msg_status = LD_COMPLETED;
        rx_queue->queue_status = LD_NO_DATA;
        rx_queue->queue_current_size = 0;
        rx_queue->queue_header = rx_queue->queue_tail;
        break;
    } /* end of switch */
  }
  
#else /* single frame support */
  lin_tl_pdu_data *lin_tl_pdu;
  l_u8            NAD;
  /* Get current configuration */
  conf = &lin_ifc_configuration[iii];
  /* Get TL description */
  tl_conf = conf->tl_desc;
  /* Get current PDU pointer of RX buffer */
  lin_tl_pdu = tl_conf->tl_current_rx_pdu_ptr;
  /* Get SCI information  */
  sid = (*lin_tl_pdu)[2];
  NAD = (*lin_tl_pdu)[0];
  if(NAD == LD_FUNCTIONAL_NAD){
    
    return;
  }
  /* Check whether or not the Service is supported by the Slave node */
   switch (sid)
  {
#if LIN_PROTOCOL == PROTOCOL_21
    case SERVICE_ASSIGN_FRAME_ID_RANGE:                  /* Mandatory for TL LIN 2.1 */
      lin_diagservice_assign_frame_id_range(iii);   
      break;
    case SERVICE_READ_BY_IDENTIFY:                       /* Mandatory for TL LIN 2.1 & 2.0, Reject by J2602 */
      lin_diagservice_read_by_identifier(iii);             /* Mandatory LIN TL 2.1 */
      break;
      
    case SERVICE_SAVE_CONFIGURATION:
	  /* Set save configuration flag */
	  if (_MASTER_ == conf->function)
	     lin_save_configuration_flg[iii] = 0;
	  else
	     lin_save_configuration_flg[iii] = 1;
	  /* Response to master - RSID */
  	  lin_tl_make_slaveres_pdu(iii, SERVICE_SAVE_CONFIGURATION, POSITIVE, 0);
	  break;
	    
#endif /* (End LIN_PROTOCOL == LIN_PROTOCOL_21) */ 

#if LIN_PROTOCOL == PROTOCOL_J2602
    case SERVICE_TARGET_RESET:
      if (LIN_PROTOCOL_J2602 == conf->lin_protocol_version){
        lin_diagservice_target_reset(iii);
      }
      break;
#endif /* (End LIN_PROTOCOL == LIN_PROTOCOL_J2602) */
    default:
      lin_tl_make_slaveres_pdu(iii, sid, NEGATIVE, SERVICE_NOT_SUPPORTED);
      break;
  } /* end of switch */
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
 
}

void lin_tl_get_response
  (
    /* [IN] interface name */
    l_ifc_handle iii
  ) 
{
  const lin_configuration* conf;
  lin_tl_descriptor *tl_conf;
  lin_tl_pdu_data *lin_tl_pdu;
  l_u8 sci=0;
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
  lin_transport_layer_queue *rx_queue;
  l_u8              *service_flag, frame_type;
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
  /* Get current configuration */
  conf = &lin_ifc_configuration[iii];
 
  /* Get TL description */
  tl_conf = conf->tl_desc;
/* multi frame support */      
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
  service_flag = tl_conf->tl_service_flags;
  /* Get current PDU pointer of RX buffer */
  rx_queue = tl_conf->tl_rx_queue;
  lin_tl_pdu = (lin_tl_pdu_data *)(tl_conf->tl_rx_queue->tl_pdu[rx_queue->queue_header]);
  /* get frame type */
  frame_type = ((*lin_tl_pdu)[1] & 0xF0) >> 4;
  /* Set configuration service status */
  *conf->tl_service_status = LD_SERVICE_IDLE; 
  if(frame_type==SF)
    sci = (*lin_tl_pdu)[2];
  else if(frame_type==FF)
    sci = (*lin_tl_pdu)[3];  
#else /* single frame support */
  /* Get current PDU pointer of RX buffer */
  lin_tl_pdu = tl_conf->tl_current_rx_pdu_ptr;
  tl_conf->tl_service_status = LD_SERVICE_IDLE;   
  sci = (*lin_tl_pdu)[2];
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */ 
  /* Update last RSID */
  tl_conf->tl_last_RSID = sci;
  /* Get SID information  */
  
  if (RES_NEGATIVE == sci){
      /* set response configuration status to negative response */
      tl_conf->tl_last_cfg_result = LD_NEGATIVE;
      /* update error code */
      tl_conf->tl_ld_error_code = (*lin_tl_pdu)[4];
      
      /* switch to normal schedule table */
      *conf->active_schedule_id = *conf->previous_schedule_id;
      /* Clear queue */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)  
      tl_conf->tl_receive_msg_status = LD_NO_MSG;
      tl_conf->tl_rx_msg_status = LD_COMPLETED;
      rx_queue->queue_status = LD_NO_DATA;
      rx_queue->queue_current_size = 0;
      rx_queue->queue_header = rx_queue->queue_tail;
      *conf->tl_service_status = LD_SERVICE_IDLE;
#else
      tl_conf->tl_service_status = LD_SERVICE_IDLE;
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */  
      return;
  }
  sci = sci - 0x40;
  
 /* Check the result of service */
  switch (sci)
  {
    case SERVICE_READ_BY_IDENTIFY:
      (tl_conf->tl_ident_data)[0] = (*lin_tl_pdu)[4];
      (tl_conf->tl_ident_data)[1] = (*lin_tl_pdu)[3];
      (tl_conf->tl_ident_data)[2] = (*lin_tl_pdu)[6];
      (tl_conf->tl_ident_data)[3] = (*lin_tl_pdu)[5]; 
      (tl_conf->tl_ident_data)[4] = (*lin_tl_pdu)[7]; 
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)  
      tl_conf->tl_receive_msg_status = LD_NO_MSG;
      tl_conf->tl_rx_msg_status = LD_COMPLETED;
      rx_queue->queue_status = LD_NO_DATA;
      rx_queue->queue_current_size = 0;
      rx_queue->queue_header = rx_queue->queue_tail;
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */    
      break;
    case SERVICE_TARGET_RESET:    
    case SERVICE_ASSIGN_FRAME_ID:
    case SERVICE_ASSIGN_FRAME_ID_RANGE:
    case SERVICE_ASSIGN_NAD:
    case SERVICE_CONDITIONAL_CHANGE_NAD:
    case SERVICE_SAVE_CONFIGURATION:

#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)  
      tl_conf->tl_receive_msg_status = LD_NO_MSG;
      tl_conf->tl_rx_msg_status = LD_COMPLETED;
      rx_queue->queue_status = LD_NO_DATA;
      rx_queue->queue_current_size = 0;
      rx_queue->queue_header = rx_queue->queue_tail;
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
      break; 
/* check diagnostic service is belong to class II or class III */
#if (_DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_II_ || _DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_III_)
    /* set diagnostic service flag */
    case SERVICE_READ_DATA_BY_IDENTIFY:
      service_flag[DIAGSRV_READ_DATA_BY_IDENTIFIER_ORDER]=1;
      break;
    case SERVICE_WRITE_DATA_BY_IDENTIFY:
      service_flag[DIAGSRV_WRITE_DATA_BY_IDENTIFIER_ORDER] = 1;
      break;
	#if (_DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_III_)
    case SERVICE_SESSION_CONTROL:
      service_flag[DIAGSRV_SESSION_CONTROL_ORDER] = 1;
      break;
    case SERVICE_IO_CONTROL_BY_IDENTIFY:
      service_flag[DIAGSRV_IO_CONTROL_BY_IDENTIFIER_ORDER]=1;
      break;
    case SERVICE_FAULT_MEMORY_READ:
      service_flag[DIAGSRV_FAULT_MEMORY_READ_ORDER] = 1;
      break;
    case SERIVCE_FAULT_MEMORY_CLEAR:
      service_flag[DIAGSRV_FAULT_MEMORY_CLEAR_ORDER] = 1;
      break;
	#endif /* End (_DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_III_) */
#endif /* End (_DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_II_ || _DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_III_) */
    default:
    break;  
  }
  /* Set the last configuration result */
  tl_conf->tl_last_cfg_result = LD_SUCCESS;
  
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)  
      *conf->tl_service_status = LD_SERVICE_IDLE;
#else
      tl_conf->tl_service_status = LD_SERVICE_IDLE;
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */  
  /* switch to normal schedule table */
  *conf->active_schedule_id = *conf->previous_schedule_id;
}


/* single frame support */      
#if (_TL_FRAME_SUPPORT_ == _TL_SINGLE_FRAME_)

void lin_tl_update_tx_flag
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
  const lin_configuration *conf;     
  lin_tl_descriptor *tl_conf;
  /* Get current configuration */  
  conf = &lin_ifc_configuration[iii];
  
  /* Get TL configuration */
  tl_conf = conf->tl_desc;
  
  /* Check if master node */
  if(_MASTER_ == conf->function) {
    /* Update TX : Master -> Slave */
    /* Master request */
    if(0x3C == *(conf->current_pid)) {
      tl_conf->tl_cnt_to_send--;   
      /* Set state of service is LD_REQUEST_FINISHED */
      tl_conf->tl_service_status = LD_REQUEST_FINISHED;
    }                                        
  }
  /* Slave node */
  else { 
    /* Update TX SlaveResponse Frame: Slave->Master */
      
  }
}

void lin_tl_put_pdu
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
  l_u8 i;
  const lin_configuration *conf;
  lin_tl_descriptor *tl_conf;
  lin_tl_pdu_data *lin_tl_pdu;
       
  /* Get current configuration */  
  conf = &lin_ifc_configuration[iii];
  
  /* Get TL configuration */
  tl_conf = conf->tl_desc;
  
  /* Set PDU for RX buffer */
  tl_conf->tl_current_rx_pdu_ptr = (lin_tl_pdu_data *)tl_conf->tl_rx_single_pdu;
    
  /* Get current PDU poiter of RX buffer */
  lin_tl_pdu = tl_conf->tl_current_rx_pdu_ptr;
    
  for (i=0; i<8; i++) {
    (*lin_tl_pdu)[i] = conf->response_buffer[i+1];
  }
}


void lin_tl_no_response
  (
    /* [IN] interface name */
    l_ifc_handle iii
  ) 
{
  const lin_configuration* conf;
 
  /* Get current configuration */
  conf = &lin_ifc_configuration[iii];
  
  /* Clear response data */
  conf->tl_desc->tl_slaveresp_cnt = 0;
  /* Set configuration service status */
  conf->tl_desc->tl_service_status = LD_SERVICE_ERROR;
  /* Set the last configuration result */
  conf->tl_desc->tl_last_cfg_result = LD_NO_RESPONSE;   
}
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_SINGLE_FRAME_) */


#endif /* End (_LIN_MODE_ == _MASTER_MODE_) */

/*------------- End of code for Multi Interface ---------------*/
/* multi frame support */      
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
/* COMMON FUNCTIONS */
void tl_put_raw
  (
    /* [IN] buffer for the data */
    const l_u8* const data,
    /* [IN] queue for containing data */
    lin_transport_layer_queue *queue,
    /* [IN] Direction */
    l_u8 direction
  )
{
  lin_tl_pdu_data           *queue_data;
  l_u8                      i;

  /* Get transmit queue */
  queue_data = queue->tl_pdu;

  /* check status of queue */
  if (LD_QUEUE_FULL != queue->queue_status){
    /* prcess queue tail */
    if ((LD_QUEUE_EMPTY == queue->queue_status)|| (LD_NO_DATA == queue->queue_status)){
      /* Set status of queue */
       if (TRANSMISSION == direction)
        queue->queue_status = LD_QUEUE_AVAILABLE;
       else
       queue->queue_status = LD_DATA_AVAILABLE;
    } else {
    /* Check if queue size equal to queue tail */
      if (queue->queue_max_size == queue->queue_tail + 1){
        queue->queue_tail = 0;
      } else {
        queue->queue_tail++;
      }
    }
    /* copy data to queue */
    for (i=0; i<8; i++){
      queue_data[queue->queue_tail][i] = data[i];
    }
    /* update queue status */
    queue->queue_current_size++;
    if (queue->queue_current_size >= queue->queue_max_size){
        queue->queue_status = LD_QUEUE_FULL;
    }
  }
}
 
void tl_get_raw
  (
    /* [OUT] buffer for store data received */
    l_u8* const data,
    /* queue for containing data */
    lin_transport_layer_queue *queue,
    /* direction for transmission */
    l_u8 direction
    )
{

  lin_transport_layer_queue *tl_queue;
  lin_tl_pdu_data           *queue_data;
  l_u8                      i;
  l_u16                      queue_header;

  tl_queue = queue;
  /* Get queue */
  queue_data = tl_queue->tl_pdu;

  /* Check queue status (is the queue contains data? if yes: get data from queue and put them to buffer)*/
  if ((LD_NO_DATA != tl_queue->queue_status)&&(LD_QUEUE_EMPTY != tl_queue->queue_status)){
    /* get data from head queue */
    queue_header = tl_queue->queue_header;
    for (i=0; i<8; i++){
      *(data+i) = queue_data[queue_header][i];
    }

    /* update queue status */
    if (0 == (--queue->queue_current_size)){
      if (TRANSMISSION==direction){
        queue->queue_status = LD_QUEUE_EMPTY;
      }else{
        queue->queue_status = LD_NO_DATA;
      }
    } else {
      if (TRANSMISSION==direction){
        queue->queue_status = LD_QUEUE_AVAILABLE;
      }else{
        queue->queue_status = LD_DATA_AVAILABLE;
      }
    }

    /* change queue header */
    if(0 != queue->queue_current_size){
      if (queue->queue_header == queue->queue_max_size - 1){
        queue->queue_header = 0;
      } else {
        queue->queue_header++;
      }
    }
  }
}
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
/**
* @}
*/
