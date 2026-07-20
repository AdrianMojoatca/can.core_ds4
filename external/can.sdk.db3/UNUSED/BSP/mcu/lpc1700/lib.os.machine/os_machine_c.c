/****************************************************************************/
/*                                                                          */
/*                         A-Task Real-Time Kernel                          */
/*                                                                          */
/*                  Copyright (C) 2000-2005 - Acacetus Inc                  */
/*                                                                          */
/*                           All rights reserved                            */
/*                                                                          */
/*                             www.acacetus.com                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*           S O F T W A R E   L I C E N S E   A G R E E M E N T            */
/*                                                                          */
/* This software is the property of Acacetus Inc. and its use is governed   */
/* by the software license agreement stipulated in the following file:      */
/*                                                                          */
/*                          "Software License.pdf"                          */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*                                NOTES                                     */
/*                                -----                                     */
/*                                                                          */
/*                       Task Stack Frame Layout                            */
/*                                                                          */
/*                              +-------+                                   */
/*                       HI-MEM |  R15  | <<-- Task PC                      */
/*                              +-------|                                   */
/*                              |  R14  | <<-- Task LR                      */
/*                              +-------+                                   */
/*                              |  R13  | <<-- Task SP                      */
/*                              +-------+                                   */
/*                              |  R12  |                                   */
/*                              +-------+                                   */
/*                              |  R11  |                                   */
/*                              +-------+                                   */
/*                              |  R10  |                                   */
/*                              +-------+                                   */
/*                              |  R9   |                                   */
/*                              +-------+                                   */
/*                              |  R8   |                                   */
/*                              +-------+                                   */
/*                              |  R7   |                                   */
/*                              +-------+                                   */
/*                              |  R6   |                                   */
/*                              +-------+                                   */
/*                              |  R5   |                                   */
/*                              +-------+                                   */
/*                              |  R4   |                                   */
/*                              +-------+                                   */
/*                              |  R3   |                                   */
/*                              +-------+                                   */
/*                              |  R2   |                                   */
/*                              +-------+                                   */
/*                              |  R1   |                                   */
/*                              +-------+                                   */
/*                              |  R0   | <<-- Task Argument 'tsk_arg'      */
/*                              +-------+                                   */
/*                       LO-MEM |  CPSR | <<-- Task CPSR                    */
/*                              +-------+                                   */
/*                                                                          */
/*                                                                          */
/*  - When a task is active:                                                */
/*                                                                          */
/*    - 'os_stk_frame' points to CPSR in the stack frame.                   */
/*                                                                          */
/*    - SUPERVISOR SP points to R15 in the stack frame.                     */
/*                                                                          */
/*  - When all tasks are blocked:                                           */
/*                                                                          */
/*    - 'os_stk_frame' has no meaning.                                      */
/*                                                                          */
/*    - SUPERVISOR SP points to dummy stack frame on top of 'os_sys_stk'    */
/*                                                                          */
/****************************************************************************/

/*==========================================================================*/
// $Id: os_machine_c.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os_private.h"
 
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct // !!! Order Critical !!!
{
  UInt8 * mem_base  ; // Addr = Region.Base
  UInt8 * mem_limit ; // Addr = Region.Base + Regions.Limit

} Mem_Region ;

//--------------------------------------------------------------------------//

typedef struct
{
  UInt32 heap_mem_allocated ; // Total number of bytes allocated to-date
  UInt32 heap_mem_unuseable ; // Total number of bytes lost due to alignment, etc.

} Mem ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void              * os_mem_request_k           (UInt16  mem_size , void * caller_addr) ;

static UInt8      * os_mem_region_allocate     (Mem_Region * const mem_region , UInt16 mem_size) ;

static void         os_mem_region_find_max     (MemInfo * const mem_info) ;

static void         os_mem_region_find_usage   (MemInfo * const mem_info) ;

static Mem_Region * os_mem_region_find_min_fit (UInt16  mem_size) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

#ifndef OS_NUM_HEAP_REGIONS
#error  !!! Must specify number of OS heap regions available !!!
#endif

#if     OS_NUM_HEAP_REGIONS > 4
#error !!! Only four OS heap regions are supported
#endif

//--------------------------------------------------------------------------//

#define REGION_DEF(_region_) extern UInt8 Image$$ER_OS_HEAP_##_region_##$$ZI$$Base ; extern UInt8 Image$$ER_OS_HEAP_##_region_##$$ZI$$Limit

