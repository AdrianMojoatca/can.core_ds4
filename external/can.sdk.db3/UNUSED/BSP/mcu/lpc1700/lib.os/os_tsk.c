/****************************************************************************/
/*                                                                          */
/*                         A-Task Real-Time Kernel                          */
/*                                                                          */
/*                    Copyright (C) 2000 - Acacetus Inc                     */
/*                                                                          */
/*                           All rights reserved                            */
/*                                                                          */
/*                             www.acacetus.com                             */
/*                                                                          */
/****************************************************************************/

/*==========================================================================*/
// $Id: os_tsk.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os_private.h"
 
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void os_k_tsk_create            (void * arg0 , va_list args) ;
static void os_k_tsk_start             (void * arg0 , va_list args) ;
static void os_k_tsk_start_s           (void * arg0 , va_list args) ;
static void os_k_tsk_stop              (void * arg0 , va_list args) ;
static void os_k_tsk_stop_s            (void * arg0 , va_list args) ;
static void os_k_tsk_wait              (void * arg0 , va_list args) ;
static void os_k_tsk_wait_stop         (void * arg0 , va_list args) ;
static void os_k_tsk_release           (void * arg0 , va_list args) ;
static void os_k_tsk_suspend           (void * arg0 , va_list args) ;
static void os_k_tsk_resume            (void * arg0 , va_list args) ;
static void os_k_tsk_set_pri           (void * arg0 , va_list args) ;

static void os_k_tsk_wait_timeout      (Tsk tsk) ;
static void os_k_tsk_suspend_timeout   (Tsk tsk) ;
static void os_k_tsk_wait_stop_timeout (Tsk tsk) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*=============================[ TASK ENTRIES ]=============================*/

