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
// $Id: os_pte.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static void os_k_pte_create   (void * arg0,va_list args) ;
static void os_k_pte_wait     (void * arg0,va_list args) ;
static void os_k_pte_wake_set (void * arg0,va_list args) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static Pte os_pte_root = NULL ;
static Pte os_pte_last = NULL ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*=============================[ TASK ENTRIES ]=============================*/

Pte os_pte_create (TskTimeout period)
{
  os_kern_process_cmd_hw (os_k_pte_create,& period,os_va_list_void) ;

  return os_tsk_running->arg.pte ;
}

/*--------------------------------------------------------------------------*/

Boolean os_pte_wait (Pte pte)
{
  os_kern_process_cmd_hw (os_k_pte_wait,& pte,os_va_list_void) ;

  return os_tsk_running->arg.status ;
}

/*--------------------------------------------------------------------------*/

Boolean os_pte_wake_set (Pte pte)
{
  os_kern_process_cmd_hw (os_k_pte_wake_set,& pte,os_va_list_void) ;

  return os_tsk_running->arg.status ;
}

/*--------------------------------------------------------------------------*/

Boolean os_pte_wake_set_i (Pte pte)
{
  if (pte->tsk_waiting)
    {
      os_eligible_insert_tail (pte->tsk_waiting) ;
    
      pte->tsk_waiting->arg.status = FALSE ;

      pte->tsk_waiting = NULL ;

      return TRUE ;
    }
  else
    {
      pte->wake_request = TRUE ;

      return FALSE ;
    }
}

/*--------------------------------------------------------------------------*/

Boolean os_pte_wake_clr (Pte pte)
{
  Boolean wake_request ;

  ATOMIC (wake_request = os_pte_wake_clr_i (pte)) ;

  return  wake_request ;
}

/*--------------------------------------------------------------------------*/

Boolean os_pte_wake_clr_i (Pte pte)
{
  Boolean wake_request = pte->wake_request ;

  pte->wake_request = FALSE ;

  return  wake_request ;
}

/*--------------------------------------------------------------------------*/

void os_pte_sync (Pte pte)
{
  ATOMIC (pte->time = pte->tsk_waiting ? 1 : 0) ;
}

/*--------------------------------------------------------------------------*/

TskTimeout os_pte_set_period (Pte pte, TskTimeout period)
{
  TskTimeout period_old ;

  ATOMIC
    (
      period_old  = pte->period ;
      pte->period = period      ;
    )

  return period_old ;
}

/*--------------------------------------------------------------------------*/

TskTimeout os_pte_get_period (Pte pte)
{
  TskTimeout period ;

  ATOMIC (period = pte->period) ;

  return  period ;
}

/*============================[ KERNEL ENTRIES ]============================*/

static void os_k_pte_create (void * arg0,va_list args)
{
  #define PERIOD (*(TskTimeout*)arg0)

  Pte pte = (Pte) os_mem_request (sizeof *pte) ;

  pte->next         = NULL   ;
  pte->time         = 0      ;
  pte->period       = PERIOD ;
  pte->tsk_waiting  = NULL   ;
  pte->wake_request = FALSE  ;
  
  if (!os_pte_root)
    {
      os_pte_root = pte ;
    }
  else
    {
      os_pte_last->next = pte ;
    }

  os_tsk_running->arg.pte = os_pte_last = pte ;
  
  #undef  PERIOD
}

/*--------------------------------------------------------------------------*/

static void os_k_pte_wait (void * arg0,va_list args)
{
  #define PTE (*(Pte*)arg0)

  if (PTE->wake_request)
    {
      PTE->wake_request = FALSE ;

      os_tsk_running->arg.status = FALSE ;
    }
  else
    {
      if (PTE->time == 0)
        {
          PTE->time =  1 ;
        }
    
      PTE->tsk_waiting = os_tsk_running ;
    
      os_tsk_running->tsk_state = OS_TSK_WAIT_PTE ;
    
      os_tsk_running = NULL ;
    }

  #undef  PTE
}

/*--------------------------------------------------------------------------*/

static void os_k_pte_wake_set (void * arg0,va_list args)
{
  #define PTE (*(Pte*)arg0)

  os_tsk_running->arg.status = os_pte_wake_set_i (PTE) ;

  #undef  PTE
}

/*--------------------------------------------------------------------------*/

void os_k_pte_service (void)
{
  Pte curr ;
  
  for (curr = os_pte_root ; curr ; curr = curr->next)
    {
      if (curr->time != 0 && --curr->time == 0)
        {
          curr->time = curr->period ;

          if (curr->tsk_waiting)
            {
              os_eligible_insert_tail (curr->tsk_waiting) ;
    
              curr->tsk_waiting->arg.status = TRUE ;

              curr->tsk_waiting = NULL ;
            }
        }
    }
}

/*=============================[ IRQ ENTRIES ]==============================*/

/*===========================[ TIMEOUT FUNCTIONS ]===========================*/

/*===========================[ SUPPORT FUNCTIONS ]===========================*/

