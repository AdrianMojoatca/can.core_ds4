/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: status_system_get.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "status_system_private.h"
#include "notify.h"
#include "sensor.h"
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
/*RS GET                                                                    */
/*--------------------------------------------------------------------------*/
Boolean  get_rs_gwr( void )
{
    return status_rs.gwr ;
}

/*--------------------------------------------------------------------------*/

Boolean  get_rs_acc( void )
{
    return status_rs.acc ;
}

/*--------------------------------------------------------------------------*/

Boolean get_rs_ign( void )
{
    return status_rs.ign ;
}

/*--------------------------------------------------------------------------*/

Boolean get_rs_start( void )
{
    return status_rs.start ;
}

/*--------------------------------------------------------------------------*/

Boolean  get_rs_smart( void )
{
    return status_rs.smart_on ;
}

/*--------------------------------------------------------------------------*/

Boolean get_rs_timer( void )
{
    return status_rs.timer_on ;
}

/*--------------------------------------------------------------------------*/

Boolean get_rs_defogger( void )
{
    return status_rs.defogger_status ;
}

/*--------------------------------------------------------------------------*/

Boolean get_rs_turbo( void )
{
    return status_rs.turbo_on ;
}

/*--------------------------------------------------------------------------*/

Boolean get_rs_idle( void )
{
    return status_rs.idle_on ;
}

/*--------------------------------------------------------------------------*/

Boolean get_rs_idle_turbo_done( void )
{
    return status_rs.idle_turbo_done ;
}

/*--------------------------------------------------------------------------*/

Boolean get_rs_mts( void )
{
    return status_rs.mts_on ;
}

/*--------------------------------------------------------------------------*/

Boolean get_rs_mts_progress( void )
{
    return status_rs.mts_in_progress ;
}

/*--------------------------------------------------------------------------*/

Boolean get_rs_mts_prepared( void )
{
    return status_rs.mts_prepared ;
}

/*--------------------------------------------------------------------------*/

Boolean get_rs_mts_lock( void )
{
    return status_rs.mts_lock ;
}

/*--------------------------------------------------------------------------*/

Boolean get_rs_mts_exit_by_hood( void )
{
    return status_rs.mts_exit_by_hood ;
}

/*--------------------------------------------------------------------------*/

UInt16 get_rs_rpm_ref( void )
{
    return  status_rs.rpm_ref ;
}

/*--------------------------------------------------------------------------*/

Ecm_Vtacho_Ref get_rs_vtacho_ref( void )
{
    return (Ecm_Vtacho_Ref)status_rs.vtacho_ref ;
}

/*--------------------------------------------------------------------------*/

UInt8 get_rs_retry_count( void )
{
    return status_rs.retry_count ;
}

/*--------------------------------------------------------------------------*/

UInt8 get_rs_error_shutdown( void )
{
    return status_rs.error_shutdown ;
}

/*--------------------------------------------------------------------------*/

UInt8 get_rs_runtime_programmed( void )
{
    return status_rs.runtime_programmed ;
}

/*--------------------------------------------------------------------------*/

UInt8 get_rs_runtime_remaining( void )
{
    return status_rs.runtime_remaining ;
}

/*--------------------------------------------------------------------------*/

Boolean get_rs_start_successful ( void )
{
    return status_rs.start_successful;
}

/*--------------------------------------------------------------------------*/

Boolean get_rs_valet( void )
{
    return status_rs.valet;
}

/*--------------------------------------------------------------------------*/

Boolean get_wire_ign_skip_status( void )
{
    return status_rs.wire_ign_skip;
}

/*--------------------------------------------------------------------------*/

Boolean get_rs_ign_off_status( void )
{
    return status_rs.rs_ign_off;
}

/*--------------------------------------------------------------------------*/

Boolean get_rs_ign_disarm_oem( void )
{
    return status_rs.ign_disarm_oem;
}

