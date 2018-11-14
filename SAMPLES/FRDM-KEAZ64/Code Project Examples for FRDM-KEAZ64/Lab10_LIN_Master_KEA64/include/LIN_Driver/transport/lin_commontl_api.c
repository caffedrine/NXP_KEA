/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2008-2009 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 ******************************************************************************/
/**************************************************************************//**
 * @addtogroup commontl_api_group
 * @{
 ******************************************************************************/
/**************************************************************************//**
 *
 * @file      lin_commontl_api.c
 *
 * @author    FPT Software
 *
 * @brief     Common LIN transport layer and configuration functions
 *
 ******************************************************************************/
/******************************************************************************
 *
 * History:
 *
 * 20090409		v1.0	First version
 * 20111005		v1.1	Added 3 functions: ld_save_configuration, ld_read_configuration, ld_set_configuration
 *
 *****************************************************************************/
 
 #include "LIN_Driver/include/lin_commontl_api.h"
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
#include "lin_commontl_proto.h"
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
/** @} */
#if LIN_MODE == _MASTER_MODE_
/********------------- Code supports SINGLE interface ----------------**********/
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)

/* Multi timer selection  */
#ifdef MULTI_TIMER_MODE
extern const l_u16 max_tl_timeout_counter[LIN_NUM_OF_IFCS];
#endif /* End MULTI_TIMER_MODE */

/** @addtogroup initialization_group
 * @{ */
void ld_init(
/* [IN] interface name*/
l_ifc_handle iii) {
	static lin_configuration *conf;
	static lin_tl_descriptor *tl_conf;
	static lin_transport_layer_queue *tl_queue;

	/* Get current configuration */
	conf = (lin_configuration *) &lin_ifc_configuration[iii];
	/* Get TL configuration */
	tl_conf = conf->tl_desc;

	/* init transmit queue */
	/* Get TL tx queue*/
	tl_queue = tl_conf->tl_tx_queue;
	tl_queue->queue_header = 0;
	tl_queue->queue_tail = 0;
	tl_queue->queue_status = LD_QUEUE_EMPTY;
	tl_queue->queue_current_size = 0;
	/* init receive queue */
	/* Get TL rx queue*/
	tl_queue = tl_conf->tl_rx_queue;
	tl_queue->queue_header = 0;
	tl_queue->queue_tail = 0;
	tl_queue->queue_status = LD_NO_DATA;
	tl_queue->queue_current_size = 0;
	/* Init transmit message */
	tl_conf->tl_rx_msg_status = LD_COMPLETED;
	tl_conf->tl_rx_msg_index = 0;
	tl_conf->tl_rx_msg_size = 0;
	tl_conf->tl_receive_msg_status = LD_NO_MSG;

	/* Init receive message */
	tl_conf->tl_tx_msg_status = LD_COMPLETED;
	tl_conf->tl_tx_msg_index = 0;
	tl_conf->tl_tx_msg_size = 0;

	tl_conf->tl_last_cfg_result = LD_SUCCESS;
	tl_conf->tl_last_RSID = 0;
	tl_conf->tl_ld_error_code = 0;

	tl_conf->tl_frame_counter = 0;
	tl_conf->tl_no_of_pdu = 0;
	tl_conf->tl_slaveresp_cnt = 0;

	tl_conf->tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
#ifdef MULTI_TIMER_MODE
   tl_conf->tl_check_timeout = max_tl_timeout_counter[iii];
#else
	tl_conf->tl_check_timeout = N_MAX_TIMEOUT_CNT;
#endif /* End MULTI_TIMER_MODE */

	*conf->tl_diag_state = LD_DIAG_IDLE;
	*conf->tl_service_status = LD_SERVICE_IDLE;
	*conf->diagnostic_mode=DIAG_NONE;
	*conf->tl_diag_interleave_state = DIAG_NOT_START;
	tl_conf->tl_interleave_timeout_counter = 0;
}
/** @} */
/** @addtogroup raw_api_group
 * @{ */
void ld_put_raw(
/* [IN] interface name*/
l_ifc_handle iii,
/* [IN] buffer for the data to be transmitted */
const l_u8* const data) {
	lin_configuration *conf;
	lin_tl_descriptor *tl_conf;
	lin_transport_layer_queue *tl_queue;

	/* Get current configuration */
	conf = (lin_configuration *) &lin_ifc_configuration[iii];
	/* Get TL configuration */
	tl_conf = conf->tl_desc;
	/* Get transmit queue */
	tl_queue = tl_conf->tl_tx_queue;

	tl_put_raw(data, tl_queue, TRANSMISSION);

}

void ld_get_raw(
/* [IN] interface name*/
l_ifc_handle iii,
/* [OUT] buffer for store data received */
l_u8* const data) {
	lin_configuration *conf;
	lin_tl_descriptor *tl_conf;
	lin_transport_layer_queue *tl_queue;

	/* Get current configuration */
	conf = (lin_configuration *) &lin_ifc_configuration[iii];
	/* Get TL configuration */
	tl_conf = conf->tl_desc;
	/* Get transmit queue */
	tl_queue = tl_conf->tl_rx_queue;

	tl_get_raw(data, tl_queue, RECEIVING);
}

