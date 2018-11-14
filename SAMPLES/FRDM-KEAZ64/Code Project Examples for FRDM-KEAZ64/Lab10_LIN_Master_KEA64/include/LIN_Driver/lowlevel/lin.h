/****************************************************************************** 
* Freescale Semiconductor Inc. 
* (c) Copyright 2008-2009 Freescale Semiconductor, Inc. 
* ALL RIGHTS RESERVED. 
*****************************************************************************/ 

/**************************************************************************//**
* @addtogroup lowlevel_group
* @{
******************************************************************************/

/**************************************************************************//** 
* 
* @file      lin.h 
* 
* @author    FPT Software
*  
* @brief     LIN low level functions
* 
* 
******************************************************************************/
/******************************************************************************
 *
 * History:
 *
 * 20090408		v1.0	First version
 * 20111005		v1.1	Added some defines and extern functions
 *
 *****************************************************************************/
 
#ifndef _LIN_H
#define _LIN_H

#include "lin_cfg/lin_cfg.h"
#include "lin_cfg/lin_hw_cfg.h"

#if (_LIN_SCI_ == 1)|(_LIN_UART_ == 1)
#include "LIN_Driver/bsp/UART/lin_reg.h"
/* Disable warning: SP debug info */
#if (_MCU_ == _S12X_)
//#pragma MESSAGE DISABLE C12056 /* Disable warning message with ID C12056 */
#endif /* End (_MCU_ == _S12X_) */
#endif /* End (_LIN_SCI_ == 1)|(_LIN_UART_ == 1) */

//#pragma MESSAGE DISABLE C5703 /* Disable warning message with ID C5703 */

#ifndef _DIAG_CLASS_SUPPORT_
  #error "_DIAG_CLASS_SUPPORT_ is not defined in lin_cfg.h"
#endif
#ifndef _TL_FRAME_SUPPORT_
  #error "_TL_FRAME_SUPPORT_ is not defined in lin_cfg.h"
#endif

#if (_LIN_GPIO_ == 0) && !defined(_MC9S08SC4_H)
/* Calculate N_As & N_Cr max timeout */
#define N_MAX_TIMEOUT_CNT ((l_u16)(1000*(1000/TIME_BASE_PERIOD)))
#endif /* End (_LIN_GPIO_ == 0) && !defined(_MC9S08SC4_H) */

/* Define data structure used for LIN driver */

/**************************************************************************//**
*
* @var typedef unsigned char l_u8
* lin unsigned char data type
* @var typedef signed char l_s8
* lin signed char data type
* @var typedef volatile unsigned char l_vu8
* lin volatile unsigned char data type
* @var typedef volatile signed char l_vs8
* lin volatile signed char data type
*
* @var typedef unsigned int l_u16
* lin unsigned int data type
* @var typedef signed int l_s16
* lin signed int data type
* @var typedef volatile unsigned int l_vu16
* lin volatile unsigned int data type
* @var typedef volatile signed int l_vs16
* lin volatile signed int data type
*
* @var typedef unsigned long l_u32
* lin unsigned long data type
* @var typedef signed long l_s32
* lin signed long data type
* @var typedef volatile unsigned long l_vu32
* lin volatile unsigned long data type
* @var typedef volatile signed long l_vs32
* lin volatile signed long data type
*
* @var typedef unsigned char l_bool
* lin bool data type (true or false)
******************************************************************************/
typedef signed   char l_s8;
typedef unsigned char l_u8;

typedef volatile signed   char l_vs8;
typedef volatile unsigned char l_vu8;

typedef signed int l_s16;
typedef unsigned int l_u16;

typedef volatile signed int l_vs16;
typedef volatile unsigned int l_vu16;

typedef signed long l_s32;
typedef unsigned long l_u32;

typedef volatile signed long l_vs32;
typedef volatile unsigned long l_vu32;

typedef unsigned char l_bool;

#define MAKE_PARITY   0					/**< make parity bits */
#define CHECK_PARITY  1					/**< check parity bits */

/* Define diagnostic services id */
#define SERVICE_ASSIGN_NAD                0xB0      /**< assign NAD service */
#define SERVICE_ASSIGN_FRAME_ID           0xB1      /**< assign frame id service */
#define SERVICE_READ_BY_IDENTIFY          0xB2      /**< read by identify service */
#define SERVICE_CONDITIONAL_CHANGE_NAD    0xB3      /**< conditional change NAD service */
#define SERVICE_DATA_DUMP                 0xB4      /**< data dump service */
#define SERVICE_ASSIGN_NAD_VIA_SNPD       0xB5      /**< assign NAD via SN service */
#define SERVICE_SAVE_CONFIGURATION        0xB6      /**< save configuration service */
#define SERVICE_ASSIGN_FRAME_ID_RANGE     0xB7      /**< assign frame id range service */

#define SERVICE_READ_DATA_BY_IDENTIFY     0x22      /**< service read data by identifier */
#define SERVICE_WRITE_DATA_BY_IDENTIFY    0x2E      /**< service write data by identifier */
#define SERVICE_SESSION_CONTROL           0x10      /**< service session control */
#define SERVICE_IO_CONTROL_BY_IDENTIFY    0x2F      /**< service I/O control */
#define SERVICE_FAULT_MEMORY_READ         0x19      /**< service fault memory read */
#define SERIVCE_FAULT_MEMORY_CLEAR        0x14      /**< service fault memory clear */

/* Define PCI's services */
#define PCI_ASSIGN_NAD              	  0x06      /**< PCI value assign NAD */
#define PCI_READ_BY_IDENTIFY        	  0x06      /**< PCI value read by identify */
#define PCI_CONDITIONAL_CHANGE_NAD  	  0x06      /**< PCI value conditional change NAD */
#define PCI_DATA_DUMP               	  0x06      /**< PCI value data dump */
#define PCI_SAVE_CONFIGURATION      	  0x01      /**< PCI value save configuration */
#define PCI_ASSIGN_FRAME_ID_RANGE   	  0x06      /**< PCI value assign frame id range */

/* Define PCI response's services */
#define PCI_RES_ASSIGN_NAD                0x01      /**< PCI response value assign NAD */
#define PCI_RES_READ_BY_IDENTIFY          0x06      /**< PCI response value read by identify */
#define PCI_RES_CONDITIONAL_CHANGE_NAD    0x01      /**< PCI response value conditional change NAD */
#define PCI_RES_DATA_DUMP                 0x06      /**< PCI response value data dump */
#define PCI_RES_SAVE_CONFIGURATION        0x01      /**< PCI response value save configuration */
#define PCI_RES_ASSIGN_FRAME_ID_RANGE     0x01      /**< PCI response value assign frame id range */

/* Identifire user defined */
#define LIN_READ_USR_DEF_MIN              32U       /**< Min user defined */
#define LIN_READ_USR_DEF_MAX              63U       /**< Max user defined */

/* Identification */
#define LD_ID_NO_RESPONSE                 0x52      /**< Positive response */
#define LD_NEGATIVE_RESPONSE              0x53      /**< Negative response */
#define LD_POSITIVE_RESPONSE              0x54      /**< Positive response */



/**
* @var typedef l_u8 lin_tl_pdu_data[8]
* PDU data
*/
typedef l_u8 lin_tl_pdu_data[8];

/**********************************************************************/
/***************        LOW LEVEL DRIVER            *******************/
/**********************************************************************/

/**
* @enum lin_checksum_type
* Checksum type
*/
typedef enum {
  NORMAL_CHECKSUM, /**< NORMAL_CHECKSUM */
  ENHANCED_CHECKSUM /**< ENHANCED_CHECKSUM */
} lin_checksum_type;

