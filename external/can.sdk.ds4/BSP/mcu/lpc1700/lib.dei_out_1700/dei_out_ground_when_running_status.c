/*==========================================================================*/
// $Id: dei_out_ground_when_running_status.c 17934 2012-10-29 19:23:36Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_out_private.h"
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

static UInt8 gwr_ignition_status = FALSE;
//--------------------------------------------------------------------------//

void dei_out_ground_when_running_status_on ( void  *payload )
{
// starter-kill becomes anti-grind

    gwr_ignition_status = get_ign_status();  // true for pit stop, false for rs situation

    prg_out_ground_when_running_status( TRUE );	

    ATOMIC( OUT_RF_LOOP( TRUE ) );

    dei_out_set_ag(gwr_ignition_status);  // antigrind is true only in pit stop situation, not rs

}

//--------------------------------------------------------------------------//

void dei_out_ground_when_running_status_off( void  *payload )
{
// anti-grind becomes starter-kill

    gwr_ignition_status = get_ign_status();  // true on user takeover, false on remote shutdown or finish ready mode

    prg_out_ground_when_running_status( FALSE );	

    ATOMIC( OUT_RF_LOOP( FALSE ) );
	
    dei_out_set_ag(FALSE); // "anti-grind" always stays off.
                           // no more antigrind whether user drives or whether user is parked
                           // starter kill is a diffent matter and might turn on due to lock/unlock or timer action
	
 
}

//--------------------------------------------------------------------------//

