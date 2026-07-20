/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_rf_start_stop.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rs_private.h"
#include "timeout_f.h"
#include "sense_rs.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Dei_Rs_Rf_Start_Stop
{
    UInt8 press_count;
    Vrtc  vrtc  ;
}Dei_Rs_Rf_Start_Stop;
    
#define AS_EXTENDED_RUNTIME_CMD_TIMEOUT		VRTC_1SEC*4

#define VSS_ENABLED  (dei_feature_get( DEI_FEAT_TACHOMETER_INPUT ) == 2 \
                      || dei_feature_get(DEI_FEAT_ENGINE_SENSING) == 1)

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_rs_rf_start_stop_flash_error      ( Dei_Rs_Evt err_start );
static void dei_rs_rf_start_stop_first_press_reset( void ) ; 
static void as_rs_notify_timeout( void ) ;
static void as_rs_extend_runtime_timeout( void );

static void rs_rf_start_speed_check(void);

static void rs_rf_restart_check(void);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
Dei_Rs_Rf_Start_Stop dei_rs_rf_start_stop_vars;
Vrtc as_rs_notify_vrtc ;
Vrtc as_rs_extend_runtime_vrtc;
Boolean as_rs_runtime_cmd_received;
Timeout rs_rf_start_speed_check_timer;
UInt8 sense_rs_pulse_count=0;

static  UInt8 rs_rf_restart_wait2safe = 0;
Timeout rs_rf_restart_guard_timer;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_rs_rf_start_stop_init( void )
{
	dei_rs_rf_start_stop_vars.vrtc = vrtc_f_create( (Exec_Func)dei_rs_rf_start_stop_first_press_reset );
	
	as_rs_notify_vrtc = vrtc_f_create( (Exec_Func) as_rs_notify_timeout );
	as_rs_extend_runtime_vrtc = vrtc_f_create( (Exec_Func) as_rs_extend_runtime_timeout );
	dei_rs_rf_start_stop_vars.press_count = 0 ;
	
	rs_rf_start_speed_check_timer = timeout_f_create(500, (Exec_Func)rs_rf_start_speed_check);
	dei_rs_safe_to_takeover = TRUE;
	dei_rs_safe_to_takeover_check = FALSE;
	
	rs_rf_restart_wait2safe = 0;
	rs_rf_restart_guard_timer = timeout_f_create(4500, (Exec_Func)rs_rf_restart_check);   // there is ignition check during 4 seconds
}	

//--------------------------------------------------------------------------//

void as_rs_extend_runtime_cmd( void )
{
	as_rs_runtime_cmd_received = TRUE;
}

//--------------------------------------------------------------------------//

// ignition is on -- is it safe to pitstop?
static Boolean safe_to_pitstop(void)
{
#define MIN_REF 13.0	
double ref;
	switch(dei_feature_get( DEI_FEAT_ENGINE_CHECKING_MODE ))
	{
		case DEI_ECM_TACHOMETER:
			if(get_rpm()==0)
				return FALSE; // engine is off
			break;
			
		case DEI_ECM_VIRTUAL_TACHO:
    case DEI_ECM_VOLTAGE:
			ref = ecm_vtacho_ref();
		  if(ref < MIN_REF)
				ref = MIN_REF;
			if(get_voltage() < ref) // should be - ECM_VTACHO_VOLTAGE_RUNNING_DIFF
				return FALSE; // low voltage, alternator and engine must be off, so its not safe to pitstop
			// fall through
		default: 		// DEI_ECM_OFF is never unsafe
		  break;
	}

	return TRUE; // safe to pitstop
#undef MIN_REF	
}

//ign is on, receives start command, 500ms timeout after req speed to decide takeover or not
static void rs_rf_start_speed_check(void)
{
    TRACE_DEI_RS("speed: [%4X]\n\r", get_speed());
    if(get_speed())
      dei_rs_safe_to_takeover = FALSE;
		
		dei_rs_safe_to_takeover_check = TRUE;
		dei_rs_rf_start_stop(NULL);  //resume the calling subroutine
}

static void rs_rf_restart_check(void)
{
		rs_rf_restart_wait2safe = 0;   // now it is safe to resetart
}

void start_restart_guard(void)
{
	rs_rf_restart_wait2safe = 1;
	timeout_start(rs_rf_restart_guard_timer);
}

