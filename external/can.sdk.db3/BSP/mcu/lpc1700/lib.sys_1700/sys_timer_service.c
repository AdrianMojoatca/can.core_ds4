/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sys_timer_service.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "sys_private.h"
#include "wdog_v.h"


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

/// Executes in SUPERVISOR mode ///

void sys_irq_timer_service (void)
{
     os_service_timer () ;       // Service the OS timeout structure

      /*service_list :*/
        {
          Timer_Entry * curr ;

		// Place a virtual watch here in case:
		//   a) the LUT is broken or circular
		//   c) a service call blocks

		  // Note: the virtual watchdog library relies on this service to check timeouts...
		  //       If a service call blocks here, the WDT will probably catch it, not the VIRTUAL_WATCH... 

		  VIRTUAL_WATCH
		  (
					
						for (curr = timer_list ; curr ; curr = curr->next)
	            {
	              if (--curr->timeout == 0)
	                {
	
	                  curr->timer_func () ;
	
	                  curr->timeout = curr->timeout_assigned ;
	                }
	            }
			);
        }
		SCB->ICSR = 	0x02000000;				// Clear Systick intterrupt	
}

