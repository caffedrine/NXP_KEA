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
* @file      lin_commontl_proto.h
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
 * 20090407		v1.0	First version
 * 20111005		v1.1	Added extern functions
 *
 *****************************************************************************/
 
#ifndef _LIN_COMMONTL_PROTO_H
#define _LIN_COMMONTL_PROTO_H

#include "LIN_Driver/lowlevel/lin.h"

/* define PCI type */                     
#define PCI_SF                            0x00      /**< Single Frame */ 
#define PCI_FF                            0x01      /**< First Frame */
#define PCI_CF                            0x02      /**< Consecutive Frame */

/* For J2602 */
#define SERVICE_TARGET_RESET              0xB5      /**< target reset service */
#define RES_POSITIVE                      0x40      /**< positive response */

/* For negative response */
#define RES_NEGATIVE                      0x7F      /**< negative response */
#define GENERAL_REJECT                    0x10      /**< Error code raised when request for service not supported comes  */
#define SERVICE_NOT_SUPPORTED             0x11      /**< Error code in negative response for not supported service */ 
#define SUBFUNCTION_NOT_SUPPORTED         0x12      /**< Error code in negative response for not supported subfunction  */

/* Response type from Slave */
#define NEGATIVE 0                                  /**< negative response */
#define POSITIVE 1                                  /**< positive response */
/* multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
/* Transmission direction */
#define TRANSMISSION 0                              /**< transmission */
#define RECEIVING    1                              /**< receiveing */
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
/* Define wildcards */
#define LD_BROADCAST                      0x7F   /**< NAD */
#define LD_FUNCTIONAL_NAD                 0x7E   /**< functional NAD */
#define LD_ANY_SUPPLIER                   0x7FFF /**< Supplier */
#define LD_ANY_FUNCTION                   0xFFFF /**< Function */   
#define LD_ANY_MESSAGE                    0xFFFF /**< Message */

/* Identifiers of node read by identifier service */
#define LIN_PRODUCT_IDENT                 0x00   /**< Node product identifier */
#define SERIAL_NUMBER                     0x01   /**< Serial number */


#if LIN_MODE == _SLAVE_MODE_
/* Unuse for GPIO */
#if (_LIN_GPIO_ == 0) && !defined(_MC9S08SC4_H)

extern l_u8 ld_read_by_id_callout(l_u8 id, l_u8 *data);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_tl_make_slaveres_pdu (l_u8 sid, l_u8 res_type, l_u8 error_code)
* @brief This function is implemented for Slave only
*
* @param sid <B>[IN]</B> service identifier
* @param res_type <B>[IN]</B> type of response
* @param error_code <B>[IN]</B> Error code in case of negative response, if positive = 0
*
* @return #void
*
* @SDD_ID LIN_SDD_350
* @endif
*
* @static_global_var
*   -# <B>#tx_single_pdu_data</B>
*   -# <B>#tl_current_tx_pdu_ptr</B>
*   -# <B>#product_id</B>
*   -# <B>#tl_pdu_frame_mode</B>
*   -# <B>#tl_slaveresp_cnt</B>


* @details
*   This function is implemented for Slave only
*//*END*----------------------------------------------------------------------*/
void lin_tl_make_slaveres_pdu(l_u8 sid, l_u8 res_type, l_u8 error_code);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_tl_attach_service()
* @brief This function make a connection to Diag service class that support for LIN function
*
*
* @return #void
*
* @SDD_ID LIN_SDD_355
* @endif
*
* @local_var
*   -# <B>#sci</B> <I>l_u8</I>
*
* @static_global_var
*   -# <B>#tl_current_rx_pdu_ptr</B>
*
* @see lin_tl_make_slaveres_pdu

