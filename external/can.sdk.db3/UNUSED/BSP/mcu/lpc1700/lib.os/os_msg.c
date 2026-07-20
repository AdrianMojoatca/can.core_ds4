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
// $Id: os_msg.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static void    os_k_msg_send         (void * arg0,va_list args) ;
static void    os_k_msg_send_c       (void * arg0,va_list args) ;
                                     
static void    os_k_msg_rcev         (void * arg0,va_list args) ;
static void    os_k_msg_rcev_c       (void * arg0,va_list args) ;

static void    os_k_msg_read         (void * arg0,va_list args) ;

static void    os_k_msg_resp         (void * arg0,va_list args) ;
static void    os_k_msg_pass         (void * arg0,va_list args) ;

static void    os_k_msg_send_timeout (Tsk tsk);
static void    os_k_msg_rcev_timeout (Tsk tsk);

static Boolean test_dest             (Tsk tsk_srce,Tsk tsk_dest);
static void    proc_dest             (Tsk tsk_srce,Tsk tsk_dest);

static Tsk     find_srce             (Tsk tsk_accept);
static void    proc_srce             (Tsk tsk_srce);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*=============================[ TASK ENTRIES ]=============================*/

Boolean os_msg_send_v (Tsk tsk_dest,...)
{
  va_list   args ;

  va_start (args,tsk_dest) ;
    {
      os_kern_process_cmd_hw (os_k_msg_send,& tsk_dest,args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.status ;
}

/*--------------------------------------------------------------------------*/

Boolean os_msg_send_c_v (Tsk tsk_dest,...)
{
  va_list   args ;

  va_start (args,tsk_dest) ;
    {
      os_kern_process_cmd_hw (os_k_msg_send_c,& tsk_dest,args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.status ;
}

/*--------------------------------------------------------------------------*/

Tsk os_msg_rcev_v (Tsk tsk_accept,...)
{
  va_list   args ;

  va_start (args,tsk_accept) ;
    {
      os_kern_process_cmd_hw (os_k_msg_rcev,& tsk_accept,args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.tsk ;
}

/*--------------------------------------------------------------------------*/

Tsk os_msg_rcev_c_v (Tsk tsk_accept,...)
{
  va_list   args ;

  va_start (args,tsk_accept) ;
    {
      os_kern_process_cmd_hw (os_k_msg_rcev_c,& tsk_accept,args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.tsk ;
}

/*--------------------------------------------------------------------------*/

void os_msg_read_v (Tsk tsk_srce,...)
{
  va_list   args ;

  va_start (args,tsk_srce) ;
    {
      os_kern_process_cmd_hw (os_k_msg_read,& tsk_srce,args) ;
    }
  va_end   (args) ;
} 

/*--------------------------------------------------------------------------*/

void os_msg_resp (Tsk tsk_srce)
{
  os_kern_process_cmd_hw (os_k_msg_resp,& tsk_srce,os_va_list_void) ;
}

/*--------------------------------------------------------------------------*/

void os_msg_pass_v (Tsk tsk_srce,...)
{
  va_list   args ;

  va_start (args,tsk_srce) ;
    {
      os_kern_process_cmd_hw (os_k_msg_pass,& tsk_srce,args) ;
    }
  va_end   (args) ;
}

/*============================[ KERNEL ENTRIES ]============================*/

static void os_k_msg_send (void * arg0,va_list args)
{
  #define TSK_DEST (*(Tsk*)arg0)

  os_tsk_running->arg.msg_send.msg = va_arg (args,void *) ;

  if (test_dest (os_tsk_running,TSK_DEST))
    {
      proc_dest (os_tsk_running,TSK_DEST) ;
    }
  else
    {
      os_tsk_running->tsk_state  = OS_TSK_WAIT_MSG_RCEV  ;
      os_tsk_running->k_tmo_func = os_k_msg_send_timeout ;

      os_q_link_lhs   (&TSK_DEST->q_msgs,cast_tsk_to_q  (os_tsk_running)) ;

      os_timer_insert ((TskTimeout) va_arg(args,VARG)) ;
    }

  os_tsk_running = NULL ;
      
  #undef  TSK_DEST
}

/*--------------------------------------------------------------------------*/

static void os_k_msg_send_c (void * arg0,va_list args) // ok
{
  #define TSK_DEST (*(Tsk*)arg0)

  os_tsk_running->arg.msg_send.msg = va_arg (args,void *) ;

  if (test_dest (os_tsk_running,TSK_DEST))
    {
      proc_dest (os_tsk_running,TSK_DEST) ;
    
      os_tsk_running = NULL ;
    }
  else
    {
      os_tsk_running->arg.status = FALSE ;
    }
  #undef  TSK_DEST
}

/*--------------------------------------------------------------------------*/

static void os_k_msg_rcev (void * arg0,va_list args)
{
  #define TSK_ACCEPT (*(Tsk*)arg0)
  
  Tsk tsk_srce ;
  
  os_tsk_running->arg.msg_rcev.msg        = va_arg (args,void *) ;
  os_tsk_running->arg.msg_rcev.tsk_accept = TSK_ACCEPT           ;
  
  tsk_srce = find_srce (TSK_ACCEPT) ;
  
  if (tsk_srce)
    {
      proc_srce (tsk_srce) ;
    }
  else
    {
      os_tsk_running->tsk_state  = OS_TSK_WAIT_MSG_SEND  ;
      os_tsk_running->k_tmo_func = os_k_msg_rcev_timeout ;
      
      os_timer_insert ((TskTimeout) va_arg(args,VARG)) ;
      
      os_tsk_running = NULL ;
    }

  #undef  TSK_ACCEPT
}

/*--------------------------------------------------------------------------*/

static void os_k_msg_rcev_c (void * arg0,va_list args) // ok
{
  #define TSK_ACCEPT (*(Tsk*)arg0)
  #define MSG        (*((void ***) ((UInt8 *)args + 0)))
  
  Tsk tsk_srce ;
  
  os_tsk_running->arg.msg_rcev.msg        = va_arg (args,void *) ;
  os_tsk_running->arg.msg_rcev.tsk_accept = TSK_ACCEPT           ;
  
  tsk_srce = find_srce (TSK_ACCEPT) ;
  
  if (tsk_srce)
    {
      proc_srce (tsk_srce) ;
    }
  else
    {
      os_tsk_running->arg.tsk = NULL ;
    }

  #undef  TSK_ACCEPT
}

/*--------------------------------------------------------------------------*/

static void os_k_msg_read (void * arg0,va_list args)
{
  #define TSK_SRCE (*(Tsk*)arg0)

  *((void **) va_arg(args,void *)) = TSK_SRCE->arg.msg_send.msg ;

  #undef  TSK_SRCE
}

/*--------------------------------------------------------------------------*/

static void os_k_msg_resp (void * arg0,va_list args)
{
  #define TSK_SRCE (*(Tsk*)arg0)
  
  os_eligible_insert_tail (TSK_SRCE) ;

  TSK_SRCE->arg.status = TRUE ;
  
  #undef  TSK_SRCE
}

/*--------------------------------------------------------------------------*/

static void os_k_msg_pass (void * arg0,va_list args)
{
  #define TSK_SRCE (*(Tsk*)arg0)
 
  Tsk tsk_dest = va_arg(args,Tsk) ;

  if (test_dest (TSK_SRCE,tsk_dest))
    {
      proc_dest (TSK_SRCE,tsk_dest) ;
    }
  else
    {
      TSK_SRCE->tsk_state = OS_TSK_WAIT_MSG_RCEV ;
    
      os_q_link_lhs (&tsk_dest->q_msgs,cast_tsk_to_q (TSK_SRCE)) ;
    }
 
  #undef  TSK_SRCE
}

/*===========================[ TIMEOUT FUNCTIONS ]==========================*/

static void os_k_msg_send_timeout (Tsk tsk)
{
  os_q_unlink (cast_tsk_to_q (tsk)) ;
  
  tsk->arg.status = FALSE ;
}

/*--------------------------------------------------------------------------*/

static void os_k_msg_rcev_timeout (Tsk tsk)
{
  tsk->arg.tsk = NULL ;
}
 
/*===========================[ SUPPORT FUNCTIONS ]==========================*/
 
static Boolean test_dest (Tsk tsk_srce,Tsk tsk_dest)
{
  #define ACCEPTING(z) (tsk_dest->arg.msg_rcev.tsk_accept == z)
  
  return tsk_dest->tsk_state == OS_TSK_WAIT_MSG_SEND && (ACCEPTING(0) || ACCEPTING(tsk_srce)) ;
  
  #undef  ACCEPTING
}

/*--------------------------------------------------------------------------*/

static void proc_dest (Tsk tsk_srce,Tsk tsk_dest)
{
  tsk_srce->tsk_state = OS_TSK_WAIT_MSG_RESP_PASS ;

  if (tsk_dest->arg.msg_rcev.msg)
    {
      *tsk_dest->arg.msg_rcev.msg = tsk_srce->arg.msg_send.msg ;
    }
  
  tsk_dest->arg.tsk = tsk_srce ;

  os_timer_remove (tsk_dest) ;

  os_eligible_insert_tail (tsk_dest) ;
}

/*--------------------------------------------------------------------------*/

static Tsk find_srce (Tsk tsk_accept)
{
  QueueNode * srce ;

  if (!tsk_accept)
    {
      srce = os_q_head (&os_tsk_running->q_msgs) ;
      
      if (srce) return cast_q_to_tsk (srce) ;
    }
  else
    {
      QueueNode * msgs = & os_tsk_running->q_msgs ;
    
      for (srce = msgs->rhs ; srce != msgs ; srce = srce->rhs)
        if (cast_q_to_tsk(srce) == tsk_accept)
          return cast_q_to_tsk (srce) ;
    }

  return NULL ;
}

/*--------------------------------------------------------------------------*/

static void proc_srce (Tsk tsk_srce)
{
  os_q_unlink (cast_tsk_to_q(tsk_srce)) ;
  
  os_timer_remove (tsk_srce) ;

  tsk_srce->tsk_state = OS_TSK_WAIT_MSG_RESP_PASS ;
  
  if (os_tsk_running->arg.msg_rcev.msg)
    {
      *os_tsk_running->arg.msg_rcev.msg = tsk_srce->arg.msg_send.msg ;
    }
    
  os_tsk_running->arg.tsk = tsk_srce ;
}