l_u8 ld_raw_tx_status(
/* [IN] interface name*/
l_ifc_handle iii) {
	lin_configuration *conf;
	lin_tl_descriptor *tl_conf;
	lin_transport_layer_queue *tl_queue;

	/* Get current configuration */
	conf = (lin_configuration *) &lin_ifc_configuration[iii];
	/* Get TL configuration */
	tl_conf = conf->tl_desc;
	/* Get transmit queue */
	tl_queue = tl_conf->tl_tx_queue;

	return (l_u8) (tl_queue->queue_status);
}

l_u8 ld_raw_rx_status(
/* [IN] interface name */
l_ifc_handle iii) {
	lin_configuration *conf;
	lin_tl_descriptor *tl_conf;
	lin_transport_layer_queue *tl_queue;

	/* Get current configuration */
	conf = (lin_configuration *) &lin_ifc_configuration[iii];
	/* Get TL configuration */
	tl_conf = conf->tl_desc;
	/* Get transmit queue */
	tl_queue = tl_conf->tl_rx_queue;

	return (l_u8) (tl_queue->queue_status);
}
/** @} */
/** @addtogroup cooked_api_group
 * @{ */
void ld_send_message(
/* [IN] interface name */
l_ifc_handle iii,
/* [IN] length of data to send */
l_u16 length,
/* [IN] Node address of slave node */
l_u8 NAD,
/* [IN] data to be sent */
const l_u8* const data) {
	lin_configuration *conf;
	lin_tl_descriptor *tl_conf;
	lin_transport_layer_queue *tl_queue;
	lin_tl_pdu_data pdu;
	l_u8 i;
	l_u8 message_size;
	l_u16 data_index = 0;
	l_u16 tmp_length = length;
	l_u16 frame_counter = 0;

	/* Get current configuration */
	conf = (lin_configuration *) &lin_ifc_configuration[iii];
	/* Get TL configuration */
	tl_conf = conf->tl_desc;
	/* Get transmit queue */
	tl_queue = tl_conf->tl_tx_queue;

	/* check message status in queue */
	if (LD_COMPLETED == tl_conf->tl_tx_msg_status) {
		/* calculate number of PDU for this message */
		if (length <= 6) {
			message_size = 1;
		} else {
			if ((length - 5) % 6 == 0)
				message_size = ((length - 5) / 6) + 1;
			else
				message_size = ((length - 5) / 6) + 2;
		}
		tl_conf->tl_slaveresp_cnt = message_size;
		if (message_size <= (tl_queue->queue_max_size - tl_queue->queue_current_size)) {
			/* update information of message in queue */
			tl_conf->tl_tx_msg_index = tl_queue->queue_tail;
			tl_conf->tl_tx_msg_size = message_size;
			tl_conf->tl_tx_msg_status = LD_IN_PROGRESS;
			*conf->tl_service_status = LD_SERVICE_BUSY;

			/* package data */
			if (length <= 6) {
				/* package single frame */
				/* ____________________________________________ */
				/* | NAD | PCI | SID | D1 | D2 | D3 | D4 | D5 | */
				/* |_____|_____|_____|____|____|____|____|____| */
				if (_MASTER_ == conf->function) {
					pdu[0] = NAD;
				} else {
					pdu[0] = conf->node_attribute->configured_NAD;
				}
				pdu[1] = (l_u8) length;
				pdu[2] = data[0]; /* SID / RSID */
				for (i = 1; i < length; i++) {
					pdu[i + 2] = data[i]; /* used data */
				}
				for (i = (l_u8) length; i < 6; i++) {
					pdu[i + 2] = 0xFF; /* unused data */
				}
				ld_put_raw(iii, pdu);
			} else {
				/* package first frame */
				/* ____________________________________________ */
				/* | NAD | PCI | LEN |SID | D2 | D3 | D4 | D5 | */
				/* |_____|_____|_____|____|____|____|____|____| */
				if (_MASTER_ == conf->function) {
					pdu[0] = NAD;
				} else {
					pdu[0] = conf->node_attribute->configured_NAD;
				}
				pdu[1] = ((length / 256) & 0x0F) | 0x10; /* PCI */
				pdu[2] = length % 256; /* length */
				pdu[3] = data[0]; /* SID / RSID */
				for (i = 1; i < 5; i++) { /* data */
					pdu[i + 3] = data[i];
				}
				data_index += 5;
				tmp_length -= 5;
				ld_put_raw(iii, pdu);

				/* package consecutive frame */
				/* ___________________________________________ */
				/* | NAD | PCI | D1 | D2 | D3 | D4 | D5 | D6 | */
				/* |_____|_____|____|____|____|____|____|____| */
				message_size--;
				if (_MASTER_ == conf->function) {
					pdu[0] = NAD;
				} else {
					pdu[0] = conf->node_attribute->configured_NAD;
				}
				while (message_size > 0) {
					frame_counter++;
					pdu[1] = 0x20 | (frame_counter & 0x0F);
					if (tmp_length < 6) { /* last PDU */
						/* used data */
						for (i = 0; i < tmp_length; i++) {
							pdu[i + 2] = data[data_index++];
						}
						/* unused data */
						for (i = (l_u8) tmp_length; i < 6; i++) {
							pdu[i + 2] = 0xFF;
						}
					} else {
						for (i = 2; i < 8; i++) {
							pdu[i] = data[data_index++];
						}
						tmp_length -= 6;
					} /* end of (tmp < 6 ) */
					message_size--;
					ld_put_raw(iii, pdu);
				} /* end of (message > 0) */
			} /* end of (length < 6) */
			if (_MASTER_ == conf->function) {
				*conf->previous_schedule_id = *conf->active_schedule_id;
  			*conf->active_schedule_id = conf->schedule_start + 2;
  			conf->schedule_start_entry[*conf->active_schedule_id] = 0;
  			/* Start send message */
  			*conf->diagnostic_mode = DIAG_INTER_LEAVE_MODE;
			}
		} /* end of check message size */
	} /* end of (LD_COMPLETED == tl_conf->tl_message_status) */
} /* end of ld_send_message */

