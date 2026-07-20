/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_doors.c 1887 2010-10-18 15:19:00Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_private.h"
#include "config_pts_ctl.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Boolean ignore_doors_status;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void sense_doors_3B3( Can_Msg *can_msg )
{    
    //TRACE("\r\ncustom3_opt2_enable: %u\r\n", (unsigned int)get_prog_custom3_opt2_enable());

/* pune snippetul �n flow-ul de doors message (unde se executa la runtime) */
{
    static UInt32 last_pts_timeout = 0xFFFFFFFFUL;
    static UInt32 last_pts_pulse_timeout = 0xFFFFFFFFUL;

    UInt32 pts_timeout = (UInt32)PTS_CTL_TIMEOUT;
    UInt32 pts_pulse_timeout = (UInt32)PTS_CTL_PULSE_TIMEOUT;

    if ((pts_timeout != last_pts_timeout) || (pts_pulse_timeout != last_pts_pulse_timeout))
    {
        TRACE("\r\n[DOORS][PTS] ver=%lu timeout=%lu pulse=%lu",
              (unsigned long)CONFIG_PTS_CTL_VERSION,
              (unsigned long)pts_timeout,
              (unsigned long)pts_pulse_timeout);

        last_pts_timeout = pts_timeout;
        last_pts_pulse_timeout = pts_pulse_timeout;
    }
}
	
	if ( ignore_doors_status ) 
	{
		sense_rs_driver_door_can(FALSE );
		sense_rs_other_doors_can( FALSE );
		return; // ignore for rap shutdown
	}
	
	if ( !!(DATA1(7) & 0x20)  )
	{
		sense_rs_driver_door_can(TRUE );
	}
	else 
	{
		sense_rs_driver_door_can(FALSE );
	}
    if (!!(DATA1(7) & 0x10) || !!(DATA1(6) & 0x01) || !!(DATA1(6) & 0x02) )
	{
		sense_rs_other_doors_can( TRUE);
	}
	else
	{
		sense_rs_other_doors_can( FALSE );
	}
}

//--------------------------------------------------------------------------//

