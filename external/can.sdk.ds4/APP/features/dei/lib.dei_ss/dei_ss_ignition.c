/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_ignition.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ss_private.h"
#include "per_exec.h"
#include "out_q.h"
#include "config_firmware.h"
#ifdef PLATFORM_933
  #include "sense_rs.h"
	#include "dei_system.h"
	#include "dei_sc.h"
#endif

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

Boolean pts_ignore=FALSE;	// When true, next ignition sense will ignore alarm trigger

void dei_ss_ignore_pts_ign() {
	
	pts_ignore=TRUE;
	}

//--------------------------------------------------------------------------//

void dei_ss_ignition_on( void )
{	
	os_tsk_wait(DEI_SS_DELAY_TRIGGER);  	

  if( get_ss_arm() && !get_ss_valet() && !get_rs_gwr() && !pts_ignore)
	{
		dei_ss_trigger_start( DEI_SS_TRIGGER_ZONE_IGN ) ; 
		return;
	}    
	pts_ignore=FALSE;
	
	if( !get_ss_arm() && !get_rs_gwr())
	{
		vrtc_update( dei_ss_trg_history_timeout , DEI_SS_TRIG_HIST_REPORT_TIMEOUT);
		vrtc_start( dei_ss_trg_history_timeout );
		dei_led_flash ( DEI_LED_DISARM , LED_ENABLE);
	}
	
}	

//--------------------------------------------------------------------------//

void dei_ss_ignition_checking_after_gwr_off( void )
{
    PER_EXEC_ATOMIC
    (
        out_q_remove( out_q_main , (Out_Func)dei_ss_ignition_checking_timeout  , TRUE );
        
        out_q_insert( out_q_main , (Out_Func)dei_ss_ignition_checking_timeout  , 
#ifdef DEI_SS_IGNITION_CHECKING_TIMEOUT
		DEI_SS_IGNITION_CHECKING_TIMEOUT,
#else 
		100*4,
#endif
	1 , 1 , 1); 
	
    )
}

//--------------------------------------------------------------------------//

void dei_ss_ignition_checking_timeout( Boolean timeout_expired )
{
	/* Here is the moment when DS4 starts to check the ignition status
	   The protection of virtual "key sense" will be applied if there were:
	    - ignition control door locked
	    - passive locking from MTS completion option
	*/
	
  if( get_ss_arm() && !get_ss_valet() && !get_rs_gwr() && timeout_expired && get_ign_status() )
	{
		if( dei_system_locked_by_ign_control ||
			   (get_gearbox_type() == GEARBOX_MANUAL && dei_system_mts_by_passive_locking)
		  )
		{
        if(dei_feature_get( DEI_FEAT_CONTROL_UNLOCK_ALL_DOOR ) == 2)    //UNLOCK_ALL_DOORS 
					cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_UNLOCK_ALL1_ON , 100 , CMD_UNLOCK_IGNITION ) ; 	
				else
					cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_UNLOCK_DRIVER1_ON , 100 , CMD_UNLOCK_IGNITION ) ; 
				
				set_ss_lock(FALSE);  // LCD screen will display with update of "UNLOCK + DISARM" status 
				
				if(dei_system_locked_by_ign_control)
				  dei_system_locked_by_ign_control = FALSE;
				if(dei_system_mts_by_passive_locking)
					dei_system_mts_by_passive_locking = FALSE;
		}
		else
		{
		  dei_ss_trigger_start( DEI_SS_TRIGGER_ZONE_IGN ) ; 
		}
	}
}  

//--------------------------------------------------------------------------//

void dei_ss_ign_on_for_trig_history(void)
{
	UInt8 i;
	
	vrtc_stop( dei_ss_trg_history_timeout );

	if( !dei_ss_alarm_hist_report_on )
	{
		if( !get_ss_arm()|| get_ss_valet() )
		{
			//ss.door_triggered  = FALSE ; 
			set_ss_door_trigger  ( FALSE );
			
			//ss.hood_triggered  = FALSE ;
			set_ss_hood_trigger  ( FALSE );
			
			//ss.trunk_triggered = FALSE ;
			set_ss_trunk_trigger ( FALSE );
			
			//ss.ign_triggered   = FALSE ;
			set_ss_ign_trigger   ( FALSE );
			
			//ss.sensor_triggered= FALSE ;
			set_ss_sensor_trigger_all( FALSE );
			
			set_ss_sensor_trigger( FALSE );

			set_ss_last_trigger( FALSE );

	#ifdef PLATFORM_CM800
			set_ss_sensor_npc    ( FALSE );
			sensor_reset( ss_sensor );
	#else
			for(i = 0; i < 3; i++)
			{
				set_trig_sensor_index(i);
				set_ss_sensor_npc    ( FALSE );
			}
			
			sensor_reset( ss_sensor[0] );
			sensor_reset( ss_sensor[1] );
			sensor_reset( ss_sensor[2] );
			if(ext_sensor_trigger_configured)
				sensor_reset(ss_sensor_ext);
	#endif

		}
	dei_ss_alarm_hist_report_on = FALSE;
	}
}

//--------------------------------------------------------------------------//

void dei_ss_ignition_off( void )
{
	if( get_ss_valet() )
		dei_led_flash ( DEI_LED_VALET_ON , LED_ENABLE);


}

//void dei_ss_brake_press (void)
//{			
//	if( get_ss_arm() && !get_ss_valet())
//	{
//		// If arm and brake press, trigger alarm using the ignition trigger zone
//		dei_ss_trigger_start( DEI_SS_TRIGGER_ZONE_IGN ) ; 
//	}    
//}	
