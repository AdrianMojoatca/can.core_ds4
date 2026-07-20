/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_sensor_adjust.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ss_private.h"

#ifdef PLATFORM_933		// (BUILD_NGMM == 1)
	#include "sensor_proto.h"
	#include "aid_dhp.h"
	#include "status_system.h"
	#include "dei_sc.h"
#endif
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void chirp_notify_send(Notify notify_cmd, UInt8 new_config);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_ss_sensor_adjust_enter( void *remote )
{
	
#ifdef PLATFORM_933	// (BUILD_NGMM == 1)
	UInt8 evt_state = 0;
	UInt8 pld[2] = {0x0E, 0};		// intialized for Testmode and alarm bit set sensor device
	
  if((ss_system_type & SYS_TYPE_SS) != SYS_TYPE_SS)
    return;
	sensors_status_invalidate();
 
  sensadj_entertest();

  sensadj_get_current_idchan(&dei_ss_curr_sensor_aid,&dei_ss_curr_sensor_chanl);
	
	if( ! get_ss_arm() && !get_rs_gwr() && !get_ss_valet() && !get_ign_status())
	{
		// stop passive arming timer
		ngmm_ss_sys_arming_disable();
		pld[1] = sensadj_get_current_index();
		// flash led
		dei_led_flash(DEI_LED_SENSOR1_ADJUST <<(pld[1]-1) , TRUE);
		// siren chirp
		dei_siren_chirp( DEI_SIREN_SENSOR_ADJUST_ENTER ); 				// to give long siren pulse for entering Sensor Adjust mode
	
		// all we can do is ask for factory bits so we can ask for testmode later		
		send_identify_request();
		
		// wait for receieve response event top happen
		evt_state = os_evt_wait( sensor_proto_evt, SENSOR_PROTO_CONFIG_RECV, SENSOR_PROTO_CONFIG_RECV,	1000);
		
		if(evt_state & SENSOR_PROTO_CONFIG_RECV)
		{
			// send notify NOTIFY_SENSOR1_ADJUST
			notify_send( NOTIFY_SENSOR1_ADJUST, get_shock_sensor_level());
		}
		else
		{
			aid_db[dei_ss_curr_sensor_idx].status &= ~(1<<AID_STATUS_BIT_CONNECTED);
			// there is no sensor and main module send the notification that 
			// sensor is disabled so that both main module and HHU stays in SENSOR ADJUST mode
			dei_siren_chirp( DEI_SIREN_SENSOR_PROGRAMMING_EXIT ); 	// to give long siren pulse for unsuccessful Sensor Adjust mode enter

			notify_send( NOTIFY_SENSOR1_ADJUST, 0 );		
		}		
		
	}
	else
	{// can't do this now
		sensadj_exittest();
		
		dei_sc_mode = DEI_SC_MODE_NORMAL;
		vrtc_stop( dei_sc_sensor_adjust_mode_vrtc );
		notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_FEATURE_NA ) ;
		
	}
#endif
}

#if 0
void dei_ss_sensor_identified(UInt8 factory_bits)
{
#ifdef PLATFORM_933	// (BUILD_NGMM == 1)	
	UInt8 pld[2];
	
	if(dei_ss_sensor_factory_bits != factory_bits) // first response since we asked?
	{
    dei_ss_sensor_factory_bits = factory_bits; // save now
		pld[0] = factory_bits | TEST_MASK; // set test mode using those bits
		pld[1] = sensadj_get_current_index();
		sensor_proto_tx(PLD_E1, dei_ss_curr_sensor_aid, dei_ss_curr_sensor_chanl , pld, 2 );
			// send request for getting the present sensor level
		
			sensor_proto_tx(PLD_E8, dei_ss_curr_sensor_aid, dei_ss_curr_sensor_chanl , 0, 0 );

			// wait for receieve response event top happen
			evt_state = os_evt_wait( sensor_proto_evt, SENSOR_PROTO_CONFIG_RECV, SENSOR_PROTO_CONFIG_RECV,	1000);
	}
	
	//FIXME:		if ( dei_ss_curr_sensor_aid )
	//FIXME:		{
			
		
		if(evt_state & SENSOR_PROTO_CONFIG_RECV)
		{
			// send notify NOTIFY_SENSOR1_ADJUST
			notify_send( NOTIFY_SENSOR1_ADJUST, get_shock_sensor_level());
		}
		else
		{
			aid_db[dei_ss_curr_sensor_idx].status &= ~(1<<AID_STATUS_BIT_CONNECTED);
			// there is no sensor and main module send the notification that 
			// sensor is disabled so that both main module and HHU stays in SENSOR ADJUST mode
			dei_siren_chirp( DEI_SIREN_SENSOR_PROGRAMMING_EXIT ); 	// to give long siren pulse for unsuccessful Sensor Adjust mode enter

			notify_send( NOTIFY_SENSOR1_ADJUST, 0 );		
		}

		
#else
	 notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_FEATURE_NA ) ;
