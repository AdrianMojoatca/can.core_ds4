/****************************************************************************/
/*                                                                          */
/*                         A-Task Real-Time Kernel                          */
/*                                                                          */
/*                  Copyright (C) 2000-2009 - Acacetus Inc                  */
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
/*                       HI-MEM |  xPSR | <<-- Task xPSR status             */
/*                              +-------+                                   */
/*                              |  R15  | <<-- Task PC                      */
/*                              +-------|                                   */
/*                              |  R14  | <<-- Task LR                      */
/*                              +-------+                                   */
/*                              |  R12  |                                   */
/*                              +-------+                                   */
/*                              |  R3   |                                   */
/*                              +-------+                                   */
/*                              |  R2   |                                   */
/*                              +-------+                                   */
/*                              |  R1   |                                   */
/*                              +-------+                                   */
/*                              |  R0   | <<-- Task Argument 'tsk_arg'      */
/*                              +-------+                                   */
/*                              |BASEPRI| <<-- Stack Frame points here      */
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
/*                       LO-MEM |  R4   | <<-- Stack Frame points here      */
/*                              +-------+                                   */
/*                                                                          */
/*  Notes:                                                                  */
/*                                                                          */
/*  - All IRQs that interact with the OS must be at level 31                */
/*                                                                          */
/*  - SVC instruction is used by kernel to enter 'handler' mode and         */
/*    must always be assigned a priority of level 30                        */
/*                                                                          */
/*  - PRIBASE must never be any other value except level 0 or 31.           */
/*                                                                          */
/*  - Interrupts that do not interact with the OS use the MSP stack.        */
/*                                                                          */
/*  - The OS stack must be large enough to support nested interrupts        */
/*    for IRQ priorities higher than level 31.                              */
/*                                                                          */
/*  - Tasks use the PSP stack.                                              */
/*                                                                          */
/*  - On Cortex-M3, R13 is either MSP or USP, so not part of context.       */
/*                                                                          */
/*  - IRQs that interact with the OS *must* be level 31.                    */
/*                                                                          */
/*  - IRQs that don't can be any level, but only level 29 and higher will   */
/*    preempt the kernel (level 30 can preempt kernel only of kernel        */
/*    was not entered by SVC call.                                          */
/*                                                                          */
/*  - Use IRQ levels 0 to 29 for implementing virtual peripherals and       */
/*    signal OS with PENDSV.                                                */
/*                                                                          */
/*  - When signaling OS with PENDSV, its priority level *must* be set       */
/*    to level 31.                                                          */
/*                                                                          */
/****************************************************************************/

/*==========================================================================*/
// $Id: os.machine_c.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os_local.h"
 
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#if !defined(OS_USE_IDLE_STACK) || OS_USE_IDLE_STACK != 1
#error !!! Cortex-M3 Version of OS requires and IDLE stack : Must define OS_USE_IDLE_STACK=1 !!!
#endif

//--------------------------------------------------------------------------//

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
static Mem_Region * os_mem_region_find_min_fit (UInt16  mem_size) ;

void              * os_mem_request           (UInt16  mem_size) ;

static UInt8      * os_mem_region_allocate     (Mem_Region * const mem_region , UInt16 mem_size) ;

static void         os_mem_region_find_max     (MemInfo * const mem_info) ;

static void         os_mem_region_find_usage   (MemInfo * const mem_info) ;


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

SInt8   os_stk_dir   = -1 ;  // +1 = INC from STK_BEG , -1 = DEC from STK_END

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

#if OS_TASK_START_REG_FILL == 1

#warning DEBUG: Registers filled with debug values on task start.

#endif

void * os_kern_tsk_start_hw (void * stk_top , TskFunc tsk_func ,void * tsk_arg)
{
  volatile UInt32 * stk_ptr = (volatile UInt32 *) stk_top ;

  *--stk_ptr = 0x01000000           ; // xPSR
  *--stk_ptr = (UInt32) tsk_func    ; // R15 (PC)
  *--stk_ptr = (UInt32) os_tsk_stop ; // R14 (LR) 

  #if OS_TASK_START_REG_FILL == 1

  *--stk_ptr = 0xCCCCCCCC ;           // R12
  *--stk_ptr = 0x33333333 ;           // R3
  *--stk_ptr = 0x22222222 ;           // R2
  *--stk_ptr = 0x11111111 ;           // R1

  #else

  stk_ptr -= 4 ;

  #endif

  *--stk_ptr = (UInt32) tsk_arg ;     // R0 (task_arg)

  *--stk_ptr = 0x00000000 ;           // BASEPRI (IRQ Enabled)

  #if OS_TASK_START_REG_FILL == 1

  *--stk_ptr = 0xBBBBBBBB ;           // R11
  *--stk_ptr = 0xAAAAAAAA ;           // R10
  *--stk_ptr = 0x99999999 ;           // R9 
  *--stk_ptr = 0x88888888 ;           // R8 
  *--stk_ptr = 0x77777777 ;           // R7 
  *--stk_ptr = 0x66666666 ;           // R6 
  *--stk_ptr = 0x55555555 ;           // R5 
  *--stk_ptr = 0x44444444 ;           // R4 

  #else

  stk_ptr -= 8 ;

  #endif

  return (void *) stk_ptr ;
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
  return 4U*(16U + 1U + 1U + 1U) ; // R0-R12 , R14-R15 , xPSR , task return PC , align word
}

//--------------------------------------------------------------------------//

void * os_kern_stk_ptr_adjust (void * stk_ptr , UInt16 stk_size)
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

void * os_mem_request (UInt16 mem_size)
{
  UInt8 * mem_addr = 0 ;

  ATOMIC
    (
      Mem_Region * const mem_region = os_mem_region_find_min_fit (mem_size) ;

      if (mem_region)
        {
          mem_addr = os_mem_region_allocate (mem_region , mem_size) ;
        }
//      else
//        {
//          os_mem_fail_func (mem_size) ;
//        }
    )

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

  mem_align = mem_addr + mem_size;
	
  mem_region->mem_base = mem_addr + ((mem_size + 7) & 0xFFF8); // 8-byte align

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


//--------------------------------------------------------------------------//
