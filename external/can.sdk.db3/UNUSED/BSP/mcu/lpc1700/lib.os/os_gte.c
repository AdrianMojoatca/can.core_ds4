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
// $Id: os_gte.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static void os_k_gte_create (void * arg0 , va_list args) ;
static void os_k_gte_wait   (void * arg0 , va_list args) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static Gte os_gte_root = NULL ;
static Gte os_gte_last = NULL ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*=============================[ TASK ENTRIES ]=============================*/

Gte os_gte_create_v (TskTimeout * period , ...)
{
  va_list   args ;

  va_start (args , period) ;
    {
      os_kern_process_cmd_hw (os_k_gte_create , period , args) ;
    }
  va_end   (args) ;
 
  return os_tsk_running->arg.gte ;
}

/*--------------------------------------------------------------------------*/

UInt16 os_gte_wait (Gte gte)
{
  os_kern_process_cmd_hw (os_k_gte_wait, & gte , os_va_list_void) ;

  return os_tsk_running->arg.gte_args ;
}

/*============================[ KERNEL ENTRIES ]============================*/

static void os_k_gte_create (void * arg0 , va_list args)
{
  #define PERIOD ((TskTimeout *) arg0)
 
  unsigned index ;
 
  UInt16   period_cnt = (UInt16) va_arg (args , VARG) ;

  Gte gte = (Gte) os_mem_request (sizeof *gte) ;

  gte->next          = NULL   ;
  gte->period        = PERIOD ;
  gte->period_cnt    = period_cnt ;
  gte->timeout_flags = 0x0000 ;
  gte->tsk_waiting   = NULL   ;
  gte->is_active     = FALSE  ;
  gte->time          = (TskTimeout *) os_mem_request (period_cnt * sizeof (TskTimeout)) ;
  
  for (index = 0 ; index < period_cnt ; index++)
    {
      gte->time[index] = PERIOD [index] ;
    }

  if (!os_gte_root)
    {
      os_gte_root = gte ;
    }
  else
    {
      os_gte_last->next = gte ;
    }

  os_tsk_running->arg.gte = os_gte_last = gte ;

  #undef  PERIOD
}

/*--------------------------------------------------------------------------*/

static void os_k_gte_wait (void * arg0 , va_list args)
{
  #define GTE (*(Gte*)arg0)

  if (GTE->timeout_flags)
    {
      os_tsk_running->arg.gte_args = GTE->timeout_flags ;
      
      GTE->timeout_flags = 0x0000 ;
    }
  else
    {
      GTE->is_active = TRUE ;
    
      GTE->tsk_waiting = os_tsk_running ;

      os_tsk_running->tsk_state = OS_TSK_WAIT_GTE ;
    
      os_tsk_running = NULL ;
    }

  #undef  GTE
}

/*--------------------------------------------------------------------------*/

void os_k_service_gte (void)
{
  Gte curr ;
  
  for (curr = os_gte_root ; curr ; curr = curr->next)
    if (curr->is_active)
      {
        UInt16 mask  = 0x0001 ;
        UInt16 index = 0 ;
        
        do
          {
            if (--curr->time[index] == 0)
              {
                curr->timeout_flags |= mask ;
                
                curr->time[index] = curr->period[index] ;
              }
          }
        while (mask <<= 1 , ++index < curr->period_cnt) ;

        if (curr->tsk_waiting && curr->timeout_flags)
          {
            curr->tsk_waiting->arg.gte_args = curr->timeout_flags ;
            
            curr->timeout_flags = 0x0000 ;
            
            os_eligible_insert_tail (curr->tsk_waiting) ;
            
            curr->tsk_waiting = NULL ;
          }
      }
}

/*=============================[ IRQ ENTRIES ]==============================*/

/*===========================[ TIMEOUT FUNCTIONS ]===========================*/

/*===========================[ SUPPORT FUNCTIONS ]===========================*/
