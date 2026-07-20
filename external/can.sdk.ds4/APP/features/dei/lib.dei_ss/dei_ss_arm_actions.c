/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_arm_actions.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ss_private.h"
#ifdef PLATFORM_933
  #include "sense_rs.h"
	#include "aid_dhp.h"
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

void dei_ss_arm_actions( Dei_Ss_Silent silent , void *remote)
{
	UInt8 cc; // Confirmation chirps feature
#ifdef PLATFORM_933
	sensadj_exit_actions();
#endif
	
	cc=dei_feature_get(DEI_FEAT_CONFIRMATION_CHIRPS);
	
	if(!get_rs_gwr()) {
		set_ss_full_silent_arm( FALSE );
		set_ss_sensor_silent_arm( FALSE );
		}
		
	if( get_ss_arm() )
		dei_ss_2nd_arm = TRUE;
	else
		dei_ss_2nd_arm = FALSE;

	dei_ss_2nd_disarm = FALSE ; 					
	
	//if( dei_ss_trigger_enable ) 
  //else 
  //  set_ss_arm(TRUE);

  dei_ss_sensor_bp      = DEI_SS_SENSOR_BYPASS_OFF ;
	dei_ss_siren_low_batt_stop();	

	if(get_ss_valet())
	{
		dei_led_flash(DEI_LED_VALET_ON,LED_ENABLE ) ; 
		return;		
	}
	
	// really arming...
	set_ss_arm( TRUE );
	prg_out_ground_when_arm(TRUE);
	
	
  dei_led_flash ( DEI_LED_ARM , LED_ENABLE ) ; 

  	
	// Siren chirp
	do  // THIS IS NOT A LOOP!
	{		
#ifdef PLATFORM_933
	#define SECOND_SILENT_MODE	( (silent == DEI_SS_SILENT) || (dei_feature_get(DEI_FEAT_CONFIRMATION_CHIRPS) == 3) || (dei_feature_get(DEI_FEAT_CONFIRMATION_CHIRPS) == 4)  || (!dei_ss_2nd_arm ))
		if( !SECOND_SILENT_MODE && !get_ss_valet() )
		{
			dei_siren_chirp( DEI_SIREN_SECOND_ARM );
      break;			
		}
	#undef SECOND_SILENT_MODE 	
#endif		
		
		if( ((silent!=DEI_SS_SILENT) && (cc!=3) && (cc!=4) )
			|| (dei_ss_arm_cmd==DEI_SS_SENSOR_SILENT_ARM)
			)
			{
				dei_siren_chirp( DEI_SIREN_ARM ); 				
			}
		
	  break; // this break is unconditional
	} while(0); // ... NOT A LOOP
	
  //if( !ss.valet       )
	//if( !get_ss_valet() )
	{
	  //sensor_bypass( ss_sensor , !ss.one_time_bypass       && (dei_feature_get( DEI_FEAT_COMFORT_CLOSURE ) != 1)? DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_ARM_COMFORT : DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_ARM  );			
		
#ifdef PLATFORM_CM800		
	    sensor_bypass( ss_sensor , !get_ss_one_time_bypass() && (dei_feature_get( DEI_FEAT_COMFORT_CLOSURE ) != 1)? DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_ARM_COMFORT : DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_ARM  );			
		
			sensor_enable( ss_sensor );
#else
	    sensor_bypass( ss_sensor[0] , !get_ss_one_time_bypass() && (dei_feature_get( DEI_FEAT_COMFORT_CLOSURE ) != 1)? DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_ARM_COMFORT : DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_ARM  );			
	    sensor_bypass( ss_sensor[1] , !get_ss_one_time_bypass() && (dei_feature_get( DEI_FEAT_COMFORT_CLOSURE ) != 1)? DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_ARM_COMFORT : DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_ARM  );			
	    sensor_bypass( ss_sensor[2] , !get_ss_one_time_bypass() && (dei_feature_get( DEI_FEAT_COMFORT_CLOSURE ) != 1)? DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_ARM_COMFORT : DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_ARM  );			

			if(!get_rs_start_successful()) {
				sensor_enable( ss_sensor[0] );
				sensor_enable( ss_sensor[1] );
				sensor_enable( ss_sensor[2] );
				}
			
		  if(ext_sensor_trigger_configured)
			{
				sensor_bypass( ss_sensor_ext , !get_ss_one_time_bypass() && (dei_feature_get( DEI_FEAT_COMFORT_CLOSURE ) != 1)? DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_ARM_COMFORT : DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_ARM  );			
		    sensor_enable(ss_sensor_ext);		
			}
#endif
	}	
}

//--------------------------------------------------------------------------//
