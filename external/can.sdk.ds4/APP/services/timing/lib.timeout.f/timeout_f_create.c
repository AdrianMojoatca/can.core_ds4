/*==========================================================================*/
// $Id: timeout_f_create.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "timeout_f_private.h"
#include "wdog_v.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Boolean free_q_ok (void);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Timeout timeout_f_create (UInt32 timeout, Exec_Func exec_func)
{
  	Timeout timeout_obj = timeout_build (timeout, timeout_f_timeout_func, sizeof (Timeout_F)) ;
 
  	timeout_f_update (timeout_obj, exec_func) ;

  	register_with_timer :
	{
	  static Boolean do_init = TRUE ;
	
	  if (do_init)
	    {
	      do_init = FALSE ;
	
	      timeout_exec_q_free = exec_q_free_create (1) ;
	    }
	}

  	return timeout_obj ;
}

// This will be called by wdog_v to check if a free q still has available free messages;
// A free q should never be empty - that can only happen if q_return() is not used properly.
Boolean free_q_ok (void)
{	
	return !q_empty(timeout_exec_q_free);
}