void ld_receive_message(
		/* [IN] interface name */
		l_ifc_handle iii,
		/* [OUT] length of data to receive */
		l_u16* const length,
		/* [OUT] Node address of slave node */
		l_u8* const NAD,
		/* [OUT] data to be sent */
		l_u8* const data
) {
	lin_configuration *conf;
	lin_tl_descriptor *tl_conf;
	lin_message_status *_tl_receive_msg_status;
	lin_transport_layer_queue *tl_queue;
	lin_tl_pdu_data *queue_data;
	lin_tl_pdu_data pdu;
	l_u8 i;
	l_u16 data_index = 0;
	l_u16 tmp_length = 0;
	l_u16 frame_counter;
	l_u8 PCI_type;

	/* Get current configuration */
	conf = (lin_configuration *) &lin_ifc_configuration[iii];
	/* Get TL configuration */
	tl_conf = conf->tl_desc;
	/* Get message status */
	_tl_receive_msg_status = &(tl_conf->tl_receive_msg_status);
	/* Get transmit queue */
	tl_queue = tl_conf->tl_rx_queue;
	queue_data = tl_queue->tl_pdu;
	/* set status of receive message */
	tl_conf->tl_rx_msg_status = LD_IN_PROGRESS;

	/* wait message is received completely */
	while (LD_COMPLETED != *_tl_receive_msg_status) {
		/* check message error */
		if ((LD_FAILED == *_tl_receive_msg_status) || (LD_WRONG_SN
				== *_tl_receive_msg_status) || (LD_N_CR_TIMEOUT
				== *_tl_receive_msg_status)) {
			tl_conf->tl_rx_msg_status = *_tl_receive_msg_status;
			return;
		}
	}
	/* Message is received completely */
	/* get data from receive queue */
	ld_get_raw(iii, pdu);
	/* Analyze data */
	if (_MASTER_ == conf->function) {
		*NAD = pdu[0];
	}
	/* Check type of pdu */
	PCI_type = (pdu[1] & 0xF0) >> 4;
	switch (PCI_type) {
	/* Single frame */
	case SF:
		tmp_length = pdu[1] & 0x0f;
		*length = tmp_length;
		data[0] = pdu[2];
		for (i = 1; i < tmp_length; i++) {
			data[i] = pdu[i + 2];
		}
		break;
		/* First frame */
	case FF:
		tmp_length = (pdu[1] & 0x0F) * 256 + pdu[2];
		*length = tmp_length;
		data[0] = pdu[3];
		for (i = 1; i < 5; i++) {
			data[i] = pdu[i + 3];
		}
		tmp_length -= 5;
		data_index += 5;
		/* Consecutive frame */
	case CF:
		while (tmp_length > 6) {
			/* get PDU */
			ld_get_raw(iii, pdu);
			frame_counter = pdu[1] & 0x0F;

			for (i = 2; i < 8; i++) {
				data[data_index++] = pdu[i];
			}
			tmp_length -= 6;
		}

		/* Last frame */
		if (tmp_length > 0) {
			/* get PDU */
			ld_get_raw(iii, pdu);
			frame_counter = pdu[1] & 0x0F;

			for (i = 0; i < tmp_length; i++) {
				data[data_index++] = pdu[i + 2];
			}
		}

		break;
	default:
		break;
	} /* end of switch */
	 *_tl_receive_msg_status = LD_NO_MSG;
	tl_conf->tl_rx_msg_status = LD_COMPLETED;
}

l_u8 ld_tx_status(
/* [IN] interface name */
l_ifc_handle iii) {
	lin_configuration *conf;
	lin_tl_descriptor *tl_conf;

	/* Get current configuration */
	conf = (lin_configuration *) &lin_ifc_configuration[iii];
	/* Get TL configuration */
	tl_conf = conf->tl_desc;

	return (l_u8) (tl_conf->tl_tx_msg_status);
}

