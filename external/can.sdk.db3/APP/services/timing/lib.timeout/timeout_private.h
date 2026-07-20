/*==========================================================================*/
// $Id: timeout_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef _TIMEOUT_PRIVATE_H_
#define _TIMEOUT_PRIVATE_H_

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os.h"
#include "timeout.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

// <h>  Task 'timeout' settings
// <s0> Name
// <o0> Priority           <0-65535>
// <i>  Lowest  priority = 0
// <i>  Highest priority = 65535
// <o1> Stack Size (bytes) <0x0000-0xFFFF:8>
// <i>  Must be a multiple of 8.
// </h>

#define TASK_TIMEOUT_NAME  "timeout"
#define TASK_TIMEOUT_PRI   200
#define TASK_TIMEOUT_STK   0x100

//--------------------------------------------------------------------------//

typedef void (* Timeout_Func)(Timeout) ;

typedef struct s_Timeout
{
  UInt32       timeout_count     ;
  UInt32       timeout_assigned  ;
  Timeout_Func timeout_func      ; 
  Boolean      timeout_func_done ;

  struct s_Timeout * next ;

} Timeout_Entry ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void    timeout_service (void) ;
Timeout timeout_build   (UInt32 timeout, Timeout_Func func, UInt16 sizeof_obj) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Timeout_Entry * timeout_list ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

#endif