Tsk os_tsk_create_v (TskFunc tsk_func , ...)
{
  va_list   args ;

  va_start (args , tsk_func) ;
    {
      os_kern_process_cmd_hw (os_k_tsk_create , & tsk_func,args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.tsk ;
}

/*--------------------------------------------------------------------------*/

Boolean os_tsk_start_v (Tsk tsk , ...)
{
  va_list   args ;

  va_start (args , tsk) ;
    {
      os_kern_process_cmd_hw (os_k_tsk_start , & tsk,args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.status ;
}

/*--------------------------------------------------------------------------*/

Boolean os_tsk_start_i (Tsk tsk , void * tsk_arg)
{
  if (tsk->tsk_state == OS_TSK_STOPPED)
    {
      tsk->tsk_stop_func = 0 ;

      cast_tsk_to_pq (tsk)->priority = tsk->tsk_priority_a ;

      os_kern_tsk_start (tsk , tsk_arg) ;

      os_eligible_insert_tail (tsk) ;

      return TRUE ;
    }

  return FALSE ;
} 

/*--------------------------------------------------------------------------*/

Boolean os_tsk_start_s_v (Tsk tsk , ...)
{
  va_list   args ;

  va_start (args , tsk) ;
    {
      os_kern_process_cmd_hw (os_k_tsk_start_s , & tsk,args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.status ;
}

/*--------------------------------------------------------------------------*/

Boolean os_tsk_start_s_i (Tsk tsk , void * tsk_arg , TskStopFunc tsk_stop_func , void * tsk_stop_arg)
{
  if (os_tsk_start_i (tsk , tsk_arg))
    {
      tsk->tsk_stop_func = tsk_stop_func ;
      tsk->tsk_stop_arg  = tsk_stop_arg  ;

      return TRUE ;
    }

  return FALSE ;
}

/*--------------------------------------------------------------------------*/

void os_tsk_stop (void)
{
  IRQ_DIS_GLOBAL ;

  #if OS_USE_HOOK_CONTEXT == 1

  os_running_hook_save = os_tsk_hook_stop ;

  #endif
  
  os_kern_process_cmd_hw (os_k_tsk_stop , NULL , os_va_list_void) ;
}

/*--------------------------------------------------------------------------*/

void * os_tsk_suspend (TskTimeout timeout)
{
  os_kern_process_cmd_hw (os_k_tsk_suspend , & timeout,os_va_list_void) ;

  return os_tsk_running->arg.tsk_resume_arg ;
}

/*--------------------------------------------------------------------------*/

Boolean os_tsk_resume_v (Tsk tsk , ...)
{
  va_list   args ;

  va_start (args , tsk) ;
    {
      os_kern_process_cmd_hw (os_k_tsk_resume , & tsk,args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.status ;
}

/*--------------------------------------------------------------------------*/

Boolean os_tsk_resume_i (Tsk tsk , void * tsk_resume_arg)
{
  if (tsk && tsk->tsk_state == OS_TSK_SUSPENDED)
    {
      os_timer_remove (tsk) ;
      
      tsk->arg.tsk_resume_arg = tsk_resume_arg ;

      os_eligible_insert_tail (tsk) ;

      return TRUE ;
    }

  return FALSE ;
}

/*--------------------------------------------------------------------------*/

void os_tsk_stop_s_v (TskStopFunc tsk_stop_func , ...)
{
  va_list   args ;

  IRQ_DIS_GLOBAL ;

  #if OS_USE_HOOK_CONTEXT == 1

  os_running_hook_save = os_tsk_hook_stop ;

  #endif

  va_start (args , tsk_stop_func) ;
    {
      os_kern_process_cmd_hw (os_k_tsk_stop_s , & tsk_stop_func , args) ;
    }
  va_end   (args) ;
}

/*--------------------------------------------------------------------------*/

void os_tsk_wait (TskTimeout timeout)
{ 
  os_kern_process_cmd_hw (os_k_tsk_wait , & timeout , os_va_list_void) ;
}

/*--------------------------------------------------------------------------*/

Boolean os_tsk_wait_stop_v (Tsk tsk , ...)
{
  va_list   args ;

  va_start (args , tsk) ;
    {
      os_kern_process_cmd_hw (os_k_tsk_wait_stop , & tsk,args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.status ;
}

/*--------------------------------------------------------------------------*/

void os_tsk_release (void)
{
  os_kern_process_cmd_hw (os_k_tsk_release , NULL , os_va_list_void) ;
}

/*--------------------------------------------------------------------------*/

void os_tsk_list_init (Tsk * tsk_list)
{
  ATOMIC (*tsk_list = os_tsk_list) ;
}

/*--------------------------------------------------------------------------*/

void os_tsk_list_next (Tsk * tsk_list)
{
  ATOMIC 
    (
      if (*tsk_list)
        {
          *tsk_list = (*tsk_list)->tsk_list ;
        }
    )
}

/*--------------------------------------------------------------------------*/

void os_tsk_info (Tsk tsk,TskInfo * tsk_info)
{
  if (! tsk) tsk = os_tsk_running ;

  ATOMIC
    (
      tsk_info->tsk_name        = tsk->tsk_name                  ;
      tsk_info->tsk_func        = tsk->tsk_func                  ;
      tsk_info->tsk_state       = tsk->tsk_state                 ;
      tsk_info->tsk_priority    = cast_tsk_to_pq (tsk)->priority ;
      tsk_info->tsk_priority_a  = tsk->tsk_priority_a            ;
      tsk_info->tsk_timeslice_a = tsk->tsk_timeslice_a           ;
    )
}

/*--------------------------------------------------------------------------*/

Tsk os_tsk_current (void)
{
  Tsk tsk_running ;

  ATOMIC (tsk_running = os_tsk_running) ;

  return  tsk_running ;
}

/*--------------------------------------------------------------------------*/

TskPri os_tsk_get_pri (Tsk tsk)
{
  TskPri tsk_pri ;

  if (! tsk) tsk = os_tsk_running ;

  ATOMIC (tsk_pri = cast_tsk_to_pq (tsk)->priority) ;

  return  tsk_pri ;
}

/*--------------------------------------------------------------------------*/

TskPri os_tsk_set_pri (TskPri tsk_pri)
{
  os_kern_process_cmd_hw (os_k_tsk_set_pri , & tsk_pri , os_va_list_void) ;

  return os_tsk_running->arg.tsk_pri ;
}

/*--------------------------------------------------------------------------*/

TskState os_tsk_state (Tsk tsk)
{
  TskState tsk_state ;

  if (!tsk) tsk = os_tsk_running ;

  ATOMIC (tsk_state = tsk->tsk_state) ;

  return  tsk_state ;
}

/*--------------------------------------------------------------------------*/

TskFunc os_tsk_set_func (Tsk tsk , TskFunc tsk_func)
{
  TskFunc old_func ;

  if (! tsk) tsk = os_tsk_running ;

  ATOMIC
    (
      old_func      = tsk->tsk_func ;
      tsk->tsk_func = tsk_func      ;
    )

  return old_func ;
}

/*--------------------------------------------------------------------------*/

TskFunc os_tsk_get_func (Tsk tsk)
{
  TskFunc tsk_func ;

  if (! tsk) tsk = os_tsk_running ;

  ATOMIC (tsk_func = tsk->tsk_func) ;

  return  tsk_func ;
}

/*--------------------------------------------------------------------------*/

void * os_tsk_get_user_ptr (Tsk tsk)
{
  void * user_ptr ;

  if (! tsk) tsk = os_tsk_running ;

  ATOMIC (user_ptr = tsk->tsk_user_ptr) ;

  return  user_ptr ;
}

/*--------------------------------------------------------------------------*/

void * os_tsk_set_user_ptr (Tsk tsk , void * user_ptr)
{
  void * user_ptr_old ;

  if (! tsk) tsk = os_tsk_running ;

  ATOMIC
    (
      user_ptr_old = tsk->tsk_user_ptr ;

      tsk->tsk_user_ptr = user_ptr     ;
    )

  return user_ptr_old ;
}

/*--------------------------------------------------------------------------*/

TskTimeout os_tsk_get_timeslice (Tsk tsk)
{
  TskTimeout timeout_curr ;

  if (! tsk) tsk = os_tsk_running ;

  ATOMIC
    (
      timeout_curr = tsk->tsk_timeslice_a ;
    )

  return timeout_curr ;
}

/*--------------------------------------------------------------------------*/

TskTimeout os_tsk_set_timeslice (Tsk tsk , TskTimeout timeout)
{
  TskTimeout timeout_old ;

  if (! tsk) tsk = os_tsk_running ;

  ATOMIC
    (
      timeout_old = tsk->tsk_timeslice_a ;

      tsk->tsk_timeslice_a = timeout     ;

      if (timeout && (tsk->tsk_timeslice > timeout || tsk->tsk_timeslice == 0))
        {
          tsk->tsk_timeslice = timeout ;
        }
    )

  return timeout_old ;
}

/*============================[ KERNEL ENTRIES ]============================*/

static void os_k_tsk_create (void * arg0 , va_list args)
{
  os_tsk_running->arg.tsk = os_kern_tsk_create (*(TskFunc *)arg0 , args) ;
}

/*--------------------------------------------------------------------------*/

static void os_k_tsk_start (void * arg0 , va_list args)
{
  #define TSK (*(Tsk *) arg0)

  os_tsk_running->arg.status = os_tsk_start_i (TSK , va_arg (args , void *)) ;
  
  #undef  TSK
}

/*--------------------------------------------------------------------------*/

static void os_k_tsk_start_s (void * arg0 , va_list args)
{
  #define TSK (*(Tsk *) arg0)

  if (os_tsk_running->arg.status = os_tsk_start_i (TSK , va_arg (args , void *)))
    {
      TSK->tsk_stop_func = va_arg (args , TskStopFunc) ;
      TSK->tsk_stop_arg  = va_arg (args , void *     ) ;
    }
  
  #undef  TSK
}

/*--------------------------------------------------------------------------*/

static void os_k_tsk_stop (void * arg0 , va_list args)
{
  Tsk         tsk           ;
  TskStopFunc tsk_stop_func ;
  QueueNode * tsk_node      ;

  while (tsk_node = os_q_head (& os_tsk_running->q_wait_stop))
    {
      #define TSK cast_q_to_tsk (tsk_node)

      os_q_unlink     (tsk_node) ;

      os_timer_remove (TSK)      ;

      TSK->arg.status = TRUE ;

      os_eligible_insert_tail (TSK) ;

      #undef  TSK
    }
  
  os_tsk_running->tsk_state = OS_TSK_STOPPED ;

  tsk           = os_tsk_running                ;
  tsk_stop_func = os_tsk_running->tsk_stop_func ;

  os_tsk_running->tsk_stop_func = NULL ;
  os_tsk_running                = NULL ;

  if (tsk_stop_func)
    {
      os_irq_executing = TRUE  ;

      tsk_stop_func (tsk->tsk_stop_arg) ;

      os_irq_executing = FALSE ;
    }
}

//--------------------------------------------------------------------------//

static void os_k_tsk_stop_s (void * arg0 , va_list args)
{
  #define TSK_STOP_FUNC (*(TskStopFunc *) arg0)

  os_tsk_running->tsk_stop_func = TSK_STOP_FUNC          ;
  os_tsk_running->tsk_stop_arg  = va_arg (args , void *) ;
  
  os_k_tsk_stop (0 , args) ;

  #undef  TSK_STOP_FUNC
}

/*--------------------------------------------------------------------------*/

static void os_k_tsk_wait (void * arg0 , va_list args)
{
  #define TIMEOUT (*(TskTimeout *) arg0)

  if (TIMEOUT)
    {
      os_tsk_running->tsk_state  = OS_TSK_WAITING ;
      os_tsk_running->k_tmo_func = os_k_tsk_wait_timeout ;

      os_timer_insert (TIMEOUT) ;

      os_tsk_running = NULL ;
    }

  #undef  TIMEOUT
}

/*--------------------------------------------------------------------------*/

static void os_k_tsk_wait_stop (void * arg0 , va_list args)
{
  #define TSK (*(Tsk *) arg0)

  if (TSK->tsk_state == OS_TSK_STOPPED)
    {
      os_tsk_running->arg.status = TRUE ;
    }
  else
    {
      TskTimeout timeout = (TskTimeout) va_arg (args , VARG) ;

      if (timeout)
        {
          os_tsk_running->k_tmo_func = os_k_tsk_wait_stop_timeout ;
    
          os_timer_insert (timeout) ;
        }

      os_q_link_rhs (& TSK->q_wait_stop , cast_tsk_to_q (os_tsk_running)) ;

      os_tsk_running->tsk_state = OS_TSK_WAIT_TSK_STOP ;
                            
      os_tsk_running = NULL ;
   }

  #undef  TSK
}

/*--------------------------------------------------------------------------*/

static void os_k_tsk_release (void * arg0 , va_list args)
{
  os_eligible_insert_tail (os_tsk_running) ;

  os_tsk_running = NULL ;
}

/*--------------------------------------------------------------------------*/

static void os_k_tsk_suspend (void * arg0 , va_list args)
{
  #define TIMEOUT (*(TskTimeout *) arg0)

  if (TIMEOUT)
    {
      os_tsk_running->k_tmo_func = os_k_tsk_suspend_timeout ;
  
      os_timer_insert (TIMEOUT) ;
    }

  os_tsk_running->tsk_state = OS_TSK_SUSPENDED ;

  os_tsk_running = NULL ;

  #undef  TIMEOUT
}

/*--------------------------------------------------------------------------*/

static void os_k_tsk_resume  (void * arg0 , va_list args)
{
  #define TSK (*(Tsk *) arg0)

  os_tsk_running->arg.status = os_tsk_resume_i (TSK , va_arg (args , void *)) ;

  #undef  TSK
}

/*--------------------------------------------------------------------------*/

static void os_k_tsk_set_pri (void * arg0 , va_list args)
{
  #define ARG_PRI (*(TskPri *) arg0)
  #define TSK_PRI cast_tsk_to_pq (os_tsk_running)->priority

  os_tsk_running->arg.tsk_pri = TSK_PRI ;

  TSK_PRI = ARG_PRI ;

  os_kern_tsk_schedule () ;

  #undef  TSK_PRI
  #undef  ARG_PRI
}

/*===========================[ TIMEOUT FUNCTIONS ]==========================*/

static void os_k_tsk_wait_timeout (Tsk tsk)
{
}

//--------------------------------------------------------------------------//

static void os_k_tsk_suspend_timeout (Tsk tsk)
{
  tsk->arg.tsk_resume_arg = NULL ;
}

//--------------------------------------------------------------------------//

static void os_k_tsk_wait_stop_timeout (Tsk tsk)
{
  os_q_unlink (cast_tsk_to_q (tsk)) ;

  tsk->arg.status = FALSE ;
}