/**
* @enum lin_lld_event_id
* Event id
*/
typedef enum {
	LIN_LLD_PID_OK,         /**< LIN_LLD_PID_OK */
	LIN_LLD_TX_COMPLETED,   /**< LIN_LLD_TX_COMPLETED */
	LIN_LLD_RX_COMPLETED,   /**< LIN_LLD_RX_COMPLETED */
	LIN_LLD_PID_ERR,        /**< LIN_LLD_PID_ERR */
	LIN_LLD_FRAME_ERR ,     /**< LIN_LLD_FRAME_ERR */
	LIN_LLD_CHECKSUM_ERR,   /**< LIN_LLD_CHECKSUM_ERR */
	LIN_LLD_READBACK_ERR,   /**< LIN_LLD_READBACK_ERR */
	LIN_LLD_NODATA_TIMEOUT,       /**< Nodata timeout */
	LIN_LLD_BUS_ACTIVITY_TIMEOUT  /**< LIN_LLD_BUS_ACTIVITY_TIMEOUT */
} lin_lld_event_id;

/**
* @enum lin_lld_mode
* Low level driver mode
*/
typedef enum {
  LIN_LLD_OK,               /**< OK */
  LIN_LLD_INVALID_MODE,     /**< Invalid mode */
  LIN_LLD_INVALID_ID,       /**< invalid ID */
  LIN_LLD_NO_ID,            /**< No ID */
  LIN_LLD_INVALID_TIMEBASE, /**< Invalid timebase */
  LIN_LLD_INVALID_PARA,     /**< Invalid parity */
  LIN_LLD_INVALID_IFC       /**< Invalid interface */
} lin_lld_mode;

#if ((_LIN_SCI_ == 1)|(_LIN_UART_ == 1))

#define IDLE                0x00          /**< IDLE state */
#define SEND_BREAK          0x01          /**< Send break field state */
#define SEND_PID            0x02          /**< send PID state */
#define RECV_SYN            0x03          /**< receive synchronize state */
#define RECV_PID            0x04          /**< receive PID state */
#define IGNORE_DATA         0x05          /**< ignore data state */
#define RECV_DATA           0x06          /**< receive data state */
#define SEND_DATA           0x07          /**< send data state */
#define SEND_DATA_COMPLETED 0x08          /**< send data completed state */
#define PROC_CALLBACK       0x09          /**< proceduce callback state */
#define SLEEP_MODE          0x0A          /**< sleep mode state */
#define UNINIT              0xFF          /**< uninitialize state */

/**
* @var typedef l_u16 sci_channel_name
*
* @details
*   SCI channel
*/
typedef l_u16 sci_channel_name;
/**
* @var typedef l_u16 uart_channel_name
*
* @details
*   UART channel
*/
#if (_LIN_UART_ == 1)
typedef l_u16 uart_channel_name;
#endif /* (_LIN_UART_ == 1) */
/**
* @union lin_status
* status of lin driver
*/
typedef union {
  l_u8 byte; /**< a data byte refer to 8 data bits follow */
  struct 
  {
    /* LIN 2.1 */
    l_u8 successful_transfer:1;     /**< Transfer flag LIN 2.1*/
    l_u8 error_in_response:1;       /**< Error response LIN 2.1*/
    l_u8 bus_activity;              /**< Bus activity timeout LIN 2.1*/
    /* J2602 */
    l_u8 framing_error:1;           /**< frame error flag J2602*/
    l_u8 checksum_error:1;          /**< checksum error flag */
    l_u8 readback_error:1;          /**< readback error in J2602 to be called Data Error */
    l_u8 parity_error:1;            /**< frame error flag */
    l_u8 reset:1;                   /**< reset flag (not implemented) */
  }bit;                                             
} lin_status;

#if (LIN_MODE == _MASTER_MODE_)
/* lin_node struct */
/**
* @struct lin_node
* contain informations of a lin node
*/
typedef struct {
#if (_LIN_SCI_ == 1)
	tSCI* pSCI;	                /**< pointer to the SCI peripheral */
#endif/* (_LIN_SCI_ = 1) */
#if (_LIN_UART_ == 1)
	tUART* pUART;	                /**< pointer to the UART peripheral */
#endif /* (_LIN_UART_ = 1) */
	l_bool func;                /**< LIN Function : Master=0 or Slave=1 */
	l_u8 ifc;                   /**< LIN core interface number */
	l_u8 state;                 /**< LIN-SCI operational state */
	lin_status l_status;        /**< LIN status */
	l_u8 cnt_byte;              /**< Byte counter used when either transmit and receive data */                    
	l_u8 *ptr;                  /**< pointer to Data out/in */
	l_u8 current_id;            /**< Current ID */
	l_u8 *response_buffer;      /**< Frame data pointer */
	l_u8 pid;                   /**< PID to send out */
	l_u16 tbit;                  /**< Tbit */   
	l_u16 frame_timeout_cnt;     /**< Frame timeout counter */
	l_u16 res_frame_timeout_cnt; /**< ResponseFrame timeout counter */
	l_u16 idle_timeout_cnt;	    /**< Idle timeout counter */
} lin_node;
#endif /* End (LIN_MODE == _MASTER_MODE_) */

#endif /* End (_LIN_SCI_ == 1) */

/**********************************************************************/
/***************             Protocol               *******************/
/**********************************************************************/

/**
* @enum lin_protocol_handle
* List of protocols
*/
typedef enum {
  LIN_PROTOCOL_21,    /**< LIN protocol version 2.1 */
  LIN_PROTOCOL_20,    /**< LIN protocol version 2.0 */
  LIN_PROTOCOL_J2602  /**< J2602 protocol  */
} lin_protocol_handle;

/**
* @typedef lin_word_status_str
* status of lin bus
* @union wstatus
* word status
*/
typedef union wstatus {                    
  l_u16 word; /**< this is a word, refer to 16 data bits follow*/
  /** 
  * @struct bit
  * 
  */
  struct { 
	  l_u16 last_pid:8;                   /**< Last PID */
	  l_u16 error_in_res:1;               /**< Error in response */
	  l_u16 successful_transfer:1;        /**< Successful transfer */
	  l_u16 overrun:1;                    /**< Overrun */
	  l_u16 gotosleep:1;                  /**< Goto sleep */
	  l_u16 bus_activity:1;               /**< Bus activity */
	  l_u16 etf_collision:1;              /**< Event trigger collision */
	  l_u16 save_conf:1;                  /**< Save configuration */
	  l_u16 dummy:1;                      /**< Dummy */
  }bit; /**< these bits contain data of status */
} lin_word_status_str;


/**********************************************************************/
/***************             Interfaces             *******************/
/**********************************************************************/
#if (LIN_MODE == _MASTER_MODE_)
/**
* @enum lin_function
* function of lin node (master or slave)
*/
typedef enum {
  _MASTER_,     /**< master node */
  _SLAVE_       /**< slave node */
} lin_function;
extern l_ifc_handle lin_diag_interface;
#endif /* End (LIN_MODE == _MASTER_MODE_) */

/**********************************************************************/
/***************             Signals                *******************/
/**********************************************************************/

/**
* @enum lin_signal_type
* list of signal types
*/
typedef enum {
	LIN_SIG_SCALAR,       /**< scalar signal */
	LIN_SIG_ARRAY         /**< array signal */
} lin_signal_type;        

/**
* @enum lin_diagnostic_signal
* List of diagnostic signals
*/
typedef enum {
  MasterReqB0, /**< diagnostic master request bit 0 */
  MasterReqB1, /**< diagnostic master request bit 1 */
  MasterReqB2, /**< diagnostic master request bit 2 */
  MasterReqB3, /**< diagnostic master request bit 3 */
  MasterReqB4, /**< diagnostic master request bit 4 */
  MasterReqB5, /**< diagnostic master request bit 5 */
  MasterReqB6, /**< diagnostic master request bit 6 */
  MasterReqB7, /**< diagnostic master request bit 7 */
  SlaveRespB0, /**< diagnostic slave response bit 0 */
  SlaveRespB1, /**< diagnostic slave response bit 1 */
  SlaveRespB2, /**< diagnostic slave response bit 2 */
  SlaveRespB3, /**< diagnostic slave response bit 3 */
  SlaveRespB4, /**< diagnostic slave response bit 4 */
  SlaveRespB5, /**< diagnostic slave response bit 5 */
  SlaveRespB6, /**< diagnostic slave response bit 6 */
  SlaveRespB7  /**< diagnostic slave response bit 7 */
} lin_diagnostic_signal;

