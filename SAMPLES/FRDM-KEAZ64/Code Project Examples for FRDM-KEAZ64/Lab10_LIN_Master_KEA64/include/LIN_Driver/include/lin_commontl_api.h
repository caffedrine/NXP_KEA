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
* @file      lin_commontl_api.h 
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
 * 20090408		v1.0	First version
 * 20111005		v1.1	Added porotytpe functions
 *
 *****************************************************************************/
 
#ifndef _LIN_COMMONTL_API_H
#define _LIN_COMMONTL_API_H
/** @} */

#include "LIN_Driver/lowlevel/lin.h"

/* Node configuration */
#define LD_READ_OK                        0x33   /**< Read OK */
#define LD_LENGTH_TOO_SHORT               0x34   /**< Lenghth too short */

#define LD_DATA_ERROR                     0x43   /**< Data error */
#define LD_LENGTH_NOT_CORRECT             0x44   /**< Lenghth not correct */
#define LD_SET_OK                         0x45   /**< Set OK */

#if LIN_MODE == _MASTER_MODE_

#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
/* Initialized transport layer */
/** @addtogroup initialization_group
* @{ */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ld_init (l_ifc_handle iii)
* @brief initialize or reinitalize the raw and cooked layers
*
* @param iii <B>[IN]</B> lin interface handle
*
* @return #void
*
* @SDD_ID LIN_SDD_17
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>tl_conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   This call will initialize or reinitalize the raw and cooked layers
*   on the interface iii.
*   All the transport layer buffers will be initialized.
*//*END*----------------------------------------------------------------------*/
void ld_init(l_ifc_handle iii);
/** @} */
/* Transport layer RAW APIs */