#endif
}
#endif


//--------------------------------------------------------------------------//

void dei_ss_sensor_adjust_put( void *remote )
{
#ifdef PLATFORM_933		// (BUILD_NGMM  == 1)
	UInt8 new_config[1];
	UInt8 	evt_state = 0;
	
	#define  EVT  (SENSOR_PROTO_WRITE_CONFIG | SENSOR_PROTO_CONFIG_RECV)

	if( dei_ss_curr_sensor_aid )
	{
		new_config[0] = ((Cmd_Rf_Pld*)(cmd_get_pld(remote)))->bits.pld;

		if(dei_ss_sensor_adjust_src == DEI_SS_LED_REMOTE_DEFAULT)
		{
		sensor_proto_tx(PLD_C1, dei_ss_curr_sensor_aid, dei_ss_curr_sensor_chanl, 0, 0 );

		sensor_proto_tx(PLD_E2, dei_ss_curr_sensor_aid, dei_ss_curr_sensor_chanl, 0, 0 );
		}
		else
		{
		// send request for setting new sensor level
		sensor_proto_tx(PLD_E9, dei_ss_curr_sensor_aid, dei_ss_curr_sensor_chanl, new_config, 1 );
		}
		
		// wait for receieve response event top happen
		evt_state = os_evt_wait( sensor_proto_evt, EVT, EVT,	500);
	}
	
	if(evt_state & EVT)
	{
		new_config[0] = get_shock_sensor_level();
		
		if( (dei_ss_sensor_adjust_src == DEI_SS_LCD_REMOTE) || (dei_ss_sensor_adjust_src == DEI_SS_LED_REMOTE_DEFAULT) )
			chirp_notify_send(NOTIFY_SENSOR1_LEVEL_UPDATE, new_config[0]);
		
		else if(dei_ss_sensor_adjust_src == DEI_SS_LED_REMOTE_INCREASE)
			chirp_notify_send(NOTIFY_SENSOR1_LEVEL_INCREASE, new_config[0]);
		
		else if(dei_ss_sensor_adjust_src == DEI_SS_LED_REMOTE_DECREASE)
			chirp_notify_send(NOTIFY_SENSOR1_LEVEL_DECREASE, new_config[0]);
			
	}
	//else if( evt_state & SENSOR_PROTO_CONFIG_RECV) // acknowledgement for reset to defualt setting
	//{
	//		notify_send(NOTIFY_SENSOR1_LEVEL_UPDATE, get_shock_sensor_level());
	//}
#else
	 notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_FEATURE_NA ) ; 
#endif
}


//--------------------------------------------------------------------------//


void dei_ss_sensor_adjust_exit( void )
{
#ifdef PLATFORM_933		// (BUILD_NGMM == 1)
// 	UInt8 pld[1] = {0};		// intialized for Testmode and alarm bit set sensor device
//	
//	pld[0] = get_shock_sensor_alert_bits();

	
	sensadj_exit_actions();
	
//	sensor_proto_tx(PLD_E1, dei_ss_curr_sensor_aid, dei_ss_curr_sensor_chanl , pld, 1 );
	
	sensors_status_invalidate();

	vrtc_stop(dei_sc_sensor_adjust_mode_vrtc);

	dei_siren_chirp( DEI_SIREN_SENSOR_PROGRAMMING_EXIT ); 				// to give long siren pulse for exit Sensor Adjust mode

	notify_send( NOTIFY_SENSOR_PROG_MODE_EXIT );
	
	wake_leave_flagged(WAKE_DEI_SS_SENS_ADJ);		// sensor adjust mode exit
#else
	 notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_FEATURE_NA ) ; 
#endif
}

//--------------------------------------------------------------------------//

#ifdef PLATFORM_933