/**********************************************************************/
/***************             General                *******************/
/**********************************************************************/

/* Node atrribute */
/**
* @struct lin_product_id
* product id structure
*/
typedef struct {
  l_u16 supplier_id;        /**< Supplier ID */
  l_u16 function_id;        /**< Function ID */
  l_u8 variant;             /**< Variant value */
} lin_product_id;

/**
* @struct lin_node_attribute
* attributies of lin node
*/
typedef struct {
  lin_protocol_handle lin_protocol;           /**< LIN protocol used in node */
  l_u8 configured_NAD;                        /**< NAD value used in configuration command */
  l_u8 initial_NAD;                           /**< Intial NAD */
  lin_product_id product_id;                  /**< Product ID */
  l_signal_handle response_error;             /**< Signal used to update response error */
  l_u8 response_error_byte_offset;            /**< Byte offset of response error signal */
  l_u8 response_error_bit_offset;             /**< Bit offset of response error signal */
  l_u8 num_of_fault_state_signal;             /**< Num of Fault state signal */
  const l_signal_handle *fault_state_signal;  /**< List of fault state signal */
  l_u16 P2_min;                               /**< P2 min */
  l_u16 ST_min;                               /**< ST min */
  l_u16 N_As_timeout;                         /**< N_As_timeout */
  l_u16 N_Cr_timeout;                         /**< N_Cr_timeout */
} lin_node_attribute; 

/**********************************************************************/
/***************             Frame                  *******************/
/**********************************************************************/

/**
* @enum lin_frame_type
* types of frame
*/
typedef enum {
	LIN_FRM_UNCD  = 0x00,                   /**< Unconditional frame */
	LIN_FRM_EVNT  = 0x01,           	    /**< Event triggered frame.*/
	LIN_FRM_SPRDC = 0x10,                   /**< Sporadic frame.*/
	LIN_FRM_DIAG  = 0x11                    /**< Diagnostic frame.*/
} lin_frame_type;

/**
* @enum lin_frame_response
* lin frame response
*/
typedef enum {
	LIN_RES_NOTHING = 0x00,    	            /**< This frame does not require to response.*/
	LIN_RES_PUB = 0x01,          	        /**< Publisher response.*/
	LIN_RES_SUB = 0x10              	    /**< Subscriber response.*/
} lin_frame_response;

/**
* @struct lin_frame_struct
* Informations of frame
*/
typedef struct  {
	lin_frame_type      frm_type; 		    /**< Frame information (unconditional or event triggered..) */
	l_u8                frm_len; 			    /**< Length of the frame */
	lin_frame_response  frm_response;     /**< action response when received PID */
	l_u8                frm_offset;       /**< Frame byte offset in frame buffer */
	l_u8                flag_offset;      /**< Flag byte offset in flag buffer */
	l_u8                flag_size;        /**< Flag size in flag buffer */
	l_u8                *frame_data;      /**< List of Signal to which the frame is associated and its offset */
} lin_frame_struct;

/**
* @struct lin_associate_frame_struct
* Informations of associated frame
*/
typedef struct  {
	l_u8                num_asct_uncn_pid;  /**< Number of associated unconditional frame ID*/
	const l_u8*         act_uncn_frm;       /**< Associated unconditional frame ID. */
#if LIN_MODE == _MASTER_MODE_
	l_u8                coll_resolver_id;  	/**< Collision resolver index in the schedule table, used in event trigger frame case MASTER*/
#endif /* End LIN_MODE == _MASTER_MODE_ */
} lin_associate_frame_struct;

/**********************************************************************/
/***************             Schedule table         *******************/
/**********************************************************************/

/* Structures of Diagnostic queue */
/**
* @typedef l_u8 lin_tl_queue[8]
* lin transport layer queue
*/
typedef l_u8 lin_tl_queue[8];   

#if (LIN_MODE == _MASTER_MODE_)

/**
* @enum lin_sch_tbl_type
* Type of schedule table
*/
typedef enum {
	LIN_SCH_TBL_NULL,	                    /**< Run nothing */
	LIN_SCH_TBL_NORM,	                    /**< Normal schedule table*/ 
	LIN_SCH_TBL_DIAG,                     	/**< Diagnostic schedule table */
	LIN_SCH_TBL_GOTO,                     	/**< Goto sleep schedule table */
	LIN_SCH_TBL_COLL                      	/**< Collision schedule table */
} lin_sch_tbl_type;

/**
* @struct lin_schedule_data
* lin schedule structure
*/
typedef struct  {
	l_frame_handle      frm_id;         	/**< Frame ID, in case of unconditional or event triggered frame. For sporadic frame the value will be 0 (zero). */
	l_u8                delay_integer;		/**< Actual slot time in INTEGER for one frame.*/
	lin_tl_queue        tl_queue_data;      /**< Data used in case of diagnostic or configuration frame*/
} lin_schedule_data;

/**
* @struct lin_schedule_struct
* Informations of schedule
*/
typedef struct  {
	l_u8                      num_slots;          /**< Number of frame slots in the schedule table.*/
	lin_sch_tbl_type          sch_tbl_type; 	  /**< Schedule table type (configuration and normal)*/
	const lin_schedule_data   *ptr_sch_data;      /**< Address of the schedule table.*/
} lin_schedule_struct;

#endif /* End (LIN_MODE == _MASTER_MODE_) */

/**********************************************************************/
/***************      Transport Layer /Diagnostic      ****************/
/**********************************************************************/

/* Diagnostic mode */
/**
* @enum l_diagnostic_mode
* Diagnostic mode
*/
#if (XGATE_SUPPORT == 1)
#pragma align  on
#endif /* End (XGATE_SUPPORT == 1) */
typedef enum {
  DIAG_NONE,              /**< diagnostic none */
  DIAG_INTER_LEAVE_MODE,  /**< diagnostic interleave mode */
  DIAG_ONLY_MODE          /**< diagnostic only mode */
} l_diagnostic_mode;
#if (XGATE_SUPPORT == 1)
#pragma align  off
#endif /* End (XGATE_SUPPORT == 1) */

/**
* @enum lin_service_status
* Status of the last configuration call for LIN 2.1
*/
#if (XGATE_SUPPORT == 1)
#pragma align  on
#endif /* End (XGATE_SUPPORT == 1) */
typedef enum {
  LD_SERVICE_BUSY,        /**< Service is ongoing */
  LD_REQUEST_FINISHED,    /**< The configuration request has been completed */
  LD_SERVICE_IDLE,        /**< The configuration  request/response combination has been completed*/
  LD_SERVICE_ERROR       /**< The configuration request or  response experienced an error */
} lin_service_status;
#if (XGATE_SUPPORT == 1)
#pragma align  off
#endif /* End (XGATE_SUPPORT == 1) */

/**
* @enum lin_last_cfg_result
* Status of the last configuration call completed for J2602
*/
typedef enum {
  LD_SUCCESS,             /**< The service was successfully carried out */
  LD_NEGATIVE,            /**< The service failed, more information can be found by parsing error_code */
  LD_NO_RESPONSE,         /**< No response was received on the request */
  LD_OVERWRITTEN          /**< The slave response frame has been overwritten by another operation */
} lin_last_cfg_result;

/* TL support */

/*------------------------ Transport layer multi frames -----------------------*/
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
/* TL Descriptor used in the TL/Diagnostic  */

#define MAX_QUEUE_SIZE 20       /**< maximum size of queue */
#define SF             0x00     /**< single frame */
#define FF             0x01     /**< first frame */
#define CF             0x02     /**< consecutive frame */

