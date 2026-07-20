/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_trigger.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rs_private.h"
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

//--------------------------------------------------------------------------//

void dei_rs_trigger_door( void )
{		
	if( ( get_ss_lock() || ( !get_rs_idle() && dei_rs_pts_out_is_programmed() )) && dei_rs_state)
	{
		if( dei_rs_mts_door_checking_timeout_status() == FALSE )
        {
            if( dei_rs_trigger_enable ||  ( !get_rs_idle() && dei_rs_pts_out_is_programmed()) )
            {
							if(dei_rs_state == dei_rs_running && !(get_ss_valet()))    // during try start, do not trigger
                dei_rs_evt_signal( DEI_RS_EVT_TRIGGER ) ;                 
            }
        }
	}
}

//--------------------------------------------------------------------------//

void dei_rs_trigger_trunk( void )
{  
#if 0	
	if( get_ss_lock() && dei_rs_state && !get_ss_trunk_release()  )
	{
	    if( dei_rs_mts_door_checking_timeout_status() == FALSE )
        {
            if( dei_rs_trigger_enable && !get_zone_bypass_status())
            {
                dei_rs_evt_signal( DEI_RS_EVT_TRIGGER );    
            }
        }
	}
#endif	
}

//--------------------------------------------------------------------------//

void dei_rs_trigger_generic( void )
{
		set_rs_mts(FALSE);
		notify_send( NOTIFY_MTS_DISABLE );	
#ifdef PLATFORM_933  
		cmd_q_rs_insert( INPUT_GND_WHEN_READY_MODE_OFF, 100 );
	
		if( dei_rs_trigger_enable && dei_rs_state)
				dei_rs_evt_signal( DEI_RS_EVT_TRIGGER );    
	
#endif  
}

//--------------------------------------------------------------------------//

Boolean dei_rs_pts_out_is_programmed( void )
{
	UInt8 i ; 

	for( i = 0 ; i < DEI_PROG_OUT_COUNT ; i ++ )
	{
		if( dei_feature_get( DEI_FEAT_OUTPUT_BASE(i) ) == DEI_OUT_PRG_SMARTKEY_CONTROL_PTS )
		{
		 	return TRUE ; 
		}
	}

	return FALSE ; 
}

//--------------------------------------------------------------------------//