void chirp_notify_send(Notify notify_cmd, UInt8 new_config)
{
	if( dei_ss_sensor_adjust_src != DEI_SS_LCD_REMOTE )
	{
		if(new_config == 0)
			dei_siren_chirp( DEI_SIREN_SENSOR_MIN_LIMIT);
		else if( new_config == 0x0F)
			dei_siren_chirp( DEI_SIREN_SENSOR_MAX_LIMIT);
		else if(dei_ss_sensor_adjust_src == DEI_SS_LED_REMOTE_INCREASE)
			dei_siren_chirp( DEI_SIREN_SENSOR_LEVEL_UP);
		else if(dei_ss_sensor_adjust_src == DEI_SS_LED_REMOTE_DECREASE)
			dei_siren_chirp( DEI_SIREN_SENSOR_LEVEL_DOWN);
		else if(dei_ss_sensor_adjust_src == DEI_SS_LED_REMOTE_DEFAULT)
			dei_siren_chirp( DEI_SIREN_SENSOR_DEFAULT_SETTING);			// siren/horn pattern for sensor adjust to default value
	}
	notify_send(notify_cmd, new_config);
}


//--------------------------------------------------------------------------//

// installer/user wants to skip ahead to a different sensor channel
void dei_ss_sensor_num_to_adjust(UInt32 tap_cnt)
{
  UInt8 id,chan;
	UInt8 evt_state;
	UInt8 pld[2];
	
	sensadj_get_current_idchan(&id,&chan);
	if(id==0)
		return; // not adjusting anything
	
	// first we say goodbye to this sensor chsnnel
	pld[0] = 0x0E; //use 0x0E when enterring sensor adjustment (was sensor_proto_get_factory_bits();)
	pld[1] = 0; // and stop flashing
	sensor_proto_tx(PLD_E1, id, chan , pld, 2 );	
	
	sensadj_advance(tap_cnt);  // advance to next device
	
	pld[1] = sensadj_get_current_index();

	sensadj_get_current_idchan(&dei_ss_curr_sensor_aid,&dei_ss_curr_sensor_chanl);

	// flash led
	dei_led_flash(DEI_LED_SENSOR1_ADJUST <<(pld[1]-1) , TRUE);	
	
	//send_identify_request();	

	if( pld[1] && dei_ss_curr_sensor_aid )
	{
		dei_siren_chirp( DEI_SIREN_SENSOR_ADJUST_ENTER ); 				
		// put next adjustable sensor in testmode
		sensor_proto_tx(PLD_E1, dei_ss_curr_sensor_aid, dei_ss_curr_sensor_chanl , pld, 2 );
		
		// send request for getting the present sensor level
		
		sensor_proto_tx(PLD_E8, dei_ss_curr_sensor_aid, dei_ss_curr_sensor_chanl , 0, 0 );
		
		// wait for receieve response event top happen
		evt_state = os_evt_wait( sensor_proto_evt, SENSOR_PROTO_CONFIG_RECV, SENSOR_PROTO_CONFIG_RECV,	1000);
		
		if(evt_state & SENSOR_PROTO_CONFIG_RECV)
		{
			// send notify NOTIFY_SENSOR1_ADJUST
			notify_send( NOTIFY_SENSOR1_ADJUST, get_shock_sensor_level());
		}
		else
		{
				dei_siren_chirp( DEI_SIREN_SENSOR_PROGRAMMING_EXIT ); 				// to give long siren pulse for unsuccessful Sensor Adjust mode enter

				notify_send( NOTIFY_SENSOR1_ADJUST, 0);
		}
	}
	else
	{
			dei_siren_chirp( DEI_SIREN_SENSOR_PROGRAMMING_EXIT ); 				// to give long siren pulse for unsuccessful Sensor Adjust mode enter

			notify_send( NOTIFY_SENSOR1_ADJUST, 0);
	}	
}

