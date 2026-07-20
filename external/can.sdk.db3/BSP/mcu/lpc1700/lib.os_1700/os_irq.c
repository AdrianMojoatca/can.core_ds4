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
// $Id: os_irq.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static void os_k_irq_create       (void * arg0,va_list args) ;
static void os_k_irq_wait         (void * arg0,va_list args) ;
static void os_k_irq_set          (void * arg0,va_list args) ;

static void os_k_irq_wait_timeout (Tsk tsk) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*=============================[ TASK ENTRIES ]=============================*/

Irq os_irq_create (void)
{
  os_kern_process_cmd_hw (os_k_irq_create,NULL,os_va_list_void) ;

  return os_tsk_running->arg.irq ;
}

/*--------------------------------------------------------------------------*/

Boolean os_irq_wait_v (Irq irq,...)
{
  va_list   args ;

  va_start (args,irq) ;
    {
      os_kern_process_cmd_hw (os_k_irq_wait,& irq,args) ;
    }
  va_end   (args) ;

  return os_tsk_running->arg.status ;
}

/*--------------------------------------------------------------------------*/

Boolean os_irq_set (Irq irq)
{
  os_kern_process_cmd_hw (os_k_irq_set,& irq,os_va_list_void) ;
  
  return os_tsk_running->arg.status ;
}

/*--------------------------------------------------------------------------*/

void os_irq_clr (Irq irq)
{
  ATOMIC (irq->state = FALSE) ;
}

/*=============================[ IRQ ENTRIES ]==============================*/

Boolean os_irq_set_i (Irq irq)
{
  if (irq->tsk_waiting)
    {
      Tsk tsk = irq->tsk_waiting ;

      os_timer_remove (tsk) ;

      irq->state       = FALSE ;
      irq->tsk_waiting = NULL  ;

      tsk->arg.status  = TRUE  ;
      
      os_eligible_insert_tail (tsk) ;

      return TRUE ;
    }
    
  irq->state = TRUE ;
  
  return FALSE ;
}

/*--------------------------------------------------------------------------*/

void os_irq_clr_i (Irq irq)
{
  irq->state = FALSE ;
}

/*============================[ KERNEL ENTRIES ]============================*/

static void os_k_irq_create (void * arg0,va_list args)
{
  Irq irq = (Irq) os_mem_request (sizeof *irq) ;

  irq->state       = FALSE ;
  irq->tsk_waiting = NULL  ;
  
  os_tsk_running->arg.irq = irq ;
}

/*--------------------------------------------------------------------------*/

static void os_k_irq_wait (void * arg0,va_list args)
{
  #define IRQ (*(Irq*)arg0)

  if ((Boolean) va_arg(args,VARG))
    {
      IRQ->state = FALSE ;

      goto wait ;
    }

  if (IRQ->state)
    {
      IRQ->state = FALSE ;

      os_tsk_running->arg.status = TRUE ;

      return ;
    }

  wait :
    {
      os_tsk_running->tsk_state  = OS_TSK_WAIT_IRQ ;
      os_tsk_running->k_tmo_func = os_k_irq_wait_timeout ;
      os_tsk_running->arg.irq    = IRQ ;

      IRQ->tsk_waiting = os_tsk_running ;

      os_timer_insert ((TskTimeout) va_arg(args,VARG)) ;

      os_tsk_running = NULL ;
    }

  #undef  IRQ
}

/*--------------------------------------------------------------------------*/

static void os_k_irq_set (void * arg0,va_list args)
{
  #define IRQ (*(Irq*) arg0)
  
  os_tsk_running->arg.status = os_irq_set_i (IRQ) ;

  #undef  IRQ
}

/*===========================[ TIMEOUT FUNCTIONS ]===========================*/

static void os_k_irq_wait_timeout (Tsk tsk)
{
  tsk->arg.irq->tsk_waiting = NULL ;

  tsk->arg.status = FALSE ;
}
