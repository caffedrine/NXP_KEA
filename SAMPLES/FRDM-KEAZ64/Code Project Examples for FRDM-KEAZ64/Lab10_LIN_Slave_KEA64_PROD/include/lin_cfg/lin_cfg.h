/****************************************************************************** 
* 
* Freescale Semiconductor Inc. 
* (c) Copyright 2014-2015 Freescale Semiconductor, Inc. 
* ALL RIGHTS RESERVED. 
* 
****************************************************************************//*!  
* 
* @file      lin_cfg.h 
* 
* @author    FPT Software
*  
* @version   1.0 
*  
* @date      Tue Jul 21 14:57:56 CDT 2015
*  
* @brief     Hardware configuration file
* 
******************************************************************************/
#ifndef    _LIN_CFG_H_  
#define    _LIN_CFG_H_  
#include "lin_cfg/lin_hw_cfg.h"
/* Define operating mode */
#define _MASTER_MODE_     0 
#define _SLAVE_MODE_      1 
#define LIN_MODE   _SLAVE_MODE_
/* Define protocol version */
#define PROTOCOL_21       0  
#define PROTOCOL_J2602    1  
#define PROTOCOL_20       2 
#define LIN_PROTOCOL    PROTOCOL_21


#define UART_ADDR        UART1_ADDR    /* For slave */

#define LIN_BAUD_RATE    9600    	 /*For slave*/
/**********************************************************************/
/***************          Diagnostic class selection  *****************/
/**********************************************************************/
#define _DIAG_CLASS_I_          0
#define _DIAG_CLASS_II_         1
#define _DIAG_CLASS_III_        2

#define _DIAG_CLASS_SUPPORT_    _DIAG_CLASS_I_
/**************** FRAME SUPPORT DEFINITION ******************/
#define _TL_SINGLE_FRAME_       0
#define _TL_MULTI_FRAME_        1

#define _TL_FRAME_SUPPORT_      _TL_SINGLE_FRAME_

/* frame buffer size */
#define LIN_FRAME_BUF_SIZE			4
#define LIN_FLAG_BUF_SIZE			3

/**********************************************************************/
/***************               Interfaces           *******************/
/**********************************************************************/
typedef enum { 
   LI0
}l_ifc_handle; 

/**********************************************************************/
/***************               Signals              *******************/
/**********************************************************************/
/* Number of signals */
#define LIN_NUM_OF_SIGS  4
/* List of signals */   
typedef enum {

   /* Interface_name = LI0 */

   LI0_ADC_signal

   , LI0_Button1_signal
  
   , LI0_Button2_signal
  
   , LI0_error
  
  
} l_signal_handle; 
/**********************************************************************/
/*****************               Frame             ********************/
/**********************************************************************/
/* Number of frames */
#define LIN_NUM_OF_FRMS  5 
/* List of frames */
typedef enum {
/* All frames for master node */

   /* Interface_name = LI0 */

   LI0_f_adc

   , LI0_f_buttons
  
   , LI0_f_error
  
   , LI0_MasterReq
  
   , LI0_SlaveResp
  
  
} l_frame_handle; 
 
/**********************************************************************/
/***************             Configuration          *******************/
/**********************************************************************/
/* Size of configuration in ROM and RAM used for interface: LI1 */
#define LIN_SIZE_OF_CFG  7 
#define LIN_CFG_FRAME_NUM  3 
/*********************************************************************
 * global macros
 *********************************************************************/
#define l_bool_rd(SIGNAL) l_bool_rd_##SIGNAL()
#define l_bool_wr(SIGNAL, A) l_bool_wr_##SIGNAL(A)
#define l_u8_rd(SIGNAL) l_u8_rd_##SIGNAL()
#define l_u8_wr(SIGNAL, A) l_u8_wr_##SIGNAL(A)
#define l_u16_rd(SIGNAL) l_u16_rd_##SIGNAL()
#define l_u16_wr(SIGNAL, A) l_u16_wr_##SIGNAL(A)
#define l_bytes_rd(SIGNAL, start, count, data)  l_bytes_rd_##SIGNAL(start, count, data)
#define l_bytes_wr(SIGNAL, start, count, data) l_bytes_wr_##SIGNAL(start, count, data)
#define l_flg_tst(FLAG) l_flg_tst_##FLAG()
#define l_flg_clr(FLAG) l_flg_clr_##FLAG()
#define LIN_TEST_BIT(A,B) ((l_bool)((((A) & (1U << (B))) != 0U) ? 1U : 0U))
#define LIN_SET_BIT(A,B)                      ((A) |= (l_u8) (1U << (B)))
#define LIN_CLEAR_BIT(A,B)               ((A) &= ((l_u8) (~(1U << (B)))))
#define LIN_BYTE_MASK  ((l_u16)(((l_u16)((l_u16)1 << CHAR_BIT)) - (l_u16)1))
#define LIN_FRAME_LEN_MAX                                             10U

