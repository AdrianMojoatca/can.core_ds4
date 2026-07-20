/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_rf_unlock.c 19287 2013-02-18 12:08:05Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_system_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define UNLOCK_TYPE		((!dei_system_2nd_unlock ? CMD_UNLOCK_WITH_DISARM : 0)|\
                          ((((Cmd_Rf_Pld*)cmd_get_pld(remote))->bits.id!=0) ? CMD_UNLOCK_USER2 : 0 ))

#define DRIVER_DOOR_LOCK_PRIORITY_OFF   (dei_feature_get( DEI_FEAT_DRIVER_DOOR_UNLOCK_PRIORITY)==2)

#define DRIVER_DOOR_LOCK_PRIORITY_ON   (dei_feature_get( DEI_FEAT_DRIVER_DOOR_UNLOCK_PRIORITY)==1)

#define RS_ON_PARKING_LIGHT_OUTPUT_ON  (dei_feature_get( DEI_FEAT_PARKING_LIGHT_OUTPUT ) != 3)

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void dei_system_rf_unlock( void *remote ) {			
    set_rs_ign_disarm_oem(FALSE) ;  

    if( get_ss_panic() ) {	  
			if( get_ss_lock() )	{
				notify_send( NOTIFY_LOCK , DEI_NOTIFY_LOCK_PLD );
				}
			else {	
				notify_send( NOTIFY_RANGE_CHECK );
				}
			dei_system_panic_stop();
			return ; 
			}

		if( dei_rs_mts_ready_for_activation() )	{
			cmd_q_rs_insert( INPUT_RF_START_STOP_TOOGLE , 100 , 0 );
			return  ; 
			}

		if( get_ign_status() && !get_rs_gwr() )	{
			notify_send( NOTIFY_RANGE_CHECK );	
			
			if( DRIVER_DOOR_LOCK_PRIORITY_OFF )
				cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_UNLOCK_ALL1_ON , 100 , UNLOCK_TYPE ) ; 
			else
				cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_UNLOCK_DRIVER1_ON , 100 , UNLOCK_TYPE ) ; 	

			set_ss_lock(FALSE);
			return ; 
			}

		if( get_ss_lock() ) {		
			dei_system_ch3_link_disarm();					
			dei_system_ch4_link_disarm();					
			dei_system_ch5_link_disarm();					
			dei_system_ch6_link_disarm();					
			}
		
		set_ss_lock(FALSE);
		set_ss_arm(FALSE);
		if(dei_system_locked_by_ign_control)
		  dei_system_locked_by_ign_control = FALSE;
		if(dei_system_mts_by_passive_locking)
		  dei_system_mts_by_passive_locking = FALSE;

		
		if(!( *cmd_get_pld( remote ) & CMD_TRUNK_WITH_DISARM ))	
			notify_send( NOTIFY_UNLOCK  , DEI_NOTIFY_UNLOCK_PLD );
		
		if( dei_system_2nd_unlock )	{ // 2nd unlock has occurred
			//TRACE("2nd Unlock\r\n");
			cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_UNLOCK_ALL1_ON , 100 , UNLOCK_TYPE ) ; 	
			// Set 2nd unlock flag to FALSE and stop timer
			dei_system_2nd_unlock = FALSE ; 
			vrtc_stop(vrtc_system_2nd_unlock);
			}
		else { // First unlock
			//TRACE("1st Unlock\r\n");
			if( dei_feature_get(DEI_FEAT_OEM_ALARM_DISARM_OUTPUT)!= DEI_OEM_DISARM_RS_ONLY ) {					
				cmd_q_rs_insert( INPUT_DISARM, 100 ) ; 
				}

			//check for do we need ignition on before UNLOCK
			if( dei_feature_get(DEI_FEAT_IGNITION_DISARM)==DEI_IGNITION_DISARM_ON )	{
				if( get_rs_gwr() == FALSE ) {
					set_wire_ign_skip_status( TRUE );
				
					set_rs_gwr(TRUE);
					cmd_q_rs_insert(INPUT_GWR_ON , 100 , 0 );
					
					os_tsk_wait(100);

					set_rs_acc(TRUE);
					cmd_q_rs_insert(INPUT_RS_ACCESSORY_ON , 100 , 0 );
					
					os_tsk_wait(100);

					set_rs_ign(TRUE);
					cmd_q_rs_insert(INPUT_RS_IGNITION_ON , 100 , 0 );
					
					os_tsk_wait(800);
					set_wire_ign_skip_status( FALSE );

					set_rs_ign_disarm_oem(TRUE) ;
					}
				}
			
			//unlock action		
			if( DRIVER_DOOR_LOCK_PRIORITY_OFF ) {
				cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_UNLOCK_ALL1_ON , 100 , UNLOCK_TYPE ) ; 	
				}
			else {
				cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_UNLOCK_DRIVER1_ON , 100 , UNLOCK_TYPE ) ; 
				// Set 2nd unlock flag to TRUE and start timer
				dei_system_2nd_unlock = TRUE ; 
				vrtc_stop(vrtc_system_2nd_unlock);
				vrtc_update(vrtc_system_2nd_unlock, SYSTEM_2ND_UNLOCK_WINDOW);
				vrtc_start(vrtc_system_2nd_unlock);
				}

			//parking lights action		
			if( get_ss_sensor_npc(1) || get_ss_sensor_npc(2) || get_ss_sensor_npc(0) ) 
			{
				os_tsk_wait(500);
				if( dei_feature_get(DEI_FEAT_PLIGHT_CONFIRMATION) != 2 )
				  dei_prk_flash( DEI_PRK_UNLOCK_NPC_ON ) ; 
			}
			else
			{		  
				if( get_ss_door_trigger() || get_ss_hood_trigger() || get_ss_trunk_trigger() || get_ss_ign_trigger() || get_ss_sensor_trigger(1) || get_ss_sensor_trigger(2) || get_ss_sensor_trigger(0) )
				{				
					os_tsk_wait(500);
					if( dei_feature_get(DEI_FEAT_PLIGHT_CONFIRMATION) != 2 )
					  dei_prk_flash( DEI_PRK_UNLOCK_TRIGGER_ON ) ;
				}
				else
				{
					if( dei_feature_get(DEI_FEAT_PLIGHT_CONFIRMATION) != 2 )
					{	//parking light off for convenience					
						os_tsk_wait(500);  //delay a bit to avoid the affect of undescovered CAN msg (HKHT1, HK4, etc)
						dei_prk_flash( (get_rs_gwr() && get_rs_start_successful() && RS_ON_PARKING_LIGHT_OUTPUT_ON ) ? DEI_PRK_UNLOCK_NO_EVENT_RS : DEI_PRK_UNLOCK_NO_EVENT ) ; 
					}
				}
			}
		}

		//ignition off if we turned it on before unlock	
		if( get_rs_ign_disarm_oem() == TRUE ) {        
			set_wire_ign_skip_status( TRUE );
			
			if( get_rs_ign() == TRUE ) {
				set_rs_ign(FALSE);
				cmd_q_rs_insert(INPUT_RS_IGNITION_OFF , 100 , 0 );               
				}
				
			os_tsk_wait(200);
				
			if( get_rs_acc() == TRUE ) {
				set_rs_acc(FALSE);
				cmd_q_rs_insert(INPUT_RS_ACCESSORY_OFF , 100 , 0 );               
				}
				
			os_tsk_wait(200);

			if( get_rs_gwr() == TRUE ) {            
				set_rs_gwr(FALSE);
				cmd_q_rs_insert(INPUT_GWR_OFF , 100 , 0 );   
				}        
				
			set_wire_ign_skip_status( FALSE );
			set_rs_ign_disarm_oem(FALSE) ;
			}
	}

//-------------------------------------------------------------------------------------------------------------//
void dei_system_2nd_unlock_reset    (void)
{
	  dei_system_2nd_unlock = FALSE;
		dei_ss_2nd_disarm = FALSE;
}


//-------------------------------------------------------------------------------------------------------------//
#undef UNLOCK_TYPE    
#undef DRIVER_DOOR_LOCK_PRIORITY_ON
#undef DRIVER_DOOR_LOCK_PRIORITY_OFF
#undef RS_ON_PARKING_LIGHT_OUTPUT_ON