#if OS_NUM_HEAP_REGIONS >= 1
REGION_DEF (0) ;
#endif

#if OS_NUM_HEAP_REGIONS >= 2
REGION_DEF (1) ;
#endif

#if OS_NUM_HEAP_REGIONS >= 3
REGION_DEF (2) ;
#endif

#if OS_NUM_HEAP_REGIONS >= 4
REGION_DEF (3) ;
#endif

#undef REGION_DEF

//--------------------------------------------------------------------------//

#define REGION_VAL(_region_) { & Image$$ER_OS_HEAP_##_region_##$$ZI$$Base , & Image$$ER_OS_HEAP_##_region_##$$ZI$$Limit } ,

static       Mem_Region mem_region_d [] = // Dynamic Region Info  (RAM)
{
  #if OS_NUM_HEAP_REGIONS >= 1
  REGION_VAL (0)
  #endif
  
  #if OS_NUM_HEAP_REGIONS >= 2
  REGION_VAL (1)
  #endif
  
  #if OS_NUM_HEAP_REGIONS >= 3
  REGION_VAL (2)
  #endif
  
  #if OS_NUM_HEAP_REGIONS >= 4
  REGION_VAL (3)
  #endif
} ;

static const Mem_Region mem_region_c [] = // Constant region info (ROM)
{
  #if OS_NUM_HEAP_REGIONS >= 1
  REGION_VAL (0)
  #endif
  
  #if OS_NUM_HEAP_REGIONS >= 2
  REGION_VAL (1)
  #endif
  
  #if OS_NUM_HEAP_REGIONS >= 3
  REGION_VAL (2)
  #endif
  
  #if OS_NUM_HEAP_REGIONS >= 4
  REGION_VAL (3)
  #endif
} ;

#undef  REGION_VAL

//--------------------------------------------------------------------------//

static Mem mem ;

SInt8  os_stk_dir = -1 ;  // +1 = INC from STK_BEG , -1 = DEC from STK_END

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

__asm void os_kern_init_hw_1 (void)
{
  IMPORT UND_Stack_Size
  IMPORT ABT_Stack_Size
  IMPORT FIQ_Stack_Size
  IMPORT IRQ_Stack_Size

  /// SYSTEM MODE ///

  stmfd sp!,{r0-r3}          // Save working regs

  mov   r0,r13               // Save SYS stack pointer
  mov   r1,r14               // Save SYS link  pointer

  swi   #0                   // Switch to SUPERVISOR mode, IRQ disabled

  /// SUPERVISOR MODE ///

  mov   r13,r0               // Use SYS stack pointer
  mov   r14,r1               // Use SYS link  pointer

  /// Setup stack fences on the UND,ABT,FIQ,IRQ stacks ///

  mrs   r1,CPSR              // Copy CPSR to R1 as working copy for mode switching

  /// ABT Fence

  orr   r1,r1,#0x04          // ABORT mode
  msr   CPSR_c,r1

  ldr   r0,=ABT_Stack_Size
  bl    wr_fence

  /// UND Fence

  bic   r1,r1,#0x04
  orr   r1,r1,#0x08
  msr   CPSR_c,r1
     
  ldr   r0,=UND_Stack_Size
  bl    wr_fence

  /// FIQ Fence

  bic   r1,r1,#0x0A
  msr   CPSR_c,r1
     
  ldr   r0,=FIQ_Stack_Size
  bl    wr_fence

  /// IRQ Fence

  bic   r1,r1,#0x01
  orr   r1,r1,#0x02
  msr   CPSR_c,r1
     
  ldr   r0,=IRQ_Stack_Size
  bl    wr_fence

  /// Return to SUPERVISOR mode, clenup stack, and return to caller ///

  orr   r1,r1,#0x01
  msr   CPSR_c,r1

  ldmfd sp!,{r0-r3}          // Restore working regs

  mov   pc,lr

wr_fence

  sub   r2,sp,r0             // Compute stack BEG in R2
  ldr   r3,=0xBBBBBBBB
  str   r3,[r2]              // Write BEG fence

  ldr   r3,=0xEEEEEEEE
  stmfd sp!,{r3}             // Write END fenced and adjust SP

  mov   pc,lr
}

//--------------------------------------------------------------------------//