/**
* @enum ld_queue_status
* status of queue
*/
typedef enum {                           
  LD_NO_DATA,         /**< Queue is empty, has no data */
  LD_DATA_AVAILABLE,  /**< data in queue is available */
  LD_RECEIVE_ERROR,   /**< receive data is error */
  LD_QUEUE_FULL,      /**< the queue is full */
  LD_QUEUE_AVAILABLE, /**< queue is available for insert data */
  LD_QUEUE_EMPTY,     /**< queue is empty */
  LD_TRANSMIT_ERROR   /**< error while transmitting */
} ld_queue_status;

/**
* @enum lin_message_status
* status of lin message
*/
typedef enum { 
  LD_NO_MSG,            /**< no message */               
  LD_IN_PROGRESS,       /**< in progress */
  LD_COMPLETED,         /**< completed */
  LD_FAILED,            /**< failed */
  LD_N_AS_TIMEOUT,      /**< N_As timeout */
  LD_N_CR_TIMEOUT,      /**< N_Cr timeout */
  LD_WRONG_SN           /**< wrong sequence number */
} lin_message_status;

/**
* enum lin_diagnostic_state
* lin diagnostic state
*/
typedef enum {
  LD_DIAG_IDLE,             /**< Diagnostic IDLE */
  LD_DIAG_TX_ACTIVE,        /**< Diagnostic transmit active */
  LD_DIAG_TX_PHY,           /**< Diagnostic transmit physical */
  LD_DIAG_INTERLEAVED_TX,   /**< Diagnostic transmit in interleave mode */
  LD_DIAG_RX_PHY,           /**< Diagnostic receive in physical */
  LD_DIAG_INTERLEAVED_RX,   /**< Diagnostic receive in interleave mode */
  LD_DIAG_RX_FUNCTIONAL     /**< Diagnostic receive functional request */
} lin_diagnostic_state;

/**
* @enum lin_message_timeout_type
* types of message timeout
*/
typedef enum {
  LD_NO_CHECK_TIMEOUT,      /**< No check timeout */
  LD_CHECK_N_AS_TIMEOUT,    /**< check N_As timeout */
  LD_CHECK_N_CR_TIMEOUT     /**< check N_Cr timeout */
} lin_message_timeout_type;

/**
* @struct lin_transport_layer_queue
* transport layer queue
*/
typedef struct {
  l_u16                     queue_header;                         /**< the first element of queue */
  l_u16                     queue_tail;                           /**< the last element of queue */
  ld_queue_status           queue_status;                         /**< status of queue */
  l_u16                     queue_current_size;                   /**< current size */
  const l_u16               queue_max_size;                       /**< maximume size */
  lin_tl_pdu_data           *tl_pdu;                              /**< PDU data */
} lin_transport_layer_queue;

#else /* single frame support */
#define MAX_QUEUE_SIZE 1       /**< maximum size of queue */
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */


#if (LIN_MODE == _MASTER_MODE_)
/* Multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
#define INTERLEAVE_MAX_TIMEOUT      15
/**
* @struct lin_tl_descriptor
* transport layer description
*/
#if (XGATE_SUPPORT == 1)
#pragma align  on
#endif /* End (XGATE_SUPPORT == 1) */
typedef struct {
  /* Declaration for both Master and Slave interface */
  lin_transport_layer_queue     *tl_tx_queue;                     /**< pointer to transmit queue on TL */
  lin_transport_layer_queue     *tl_rx_queue;                     /**< pointer to receive queue on TL */
  
  /* Declaration only for Master interface */
  /* message information in transmit queue */
  lin_message_status            tl_rx_msg_status;                 /**< cooked rx status */
  l_u16                         tl_rx_msg_index;                  /**< index of message in queue */
  l_u16                         tl_rx_msg_size;                   /**< Size of message in queue */
  lin_message_status            tl_receive_msg_status;            /**< receive message status */

  /* message information in receive queue */
  lin_message_status            tl_tx_msg_status;                 /**< cooked tx status */
  l_u16                         tl_tx_msg_index;                  /**< index of message in queue */
  l_u16                         tl_tx_msg_size;                   /**< Size of message in queue */

  lin_last_cfg_result           tl_last_cfg_result;               /**< Status of the last configuration service in LIN 2.0, J2602 */
  l_u8                          tl_last_RSID;                     /**< RSID of the last node configuration service */
  l_u8                          tl_ld_error_code;                 /**< Error code in case of positive response */
  
  l_u8                          tl_no_of_pdu;                     /**< number of received pdu */
  l_u8                          tl_frame_counter;                 /**< frame counter in received message */
  
  lin_message_timeout_type      tl_check_timeout_type;            /**< timeout type */
  l_u16                         tl_check_timeout;                 /**< timeout counter*/
  
  l_u8                          *tl_ident_data;                   /**< To store address of RAM area contain response */
  
  l_u8                          tl_interleave_timeout_counter;    /**< interleaved timeout counter */
  l_u8                          *tl_service_flags;                /**< diagnostic service flags */
  
  /* Declaration only for Slave interface */
  l_u8                          tl_slaveresp_cnt;                 /**< Slave Response data counter */
} lin_tl_descriptor;

/**
* @struct lin_tl_descriptor1
* transport layer description J2602
*/
typedef struct {
  const lin_tl_pdu_data         *tl_tx_single_pdu;                /**< pointer to transmit PDU data of Single Frame on TL */
  const lin_tl_pdu_data         *tl_rx_single_pdu;                /**< pointer to receive PDU data of Single Frame on TL */
  lin_tl_pdu_data               *tl_current_tx_pdu_ptr;           /**< current PDU pointer*/
  lin_tl_pdu_data               *tl_current_rx_pdu_ptr;           /**< current PDU pointer*/

/* Declaration only for Master interface */   
  l_u8                          tl_cnt_to_send;                   /**< Send counter, if=0 no data, #0 number of PDU need to be sent */
  lin_service_status            tl_service_status;                /**< Status of the last configuration service */

  lin_last_cfg_result           tl_last_cfg_result;               /**< Status of the last configuration service in LIN 2.0, J2602 */
  l_u8                          tl_last_RSID;                     /**< RSID of the last node configuration service */
  l_u8                          tl_ld_error_code;                 /**< Error code in case of positive response */
  l_u8                          *tl_ident_data;                   /**< To store address of RAM area contain response */
/* End of declaration for only Master interface */

/* Declaration only for Slave interface */
  l_u8                          tl_slaveresp_cnt;                 /**< Slave Response data counter */
/* End of declaration only for Slave interface */
} lin_tl_descriptor1;
/** @enum diag_interleaved_state
* state of diagnostic interleaved mode */
typedef enum {
  DIAG_NOT_START,             /**< not into slave response schedule with interleaved mode */
  DIAG_NO_RESPONSE,           /**< master send 0x3D but slave does not response */
  DIAG_RESPONSE               /**< response receive */
} diag_interleaved_state;
#else  /* Single frame support */
/**
* @struct lin_tl_descriptor1
* transport layer description J2602
*/
typedef struct {
  const lin_tl_pdu_data         *tl_tx_single_pdu;                /**< pointer to transmit PDU data of Single Frame on TL */
  const lin_tl_pdu_data         *tl_rx_single_pdu;                /**< pointer to receive PDU data of Single Frame on TL */
  lin_tl_pdu_data               *tl_current_tx_pdu_ptr;           /**< current PDU pointer*/
  lin_tl_pdu_data               *tl_current_rx_pdu_ptr;           /**< current PDU pointer*/

/* Declaration only for Master interface */   
  l_u8                          tl_cnt_to_send;                   /**< Send counter, if=0 no data, #0 number of PDU need to be sent */
  lin_service_status            tl_service_status;                /**< Status of the last configuration service */

  lin_last_cfg_result           tl_last_cfg_result;               /**< Status of the last configuration service in LIN 2.0, J2602 */
  l_u8                          tl_last_RSID;                     /**< RSID of the last node configuration service */
  l_u8                          tl_ld_error_code;                 /**< Error code in case of positive response */
  l_u8                          *tl_ident_data;                   /**< To store address of RAM area contain response */
/* End of declaration for only Master interface */

/* Declaration only for Slave interface */
  l_u8                          tl_slaveresp_cnt;                 /**< Slave Response data counter */
/* End of declaration only for Slave interface */
} lin_tl_descriptor;
#if (XGATE_SUPPORT == 1)
#pragma align  off
#endif /* End (XGATE_SUPPORT == 1) */
#endif /*End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)*/
/* End of TL support */

