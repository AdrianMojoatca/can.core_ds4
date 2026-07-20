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

void dei_rs_rf_combo_valet( void ) {		
	
	if( ! dei_rs_state ) {
		
		if( !(get_rs_valet() || get_ss_valet()) ) { // Combo Valet ON
			set_rs_valet(TRUE);
			set_ss_valet(TRUE);
			dei_led_flash(DEI_LED_VALET_ON, LED_ENABLE);		
			notify_send(NOTIFY_COMBO_VALET_ON);
			dei_prk_flash(DEI_PRK_RS_VALET_ON);
			dei_siren_chirp(DEI_SIREN_COMBO_VALET_ON);
			}
		else { // Combo Valet OFF
			set_rs_valet(FALSE);
			set_ss_valet(FALSE);
			dei_led_flash(DEI_LED_DISARM, LED_ENABLE);		
			notify_send(NOTIFY_COMBO_VALET_OFF);
			dei_prk_flash(DEI_PRK_RS_VALET_OFF);
			dei_siren_chirp(DEI_SIREN_COMBO_VALET_OFF);
			}
		}
	else
	{
		notify_send( NOTIFY_ERROR, NOTIFY_PLD_ERROR_RS_VALET );
	}
	
	if( get_rs_timer() )
	{
		set_rs_timer( FALSE );
		dei_sc_info_set( DEI_SC_INFO_TIMER_MODE , FALSE);        
	}
	if( get_rs_smart() )
	{
		set_rs_smart( FALSE );
		dei_sc_info_set( DEI_SC_INFO_SMART_MODE , FALSE);        
	}
	
	notify_send( get_ss_valet() ? NOTIFY_VALET_ON : NOTIFY_VALET_OFF );

}

//--------------------------------------------------------------------------//

