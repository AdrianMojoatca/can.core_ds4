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
// $Id: os_flg.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static void os_k_flg_create   (void * arg0 , va_list args) ;
static void os_k_flg_set      (void * arg0 , va_list args) ;
static void os_k_flg_clr      (void * arg0 , va_list args) ;
static void os_k_flg_wait_set (void * arg0 , va_list args) ;
static void os_k_flg_wait_clr (void * arg0 , va_list args) ;

static void os_k_flg_wait_timeout (Tsk tsk) ;

static Boolean wake_all (QueueNode * q) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*=============================[ TASK ENTRIES ]=============================*/

Flg os_flg_create (Boolean flg_ival)
{
  os_kern_process_cmd_hw (os_k_flg_create , & flg_ival , os_va_list_void) ;

  return os_tsk_running->arg.flg ;
}

//--------------------------------------------------------------------------//

Boolean os_flg_state (Flg flg)
{
  Boolean flg_state ;

  ATOMIC (flg_state = flg->flg_state) ;

  return  flg_state ;
}

//--------------------------------------------------------------------------//

Boolean os_flg_set (Flg flg)
{
  os_kern_process_cmd_hw (os_k_flg_set , & flg , os_va_list_void) ;

  return os_tsk_running->arg.status ;
}

//--------------------------------------------------------------------------//

Boolean os_flg_set_i (Flg flg)
{
  flg->flg_state = TRUE ;

  return wake_all (& flg->q_wait_set) ;
}

//--------------------------------------------------------------------------//

Boolean os_flg_clr (Flg flg)
{
  os_kern_process_cmd_hw (os_k_flg_clr , & flg , os_va_list_void) ;

  return os_tsk_running->arg.status ;
}

//--------------------------------------------------------------------------//

Boolean os_flg_clr_i (Flg flg)
{
  flg->flg_state = FALSE ;

  return wake_all (& flg->q_wait_clr) ;
}

//--------------------------------------------------------------------------//

Boolean os_flg_wait_set_v (Flg flg , ...)
{
  va_list   args ;

  va_start (args , flg) ;
    {
      os_kern_process_cmd_hw (os_k_flg_wait_set , & flg , args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.status ;
}

//--------------------------------------------------------------------------//

Boolean os_flg_wait_clr_v (Flg flg , ...)
{
  va_list   args ;

  va_start (args , flg) ;
    {
      os_kern_process_cmd_hw (os_k_flg_wait_clr , & flg , args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.status ;
}

/*============================[ KERNEL ENTRIES ]============================*/

static void os_k_flg_create (void * arg0 , va_list args)
{
  #define FLG_IVAL (*(Boolean *) arg0)

  Flg flg = (Flg) os_mem_request (sizeof *flg) ;

  flg->flg_state = FLG_IVAL ;

  os_q_init (& flg->q_wait_set) ;
  os_q_init (& flg->q_wait_clr) ;

  os_tsk_running->arg.flg = flg ;

  #undef  FLG_IVAL
}

//--------------------------------------------------------------------------//

static void os_k_flg_set (void * arg0 , va_list args)
{
  #define FLG (*(Flg *) arg0)

  os_tsk_running->arg.status = os_flg_set_i (FLG) ;

  #undef  FLG
}

//--------------------------------------------------------------------------//

static void os_k_flg_clr (void * arg0 , va_list args)
{
  #define FLG (*(Flg *) arg0)

  os_tsk_running->arg.status = os_flg_clr_i (FLG) ;

  #undef  FLG
}

//--------------------------------------------------------------------------//

static void os_k_flg_wait_set (void * arg0 , va_list args)
{
  #define FLG (*(Flg *) arg0)

  if (FLG->flg_state)
    {
      os_tsk_running->arg.status = TRUE ;
    }
  else
    {
      os_tsk_running->tsk_state  = OS_TSK_WAIT_FLG_SET   ;
      os_tsk_running->k_tmo_func = os_k_flg_wait_timeout ;

      os_q_link_lhs   (& FLG->q_wait_set , cast_tsk_to_q (os_tsk_running)) ;

      os_timer_insert ((TskTimeout) va_arg (args , VARG)) ;

      os_tsk_running = NULL ;
    }

  #undef  FLG
}

//--------------------------------------------------------------------------//

static void os_k_flg_wait_clr (void * arg0 , va_list args)
{
  #define FLG (*(Flg *) arg0)
                         
  if (!FLG->flg_state)
    {
      os_tsk_running->arg.status = TRUE ;
    }
  else
    {
      os_tsk_running->tsk_state  = OS_TSK_WAIT_FLG_CLR   ;
      os_tsk_running->k_tmo_func = os_k_flg_wait_timeout ;

      os_q_link_lhs   (& FLG->q_wait_clr , cast_tsk_to_q (os_tsk_running)) ;

      os_timer_insert ((TskTimeout) va_arg (args , VARG)) ;

      os_tsk_running = NULL ;
    }

  #undef  FLG
}

/*===========================[ TIMEOUT FUNCTIONS ]===========================*/

static void os_k_flg_wait_timeout (Tsk tsk)
{
  os_q_unlink (cast_tsk_to_q (tsk)) ;

  tsk->arg.status = FALSE ;
}

/*===========================[ SUPPORT FUNCTIONS ]===========================*/

static Boolean wake_all (QueueNode * q)
{
  QueueNode * t ;

  Boolean status = FALSE ;

  while (t = q->rhs , t != q)
    {
      Tsk tsk = cast_q_to_tsk (t) ;
    
      os_q_unlink (t) ;

      os_timer_remove (tsk) ;
      
      tsk->arg.status = FALSE ;
    
      os_eligible_insert_tail (tsk) ;

      status = TRUE ;
    }

  return status ;
}

