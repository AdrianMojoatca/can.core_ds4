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
// $Id: os_pip.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static void    os_k_pip_create   (void * arg0 , va_list args) ;
static void    os_k_pip_insert   (void * arg0 , va_list args) ;
static void    os_k_pip_insert_c (void * arg0 , va_list args) ;
static void    os_k_pip_remove   (void * arg0 , va_list args) ;
static void    os_k_pip_remove_c (void * arg0 , va_list args) ;
static void    os_k_pip_flush    (void * arg0 , va_list args) ;

static void    os_k_pip_wait_timeout_ins (Tsk tsk) ;
static void    os_k_pip_wait_timeout_rem (Tsk tsk) ;

static int     fifo_remove      (Pip pip)            ;
static Boolean fifo_insert      (Pip pip , int data) ;

static void    wake_task_insert (Pip pip , QueueNode * tsk_n) ;
static void    wake_task_remove (Pip pip , QueueNode * tsk_n) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*=============================[ TASK ENTRIES ]=============================*/

Pip os_pip_create (UInt16 pip_size)
{
  os_kern_process_cmd_hw (os_k_pip_create , & pip_size , os_va_list_void) ;

  return os_tsk_running->arg.pip ;
}

//--------------------------------------------------------------------------//

Boolean os_pip_insert_v (Pip pip , ...)
{
  va_list   args ;

  va_start (args , pip) ;
    {
      os_kern_process_cmd_hw (os_k_pip_insert , & pip,args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.status ;
}

//--------------------------------------------------------------------------//

Boolean os_pip_insert_c_v (Pip pip , ...)
{
  va_list   args ;

  va_start (args , pip) ;
    {
      os_kern_process_cmd_hw (os_k_pip_insert_c , & pip,args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.status ;
}

//--------------------------------------------------------------------------//

int os_pip_remove_v (Pip pip , ...)
{
  va_list   args ;

  va_start (args,pip) ;
    {
      os_kern_process_cmd_hw (os_k_pip_remove , & pip,args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.pip_data ;
}

//--------------------------------------------------------------------------//

int os_pip_remove_c (Pip pip)
{
  os_kern_process_cmd_hw (os_k_pip_remove_c , & pip,os_va_list_void) ;

  return os_tsk_running->arg.pip_data ;
}

//--------------------------------------------------------------------------//

Boolean os_pip_test_full (Pip pip)
{
  Boolean is_full ;

  ATOMIC (is_full = pip->entries == pip->size) ;

  return  is_full ;
}

//--------------------------------------------------------------------------//

Boolean  os_pip_test_empty (Pip pip)
{
  Boolean is_empty ;

  ATOMIC (is_empty = pip->entries == 0U) ;

  return  is_empty ;
}

//--------------------------------------------------------------------------//

void os_pip_flush (Pip pip)
{
  os_kern_process_cmd_hw (os_k_pip_flush , & pip , os_va_list_void) ;
}

/*=============================[ IRQ ENTRIES ]==============================*/

Boolean os_pip_insert_i (Pip pip , int data)
{
  if (fifo_insert (pip , data))
    {
      if (os_q_head (& pip->q_wait_remove))
        {
          wake_task_remove (pip , os_q_head (& pip->q_wait_remove)) ;
        }

      return TRUE ;
    }

  return FALSE ;
}

//--------------------------------------------------------------------------//

int os_pip_remove_i (Pip pip)
{
  int data = fifo_remove (pip) ;

  if (data != -1 && os_q_head (& pip->q_wait_insert))
    {
      wake_task_insert (pip , os_q_head (& pip->q_wait_insert)) ;
    }

  return data ;
}

//--------------------------------------------------------------------------//

void os_pip_flush_i (Pip pip)
{
  pip->entries = 0         ;
  pip->tail    = pip->head ;

  while (os_q_head (& pip->q_wait_insert))
    {
      wake_task_insert (pip , os_q_head (& pip->q_wait_insert)) ;

      if (pip->entries >= pip->size)
        {
          return ;
        }
    }
}

/*============================[ KERNEL ENTRIES ]============================*/

static void os_k_pip_create (void * arg0 , va_list args)
{
  #define SIZE (*(UInt16 *) arg0)

  Pip pip      = (Pip)     os_mem_request (sizeof *pip) ;

  pip->buff    = (UInt8 *) os_mem_request (SIZE) ;

  pip->head    = pip->buff ;
  pip->tail    = pip->buff ;

  pip->size    = SIZE      ;
  pip->entries = 0         ;

  os_q_init (& pip->q_wait_insert) ;
  os_q_init (& pip->q_wait_remove) ;

  os_tsk_running->arg.pip = pip ;

  #undef  SIZE
}

//--------------------------------------------------------------------------//

static void os_k_pip_insert (void * arg0 , va_list args)
{
  #define PIP (*(Pip *) arg0)

  os_tsk_running->arg.pip_data = (int) va_arg (args , VARG) ;

  if (os_pip_insert_i (PIP , os_tsk_running->arg.pip_data))
    {
      os_tsk_running->arg.status = TRUE ;
    }
  else
    {
      os_tsk_running->tsk_state  = OS_TSK_WAIT_PIP_INS       ;
      os_tsk_running->k_tmo_func = os_k_pip_wait_timeout_ins ;

      os_q_link_lhs (& PIP->q_wait_insert , cast_tsk_to_q (os_tsk_running)) ;

      os_timer_insert ((TskTimeout) va_arg(args , VARG)) ;

      os_tsk_running = NULL ;
    }

  #undef  PIP
}

//--------------------------------------------------------------------------//

static void os_k_pip_insert_c (void * arg0 , va_list args)
{
  #define PIP (*(Pip *) arg0)

  os_tsk_running->arg.status = os_pip_insert_i (PIP , (int) va_arg (args , VARG)) ;

  #undef  PIP
}

//--------------------------------------------------------------------------//

static void os_k_pip_remove (void * arg0,va_list args)
{
  #define PIP (*(Pip *) arg0)

  os_tsk_running->arg.pip_data = os_pip_remove_i (PIP) ;

  if (os_tsk_running->arg.pip_data == -1)
    {
      os_tsk_running->tsk_state  = OS_TSK_WAIT_PIP_REM ;
      os_tsk_running->k_tmo_func = os_k_pip_wait_timeout_rem ;

      os_q_link_lhs (& PIP->q_wait_remove , cast_tsk_to_q (os_tsk_running)) ;

      os_timer_insert ((TskTimeout) va_arg (args , VARG)) ;

      os_tsk_running = NULL ;
    }
  
  #undef  PIP
}

//--------------------------------------------------------------------------//

static void os_k_pip_remove_c (void * arg0 , va_list args)
{
  #define PIP (*(Pip *) arg0)

  os_tsk_running->arg.pip_data = os_pip_remove_i (PIP) ;

  #undef  PIP
}

//--------------------------------------------------------------------------//

static void os_k_pip_flush (void * arg0 , va_list args)
{
  #define PIP (*(Pip *) arg0)

  os_pip_flush_i (PIP) ;

  #undef  PIP
}

/*===========================[ TIMEOUT FUNCTIONS ]===========================*/

static void os_k_pip_wait_timeout_ins (Tsk tsk)
{
  os_q_unlink (cast_tsk_to_q (tsk)) ;

  tsk->arg.status = FALSE ;
}

//--------------------------------------------------------------------------//

static void os_k_pip_wait_timeout_rem (Tsk tsk)
{
  os_q_unlink (cast_tsk_to_q (tsk)) ;

  tsk->arg.pip_data = -1 ;
}

/*===========================[ SUPPORT FUNCTIONS ]===========================*/

static Boolean fifo_insert (Pip pip , int data)
{
  if (pip->entries < pip->size)
    {
      pip->entries++ ;

      *pip->tail++ = (UInt8) data ;

      if (pip->tail - pip->buff >= pip->size)
        {
          pip->tail = pip->buff ;
        }

      return TRUE ;
    }

  return FALSE ;
}

//--------------------------------------------------------------------------//

static int fifo_remove (Pip pip)
{
  int data = -1 ;

  if (pip->entries > 0)
    {
      pip->entries-- ;
 
      data = *pip->head++ ;

      if (pip->head - pip->buff >= pip->size)
        {
          pip->head = pip->buff ;
        }
    }

  return data ;
}

//--------------------------------------------------------------------------//

static void wake_task_insert (Pip pip , QueueNode * tsk_n)
{
  #define TSK cast_q_to_tsk (tsk_n)

  os_q_unlink     (tsk_n) ;

  os_timer_remove (TSK)   ;
  
  fifo_insert (pip , TSK->arg.pip_data) ;

  TSK->arg.status = TRUE ;

  os_eligible_insert_tail (TSK) ;

  #undef  TSK
}

//--------------------------------------------------------------------------//

static void wake_task_remove (Pip pip , QueueNode * tsk_n)
{
  #define TSK cast_q_to_tsk (tsk_n)

  os_q_unlink     (tsk_n) ;

  os_timer_remove (TSK)   ;
  
  TSK->arg.pip_data = fifo_remove (pip) ;

  os_eligible_insert_tail (TSK) ;

  #undef  TSK
}

