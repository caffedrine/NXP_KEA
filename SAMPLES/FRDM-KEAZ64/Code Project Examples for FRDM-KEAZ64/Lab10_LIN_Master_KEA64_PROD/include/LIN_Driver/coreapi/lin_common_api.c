/****************************************************************************** 
* Freescale Semiconductor Inc. 
* (c) Copyright 2008-2009 Freescale Semiconductor, Inc. 
* ALL RIGHTS RESERVED. 
*****************************************************************************/ 

/** @addtogroup common_core_api_group
* @{ */

/**************************************************************************//** 
* 
* @file      lin_common_api.c 
* 
* @author    FPT Software
*  
* @brief     Common LIN API functions
* 
* 
******************************************************************************/
/******************************************************************************
 *
 * History:
 *
 * 20090408		v1.0	First version
 *
 *****************************************************************************/
 
/** @} */

#include "LIN_Driver/include/lin_common_api.h"
#include "LIN_Driver/include/lin_common_proto.h"
#include "LIN_Driver/lowlevel/lin.h"

#if (XGATE_SUPPORT == 1)
#include "lin_lld_xgate.h"
#endif /* End (XGATE_SUPPORT == 1) */

/*-------------------------------FUNCTION COMMON ---------------------------*/
/** @addtogroup driver_cluster_group 
* @{ */
l_bool l_sys_init ()
{
#if XGATE_SUPPORT == 1
  lin_lld_setup_xgate();
#endif /* End (XGATE_SUPPORT == 1) */
  lin_lld_timer_init();
  return 0;
} /* end of l_sys_init() */
/** @} */

#if LIN_MODE == _MASTER_MODE_
/** @addtogroup schedule_management_group 
* @{ */
void l_sch_set
    (
        /* [IN] interface name */
        l_ifc_handle iii,
        /* [IN] schedule table for interface */
        l_schedule_handle schedule_iii,
        /* [IN] entry to be set */
        l_u8 entry
	)
{
  lin_configuration const *conf;
  /* Get current configuration */
  conf = &lin_ifc_configuration[iii];
  /* save the new schedule which is activated after the      */
  /* current schedule reaches its next schedule entry point  */
  *(conf->previous_schedule_id) = *(conf->active_schedule_id);
  /* Insert new schedule at that point of the current schdule*/
  *(conf->active_schedule_id) = (l_u8)schedule_iii;
  /*Assign the start entry of new schedule table by argument entry */
  conf->schedule_start_entry[schedule_iii] = entry;
} /* end of l_sch_set() */

l_u8 l_sch_tick 
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
  return lin_tick_callback_handler(iii);
} /* end of l_sch_tick() */
/** @} */
#endif /* End LIN_MODE == _MASTER_MODE_*/

/** @addtogroup interface_management_group 
* @{ */
l_bool l_ifc_init 
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
#if LIN_MODE == _SLAVE_MODE_
  return lin_lld_init();
#else
  return lin_lld_init(iii);
#endif /* End LIN_MODE == _SLAVE_MODE_*/
} /* end of l_ifc_init() */

void l_ifc_goto_sleep 
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
  /* If node is master node */
#if LIN_MODE == _MASTER_MODE_
  /* Get current configuration */
  lin_configuration const *conf; 
  /* Get current configuration */
  conf = &lin_ifc_configuration[iii];
  /* Set active schedule as GOTO_SLEEP_SCHEDULE */
  l_sch_set(iii, (l_schedule_handle)(conf->schedule_start + 1), 0); 
  /* Update goto_sleep flag */
  (*conf->goto_sleep_flg) = 1;
  
#endif /*End LIN_MODE == _MASTER_MODE_ */    
} /* end of l_ifc_goto_sleep() */

void l_ifc_wake_up
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
#if LIN_MODE == _SLAVE_MODE_
  lin_lld_tx_wake_up();
#else
  /* Clear goto sleep flag */
  if(1 == (*lin_ifc_configuration[iii].goto_sleep_flg)){
    (*lin_ifc_configuration[iii].goto_sleep_flg) = 0;
  }
  
  /* Send wakeup signal */
  lin_lld_tx_wake_up(iii);

#endif /* End (LIN_MODE == _SLAVE_MODE_) */ 
} /* end of l_ifc_wake_up() */

void l_ifc_rx
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{

} /* end of l_ifc_rx() */
  
void l_ifc_tx 
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{

} /* end of l_ifc_tx() */

void l_ifc_aux 
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
 
} /* end of l_ifc_aux() */
l_u16 l_ifc_read_status
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
  static l_u16 tmp_word_status;
#if LIN_MODE == _SLAVE_MODE_
  tmp_word_status = lin_word_status.word;
  /* Clear word status */
  lin_word_status.word = 0;
  /* Clear save configuration flag value */
  lin_save_configuration_flg = 0;
#else
  lin_configuration const *conf;
  conf = &lin_ifc_configuration[iii];
  tmp_word_status = *((l_u16*)(conf->word_status));
  /* Read-reset call; meaning that after the call has returned, 
  the status word is set to 0*/
  /* Clear word status */
  *((l_u16*)(conf->word_status)) = 0;
  /* Clear save configuration flag value */
  lin_save_configuration_flg[iii] = 0;
#endif /* End (LIN_MODE == _SLAVE_MODE_) */

  return (tmp_word_status);
} /* end of l_ifc_read_status() */
/** @} */

/** @addtogroup call_out_group 
* @{ */
l_u16 l_sys_irq_disable 
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
#if LIN_MODE == _SLAVE_MODE_
  return lin_lld_int_disable();
#else
  return lin_lld_int_disable(iii);
#endif /* End (LIN_MODE == _SLAVE_MODE_) */
} /* end of l_sys_irq_disable() */

void l_sys_irq_restore 
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
#if LIN_MODE == _SLAVE_MODE_
  lin_lld_int_enable();
#else
  lin_lld_int_enable(iii);
#endif /* End (LIN_MODE == _SLAVE_MODE_) */
} /* end of l_sys_irq_restore() */
 /** @} */
