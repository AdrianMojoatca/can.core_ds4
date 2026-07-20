/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: wdog_v_release_watch.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "wdog_v_private.h"

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

Boolean wdog_v_release_watch (vWatch instance)
{
	CRITICAL ;

	// In case of error, this function returns FALSE
	// We don't log errors, we are too low-level; it's the caller's responsability

	// Sanity checks
	if (instance == NULL) return FALSE;
	if (!instance->active) return TRUE;

	// Deactivate
	C_ENTER ;
    {
		// Don't actually release it from memory (we are in an embedded environment)
		// Just make instance inactive - it can be reused later
		instance->active    = FALSE;
		instance->timestamp = 0;
    }
	C_LEAVE ;

	return TRUE;
}

