/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_common_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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
static void dei_system_aux_icon_init( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_system_common_init( void )
{			
	dei_system_aux_icon_init() ;
		
	dei_prk_init();	
			
	dei_system_ign_lock_init();
				
	dei_led_init();	
	
	dei_out_init();		

	dei_system_ign_stat_init();

	dei_system_rf_1btn_init();

    dei_system_panic_init();
	
	dei_system_smart_start_init();		
		
	dei_system_common_events(); //the events from common events should be executing last  

	dei_system_ign_dome_init();
	
	dei_system_prg_init();    
}    
//--------------------------------------------------------------------------//

void dei_system_aux_icon_init( void )
{
	const  Notify_Pld_Aux_Type aux_type[] = 
	{
		NOTIFY_PLD_AUX_TRUNK      , 
    	NOTIFY_PLD_AUX_WINDOW     , 
    	NOTIFY_PLD_AUX_SUNRROF    , 
    	NOTIFY_PLD_AUX_AUDIO      ,
    	NOTIFY_PLD_AUX_LIGHTS     ,
    	NOTIFY_PLD_AUX_LEFT_DOOR  ,
    	NOTIFY_PLD_AUX_RIGHT_DOOR ,
    	NOTIFY_PLD_AUX_REAR_HATCH ,
    	NOTIFY_PLD_AUX_TIMED      ,
    	NOTIFY_PLD_AUX_GARAGE_DOOR,
        NOTIFY_PLD_AUX_PULSED     ,
	};

	notify_send( NOTIFY_AUX_CH2_TYPE , aux_type[ dei_feature_get( DEI_FEAT_CH2_ICON   ) - 1 ] ) ; 
	notify_send( NOTIFY_AUX_CH3_TYPE , aux_type[ dei_feature_get( DEI_FEAT_AUX_1_ICON ) - 1 ] ) ; 
	notify_send( NOTIFY_AUX_CH4_TYPE , aux_type[ dei_feature_get( DEI_FEAT_AUX_2_ICON ) - 1 ] ) ; 
	notify_send( NOTIFY_AUX_CH5_TYPE , aux_type[ dei_feature_get( DEI_FEAT_AUX_3_ICON ) - 1 ] ) ; 	
}
	
//--------------------------------------------------------------------------//