l_u8 ld_rx_status(
/* [IN] interface name */
l_ifc_handle iii) {
	lin_configuration *conf;
	lin_tl_descriptor *tl_conf;

	/* Get current configuration */
	conf = (lin_configuration *) &lin_ifc_configuration[iii];
	/* Get TL configuration */
	tl_conf = conf->tl_desc;

	return (l_u8) (tl_conf->tl_rx_msg_status);
}
/** @} */
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */

/** @addtogroup node_configuration_group
 * @{ */
void ld_assign_NAD(
/* [IN] interface name */
l_ifc_handle iii,
/* [IN] initial node address  of the target node */
l_u8 initial_NAD,
/* [IN] supplier ID of the target node */
l_u16 supplier_id,
/* [IN] function identifier of the target node */
l_u16 function_id,
/* [IN] new node address */
l_u8 new_NAD) {
	/* Multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
	l_u8 data[6];
	lin_configuration *conf;

	/* Get current configuration */
	conf = (lin_configuration *) &lin_ifc_configuration[iii];

	/* check service is busy? */
	if (*conf->tl_service_status != LD_SERVICE_BUSY) {
		data[0] = 0xB0;
		data[1] = supplier_id & 0x00FF;
		data[2] = (supplier_id >> 8) & 0x00FF;
		data[3] = function_id & 0x00FF;
		data[4] = (function_id >> 8) & 0x00FF;
		data[5] = new_NAD;

		/* put data into TX_QUEUE */
		ld_send_message(iii, 6, initial_NAD, data);

		/* set node config status to busy */
		*conf->tl_service_status = LD_SERVICE_BUSY;
	} /* End of checking service status */

#else /*Single frame support */

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

		/* Create data for Assign NAD command */
		(*lin_tl_pdu)[0] = initial_NAD;
		(*lin_tl_pdu)[1] = 0x06;
		(*lin_tl_pdu)[2] = 0xB0;
		(*lin_tl_pdu)[3] = supplier_id &0x00FF;
		(*lin_tl_pdu)[4] = (supplier_id >> 8) & 0x00FF;
		(*lin_tl_pdu)[5] = function_id &0x00FF;
		(*lin_tl_pdu)[6] = (function_id >> 8) & 0x00FF;
		(*lin_tl_pdu)[7] = new_NAD;

		/* Set state of service is BUSY */
		tl_conf->tl_service_status = LD_SERVICE_BUSY;
		/* Notify to Master task sending frame */
		tl_conf->tl_cnt_to_send = 1;
		/* Switch to master request schedule table */
		*conf->previous_schedule_id = *conf->active_schedule_id;
		*conf->active_schedule_id = conf->schedule_start + 2;
		conf->schedule_start_entry[*conf->active_schedule_id] = 0;
	} /* End of checking service status */
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
}

void ld_conditional_change_NAD(
/* [IN] interface name */
l_ifc_handle iii,
/* [IN] current NAD value of the target node */
l_u8 NAD,
/* [IN] property ID of the target node */
l_u8 id,
/* [IN] byte location of property value to be read from the target node */
l_u8 byte_data,
/* [IN] value for masking the read property byte */
l_u8 mask,
/* [IN] value for excluding the read property byte */
l_u8 invert,
/* [IN] new NAD value to be assigned when the condition is met */
l_u8 new_NAD) {
	/* Multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
	l_u8 data[6];
	lin_configuration *conf;

	/* Get current configuration */
	conf = (lin_configuration *) &lin_ifc_configuration[iii];

	/* check service is busy? */
	if (*conf->tl_service_status != LD_SERVICE_BUSY) {
		data[0] = 0xB3;
		data[1] = id;
		data[2] = byte_data;
		data[3] = mask;
		data[4] = invert;
		data[5] = new_NAD;

		/* put data into TX_QUEUE */
		ld_send_message(iii, 6, NAD, data);

		/* set node config status to busy */
		*conf->tl_service_status = LD_SERVICE_BUSY;
	}/* End of checking service status */

#else /*Single frame support */
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
		/* Create data for Conditional change NAD command */
		(*lin_tl_pdu)[0] = NAD;
		(*lin_tl_pdu)[1] = 0x06;
		(*lin_tl_pdu)[2] = 0xB3;
		(*lin_tl_pdu)[3] = id;
		(*lin_tl_pdu)[4] = byte_data;
		(*lin_tl_pdu)[5] = mask;
		(*lin_tl_pdu)[6] = invert;
		(*lin_tl_pdu)[7] = new_NAD;
		/* Set state of service is BUSY */
		tl_conf->tl_service_status = LD_SERVICE_BUSY;
		/* Notify to Master task sending frame */
		tl_conf->tl_cnt_to_send = 1;
		/* Switch to master request schedule table */
		*conf->previous_schedule_id = *conf->active_schedule_id;
		*conf->active_schedule_id = conf->schedule_start + 2;
		conf->schedule_start_entry[*conf->active_schedule_id] = 0;
	} /* End of checking service status */
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */  
}
/** @} */

