/****************************************************************************** 
* 
* Freescale Semiconductor Inc. 
* (c) Copyright 2014-2015 Freescale Semiconductor, Inc. 
* ALL RIGHTS RESERVED. 
* 
***************************************************************************//*! 
* 
* @file      lin_cfg.c 
* 
* @author    FPT Software
*  
* @version   1.0 
*  
* @date      Mon Jan 26 11:55:03 CST 2015 
*  
* @brief     Common LIN configuration, data structure
* 
******************************************************************************/
#include "lin_cfg.h"
#include "LIN_Driver/lowlevel/lin.h"
/* Mapping interface with hardware */
 
const lin_hardware_name lin_virtual_ifc[LIN_NUM_OF_IFCS] = {uart1};
/* Low level response buffer */
l_u8 lin_lld_response_buffer[LIN_NUM_OF_IFCS][10];
 
 /* Successful transfer flags */
l_u8 lin_successful_transfer[LIN_NUM_OF_IFCS]; 
/* Error in response */
l_u8 lin_error_in_response[LIN_NUM_OF_IFCS]; 
/* Goto sleep flag */
l_u8 lin_goto_sleep_flg[LIN_NUM_OF_IFCS];
/* Save configuration flag */
l_u8 lin_save_configuration_flg[LIN_NUM_OF_IFCS] = {0};
 /* Next transmit tick */
l_u8 lin_next_transmit[LIN_NUM_OF_IFCS];
 /* lin word status */
lin_word_status_str lin_word_status[LIN_NUM_OF_IFCS]; 
/* current pid */
l_u8 lin_current_pid[LIN_NUM_OF_IFCS];
 

/* definition and initialization of signal array */
l_u8    lin_pFrameBuf[LIN_FRAME_BUF_SIZE] =
{


  0x00 /* 0 : 00000000 */ /* start of frame LI0_f_adc */



  ,0x00 /* 1 : 00000000 */ /* start of frame LI0_f_buttons */

  ,0x00 /* 2 : 00000000 */
  

  ,0xfe /* 3 : 11111110 */ /* start of frame LI0_f_error */

};

/* definition and initialization of signal array */
l_u8    lin_flag_handle_tbl[LIN_FLAG_BUF_SIZE] =
{


  0x00 /* 0: start of flag frame LI0_f_adc */



  ,0x00 /* 1: start of flag frame LI0_f_buttons */


  ,0x00 /* 2: start of flag frame LI0_f_error */

};


/*************************** Flag set when signal is updated ******************/
/* Diagnostic signal */
l_u8 lin_diag_signal_tbl[LIN_NUM_OF_IFCS][16];
/*****************************event trigger frame*****************************/
/* all event trigger frames for master node */


  
/*****************************sporadic frame*****************************/
/*all sporadic frames for master node*/
/**********************************  Frame table **********************************/
const lin_frame_struct lin_frame_tbl[LIN_NUM_OF_FRMS] ={

    { LIN_FRM_UNCD, 1, LIN_RES_PUB, 0, 0, 1   , (l_u8*)0 }

   ,{ LIN_FRM_UNCD, 2, LIN_RES_SUB, 1, 1, 1 , (l_u8*)0 }
  
   ,{ LIN_FRM_UNCD, 1, LIN_RES_SUB, 3, 2, 1 , (l_u8*)0 }
  
   ,{ LIN_FRM_DIAG, 8, LIN_RES_PUB, 0, 0, 0 , (l_u8*)0 }
  
   ,{ LIN_FRM_DIAG, 8, LIN_RES_SUB, 0, 0, 0 , (l_u8*)0 }
  
};

/*********************************** Frame flag Initialization **********************/
l_bool lin_frame_flag_tbl[LIN_NUM_OF_FRMS] = {0, 0, 0, 0, 0};

/**************************** Lin configuration Initialization ***********************/
/* max_response_frame_timeout = round((1.4x(10+Nx10)xTbit)/Tbase_period) + 3 */
const l_u16 lin_max_frame_res_timeout_val[LIN_NUM_OF_IFCS][8]= {

   {9, 12, 15, 18, 21, 23, 26, 29 }
  
};


l_u8 LI0_lin_configuration_RAM[LI0_LIN_SIZE_OF_CFG]= {0x00, 0x17, 0x18, 0x19, 0x3C, 0x3D ,0xFF};


const l_u16  LI0_lin_configuration_ROM[LI0_LIN_SIZE_OF_CFG]= {0x0000, 0x17, 0x18, 0x19, 0x3C, 0x3D ,0xFFFF};


/**************** Node attributes Initialization  ****************************/





/********************** Go to sleep Initialization *************************/
const lin_schedule_data LI0_lin_gotosleep_data[1] = { 
   {LI0_MasterReq, 15, {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}}
};


/******************** Schedule table Initialization ************************/
const lin_schedule_data LI0_NormalTable_data[3] = {

   {LI0_f_adc, 10, {0}}
  

   , {LI0_f_buttons, 10, {0}}
  
   , {LI0_f_error, 10, {0}}
  
};

/******************** Schedule table Initialization ************************/
const lin_schedule_data LI0_MasterReqTable_data[1] = {

   {LI0_MasterReq, 15, {0}}
  
};

/******************** Schedule table Initialization ************************/
const lin_schedule_data LI0_SlaveRespTable_data[1] = {

   {LI0_SlaveResp, 15, {0}}
  
};



