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
// $Id: os_mbx.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static void os_k_mbx_create          (void * arg0,va_list args) ;
static void os_k_mbx_send            (void * arg0,va_list args) ;
static void os_k_mbx_rcev            (void * arg0,va_list args) ;
static void os_k_mbx_rcev_c          (void * arg0,va_list args) ;

static void os_k_mbx_request_timeout (Tsk tsk);
static void os_mbx_send_process      (Mbx mbx,MbxMsg * msg);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*=============================[ TASK ENTRIES ]=============================*/

Mbx os_mbx_create (void)
{
  os_kern_process_cmd_hw (os_k_mbx_create,NULL,os_va_list_void) ;

  return os_tsk_running->arg.mbx ;
}

/*--------------------------------------------------------------------------*/

void os_mbx_send_v (Mbx mbx,...)
{
  va_list   args ;

  va_start (args,mbx) ;
    {
      os_kern_process_cmd_hw (os_k_mbx_send,& mbx,args) ;
    }
  va_end   (args) ;
}

/*--------------------------------------------------------------------------*/

MbxMsg * os_mbx_rcev_v (Mbx mbx,...)
{
  va_list   args ;

  va_start (args,mbx) ;
    {
      os_kern_process_cmd_hw (os_k_mbx_rcev,& mbx,args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.mbx_msg ;
}

/*--------------------------------------------------------------------------*/

MbxMsg * os_mbx_rcev_c (Mbx mbx)
{
  os_kern_process_cmd_hw (os_k_mbx_rcev_c,& mbx,os_va_list_void) ;

  return os_tsk_running->arg.mbx_msg ;
}

/*--------------------------------------------------------------------------*/

Boolean os_mbx_test_empty (Mbx mbx)
{
  Boolean is_empty ;

  ATOMIC (is_empty = mbx->head ? FALSE : TRUE) ;

  return  is_empty ;
}

/*============================[ KERNEL ENTRIES ]============================*/

static void os_k_mbx_create (void * arg0,va_list args)
{
  Mbx mbx = (Mbx) os_mem_request(sizeof *mbx) ;

  os_q_init (&mbx->q_waiting) ;

  mbx->head = NULL ;

  os_tsk_running->arg.mbx = mbx ;
}

/*--------------------------------------------------------------------------*/

static void os_k_mbx_send (void * arg0,va_list args)
{
  #define MBX (*(Mbx*)arg0)

  os_mbx_send_process (MBX,va_arg(args,MbxMsg *)) ;

  #undef  MBX
}

/*--------------------------------------------------------------------------*/

static void os_k_mbx_rcev (void * arg0,va_list args)
{
  #define MBX (*(Mbx*)arg0)

  if (MBX->head)
    {
      os_tsk_running->arg.mbx_msg = MBX->head ;

      MBX->head = MBX->head->next ;
    }
  else
    {
      os_tsk_running->tsk_state  = OS_TSK_WAIT_MBX          ;
      os_tsk_running->k_tmo_func = os_k_mbx_request_timeout ;

      os_pq_insert_tail (&MBX->q_waiting,cast_tsk_to_pq(os_tsk_running)) ;
      
      os_timer_insert ((TskTimeout) va_arg(args,VARG)) ;

      os_tsk_running = NULL ;
    }

  #undef  MBX
}

/*--------------------------------------------------------------------------*/

static void os_k_mbx_rcev_c (void * arg0,va_list args)
{
  #define MBX (*(Mbx*)arg0)

  if (os_tsk_running->arg.mbx_msg = MBX->head)
    {
      MBX->head = MBX->head->next ;
    }

  #undef  MBX
}

/*============================[ IRQ ENTRIES ]===============================*/

void os_mbx_send_i (Mbx mbx,MbxMsg * msg)
{
  os_mbx_send_process (mbx,msg) ;
}

/*--------------------------------------------------------------------------*/

MbxMsg * os_mbx_rcev_i (Mbx mbx)
{
  MbxMsg * msg = mbx->head ;

  if (mbx->head)
    {
      mbx->head = mbx->head->next ;
    }

  return msg ;
}

/*--------------------------------------------------------------------------*/

Boolean os_mbx_test_empty_i (Mbx mbx)
{
  return mbx->head ? FALSE : TRUE ;
}

/*==========================[ SUPPORT FUNCTIONS ]===========================*/

static void os_mbx_send_process (Mbx mbx,MbxMsg * msg)
{
  PQueueNode * tsk = os_pq_head (&mbx->q_waiting) ;

  if (!tsk)
    {
      msg->next = NULL ;

      if (!mbx->head)
        {
          mbx->head = msg ;
        }
      else
        {
          mbx->tail->next = msg ;
        }

      mbx->tail = msg ;
    }
  else
    {
      #define TSK cast_pq_to_tsk(tsk)

      os_pq_remove (tsk) ;

      os_timer_remove (TSK) ;

      TSK->arg.mbx_msg = msg ;

      os_eligible_insert_tail (TSK) ;

      #undef  TSK
    }
}

/*==========================[ TIMEOUT FUNCTIONS ]===========================*/

static void os_k_mbx_request_timeout (Tsk tsk)
{
  os_pq_remove (cast_tsk_to_pq(tsk)) ;

  tsk->arg.mbx_msg = NULL ;
}