/** @addtogroup node_identification_group
 * @{ */
void ld_read_by_id(
/* [IN] interface name */
l_ifc_handle iii,
/* [IN] value of the target node */
l_u8 NAD,
/* [IN] supplier ID of the target node */
l_u16 supplier_id,
/* [IN] function ID of the target node */
l_u16 function_id,
/* [IN] ID of the target node */
l_u8 id,
/* [OUT] buffer for saving the data read from the node */
l_u8* const data) {
	/* Multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)  
	l_u8 buff[6];

	lin_configuration *conf;
	lin_tl_descriptor *tl_conf;

	/* Get current configuration */
	conf = (lin_configuration *) &lin_ifc_configuration[iii];
	tl_conf = conf->tl_desc;

	/* check service is busy? */
	if (*conf->tl_service_status != LD_SERVICE_BUSY) {
		/* Create data for Read by Identifier command */
		buff[0] = 0xB2;
		buff[1] = id;
		buff[2] = supplier_id & 0x00FF;
		buff[3] = (supplier_id >> 8) & 0x00FF;
		buff[4] = function_id & 0x00FF;
		buff[5] = (function_id >> 8) & 0x00FF;

		/* Store address of RAM data which contain response infor */
		tl_conf->tl_ident_data = data;

		/* put data into TX_QUEUE */
		ld_send_message(iii, 6, NAD, buff);

		/* set node config status to busy */
		*conf->tl_service_status = LD_SERVICE_BUSY;

	} /* End of checking service status */
#else /* Single frame support */
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
		/* Create data for Read by Identifier command */
		(*lin_tl_pdu)[0] = NAD;
		(*lin_tl_pdu)[1] = 0x06;
		(*lin_tl_pdu)[2] = 0xB2;
		(*lin_tl_pdu)[3] = id;
		(*lin_tl_pdu)[4] = supplier_id &0x00FF;
		(*lin_tl_pdu)[5] = (supplier_id >> 8) & 0x00FF;
		(*lin_tl_pdu)[6] = function_id &0x00FF;
		(*lin_tl_pdu)[7] = (function_id >> 8) & 0x00FF;

		/* Store address of RAM data which contain response infor */
		tl_conf->tl_ident_data = data;
		/* Set state of service is BUSY */
		tl_conf->tl_service_status = LD_SERVICE_BUSY;
		/* Notify to Master task sending frame */
		tl_conf->tl_cnt_to_send = 1;

		/* Switch to master request schedule table */
		*conf->previous_schedule_id = *conf->active_schedule_id;
		*conf->active_schedule_id = conf->schedule_start + 2;
		conf->schedule_start_entry[*conf->active_schedule_id] = 0;
	} /* End of checking service status */
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
}



void ld_save_configuration(
/* [IN] interface name */
l_ifc_handle iii,
/* [IN] node address of the target node */
l_u8 NAD
) {
	/* Multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
	l_u8 data[6];
	lin_configuration *conf;

	/* Get current configuration */
	conf = (lin_configuration *) &lin_ifc_configuration[iii];

	/* check service is busy? */
	if (*conf->tl_service_status != LD_SERVICE_BUSY) {
		data[0] = SERVICE_SAVE_CONFIGURATION;
		data[1] = 0xFF;
		data[2] = 0xFF;
		data[3] = 0xFF;
		data[4] = 0xFF;
		data[5] = 0xFF;
		/* put data into TX_QUEUE */
		ld_send_message(iii, PCI_SAVE_CONFIGURATION, NAD, data);

		/* set node config status to busy */
		*conf->tl_service_status = LD_SERVICE_BUSY;
	} /* End of checking service status */

#else /* Single frame support */
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
		/* Create data for Assign NAD command */
		(*lin_tl_pdu)[0] = NAD;
		(*lin_tl_pdu)[1] = PCI_SAVE_CONFIGURATION;
		(*lin_tl_pdu)[2] = SERVICE_SAVE_CONFIGURATION;
		(*lin_tl_pdu)[3] = 0xFF;
		(*lin_tl_pdu)[4] = 0xFF;
		(*lin_tl_pdu)[5] = 0xFF;
		(*lin_tl_pdu)[6] = 0xFF;
		(*lin_tl_pdu)[7] = 0xFF;
		/* Set state of service is BUSY */
		tl_conf->tl_service_status = LD_SERVICE_BUSY;
		/* Notify to Master task sending frame */
		tl_conf->tl_cnt_to_send = 1;
		/* Switch to master request schedule table */
		*conf->previous_schedule_id = *conf->active_schedule_id;
		*conf->active_schedule_id = conf->schedule_start + 2;
		conf->schedule_start_entry[*conf->active_schedule_id] = 0;
	} /* End of checking service status */
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
}