/**********************************************************************/
/***************             Configuration          *******************/
/**********************************************************************/
/**
* @struct lin_configuration
* configuration structure
*/
typedef struct {
  lin_protocol_handle           lin_protocol_version;           /**< Protocol version */
  lin_protocol_handle           lin_language_version;           /**< Language version */
  const l_u16                   baud_rate;                      /**< Cluster baudrate */
  lin_function                  function;                       /**< Function (_MASTER_ or _SLAVE_) */ 
  lin_node_attribute            *node_attribute;                /**< Node attribute */
/* LIN data pointer */  
  l_u8                          *response_buffer;               /**< Response buffer */
  l_u8                          *successful_transfer;           /**< Transfer flag */
  l_u8                          *error_in_response;             /**< Error response */
  l_u8                          *goto_sleep_flg;                /**< Goto sleep flag */
  l_u8                          *current_pid;                   /**< Current pid */
  lin_word_status_str           *word_status;                   /**< Word status */
/* Protocol */ 
  l_u8                          timebase;                       /**< Timebase only used for master */   
  l_u8                          *diag_signal_tbl;               /**< Diagnostic signal list */
  l_u8                          num_of_frames;                  /**< Number of frame except diagnostic frames */
  l_u8                          frame_start;                    /**< Start index of frame list */  
  const lin_frame_struct        *frame_tbl;                     /**< Frame list except diagnostic frames */
  l_u8                          *frame_flg;                     /**< Frame flag */
  l_u8                          num_of_schedules;               /**< Number of schedule table */
  l_u8                          schedule_start;                 /**< Start index of schedule table list */
  const lin_schedule_struct     *schedule_tbl;                  /**< Shedule table list */
  l_u8                          *schedule_start_entry;          /**< Start entry of each schedule table */
  l_u8                          *next_transmit_tick;            /**< Used to count the next transmit tick */
  l_u8                          *active_schedule_id;            /**< Active schedule table id */
  l_u8                          *previous_schedule_id;          /**< Previous schedule table id */
  l_u8                          *diagnostic_frame_to_send;      /**< Number of diagnostic frame to send */
/* Multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
  l_diagnostic_mode             *diagnostic_mode;               /**< Diagnostic mode */
#else  /* Single frame support */
  l_diagnostic_mode             diagnostic_mode;                /**< Diagnostic mode */
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
  l_u8                          *configuration_RAM;             /**< Configuration in RAM */
  const l_u16                   *configuration_ROM;             /**< Configuration in ROM */
  /* TL support*/
  lin_tl_descriptor             *tl_desc;                       /**< TL Configuration */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)  
  lin_diagnostic_state          *tl_diag_state;                 /**< diagnostic state */   
  lin_service_status            *tl_service_status;             /**< Status of the last configuration service */
  diag_interleaved_state        *tl_diag_interleave_state;      /**< state of diagnostic interleaved mode */
#endif  /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
} lin_configuration;
/* End of data definition */
#endif /* End (LIN_MODE == _MASTER_MODE_) */

/***** Macros *****/
#define BIT(A,B)      (((A)>>(B))&0x01)   /**< return bit has position \a B in byte \a A, A is the variable while */

/* Global functions */
/**
* @def CALLBACK_HANDLER(iii, event_id, pid)
* call lin_pid_response_callback_handler function in MASTER mode
*/
#if (LIN_MODE == _MASTER_MODE_)
#define CALLBACK_HANDLER(iii, event_id, pid) lin_pid_response_callback_handler((iii), (event_id), (pid))
#else
#define CALLBACK_HANDLER(iii, event_id, pid) lin_pid_response_callback_handler((event_id), (pid))
#endif /* End (LIN_MODE == _MASTER_MODE_) */

