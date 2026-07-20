/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_rf_car_finder.c 17934 2012-10-29 19:23:36Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_system_private.h"
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

static unsigned char tdelay;  // the only values are: 10, 30, 120

// also called when the vrtc expires...
void dei_system_headlight_process(void)
{
	
		if( get_headlight_source() == SOURCE_NONE // no headlight source?
			|| get_headlight_wire_status() == FALSE  // no detection
		  || get_rs_gwr() // remote start running doesn't care about headlights
		  || !get_ss_lock() // we don't care if system is unlocked
      )
      return;  // stop checking, there's nothing to do		

		
    // the "next" check will happen in tdelay seconds
		vrtc_stop( dei_system_headlight_alert_vrtc );
		vrtc_update( dei_system_headlight_alert_vrtc, VRTC_1SEC * tdelay );
		vrtc_start( dei_system_headlight_alert_vrtc );

		
		// decide what delay will come after that
		if(tdelay==10) // first time?
		{
			tdelay = 30; // next time will be 30 seconds
			return;  // we don't send headlight alert when called by dei_system_headlight_begin
		}
		else // 30 or 120
			tdelay = 120; // all the rest of the delays will be 120 seconds
		
		
		// send headlight alert now
		dei_sc_set_alert_event( 0x02 );
		notify_send( NOTIFY_CONVENIENCE_ALERT  ) ; 			
}


// called when headlight monitoring process should begin
void dei_system_headlight_begin( void )
{
  tdelay=10; // start the machine from the beginning
  dei_system_headlight_process();
}



//--------------------------------------------------------------------------//
