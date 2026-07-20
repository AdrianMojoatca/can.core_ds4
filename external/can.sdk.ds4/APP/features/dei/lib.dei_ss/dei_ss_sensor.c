/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_sensor.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ss_private.h"

#ifdef PLATFORM_933		// (BUILD_NGMM == 1)
	#include "sensor_proto.h"
	#include "sense_rs.h"
#endif

#include "sensor_private.h"
#include "npc_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define SENSE_EXT_TRIGGER_CMD_PLD(x) ((UInt32)(*cmd_get_pld(x)))
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

void dei_ss_sensor_bp_rs_done( void )
{
#ifdef PLATFORM_CM800
    sensor_bypass( ss_sensor , DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_RS_DONE ); //10 seconds
#else
	
	// bypass all sensors while RS sequence for 10 sec 
    sensor_bypass( ss_sensor[0] , DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_RS_DONE ); //10 seconds
    sensor_bypass( ss_sensor[1] , DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_RS_DONE ); //10 seconds
    sensor_bypass( ss_sensor[2] , DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_RS_DONE ); //10 seconds
	  if(ext_sensor_trigger_configured)
		{
			sensor_bypass( ss_sensor_ext , DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_RS_DONE);			
		}
	
#endif

}		

//--------------------------------------------------------------------------//

void dei_ss_sensor_warning( void *pld )
{	
	static const UInt32 warning_source[] = 
	{
		 NOTIFY_PLD_TRIG_S1, NOTIFY_PLD_TRIG_S2, NOTIFY_PLD_TRIG_S3
	};
#ifdef PLATFORM_933		// (BUILD_NGMM == 1)

		UInt8 evt;
	
		evt = os_evt_wait( sensor_proto_evt, SENSOR_PROTO_ALRM_SRC, SENSOR_PROTO_ALRM_SRC,	500);

	
	if(evt & SENSOR_PROTO_ALRM_SRC)
		{
	//		set_ss_trig_src(DEI_SS_SENSOR_TRIGGER_SHOCK);
		}
		else
		{
		//	set_ss_trig_src(DEI_SS_SENSOR_TRIGGER_SHOCK);
			//set_ss_trig_src(DEI_SS_SENSOR_TRIGGER_TILT);
			set_ss_warn_src(DEI_SS_SENSOR_WARN_UNKNOWN);
			set_trig_sensor_index(0);
		}


	#define WARNING_ALLOWED ( get_ss_arm()      && !get_ss_valet()    && !get_rs_gwr()                    &&\
	                         !get_ss_panic()    && !get_ss_trigger()  && !get_ss_trunk_release()          &&\
						      sensor_status_warning_allowed(ss_sensor[get_trig_sensor_index()])&& !sensor_status_bypass( ss_sensor[get_trig_sensor_index()]))
#else
		
	#define WARNING_ALLOWED ( get_ss_arm()      && !get_ss_valet()    && !get_rs_gwr()                    &&\
	                         !get_ss_panic()    && !get_ss_trigger()  && !get_ss_trunk_release()        &&\
						      sensor_status_warning_allowed(ss_sensor)&& !sensor_status_bypass( ss_sensor))
#endif
	
	if( WARNING_ALLOWED && dei_ss_trigger_enable )
	{		
        if( (dei_feature_get(DEI_FEAT_CONFIRMATION_CHIRPS) != 2) && (dei_feature_get(DEI_FEAT_CONFIRMATION_CHIRPS) != 4))
		{
		  //if( !ss.full_silent_arm       && !ss.sensor_silent_arm       )
			if( !get_ss_full_silent_arm() && !get_ss_sensor_silent_arm() )
            {
                dei_siren_chirp( DEI_SIREN_WARNING ) ; 
            }

			notify_send( NOTIFY_WARNING  , warning_source[get_ss_warn_src()-1] );
  
#ifdef PLATFORM_CM800						
        	if( sensor_warning( ss_sensor ) )
#else
					cmd_q_rs_insert( INPUT_WARNING_ON  , 100 );
					
        	if( sensor_warning( ss_sensor[get_trig_sensor_index()] ) )
#endif
			{
			  //if( !ss.full_silent_arm       && !ss.sensor_silent_arm       )
				if( !get_ss_full_silent_arm() && !get_ss_sensor_silent_arm() )
            	{
                	dei_prk_flash  ( DEI_PRK_WARNING ); 
            	}            
     	}
		}		
	}

	#undef WARNING_ALLOWED
	
}	

