/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: ctl_dl_brake.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "ctl_dl_private.h"
#include "status.h"
#include "dbg.h"

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

void ctl_dl_brake_press( void *arg )
{
	// If we come here, it's because the feature is programmed to act on BRAKE_PRESS

	// IGN must be ON, but don't check GWR yet - it may be ON for a short while until take-over

	if (ctl_dl_flag_ign == IGN_ON && !ctl_dl_lock_done && rpm.word)
	{
	  #if DBG_CTL_DL == 1
		  dbg_tx_text ("\nctl_dl brake_on") ;
	  #endif

		if (PROG_CFG_CTL_DOORLOCK_BRAKE_ENABLE &&
		    (prog_retrieve_option(FEAT_CTL_DOORLOCK) == CTL_DOORLOCK_BRAKE))
		{
			// Lock the doors after a delay, if IGN=ON / GWR=DONT_CARE
			ctl_dl_defer_action(CTL_DL_ACTION_LOCK, IGN_ON, GWR_DONT_CARE, BRAKE_PRESS_DELAY);
			// Only do this once per ignition cycle
			ctl_dl_lock_done = TRUE;
		}
	}

}
