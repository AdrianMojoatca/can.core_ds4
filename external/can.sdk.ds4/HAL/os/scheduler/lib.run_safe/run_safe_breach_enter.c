/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: run_safe_breach_enter.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "run_safe_private.h"

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
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

void run_safe_breach_enter (void)
{	
	Boolean door_trigger_open = FALSE ; 
	
	door_trigger_open = (door.byte > run_safe_entry.runsafe_door_status) ? TRUE : FALSE ; // Runsafe breach enter() gets called also at a door close if another door is opened at the same time. We have to determine iniside run_safe_breach_eneter() if its a door open or door close.
			
  if(run_safe_entry.gwr_state && run_safe_entry.run_safe_state && door_trigger_open )   // If its a door open and runsafe is already active, trigger the shutdown
  {
	  if (run_safe_entry.func )
    {
			run_safe_entry.func() ;
			
			#if DBG_RUN_SAFE == 1
				dbg_tx_text("\r\nRunSafe Breach Enter - Runsafe Shutdown") ;
			#endif
    }       
  }
	else if (run_safe_entry.gwr_state && !run_safe_entry.run_safe_state)       						// If runsafe wasnt active, restart the 45 seconds timer to activate it      
	{
		timeout_start (run_safe_entry.run_safe_timeout) ;
		      
		#if DBG_RUN_SAFE == 1
        dbg_tx_text("\nRunSafe Breach Enter/Leave - Timeout Start") ;
    #endif
	}
	else
	{
		#if DBG_RUN_SAFE == 1
        dbg_tx_text("\nRunSafe Breach Enter ") ;
    #endif
	}
  
	run_safe_entry.runsafe_door_status = door.byte ;
 				 
}

