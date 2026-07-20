/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: ctl_dl_ign.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "ctl_dl_private.h"
#include "prog.h"
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

void ctl_dl_ign_on( void *arg )
{
	if (ctl_dl_flag_gwr == GWR_OFF && ctl_dl_flag_ign == IGN_OFF)
	{
	  #if DBG_CTL_DL == 1
	    dbg_tx_text ("\nctl_dl ign_on") ;
	  #endif

		if (PROG_CFG_CTL_DOORLOCK_IGNITION_ENABLE &&
		    (prog_retrieve_option(FEAT_CTL_DOORLOCK) == CTL_DOORLOCK_IGNITION))
		{
			// Lock the doors after a delay, if IGN=ON / GWR=OFF
			ctl_dl_defer_action(CTL_DL_ACTION_LOCK, IGN_ON, GWR_OFF, LOCK_DELAY);
		}
	}

	ctl_dl_flag_ign = IGN_ON;
	
}

void ctl_dl_ign_off( void *arg )
{
	if (ctl_dl_flag_gwr == GWR_OFF && ctl_dl_flag_ign == IGN_ON) 
	{
		#if DBG_CTL_DL == 1
		  dbg_tx_text ("\nctl_dl ign_off") ;
		#endif

		if(prog_retrieve_option(FEAT_CTL_DOORLOCK) > CTL_DOORLOCK_DISABLE)
		{
			// We can unlock the doors after a delay, if IGN=OFF / GWR=OFF
			ctl_dl_defer_action(CTL_DL_ACTION_UNLOCK, IGN_OFF, GWR_OFF, UNLOCK_DELAY);
		}
	}
	
	ctl_dl_lock_done = FALSE;

	ctl_dl_flag_ign = IGN_OFF;	
}

