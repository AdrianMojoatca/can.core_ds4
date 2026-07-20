/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_disarm_actions.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ss_private.h"
#ifdef PLATFORM_933
#include "dei_sc.h"       //dei_sc_extended_page_update
#include "sense_rs.h"
#endif
#include "prg_out.h"
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
#ifdef PLATFORM_933
Dei_Led_Pattern dei_ss_led_alarm_last_trig(void)
{
Dei_Led_Pattern x;

	  switch(get_ss_last_trigger())
		{
		case DEI_SS_TRIGGER_ZONE_DOOR:
      return DEI_LED_DOOR_TRIGGERED;
    case DEI_SS_TRIGGER_ZONE_HOOD:
      return DEI_LED_HOOD_TRIGGERED;
    case DEI_SS_TRIGGER_ZONE_TRUNK:
      return DEI_LED_TRUNK_TRIGGERED;
		case DEI_SS_TRIGGER_ZONE_IGN:
      return DEI_LED_IGN_TRIGGERED;
    case DEI_SS_TRIGGER_ZONE_SENSOR:
      return DEI_LED_SHOCK_SENSOR_TRIGGERED;
    case DEI_SS_TRIGGER_ZONE_SENSOR2:
			return DEI_LED_TILT_SENSOR_TRIGGERED;
    case DEI_SS_TRIGGER_ZONE_SENSOR3:
			return DEI_LED_UNKNOWN_SENSOR_TRIGGERED;
    default:
      break;			
		}
	return DEI_LED_NONE;
}
#endif

void dei_ss_disarm_actions( Dei_Ss_Silent silent , void *remote )
{
#ifdef DEV_LPC_176x
   Dei_Led_Pattern led_disarm_hist;
#endif	
	//ss.one_time_bypass = FALSE ;
	set_ss_one_time_bypass( FALSE );
	set_ss_full_silent_arm( FALSE )  ;
	
	
	set_ss_sensor_silent_arm( FALSE );
		//set_ss_arm ( FALSE );
		//set_ss_lock( FALSE );
		dei_ss_2nd_arm = FALSE;
	//if( ss.valet )
	if( get_ss_valet() )
	{
	  //ss.armed = FALSE ;
		set_ss_arm ( FALSE );
		set_ss_lock( FALSE );

	  dei_led_flash  ( DEI_LED_VALET_ON , LED_ENABLE);		

		return  ;
	}

	os_tsk_wait(40);   //differ the LED pattern with the RF feedback to IVU to make sure IVU to receive LED pattern
	                   //TODO optimize with 1 thread
	if( DEI_SS_LED_ALARM_LAST_TRIG == DEI_SS_NO_TRIGGER || get_ign_status() ) {
#ifdef PLATFORM_933
		if(dei_feature_get( DEI_FEAT_AUTO_REARM ) == 1) 
#endif
			dei_led_flash ( DEI_LED_DISARM , LED_ENABLE);
	}
	else
	{
		dei_led_flash ( DEI_SS_LED_ALARM_LAST_TRIG , LED_ENABLE);
#ifdef PLATFORM_933  
		dei_sc_extended_page_update( TRUE );
#endif  
  }
	

  //if( ss.armed )
/*  	if( get_ss_arm() || get_ss_lock() )
	{				
		dei_ss_2nd_disarm = FALSE ; 					
	}
*/		
#ifdef PLATFORM_CM800	
	#define SILENT_MODE	((silent == DEI_SS_SILENT ) || (dei_feature_get(DEI_FEAT_CONFIRMATION_CHIRPS) == 3) || (dei_feature_get(DEI_FEAT_CONFIRMATION_CHIRPS) == 4))
#else
	#define SILENT_MODE	(((silent == DEI_SS_SILENT) || (silent == DEI_SS_SILENT_LOGIC_RESUME)) || (dei_feature_get(DEI_FEAT_CONFIRMATION_CHIRPS) == 3) || (dei_feature_get(DEI_FEAT_CONFIRMATION_CHIRPS) == 4))
#endif
		
	if( !dei_ss_2nd_disarm )
	{				
		dei_ss_siren_low_batt_check( remote );
		
		if( !SILENT_MODE )
		{
		  //if( ss.sensor_npc )
			if( get_ss_sensor_npc(1) || get_ss_sensor_npc(2) || get_ss_sensor_npc(0) )
			{
				dei_siren_chirp( DEI_SIREN_DISARM_NPC_ON ) ;					
				dei_siren_send_zone_npc();
			}
			else
			{
			  //if( ss.door_triggered    || ss.hood_triggered     || ss.ign_triggered     || ss.trunk_triggered     || ss.sensor_triggered   )
				if( get_ss_door_trigger()|| get_ss_hood_trigger() || get_ss_ign_trigger() || get_ss_trunk_trigger() || get_ss_hood_trigger() || get_ss_sensor_trigger(1) || get_ss_sensor_trigger(2) || get_ss_sensor_trigger(0) )
				{
					dei_siren_chirp( DEI_SIREN_DISARM_TRIGGER_ON ) ; 			
					dei_siren_send_zone_trigger();
				}								
				else
				{
					dei_siren_chirp( DEI_SIREN_DISARM_NO_EVENT );				
				}
			}
		}
	}
	
	#undef SILENT_MODE 

#ifdef PLATFORM_CM800
	if( dei_feature_get( DEI_FEAT_2ND_UNLOCK ) != DEI_2ND_UNLOCK_OFF )
	{
		dei_ss_2nd_disarm = !dei_ss_2nd_disarm ; 
	}
#else
	if( (dei_feature_get( DEI_FEAT_DRIVER_DOOR_UNLOCK_PRIORITY)==1) && (silent != DEI_SS_SILENT_LOGIC_RESUME) )
	{
		dei_ss_2nd_disarm = !dei_ss_2nd_disarm ; 
	}
#endif
		
	//ss.armed = FALSE ;					
	set_ss_arm ( FALSE );
	
	prg_out_ground_when_arm(FALSE);
	
//	set_ss_lock( FALSE );
    		
#ifdef PLATFORM_CM800
	sensor_disable( ss_sensor );
#else
	sensor_disable( ss_sensor[0] );
	sensor_disable( ss_sensor[1] );
	sensor_disable( ss_sensor[2] );
  if(ext_sensor_trigger_configured)
    sensor_disable(ss_sensor_ext);
#endif
}	

//--------------------------------------------------------------------------//

