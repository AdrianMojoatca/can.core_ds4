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
// $Id: os_tfr.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static void os_k_tfr_create       (void * arg0,va_list args) ;
static void os_k_tfr_send         (void * arg0,va_list args) ;
static void os_k_tfr_send_c       (void * arg0,va_list args) ;
static void os_k_tfr_rcev         (void * arg0,va_list args) ;
static void os_k_tfr_rcev_c       (void * arg0,va_list args) ;

static void os_k_tfr_wait_timeout (Tsk tsk) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*=============================[ TASK ENTRIES ]=============================*/

Tfr os_tfr_create (void)
{
  os_kern_process_cmd_hw (os_k_tfr_create,NULL,os_va_list_void) ;

  return os_tsk_running->arg.tfr ;
}

/*--------------------------------------------------------------------------*/

Boolean os_tfr_send_v (Tfr tfr,...)
{
  va_list   args ;

  va_start (args,tfr) ;
    {
      os_kern_process_cmd_hw (os_k_tfr_send,& tfr,args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.status ;
}

/*--------------------------------------------------------------------------*/

Boolean os_tfr_send_c_v (Tfr tfr,...)
{
  va_list   args ;

  va_start (args,tfr) ;
    {
      os_kern_process_cmd_hw (os_k_tfr_send_c,& tfr,args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.status ;
}

/*--------------------------------------------------------------------------*/

Boolean os_tfr_rcev_v (Tfr tfr,...)
{
  va_list   args ;

  va_start (args,tfr) ;
    {
      os_kern_process_cmd_hw (os_k_tfr_rcev,& tfr,args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.status ; 
}

/*--------------------------------------------------------------------------*/

Boolean os_tfr_rcev_c_v (Tfr tfr,...)
{
  va_list   args ;

  va_start (args,tfr) ;
    {
      os_kern_process_cmd_hw (os_k_tfr_rcev_c,& tfr,args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.status ; 
}

/*============================[ KERNEL ENTRIES ]============================*/

static void os_k_tfr_create (void * arg0,va_list args)
{
  Tfr tfr = (Tfr) os_mem_request(sizeof *tfr) ;

  os_q_init (& tfr->q_wait_send) ;
  os_q_init (& tfr->q_wait_rcev) ;

  os_tsk_running->arg.tfr = tfr  ;
}

/*--------------------------------------------------------------------------*/

static void os_k_tfr_send (void * arg0,va_list args)
{
  #define TFR (*(Tfr*)arg0)

  void * object = va_arg (args,void *) ;

  if (os_tfr_send_i (TFR , object))
    {
      os_tsk_running->arg.status = TRUE ;
    }
  else
    {
      os_tsk_running->tsk_state        = OS_TSK_WAIT_TFR_SEND  ;
      os_tsk_running->k_tmo_func       = os_k_tfr_wait_timeout ;
      os_tsk_running->arg.tfr_arg_send = object                ;

      os_q_link_lhs   (& TFR->q_wait_send , cast_tsk_to_q (os_tsk_running)) ;

      os_timer_insert ((TskTimeout) va_arg(args,VARG)) ;

      os_tsk_running = NULL ;
    }

  #undef  TFR
}

/*--------------------------------------------------------------------------*/

static void os_k_tfr_send_c (void * arg0,va_list args)
{
  #define TFR (*(Tfr*)arg0)

  os_tsk_running->arg.status = os_tfr_send_i (TFR , va_arg (args,void *)) ;

  #undef  TFR
}

/*--------------------------------------------------------------------------*/

static void os_k_tfr_rcev (void * arg0,va_list args)
{
  #define TFR (*(Tfr*)arg0)

  void * object = va_arg (args,void *) ;

  if (os_tfr_rcev_i (TFR , object))
    {
      os_tsk_running->arg.status = TRUE ;
    }
  else
    {
      os_tsk_running->tsk_state        = OS_TSK_WAIT_TFR_RCEV  ;
      os_tsk_running->k_tmo_func       = os_k_tfr_wait_timeout ;
      os_tsk_running->arg.tfr_arg_rcev = (void **) object      ;

      os_q_link_lhs   (& TFR->q_wait_rcev , cast_tsk_to_q (os_tsk_running)) ;

      os_timer_insert ((TskTimeout) va_arg(args,VARG)) ;

      os_tsk_running = NULL ;
    }

  #undef  TFR
}

/*--------------------------------------------------------------------------*/

static void os_k_tfr_rcev_c (void * arg0,va_list args)
{
  #define TFR (*(Tfr*)arg0)

  os_tsk_running->arg.status = os_tfr_rcev_i (TFR , va_arg (args,void *)) ;

  #undef  TFR
}

/*============================[ IRQ ENTRIES ]===============================*/

Boolean os_tfr_send_i (Tfr tfr,void * object)
{
  QueueNode * tsk_q = os_q_head (& tfr->q_wait_rcev) ;

  if (tsk_q)
    {
      #define TSK cast_q_to_tsk (tsk_q)

      os_q_unlink (tsk_q) ;

      os_timer_remove (TSK) ;

      *TSK->arg.tfr_arg_rcev = object ;

      os_eligible_insert_tail (TSK) ;
      
      return TSK->arg.status = TRUE ;

      #undef TSK
    }

  return FALSE ;
}

/*--------------------------------------------------------------------------*/

Boolean os_tfr_rcev_i (Tfr tfr,void * object)
{
  QueueNode * tsk_q = os_q_head (& tfr->q_wait_send) ;

  if (tsk_q)
    {
      #define TSK cast_q_to_tsk (tsk_q)

      os_q_unlink (tsk_q) ;

      os_timer_remove (TSK) ;

      * (void **) object = TSK->arg.tfr_arg_send ;

      return TSK->arg.status = TRUE ;

      #undef TSK
    }

  return FALSE ;
}

/*==========================[ SUPPORT FUNCTIONS ]===========================*/

/*==========================[ TIMEOUT FUNCTIONS ]===========================*/

static void os_k_tfr_wait_timeout (Tsk tsk)
{
  os_q_unlink (cast_tsk_to_q (tsk)) ;

  tsk->arg.status = FALSE ;
}

