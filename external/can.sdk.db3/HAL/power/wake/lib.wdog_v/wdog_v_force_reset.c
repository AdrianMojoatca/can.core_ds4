/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: wdog_v_force_reset.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include <string.h>

#include "wdog_v_private.h"
#include "wdog.h"

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

void wdog_v_force_reset(e_Wdog_v_Reset_Type reset_type, char *source, UInt16 line)
{

	// Store information about the reset in memory that will not be reset by C startup code

	// Source (C module, task...) - unfortunately we have limited space for this info...
	char *p = (char *)WDOG_V_RESET_SOURCE;
	memcpy(p, source, WDOG_V_MAX_RESET_SOURCE_SIZE);
	{
		// Pad source name with spaces if it is too short
		int i;
		for (i=0; source[i] && i<WDOG_V_MAX_RESET_SOURCE_SIZE; i++);
		for (; i<WDOG_V_MAX_RESET_SOURCE_SIZE; i++) p[i] = ' ';
	}

	// Line number (only applies to C module)
	WDOG_V_RESET_LINE = line;

	// Reset type
    WDOG_V_RESET_APP_CODE = reset_type ;

	// Call the preamble function, if one was registered
	if (preamble_list)
	{
		preamble_func_running_since = wdog_v_ticks;

		wdog_v_preamble_execute( reset_type, 
						reset_type == WDOG_V_RESET_STK_CHECK || reset_type == WDOG_V_RESET_SCHEDULED);
	}

	// For debug purposes, only log, don't reset
	#ifdef WDOG_V_ONLY_LOG
		if (!already_logged)
		{
			wdog_v_log_reset_type();
			already_logged = TRUE;
			preamble_list = NULL;
		}
	#else
		// Do the force-reset
		wdog_force_reset(reset_type);
	#endif

}

