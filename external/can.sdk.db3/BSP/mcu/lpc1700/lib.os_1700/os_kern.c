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
// $Id: os_kern.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os_local.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void os_execute (TskFunc tsk_func,TskPri tsk_pri,UInt16 stk_size,void * hook_arg,StrConst * tsk_name,void * tsk_arg,UInt16 sys_stk_size)
{
  IRQ_DIS_GLOBAL ;

  os_kern_init_hw_1 () ;

  os_q_init (&os_q_eligible) ;

  os_q_init (cast_tq_to_q(&os_q_timer)) ;

  os_sys_stk  = os_kern_mem_request_stk (sys_stk_size , & os_sys_stk_limits ) ;

  #if OS_USE_IDLE_STACK == 1

  os_idle_stk = os_kern_mem_request_stk (sys_stk_size , & os_idle_stk_limits) ;

  #endif

  os_kern_init_hw_2 () ;

  if (os_sys_init_func)  os_sys_init_func () ;

  create_main :
    {
      os_tsk_running = os_kern_tsk_create_v (tsk_func , (VARG) tsk_pri , (VARG) stk_size , hook_arg , tsk_name) ;

      os_kern_tsk_start  (os_tsk_running , tsk_arg ) ;
    }

  os_kern_tsk_schedule () ;
}

//--------------------------------------------------------------------------//

void os_service_timer (void)
{
  #if OS_COMPONENT_PTE == 1

  os_k_pte_service () ;

  #endif

  #if OS_COMPONENT_GTE == 1

  os_k_gte_service () ;

  #endif

  os_k_tmr_service () ;
}

/*--------------------------------------------------------------------------*/

void * os_kern_mem_request_stk (UInt16 mem_size,StkLimits * stk_limits)
{
  UInt8 * stk_beg ;
  UInt8 * stk_end ;

  unsigned count = OS_STK_FENCE_SIZE ;

  mem_size += os_kern_mem_request_stk_extra () ;

  stk_beg = (UInt8 *) os_mem_request (mem_size + 2*OS_STK_FENCE_SIZE) ;
  stk_end = stk_beg + (mem_size + OS_STK_FENCE_SIZE) ;

  do
    {
      *stk_beg++ = OS_STK_FENCE_MARK_BEG ;
      *stk_end++ = OS_STK_FENCE_MARK_END ;
    }
  while (--count) ;

  stk_limits->stk_beg = stk_beg ;
  stk_limits->stk_end = stk_beg + (mem_size - 1) ;

  return os_kern_stk_ptr_adjust (stk_beg,mem_size) ;
}

/*--------------------------------------------------------------------------*/

void os_kern_tsk_schedule (void)
{
  PQueueNode * tsk_next = os_pq_head (&os_q_eligible) ;

  if (!tsk_next)
    {
      if (!os_tsk_running)
        {
          os_kern_tsk_idle () ;
        }
    }
  else if (!os_tsk_running)
    {
      os_pq_remove (tsk_next) ;

      os_tsk_running = cast_pq_to_tsk (tsk_next) ;
    }
  else if (tsk_next->priority > cast_tsk_to_pq (os_tsk_running)->priority)
    {
      os_pq_remove (tsk_next) ;

      os_eligible_insert_head (os_tsk_running) ;

      os_tsk_running = cast_pq_to_tsk (tsk_next) ;
    }

  os_tsk_running->tsk_state = OS_TSK_RUNNING ;

  os_kern_context_load :
    {
      os_tsk_local = os_tsk_running->tsk_local ;

      #if OS_USE_HOOK_CONTEXT == 1

      os_tsk_running->running_hook_load (os_tsk_running , os_tsk_running->tsk_hook_arg) ;

      #endif

      os_stk_frame = os_tsk_running->stk_frame ;
    }

  #if OS_USE_HOOK_CONTEXT == 1

  os_kern_context_hooks :
    {
      os_running_hook_save              = os_tsk_hook_save ;
      os_tsk_running->running_hook_load = os_tsk_hook_load ;
    }

  #endif

  os_kern_tsk_execute_hw () ;
}

/*--------------------------------------------------------------------------*/

