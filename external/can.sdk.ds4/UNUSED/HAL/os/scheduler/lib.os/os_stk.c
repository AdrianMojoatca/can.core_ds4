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
// $Id: os_stk.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static Boolean check_fence    (UInt8 fence_mark , UInt8 * stk_addr) ;
static UInt16  check_free_mem (const StkLimits * stk_limits) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void os_stk_info (Tsk tsk,StkInfo * stk_info)
{
  const StkLimits * stk_limits = tsk ? & tsk->stk_limits : & os_sys_stk_limits ;

  stk_info->fence_ok_beg = check_fence (OS_STK_FENCE_MARK_BEG,stk_limits->stk_beg - OS_STK_FENCE_SIZE) ;
  stk_info->fence_ok_end = check_fence (OS_STK_FENCE_MARK_END,stk_limits->stk_end + 1                ) ;

  stk_info->free_mem     = check_free_mem (stk_limits) ;

  stk_info->stk_beg      = stk_limits->stk_beg ;
  stk_info->stk_end      = stk_limits->stk_end ;
}

//--------------------------------------------------------------------------//

static Boolean check_fence (UInt8 fence_mark , UInt8 * stk_addr)
{
  UInt8 count = OS_STK_FENCE_SIZE ;
  
  do
    {
      if (*stk_addr++ != fence_mark)
        {
          return FALSE ;
        }
    }
  while (--count > 0U) ;

  return TRUE ;
}

//--------------------------------------------------------------------------//

static UInt16 check_free_mem (const StkLimits * stk_limits)
{
  SInt16  count = 0 ;

  UInt8 * mem_test  ;
  UInt8 * mem_limit ;

  if (os_stk_dir < 0)
    {
      mem_test  = stk_limits->stk_beg     ;
      mem_limit = stk_limits->stk_end + 1 ;
    }
  else
    {
      mem_test  = stk_limits->stk_end     ;
      mem_limit = stk_limits->stk_beg - 1 ;
    }

  do
    {
      if (mem_test [count] != OS_MEM_FILL_VAL)
        {
          break ;
        }

      count -= os_stk_dir ;
    }
  while (& mem_test [count] != mem_limit) ;

  return os_stk_dir < 0 ? +count : -count ;
}