void os_kern_init_hw_2 (void)
{
}

//--------------------------------------------------------------------------//

void * os_kern_tsk_start_hw (void * stk_top,TskFunc tsk_func,void * tsk_arg)
{
  UInt32 * stk_ptr = (UInt32 *) stk_top ;

  *--stk_ptr = (UInt32) tsk_func        ; // r15 To task entry point
  *--stk_ptr = (UInt32) os_tsk_stop     ; // r14 To task exit  point on return
  *--stk_ptr = (UInt32) stk_top  - 4*17 ; // r13 To bottom of context storage

  #if OS_TASK_START_REG_FILL == 0

    stk_ptr -= 12 ;           // R1-r12         

  #else

    *--stk_ptr = 0xCCCCCCCC ; // r12
    *--stk_ptr = 0xBBBBBBBB ; // r11
    *--stk_ptr = 0xAAAAAAAA ; // r10
    *--stk_ptr = 0x99999999 ; // r9 
    *--stk_ptr = 0x88888888 ; // r8 
    *--stk_ptr = 0x77777777 ; // r7 
    *--stk_ptr = 0x66666666 ; // r6 
    *--stk_ptr = 0x55555555 ; // r5 
    *--stk_ptr = 0x44444444 ; // r4 
    *--stk_ptr = 0x33333333 ; // r3 
    *--stk_ptr = 0x22222222 ; // r2 
    *--stk_ptr = 0x11111111 ; // r1 

    #warning DEBUG: On task start, R1 to R12 will be filled with test values.

  #endif

  *--stk_ptr = (UInt32) tsk_arg ; // r0  (tasks initial arg)

  *--stk_ptr = 0x0000001F       ; // SYS mode, IRQ=ENA , FIRQ=ENA

  return (void *) stk_ptr ;
}

//--------------------------------------------------------------------------//

__asm void os_kern_process_cmd_hw (K_Func k_func,void * arg1,va_list args)
{
  preserve8

  import os_sys_stk
  import os_kern_process_cmd

  swi   #0                      // Enter SUPERVISOR mode with IRQ disabled

  stm   sp,{lr}^                // R15 saved (task return address taken from SYSTEM LR reg)
  nop                           // Avoid banking hazzard

  sub   sp,sp,#4*15             // Point to R0 storage

  stm   sp,{r0-r14}^            // Save R0-R14 (R13,R14 are SYSTEM regs)
  nop                           // Avoid banking hazzard

  mrs   lr,SPSR                 // Fetch tasks saved CPSR
  stmfd sp!,{lr}                // Save it

  ldr   sp,=os_sys_stk          // Get OS stack variable
  ldr   sp,[sp]                 // Setup SUPERVISOR stack for OS call

  sub   sp,#4                   // Align to 8-byte boundry

  b     os_kern_process_cmd     // Goto OS command processor
}

//--------------------------------------------------------------------------//

__asm void os_kern_tsk_execute_hw (void)
{
  import os_stk_frame

  ldr   sp,=os_stk_frame      // Fetch Task stack frame
  ldr   sp,[sp]

  ldmfd sp!,{r0}              // Fetch task CPSR
  msr   SPSR_cxsf,r0          // Setup for restore

  ldm   sp ,{R0-R14}^         // Restore R0 to R14 (R13,R14 in SYSTEM banks)
  nop                         // Avoid banking hazzard

  add   sp,sp,#4*15           // Point to R15

  ldm   sp,{R15}^             // Restore CPSR from SPSR and set new PC in R15
}

//--------------------------------------------------------------------------//

__asm void os_kern_tsk_idle (void)
{
  import os_sys_stk
  import os_sys_idle_func

  ldr   sp,=os_sys_stk        // Get address of OS stack variable
  ldr   sp,[sp]               // Setup supervisor stack
  sub   sp,#4                 // Align to 8-byte boundry

  mov   r1,sp                 // Save copy for SYSTEM mode SP

  ldr   r0,=run_as_sys        // Get address of code to execute in SYSTEM mode
  stmfd sp!,{r0}              // Setup PC for switch to SYSTEM mode

  msr   SPSR_c,#0x0000009F    // Setup CPSR for idle function (mode=system,irq=disabled,firq=enabled,arm-code)

  ldm   sp,{r15}^             // Load PC from stack, CPSR from SPSR : Now executing code at 'run_as_sys' below in SYSTEM mode

run_as_sys

  nop                         // Avoid banking hazzard

  mov   sp,r1                 // Setup SYSTEM SP

  ldr   r0,=os_sys_idle_func  // Fetch poimter to system idle function
  ldr   r0,[r0]               // Fetch address of   "      "     "

  ldr   lr,=ret_from_idle     // Setup return address in LR
  mov   pc,r0                 // Call idle function

ret_from_idle

  msr   CPSR_c,#0x0000001F    // Enable IRQ and FIRQ : mode = SYSTEM

wait_for_irq

  b wait_for_irq              // Wait until IRQ causes task schedule
}