l_u8 ld_read_configuration
(
	/* [IN] interface name */
	l_ifc_handle iii,
	/* [IN] data area to save configuration */ 
	l_u8* const data,
	/* [IN] length of data area */ 
	l_u8* const length
)
{
    l_u8 i, temp;
    /* Set the default returned value to LD_READ_OK */
    l_u8 retval = (l_u8)LD_READ_OK;
    /** Set the expected length value to 
     * EXP = NN + NF, where :
     * NN = the number of NAD.
     * NF = the number of configurable frames;
     * Moreover:
     * Not taken PID's diagnostics frame: 3C, 3D
     */
    l_u8 expected_length;
    
    lin_configuration const *conf;     
    lin_node_attribute *node_att;
    
    /* Get current configuration */  
    conf = &lin_ifc_configuration[iii];
    
    expected_length = (l_u8) (conf->num_of_frames - 1);
    /* Check if slave node */
    if(_SLAVE_ == conf->function) {
        temp = *length;
        /* Get node attibute */
        node_att = conf-> node_attribute;
    
        if (temp < expected_length)
        {
            /* The 'data' size is not enough to store NAD+PIDs */
            retval = (l_u8)LD_LENGTH_TOO_SHORT;
        }
        else 
        {
		    /* The 'data' size is enough to store NAD+PIDs, so proceed to store them */
		    /* Copy actual NAD to 'data' */
		    data[0] = node_att -> configured_NAD;
		
		    /* Copy protected IDs to 'data' */
		    for (i = 1U; i < expected_length; ++i)
		    {
			    data[i] = conf->configuration_RAM[i];
		    }
		    /* Set the length parameter to the actual size of the configuration */
		    *length = expected_length;
        }
    }
    return retval;
} /* End ld_read_configuration() */
  


l_u8 ld_set_configuration
(
	/* [IN] interface name */
	l_ifc_handle iii,
	/* [IN] data area to set configuration */  
	const l_u8* const data,
	/* [IN] length of data area */  
	l_u16 length
)
{
    l_u8 i;
    /* Set the default returned value to LD_DATA_ERROR */
    l_u8 retval = LD_DATA_ERROR;
    /** Set the expected length value to 
     * EXP = NN + NF, where :
     * NN = the number of NAD.
     * NF = the number of configurable frames;
     * Moreover:
     * Not taken PID's diagnostics frame: 3C, 3D
     */
    l_u16 expected_length;
    lin_configuration const *conf;     
    lin_node_attribute *node_att;
    
    /* Get current configuration */  
    conf = &lin_ifc_configuration[iii];
    
    expected_length = (l_u16) (conf->num_of_frames - 1);
    
    /* Check if slave node */
    if(_SLAVE_ == conf->function) {
        /* Get node attibute */
        node_att = conf-> node_attribute;
    
        if (length < expected_length)
        {
		    /* The 'data' size is not enough to contain NAD+PIDs */
		    retval = LD_LENGTH_NOT_CORRECT;
        }
        else 
        {
		    /* The 'data' size is enough to contain NAD+PIDs, so proceed to read from 'data' */
		    /* Read actual NAD from 'data' */
		    node_att -> configured_NAD = data[0];
		    
		    /* Copy protected IDs in 'data' to RAM configuration */
		    for (i = 1U; i < expected_length; ++i)
		    {
			    conf->configuration_RAM[i] = data[i];
		    }
		    /* No error, return OK */
		    retval = LD_SET_OK;
        } 
    }
    return retval;
} /* End ld_set_configuration() */


/** @} */
#endif /* End LIN_MODE == _MASTER_MODE_ */

/* -------------------------------------------------------------------- */

#if LIN_MODE == _SLAVE_MODE_
/* Unuse for GPIO */
#if (_LIN_GPIO_ == 0) && !defined(_MC9S08SC4_H)
/* Multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)

/* INITIALIZATION */
/** @addtogroup initialization_group
 * @{ */
void ld_init(void) {
	/* init transmit queue */
	lin_tl_tx_queue.queue_header = 0;
	lin_tl_tx_queue.queue_tail = 0;
	lin_tl_tx_queue.queue_status = LD_QUEUE_EMPTY;
	lin_tl_tx_queue.queue_current_size = 0;
	/* init receive queue */
	lin_tl_rx_queue.queue_header = 0;
	lin_tl_rx_queue.queue_tail = 0;
	lin_tl_rx_queue.queue_status = LD_NO_DATA;
	lin_tl_rx_queue.queue_current_size = 0;

	/* Init transmit message */
	tl_rx_msg_status = LD_COMPLETED;
	tl_rx_msg_index = 0;
	tl_rx_msg_size = 0;
	tl_receive_msg_status = LD_NO_MSG;

	/* Init receive message */
	tl_tx_msg_status = LD_COMPLETED;
	tl_tx_msg_index = 0;
	tl_tx_msg_size = 0;

	tl_last_cfg_result = LD_SUCCESS;
	tl_last_RSID = 0;
	tl_ld_error_code = 0;

	tl_frame_counter = 0;
	tl_no_of_pdu = 0;
	tl_slaveresp_cnt = 0;

	tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
	tl_check_timeout = N_MAX_TIMEOUT_CNT;

	tl_diag_state = LD_DIAG_IDLE;
	tl_service_status = LD_SERVICE_IDLE;
}
/** @} */
/* RAW APIs */
/** @addtogroup raw_api_group
 * @{ */