void dei_rs_rf_start_stop( void *remote )
{	
	Dei_Rs_Evt error_start;

	if(rs_rf_restart_wait2safe)
		return;
	
	dei_ign_before_rs = get_ign_status() ? TRUE : FALSE;
#ifdef PLATFORM_933
	if( (brand_id!= BRAND_VP_SC) && (!dei_rs_state) && (!get_rs_valet()) && (!get_ign_status()))
		{
			notify_send( NOTIFY_RANGE_CHECK );
		}
#endif
	if( (get_ss_trigger()  || get_ss_panic()) && !dei_rs_state )
    {
			#ifdef PLATFORM_933
			  //dei_prk_flash( DEI_PRK_RS_VALET_ON );
				//dei_prk_flash(DEI_PRK_OFF);
				//dei_siren_chirp( DEI_SIREN_STOP );
		  #endif
        notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_ALARM_TRIGGER );
        return ;
    }
                            
		if( get_rs_valet() && !dei_rs_state )
    {
			#ifdef PLATFORM_933
			  dei_prk_flash( DEI_PRK_RS_VALET_ON );
		  #endif
        notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_RS_VALET );
        
        return ;
    }

    if ( dei_feature_get( DEI_FEAT_ACTIVATION_PULSE_COUNT ) == 4)
		{
			notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_TRANSMITTER );
			return;
		}
		
		if(fob_ctrl_dei_rs_status_get() // did this come from OEM activation?
			|| (onebutton_type == 1)
			|| (get_rs_mts_progress() && (dei_feature_get( DEI_FEAT_MANUAL_TRANSMISSION_INIT ) != 1))) //is MTS initializing by park brake?
		{			
			dei_rs_rf_start_stop_vars.press_count = dei_feature_get( DEI_FEAT_ACTIVATION_PULSE_COUNT ); // then they pressed enough
		}
		else
		{
      dei_rs_rf_start_stop_vars.press_count ++ ; // increment another rf command (or wire pulse)
			if(sense_rs_trig_cmd_arrive)
			{
				sense_rs_trig_cmd_arrive = FALSE;
				sense_rs_pulse_count++;
			}
		}
      
		if( !get_rs_turbo() && dei_rs_rf_start_stop_vars.press_count != dei_feature_get( DEI_FEAT_ACTIVATION_PULSE_COUNT ) )
    {
        notify_send( NOTIFY_RANGE_CHECK ) ; 
    
    	vrtc_stop( dei_rs_rf_start_stop_vars.vrtc );
    		
    	vrtc_update( dei_rs_rf_start_stop_vars.vrtc  , DEI_RS_VRTC_TIMEOUT_FIRST_PRESS );
    		
    	vrtc_start ( dei_rs_rf_start_stop_vars.vrtc );
    	
      if((brand_id == BRAND_VP_SC && onebutton_type == 0 && (dei_feature_get(DEI_FEAT_ACTIVATION_PULSE_COUNT) != 0)
				&& dei_rs_state && onebutton_cmd_arrive)	!= 1)  //exception: VPX old 1btn behavior, ignore activation count
    	  return ;    	
    }

	vrtc_stop( dei_rs_rf_start_stop_vars.vrtc );
		
	dei_rs_rf_start_stop_vars.press_count = 0 ;
		
	if( !dei_rs_state ) //remote start is off
	{
		sense_rs_pulse_count = 0;
		if( (error_start = dei_rs_error_start()) == DEI_RS_EVT_TIMEOUT )
		{												
			if( get_rs_turbo() )
			{
				dei_rs_runtime_set( DEI_RS_TURBO_START ) ; 
			}
			else
			{
				dei_rs_runtime_set( DEI_RS_NORMAL_START ) ; 				
				
				if((get_gearbox_type() == GEARBOX_MANUAL))
			    set_rs_turbo( get_rs_mts_progress()&& get_door_status() );
			}

#ifdef PLATFORM_933			
			TRACE_DEI_RS("VSS_ENABLED: [%d]\n\r", VSS_ENABLED);
			if(VSS_ENABLED)              //we will spend 500ms to check if safe to do takeover
			{
				if(get_ign_status())   //check while ign ON ONLY
				{
					if( !dei_rs_safe_to_takeover_check) //re-check later
					{  //req speed in 500ms
						dei_rs_safe_to_takeover = TRUE; //normal
						timeout_start(rs_rf_start_speed_check_timer);   //start check timer, send speed req
						cmd_q_rs_insert(INPUT_REQ_SPEED, 100); 
						return;
					}

					dei_rs_safe_to_takeover_check = FALSE; //reset
					if(!dei_rs_safe_to_takeover)
					{
						notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_IGN_DETECTED  ) ;
						return;
					}
			  }
			}
#endif

			if(get_ign_status() && !safe_to_pitstop())
				notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_IGN_DETECTED  ) ;
			else 
				{ // IGN OFF or otherwise safe to takeover with ignition on
				
#ifdef PLATFORM_CM800			
				notify_send( NOTIFY_RS_START );
#else
				if( brand_id == BRAND_VP_SC)			
					notify_send( NOTIFY_RS_START ); 
#endif	
				if(get_ign_status()) {
					door_open_allow_once=TRUE;
					}
				dei_rs_execute();
				
				}
		}
		else
		{			
			set_rs_turbo( FALSE );
			
			notify_send( NOTIFY_ERROR , error_start );
      dei_rs_rf_start_stop_flash_error( error_start ); 			
		}
	}
	else // rs is on
	{				
#ifdef PLATFORM_CM800		
        dei_rs_evt_signal( DEI_RS_EVT_TRANSMITTER_SHUTDOWN );                
#else
		    dei_ss_lock_stored = get_ss_lock() || get_ss_arm();  // update status to determine Smart Safelock
		
		    if( (brand_id == BRAND_VP_SC) || ( fob_ctrl_dei_rs_status_get() ) )
				{
					dei_rs_evt_signal( DEI_RS_EVT_TRANSMITTER_SHUTDOWN );                
				}
				
				else if( (brand_id == BRAND_AUTOSTART_LCD2_HDR) || (brand_id == BRAND_ASTROSTART_LCD2) )
				{
					if(onebutton_cmd_arrive ||\
				    ((dei_feature_get(DEI_FEAT_ACTIVATION_PULSE_COUNT) != 0) && (sense_rs_pulse_count == dei_feature_get(DEI_FEAT_ACTIVATION_PULSE_COUNT))) )
					{
						sense_rs_pulse_count = 0;
						dei_rs_evt_signal( DEI_RS_EVT_TRANSMITTER_SHUTDOWN );
					}
					else
					{
						vrtc_stop( as_rs_extend_runtime_vrtc );
						vrtc_update( as_rs_extend_runtime_vrtc , AS_EXTENDED_RUNTIME_CMD_TIMEOUT);
						vrtc_start( as_rs_extend_runtime_vrtc );
					}
				}
#endif
	}
	
	fob_ctrl_dei_rs_status_set(FALSE); // consume that 'come from' status now...
}
//--------------------------------------------------------------------------//