/* Returns the low byte of the 32-bit value    */
#define BYTE_0(n)                              ((l_u8)((n) & (l_u8)0xFF))
/* Returns the second byte of the 32-bit value */
#define BYTE_1(n)                        ((l_u8)(BYTE_0((n) >> (l_u8)8)))
/* Returns the third byte of the 32-bit value  */
#define BYTE_2(n)                       ((l_u8)(BYTE_0((n) >> (l_u8)16)))
/* Returns high byte of the 32-bit value       */
#define BYTE_3(n)                       ((l_u8)(BYTE_0((n) >> (l_u8)24)))

/*
 * defines for signal access
 */


#define LIN_BYTE_OFFSET_LI0_ADC_signal    0
#define LIN_BIT_OFFSET_LI0_ADC_signal    0
#define LIN_SIGNAL_SIZE_LI0_ADC_signal    8
#define LIN_FLAG_BYTE_OFFSET_LI0_ADC_signal    0
#define LIN_FLAG_BIT_OFFSET_LI0_ADC_signal    1
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_ADC_signal    0

#define LIN_BYTE_OFFSET_LI0_Button1_signal    1
#define LIN_BIT_OFFSET_LI0_Button1_signal    0
#define LIN_SIGNAL_SIZE_LI0_Button1_signal    8
#define LIN_FLAG_BYTE_OFFSET_LI0_Button1_signal    1
#define LIN_FLAG_BIT_OFFSET_LI0_Button1_signal    1
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_Button1_signal    1

#define LIN_BYTE_OFFSET_LI0_Button2_signal    2
#define LIN_BIT_OFFSET_LI0_Button2_signal    0
#define LIN_SIGNAL_SIZE_LI0_Button2_signal    8
#define LIN_FLAG_BYTE_OFFSET_LI0_Button2_signal    1
#define LIN_FLAG_BIT_OFFSET_LI0_Button2_signal    2
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_Button2_signal    1

#define LIN_BYTE_OFFSET_LI0_error    3
#define LIN_BIT_OFFSET_LI0_error    0
#define LIN_SIGNAL_SIZE_LI0_error    1
#define LIN_FLAG_BYTE_OFFSET_LI0_error    2
#define LIN_FLAG_BIT_OFFSET_LI0_error    1
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_error    2




#define LIN_FLAG_BYTE_OFFSET_LI0_f_adc             0
#define LIN_FLAG_BIT_OFFSET_LI0_f_adc              0

#define LIN_FLAG_BYTE_OFFSET_LI0_f_buttons             1
#define LIN_FLAG_BIT_OFFSET_LI0_f_buttons              0

#define LIN_FLAG_BYTE_OFFSET_LI0_f_error             2
#define LIN_FLAG_BIT_OFFSET_LI0_f_error              0


/**********************************************************************/
/***************        Static API Functions        *******************/
/**********************************************************************/
/*
 * the static signal access macros
 */


/* static access macros for signal LI0_ADC_signal */
#define l_u8_rd_LI0_ADC_signal() \
    ((l_u8) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_ADC_signal] & \
    (((1U << LIN_SIGNAL_SIZE_LI0_ADC_signal) - 1) \
    << LIN_BIT_OFFSET_LI0_ADC_signal )) >> LIN_BIT_OFFSET_LI0_ADC_signal))
#define l_u8_wr_LI0_ADC_signal(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_ADC_signal] = \
    ((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_ADC_signal] & \
    ((l_u8) (~(((1U << LIN_SIGNAL_SIZE_LI0_ADC_signal) - 1) << LIN_BIT_OFFSET_LI0_ADC_signal)))))  | \
    ((l_u8) ((((1U << LIN_SIGNAL_SIZE_LI0_ADC_signal) - 1) & (A)) << LIN_BIT_OFFSET_LI0_ADC_signal))));\
    lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_ADC_signal] = 1;}

/* static access macros for signal LI0_Button1_signal */
#define l_u8_rd_LI0_Button1_signal() \
    ((l_u8) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Button1_signal] & \
    (((1U << LIN_SIGNAL_SIZE_LI0_Button1_signal) - 1) \
    << LIN_BIT_OFFSET_LI0_Button1_signal )) >> LIN_BIT_OFFSET_LI0_Button1_signal))