/********************* Schedule table structure Initialization ***************/
const lin_schedule_struct lin_schedule_tbl[LIN_NUM_OF_SCHD_TBL] ={

   /*interface_name = LI0 */
   {0, LIN_SCH_TBL_NULL  ,(lin_schedule_data*)0 }
  

   ,{1, LIN_SCH_TBL_GOTO  , &LI0_lin_gotosleep_data[0] }
  
   ,{1, LIN_SCH_TBL_DIAG  , &LI0_MasterReqTable_data[0] }
  
   ,{1, LIN_SCH_TBL_DIAG  , &LI0_SlaveRespTable_data[0] }
  
   ,{3, LIN_SCH_TBL_NORM  , &LI0_NormalTable_data[0] }
  
};
/********************** Schedule table status Initialization ******************/
l_u8 lin_schedule_start_entry[LIN_NUM_OF_SCHD_TBL] = {0, 0, 0, 0, 0};
l_u8 lin_active_schedule_id[LIN_NUM_OF_IFCS] = {0};
l_u8 lin_previous_schedule_id[LIN_NUM_OF_IFCS] = {0};

 
l_u8 lin_diag_frame_to_send[LIN_NUM_OF_IFCS] = {0}; 
/********************** PDU data Initialization *******************************/
lin_tl_pdu_data tx_single_pdu_data[LIN_NUM_OF_IFCS] = {
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
 
};
lin_tl_pdu_data rx_single_pdu_data[LIN_NUM_OF_IFCS] = {
 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};


 
 /****************************Transport Layer Initialization ***********************/

lin_tl_descriptor lin_tl_desc[LIN_NUM_OF_IFCS] = { 

   /* interface_name = LI0 */
   {
   (const lin_tl_pdu_data *) &tx_single_pdu_data[LI0],          /* *pointer to transmit queue on TL */
   (const lin_tl_pdu_data *) &rx_single_pdu_data[LI0],          /* *pointer to receive queue on TL */
   (lin_tl_pdu_data*)0,          /* tl_current_tx_pdu_ptr */
   (lin_tl_pdu_data*)0,          /* tl_current_rx_pdu_ptr */
   /* Declaration only for Master interface */
   0,      /* tl_cnt_to_send */
   LD_SERVICE_IDLE,    /* tl_service_status */
   LD_SUCCESS,   /* tl_last_cfg_result */
   0,    /* last_RSID */
   0,    /* ld_error_code */
   (l_u8*)0,   /* *tl_ident_data */
   /* End of declaration for only Master interface */  
   /* Declaration only for Slave interface */
   0 /* tl_slaveresp_cnt */
   /* End of declaration only for Slave interface */ 

 
   }

};

/****************************LIN interface configuration ****************************/
const lin_configuration lin_ifc_configuration[LIN_NUM_OF_IFCS] = {
 
   /* Interface_name = LI0 */
   {
   LIN_PROTOCOL_21,         /*lin_protocol_version */
   LIN_PROTOCOL_21,         /*lin_language_version */
   (const l_u16)9600,            /*  baud_rate */
   _MASTER_,                 /*  function _SLAVE_ | _MASTER_*/
   0,                        /*  node attribute is only used for slave node*/
   /* LIN data pointer */
   &lin_lld_response_buffer[LI0][0],        /*  *response_buffer */
   &lin_successful_transfer[LI0],           /*  *lin_successful_transfer */
   &lin_error_in_response[LI0],             /*  *lin_error_in_response */
   &lin_goto_sleep_flg[LI0],              /*  *goto_sleep_flg */
   &lin_current_pid[LI0],                 /*  *current_pid */
   &lin_word_status[LI0],                 /*  *word_status */
   /* Protocol */
    
   10,                          /*  2*timebase */
   &lin_diag_signal_tbl[LI0][0],          /*  *diag_signal_tbl */
   5,                            /*  num_of_frames */
   0,                              /*  frame_start */
   (const lin_frame_struct*)&lin_frame_tbl[0],                                          /*  frame_tbl */
   &lin_frame_flag_tbl[0],                                       /*  *frame_flg */

    
   5,                     /*  num_of_schedules */
   0,                       /*  schedule_start */
   (const lin_schedule_struct*)&lin_schedule_tbl[0],                                   /*  schedule_tbl */
   &lin_schedule_start_entry[0],                             /*  schedule_start_entry */
   &lin_next_transmit[LI0],             /*  next_transmit_tick */
   &lin_active_schedule_id[LI0],          /*  active_schedule_id */
   &lin_previous_schedule_id[LI0],        /*  previous_schedule_id */
   &lin_diag_frame_to_send[LI0],          /*  *diagnostic_frame_to_send */
   DIAG_NONE,    /*  diagnostic_mode */
   &LI0_lin_configuration_RAM[0],   /*  *configuration_RAM */
   (const l_u16 *)&LI0_lin_configuration_ROM[0],   /*  *configuration_ROM */
   &lin_tl_desc[LI0]   
   }

}; 
 
/*************************** Node hardware configuration definition *************************/
/* Node hardware configuration */
lin_node lin_node_descrs[NUM_OF_UART_CHANNEL]={ 
  
 
   {(tUART*)UART0_ADDR, 0, 0xFF, UNINIT,  {0}, 0,(l_u8*)0, 0x00, (l_u8*)0, 0x80, 0, 0, 0, 0 },
   {(tUART*)UART1_ADDR, 0, 0xFF, UNINIT,  {0}, 0,(l_u8*)0, 0x00, (l_u8*)0, 0x80, 0, 0, 0, 0 },
   {(tUART*)UART2_ADDR, 0, 0xFF, UNINIT,  {0}, 0,(l_u8*)0, 0x00, (l_u8*)0, 0x80, 0, 0, 0, 0 }
};

 
/* This function is an example of response; real implementation is application-dependent */
/* You can use one of the following define to set PCI of response frame for */
/* this service to the correct value */
l_u8 ld_read_by_id_callout(l_ifc_handle iii, l_u8 id, l_u8 *data){
	(void) iii;
	(void) id;
	(void) data;
	return LD_ID_NO_RESPONSE;
}