/** @addtogroup raw_api_group
* @{ */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ld_put_raw (l_ifc_handle iii, const l_u8* const data)
* @brief The call queues the transmisson of 8 bytes of data in one frame
*
* @param iii <B>[IN]</B> lin interface handle
* @param data <B>[IN]</B> buffer for the data to be transmitted
*
* @return #void
*
* @SDD_ID LIN_SDD_318
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>tl_conf</I>
*   -# <B>#lin_transport_layer_queue</B> <I>tl_queue</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   The call queues the transmisson of 8 bytes of data in one frame
*   The data is sent in the next suitable frame.
*
* @see #tl_put_raw
*//*END*----------------------------------------------------------------------*/
void ld_put_raw(l_ifc_handle iii,const l_u8* const data);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ld_get_raw (l_ifc_handle iii, l_u8* const data)
* @brief The call copies the oldest received diagnostic frame data to the memory
*   specified by data
*
* @param iii <B>[IN]</B> lin interface handle
* @param data <B>[IN]</B> buffer for store data received
*
* @return #void
*
* @SDD_ID LIN_SDD_319
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>tl_conf</I>
*   -# <B>#lin_transport_layer_queue</B> <I>tl_queue</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   The call copies the oldest received diagnostic frame data to the memory
*   specified by data.
*   The data returned is received from master request frame for slave node
*   and the slave response frame for master node.
*//*END*----------------------------------------------------------------------*/
void ld_get_raw(l_ifc_handle iii,l_u8* const data);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 ld_raw_tx_status (l_ifc_handle iii)
* @brief The call returns the status of the raw frame transmission function
*
* @param iii <B>[IN]</B> lin interface handle
*
* @return #void
*
* @SDD_ID LIN_SDD_320
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>tl_conf</I>
*   -# <B>#lin_transport_layer_queue</B> <I>tl_queue</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   The call returns the status of the raw frame transmission function:<br/>
*      LD_QUEUE_EMPTY    : The transmit queue is empty. In case previous calls to <br/>
*                          ld_put_raw, all frames in the queue have been          <br/>
*                          trans-mitted.                                          <br/>
*      LD_QUEUE_AVAILABLE: The transmit queue contains entries, but is not full.  <br/>
*      LD_QUEUE_FULL     : The transmit queue is full and can not accept further  <br/>
*                          frames.                                                <br/>
*      LD_TRANSMIT_ERROR :  LIN protocol errors occurred during  the transfer;    <br/>
*                           ini-tialize and redo the transfer.
*//*END*----------------------------------------------------------------------*/
l_u8 ld_raw_tx_status(l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 ld_raw_rx_status (l_ifc_handle iii)
* @brief The call returns the status of the raw frame receive function
*
* @param iii <B>[IN]</B> lin interface handle
*
* @return #void
*
* @SDD_ID LIN_SDD_321
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>tl_conf</I>
*   -# <B>#lin_transport_layer_queue</B> <I>tl_queue</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   The call returns the status of the raw frame receive function:
*       LD_NO_DATA          The receive queue is empty.
*       LD_DATA_AVAILABLE   The receive queue contains data that can be read.
*       LD_RECEIVE_ERROR    LIN protocol errors occurred during  the transfer;
*                           ini-tialize and redo the transfer.
*//*END*----------------------------------------------------------------------*/
l_u8 ld_raw_rx_status(l_ifc_handle iii);
/** @} */
/* Transport layer COOKED APIs */
/** @addtogroup cooked_api_group 
* @{ */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ld_send_message (l_ifc_handle iii, l_u16 length, l_u8 NAD, const l_u8* const data)
* @brief The call packs the information specified by data and length into one or multiple diagnostic frames
*
* @param iii <B>[IN]</B> lin interface handle
* @param length <B>[IN]</B> length of data to send
* @param NAD <B>[IN]</B> Node address of slave node
* @param data <B>[IN]</B> data to be sent
* @return #void
*
* @SDD_ID LIN_SDD_322
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>tl_conf</I>
*   -# <B>#lin_transport_layer_queue</B> <I>tl_queue</I>
*   -# <B>#lin_tl_pdu_data</B> <I>queue_data</I>
*   -# <B>#lin_tl_pdu_data</B> <I>pdu</I>
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#l_u8</B> <I>message_size</I>
*   -# <B>#l_u16</B> <I>data_index = 0</I>
*   -# <B>#l_u16</B> <I>tmp_length = length</I>
*   -# <B>#l_u16</B> <I>frame_counter = 0</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*     The call packs the information specified by data and length into one or multiple
*     diagnostic frames. If the call is made in a master node application the frames
*     are transmitted to the slave node  with the address NAD. If the  call is made in
*     a slave node application the frames are transmitted to the master node with the
*     address NAD. The parameter NAD is not used in slave nodes.
*//*END*----------------------------------------------------------------------*/
/** @} */
void ld_send_message(l_ifc_handle iii, l_u16 length, l_u8 NAD, const l_u8* const data);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ld_receive_message (l_ifc_handle iii, l_u16 *const length, l_u8 *const NAD, l_u8 *const data)
* @brief The call prepares the LIN diagnostic module to receive one message and store it in the buffer pointed to  by data
*
* @param iii <B>[IN]</B> lin interface handle
* @param length <B>[OUT]</B> length of data to receive
* @param NAD <B>[OUT]</B> Node address of slave node
* @param data <B>[OUT]</B> data to be sent
*
* @return #void
*
* @SDD_ID LIN_SDD_323
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>tl_conf</I>
*   -# <B>#lin_transport_layer_queue</B> <I>tl_queue</I>
*   -# <B>#lin_tl_pdu_data</B> <I>queue_data</I>
*   -# <B>#lin_tl_pdu_data</B> <I>pdu</I>
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#l_u16</B> <I>data_index = 0</I>
*   -# <B>#l_u16</B> <I>tmp_length</I>
*   -# <B>#l_u16</B> <I>frame_counter</I>
*   -# <B>#l_u8</B> <I>PCI_type</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*     The call prepares the LIN diagnostic module to receive one message and store it in
*     the buffer pointed to  by data. At the call, length shall specify the maximum length
*     allowed. When the reception has completed, length is changed to the actual length
*     and NAD to the NAD in the message.
*//*END*----------------------------------------------------------------------*/
void ld_receive_message(l_ifc_handle iii, l_u16* const length, l_u8* const NAD, l_u8* const data);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 ld_tx_status (l_ifc_handle iii)
* @brief The call returns the status of the last made call to ld_send_message.
*
* @param iii <B>[IN]</B> lin interface handle
* @return #l_u8
*
* @SDD_ID LIN_SDD_324
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>tl_conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*     The call returns the status of the last made call to ld_send_message.
*     The following values can be returned:
*         LD_IN_PROGRESS:  The transmission is not yet completed.
*         LD_COMPLETED:    The transmission has  completed successfully (and
*                          you can issue a new ld_send_message call). This
*                          value is also returned after  initialization of
*                          the transport layer.
*         LD_FAILED:       The transmission ended in an error. The data was
*                          only partially sent. The transport layer shall
*                          be reinitialized before processing  further messages.
*                          To find out why a transmission has  failed, check
*                          the status management function l_read_status
*         LD_N_AS_TIMEOUT: The transmission failed because of a N_As timeout,
*//*END*----------------------------------------------------------------------*/
l_u8 ld_tx_status(l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 ld_rx_status (l_ifc_handle iii)
* @brief The call returns the status of the last made call to ld_send_message.
*
* @param iii <B>[IN]</B> lin interface handle
* @return #void
*
* @SDD_ID LIN_SDD_325
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>tl_conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*     The call returns the status of the last made call to ld_receive_message.          <br/>
*     The following values can be returned:                                             <br/>
*         LD_IN_PROGRESS: The reception is not yet completed.                           <br/>
*         LD_COMPLETED:   The reception has  completed successfully and all             <br/>
*                         information (length, NAD, data) is available. (You can        <br/>
*                         also issue a new ld_receive_message call). This               <br/>
*                         value is also returned after  initialization of the           <br/>
*                         transport layer.                                              <br/>
*         LD_FAILED:      The reception ended in an error. The data was only            <br/>
*                         partially received and should not be trusted. Initialize      <br/>
*                         before processing further  transport layer messages.          <br/>
*                         To find out why a reception has failed, check the status      <br/>
*                         management function  l_read_status.                           <br/>
*         LD_N_CR_TIMEOUT The reception failed because of a N_Cr timeout                <br/>
*         LD_WRONG_SN     The reception failed because of an unexpected sequence number.
*//*END*----------------------------------------------------------------------*/
l_u8 ld_rx_status(l_ifc_handle iii);
/** @} */

/* Configuration and Identify APIs */
/** @addtogroup node_configuration_group 
* @{ */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ld_assign_NAD (l_ifc_handle iii, l_u8 initial_NAD, l_u16 supplier_id, l_u16 function_id, l_u8 new_NAD)
* @brief This call assigns the NAD (node diagnostic address) of all slave nodes that matches the initial_NAD, the supplier ID and the function ID.
*
* @param iii <B>[IN]</B> lin interface handle
* @param initial_NAD <B>[IN]</B> initial node address  of the target node
* @param supplier_id <B>[IN]</B> supplier ID of the target node
* @param function_id <B>[IN]</B> function identifier of the target node
* @param new_NAD <B>[IN]</B> new node address
*
* @return #void
*
* @SDD_ID LIN_SDD_305
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>data[6]</I>
*   -# <B>#lin_configuration</B> <I>conf</I>

*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   This call assigns the NAD (node diagnostic address) of all slave nodes
*   that matches the initial_NAD, the supplier ID and the function ID.
*   The new NAD of the slave node will be new_NAD.
*//*END*----------------------------------------------------------------------*/
#endif /*End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)*/
void ld_assign_NAD(l_ifc_handle iii, l_u8 initial_NAD, l_u16 supplier_id, l_u16 function_id, l_u8 new_NAD);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ld_conditional_change_NAD (l_ifc_handle iii, l_u8 NAD, l_u8 id, l_u8 byte, l_u8 mask, l_u8 invert, l_u8 new_NAD)
* @brief This call changes the NAD if the node properties fullfil the test specified by id, byte, mask and invert.
*
* @param iii <B>[IN]</B> lin interface handle
* @param NAD <B>[IN]</B> current NAD value of the target node
* @param id <B>[IN]</B> property ID of the target node
* @param byte <B>[IN]</B> byte location of property value to be read from the target node
* @param mask <B>[IN]</B> value for masking the read property byte
* @param invert <B>[IN]</B> value for excluding the read property byte
* @param new_NAD <B>[IN]</B> new NAD value to be assigned when the condition is met
*
* @return #void
*
* @SDD_ID LIN_SDD_206
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>data[6]</I>
*   -# <B>#lin_configuration</B> <I>conf</I>

*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   This call changes the NAD if the node properties fullfil the test
*   specified by id, byte, mask and invert.
*//*END*----------------------------------------------------------------------*/
void ld_conditional_change_NAD (l_ifc_handle iii, l_u8 NAD, l_u8 id, l_u8 byte, l_u8 mask, l_u8 invert, l_u8 new_NAD);
/** @} */
/** @addtogroup node_identification_group
* @{ */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ld_read_by_id(l_ifc_handle iii, l_u8 NAD, l_u16 supplier_id, l_u16 function_id, l_u8 id, l_u8* const data)
* @brief The call requests the slave node selected with the NAD to return the property associated with the id parameter.
*
* @param iii <B>[IN]</B> lin interface handle
* @param NAD <B>[IN]</B> alue of the target node
* @param supplier_id <B>[IN]</B> supplier ID of the target node
* @param function_id <B>[IN]</B> function ID of the target node
* @param id <B>[IN]</B> ID of the target node
* @param data <B>[IN]</B> buffer for saving the data read from the node
*
* @return #void
*
* @SDD_ID LIN_SDD_307
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>buf[6]</I>
*   -# <B>#lin_configuration</B> <I>conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>tl_conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   The call requests the slave node selected with the NAD to return the
*   property associated with the id parameter.
*//*END*----------------------------------------------------------------------*/
void ld_read_by_id (l_ifc_handle iii, l_u8 NAD, l_u16 supplier_id, l_u16 function_id, l_u8 id, l_u8* const data);



/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ld_save_configuration(l_ifc_handle iii,l_u8 NAD)
* @brief This function to issue a save configuration request to a slave node
*
* @param iii <B>[IN]</B> interface name
* @param NAD <B>[IN]</B> node adress of target
*
* @return #void
*
* @SDD_ID LIN_SDD_xxx
* @endif
*
* @param iii <B>[IN]</B> lin interface handle
* @param NAD <B>[IN]</B> Node address of target
*
* @local_var
*   -# <B>#lin_configuration</B> <I>*conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>*tl_conf</I>
*   -# <B>#lin_tl_pdu_data</B> <I>*lin_tl_pdu</I>
*   -# <B>l_u8</B> <I>data[6]</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
*
* @details
*   This function is called to send a save configuration request to a specific slave node <br>
*   with the given NAD, or to all slave nodes if NAD is set to broadcast
*   This function is implemented for Master Only
*//*END*----------------------------------------------------------------------*/
void ld_save_configuration(l_ifc_handle iii,l_u8 NAD);


/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 ld_read_configuration(l_ifc_handle iii, l_u8* const data, l_u8* const length)
* @brief This function to copy current configuration in a reserved area
*
*
* @return #l_u8
*
* @SDD_ID LIN_SDD_xxx
* @endif
*
* @param iii <B>[IN]</B> lin interface handle
* @param data <B>[IN]</B> data area to save configuration, 
* @param length <B>[IN]</B> length of data arean (1+n, NAD+PIDs)
*
* @local_var
*   -# <B>#i</B> <I>l_u8</I>
*   -# <B>#temp</B> <I>l_u8</I>
*   -# <B>#retval</B> <I>l_u8</I>
*   -# <B>#expected_length</B> <I>l_u8</I>
*
* @static_global_var
*   -# <B>#lin_configured_NAD</B>
*   -# <B>#lin_configuration_RAM</B>
*
* @details
*   This function is implemented Slave Only
*//*END*----------------------------------------------------------------------*/
l_u8 ld_read_configuration(l_ifc_handle iii, l_u8* const data, l_u8* const length);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 ld_set_configuration(l_ifc_handle iii, const l_u8* const data, l_u16 length)
* @brief This function function to configure slave node according to data
*
*
* @return #l_u8
*
* @SDD_ID LIN_SDD_xxx
* @endif
*
* @param iii <B>[IN]</B> lin interface handle
* @param data <B>[IN]</B> structure containing the NAD and all the n PIDs for the frames of the specified NAD, 
* @param length <B>[IN]</B> length of data arean (1+n, NAD+PIDs)
*
* @local_var
*   -# <B>#i</B> <I>l_u8</I>
*   -# <B>#retval</B> <I>l_u8</I>
*   -# <B>#expected_length</B> <I>l_u8</I>
*
* @static_global_var
*   -# <B>#lin_configured_NAD</B>
*   -# <B>#lin_configuration_RAM</B>
*
* @details
*   This function is implemented Slave Only
*//*END*----------------------------------------------------------------------*/
l_u8 ld_set_configuration(l_ifc_handle iii, const l_u8* const data, l_u16 length);


/** @} */
#endif /* End LIN_MODE == _MASTER_MODE_ */



/* SLAVE MODE */
#if LIN_MODE == _SLAVE_MODE_
/* Unuse for GPIO */
#if (_LIN_GPIO_ == 0) && !defined(_MC9S08SC4_H)
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)

/* INITIALIZATION */
/** @addtogroup initialization_group
* @{ */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ld_init (void)
* @brief initialize or reinitalize the transport layer
*
* @return #void
*
* @SDD_ID LIN_SDD_341
* @endif 
*
* @static_global_var
*   -# <B>#lin_tl_tx_queue</B>
*   -# <B>#lin_tl_rx_queue</B>
*   -# <B>#tl_rx_msg_status</B>
*   -# <B>#tl_rx_msg_index</B>
*   -# <B>#tl_rx_msg_size</B>
*   -# <B>#tl_receive_msg_status</B>
*   -# <B>#tl_tx_msg_status</B>
*   -# <B>#tl_tx_msg_index</B>
*   -# <B>#tl_tx_msg_size</B>
*   -# <B>#tl_last_cfg_result</B>
*   -# <B>#tl_last_RSID</B> 
*   -# <B>#tl_ld_error_code</B>
*   -# <B>#tl_frame_counter</B>
*   -# <B>#tl_no_of_pdu</B>
*   -# <B>#tl_slaveresp_cnt</B>
*   -# <B>#tl_check_timeout_type</B>
*   -# <B>#tl_check_timeout</B>
*   -# <B>#tl_service_status</B>
*
* @details
*   This call will initialize or reinitalize the raw and cooked layers
*   on the interface iii.
*   All the transport layer buffers will be initialized.
*//*END*----------------------------------------------------------------------*/
void ld_init(void);
/** @} */

/* RAW APIs */
/** @addtogroup raw_api_group
* @{ */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ld_put_raw (const l_u8* const data)
* @brief The call queues the transmisson of 8 bytes of data in one frame
*
* @param data <B>[IN]</B> buffer for the data to be transmitted
*
* @return #void
*
* @SDD_ID LIN_SDD_342
* @endif
* 
* @details
*   The call queues the transmisson of 8 bytes of data in one frame
*   The data is sent in the next suitable frame.
*
* @see #tl_put_raw
*//*END*----------------------------------------------------------------------*/
void ld_put_raw(const l_u8* const data);
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ld_get_raw (l_u8* const data)
* @brief The call copies the oldest received diagnostic frame data to the memory
*   specified by data
*
* @param data <B>[IN]</B> buffer for store data received
*
* @return #void
*
* @SDD_ID LIN_SDD_343
* @endif
*
* @details
*   The call copies the oldest received diagnostic frame data to the memory
*   specified by data.
*   The data returned is received from master request frame for slave node
*   and the slave response frame for master node.
* @see #tl_get_raw
*//*END*----------------------------------------------------------------------*/
void ld_get_raw(l_u8* const data);
/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 ld_raw_tx_status (void)
* @brief The call returns the status of the raw frame transmission function
*
* @return #void
*
* @SDD_ID LIN_SDD_344
* @endif
*
* @static_global_var
*   -# <B>#lin_tl_tx_queue</B>
*
* @details
*   The call returns the status of the raw frame transmission function:<br/>
*      LD_QUEUE_EMPTY    : The transmit queue is empty. In case previous calls to <br/>
*                          ld_put_raw, all frames in the queue have been          <br/>
*                          trans-mitted.                                          <br/>
*      LD_QUEUE_AVAILABLE: The transmit queue contains entries, but is not full.  <br/>
*      LD_QUEUE_FULL     : The transmit queue is full and can not accept further  <br/>
*                          frames.                                                <br/>
*      LD_TRANSMIT_ERROR :  LIN protocol errors occurred during  the transfer;    <br/>
*                           ini-tialize and redo the transfer.
*//*END*----------------------------------------------------------------------*/
l_u8 ld_raw_tx_status(void);
/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 ld_raw_rx_status (void)
* @brief The call returns the status of the raw frame receive function
*
* @return #void
*
* @SDD_ID LIN_SDD_345
* @endif
* 
* @static_global_var
*   -# <B>#lin_tl_rx_queue</B>
*
* @details
*   The call returns the status of the raw frame receive function:
*       LD_NO_DATA          The receive queue is empty.
*       LD_DATA_AVAILABLE   The receive queue contains data that can be read.
*       LD_RECEIVE_ERROR    LIN protocol errors occurred during  the transfer;
*                           ini-tialize and redo the transfer.
*//*END*----------------------------------------------------------------------*/
l_u8 ld_raw_rx_status(void);
/** @} */
/* COOKED APIs */
/** @addtogroup cooked_api_group
* @{ */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ld_send_message (l_u16 length, const l_u8* const data)
* @brief The call packs the information specified by data and length into one or multiple diagnostic frames
*
* @param length <B>[IN]</B> length of data to send
* @param data <B>[IN]</B> data to be sent
*
* @return #void
*
* @SDD_ID LIN_SDD_346
* @endif
*
* @local_var
*   -# <B>#lin_tl_pdu_data</B> <I>pdu</I>
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#l_u8</B> <I>message_size</I>
*   -# <B>#l_u16</B> <I>data_index</I>
*   -# <B>#l_u16</B> <I>tmp_length</I>
*   -# <B>#l_u16</B> <I>frame_counter</I>
*
* @static_global_var
*   -# <B>#tl_tx_msg_status</B>
*   -# <B>#tl_slaveresp_cnt</B>
*   -# <B>#lin_tl_tx_queue</B>
*   -# <B>#tl_tx_msg_index</B>
*   -# <B>#tl_tx_msg_size</B>
*   -# <B>#tl_tx_msg_status</B>
*   -# <B>#lin_configured_NAD</B>
*   -# <B>#tl_check_timeout</B>
*   -# <B>#tl_check_timeout_type</B>
*
* @details
*     The call packs the information specified by data and length into one or multiple
*     diagnostic frames. If the call is made in a master node application the frames
*     are transmitted to the slave node  with the address NAD. If the  call is made in
*     a slave node application the frames are transmitted to the master node with the
*     address NAD. The parameter NAD is not used in slave nodes.
*
* @see #ld_put_raw
*//*END*----------------------------------------------------------------------*/
void ld_send_message(l_u16 length, const l_u8* const data);
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ld_receive_message (l_u16 *const length, l_u8 *const data)
* @brief The call prepares the LIN diagnostic module to receive one message and store it in the buffer pointed to  by data
*
* @param length <B>[OUT]</B> length of data to receive
* @param data <B>[OUT]</B> data to be sent
*
* @return #void
*
* @SDD_ID LIN_SDD_347
* @endif
*
* @local_var
*   -# <B>#lin_tl_pdu_data</B> <I>pdu</I>
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#l_u16</B> <I>data_index = 0</I>
*   -# <B>#l_u16</B> <I>tmp_length</I>
*   -# <B>#l_u16</B> <I>frame_counter</I>
*   -# <B>#l_u8</B> <I>PCI_type</I>
*
* @static_global_var
*   -# <B>#tl_rx_msg_status</B>
*   -# <B>#tl_receive_msg_status</B>
*
* @details
*     The call prepares the LIN diagnostic module to receive one message and store it in
*     the buffer pointed to  by data. At the call, length shall specify the maximum length
*     allowed. When the reception has completed, length is changed to the actual length
*     and NAD to the NAD in the message.
*
* @see #ld_get_raw
*//*END*----------------------------------------------------------------------*/
void ld_receive_message(l_u16* const length, l_u8* const data);
/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 ld_tx_status ()
* @brief The call returns the status of the last made call to ld_send_message.
*
* @return #l_u8
*
* @SDD_ID LIN_SDD_348
* @endif
*
* @static_global_var
*   -# <B>#tl_tx_msg_status</B>
*
* @details
*     The call returns the status of the last made call to ld_send_message.
*     The following values can be returned:
*         LD_IN_PROGRESS:  The transmission is not yet completed.
*         LD_COMPLETED:    The transmission has  completed successfully (and
*                          you can issue a new ld_send_message call). This
*                          value is also returned after  initialization of
*                          the transport layer.
*         LD_FAILED:       The transmission ended in an error. The data was
*                          only partially sent. The transport layer shall
*                          be reinitialized before processing  further messages.
*                          To find out why a transmission has  failed, check
*                          the status management function l_read_status
*         LD_N_AS_TIMEOUT: The transmission failed because of a N_As timeout,
*//*END*----------------------------------------------------------------------*/
l_u8 ld_tx_status(void);
/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 ld_rx_status (void)
* @brief The call returns the status of the last made call to ld_send_message.
*
* @return #void
*
* @SDD_ID LIN_SDD_349
* @endif
*
* @static_global_var
*   -# <B>#tl_rx_msg_status</B>
*
* @details
*     The call returns the status of the last made call to ld_receive_message.          <br/>
*     The following values can be returned:                                             <br/>
*         LD_IN_PROGRESS: The reception is not yet completed.                           <br/>
*         LD_COMPLETED:   The reception has  completed successfully and all             <br/>
*                         information (length, NAD, data) is available. (You can        <br/>
*                         also issue a new ld_receive_message call). This               <br/>
*                         value is also returned after  initialization of the           <br/>
*                         transport layer.                                              <br/>
*         LD_FAILED:      The reception ended in an error. The data was only            <br/>
*                         partially received and should not be trusted. Initialize      <br/>
*                         before processing further  transport layer messages.          <br/>
*                         To find out why a reception has failed, check the status      <br/>
*                         management function  l_read_status.                           <br/>
*         LD_N_CR_TIMEOUT The reception failed because of a N_Cr timeout                <br/>
*         LD_WRONG_SN     The reception failed because of an unexpected sequence number.
*//*END*----------------------------------------------------------------------*/
l_u8 ld_rx_status(void);
/** @} */
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */



/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ld_read_configuration()
* @brief This function to copy current configuration in a reserved area
*
*
* @return #l_u8
*
* @SDD_ID LIN_SDD_xxx
* @endif
* @param data <B>[IN]</B> data area to save configuration, 
* @param length <B>[IN]</B> length of data arean (1+n, NAD+PIDs)
*
* @local_var
*   -# <B>#i</B> <I>l_u8</I>
*   -# <B>#temp</B> <I>l_u8</I>
*   -# <B>#retval</B> <I>l_u8</I>
*   -# <B>#expected_length</B> <I>l_u8</I>
*
* @static_global_var
*   -# <B>#lin_configured_NAD</B>
*   -# <B>#lin_configuration_RAM</B>
*
* @details
*   This function is implemented Slave Only
*//*END*----------------------------------------------------------------------*/
l_u8 ld_read_configuration(l_u8* const data, l_u8* const length);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ld_set_configuration()
* @brief This function function to configure slave node according to data
*
*
* @return #l_u8
*
* @SDD_ID LIN_SDD_xxx
* @endif
*
* @param data <B>[IN]</B> structure containing the NAD and all the n PIDs for the frames of the specified NAD, 
* @param length <B>[IN]</B> length of data arean (1+n, NAD+PIDs)
*
* @local_var
*   -# <B>#i</B> <I>l_u8</I>
*   -# <B>#retval</B> <I>l_u8</I>
*   -# <B>#expected_length</B> <I>l_u8</I>
*
* @static_global_var
*   -# <B>#lin_configured_NAD</B>
*   -# <B>#lin_configuration_RAM</B>
*
* @details
*   This function is implemented Slave Only
*//*END*----------------------------------------------------------------------*/
l_u8 ld_set_configuration(const l_u8* const data, l_u16 length);

#endif /* End (_LIN_GPIO_ == 0) && !defined(_MC9S08SC4_H) */
#endif /* End LIN_MODE == _SLAVE_MODE_ */
#endif  /* _LIN_COMMONTL_API_H */