// factory bits from sensor under test
void dei_ss_sensor_identified( UInt8 factory_bits )
{
	UInt32 i;
	UInt8 evt_state;
  UInt8 id,chan;
	UInt8 pld[2];


	sensadj_get_current_idchan(&id,&chan); // which one is it?
  if(id)
  {
		sensor_proto_set_factory_bits(factory_bits); // remember this for when we say goodbye
		pld[0] = factory_bits | TEST_MASK; // set test mode
		pld[1] = sensadj_get_current_index();
		
	}		
		
	
	if( dei_sc_mode == DEI_SC_MODE_SENSOR_ADJUST )
	{		
		
		// flash led
		dei_led_flash((DEI_LED_SENSOR1_ADJUST <<(pld[1]-1)) , TRUE);

		//tap_cnt = 0;

		if( pld[1] && dei_ss_curr_sensor_aid )
		{
			dei_siren_chirp( DEI_SIREN_SENSOR_ADJUST_ENTER ); 				
			// put next adjustable sensor in testmode
			sensor_proto_tx(PLD_E1, dei_ss_curr_sensor_aid, dei_ss_curr_sensor_chanl , pld, 2 );
			
			// send request for getting the present sensor level
			
			sensor_proto_tx(PLD_E8, dei_ss_curr_sensor_aid, dei_ss_curr_sensor_chanl , 0, 0 );
			
			// wait for receieve response event top happen
			evt_state = os_evt_wait( sensor_proto_evt, SENSOR_PROTO_CONFIG_RECV, SENSOR_PROTO_CONFIG_RECV,	1000);
			
			if(evt_state & SENSOR_PROTO_CONFIG_RECV)
			{
				// send notify NOTIFY_SENSOR1_ADJUST
				notify_send( NOTIFY_SENSOR1_ADJUST, get_shock_sensor_level());
			}
			else
			{
					dei_siren_chirp( DEI_SIREN_SENSOR_PROGRAMMING_EXIT ); 				// to give long siren pulse for unsuccessful Sensor Adjust mode enter

					notify_send( NOTIFY_SENSOR1_ADJUST, 0);
			}
		}
		else
		{
				dei_siren_chirp( DEI_SIREN_SENSOR_PROGRAMMING_EXIT ); 				// to give long siren pulse for unsuccessful Sensor Adjust mode enter

				notify_send( NOTIFY_SENSOR1_ADJUST, 0);
		}

	}
}

//--------------------------------------------------------------------------//

void dei_ss_sensor_mode_exit_ign_on( void )
{
 	UInt8 pld[1] = {0};		// intialized for Testmode and alarm bit set sensor device
	
	if( dei_sc_mode == DEI_SC_MODE_SENSOR_ADJUST )
	{
		pld[0] = get_shock_sensor_alert_bits();

		sensor_proto_tx(PLD_E1, dei_ss_curr_sensor_aid, dei_ss_curr_sensor_chanl , pld, 1 );
		sensors_status_invalidate();

		dei_sc_mode = DEI_SC_MODE_NORMAL;
		vrtc_stop( dei_sc_sensor_adjust_mode_vrtc );

		dei_siren_chirp( DEI_SIREN_SENSOR_PROGRAMMING_EXIT ); 				// to give long siren pulse for exit Sensor Adjust mode

		notify_send( NOTIFY_SENSOR_PROG_MODE_EXIT );
		
		wake_leave_flagged(WAKE_DEI_SS_SENS_ADJ);		// sensor adjust mode exit
	}

}

//--------------------------------------------------------------------------//

void dei_ss_sensor_tap1_count( void )
{
	UInt32 tap_cnt = 1;
	dei_ss_sensor_num_to_adjust( tap_cnt );
}
//--------------------------------------------------------------------------//

void dei_ss_sensor_tap2_count( void )
{
	UInt32 tap_cnt = 2;
	dei_ss_sensor_num_to_adjust( tap_cnt );
}
//--------------------------------------------------------------------------//

void dei_ss_sensor_tap3_count( void )
{
	UInt32 tap_cnt = 3;
	dei_ss_sensor_num_to_adjust( tap_cnt );
}
//--------------------------------------------------------------------------//

void dei_ss_sensor_tap4_count( void )
{
	UInt32 tap_cnt = 4;
	dei_ss_sensor_num_to_adjust( tap_cnt );
}
//--------------------------------------------------------------------------//

void dei_ss_sensor_tap5_count( void )
{
	UInt32 tap_cnt = 5;

	dei_ss_sensor_num_to_adjust( tap_cnt );
}
//--------------------------------------------------------------------------//


UInt8 dei_ss_sensor_curr_aid( void )
{
	return dei_ss_curr_sensor_aid;
}

//--------------------------------------------------------------------------//

UInt8 dei_ss_sensor_curr_chanl( void )
{
	return dei_ss_curr_sensor_chanl;
}

//--------------------------------------------------------------------------//

void dei_ss_sensor_curr_aid_reset( void )
{
	dei_ss_curr_sensor_aid =0;
}

//--------------------------------------------------------------------------//

void dei_ss_sensor_curr_chanl_reset( void )
{
	dei_ss_curr_sensor_chanl = 0;
}

#endif
