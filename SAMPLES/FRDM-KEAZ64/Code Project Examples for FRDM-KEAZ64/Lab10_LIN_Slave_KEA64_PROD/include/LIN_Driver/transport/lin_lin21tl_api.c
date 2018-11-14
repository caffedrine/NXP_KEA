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
 * @file      lin_lin21tl_api.c
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
 * 20090409		v1.0	First version
 *
 *****************************************************************************/
 
#include "LIN_Driver/include/lin_lin21tl_api.h"

#if LIN_MODE == _MASTER_MODE_
#if LIN_PROTOCOL == PROTOCOL_21


/** @} */

/** @addtogroup node_configuration_group
 * @{ */
l_u8 ld_is_ready(
/* [IN] interface name */
l_ifc_handle iii) {
	/* multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
	return (l_u8) (*lin_ifc_configuration[iii].tl_service_status);
#else /* single frame support */
	lin_configuration const *conf;
	/* Get current configuration*/
	conf = &lin_ifc_configuration[iii];
	return (l_u8)(conf->tl_desc->tl_service_status);
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
}

void ld_check_response(
/* [IN] interface name */
l_ifc_handle iii,
/* [OUT] buffer for saving the response ID */
l_u8* const RSID,
/* [OUT] buffer for saving the error code */
l_u8* const error_code) {
	lin_configuration const *conf;
	lin_tl_descriptor *_tl_desc;

	/* Get the current configuration */
	conf = &lin_ifc_configuration[iii];
	/* Get TL configuration */
	_tl_desc = conf->tl_desc;
	/* Get last reponse service identifier */
	*RSID = _tl_desc->tl_last_RSID;
	/* Get the error code of the last config sercive if it is negative response */
	if (LD_NEGATIVE == (_tl_desc->tl_last_cfg_result)) {
		*error_code = _tl_desc->tl_ld_error_code;
	}
}

void ld_assign_frame_id_range(
/* [IN] interface name */
l_ifc_handle iii,
/* [IN] NAD value of the target node */
l_u8 NAD,
/* [IN] specifies which is the first frame to assign a PID */
l_u8 start_index,
/* [IN] list of protected identifier */
const l_u8* const PIDs) {
	/* multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
	const lin_configuration * conf;
	l_u8 Buff[6];

	/* Get the current configuration */
	conf = &lin_ifc_configuration[iii];

	if (*conf->tl_service_status != LD_SERVICE_BUSY) {
		/* pack data into a single frame */
		Buff[0] = 0xB7;
		Buff[1] = start_index;
		Buff[2] = PIDs[0];
		Buff[3] = PIDs[1];
		Buff[4] = PIDs[2];
		Buff[5] = PIDs[3];

		/* put data into TX_QUEUE */
		ld_send_message(iii, 6, NAD, Buff);

		/* set node config status to busy */
		*conf->tl_service_status = LD_SERVICE_BUSY;
	}
#else /* single frame support */
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
		(*lin_tl_pdu)[0] = NAD;
		(*lin_tl_pdu)[1] = 0x06;
		(*lin_tl_pdu)[2] = 0xB7;
		(*lin_tl_pdu)[3] = start_index;
		(*lin_tl_pdu)[4] = PIDs[0];
		(*lin_tl_pdu)[5] = PIDs[1];
		(*lin_tl_pdu)[6] = PIDs[2];
		(*lin_tl_pdu)[7] = PIDs[3];

		/* Set state of service is BUSY */
		tl_conf->tl_service_status = LD_SERVICE_BUSY;
		/* Notify to Master task sending frame */
		tl_conf->tl_cnt_to_send = 1;

		/* Switch to master request schedule table */
		*conf->previous_schedule_id = *conf->active_schedule_id;
		*conf->active_schedule_id = conf->schedule_start + 2;
		conf->schedule_start_entry[*conf->active_schedule_id] = 0;

		/* End of switch to diagnostic schedule table */
	} /* End of checking service status */
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
} /* End of checking service status */
/** @} */

/** @addtogroup lin21tl_api_group
 * @{ */
/* multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
l_u8 diag_get_flag(
  /* [IN] interface name*/
  l_ifc_handle iii,
  /* [IN] flag order*/
  l_u8 flag_order){
  lin_configuration *conf;
	lin_tl_descriptor *tl_conf;	
  l_u8              *service_flag;
	
	/* Get current configuration */
	conf = (lin_configuration *) &lin_ifc_configuration[iii];
	/* Get TL configuration */
	tl_conf = conf->tl_desc;
  service_flag = tl_conf->tl_service_flags; 
  
  if(flag_order >= _DIAG_NUMBER_OF_SERVICES_){
    return 0;
  }else{
    return (l_u8) service_flag[flag_order];
  }
  
}
 
void diag_clear_flag(
  /* [IN] interface name*/
  l_ifc_handle iii,
  /* [IN] flag order*/
  l_u8 flag_order){
  lin_configuration *conf;
	lin_tl_descriptor *tl_conf;	
  l_u8              *service_flag;
	
	/* Get current configuration */
	conf = (lin_configuration *) &lin_ifc_configuration[iii];
	/* Get TL configuration */
	tl_conf = conf->tl_desc;
  service_flag = tl_conf->tl_service_flags; 
  
  if(flag_order < _DIAG_NUMBER_OF_SERVICES_){
    service_flag[flag_order] = 0;
  }
}
/* diagnostic services class II */
void diag_read_data_by_identifier(l_ifc_handle iii, const l_u8 NAD,
		l_u8 * const data) {

	l_u8 Buff[3];
	const lin_configuration *conf;
	/* Get the current configuration */
	conf = &lin_ifc_configuration[iii];

	/* check whether service status is idle or not */
	if (*conf->tl_service_status != LD_SERVICE_BUSY) {
		/* check whether input data contains correct data */
		
		/* pack data */
		Buff[0] = 0x22;
		Buff[1] = data[0];
		Buff[2] = data[1];

		/* send message to transport layer */
		ld_send_message(iii, 3, NAD, Buff);

		/* set service status to busy */
		*conf->tl_service_status = LD_SERVICE_BUSY;
	}
}

void diag_write_data_by_identifier(l_ifc_handle iii, const l_u8 NAD,
		l_u16 data_length, l_u8 * const data) {
	l_u8 Buff[MAX_LENGTH_SERVICE];
	l_u8 i;
	l_u16 temp = 0;

	const lin_configuration * conf;
	/* Get the current configuration */
	conf = &lin_ifc_configuration[iii];

	/* Check if length of data is greater than length maximum */
	if (data_length < MAX_LENGTH_SERVICE) {
		/* check whether service status is idle or not */
		if (*conf->tl_service_status != LD_SERVICE_BUSY) {
			/* check whether input data contains correct data */
			temp = data[0];
			temp <<= 8;
			temp |= data[1];

			/* pack data */
			Buff[0] = 0x2E;

			if ((temp == 0x0092) || (temp == 0x0205)) {
				for (i = 0; i < data_length; i++) {
					Buff[i + 1] = data[i];
				}
				ld_send_message(iii, data_length + 1, NAD, Buff);
				/* set service status to busy */
				*conf->tl_service_status = LD_SERVICE_BUSY;
			}
		}
	}
}

/* Diagnostic services class III */
#if(_DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_III_)

/* ----------------- Session control -------------------- */
void diag_session_control (l_ifc_handle iii, const l_u8 NAD, l_u16 data_length, l_u8 *const data) {
	l_u8 Buff[MAX_LENGTH_SERVICE];
	l_u8 i;

	const lin_configuration * conf;
	/* Get the current configuration */
	conf = &lin_ifc_configuration[iii];

	/* Check if length of data is greater than length maximum */
	if(data_length < MAX_LENGTH_SERVICE) {
		/* check whether service status is idle or not */
		if(*conf->tl_service_status!=LD_SERVICE_BUSY) {
			/* pack data */
			Buff[0] = 0x10;

			for(i=0;i<data_length;i++) {
				Buff[i+1] = data[i];
			}
			ld_send_message(iii,data_length+1,NAD,Buff);
			/* set service status to busy */
			*conf->tl_service_status = LD_SERVICE_BUSY;
		}
	}
}

