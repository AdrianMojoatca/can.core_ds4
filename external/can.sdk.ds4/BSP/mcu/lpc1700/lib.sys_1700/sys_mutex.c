/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sys_mutex.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*
//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
*/

//---------------------------------------------------------------------------//
//                                 NOTE                                      //
//                                                                           //
// Either BASEPRI or PRIMASK can be selected as the means of protecting      //
// malloc() & free() calls.                                                  //
//                                                                           //
// The PRIMASK method will block *all* interrupts regardless of level.       //
//                                                                           //
// The BASEPRI method will block only levels equal to or lower than the      //
// one specified by BASEPRI_LEVEL.                                           //
//                                                                           //
// If it can be gauranteed that no interrupt code will ever call malloc/free //
// then the BASEPRI method can be used to allow higher priority interrupts   //
// to preempt the malloc/free execution.                                     //
//                                                                           //
// Note that in C++, exception handling and some virtual-member constructor  //
// code can call mallo/free.  If any code will be called from an interrupt   //
// handler that could potentially preempt the malloc/free code, it should    //
// be protected with PRIMASK.  Note that this can create larger IRQ latency  //
// delays than the BASEPRI method.                                           //
//                                                                           //
// Choose carefully.                                                         //
//---------------------------------------------------------------------------//

// <e0> Use BASEPRI for malloc/free mutex
// <i>  Check to use BASEPRI as locking method.  Othewise
// <i>  PRIMASK will be used.  If BASEPRI is used, it will
// <i>  use the level defined below.
// <o1> Priority level to use <1-31>
// <i>  The priority level to use when locking with BASEPRI.
// </e>

#define BASEPRI_USED  1
#define BASEPRI_LEVEL 31

//--------------------------------------------------------------------------//

#if BASEPRI_USED == 1

  #define REG_VAL         0xF8
  #define REG_NAME     BASEPRI
  #define REG_NAME_MAX BASEPRI_MAX

#else

  #define REG_VAL            1
  #define REG_NAME     PRIMASK
  #define REG_NAME_MAX PRIMASK

#endif

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static volatile UInt32 _primask_saved_ ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

int _mutex_initialize (UInt32 * mutex)
{
  *mutex = 0 ;

  return 1 ;
}

//--------------------------------------------------------------------------//

void _mutex_acquire (UInt32 * mutex)
{
  UInt32 _primask_current_ = REG_NAME ;

  REG_NAME_MAX = REG_VAL ;

  if ((*mutex)++ == 0)
    {
      _primask_saved_ = _primask_current_ ;
    }
}

//--------------------------------------------------------------------------//

void _mutex_release (UInt32 * mutex)
{
  if (--(*mutex) == 0)
    {
      REG_NAME = _primask_saved_ ;
    }
}

