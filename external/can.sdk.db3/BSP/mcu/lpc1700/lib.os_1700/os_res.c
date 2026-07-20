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
// $Id: os_res.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static void os_k_res_create             (void * arg0,va_list args) ;
static void os_k_res_release            (void * arg0,va_list args) ;
static void os_k_res_request            (void * arg0,va_list args) ;
static void os_k_res_request_c          (void * arg0,va_list args) ;

static Boolean os_k_res_request_i       (Res res , Tsk tsk_owner) ;

static void    os_k_res_request_timeout (Tsk tsk) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*=============================[ TASK ENTRIES ]=============================*/

Res os_res_create (void)
{
  os_kern_process_cmd_hw (os_k_res_create,NULL,os_va_list_void) ;

  return os_tsk_running->arg.res ;
}

/*--------------------------------------------------------------------------*/

Boolean os_res_request_v (Res res,...)
{
  va_list   args ;

  va_start (args,res) ;
    {
      os_kern_process_cmd_hw (os_k_res_request,& res,args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.status ;
}

/*--------------------------------------------------------------------------*/

Boolean  os_res_request_c (Res res)
{
  os_kern_process_cmd_hw (os_k_res_request_c,& res,os_va_list_void) ;

  return os_tsk_running->arg.status ;
}

/*--------------------------------------------------------------------------*/

Boolean os_res_request_i (Res res)
{
  return os_k_res_request_i (res,NULL) ;
}

/*--------------------------------------------------------------------------*/

void os_res_release (Res res)
{
  os_kern_process_cmd_hw (os_k_res_release,& res,os_va_list_void) ;
}

/*--------------------------------------------------------------------------*/

void os_res_release_i (Res res)
{
  if (--res->req_depth == 0)
    {
      PQueueNode * tsk = os_pq_head (&res->q_waiting) ;

      if (tsk)
        {
          #define TSK cast_pq_to_tsk(tsk)

          os_pq_remove     (tsk) ;

          os_timer_remove  (TSK) ;

          res->tsk_owner  = TSK  ;
      
          res->req_depth  = 1    ;

          TSK->arg.status = TRUE ;

          os_eligible_insert_tail (TSK) ;

          #undef  TSK
        }
    }
}

/*============================[ KERNEL ENTRIES ]============================*/

static void os_k_res_create (void * arg0,va_list args)
{
  Res res = (Res) os_mem_request (sizeof *res) ;

  os_q_init (&res->q_waiting) ;
  
  res->req_depth = 0    ;
  res->tsk_owner = NULL ;
  
  os_tsk_running->arg.res = res ;
}

/*--------------------------------------------------------------------------*/

static void os_k_res_request (void * arg0,va_list args)
{
  #define RES (*(Res*)arg0)

  if (os_k_res_request_i (RES,os_tsk_running))
    {
      os_tsk_running->arg.status = TRUE ;
    }
  else
    {
      os_tsk_running->tsk_state  = OS_TSK_WAIT_RES          ;
      os_tsk_running->k_tmo_func = os_k_res_request_timeout ;

      os_pq_insert_tail (&RES->q_waiting,cast_tsk_to_pq(os_tsk_running)) ;
      
      os_timer_insert ((TskTimeout) va_arg(args,VARG)) ;

      os_tsk_running = NULL ;
    }

  #undef  RES
}

/*--------------------------------------------------------------------------*/

static void os_k_res_request_c (void * arg0,va_list args)
{
  #define RES (*(Res*)arg0)

  os_tsk_running->arg.status = os_k_res_request_i (RES,os_tsk_running) ;

  #undef  RES
}

/*--------------------------------------------------------------------------*/

static void os_k_res_release (void * arg0,va_list args)
{
  #define RES (*(Res*)arg0)

  os_res_release_i (RES) ;

  #undef  RES
}

/*--------------------------------------------------------------------------*/

static Boolean os_k_res_request_i (Res res , Tsk tsk_owner)
{
  if (res->req_depth == 0)
    {
      res->req_depth = 1         ;
      res->tsk_owner = tsk_owner ;

      return TRUE ;
    }

  if (res->tsk_owner == tsk_owner)
    {
      res->req_depth++ ;

      return TRUE ;
    }

  return FALSE ;
}

/*==========================[ TIMEOUT FUNCTIONS ]===========================*/

static void os_k_res_request_timeout (Tsk tsk)
{
  os_pq_remove (cast_tsk_to_pq(tsk)) ;

  tsk->arg.status = FALSE ;
}