void dei_rs_rf_start_stop_flash_error( Dei_Rs_Evt err_start )
{
	switch( err_start )
	{
		case DEI_RS_EVT_HOOD :
		{
			dei_prk_flash( DEI_PRK_ERROR_START_HOOD ) ; 
		}
		break;

		case DEI_RS_EVT_BRAKE :
		{
			dei_prk_flash( DEI_PRK_ERROR_START_BRAKE ) ; 
		}
		break;

		case DEI_RS_EVT_NEUTRAL:
		{
			dei_prk_flash( DEI_PRK_ERROR_START_PARKING ) ; 
		}
		break;

		case DEI_RS_EVT_LOW_TACH :
		{
			dei_prk_flash( DEI_PRK_ERROR_LOW_TACH ) ; 
		}
		break;	
	
		case DEI_RS_EVT_MTS:
		{
			dei_prk_flash( DEI_PRK_ERROR_START_MTS ) ;            
		}
		break;

		default : break;
	}
}
	
//--------------------------------------------------------------------------//	

void dei_rs_rf_start_stop_first_press_reset( void )
{
	dei_rs_rf_start_stop_vars.press_count = 0 ;

	notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_TRANSMITTER );			
}

//--------------------------------------------------------------------------//

void as_rs_notify_timeout( void )
{
	Dei_Rs_Evt error_start;
	if( (error_start = dei_rs_error_start()) != DEI_RS_EVT_TIMEOUT )
		notify_send( NOTIFY_RS_START );
	else
		notify_send( NOTIFY_ERROR , error_start );
}

//--------------------------------------------------------------------------//

void as_rs_extend_runtime_timeout( void )
{
	vrtc_stop( as_rs_extend_runtime_vrtc );
	
	if( brand_id == BRAND_AUTOSTART_LCD2_HDR )
	{
		if( as_rs_runtime_cmd_received )
		{
			as_rs_runtime_cmd_received = FALSE;
			// extend runtime function
			cmd_q_rs_insert( INPUT_RF_RUNTIME_RESET, 100 );
		}
		else
			dei_rs_evt_signal( DEI_RS_EVT_TRANSMITTER_SHUTDOWN );                
	}
	
	if( brand_id == BRAND_ASTROSTART_LCD2 )
	{
		if( as_rs_runtime_cmd_received )
		{
			as_rs_runtime_cmd_received = FALSE;
			dei_rs_evt_signal( DEI_RS_EVT_TRANSMITTER_SHUTDOWN );                
		}
		else
			// extend runtime function
			cmd_q_rs_insert( INPUT_RF_RUNTIME_RESET, 100 );
	}
	
}