#define l_u8_wr_LI0_Button1_signal(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Button1_signal] = \
    ((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Button1_signal] & \
    ((l_u8) (~(((1U << LIN_SIGNAL_SIZE_LI0_Button1_signal) - 1) << LIN_BIT_OFFSET_LI0_Button1_signal)))))  | \
    ((l_u8) ((((1U << LIN_SIGNAL_SIZE_LI0_Button1_signal) - 1) & (A)) << LIN_BIT_OFFSET_LI0_Button1_signal))));\
    lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_Button1_signal] = 1;}

/* static access macros for signal LI0_Button2_signal */
#define l_u8_rd_LI0_Button2_signal() \
    ((l_u8) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Button2_signal] & \
    (((1U << LIN_SIGNAL_SIZE_LI0_Button2_signal) - 1) \
    << LIN_BIT_OFFSET_LI0_Button2_signal )) >> LIN_BIT_OFFSET_LI0_Button2_signal))
#define l_u8_wr_LI0_Button2_signal(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Button2_signal] = \
    ((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Button2_signal] & \
    ((l_u8) (~(((1U << LIN_SIGNAL_SIZE_LI0_Button2_signal) - 1) << LIN_BIT_OFFSET_LI0_Button2_signal)))))  | \
    ((l_u8) ((((1U << LIN_SIGNAL_SIZE_LI0_Button2_signal) - 1) & (A)) << LIN_BIT_OFFSET_LI0_Button2_signal))));\
    lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_Button2_signal] = 1;}

/* static access macros for signal LI0_error */
#define l_bool_rd_LI0_error() \
   	(LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_error], \
   	LIN_BIT_OFFSET_LI0_error))
#define l_bool_wr_LI0_error(A) \
	{(A) ? \
  	(LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_error], \
  	LIN_BIT_OFFSET_LI0_error)):\
  	(LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_error], \
  	LIN_BIT_OFFSET_LI0_error));\
  	lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_error] = 1;}



/* Signal flag APIs */


#define l_flg_tst_LI0_ADC_signal_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_ADC_signal],\
         LIN_FLAG_BIT_OFFSET_LI0_ADC_signal)
#define l_flg_clr_LI0_ADC_signal_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_ADC_signal],\
         LIN_FLAG_BIT_OFFSET_LI0_ADC_signal)

#define l_flg_tst_LI0_Button1_signal_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Button1_signal],\
         LIN_FLAG_BIT_OFFSET_LI0_Button1_signal)
#define l_flg_clr_LI0_Button1_signal_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Button1_signal],\
         LIN_FLAG_BIT_OFFSET_LI0_Button1_signal)

#define l_flg_tst_LI0_Button2_signal_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Button2_signal],\
         LIN_FLAG_BIT_OFFSET_LI0_Button2_signal)
#define l_flg_clr_LI0_Button2_signal_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Button2_signal],\
         LIN_FLAG_BIT_OFFSET_LI0_Button2_signal)

#define l_flg_tst_LI0_error_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_error],\
         LIN_FLAG_BIT_OFFSET_LI0_error)
#define l_flg_clr_LI0_error_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_error],\
         LIN_FLAG_BIT_OFFSET_LI0_error)



/* Frame flag APIs */

#define l_flg_tst_LI0_f_adc_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_f_adc],\
         LIN_FLAG_BIT_OFFSET_LI0_f_adc)
#define l_flg_clr_LI0_f_adc_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_f_adc],\
         LIN_FLAG_BIT_OFFSET_LI0_f_adc)

#define l_flg_tst_LI0_f_buttons_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_f_buttons],\
         LIN_FLAG_BIT_OFFSET_LI0_f_buttons)
#define l_flg_clr_LI0_f_buttons_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_f_buttons],\
         LIN_FLAG_BIT_OFFSET_LI0_f_buttons)

#define l_flg_tst_LI0_f_error_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_f_error],\
         LIN_FLAG_BIT_OFFSET_LI0_f_error)
#define l_flg_clr_LI0_f_error_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_f_error],\
         LIN_FLAG_BIT_OFFSET_LI0_f_error)



/* INTERFACE MANAGEMENT */

#define l_ifc_init_LI0() l_ifc_init(LI0)



#define l_ifc_wake_up_LI0() l_ifc_wake_up(LI0)



#define l_ifc_rx_LI0() l_ifc_rx(LI0)



#define l_ifc_tx_LI0() l_ifc_tx(LI0)



#define l_ifc_aux_LI0() l_ifc_aux(LI0)



#define l_ifc_read_status_LI0() l_ifc_read_status(LI0)


#endif    /* _LIN_CFG_H_ */
