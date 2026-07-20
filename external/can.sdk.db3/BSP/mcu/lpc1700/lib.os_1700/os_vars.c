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
// $Id: os_vars.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

void           * os_stk_frame       ;                           // Storage for task stack frame
                               
TQueueNode       os_q_timer         ;                           // Queue of tasks waiting with timeout
QueueNode        os_q_eligible      ;                           // Queue of tasks eligible for the CPU
                               
Tsk              os_tsk_running     ;                           // Current running task

void           * os_sys_stk         ;                           // Kernel stack pointer
StkLimits        os_sys_stk_limits  ;                           // Kernel stack information

#if OS_USE_IDLE_STACK == 1

void           * os_idle_stk        ;                           // Idle stack pointer
StkLimits        os_idle_stk_limits ;                           // Idle stack information

#endif

void          (* os_sys_idle_func )(void) = os_kern_null_idle ; // System Idle Hook  
void          (* os_sys_init_func )(void) ;                     // System Init Hook

void          (* os_mem_fail_func)(UInt16) = os_kern_null_mem_fail ; // Called if os_mem_request fails

Tsk              os_tsk_list       = NULL ;                     // Linked list of all tasks

#if OS_USE_HOOK_CONTEXT == 1

TskHookFunc      os_running_hook_save ;                         // Current SAVE hook

TskHookFunc      os_tsk_hook_start  = os_kern_null_hook ;       // START  Task Hook
TskHookFunc      os_tsk_hook_stop   = os_kern_null_hook ;       // STOP   Task Hook
TskHookFunc      os_tsk_hook_save   = os_kern_null_hook ;       // SAVE   Task Hook
TskHookFunc      os_tsk_hook_load   = os_kern_null_hook ;       // LOAD   Task Hook
TskHookFunc      os_tsk_hook_create = os_kern_null_hook ;       // CREATE Task Hook

#endif

void           * os_tsk_local ;                                 // Task local scope pointer

volatile Boolean os_irq_executing = FALSE ;                     // IRQ or TASK executing

va_list          os_va_list_void ;                              // Used to satisfy compiler

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
