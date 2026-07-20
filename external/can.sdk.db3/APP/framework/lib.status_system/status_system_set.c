/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: status_system_set.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "status_system_private.h"
#include "notify.h"
#include "sensor.h"
#include "wake.h"
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

/*--------------------------------------------------------------------------*/
/*RS SET                                                                    */
/*--------------------------------------------------------------------------*/
void set_rs_gwr( Boolean status )
{
    ATOMIC( status_rs.gwr = status );
}

/*--------------------------------------------------------------------------*/

void set_rs_acc( Boolean status )
{
    ATOMIC( status_rs.acc = status );
}

/*--------------------------------------------------------------------------*/

void set_rs_ign( Boolean status )
{
    ATOMIC( status_rs.ign = status );
}

/*--------------------------------------------------------------------------*/

void set_rs_start( Boolean status )
{
    ATOMIC( status_rs.start = status );
}

/*--------------------------------------------------------------------------*/

void set_rs_smart( Boolean status )
{
    ATOMIC( status_rs.smart_on = status );
}

/*--------------------------------------------------------------------------*/

void set_rs_timer( Boolean status )
{
    ATOMIC( status_rs.timer_on = status );
}

/*--------------------------------------------------------------------------*/

void set_rs_defogger( Boolean status )
{
    ATOMIC( status_rs.defogger_status = status );
}

/*--------------------------------------------------------------------------*/

void set_rs_turbo( Boolean status )
{
    ATOMIC( status_rs.turbo_on = status );
}

/*--------------------------------------------------------------------------*/

void set_rs_idle( Boolean status )
{
    ATOMIC( status_rs.idle_on = status );
}

/*--------------------------------------------------------------------------*/

void set_rs_idle_turbo_done( Boolean status )
{
    ATOMIC( status_rs.idle_turbo_done = status );
}

//---------------------------------------------------------------------------//

void set_rs_mts( Boolean status )
{
    ATOMIC( status_rs.mts_on = status );
}

/*--------------------------------------------------------------------------*/

void set_rs_mts_progress( Boolean status )
{
    ATOMIC( status_rs.mts_in_progress = status );
}

//---------------------------------------------------------------------------//

void set_rs_mts_prepared( Boolean status )
{
    ATOMIC( status_rs.mts_prepared = status );
}


/*--------------------------------------------------------------------------*/

void set_rs_mts_lock( Boolean status )
{
    ATOMIC( status_rs.mts_lock = status );
}

/*--------------------------------------------------------------------------*/

void set_rs_mts_exit_by_hood( Boolean status )
{
    ATOMIC( status_rs.mts_exit_by_hood = status );
}

/*--------------------------------------------------------------------------*/

void set_rs_rpm_ref( UInt16 rpm_ref )
{
    ATOMIC( status_rs.rpm_ref = rpm_ref );
}

/*--------------------------------------------------------------------------*/

void set_rs_vtacho_ref( UInt8 vtacho_ref )
{
    ATOMIC( status_rs.vtacho_ref = vtacho_ref );
}

/*--------------------------------------------------------------------------*/

void set_rs_retry_count( UInt8 retry_count )
{
    ATOMIC( status_rs.retry_count = retry_count );
}

/*--------------------------------------------------------------------------*/

void set_rs_error_shutdown( UInt8 error_shutdown )
{
    ATOMIC( status_rs.error_shutdown = error_shutdown );
}

/*--------------------------------------------------------------------------*/

void set_rs_runtime_programmed( UInt8 runtime )
{
    ATOMIC( status_rs.runtime_programmed = runtime );
}

/*--------------------------------------------------------------------------*/

void set_rs_runtime_remaining( UInt8 runtime )
{
    ATOMIC( status_rs.runtime_remaining = runtime );
}

/*--------------------------------------------------------------------------*/

void set_rs_start_successful( Boolean status )
{
    ATOMIC( status_rs.start_successful = status );
}

/*--------------------------------------------------------------------------*/

void set_rs_valet( Boolean status )
{
    ATOMIC( status_rs.valet = status );
}

