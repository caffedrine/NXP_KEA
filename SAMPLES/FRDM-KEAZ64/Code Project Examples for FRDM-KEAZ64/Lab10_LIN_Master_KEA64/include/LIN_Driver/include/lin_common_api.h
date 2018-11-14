/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2008-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************/
/** @addtogroup common_core_api_group
* @{ */
/**************************************************************************//**
*
* @file      lin_common_api.h
*
* @author    FPT Software
*
* @brief     Common LIN API functions
*
******************************************************************************/ 
/******************************************************************************
 *
 * History:
 *
 * 20090407		v1.0	First version
 *
 *****************************************************************************/
 
/** @} */

#ifndef _LIN_COMMON_API_H
#define _LIN_COMMON_API_H

#include "LIN_Driver/lowlevel/lin.h"
/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_bool l_sys_init ( void )
* @brief This function performs the initialization of the LIN core
*
* @return #l_bool
* 
* @SDD_ID LIN_SDD_238
* @endif
*
* @details
*   This function performs the initialization of the LIN core
*
* @see  #lin_lld_setup_xgate
* @see  #lin_lld_timer_init
*
* @addtogroup driver_cluster_group
*//*END*----------------------------------------------------------------------*/
l_bool        l_sys_init (void);
  
/** @} */
#if LIN_MODE == _MASTER_MODE_
/** @addtogroup schedule_management_group 
* @{ */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void l_sch_set (l_ifc_handle iii, l_schedule_handle schedule_iii, l_u8 entry)
* @brief Set up the next schedule to be followed by the l_sch_tick
*
* @param iii <B>[IN]</B> interface name 
* @param schedule_iii <B>[IN]</B> schedule table for interface
* @param entry <B>[IN]</B> entry to be set
*
* @return #void
* 
* @SDD_ID LIN_SDD_249
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   Set up the next schedule to be followed by the l_sch_tick 
*   function for a certain interface. The new schedule will be 
*   activated as soon as the current schedule reaches 
*   its next schedule entry point
*
*//*END*----------------------------------------------------------------------*/
void          l_sch_set (l_ifc_handle iii, l_schedule_handle schedule_iii, l_u8 entry); 

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 l_sch_tick (l_ifc_handle iii)
* @brief  follows a schedule
*
* @param iii <B>[IN]</B> interface name 
*
* @return #l_u8
* 
* @SDD_ID LIN_SDD_250
* @endif
*
* @details
*   This function follows a schedule. When a frame becomes due, its 
*   transmission is initiated. When the end of the current schedule is reached, 
*   this function starts again at the beginning of the schedule.
*
* @see #lin_tick_callback_handler
*//*END*----------------------------------------------------------------------*/
l_u8          l_sch_tick (l_ifc_handle iii);
/** @} */

/** @addtogroup interface_management_group 
* @{ */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void l_ifc_goto_sleep (l_ifc_handle iii)
* @brief Request slave nodes  to enter bus sleep mode
*
* @param iii <B>[IN]</B> interface name 
*
* @return #void
* 
* @SDD_ID LIN_SDD_252
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   Request slave nodes on the cluster connected to the interface to enter 
*   bus sleep mode by issuing one go to sleep command.
*
* @see  #l_sch_set
*//*END*----------------------------------------------------------------------*/
void          l_ifc_goto_sleep (l_ifc_handle iii);
/** @} */
#endif /* LIN_MODE == _MASTER_MODE_ */
/** @addtogroup interface_management_group 
* @{ */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_bool l_ifc_init (l_ifc_handle iii)
* @brief Initialize the controller specified by name
*
* @param iii <B>[IN]</B> interface name
*
* @return #l_bool
* 
* @SDD_ID LIN_SDD_251
* @endif
*
* @details
*   Initialize the controller specified by name, i.e. sets up internal 
*   functions such as the baud rate. The default schedule set by the l_ifc_init 
*   call will be the L_NULL_SCHEDULE where no frames will be sent and received.
*   This is the first call a user must perform, before using any other interface 
*   related LIN API functions.
*   The function returns zero if the initialization was successful and non-zero if failed.
*
* @see  #lin_lld_init
*//*END*----------------------------------------------------------------------*/
l_bool        l_ifc_init (l_ifc_handle iii);
/* l_ifc_ioctl  */

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void l_ifc_wake_up (l_ifc_handle iii)
* @brief Transmit the wake up signal. 
*
* @param iii <B>[IN]</B> interface name
*
* @return #void
* 
* @SDD_ID LIN_SDD_253
* @endif
*
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   Transmit the wake up signal. 
*
* @see  #lin_lld_tx_wake_up
*//*END*----------------------------------------------------------------------*/
void          l_ifc_wake_up (l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void l_ifc_rx (l_ifc_handle iii)
* @brief Receive one frame 
*
* @param iii <B>[IN]</B> interface name
*
* @return #void
* 
* @SDD_ID LIN_SDD_254
* @endif
*
* @details
*   Receive one frame 
*
*//*END*----------------------------------------------------------------------*/
void          l_ifc_rx (l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void l_ifc_tx (l_ifc_handle iii)
* @brief Transmit one frame 
*
* @param iii <B>[IN]</B> interface name
*
* @return #void
* 
* @SDD_ID LIN_SDD_255
* @endif
*
* @details
*   Transmit one frame 
*
*//*END*----------------------------------------------------------------------*/
void          l_ifc_tx (l_ifc_handle iii); 

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u16 l_ifc_read_status (l_ifc_handle iii)
* @brief This function will return the status of the previous communication 
*
* @param iii <B>[IN]</B> interface name
*
* @return #l_u16
* 
* @SDD_ID LIN_SDD_259
* @endif
*
* @local_var
*   -# <B>#l_u16</B> <I>tmp_word_status</I>
*   -# <B>#lin_configuration</B> <I>conf</I>
*
* @static_global_var
*   -# <B>#lin_word_status</B>
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   This function will return the status of the previous communication 
*
*//*END*----------------------------------------------------------------------*/
l_u16         l_ifc_read_status (l_ifc_handle iii);
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void l_ifc_aux (l_ifc_handle iii)
* @brief synchronize to the break/sync field
*
* @param iii <B>[IN]</B> interface name
*
* @return #void
* 
* @SDD_ID LIN_SDD_256
* @endif
*
* @details
*   This function may be used in the slave nodes to synchronize to the 
*   break/sync field sequence transmitted by the master node on the 
*   interface specified by name. 
*
*//*END*----------------------------------------------------------------------*/

void          l_ifc_aux (l_ifc_handle iii);
/** @} */

/** @addtogroup call_out_group 
* @{ */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u16 l_sys_irq_disable (l_ifc_handle iii)
* @brief Disable LIN related IRQ 
*
* @param iii <B>[IN]</B> interface name
*
* @return #l_u16
* 
* @SDD_ID LIN_SDD_257
* @endif
*
* @details
*   Disable LIN related IRQ 
*
* @see #lin_lld_int_disable
*//*END*----------------------------------------------------------------------*/
l_u16         l_sys_irq_disable (l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void l_sys_irq_restore (l_ifc_handle iii)
* @brief Enable LIN related IRQ 
*
* @param iii <B>[IN]</B> interface name
*
* @return #void
* 
* @SDD_ID LIN_SDD_258
* @endif
*
* @details
*   Enable LIN related IRQ 
*
* @see #lin_lld_int_enable
*//*END*----------------------------------------------------------------------*/
void          l_sys_irq_restore (l_ifc_handle iii);
/** @} */
/** @addtogroup common_core_api_group
* @{ */
/** @} */
#endif  /* _LIN_COMMON_API_H */
