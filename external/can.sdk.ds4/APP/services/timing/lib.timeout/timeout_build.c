/*==========================================================================*/
// $Id: timeout_build.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "sys.h"
#include "timeout_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Timeout timeout_build (UInt32 timeout, Timeout_Func func, UInt16 sizeof_obj)
{
  CRITICAL ;

  Timeout timeout_obj = (Timeout) os_mem_request (sizeof_obj) ;

  timeout_obj->timeout_count     = 0       ;
  timeout_obj->timeout_assigned  = timeout ;
  timeout_obj->timeout_func      = func    ;
  timeout_obj->timeout_func_done = TRUE    ;

  C_ENTER ;
    {
      register_with_timer :
        {
          static Boolean do_init = TRUE ;

          if (do_init)
            {
              do_init = FALSE ;
    
              sys_timer_register (timeout_service , 1) ;
            }
        }

      add_to_list :
        {
          timeout_obj->next = timeout_list ;

          timeout_list = timeout_obj       ;
        }
    }
  C_LEAVE ;

  return timeout_obj ;
}