//--------------------------------------------------------------------------//
void dei_ss_sensor_trigger( void *pld)
{	
#ifdef PLATFORM_933		// (BUILD_NGMM == 1)
	UInt8 evt_state, index;

		if((SENSE_EXT_TRIGGER_CMD_PLD(pld) & CMD_EXT_TRIGGER)
       && ext_sensor_trigger_configured)
		{
        evt_state = 0;
			  set_ss_trig_src(DEI_SS_SENSOR_TRIGGER_UNKNOWN);
			  set_trig_sensor_index(0); //ZONE_SENSOR UNKNOWN
			  if( get_ss_arm()      && !get_ss_valet()     && !get_rs_gwr()                  &&\
						!get_ss_panic()    && !get_ss_trigger()   && !get_ss_trunk_release() 			 &&\
				    sensor_status_trigger_allowed(ss_sensor_ext)                               &&\
				    !sensor_status_bypass( ss_sensor_ext )                                     &&\
				    dei_ss_trigger_enable
						)
				{
					if(sensor_trigger(ss_sensor_ext))
					{
  					set_ss_sensor_trigger( TRUE );
	  				dei_ss_trigger_start( DEI_SS_TRIGGER_ZONE_SENSOR3 );    //ZONE_SENSOR UNKNOWN
					}
				}
				set_ss_sensor_npc( sensor_status_npc( ss_sensor_ext) );
		}
		else
		{
			evt_state = os_evt_wait( sensor_proto_evt, SENSOR_PROTO_ALRM_SRC, SENSOR_PROTO_ALRM_SRC,	500);
		
		if(evt_state & SENSOR_PROTO_ALRM_SRC)
			{
			}
			else
			{
				set_ss_trig_src(DEI_SS_SENSOR_TRIGGER_UNKNOWN);
				set_trig_sensor_index(0);
			}

		#define TRIGGER_ALLOWED ( get_ss_arm()      && !get_ss_valet()     && !get_rs_gwr()                   &&\
															 !get_ss_panic()    && !get_ss_trigger()   && !get_ss_trunk_release()  				&&\
													sensor_status_trigger_allowed(ss_sensor[get_trig_sensor_index()]) && !sensor_status_bypass( ss_sensor[get_trig_sensor_index()]))       

				if( TRIGGER_ALLOWED && dei_ss_trigger_enable )
			{
				if(sensor_trigger( ss_sensor[get_trig_sensor_index()] ) )
				{
					//ss.sensor_triggered = TRUE ; 
					set_ss_sensor_trigger( TRUE );
					if( get_trig_sensor_index() == 1)
						dei_ss_trigger_start( DEI_SS_TRIGGER_ZONE_SENSOR );             
					if( get_trig_sensor_index() == 2 )
						dei_ss_trigger_start( DEI_SS_TRIGGER_ZONE_SENSOR2 );             
					if( get_trig_sensor_index() == 0 )
						dei_ss_trigger_start( DEI_SS_TRIGGER_ZONE_SENSOR3 );             
				}			
        else
				{
					sensor_enable( ss_sensor[get_trig_sensor_index()] );
				}
			}

				set_ss_sensor_npc( sensor_status_npc( ss_sensor[get_trig_sensor_index()] ) );
		}
#else
		
		#define TRIGGER_ALLOWED ( get_ss_arm()      && !get_ss_valet()     && !get_rs_gwr()                   &&\
												 !get_ss_panic()    && !get_ss_trigger()   && !get_ss_trunk_release()  				&&\
										sensor_status_trigger_allowed(ss_sensor) && !sensor_status_bypass( ss_sensor ))       

		if( TRIGGER_ALLOWED && dei_ss_trigger_enable )
			{
				if(sensor_trigger( ss_sensor ) )
				{
					//ss.sensor_triggered = TRUE ; 
					set_ss_sensor_trigger( TRUE );

					dei_ss_trigger_start( DEI_SS_TRIGGER_ZONE_SENSOR );             
				}		        
			}

				set_ss_sensor_npc( sensor_status_npc( ss_sensor ) );
#endif
			

			#undef TRIGGER_ALLOWED
}

//--------------------------------------------------------------------------//