void ld_put_raw(const l_u8* const data) {
	tl_put_raw(data, &lin_tl_tx_queue, TRANSMISSION);
}

void ld_get_raw(l_u8* const data) {
	tl_get_raw(data, &lin_tl_rx_queue, RECEIVING);
}

l_u8 ld_raw_tx_status(void) {
	return (l_u8)lin_tl_tx_queue.queue_status;
}

l_u8 ld_raw_rx_status(void) {
	return (l_u8)lin_tl_rx_queue.queue_status;
}
/** @} */
/* COOKED APIs */
/** @addtogroup cooked_api_group
 * @{ */
void ld_send_message(l_u16 length, const l_u8* const data) {
	lin_tl_pdu_data pdu;
	l_u8 i;
	l_u8 message_size;
	l_u16 data_index = 0;
	l_u16 tmp_length = length;
	l_u16 frame_counter = 1;

	/* check message status in queue */
	if (LD_COMPLETED == tl_tx_msg_status) {
		/* calculate number of PDU for this message */
		if (length <= 6) {
			message_size = 1;
		} else {
			if((length-5)%6==0)
			message_size = ((length - 5) / 6) + 1;
			else
			message_size = ((length - 5) / 6) + 2;
		}
		tl_slaveresp_cnt=message_size;

		if (message_size <= (lin_tl_tx_queue.queue_max_size - lin_tl_tx_queue.queue_current_size)) {
			/* update information of message in queue */
			tl_tx_msg_index = lin_tl_tx_queue.queue_tail;
			tl_tx_msg_size = message_size;
			tl_tx_msg_status = LD_IN_PROGRESS;
			tl_service_status = LD_SERVICE_BUSY;

			/* package data */
			if (length <= 6) {
				/* package single frame */
				/* ____________________________________________ */
				/* | NAD | PCI | SID | D1 | D2 | D3 | D4 | D5 | */
				/* |_____|_____|_____|____|____|____|____|____| */

				pdu[0] = lin_configured_NAD;

				pdu[1] = (l_u8) length;
				pdu[2] = data[0]; /* SID / RSID */
				for (i = 1; i < length; i++) {
					pdu[i + 2] = data[i]; /* used data */
				}
				for (i = (l_u8) length; i < 6; i++) {
					pdu[i + 2] = 0xFF; /* unused data */
				}
				ld_put_raw(pdu);
			} else {
				/* package first frame */
				/* ____________________________________________ */
				/* | NAD | PCI | LEN |SID | D2 | D3 | D4 | D5 | */
				/* |_____|_____|_____|____|____|____|____|____| */

				pdu[0] = lin_configured_NAD;
				pdu[1] = ((length / 256) & 0x0F) | 0x10; /* PCI */
				pdu[2] = length % 256; /* length */
				pdu[3] = data[0]; /* SID / RSID */
				for (i = 1; i < 5; i++) { /* data */
					pdu[i + 3] = data[i];
				}
				data_index += 5;
				tmp_length -= 5;
				ld_put_raw(pdu);

				/* package consecutive frame */
				/* ___________________________________________ */
				/* | NAD | PCI | D1 | D2 | D3 | D4 | D5 | D6 | */
				/* |_____|_____|____|____|____|____|____|____| */
				message_size--;

				pdu[0] = lin_configured_NAD;
				while (message_size > 0) {
					pdu[1] = 0x20 | (frame_counter & 0x0F);
					frame_counter++;
					if(frame_counter > 15)
					frame_counter = 0;
					if (tmp_length < 6) { /* last PDU */
						/* used data */
						for (i = 0; i < tmp_length; i++) {
							pdu[i + 2] = data[data_index++];
						}
						/* unused data */
						for (i = (l_u8) tmp_length; i < 6; i++) {
							pdu[i + 2] = 0xFF;
						}
					} else {
						for (i = 2; i < 8; i++) {
							pdu[i] = data[data_index++];
						}
						tmp_length -= 6;
					} /* end of (tmp < 6 ) */
					message_size--;
					ld_put_raw(pdu);
				} /* end of (message > 0) */
			} /* end of (length < 6) */
			
		} /* end of check message size */
	} /* end of (LD_COMPLETED == tl_conf->tl_message_status) */
}