*
* @details
*   This function make a connection to Diag service class that support for LIN function
*   This function is implemented for Slave only
*//*END*----------------------------------------------------------------------*/
void lin_tl_attach_service(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_tl_get_pdu()
* @brief This function copy the current PDU to Response Buffer in Low-level
*
*
* @return #void
*
* @SDD_ID LIN_SDD_351
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>i</I>
*
* @static_global_var
*   -# <B>#lin_lld_response_buffer[9]</B>
*   -# <B>#tl_current_tx_pdu_ptr</B>
*
* @details
*   This function copy the current PDU to Response Buffer in Low-level
*   This function is implemented for Slave and Master
*//*END*----------------------------------------------------------------------*/
void lin_tl_get_pdu(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_tl_put_pdu()
* @brief This function put response buffer (in Low-level ) to PDU data on the Transport Layer
*
*
* @return #void
*
* @SDD_ID LIN_SDD_352
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>i</I>
*
* @static_global_var
*   -# <B>#lin_lld_response_buffer</B>
*   -# <B>#tl_current_rx_pdu_ptr</B>
*   -# <B>#rx_single_pdu_data</B>
*
* @details
*   This function put response buffer (in Low-level ) to PDU data on the Transport Layer
*   Call in callback function
*   This function is implemented for Slave and Master
*//*END*----------------------------------------------------------------------*/
void lin_tl_put_pdu(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_tl_handler()
* @brief This function handles the diag service and control event and data in Transport Layer
*
*
* @return #void
*
* @SDD_ID LIN_SDD_353
* @endif
*
* @local_var
*   -# <B>#pci_type</B> <I>l_u8</I>
*   -# <B>#length</B> <I>l_u8</I>
*
* @static_global_var
*   -# <B>#lin_initial_NAD[9]</B>
*   -# <B>#tl_current_rx_pdu_ptr</B>
*   -# <B>#lin_configured_NAD</B>
*   -# <B>#lin_tl_attach_service</B>
*
* @details
*   This function handles the diag service and control event and data in TL
*   This function is implemented for Slave and Master
*//*END*----------------------------------------------------------------------*/
void lin_tl_handler(void);

#endif /* End (_LIN_GPIO_ == 0) && !defined(_MC9S08SC4_H) */
#endif /* End (LIN_MODE == _SLAVE_MODE_)*/


#if LIN_MODE == _MASTER_MODE_

extern l_u8 ld_read_by_id_callout(l_ifc_handle iii, l_u8 id, l_u8 *data);


/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_tl_make_mreq_pdu(l_ifc_handle iii)
* @brief This function will be called on Master Task in LIN protocol, it create a pointer to PDU data
*
* @param iii <B>[IN]</B> interface name
* @return #void
*
* @SDD_ID LIN_SDD_308
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>tl_conf</I>
*
* @static_global_var
*   -# <B>#tl_current_rx_pdu_ptr</B>
*
* @see lin_tl_make_slaveres_pdu
*
* @details
*   This function will be called on Master Task in LIN protocol, it create a pointer to PDU data
*   which available for Master request commmand, correspond to Diag frame to send
*   This function is implemented for Master
*//*END*----------------------------------------------------------------------*/
/* multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
void lin_tl_make_mreq_pdu(l_ifc_handle iii);
#else /*Single frame support */
void lin_tl_make_mreq_pdu(l_ifc_handle iii,lin_tl_pdu_data *pdu_ptr);
#endif /*End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_tl_get_response(l_ifc_handle iii)
* @brief This function check result of service, it is called when the Slave Response Frame received
*
* @param iii <B>[IN]</B> interface name
*
* @return #void
*
* @SDD_ID LIN_SDD_315
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>*conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>*tl_conf</I>
*   -# <B>#lin_tl_pdu_data</B> <I>*lin_tl_pdu</I>
*   -# <B>#l_u8</B> <I>sci</I>*
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
*
* @details
*   This function check result of service, it is called when the Slave Response Frame received
*   This function is implemented for Master only
*//*END*----------------------------------------------------------------------*/
void lin_tl_get_response(l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_tl_make_slaveres_pdu(l_ifc_handle iii, l_u8 sid, l_u8 res_type, l_u8 error_code)
* @brief This function is implemented for Slave only
*
* @param iii <B>[IN]</B> interface name
* @param sid <B>[IN]</B> service identifier
* @param res_type <B>[IN]</B> type of response
* @param error_code <B>[IN]</B> Error code in case of negative response, if positive = 0
*
* @return #void
*
* @SDD_ID LIN_SDD_309
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>tl_conf</I>
*   -# <B>#lin_tl_pdu_data</B> <I>lin_tl_pdu</I>
*   -# <B>#lin_node_attribute</B> <I>node_conf</I>
*   -# <B>#lin_product_id</B> <I>ident</I>
*   -# <B>#l_u8</B> <I>i</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
*
* @details
*   This function is implemented for Slave only
*//*END*----------------------------------------------------------------------*/
void lin_tl_make_slaveres_pdu(l_ifc_handle iii, l_u8 sid, l_u8 res_type, l_u8 error_code);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_tl_attach_service(l_ifc_handle iii)
* @brief This function make a connection to Diag service class that support for LIN function
*
* @param iii <B>[IN]</B> interface name
*
* @return #void
*
* @SDD_ID LIN_SDD_314
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>*conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>*tl_conf</I>
*   -# <B>#lin_transport_layer_queue</B> <I>*tl_queue</I>
*   -# <B>#lin_tl_pdu_data</B> <I>*lin_tl_pdu</I>
*   -# <B>#l_u8</B> <I>sci</I>*
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
*
* @details
*   This function make a connection to Diag service class that support for LIN function
*   This function is implemented for Slave only
*//*END*----------------------------------------------------------------------*/
void lin_tl_attach_service(l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_tl_get_pdu(l_ifc_handle iii)
* @brief This function copy the current PDU to Response Buffer in Low-level
*
* @param iii <B>[IN]</B> interface name
*
* @return #void
*
* @SDD_ID LIN_SDD_311
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#lin_configuration</B> <I>*conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>*tl_conf</I>
*   -# <B>#lin_tl_pdu_data</B> <I>lin_tl_pdu</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @see tl_get_raw
* @details
*   This function copy the current PDU to Response Buffer in Low-level
*   This function is implemented for Slave and Master
*//*END*----------------------------------------------------------------------*/
void lin_tl_get_pdu(l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_tl_handler(l_ifc_handle iii)
* @brief This function handles the diag service and control event and data in TL
*
* @param iii <B>[IN]</B> interface name
*
* @return #void
*
* @SDD_ID LIN_SDD_313
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>pci_type</I>
*   -# <B>#l_u8</B> <I>length</I>
*   -# <B>#lin_configuration</B> <I>*conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>*tl_conf</I>
*   -# <B>#lin_tl_pdu_data</B> <I>*lin_tl_pdu</I>
*   -# <B>#lin_node_attribute</B> <I>*node_conf</I>
*
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @see lin_process_pdu
*
* @details
*   This function handles the diag service and control event and data in TL
*   This function is implemented for Slave and Master
*//*END*----------------------------------------------------------------------*/

void lin_tl_handler(l_ifc_handle iii);

/* multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_tl_handler_error(l_ifc_handle iii, l_u8 pid)
* @brief This function handles the diag service and control error in TL
*
* @param iii <B>[IN]</B> interface name
* @param pid <B>[IN]</B> interface name
*
* @return #void
*
* @SDD_ID LIN_SDD_328
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>*conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>*tl_conf</I>
*
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
*
* @details
*   This function handles the diag service and control error in TL
*   This function is implemented for Slave and Master
*//*END*----------------------------------------------------------------------*/
void lin_tl_handler_error(l_ifc_handle iii, l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void tl_process_mreq(l_ifc_handle iii)
* @brief This function is process transport layer
* @param iii <B>[IN]</B> interface name
* @return #void
*
* @SDD_ID LIN_SDD_326
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
* @see lin_tl_make_slaveres_pdu
*
* @details
*   This function is process transport layer
*//*END*----------------------------------------------------------------------*/
void tl_process_mreq(l_ifc_handle iii);
#else /* Single frame support */

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_tl_no_response(l_ifc_handle iii)
* @brief set the result of service
*
* @param iii <B>[IN]</B> interface name
*
* @return #void
*
* @SDD_ID LIN_SDD_316
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>*conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   This function set the result of service, it is called when the master has no Slave Response Frame. <br />
*   This function is implemented for Master only
*//*END*----------------------------------------------------------------------*/
void lin_tl_no_response(l_ifc_handle iii);
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_tl_update_tx_flag(l_ifc_handle iii)
* @brief update flags related to Diag frame when received and transmited successful
*
* @param iii <B>[IN]</B> interface name
*
* @return #void
*
* @SDD_ID LIN_SDD_310
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>*conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>*tl_conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   This function update flags related to Diag frame when received and transmited successful. <br />
*   This function is implemented for Slave and Master
*//*END*----------------------------------------------------------------------*/
void lin_tl_update_tx_flag(l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_tl_put_pdu(l_ifc_handle iii)
* @brief put response buffer (in Low-level ) to PDU data on the Transport Layer
*
* @param iii <B>[IN]</B> interface name
*
* @return #void
*
* @SDD_ID LIN_SDD_312
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>*conf</I>
*   -# <B>#lin_tl_descriptor</B> <I>*tl_conf</I>
*   -# <B>#lin_tl_pdu_data</B> <I>*lin_tl_pdu</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   This function put response buffer (in Low-level ) to PDU data on the Transport Layer
*   Call in callback function
*   This function is implemented for Slave and Master
*//*END*----------------------------------------------------------------------*/
void lin_tl_put_pdu(l_ifc_handle iii);


#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)*/
#endif /* End (LIN_MODE == _MASTER_MODE_)*/

#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
/* COMMON FUNCTIONS */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void tl_put_raw(const l_u8* const data, lin_transport_layer_queue *queue, l_u8 direction)
* @brief The call copies the raw data specified by data to the queue specified by queue.
*
* @param data <B>[IN]</B> buffer for the data
* @param *queue  queue for containing data
* @param direction  Direction
*
* @return #void
*
* @SDD_ID LIN_SDD_303
* @endif
*
* @local_var
*   -# <B>#lin_tl_pdu_data</B> <I>*queue_data</I>
*   -# <B>#l_u8</B> <I>i</I>
*
* @static_global_var
*
* @details
*   The call copies the raw data specified by data to the queue specified by queue.
*//*END*----------------------------------------------------------------------*/
void tl_put_raw(const l_u8* const data,lin_transport_layer_queue *queue,l_u8 direction);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void tl_get_raw(l_u8* const data, lin_transport_layer_queue *queue, l_u8 direction)
* @brief
*   The call gets the raw data from the queue specified by queue to
*   the raw data specified by data
*   specified by queue.
*
* @param data <B>[OUT]</B> buffer for store data received
* @param *queue queue for containing data
* @param direction Direction for transmission
*
* @return #void
*
* @SDD_ID LIN_SDD_304
* @endif
*
* @local_var
*   -# <B>#lin_transport_layer_queue</B> <I>*tl_queue</I>
*   -# <B>#lin_tl_pdu_data</B> <I>*queue_data</I>
*   -# <B>#l_u8</B> <I>i</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
*
* @details
*   The call gets the raw data from the queue specified by queue to
*   the raw data specified by data
*   specified by queue.
*//*END*----------------------------------------------------------------------*/
void tl_get_raw(l_u8* const data,lin_transport_layer_queue *queue,l_u8 direction);
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
#endif /* end of_LIN_COMMONTL_PROTO_H */ 
/** @} */