void os_kern_tsk_start (Tsk tsk,void * tsk_arg)
{
  tsk->stk_frame         = os_kern_tsk_start_hw (tsk->stk_top,tsk->tsk_func,tsk_arg) ;

  #if OS_USE_HOOK_CONTEXT == 1

  tsk->running_hook_load = os_tsk_hook_start ;

  #endif

  tsk->tsk_timeout_func  = 0 ;
}

/*--------------------------------------------------------------------------*/

void os_kern_null_idle (void)
{
}

/*--------------------------------------------------------------------------*/

#if OS_USE_HOOK_CONTEXT == 1

void os_kern_null_hook (Tsk tsk , void * hook_arg)
{
}

#endif

//--------------------------------------------------------------------------//

void os_kern_null_mem_fail (UInt16 size_requested)
{
}

/*--------------------------------------------------------------------------*/

Tsk os_kern_tsk_create (TskFunc tsk_func,va_list tsk_info)
{
  TskPri  tsk_pri  = (TskPri) va_arg (tsk_info,VARG)  ;
  UInt16  stk_size = (UInt16) va_arg (tsk_info,VARG)  ;

  Tsk     tsk = (Tsk) os_mem_request (sizeof *tsk) ;

  tsk_init :
    {
      tsk->stk_top        = (UInt8 *) os_kern_mem_request_stk (stk_size,&tsk->stk_limits) ;
  
      tsk->tsk_func       = tsk_func ;

      #if OS_USE_HOOK_CONTEXT == 1
      
      tsk->tsk_hook_arg   = va_arg (tsk_info,void *) ;

      #else

      va_arg (tsk_info,void *) ;         

      #endif
      
      tsk->tsk_name       = va_arg (tsk_info,StrConst *) ;

      tsk->is_timer_used  = FALSE   ;

      tsk->tsk_state      = OS_TSK_STOPPED ;

      tsk->tsk_local      = NULL ;

      tsk->tsk_priority_a = cast_tsk_to_pq(tsk)->priority = tsk_pri ;

      tsk->tsk_user_ptr   = 0 ;

      os_q_init (&tsk->q_msgs     ) ;
      os_q_init (&tsk->q_wait_stop) ;
    }

  tsk_link :
    {
      static Tsk tsk_last ;

      if (os_tsk_list)
        {
          tsk_last->tsk_list = tsk ;
        }
      else
        {
          os_tsk_list = tsk ;
        }

      tsk_last = tsk ;

      tsk->tsk_list = NULL ;
    }

  #if OS_USE_HOOK_CONTEXT == 1

  os_tsk_hook_create (tsk , tsk->tsk_hook_arg) ;

  #endif

  return tsk ;
}
//---------------------------------------------------------------------------//

Tsk os_kern_tsk_create_v (TskFunc tsk_func,...)
{
  Tsk tsk ;

  va_list arg ;

  va_start (arg,tsk_func) ;
    {
      tsk = os_kern_tsk_create (tsk_func,arg) ;
    }
  va_end   (arg) ;
  return tsk ;
}

/*--------------------------------------------------------------------------*/

void os_kern_process_cmd (K_Func k_func,void * arg0,va_list args)
{
  os_kern_context_save :
    {
      os_tsk_running->stk_frame = os_stk_frame ;

      #if OS_USE_HOOK_CONTEXT == 1
       
      os_running_hook_save (os_tsk_running , os_tsk_running->tsk_hook_arg) ;

      #endif

      os_tsk_running->tsk_local = os_tsk_local ;
    }

  k_func (arg0,args) ;

  os_kern_tsk_schedule () ;
}

/*--------------------------------------------------------------------------*/

void os_k_tmr_service (void)
{
  TQueueNode * tsk_head = os_tq_head (&os_q_timer) ;

  if (tsk_head && --tsk_head->timeout == 0)
    do
      {
        Tsk tsk = cast_tq_to_tsk(tsk_head) ;

        os_timer_remove_z (tsk) ;

        tsk->k_tmo_func(tsk) ;

        if (tsk->tsk_timeout_func)
          {
            tsk->tsk_timeout_func (tsk->tsk_timeout_func_arg) ;
          }

        os_eligible_insert_tail (tsk) ;
      }
    while ((tsk_head = os_tq_head(&os_q_timer)) && tsk_head->timeout == 0) ;
}