//--------------------------------------------------------------------------//

__asm void os_irq_enter (void)
{
  preserve8

  import os_irq_enter_c

  stmfd sp!,{r0,r1,lr}        // Save TASK R0-R1 and IRQ LR on IRQ stack

  mov   r1,sp                 // Fetch IRQ  SP
  mrs   r0,SPSR               // Fetch TASK CPSR

  add   sp,sp,#4*4            // Reset IRQ stack pointer for next IRQ entry

  mrs   lr,CPSR               // Fetch current CPSR
  orr   lr,lr,#0x01           // Setup SUPERVISOR mode
  msr   CPSR_c,lr             // Switch to SUPERVISOR mode

  ldr   lr,[r1,#+3*4]         // Fetch TASK PC
  sub   lr,lr,#4              // Correct offset generated by ARM CPU (stupid, but hey, that's how it is)
  stm   sp,{lr}               // R15 : Save in stack frame (SUPERVISOR SP points to top)
  
  sub   sp,sp,#13*4           // Create space for TASK R14 to R2
  stm   sp,{r2-r14}^          // R14 to R2 of TASK regs

  ldm   r1,{r2,r3}            // Fetch TASK R0-R1
  stmfd sp!,{r0,r2,r3}        // R1,R0, and CPSR of TASK regs

  ldr   sp,=os_sys_stk        // Get pointer to OS stack
  ldr   sp,[sp]               // Setup SUPERVISOR SP to service application IRQ function
  sub   sp,#4                 // Align to 8-byte boundry

  ldr   lr,[r1,#+2*4]         // Setup return to IRQ caller

  b     os_irq_enter_c        // Handle C part and return to caller
}

//--------------------------------------------------------------------------//

__asm void os_irq_leave (void)
{
  preserve8

  import os_irq_leave_c
  import os_kern_tsk_schedule

  bl os_irq_leave_c           // Call 'C' part of irq leave

  b  os_kern_tsk_schedule     // Go into scheduler : Does not return
}

//--------------------------------------------------------------------------//

void os_irq_enter_c (void)
{
  os_irq_executing = TRUE ;

  if (os_tsk_running)
    {
      os_tsk_running->stk_frame = os_stk_frame ;
  
      #if OS_USE_HOOK_CONTEXT == 1
       
      os_running_hook_save (os_tsk_running , os_tsk_running->tsk_hook_arg) ;

      #endif

      os_tsk_running->tsk_local = os_tsk_local ;
    }
}

//--------------------------------------------------------------------------//

void os_irq_leave_c (void)
{
  os_irq_executing = FALSE ;
}

//--------------------------------------------------------------------------//

UInt16 os_kern_mem_request_stk_extra (void)
{
  return 4U*(16U + 1U) + 4 ; // R0-R15,STATUS, 8-byte align
}

//--------------------------------------------------------------------------//

void * os_kern_stk_ptr_adjust (void * stk_ptr,UInt16 stk_size)
{
  return (UInt8 *) stk_ptr + stk_size ;
}

//--------------------------------------------------------------------------//

void os_mem_info (MemInfo * mem_info)
{
  ATOMIC
    (
      mem_info->addr_inc = +1 ;

      os_mem_region_find_max   (mem_info) ;

      os_mem_region_find_usage (mem_info) ;
    )
}

//--------------------------------------------------------------------------//

__asm void * os_mem_request (UInt16 size)
{
  preserve8

  import os_mem_request_k

  push {r1,lr}            // Save regs (must be 8-byte aligned)

  mov  r1,lr              // Get return address
  subs r1,#4              // Point to callers instruction

  bl   os_mem_request_k   // Request memory

  pop  {r1,pc}            // Return to caller with memory, NULL if error

  align
}

//--------------------------------------------------------------------------//

void * os_mem_request_k (UInt16 mem_size , void * caller_addr)
{
  UInt8 * mem_addr = 0 ;

  
      Mem_Region * const mem_region = os_mem_region_find_min_fit (mem_size) ;

      if (mem_region)
        {
          mem_addr = os_mem_region_allocate (mem_region , mem_size) ;
        }
      else
        {
          os_mem_fail_func (caller_addr , mem_size) ;
        }
    

  return mem_addr ;
}

//--------------------------------------------------------------------------//

static Mem_Region * os_mem_region_find_min_fit (UInt16 mem_size)
{
  Mem_Region * mem_region_min_fit  = 0            ;
  UInt32       mem_region_min_size = 0xFFFFFFFFUL ;
  Mem_Region * mem_region          = mem_region_d ;

  do
    {
      if (mem_region->mem_base)
        {
          UInt16 mem_region_size = mem_region->mem_limit - mem_region->mem_base ;
    
          if (mem_region_size >= mem_size && mem_region_size < mem_region_min_size)
            {
              mem_region_min_fit  = mem_region      ;
              mem_region_min_size = mem_region_size ;
            }
        }
    }
  while (++mem_region < mem_region_d + sizeof_array (mem_region_d)) ;

  return mem_region_min_fit ;
}

//--------------------------------------------------------------------------//

static UInt8 * os_mem_region_allocate (Mem_Region * const mem_region , UInt16 mem_size)
{
  UInt8 * mem_fill  ;

  UInt8 * mem_align ;

  UInt8 * mem_addr  = mem_region->mem_base ;

  mem_region->mem_base += mem_size ;

  mem_align = mem_region->mem_base ;

  mem_region->mem_base += (0x08 - (UInt8) mem_region->mem_base) & 0x07 ; // 8-byte align

  if (mem_region->mem_base < mem_region->mem_limit)
    {
      mem_fill = mem_region->mem_base  ;
    }
  else
    {
      mem_fill = mem_region->mem_limit ;

      mem_region->mem_base = mem_region->mem_limit = 0 ;
    }

  mem.heap_mem_allocated += mem_fill - mem_addr  ;
  mem.heap_mem_unuseable += mem_fill - mem_align ;

  do
    {
      *--mem_fill = OS_MEM_FILL_VAL ;
    }
  while (mem_fill > mem_addr) ;

  return mem_addr ;
}

//--------------------------------------------------------------------------//

static void os_mem_region_find_max (MemInfo * const mem_info)
{
  SInt16       mem_region_max_index = -1           ;
  UInt32       mem_region_max_size  =  0           ;
  Mem_Region * mem_region           = mem_region_d ;

  do
    {
      if (mem_region->mem_base)
        {
          UInt16 mem_region_size = mem_region->mem_limit - mem_region->mem_base ;
    
          if (mem_region_size > mem_region_max_size)
            {
              mem_region_max_index = mem_region - mem_region_d ;
              mem_region_max_size  = mem_region_size           ;
            }
        }
    }
  while (++mem_region < mem_region_d + sizeof_array (mem_region_d)) ;

  if (mem_region_max_index == -1)
    {
      mem_info->addr_min  = 0 ;
      mem_info->addr_max  = 0 ;
      mem_info->addr_next = 0 ;
    }
  else
    {
      mem_info->addr_min  = mem_region_c [mem_region_max_index].mem_base       ;
      mem_info->addr_max  = mem_region_c [mem_region_max_index].mem_limit - 1U ; 
      mem_info->addr_next = mem_region_d [mem_region_max_index].mem_base       ;
    }
}

//--------------------------------------------------------------------------//

static void os_mem_region_find_usage (MemInfo * const mem_info)
{
  Mem_Region const * mr_c  = mem_region_c ;
  Mem_Region const * mr_d  = mem_region_d ;

  mem_info->heap_allocated = mem.heap_mem_allocated ;
  mem_info->heap_unuseable = mem.heap_mem_unuseable ;

  mem_info->heap_remaining = 0 ;

  do
    {
      if (mr_d->mem_base)
        {
          mem_info->heap_remaining += mr_d->mem_limit - mr_d->mem_base ;
        }
    }
  while (++mr_d , ++mr_c < mem_region_c + sizeof_array (mem_region_c)) ;
}