/*--------------------------------------------------------------------------*/
Boolean get_rs_takeover(void)
{
    return status_rs_takeover;
}

/*--------------------------------------------------------------------------*/
Boolean get_rs_iid_engine_running(void)
{
	return status_rs.iid_engine_running;
}

/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*SS GET                                                                    */
/*--------------------------------------------------------------------------*/
Boolean get_ss_arm( void )
{
    return status_ss.armed  ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_lock( void )
{
    return status_ss.locked  ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_trigger( void )
{
    return status_ss.trigger_active  ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_panic( void )
{
    return status_ss.panic_active  ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_valet( void )
{
    return status_ss.valet  ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_sensor_npc( UInt8 index)
{
    if( index == 1 )
        return status_ss.shock_sensor_npc  ;

    if( index == 2 )
        return status_ss.tilt_sensor_npc  ;

    if( index == 0 )
        return status_ss.unknown_sensor_npc  ;

    return FALSE;

}

/*--------------------------------------------------------------------------*/

Boolean get_ss_sensor_trigger( UInt8 index )
{
    if( index == 1 )
        return status_ss.shock_sensor_triggered  ;

    if( index == 2 )
        return status_ss.tilt_sensor_triggered  ;

    if( index == 0 )
        return status_ss.unknown_sensor_triggered  ;

    return FALSE;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_last_sensor_trigger( void )
{
    if( get_trig_sensor_index() == 1 )
        return status_ss.shock_sensor_triggered  ;

    if( get_trig_sensor_index() == 2 )
        return status_ss.tilt_sensor_triggered  ;

    if( get_trig_sensor_index() == 0 )
        return status_ss.unknown_sensor_triggered  ;

    return FALSE;
}

/*--------------------------------------------------------------------------*/

UInt8 get_ss_sensor_trigger_src_history( void )
{
    return status_ss.sensor_trig_src ;
}


/*--------------------------------------------------------------------------*/

Boolean get_ss_door_trigger( void )
{
    return status_ss.door_triggered  ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_hood_trigger( void )
{
    return status_ss.hood_triggered  ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_trunk_trigger( void )
{
    return status_ss.trunk_triggered  ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_ign_trigger( void )
{
    return status_ss.ign_triggered  ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_door_bypass( void )
{
    return status_ss.door_bypassed  ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_trunk_bypass( void )
{
    return status_ss.trunk_bypassed  ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_hood_bypass( void )
{
    return status_ss.hood_bypassed  ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_inst_trig_bypass( void )
{
    return status_ss.inst_trig_bypassed  ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_trunk_release( void )
{
    return status_ss.trunk_release_active  ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_sensor_silent_arm( void )
{
    return status_ss.sensor_silent_arm  ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_full_silent_arm( void )
{
    return status_ss.full_silent_arm  ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_one_time_bypass( void )
{
    return status_ss.one_time_bypass  ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_ch3( void )
{
    return status_ss.ch3_status  ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_ch4( void )
{
    return status_ss.ch4_status  ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_ch5( void )
{
    return status_ss.ch5_status  ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_ch6( void )
{
    return status_ss.ch6_status  ;
}

/*--------------------------------------------------------------------------*/
Boolean get_ss_programming_on( void )
{
    return status_ss.programming_on  ;
}

/*--------------------------------------------------------------------------*/

UInt8 get_ss_last_trigger( void )
{
    return status_ss.last_trigger ;
}

/*--------------------------------------------------------------------------*/

UInt8 get_ss_trig_src( void )
{
    return status_ss.trig_src ;
}

/*--------------------------------------------------------------------------*/

UInt8 get_ss_warn_src( void )
{
    return status_ss.warn_src ;
}

/*--------------------------------------------------------------------------*/

UInt8 get_ss_memory_trigger_event( UInt8 pos )
{
    UInt8 evt = 0 ;

    if( pos < SS_TRIGGER_MEM_SIZE )
    {
        evt = status_ss.event_memory[ pos ];
    }

    return evt ;
}

/*--------------------------------------------------------------------------*/

Boolean get_ss_custom_trigger_zone( void )
{
    return status_ss.custom_trig_zone;
}

/*--------------------------------------------------------------------------*/
Boolean get_rpm_control_lock_status( void )
{
    return status_ss.rpm_control_lock_status  ;
}


/*--------------------------------------------------------------------------*/
/*SENSE GET                                                                 */
/*--------------------------------------------------------------------------*/

Boolean  get_door_status( void )
{
    Boolean status  = FALSE;

    Boolean truewire = status_sense.door_wire;

    if( status_sense.door_skip ) return status_sense.door_status_before_skip ;

    switch( status_sense_source.door )
    {
        case SOURCE_AUTO :  status = status_sense.driver_door_data || status_sense.other_door_data || truewire; /*status_sense.door_wire;*/ break;
        case SOURCE_DATA :  status = status_sense.driver_door_data || status_sense.other_door_data; break;
        case SOURCE_WIRE :  status = truewire; break;
    }

    return status ;
}

/*--------------------------------------------------------------------------*/

Boolean  get_driver_door_status( void )
{
    Boolean status  = FALSE;

    switch( status_sense_source.door )
    {
        case SOURCE_AUTO :  status = status_sense.driver_door_data || status_sense.door_wire ;break;
        case SOURCE_DATA :  status = status_sense.driver_door_data                           ;break;
        case SOURCE_WIRE :  status = status_sense.door_wire                                                                  ;break;
    }

    return status ;
}

/*--------------------------------------------------------------------------*/

Boolean  get_other_door_status( void )
{
    Boolean status  = FALSE;


    switch( status_sense_source.door )
    {
        case SOURCE_AUTO :  status = status_sense.other_door_data || status_sense.door_wire ;break;
        case SOURCE_DATA :  status = status_sense.other_door_data                           ;break;
        case SOURCE_WIRE :  status = status_sense.door_wire                                                                  ;break;
    }

    return status ;
}

/*--------------------------------------------------------------------------*/

Boolean  get_hood_status( void )
{
    Boolean status = FALSE ;

    switch( status_sense_source.hood )
    {
        case SOURCE_AUTO : status = status_sense.hood_data || status_sense.hood_wire; break;
        case SOURCE_DATA : status = status_sense.hood_data ; break;
        case SOURCE_WIRE : status =status_sense.hood_wire; break;
    }

    return status ;
}

/*--------------------------------------------------------------------------*/

Boolean  get_trunk_status( void )
{
    Boolean status  = FALSE;

    switch( status_sense_source.trunk )
    {
        case SOURCE_AUTO :  status = status_sense.trunk_data || status_sense.trunk_wire ;break;
        case SOURCE_DATA :  status = status_sense.trunk_data                            ;break;

        case SOURCE_WIRE :  status = status_sense.trunk_wire                            ;break;
    }

    return status ;
}

/*--------------------------------------------------------------------------*/

Boolean  get_ign_status( void )
{
    Boolean status  = FALSE;

    switch( status_sense_source.ign )
    {
        case SOURCE_AUTO :  status = status_sense.ign_data || status_sense.ign_wire ;break;
        case SOURCE_DATA :  status = status_sense.ign_data                          ;break;
        case SOURCE_WIRE :  status = status_sense.ign_wire                          ;break;
    }

    return status ;
}

/*--------------------------------------------------------------------------*/

Boolean  get_key_in_status( void )
{
    Boolean status ;

    status = status_sense.key_in_data ;

    return status ;
}

/*--------------------------------------------------------------------------*/


Boolean  get_brake_status( void )
{
    Boolean status  = FALSE;

    switch( status_sense_source.brake )
    {
        case SOURCE_AUTO :  status = status_sense.brake_data || status_sense.brake_wire ;break;
        case SOURCE_DATA :  status = status_sense.brake_data                            ;break;
        case SOURCE_WIRE :  status = status_sense.brake_wire                            ;break;
    }

    return status ;
}

/*--------------------------------------------------------------------------*/

Boolean  get_handbrake_status( void )
{
    Boolean status  = FALSE;

    switch( status_sense_source.handbrake )
    {
        case SOURCE_AUTO :  status = status_sense.handbrake_data || status_sense.handbrake_wire ;break;
        case SOURCE_DATA :  status = status_sense.handbrake_data                                ;break;
        case SOURCE_WIRE :  status = status_sense.handbrake_wire                                ;break;
    }

    return status ;
}

/*--------------------------------------------------------------------------*/

Boolean  get_glowplug_off_status( void )
{
    Boolean status  = FALSE;

    switch( status_sense_source.glowplug )
    {
        case SOURCE_AUTO :  status = status_sense.glowplug_off_data || status_sense.glowplug_off_wire ;break;
        case SOURCE_DATA :  status = status_sense.glowplug_off_data                                   ;break;
        case SOURCE_WIRE :  status = status_sense.glowplug_off_wire                                   ;break;
    }

    return status ;
}

/*--------------------------------------------------------------------------*/


UInt16  get_rpm( void )
{
    UInt16 status = 0 ;

    switch( status_sense_source.rpm )
    {
        case SOURCE_DATA : status = status_sense.rpm_data; break;
        case SOURCE_WIRE : status = status_sense.rpm_wire; break;
        case SOURCE_AUTO :
        {
            if( status_sense_source.rpm_updated_by_data )
            {
                status = status_sense.rpm_data ;
            }
            else
            {
                status = status_sense.rpm_wire ;
            }
        }
        break;
    }

    return status ;
}

/*--------------------------------------------------------------------------*/

UInt32 get_speed( void )
{
    UInt32 status = 0 ;

    switch( status_sense_source.speed )
    {
        case SOURCE_AUTO :
        {
            if( status_sense_source.speed_updated_by_data )
            {
                status = status_sense.speed ;
            }
            else
            {
                status = status_sense.speed_wire ;
            }
        }
        break;

        case SOURCE_DATA :
                {
                    if( status_sense_source.speed_updated_by_data )
                        status = status_sense.speed;
                }
                    break;
        case SOURCE_WIRE :
                {
                    if( status_sense_source.speed_updated_by_wire )
                        status = status_sense.speed_wire;
                }
                break;
    }

    return status ;
}

/*--------------------------------------------------------------------------*/

Boolean get_skd_present( void )
{
    Boolean status ;

    status = status_sense.skd_is_present ;

    return status ;
}

/*--------------------------------------------------------------------------*/

double  get_voltage( void )
{
    return status_sense.voltage ;
}

/*--------------------------------------------------------------------------*/

Boolean  get_temp_sensor_present( void )
{
    return status_sense.temp.is_present ;
}

/*--------------------------------------------------------------------------*/

UInt8  get_temp_sensor_value( void )
{
    return status_sense.temp.value ;
}

/*--------------------------------------------------------------------------*/

UInt8  get_temp_sensor_db_num( void )
{
    return status_sense.temp.db_entry_num ;
}

/*--------------------------------------------------------------------------*/

UInt8  get_temp_chanl_num( void )
{
    return status_sense.temp.tempsens_chanl  ;
}

/*--------------------------------------------------------------------------*/

Boolean  get_shock_sensor_present( void )
{
    return status_sense.shock.is_present ;
}

/*--------------------------------------------------------------------------*/

UInt8  get_shock_sensor_level( void )
{
    return status_sense.shock.sens_level ;
}

/*--------------------------------------------------------------------------*/

UInt8  get_shock_sensor_detect_bits( void )
{
    return status_sense.shock.sens_ena_detect_bits ;
}

/*--------------------------------------------------------------------------*/

UInt8  get_shock_sensor_alert_bits( void )
{
    return status_sense.shock.sens_ena_alert_bits ;
}

/*--------------------------------------------------------------------------*/

UInt8  get_shock_sensor_db_num( void )
{
    return status_sense.temp.db_entry_num ;
}

/*--------------------------------------------------------------------------*/

UInt8  get_shock_chanl_num( void )
{
    return status_sense.shock.shock_chanl ;
}

/*--------------------------------------------------------------------------*/

Boolean get_sensor_status( void )
{
    return status_sense.sensor ;
}

/*--------------------------------------------------------------------------*/

Boolean get_push_board_status( void )
{
    return status_sense.push_board ;
}

/*--------------------------------------------------------------------------*/

Boolean get_push_antenna_status( void )
{
    return status_sense.push_antenna ;
}

/*--------------------------------------------------------------------------*/

GEARBOX get_gearbox_type( void )
{
    return status_sense.gearbox_type ;
}

/*--------------------------------------------------------------------------*/

DOOR_TYPE get_door_close_type( void )
{
    return status_sense.door_close_type ;
}

/*--------------------------------------------------------------------------*/

HOOD_TYPE get_hood_close_type( void )
{
    return status_sense.hood_close_type ;
}

/*--------------------------------------------------------------------------*/
TRUNK_TYPE get_trunk_close_type(void)
{
    return status_sense.trunk_close_type;
}

/*--------------------------------------------------------------------------*/

SAFETY  get_safety_sw_status ( void )
{
    return status_sense.safety_sw ;
}

/*--------------------------------------------------------------------------*/

Boolean get_headlight_wire_status ( void )
{
    return status_sense.headlight_wire ;
}

/*--------------------------------------------------------------------------*/

Boolean get_inst_trig_wire_status ( void )
{
    return status_sense.inst_trig_wire ;
}


/*--------------------------------------------------------------------------*/
/*SENSE SOURCE GET                                                          */
/*--------------------------------------------------------------------------*/

SOURCE get_door_source( void )
{
    return status_sense_source.door  ;
}

/*--------------------------------------------------------------------------*/

SOURCE get_hood_source( void )
{
    return status_sense_source.hood  ;
}

/*--------------------------------------------------------------------------*/

SOURCE get_trunk_source( void )
{
    return status_sense_source.trunk  ;
}

/*--------------------------------------------------------------------------*/

SOURCE get_ign_source( void )
{
    return status_sense_source.ign  ;
}

/*--------------------------------------------------------------------------*/

SOURCE get_brake_source( void )
{
    return status_sense_source.brake  ;
}

/*--------------------------------------------------------------------------*/

SOURCE get_handbrake_source( void )
{
    return status_sense_source.handbrake  ;
}

/*--------------------------------------------------------------------------*/

SOURCE get_rpm_source( void )
{
    return status_sense_source.rpm  ;
}

/*--------------------------------------------------------------------------*/

SOURCE get_glowplug_source( void )
{
    return status_sense_source.glowplug  ;
}

/*--------------------------------------------------------------------------*/

SOURCE get_headlight_source( void )
{
    return status_sense_source.headlight;
}
/*-------------------------------------------------------------------------*/
/* RKE USAGE GET                                                           */
/*-------------------------------------------------------------------------*/

RKE_USE get_rke_lock_use( void )
{
    return status_rke_use.lock ;
}

/*--------------------------------------------------------------------------*/

RKE_USE get_rke_unlock_use( void )
{
    return status_rke_use.unlock ;
}

/*--------------------------------------------------------------------------*/

RKE_USE get_rke_trunk_use( void )
{
    return status_rke_use.trunk ;
}

/*--------------------------------------------------------------------------*/

Boolean get_zone_bypass_status( void )
{
    return status_ss.zone_bypass_in_progress;
}


/*-------------------------------------------------------------------------*/
/* Special case: get_cmd_source_d2d(void)                                  */
/*-------------------------------------------------------------------------*/
Boolean get_cmd_source_d2d(void)
{
    return cmd_source_d2d;
}


/*--------------------------------------------------------------------------*/