#if LIN_MODE == _MASTER_MODE_
/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_lld_init (l_ifc_handle iii)
* @brief This function will initialize the specified interface (if available)
*     with the predefine configuration
*
* @param iii <B>[IN]</B> lin interface handle
*
* @return #l_u8
*     <BR>0 if successful, else return error code
*
* @SDD_ID LIN_SDD_216
* @endif
*
* @local_var
*   -# <B>#lin_hardware_name</B> <I>lin_hw</I>
*   -# <B>#l_u8</B> <I>ret</I>
*       <BR>return this variable at the end of function
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*
* @details
*   This function will initialize the specified interface (if available)
*   with the predefine configuration
*
* @see  #lin_lld_sci_init
* @see  #lin_lld_xgate_init
* @see  #lin_lld_slic_init
* @see  #lin_lld_gpio_init
*//*END*----------------------------------------------------------------------*/
l_u8 lin_lld_init(l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_lld_deinit (l_ifc_handle iii)
* @brief This function disconnect the node from the cluster and
*   free all hardware used
*
* @param iii <B>[IN]</B> lin interface handle
*
* @return #l_u8
*     <BR>0 if successful, else return error code
*
* @SDD_ID LIN_SDD_217
* @endif
*
* @local_var
*   -# <B>#lin_hardware_name</B> <I>lin_hw</I>
*   -# <B>#l_u8</B> <I>ret</I>
*       <BR>return this variable at the end of function
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*
* @details
*   This function disconnect the node from the cluster and
*   free all hardware used
*
* @see  #lin_lld_sci_deinit
* @see  #lin_lld_xgate_deinit
* @see  #lin_lld_slic_deinit
* @see  #lin_lld_gpio_deinit
*//*END*----------------------------------------------------------------------*/
l_u8 lin_lld_deinit(l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_tx_header (l_ifc_handle iii, l_u8 pid)
* @brief Master transmit header
*
* @param iii <B>[IN]</B> lin interface handle
* @param pid <B>[IN]</B> ID of the header to be sent
*
* @return #void
*
* @SDD_ID LIN_SDD_219
* @endif
*
* @local_var
*   -# <B>#lin_hardware_name</B> <I>lin_hw</I> Indicator of LIN HW
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*
* @details
*   Issues the LIN header with specified ID. The function cause the LIN frame transmission,
*   applicable for the master only
*
* @see  #lin_lld_sci_tx_header
* @see  #lin_lld_xgate_tx_header
*//*END*----------------------------------------------------------------------*/
void lin_lld_tx_header(l_ifc_handle iii, l_u8 pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_tx_wake_up (l_ifc_handle iii)
* @brief This function send wakeup signal
*
* @param iii <B>[IN]</B> lin interface handle
*
* @return #void
*
* @SDD_ID LIN_SDD_220
* @endif
*
* @local_var
*   -# <B>#lin_hardware_name</B> <I>lin_hw</I> Indicator of LIN HW
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*
* @details
*   This function send wakeup signal
*
* @see #lin_lld_sci_tx_wake_up
* @see #lin_lld_xgate_tx_wake_up
* @see #lin_lld_slic_tx_wake_up
* @see #lin_lld_gpio_tx_wake_up
*//*END*----------------------------------------------------------------------*/
void lin_lld_tx_wake_up(l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_int_enable (l_ifc_handle iii)
*
* @brief Enable the interrupt related the interface
*
* @param iii <B>[IN]</B> lin interface handle
*
* @return #void
*
* @SDD_ID LIN_SDD_221
* @endif
*
* @local_var
*   -# <B>#lin_hardware_name</B> <I>lin_hw</I> Indicator of LIN HW
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*
* @details
*   This function send wakeup signal
*
* @see #lin_lld_sci_int_enable
* @see #lin_lld_xgate_int_enable
* @see #lin_lld_slic_int_enable
* @see #lin_lld_gpio_int_enable
*//*END*----------------------------------------------------------------------*/
void lin_lld_int_enable(l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_lld_int_disable (l_ifc_handle iii)
*
* @brief
*   Disable the interrupt related the interface
*
* @param iii <B>[IN]</B> lin interface handle
*
* @return #l_u8
*
* @SDD_ID LIN_SDD_222
* @endif
*
* @local_var
*   -# <B>#lin_hardware_name</B> <I>lin_hw</I> Indicator of LIN HW
*   -# <B>#l_u8</B> <I>ret</I>
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*
* @details
*   Disable the interrupt related the interface
*
* @see #lin_lld_sci_int_disable
* @see #lin_lld_xgate_int_disable
* @see #lin_lld_slic_int_disable
* @see #lin_lld_gpio_int_disable
*//*END*----------------------------------------------------------------------*/
l_u8 lin_lld_int_disable(l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_ignore_response (l_ifc_handle iii)
*
* @brief
*   Let the low level driver ignore the next respond
*
* @param iii <B>[IN]</B> lin interface handle
*
* @return #void
*
* @SDD_ID LIN_SDD_223
* @endif
*
* @local_var
*   -# <B>#lin_hardware_name</B> <I>lin_hw</I> Indicator of LIN HW
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*
* @details
*  Let the low level driver ignore the next respond
*   (i.e. because the PID is not relevant)
*
* @see #lin_lld_sci_ignore_response
* @see #lin_lld_xgate_ignore_response
* @see #lin_lld_slic_ignore_response
* @see #lin_lld_gpio_ignore_response
*//*END*----------------------------------------------------------------------*/
void lin_lld_ignore_response(l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_set_low_power_mode (l_ifc_handle iii)
*
* @brief
*   Let the low level driver go to low power mode
*
* @param iii <B>[IN]</B> lin interface handle
*
* @return #void
*
* @SDD_ID LIN_SDD_224
* @endif
*
* @local_var
*   -# <B>#lin_hardware_name</B> <I>lin_hw</I> Indicator of LIN HW
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*
* @details
*  Let the low level driver go to low power mode
*   (In sleep mode for example)
*
* @see #lin_lld_sci_set_low_power_mode
* @see #lin_lld_xgate_set_low_power_mode
* @see #lin_lld_slic_set_low_power_mode
* @see #lin_lld_gpio_set_low_power_mode
*//*END*----------------------------------------------------------------------*/
void lin_lld_set_low_power_mode(l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_lld_set_response (l_ifc_handle iii, l_u8 response_length)
*
* @brief
*   Switch the low level driver to transmit response state
*
* @param iii <B>[IN]</B> lin interface handle
* @param response_length <B>[IN]</B> length of response
*
* @return #l_u8
*
* @SDD_ID LIN_SDD_225
* @endif
*
* @local_var
*   -# <B>#lin_hardware_name</B> <I>lin_hw</I> Indicator of LIN HW
*   -# <B>#l_u8</B> <I>ret</I>
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*  Switch the low level driver to transmit response state
*   (RX Request for response)
*
* @see #lin_lld_sci_tx_response
* @see #lin_lld_xgate_tx_response
* @see #lin_lld_slic_tx_response
* @see #lin_lld_gpio_tx_response
*//*END*----------------------------------------------------------------------*/
l_u8 lin_lld_set_response(l_ifc_handle iii, l_u8 response_length);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_lld_rx_response (l_ifc_handle iii, l_u8 response_length)
*
* @brief
*   Switch the low level driver to receiving respond state
*
* @param iii <B>[IN]</B> lin interface handle
* @param response_length <B>[IN]</B> length of response
*
* @return #l_u8
*
* @SDD_ID LIN_SDD_226
* @endif
*
* @local_var
*   -# <B>#lin_hardware_name</B> <I>lin_hw</I> Indicator of LIN HW
*   -# <B>#l_u8</B> <I>ret</I>
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*
* @details
*  Switch the low level driver to receiving respond state
*
* @see #lin_lld_sci_rx_response
* @see #lin_lld_xgate_rx_response
* @see #lin_lld_slic_rx_response
* @see #lin_lld_gpio_rx_response
*//*END*----------------------------------------------------------------------*/
l_u8 lin_lld_rx_response(l_ifc_handle iii, l_u8 response_length);

#endif /* End (LIN_MODE == _MASTER_MODE_) */

#if (LIN_MODE == _SLAVE_MODE_)

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_lld_init ( void )
*
* @brief
*   Initialize the specified interface
*
* @return #l_u8
*
* @SDD_ID LIN_SDD_227
* @endif
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*
* @details
*  This function will initialize the specified interface (if available)
*   with the predefine configuration
*
* @see #lin_lld_sci_init
* @see #lin_lld_xgate_init
* @see #lin_lld_slic_init
* @see #lin_lld_gpio_init
*//*END*----------------------------------------------------------------------*/
l_u8 lin_lld_init(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_lld_deinit ( void )
*
* @brief
*   disconnect the node from the cluster
*
* @return #l_u8
*
* @SDD_ID LIN_SDD_228
* @endif
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*
* @details
*  This function disconnect the node from the cluster and
*   free all hardware used
*
* @see #lin_lld_sci_deinit
* @see #lin_lld_xgate_deinit
* @see #lin_lld_slic_deinit
* @see #lin_lld_gpio_deinit
*//*END*----------------------------------------------------------------------*/
l_u8 lin_lld_deinit(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_lld_get_status ( void )
*
* @brief
*   This function gets current status of an interface
*
* @return #l_u8
*
* @SDD_ID LIN_SDD_229
* @endif
*
* @local_var
*   -#  <B>#l_u8</B> <I>ret</I>
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*
* @details
*  This function gets current status of an interface
*
* @see #lin_lld_sci_get_status
* @see #lin_lld_xgate_get_status
*//*END*----------------------------------------------------------------------*/
l_u8 lin_lld_get_status(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_lld_get_state ( void )
*
* @brief
*   This function gets current state of an interface
*
* @return #l_u8
*
* @SDD_ID N/A
* @endif
*
* @local_var
*   -#  <B>#l_u8</B> <I>ret</I>
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*
* @details
*  This function gets current state of an interface
*
* @see #lin_lld_sci_get_state
* @see #lin_lld_xgate_get_state
* @see #lin_lld_slic_get_state
* @see #lin_lld_gpio_get_status
*//*END*----------------------------------------------------------------------*/
l_u8 lin_lld_get_state(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_tx_wake_up ( void )
*
* @brief
*   This function send wakeup signal
*
* @return #void
*
* @SDD_ID LIN_SDD_230
* @endif
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*
* @details
*  This function send wakeup signal
*
* @see #lin_lld_sci_tx_wake_up
* @see #lin_lld_xgate_tx_wake_up
* @see #lin_lld_slic_tx_wake_up
* @see #lin_lld_gpio_tx_wake_up
*//*END*----------------------------------------------------------------------*/
void lin_lld_tx_wake_up(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_int_enable ( void )
*
* @brief
*   Enable the interrupt related the interface
*
* @return #void
*
* @SDD_ID LIN_SDD_231
* @endif
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*
* @details
*  Enable the interrupt related the interface
*
* @see #lin_lld_sci_int_enable
* @see #lin_lld_xgate_int_enable
* @see #lin_lld_slic_int_enable
* @see #lin_lld_gpio_int_enable
*//*END*----------------------------------------------------------------------*/
void lin_lld_int_enable(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_lld_int_disable ( void )
*
* @brief
*   Disable the interrupt related the interface
*
* @return #l_u8
*
* @SDD_ID LIN_SDD_232
* @endif
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*
* @details
*  Disable the interrupt related the interface
*
* @see #lin_lld_sci_int_disable
* @see #lin_lld_xgate_int_disable
* @see #lin_lld_slic_int_disable
* @see #lin_lld_gpio_int_disable
*//*END*----------------------------------------------------------------------*/
l_u8 lin_lld_int_disable(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_ignore_response ( void )
*
* @brief
*   Let the low level driver ignore the next respond
*
* @return #void
*
* @SDD_ID LIN_SDD_233
* @endif
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*
* @details
*  Let the low level driver ignore the next respond
*   (i.e. because the PID is not relevant)
*
* @see #lin_lld_sci_ignore_response
* @see #lin_lld_xgate_ignore_response
* @see #lin_lld_slic_ignore_response
* @see #lin_lld_gpio_ignore_response
*//*END*----------------------------------------------------------------------*/
void lin_lld_ignore_response(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_set_low_power_mode ( void )
*
* @brief
*   Let the low level driver go to low power mode
*
* @return #void
*
* @SDD_ID LIN_SDD_234
* @endif
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*
* @details
*  Let the low level driver go to low power mode
*   (In sleep mode for example)
*
* @see #lin_lld_sci_set_low_power_mode
* @see #lin_lld_xgate_set_low_power_mode
* @see #lin_lld_slic_set_low_power_mode
* @see #lin_lld_gpio_set_low_power_mode
*//*END*----------------------------------------------------------------------*/
void lin_lld_set_low_power_mode(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_lld_set_response (l_u8 response_length)
*
* @brief
*   Switch the low level driver to transmit response state
*
* @param response_length <B>[IN]</B> length of response
*
* @return #l_u8
*
* @SDD_ID LIN_SDD_235
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>ret</I>
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*   -# <B>#lin_lld_response_buffer</B>
*
* @details
*  Switch the low level driver to transmit response state
*   (RX Request for response)
*
* @see #lin_lld_sci_tx_response
* @see #lin_lld_xgate_tx_response
* @see #lin_lld_slic_tx_response
* @see #lin_lld_gpio_tx_response
*//*END*----------------------------------------------------------------------*/
l_u8 lin_lld_set_response(l_u8 response_length);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_lld_rx_response (l_u8 response_length)
*
* @brief
*   Switch the low level driver to receiving respond state
*
* @param response_length <B>[IN]</B> length of response
*
* @return #l_u8
*
* @SDD_ID LIN_SDD_236
* @endif
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*
* @details
*  Switch the low level driver to receiving respond state
*
* @see #lin_lld_sci_rx_response
* @see #lin_lld_xgate_rx_response
* @see #lin_lld_slic_rx_response
* @see #lin_lld_gpio_rx_response
*//*END*----------------------------------------------------------------------*/
l_u8 lin_lld_rx_response(l_u8 response_length);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_mcu_reset ( void )
*
* @brief
*   Reset device
*
* @return #void
*
* @SDD_ID LIN_SDD_237
* @endif
*
* @static_global_var
*   -# <B>#lin_virtual_ifc</B>
*
* @details
*  Reset device
*//*END*----------------------------------------------------------------------*/
void lin_lld_mcu_reset(void);

#endif /* End (LIN_MODE == _SLAVE_MODE_) */

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_timer_init ( void )
*
* @brief
*   Initialize the timer
*
* @return #void
*
* @SDD_ID LIN_SDD_215
* @endif
*
* @details
*  Initialize the timer
*
* @see #lin_lld_timer_S12X_init
* @see #lin_lld_timer_S12_init
* @see #lin_lld_timer_S08_init
*//*END*----------------------------------------------------------------------*/
void lin_lld_timer_init(void); 


/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_checksum (l_u8 *buffer, l_u8 raw_pid)
*
* @brief
*   Calculate checksum byte
*
* @param *buffer <B>[IN]</B> pointer to buffer
* @param raw_pid <B>[IN]</B> product identifier
*
* @return #l_u8
*   <BR>checksum byte
*
* @SDD_ID LIN_SDD_214
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>length</I>
*   -# <B>#l_u16</B> <I>check_sum</I>
*
* @details
*  Calculate checksum byte
*//*END*----------------------------------------------------------------------*/
l_u8 lin_checksum(l_u8 *buffer, l_u8 raw_pid);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 lin_process_parity (l_u8 pid, l_u8 type)
*
* @brief
*   Check parity and generate PID
*
* @param pid <B>[IN]</B> product identifier
* @param type <B>[IN]</B> check parity or make parity
*
* @return #l_u8
*   <BR>#pid if checkparity ok, otherwise return 0xFF
*
* @SDD_ID LIN_SDD_213
* @endif
*
* @local_var
*   -# <B>#_lu8</B> <I>parity</I>
*   -# <B>#l_u8</B> <I>ret</I>
*
* @details
*  Check parity and generate PID
*    XGATE Module not use this routine
*//*END*----------------------------------------------------------------------*/
l_u8 lin_process_parity(l_u8 pid, l_u8 type);

/*****************************************************************/
/****                extern variables                         ****/
/*****************************************************************/
/*****    Common variables    *****/
extern const lin_frame_struct   lin_frame_tbl[LIN_NUM_OF_FRMS]; 
extern l_bool                   lin_frame_flag_tbl[LIN_NUM_OF_FRMS];
extern l_u8                     lin_pFrameBuf[LIN_FRAME_BUF_SIZE];
extern l_u8                     lin_flag_handle_tbl[LIN_FLAG_BUF_SIZE];


/***** Slave mode variables   *****/
#if LIN_MODE == _SLAVE_MODE_
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
/* diagnostic flag */
extern l_u8 lin_diag_services_flag[_DIAG_NUMBER_OF_SERVICES_];
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */

extern const  lin_frame_struct    lin_frame_tbl[LIN_NUM_OF_FRMS];
extern l_u8                       lin_configuration_RAM[LIN_SIZE_OF_CFG];
extern l_u8                       lin_successful_transfer;
extern l_u8                       lin_error_in_response;
extern l_u8                       lin_goto_sleep_flg;

extern l_u8                       lin_save_configuration_flg;

extern l_u8                       lin_diag_signal_tbl[16];
extern const l_signal_handle      response_error;
extern const l_u8                 response_error_byte_offset;
extern const l_u8                 response_error_bit_offset;
extern l_u8                       tl_slaveresp_cnt;
extern lin_word_status_str        lin_word_status;
/**
* @var l_u8 frame_index
* @details
*   index of frame in frames table
*/
extern l_u8                       frame_index;
extern const l_u16                lin_configuration_ROM[LIN_SIZE_OF_CFG];  /* Configuration stores in ROM */
extern const lin_product_id       product_id;
extern l_u8                       tl_slaveresp_cnt;       /**< Slave Response data counter */
/* if single frame */
#if (_TL_FRAME_SUPPORT_ == _TL_SINGLE_FRAME_)
extern lin_tl_pdu_data            tx_single_pdu_data;
extern lin_tl_pdu_data            rx_single_pdu_data ;
extern lin_tl_pdu_data            *tl_current_tx_pdu_ptr;    /* current PDU pointer*/
extern lin_tl_pdu_data            *tl_current_rx_pdu_ptr;    /* current PDU pointer*/
#else /* Multi frame support */
/*transport layer support */
extern lin_transport_layer_queue lin_tl_tx_queue;         /**< transport layer transmit queue */
extern lin_transport_layer_queue lin_tl_rx_queue;         /**< transport layer receive queue */
extern lin_message_status tl_rx_msg_status;               /**< transport layer rx message status */
extern l_u16 tl_rx_msg_index;                             /**< rx message index */
extern l_u16 tl_rx_msg_size;                              /**< rx message size */
extern lin_message_status tl_receive_msg_status;          /**< receive message status */

extern lin_message_status tl_tx_msg_status;               /**< cooked tx status */
extern l_u16 tl_tx_msg_index;                             /**< index of message in queue */
extern l_u16 tl_tx_msg_size;                              /**< Size of message in queue */

extern lin_last_cfg_result tl_last_cfg_result;            /**< Status of the last configuration service in LIN 2.0, J2602 */
extern l_u8 tl_last_RSID;                                 /**< RSID of the last node configuration service */
extern l_u8 tl_ld_error_code;                             /**< Error code in case of positive response */

extern l_u8 tl_no_of_pdu;                                 /**< number of received pdu */
extern l_u8 tl_frame_counter;                             /**< frame counter in received message */

extern lin_message_timeout_type tl_check_timeout_type;    /**< timeout type */
extern l_u16 tl_check_timeout;                            /**< timeout counter*/

extern l_u8 *tl_ident_data;                               /**< To store address of RAM area contain response */

/* Declaration only for Slave interface */
extern lin_diagnostic_state tl_diag_state;                /**< diagnostic state */
extern lin_service_status tl_service_status;              /**< Status of the last configuration service */

#endif /*End (_TL_FRAME_SUPPORT_ == _TL_SINGLE_FRAME_)*/

extern l_u8                       lin_current_pid;
extern l_u8                       lin_configured_NAD;
extern const l_u8                 lin_initial_NAD;



/* if XGATE Support */
#if XGATE_SUPPORT == 1
#pragma DATA_SEG  SHARED_DATA /* Define a specific segment to store the data */
extern const lin_hardware_name    lin_virtual_ifc;
extern l_u8                       lin_lld_response_buffer[10];
#pragma DATA_SEG  DEFAULT /* Return to default data segment */
#endif /* End (XGATE_SUPPORT == 1) */

/* If XGATE not support */
#if XGATE_SUPPORT == 0
extern const lin_hardware_name    lin_virtual_ifc;
extern l_u8                       lin_lld_response_buffer[10];
#endif /* End (XGATE_SUPPORT == 0) */

#endif /* End (LIN_MODE == _SLAVE_MODE_) */
/***** Master mode variables  *****/

#if LIN_MODE == _MASTER_MODE_

extern l_u8                       lin_save_configuration_flg[LIN_NUM_OF_IFCS];

/* if XGATE Support */
#if XGATE_SUPPORT == 1
#pragma DATA_SEG  SHARED_DATA /* Define a specific segment to store the data */
extern const lin_hardware_name lin_virtual_ifc[LIN_NUM_OF_IFCS];
extern l_u8 lin_lld_response_buffer[LIN_NUM_OF_IFCS][10];
#pragma DATA_SEG  DEFAULT /* Return to default data segment */
#endif /* End (XGATE_SUPPORT == 1) */

/* If XGATE not support */
#if XGATE_SUPPORT == 0
extern const lin_hardware_name lin_virtual_ifc[LIN_NUM_OF_IFCS];
#endif /* End (XGATE_SUPPORT == 1) */

extern const lin_configuration lin_ifc_configuration[LIN_NUM_OF_IFCS];
#endif /* End (LIN_MODE == _MASTER_MODE_) */

/*****************************************************************/
/****                extern api functions                     ****/
/*****************************************************************/
/********************************/
/******* common core apis *******/
/*********************************/
extern l_bool        l_sys_init (void);
/*********************************/
/***** APIs for Master Mode ******/
/*********************************/ 
#if LIN_MODE == _MASTER_MODE_
extern void          l_sch_set (l_ifc_handle iii, l_schedule_handle schedule_iii, l_u8 entry); 
extern l_u8          l_sch_tick (l_ifc_handle iii);
extern void          l_ifc_goto_sleep (l_ifc_handle iii);
#endif /* End (LIN_MODE == _MASTER_MODE_) */

/*********************************/
/***** APIs for Slave Mode *******/
/*********************************/ 
extern l_bool        l_ifc_init (l_ifc_handle iii);
extern void          l_ifc_wake_up (l_ifc_handle iii);
extern void          l_ifc_rx (l_ifc_handle iii);
extern void          l_ifc_tx (l_ifc_handle iii); 
extern l_u16         l_ifc_read_status (l_ifc_handle iii);
extern void          l_ifc_aux (l_ifc_handle iii);
extern l_u16         l_sys_irq_disable (l_ifc_handle iii);
extern void          l_sys_irq_restore (l_ifc_handle iii);
#if LIN_MODE == _SLAVE_MODE_

#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
/* INITIALIZATION */
extern void ld_init(void);
/* RAW APIs */
extern void ld_put_raw(const l_u8* const data);
extern void ld_get_raw(l_u8* const data);
extern l_u8 ld_raw_tx_status(void);
extern l_u8 ld_raw_rx_status(void);
/* COOKED APIs */
extern void ld_send_message(l_u16 length, const l_u8* const data);
extern void ld_receive_message(l_u16* const length, l_u8* const data);
extern l_u8 ld_tx_status(void);
extern l_u8 ld_rx_status(void);
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
#endif /* End LIN_MODE == _SLAVE_MODE_) */

/* J2602 core apis */
extern l_bool l_ifc_connect (l_ifc_handle iii);
extern l_bool l_ifc_disconnect (l_ifc_handle iii);

/****************************************/
/******** Transport layer APIs **********/
/****************************************/
/*********************************/
/***** APIs for Master Mode ******/
/*********************************/ 
#if LIN_MODE == _MASTER_MODE_
/* Multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
extern void ld_init(l_ifc_handle iii);
extern void ld_put_raw(l_ifc_handle iii,const l_u8* const data);
extern void ld_get_raw(l_ifc_handle iii,l_u8* const data);
extern l_u8 ld_raw_tx_status(l_ifc_handle iii);
extern l_u8 ld_raw_rx_status(l_ifc_handle iii);
extern void ld_send_message(l_ifc_handle iii, l_u16 length, l_u8 NAD, const l_u8* const data);
extern void ld_receive_message(l_ifc_handle iii, l_u16* const length, l_u8* const NAD, l_u8* const data);
extern l_u8 ld_tx_status(l_ifc_handle iii);
extern l_u8 ld_rx_status(l_ifc_handle iii);
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */

extern void ld_assign_NAD(l_ifc_handle iii, l_u8 initial_NAD, l_u16 supplier_id, l_u16 function_id, l_u8 new_NAD);
extern void ld_conditional_change_NAD (l_ifc_handle iii, l_u8 NAD, l_u8 id, l_u8 byte, l_u8 mask, l_u8 invert, l_u8 new_NAD);
extern void ld_read_by_id (l_ifc_handle iii, l_u8 NAD, l_u16 supplier_id, l_u16 function_id, l_u8 id, l_u8* const data);


/***** J2602 *****/
#if LIN_PROTOCOL == PROTOCOL_J2602
extern l_bool ld_is_ready (l_ifc_handle iii);
extern l_u8 ld_check_response (l_ifc_handle iii, l_u8* const RSID, l_u8* const error_code);
extern void ld_assign_frame_id_range (l_ifc_handle iii, l_u8 initial_NAD, l_u16 supplier_id, l_u16 message_id, l_u8 PID);
#endif /* End (LIN_PROTOCOL == PROTOCOL_J2602) */
/***** LIN21 *****/
#if LIN_PROTOCOL == PROTOCOL_21
extern l_u8 ld_is_ready (l_ifc_handle iii);
extern void ld_check_response (l_ifc_handle iii, l_u8* const RSID, l_u8* const error_code);
extern void ld_assign_frame_id_range (l_ifc_handle iii,l_u8 NAD, l_u8 start_index, const l_u8* const PIDs);
#endif /* End (LIN_PROTOCOL == PROTOCOL_21) */

l_u8 ld_read_by_id_callout(l_ifc_handle iii, l_u8 id, l_u8 *data);

#endif /* End (LIN_MODE == _MASTER_MODE_) */


#endif  /* _LIN_H */
/** @} */
