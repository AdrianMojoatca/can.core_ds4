/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: wdog_v_tick.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
 
#include "wdog_v_private.h"
#include "wdog.h"
#include "wake.h"

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

void wdog_v_tick(void)
{
	UInt32 interval;

	// Keep a system tick count to be used as a timestamp for virtual watch instances
	#ifdef DEV_LPC_176x
	if(!wake_going_sleep)
	#endif
		wdog_v_ticks++;	

	// If preamble function has been running for too long, force a reset
	if (preamble_func_running_since != 0)
	{
		if (wdog_v_ticks < preamble_func_running_since) 
		{
			// Handle rollover of system ticks
			interval = (wdog_v_ticks + (0xFFFFFFFF - preamble_func_running_since));
		}
		else
		{
			interval = wdog_v_ticks - preamble_func_running_since;
		}
		if (interval >= PREAMBLE_TIMEOUT)
		{
#ifndef WDOG_V_ONLY_LOG
			wdog_force_reset(0);
#endif
		}
	}
}



