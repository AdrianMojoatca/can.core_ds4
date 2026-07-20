/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: ctl_dl_gwr.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "ctl_dl_private.h"
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

void ctl_dl_gwr_on( void *arg )
{
	timeout_stop (ctl_dl_gwr) ;

	if (ctl_dl_flag_ign == IGN_ON && ctl_dl_flag_gwr == GWR_OFF)
	{
		#if DBG_CTL_DL == 1
		  dbg_tx_text ("\nctl_dl gwr_on") ;
		#endif

		if(prog_retrieve_option(FEAT_CTL_DOORLOCK) > CTL_DOORLOCK_DISABLE)
		{
			// Idle mode: we can unlock the doors after a delay, if IGN=ON / GWR=ON
			ctl_dl_defer_action(CTL_DL_ACTION_UNLOCK, IGN_ON, GWR_ON, UNLOCK_DELAY);
		}
	}
	ctl_dl_flag_gwr  = GWR_ON;
	ctl_dl_lock_done = FALSE;
}

void ctl_dl_gwr_off( void *arg )
{
	timeout_start (ctl_dl_gwr) ;	// start timeout to clr gwr flag
	#if DBG_CTL_DL == 1
	  dbg_tx_text ("\nctl_dl gwr_off") ;
	#endif
}
