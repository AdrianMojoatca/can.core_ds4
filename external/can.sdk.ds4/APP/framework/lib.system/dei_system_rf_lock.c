/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_rf_lock.c 17934 2012-10-29 19:23:36Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_system_private.h"
#include "dei_ss.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define RS_ON_PARKING_LIGHT_OUTPUT_ON  (dei_feature_get( DEI_FEAT_PARKING_LIGHT_OUTPUT ) != 3)

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern UInt8 					dei_ss_arm_cmd;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_system_rf_lock( void *remote )
{				
  	if( get_ss_panic()  )
	{
	    if( get_ss_lock() )
		{
			notify_send( NOTIFY_LOCK , DEI_NOTIFY_LOCK_PLD );
		}
		else
		{	
			notify_send( NOTIFY_RANGE_CHECK );			
		}

		dei_system_panic_stop();

		return ; 
	}
	
	
	if( dei_rs_mts_ready_for_activation() )
	{
		cmd_q_rs_insert( INPUT_RF_START_STOP_TOOGLE , 100 , 0 );

		return  ; 
	}
	
		
  	if( (get_ign_status() && !get_rs_gwr()) )
	{
		dei_system_rf_range_check();
		
		return ; 
	}
    				
	
							 			
				
  	if( !get_ss_lock() && !get_ss_one_time_bypass()  ) 
	{		
		dei_system_ch3_link_arm();
		dei_system_ch4_link_arm();
		dei_system_ch5_link_arm();
		dei_system_ch6_link_arm();
	}
		
    if( !get_ss_lock() )
	{
		cmd_q_rs_insert( INPUT_ARM   , 100 , (void*)( CMD_LOCK_WITH_ARM)) ; 
	}
						
  
	#define COMFORT		(((dei_feature_get(DEI_FEAT_COMFORT_CLOSURE)!=1) && !get_ss_one_time_bypass()) ? CMD_LOCK_WITH_COMFORT : 0)

		cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_LOCK1_ON , 100 , ( COMFORT | CMD_LOCK_WITH_ARM) ) ; 
//		cmd_q_rs_insert( INPUT_LOCK1 | INPUT_ON , 100 , (void*)( COMFORT | CMD_LOCK_WITH_ARM) ) ; 
	
	#undef COMFORT
	
	
    set_ss_lock( TRUE );
	  dei_system_2nd_unlock = FALSE;
	  vrtc_stop(vrtc_system_2nd_unlock);

	//--- will not notify the door status
    //--- the SS will notify after zone bypass timeout
  if( dei_ss_arm_cmd_status() == DEI_SS_ARM_NONE )
    notify_send( NOTIFY_LOCK , DEI_NOTIFY_LOCK_PLD );		

	
	if( get_rs_mts_lock() && !get_rs_turbo()  ) //the lock command was for MTS activation 
	{
		dei_system_wait_ign_off(2000);
	}
    

    if( dei_feature_get(DEI_FEAT_PLIGHT_CONFIRMATION) != 2 )//parking light off
    {
			if(dei_ss_arm_cmd==DEI_SS_SENSOR_SILENT_ARM) dei_prk_flash(DEI_PRK_SENSOR_SILENT_ARM);
			else if(dei_ss_arm_cmd==DEI_SS_FULL_SILENT_ARM) dei_prk_flash(DEI_PRK_FULL_SILENT_ARM);
      else dei_prk_flash( (get_rs_gwr() && RS_ON_PARKING_LIGHT_OUTPUT_ON) ? DEI_PRK_LOCK_RS : DEI_PRK_LOCK );
    }
		
		if( get_headlight_source() != SOURCE_NONE)
		{
			if( get_headlight_wire_status() )
			{
				cmd_q_rs_insert( INPUT_HEADLIGHT_STATUS_ON   , 100 ) ; 
			}
			else
			{
				cmd_q_rs_insert( INPUT_HEADLIGHT_STATUS_OFF   , 100 ) ; 
			}
		}
}

//--------------------------------------------------------------------------//
#undef RS_ON_PARKING_LIGHT_OUTPUT_ON