void ld_receive_message(l_u16* const length, l_u8* const data) {

	lin_tl_pdu_data pdu;
	l_u8 i;
	l_u16 data_index = 0;
	l_u16 tmp_length;
	l_u16 frame_counter;
	l_u8 PCI_type;

	/* set status of receive message */
	tl_rx_msg_status = LD_IN_PROGRESS;

	/* wait message is received completely */
	while (LD_COMPLETED != tl_receive_msg_status) {
		/* check message error */
		if ((LD_FAILED == tl_receive_msg_status) || (LD_WRONG_SN
						== tl_receive_msg_status) || (LD_N_CR_TIMEOUT
						== tl_receive_msg_status)) {
			tl_rx_msg_status = tl_receive_msg_status;
			return;
		}
	}
	/* Message is received completely */
	/* get data from receive queue */
	ld_get_raw(pdu);
	/* Check type of pdu */
	PCI_type = (pdu[1] & 0xF0) >> 4;
	switch (PCI_type) {
		/* Single frame */
		case SF:
		tmp_length = pdu[1] & 0x0f;
		*length = tmp_length;
		data[0] = pdu[2];
		for (i = 1; i < tmp_length; i++) {
			data[i] = pdu[i + 2];
		}
		break;
		/* First frame */
		case FF:
		tmp_length = (pdu[1] & 0x0F) * 256 + pdu[2];
		*length = tmp_length;
		data[0] = pdu[3];
		for (i = 1; i < 5; i++) {
			data[i] = pdu[i + 3];
		}
		tmp_length -= 5;
		data_index += 5;
		/* Consecutive frame */
		case CF:
		while (tmp_length > 6) {
			/* get PDU */
			ld_get_raw(pdu);
			frame_counter = pdu[1] & 0x0F;

			for (i = 2; i < 8; i++) {
				data[data_index++] = pdu[i];
			}
			tmp_length -= 6;
		}

		/* Last frame */
		if (tmp_length > 0) {
			/* get PDU */
			ld_get_raw(pdu);
			frame_counter = pdu[1] & 0x0F;

			for (i = 0; i < tmp_length; i++) {
				data[data_index++] = pdu[i + 2];
			}
		}

		break;
		default:
		break;
	} /* end of switch */
	tl_receive_msg_status = LD_NO_MSG;
	tl_rx_msg_status = LD_COMPLETED;
}

l_u8 ld_tx_status(void) {
	return (l_u8)tl_tx_msg_status;
}

l_u8 ld_rx_status(void) {
	return (l_u8)tl_rx_msg_status;
}
/** @} */
#endif /*End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)*/


l_u8 ld_read_configuration
(
	/* [IN] data area to save configuration */ 
	l_u8* const data,
	/* [IN] length of data area */ 
	l_u8* const length
)
{
    l_u8 i, temp;
    /* Set the default returned value to LD_READ_OK */
    l_u8 retval = (l_u8)LD_READ_OK;
    /** Set the expected length value to 
     * EXP = NN + NF, where :
     * NN = the number of NAD.
     * NF = the number of configurable frames;
     * Moreover:
     * Not taken PID's diagnostics frame: 3C, 3D
     */
    l_u8 expected_length = (l_u8)(LIN_NUM_OF_FRMS - 1U);
    temp = *length;
    if (temp < expected_length)
    {
      /* The 'data' size is not enough to store NAD+PIDs */
      retval = (l_u8)LD_LENGTH_TOO_SHORT;
    }
    else 
    {
		/* The 'data' size is enough to store NAD+PIDs, so proceed to store them */
		/* Copy actual NAD to 'data' */
		data[0] = lin_configured_NAD;
		/* Copy protected IDs to 'data' */
		for (i = 1U; i < expected_length; ++i)
		{
			data[i] = lin_configuration_RAM[i];
		}
		/* Set the length parameter to the actual size of the configuration */
		*length = expected_length;
    }
    return retval;
} /* End ld_read_configuration() */
  

l_u8 ld_set_configuration
(
	/* [IN] data area to set configuration */  
	const l_u8* const data,
	/* [IN] length of data area */  
	l_u16 length
)
{
    l_u8 i;
    /* Set the default returned value to LD_DATA_ERROR */
    l_u8 retval = LD_DATA_ERROR;
    /** Set the expected length value to 
     * EXP = NN + NF, where :
     * NN = the number of NAD.
     * NF = the number of configurable frames;
     * Moreover:
     * Not taken PID's diagnostics frame: 3C, 3D
     */
    l_u16 expected_length = (l_u8)(LIN_NUM_OF_FRMS - 1U);
    if (length < expected_length)
    {
		/* The 'data' size is not enough to contain NAD+PIDs */
		retval = LD_LENGTH_NOT_CORRECT;
    }
    else 
    {
		/* The 'data' size is enough to contain NAD+PIDs, so proceed to read from 'data' */
		/* Read actual NAD from 'data' */
		lin_configured_NAD = data[0];
		/* Copy protected IDs in 'data' to RAM configuration */
		for (i = 1U; i < expected_length; ++i)
		{
			lin_configuration_RAM[i] = data[i];
		}
		/* No error, return OK */
		retval = LD_SET_OK;
    }
    return retval;
} /* End ld_set_configuration() */

#endif /* End (_LIN_GPIO_ == 0) && !defined(_MC9S08SC4_H) */
#endif /* End LIN_MODE == _SLAVE_MODE_ */
/**
 * @}
 */
