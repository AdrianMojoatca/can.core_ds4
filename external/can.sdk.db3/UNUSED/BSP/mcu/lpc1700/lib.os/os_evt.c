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
// $Id: os_evt.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static void os_k_evt_create       (void * arg0 , va_list args) ;
static void os_k_evt_signal       (void * arg0 , va_list args) ;
static void os_k_evt_wait         (void * arg0 , va_list args) ;

static void os_k_evt_wait_timeout (Tsk tsk) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*=============================[ TASK ENTRIES ]=============================*/

Evt os_evt_create (void)
{
  os_kern_process_cmd_hw (os_k_evt_create , NULL , os_va_list_void) ;

  return os_tsk_running->arg.evt ;
}

//--------------------------------------------------------------------------//

void os_evt_clr (Evt evt , UInt16 evt_flags)
{
  CRITICAL ;

  C_ENTER ;
    {
      evt->evt_bits &= ~ evt_flags ;
    }
  C_LEAVE ;
}

//--------------------------------------------------------------------------//

Boolean os_evt_signal_v (Evt evt , ...)
{
  va_list   args ;

  va_start (args , evt) ;
    {
      os_kern_process_cmd_hw (os_k_evt_signal , & evt , args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.status ;
}

//--------------------------------------------------------------------------//

Boolean os_evt_signal_i (Evt evt , UInt16 evt_flags)
{
  evt->evt_bits |= evt_flags ;

  if (evt->tsk_waiting)
    {
      UInt16 evt_bits = evt->evt_bits & evt->evt_flags ;
      
      if (evt_bits)
        {
          Tsk tsk = evt->tsk_waiting ;

          os_timer_remove (tsk) ;

          evt->tsk_waiting =   NULL           ;
          evt->evt_bits   &= ~ evt->evt_flags ;

          tsk->arg.evt_args =  evt_bits ;

          os_eligible_insert_tail (tsk) ;

          return TRUE ;
        }
    }

  return FALSE ;
}

//--------------------------------------------------------------------------//

UInt16 os_evt_wait_v (Evt evt , ...)
{
  va_list   args ;

  va_start (args , evt) ;
    {
      os_kern_process_cmd_hw (os_k_evt_wait , & evt , args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.evt_args ;
}

/*============================[ KERNEL ENTRIES ]============================*/

static void os_k_evt_create (void * arg0 , va_list args)
{
  Evt evt = (Evt) os_mem_request (sizeof *evt) ;

  evt->evt_bits    = 0x0000 ;
  evt->tsk_waiting = NULL   ;

  os_tsk_running->arg.evt = evt ;
}

//--------------------------------------------------------------------------//

static void os_k_evt_signal (void * arg0 , va_list args)
{
  #define EVT (*(Evt *) arg0)

  os_tsk_running->arg.status = os_evt_signal_i (EVT , (UInt16) va_arg (args , VARG)) ;

  #undef  EVT
}

//--------------------------------------------------------------------------//

static void os_k_evt_wait (void * arg0 , va_list args)
{
  #define EVT (*(Evt *) arg0)

  EVT->evt_flags = (UInt16) va_arg (args , VARG) ;
  EVT->clr_flags = (UInt16) va_arg (args , VARG) ;

  EVT->evt_bits &= ~ EVT->clr_flags ;

  test_for_event :
    {
      UInt16 evt_bits = EVT->evt_bits & EVT->evt_flags ;

      if (evt_bits)
        {
          os_tsk_running->arg.evt_args = evt_bits ;

          EVT->evt_bits &= ~EVT->evt_flags ;
        }
      else
        {
          os_tsk_running->tsk_state  = OS_TSK_WAIT_EVT       ;
          os_tsk_running->k_tmo_func = os_k_evt_wait_timeout ;
          os_tsk_running->arg.evt    = EVT                   ;

          EVT->tsk_waiting = os_tsk_running ;

          os_timer_insert ((TskTimeout) va_arg(args , VARG)) ;

          os_tsk_running = NULL ;
        }
    }

  #undef  EVT
}

/*===========================[ TIMEOUT FUNCTIONS ]==========================*/

static void os_k_evt_wait_timeout (Tsk tsk)
{
  tsk->arg.evt->tsk_waiting = NULL ;

  tsk->arg.evt_args = 0x0000 ;
}