/* ----------------- Fault memory -------------------- */
void diag_fault_memory_read (l_ifc_handle iii, const l_u8 NAD, l_u16 data_length, l_u8 *const data) {
	l_u8 Buff[MAX_LENGTH_SERVICE];
	l_u8 i;
	l_u16 temp=0;

	const lin_configuration * conf;
	/* Get the current configuration */
	conf = &lin_ifc_configuration[iii];

	/* Check if length of data is greater than length maximum */
	if(data_length < MAX_LENGTH_SERVICE) {
		/* check whether service status is idle or not */
		if(*conf->tl_service_status!=LD_SERVICE_BUSY) {

			/* pack data */
			Buff[0] = 0x19;

			for(i=0;i<data_length;i++) {
				Buff[i+1] = data[i];
			}
			ld_send_message(iii,data_length+1,NAD,Buff);
			/* set service status to busy */
			*conf->tl_service_status = LD_SERVICE_BUSY;
		}
	}
}
void diag_fault_memory_clear (l_ifc_handle iii, const l_u8 NAD, l_u16 data_length, l_u8 *const data) {
	l_u8 Buff[MAX_LENGTH_SERVICE];
	l_u8 i;
	l_u16 temp=0;

	const lin_configuration * conf;
	/* Get the current configuration */
	conf = &lin_ifc_configuration[iii];

	/* Check if length of data is greater than length maximum */
	if(data_length < MAX_LENGTH_SERVICE) {
		/* check whether service status is idle or not */
		if(*conf->tl_service_status!=LD_SERVICE_BUSY) {

			/* pack data */
			Buff[0] = 0x14;

			for(i=0;i<data_length;i++) {
				Buff[i+1] = data[i];
			}
			ld_send_message(iii,data_length+1,NAD,Buff);
			/* set service status to busy */
			*conf->tl_service_status = LD_SERVICE_BUSY;
		}
	}
}
/* ----------------- I/O control -------------------- */
void diag_IO_control (l_ifc_handle iii,const l_u8 NAD,l_u16 data_length, l_u8 *const data) {
	l_u8 Buff[MAX_LENGTH_SERVICE];
	l_u8 i;
	l_u16 temp=0;

	const lin_configuration * conf;
	/* Get the current configuration */
	conf = &lin_ifc_configuration[iii];

	/* Check if length of data is greater than length maximum */
	if(data_length < MAX_LENGTH_SERVICE) {
		/* check whether service status is idle or not */
		if(*conf->tl_service_status!=LD_SERVICE_BUSY) {

			/* pack data */
			Buff[0] = 0x2F;

			for(i=0;i<data_length;i++) {
				Buff[i+1] = data[i];
			}
			ld_send_message(iii,data_length+1,NAD,Buff);
			/* set service status to busy */
			*conf->tl_service_status = LD_SERVICE_BUSY;
		}
	}
}

#endif /*End (_DIAG_CLASS_SUPPORT_ == _DIAG_CLASS_III_)*/

#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
#endif /* End of (LIN_PROTOCOL == PROTOCOL_21) */
#endif /* End LIN_MODE == _MASTER_MODE_ */ 
#if LIN_MODE == _SLAVE_MODE_
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
/* get diagnostic flag */
l_u8 diag_get_flag( 
  /* [IN] flag order*/
  l_u8 flag_order){  
  
  if(flag_order >= _DIAG_NUMBER_OF_SERVICES_){
    return 0;
  }else{
    return (l_u8) lin_diag_services_flag[flag_order];
  }
  
}
 
void diag_clear_flag( 
  /* [IN] flag order*/
  l_u8 flag_order){  
  
  if(flag_order < _DIAG_NUMBER_OF_SERVICES_){
    lin_diag_services_flag[flag_order] = 0;
  }
}
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)*/
#endif/* End LIN_MODE == _SLAVE_MODE_ */
/** @} */