/*--------------------------------------------------------------------------*/

void set_wire_ign_skip_status( Boolean status )
{
    ATOMIC( status_rs.wire_ign_skip = status );
}

/*--------------------------------------------------------------------------*/

void set_rs_ign_off_status( Boolean status )
{
    ATOMIC( status_rs.rs_ign_off = status );
}

/*--------------------------------------------------------------------------*/

void set_rs_ign_disarm_oem( Boolean status )
{
    ATOMIC( status_rs.ign_disarm_oem = status );
}


/*--------------------------------------------------------------------------*/

void set_rs_takeover( Boolean status )
{
    ATOMIC( status_rs_takeover = status );
}

/*--------------------------------------------------------------------------*/

void set_rs_iid_engine_running( Boolean status )
{
    ATOMIC( status_rs.iid_engine_running = status );
}


/*--------------------------------------------------------------------------*/
/*SS SET                                                                    */
/*--------------------------------------------------------------------------*/

void set_ss_arm( Boolean status )
{
    Boolean prev;	
	ATOMIC( prev = status_ss.armed;	status_ss.armed = status );
#ifdef PLATFORM_933
	if(prev != status)
		wake_sleep_soon();
#endif
}

/*--------------------------------------------------------------------------*/

void set_ss_lock( Boolean status )
{
    ATOMIC( status_ss.locked = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_trigger( Boolean status )
{
    ATOMIC( status_ss.trigger_active = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_panic( Boolean status )
{
    ATOMIC( status_ss.panic_active = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_valet( Boolean status )
{
    ATOMIC( status_ss.valet = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_sensor_npc( Boolean status )
{
    if( get_trig_sensor_index() == 1)
        ATOMIC( status_ss.shock_sensor_npc = status );

    if( get_trig_sensor_index() == 2 )
        ATOMIC( status_ss.tilt_sensor_npc = status );

    if( get_trig_sensor_index() == 0 )
        ATOMIC( status_ss.unknown_sensor_npc = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_sensor_trigger( Boolean status )
{
    if( get_trig_sensor_index() == 1)
        ATOMIC( status_ss.shock_sensor_triggered = status );

    if( get_trig_sensor_index() == 2 )
        ATOMIC( status_ss.tilt_sensor_triggered = status );

    if( get_trig_sensor_index() == 0 )
        ATOMIC( status_ss.unknown_sensor_triggered = status );
}


/*--------------------------------------------------------------------------*/

void set_ss_sensor_trigger_all( Boolean status )
{
        status_ss.shock_sensor_triggered = status ;

        status_ss.tilt_sensor_triggered = status ;

        status_ss.unknown_sensor_triggered = status ;
}

/*--------------------------------------------------------------------------*/

void set_ss_sensor_trigger_src_history( UInt8 src )
{
    static const UInt32 trigger_source[] =
    {
        NOTIFY_PLD_TRIG_TRUNK , NOTIFY_PLD_TRIG_S1 , NOTIFY_PLD_TRIG_DOOR  , NOTIFY_PLD_TRIG_IGN , NOTIFY_PLD_TRIG_HOOD, NOTIFY_PLD_TRIG_S2, NOTIFY_PLD_TRIG_S3
    };

    status_ss.sensor_trig_src |= trigger_source[src];
}

/*--------------------------------------------------------------------------*/

void set_ss_door_trigger( Boolean status )
{
    ATOMIC( status_ss.door_triggered = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_hood_trigger( Boolean status )
{
    ATOMIC( status_ss.hood_triggered = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_trunk_trigger( Boolean status )
{
    ATOMIC( status_ss.trunk_triggered = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_ign_trigger( Boolean status )
{
    ATOMIC( status_ss.ign_triggered = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_door_bypass( Boolean status )
{
    ATOMIC( status_ss.door_bypassed = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_trunk_bypass( Boolean status )
{
    ATOMIC( status_ss.trunk_bypassed = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_hood_bypass( Boolean status )
{
    ATOMIC( status_ss.hood_bypassed = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_inst_trig_bypass( Boolean status )
{
    ATOMIC( status_ss.inst_trig_bypassed = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_trunk_release( Boolean status )
{
    ATOMIC( status_ss.trunk_release_active = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_sensor_silent_arm( Boolean status )
{
    ATOMIC( status_ss.sensor_silent_arm = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_full_silent_arm( Boolean status )
{
    ATOMIC( status_ss.full_silent_arm = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_one_time_bypass( Boolean status )
{
    ATOMIC( status_ss.one_time_bypass = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_ch3( Boolean status )
{
    ATOMIC( status_ss.ch3_status = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_ch4( Boolean status )
{
    ATOMIC( status_ss.ch4_status = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_ch5( Boolean status )
{
    ATOMIC( status_ss.ch5_status = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_ch6( Boolean status )
{
    ATOMIC( status_ss.ch6_status = status );
}

/*--------------------------------------------------------------------------*/
void    set_ss_programming_on( Boolean status )
{
    ATOMIC( status_ss.programming_on = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_last_trigger( UInt8 evt )
{
    ATOMIC( status_ss.last_trigger = evt );
}

/*--------------------------------------------------------------------------*/

void set_ss_trig_src( UInt8 src )
{
    ATOMIC( status_ss.trig_src = src );
}

/*--------------------------------------------------------------------------*/

void set_ss_warn_src( UInt8 src )
{
    ATOMIC( status_ss.warn_src = src );
}

/*--------------------------------------------------------------------------*/
#ifdef PLATFORM_933
static UInt8 exist_evt_check(UInt8 idx_cur, UInt8 evt)
{
    UInt8 ret_val = 0xFF;
    UInt8 x;

    for(x = 0; x < (idx_cur & (SS_TRIGGER_MEM_SIZE-1)); x++)
    {
        if(evt == status_ss.event_memory[x])
        {
            ret_val = x;
            break;
        }
    }
    return ret_val;
}
#endif

void set_ss_memory_trigger_event( UInt8 evt )
{
    static UInt8 idx = 0 ;

#ifdef PLATFORM_933
    UInt8 exist_evt_idx;
#endif

    if(!evt)
        return;

//  ATOMIC
//  (
#ifdef PLATFORM_933    //leave the newest in the end
    if(idx < SS_TRIGGER_MEM_SIZE)
    {
        exist_evt_idx = exist_evt_check(idx, evt);
        if(exist_evt_idx == 0xFF)
        status_ss.event_memory[idx++] = evt ;
        else
        {
            //new one exist, put it in the end of queue
            memcpy(status_ss.event_memory + exist_evt_idx, status_ss.event_memory + exist_evt_idx +1, (idx-exist_evt_idx));
            status_ss.event_memory[idx] = evt;
        }
    }
    else
    {
        exist_evt_idx = exist_evt_check((SS_TRIGGER_MEM_SIZE-1), evt);  //check full history mem
        if(exist_evt_idx == 0xFF)
        {  //not exist, put it in the end of queue, lost oldest
            memcpy(status_ss.event_memory, status_ss.event_memory + 1, 3);
            status_ss.event_memory[SS_TRIGGER_MEM_SIZE-1] = evt;
            idx++;
        }
        else
        {
            if(exist_evt_idx < (SS_TRIGGER_MEM_SIZE-1))
      {
                memcpy(status_ss.event_memory + exist_evt_idx, status_ss.event_memory + exist_evt_idx + 1, (SS_TRIGGER_MEM_SIZE - 1 - exist_evt_idx));
                status_ss.event_memory[SS_TRIGGER_MEM_SIZE-1] = evt;
            }
            //no change if the new one is the same of newest in the queue
        }
    }
#else
        status_ss.event_memory[ idx ] = evt ;

        idx = (idx + 1)%SS_TRIGGER_MEM_SIZE ;
#endif
//  )
}

/*--------------------------------------------------------------------------*/

void set_ss_custom_trigger( Boolean status )
{
    ATOMIC( status_ss.custom_triggered = status );
}

/*--------------------------------------------------------------------------*/

void set_ss_custom_trigger_zone( Boolean status )
{
    ATOMIC( status_ss.custom_trig_zone = status );
}

/*--------------------------------------------------------------------------*/

void set_rpm_control_lock_status( Boolean status )
{
    ATOMIC( status_ss.rpm_control_lock_status = status );
}

/*--------------------------------------------------------------------------*/
/*SENSE STATUS SET                                                          */
/*--------------------------------------------------------------------------*/

void set_driver_door_data_status( Boolean status )
{
    ATOMIC
    (
        status_sense.driver_door_data = status;
    )

}

/*--------------------------------------------------------------------------*/

void set_other_door_data_status( Boolean status )
{
    ATOMIC
    (
        status_sense.other_door_data = status;
    )
}

/*--------------------------------------------------------------------------*/

void set_door_wire_status( Boolean status )
{
    status_sense.door_wire = status;
}

/*--------------------------------------------------------------------------*/

void set_hood_data_status( Boolean status )
{
    ATOMIC( status_sense.hood_data = status );
}

/*--------------------------------------------------------------------------*/

void set_hood_wire_status( Boolean status )
{
    ATOMIC( status_sense.hood_wire = status );
}

/*--------------------------------------------------------------------------*/

void set_trunk_data_status( Boolean status )
{
    ATOMIC( status_sense.trunk_data = status );
}

/*--------------------------------------------------------------------------*/

void set_trunk_wire_status( Boolean status )
{
    ATOMIC( status_sense.trunk_wire = status );
}

/*--------------------------------------------------------------------------*/

void set_brake_data_status( Boolean status )
{
    ATOMIC( status_sense.brake_data = status );
}

/*--------------------------------------------------------------------------*/

void set_brake_wire_status( Boolean status )
{
    ATOMIC( status_sense.brake_wire = status );
}

/*--------------------------------------------------------------------------*/

void set_handbrake_wire_status( Boolean status )
{
    ATOMIC( status_sense.handbrake_wire = status );
}

/*--------------------------------------------------------------------------*/

void set_handbrake_data_status( Boolean status )
{
    ATOMIC( status_sense.handbrake_data = status );
}

/*--------------------------------------------------------------------------*/

void set_ign_wire_status( Boolean status )
{
    ATOMIC( status_sense.ign_wire = status );

    if( !get_ign_status() )
    {
        ATOMIC( status_sense.rpm_data = 0 );
    }
        if( get_ign_status() )
        {
            set_rs_ign_off_status( FALSE );     // to clear the status for RS ignition off
        }
}

/*--------------------------------------------------------------------------*/

void set_ign_data_status( Boolean status )
{
    ATOMIC( status_sense.ign_data = status );

    if( !get_ign_status() )
    {
        ATOMIC( status_sense.rpm_data = 0 );
    }
        if( get_ign_status() )
        {
            set_rs_ign_off_status( FALSE );     // to clear the status for RS ignition off
        }
}

/*--------------------------------------------------------------------------*/

void set_key_in_data_status( Boolean status )
{
    ATOMIC( status_sense.key_in_data = status );
}

/*--------------------------------------------------------------------------*/

void set_rpm_wire( UInt16 rpm )
{
    ATOMIC
    (
        status_sense.rpm_wire = rpm ;
        status_sense_source.rpm_updated_by_data = FALSE ;
        status_sense_source.rpm_updated_by_wire = TRUE ;
    )
}

/*--------------------------------------------------------------------------*/

void set_rpm_data( UInt16 rpm )
{
    if( get_ign_status() || get_rs_ign ())
    {
        ATOMIC
        (
            status_sense.rpm_data = rpm ;
            status_sense_source.rpm_updated_by_data = TRUE ;
            status_sense_source.rpm_updated_by_wire = FALSE ;
        )
    }
    else
    {
        ATOMIC( status_sense.rpm_data = 0 );
    }
}

/*--------------------------------------------------------------------------*/

void set_speed( UInt32 speed )
{
        ATOMIC
        (
                status_sense.speed = speed ;
                status_sense_source.speed_updated_by_data = TRUE ;
                status_sense_source.speed_updated_by_wire = FALSE ;
        )
}

/*--------------------------------------------------------------------------*/

void set_speed_wire( UInt32 speed )
{
        ATOMIC
        (
                status_sense.speed_wire = speed ;
                status_sense_source.speed_updated_by_data = FALSE ;
                status_sense_source.speed_updated_by_wire = TRUE ;
        )
}

/*--------------------------------------------------------------------------*/

void set_skd_present( Boolean is_present )
{
    ATOMIC( status_sense.skd_is_present = is_present );
}

/*--------------------------------------------------------------------------*/

void set_glowplug_off_wire_status( Boolean status )
{
    ATOMIC( status_sense.glowplug_off_wire = status );
}

/*--------------------------------------------------------------------------*/

void set_glowplug_off_data_status( Boolean status )
{
    ATOMIC( status_sense.glowplug_off_data = status );
}

/*--------------------------------------------------------------------------*/

void set_safety_sw_status( SAFETY status )
{
    ATOMIC( status_sense.safety_sw = status );
}

/*--------------------------------------------------------------------------*/

void set_hood_close_type ( HOOD_TYPE type )
{
    ATOMIC( status_sense.hood_close_type = type );
}

/*--------------------------------------------------------------------------*/

void set_temp_sensor_present( Boolean is_present )
{
    ATOMIC( status_sense.temp.is_present = is_present );
}

/*--------------------------------------------------------------------------*/

void set_temp_sensor_value( UInt8 temp_value )
{
    ATOMIC( status_sense.temp.value = temp_value );
}

/*--------------------------------------------------------------------------*/

void  set_temp_sensor_db_num( UInt8 num )
{
    ATOMIC( status_sense.temp.db_entry_num = num );
}

/*--------------------------------------------------------------------------*/

void  set_temp_chanl_num( UInt8 chanl )
{
    ATOMIC( status_sense.temp.tempsens_chanl = chanl) ;
}

/*--------------------------------------------------------------------------*/

void  set_shock_sensor_present( Boolean is_present )
{
    ATOMIC( status_sense.shock.is_present = is_present ) ;
}

/*--------------------------------------------------------------------------*/

void  set_shock_sensor_level( UInt8 level )
{
    ATOMIC( status_sense.shock.sens_level = level ) ;
}

/*--------------------------------------------------------------------------*/

void  set_shock_sensor_detect_bits( UInt8 bits )
{
    ATOMIC( status_sense.shock.sens_ena_detect_bits = bits ) ;
}

/*--------------------------------------------------------------------------*/

void  set_shock_sensor_alert_bits( UInt8 bits )
{
    ATOMIC( status_sense.shock.sens_ena_alert_bits = bits ) ;
}

/*--------------------------------------------------------------------------*/

void  set_shock_sensor_db_num( UInt8 num )
{
    ATOMIC( status_sense.shock.db_entry_num = num );
}

/*--------------------------------------------------------------------------*/

void  set_shock_chanl_num( UInt8 chanl )
{
    ATOMIC( status_sense.shock.shock_chanl = chanl) ;
}


/*--------------------------------------------------------------------------*/

void set_sensor_status( Boolean status )
{
    ATOMIC( status_sense.sensor = status );
}

/*--------------------------------------------------------------------------*/

void set_push_board_status( Boolean status )
{
    ATOMIC( status_sense.push_board = status );
}

/*--------------------------------------------------------------------------*/

void set_push_antenna_status( Boolean status )
{
    ATOMIC( status_sense.push_antenna = status);
}

/*--------------------------------------------------------------------------*/

void set_gearbox_type( GEARBOX type )
{
    ATOMIC( status_sense.gearbox_type = type);
}

/*--------------------------------------------------------------------------*/

void    set_voltage( double voltage )
{
    ATOMIC( status_sense.voltage = voltage);
}

/*--------------------------------------------------------------------------*/

void set_door_close_type ( DOOR_TYPE type )
{
    ATOMIC( status_sense.door_close_type = type );
}

/*--------------------------------------------------------------------------*/

void set_trunk_close_type ( TRUNK_TYPE type )
{
    ATOMIC( status_sense.trunk_close_type = type );
}

/*--------------------------------------------------------------------------*/

void set_headlight_wire_status( Boolean status )
{
    ATOMIC( status_sense.headlight_wire = status );
}

/*--------------------------------------------------------------------------*/

void set_inst_trig_wire_status( Boolean status )
{
    ATOMIC( status_sense.inst_trig_wire = status );
}


/*--------------------------------------------------------------------------*/
/*SET SENSING SOURCE                                                        */
/*--------------------------------------------------------------------------*/

void set_door_source( SOURCE source )
{
    ATOMIC( status_sense_source.door = source );
}

/*--------------------------------------------------------------------------*/

void set_hood_source( SOURCE source )
{
    ATOMIC( status_sense_source.hood = source );
}

/*--------------------------------------------------------------------------*/

void set_trunk_source( SOURCE source )
{
    ATOMIC( status_sense_source.trunk = source );
}

/*--------------------------------------------------------------------------*/

void set_ign_source( SOURCE source )
{
    ATOMIC( status_sense_source.ign = source );
}

/*--------------------------------------------------------------------------*/

void set_brake_source( SOURCE source )
{
    ATOMIC( status_sense_source.brake = source );
}

/*--------------------------------------------------------------------------*/

void set_handbrake_source( SOURCE source )
{
    ATOMIC( status_sense_source.handbrake = source );
}

/*--------------------------------------------------------------------------*/

void set_rpm_source( SOURCE source )
{
    ATOMIC( status_sense_source.rpm = source );
}

/*--------------------------------------------------------------------------*/

void set_glowplug_source( SOURCE source )
{
    ATOMIC( status_sense_source.glowplug = source );
}

/*--------------------------------------------------------------------------*/

void set_speed_source( SOURCE source )
{
    ATOMIC( status_sense_source.speed = source );
}

/*--------------------------------------------------------------------------*/

void set_headlight_source( SOURCE source )
{
    ATOMIC( status_sense_source.headlight = source );
}

/*--------------------------------------------------------------------------*/
/* SET THE USAGE OF RKE                                                     */
/*--------------------------------------------------------------------------*/

void set_rke_lock_use( RKE_USE status )
{
    ATOMIC( status_rke_use.lock = status );
}

/*--------------------------------------------------------------------------*/

void set_rke_unlock_use( RKE_USE status )
{
    ATOMIC( status_rke_use.unlock = status );
}

/*--------------------------------------------------------------------------*/

void set_rke_trunk_use( RKE_USE status )
{
    ATOMIC( status_rke_use.trunk = status );
}

/*--------------------------------------------------------------------------*/

void set_door_skip( void )
{
    ATOMIC
    (
        status_sense.door_status_before_skip = get_door_status();
        status_sense.door_skip = TRUE  ;
    )
}

/*--------------------------------------------------------------------------*/

void clr_door_skip( void )
{
    Boolean do_update ;

    ATOMIC
    (
        status_sense.door_skip = FALSE ;

        do_update = status_sense.door_status_before_skip != get_door_status();
    )

    if( do_update )
    {
        cmd_q_rs_insert( get_door_status() ? INPUT_ALL_DOOR_STATUS_OPEN :  INPUT_ALL_DOOR_STATUS_CLOSE , STATUS_Q_TIMEOUT );
    }
}

/*--------------------------------------------------------------------------*/

void set_zone_bypass_status( Boolean status )
{
    ATOMIC( status_ss.zone_bypass_in_progress = status );
}

void set_cmd_source_d2d(Boolean status)
{
     cmd_source_d2d = status;
}

/*--------------------------------------------------------------------------*/
