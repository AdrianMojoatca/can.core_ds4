/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: fw_init.c 15282 2012-06-12 13:29:20Z louis-philippe.rispoli $
/*==========================================================================*/


/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "fw.h"
//#include "prog_private.h"
//#include "key.h"
#include "detect_private.h"
#include "build_identity.h"
#include "core_contract_db3_firmware_config.h"
#include "core_contract_db3_can_config.h"
#include "core_contract_db3_gpio_config.h"
#include "core_contract_db3_pts_ctl_config.h"
#include "core_contract_db3_status_config.h"
#include "core_contract_db3_prog_config.h"
#include "core_contract_db3_prog_option_defs.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void fw_options_set( void );
static void fw_prog_vehicle_specific_enable_apply( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Func_Cfg func_cfg ;  
//extern Frd_Car      frd_car                     ;

//extern Detect_Vars     detect_vars  ;      
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void core_contract_db3_pts_ctl_config_apply_vehicle_specific(void)
{
    set_config_pts_ctl_version(1U);
    set_pts_ctl_timeout(750U);
    set_pts_ctl_pulse_timeout(4000U);
}

/*--------------------------------------------------------------------------*/

void core_contract_db3_status_config_apply_vehicle_specific(void)
{
    set_status_vin_enable(1U);
    set_status_rke_enable(1U);
    set_status_key_enable(1U);
    set_status_rpm_enable(1U);
    set_status_door_enable(1U);
    set_status_speed_enable(1U);
    set_status_brake_enable(1U);
    set_status_trans_enable(1U);
    set_status_alarm_enable(1U);
    set_status_engine_enable(1U);
    set_status_comfort_enable(1U);
    set_status_temperature_enable(1U);
    set_status_req_input_enable(1U);
    set_status_dtc_enable(1U);
    set_status_mileage_enable(1U);
    set_status_tpms_enable(1U);
    set_status_doorlock_enable(1U);

    set_status_oil_alarm_enable(1U);
    set_status_tire_alarm_enable(1U);
    set_status_seat_belts_enable(1U);
    set_status_seat_weights_enable(1U);
    set_status_ev_battery_lvl_enable(1U);
    set_status_ev_range_enable(1U);
    set_status_oil_life_enable(1U);
    set_status_fuel_lvl_enable(1U);
    set_status_fuel_cap_enable(1U);
    set_status_cabin_temp_enable(1U);
    set_status_tire_pressure_enable(1U);
    set_status_odometer_enable(1U);
    set_status_battery_voltage_enable(1U);

    set_function_honks_enable(1U);
    set_function_headlights_enable(1U);
    set_function_rib_enable(1U);
}

/*--------------------------------------------------------------------------*/

void core_contract_db3_prog_config_apply_vehicle_specific(void)
{
    /* Controlled door lock option flags */
    set_prog_ctl_doorlock_brake_enable(1U);
    set_prog_ctl_doorlock_ignition_enable(1U);
    set_prog_ctl_doorlock_speed_enable(1U);

    /* Custom1 available options */
    set_prog_custom1_opt2_enable(1U);
    set_prog_custom1_opt3_enable(1U);
    set_prog_custom1_opt4_enable(1U);
    set_prog_custom1_opt5_enable(0U);

    /* Custom2 available options */
    set_prog_custom2_opt2_enable(0U);
    set_prog_custom2_opt3_enable(0U);
    set_prog_custom2_opt4_enable(0U);
    set_prog_custom2_opt5_enable(0U);

    /* Custom3 available options */
    set_prog_custom3_opt2_enable(0U);
    set_prog_custom3_opt3_enable(0U);
    set_prog_custom3_opt4_enable(0U);
    set_prog_custom3_opt5_enable(0U);

    /* Custom4 available options */
    set_prog_custom4_opt2_enable(1U);
    set_prog_custom4_opt3_enable(1U);
    set_prog_custom4_opt4_enable(1U);
    set_prog_custom4_opt5_enable(1U);

    /* Feature gate + selected value */
    set_prog_feat_auto_lock_enable(0U);
    set_prog_feat_auto_lock_value(1U);

    set_prog_feat_auto_unlock_enable(0U);
    set_prog_feat_auto_unlock_value(1U);

    set_prog_feat_aux1_default(8U);
    set_prog_feat_aux1_enable(0U);

    set_prog_feat_aux2_default(7U);
    set_prog_feat_aux2_enable(0U);

    set_prog_feat_aux3_default(4U);
    set_prog_feat_aux3_enable(0U);

    set_prog_feat_aux4_default(7U);
    set_prog_feat_aux4_enable(0U);

    /* Comfort closure */
    set_prog_feat_comfort_closure_enable(0U);
    set_prog_feat_comfort_closure_value(1U);

    /* Controlled door lock */
    set_prog_feat_ctl_doorlock_enable(0U);
    set_prog_feat_ctl_doorlock_value(1U);

    /* Takeover */
    set_prog_feat_custom1_default_value(1U);
    set_prog_feat_custom1_enable(0U);

    /* Digital Aux1 */
    set_prog_feat_custom2_value(1U);
    set_prog_feat_custom2_enable(0U);

    /* Digital Aux2 */
    set_prog_feat_custom3_value(1U);
    set_prog_feat_custom3_enable(0U);

    /* Digital Aux3 */
    set_prog_feat_custom4_value(1U);
    set_prog_feat_custom4_enable(0U);

    set_prog_feat_defroster_enable(0U);
    set_prog_feat_defroster_value(1U);

    set_prog_feat_ext_light_supervision_enable(0U);
    set_prog_feat_ext_light_supervision_value(1U);

    set_prog_feat_heated_seats_enable(0U);
    set_prog_feat_heated_seats_value(1U);

    set_prog_feat_horn_confirmation_enable(0U);
    set_prog_feat_horn_confirmation_value(1U);

    set_prog_feat_key_ign_arm_defeat_enable(0U);
    set_prog_feat_key_ign_arm_defeat_value(2U);

    set_prog_feat_light_control_enable(0U);
    set_prog_feat_light_control_value(1U);

    set_prog_feat_oem_security_enable(0U);
    set_prog_feat_oem_security_value(1U);

    set_prog_feat_relock_enable(0U);
    set_prog_feat_relock_value(1U);

    set_prog_feat_rf_output_enable(1U);
    set_prog_feat_rf_output_value(1U);

    set_prog_feat_seats_temp_control_enable(0U);
    set_prog_feat_seats_temp_control_value(1U);

    set_prog_feat_seats_temp_level_enable(0U);
    set_prog_feat_seats_temp_level_value(1U);

    set_prog_feat_secure_lock_enable(0U);
    set_prog_feat_secure_lock_value(3U);

    set_prog_feat_trunk_default(0U);
    set_prog_feat_trunk_enable(0U);

    /* Unlock driver priority */
    set_prog_feat_unlock_driver_enable(0U);
    set_prog_feat_unlock_driver_value(1U);

    /* Heated seats level options */
    set_prog_heated_seats_level_high_enable(0U);
    set_prog_heated_seats_level_low_enable(0U);
    set_prog_heated_seats_level_med_enable(0U);

    /* Trunk/Aux available outputs */
    set_prog_option_gas_cap_enable(0U);
    set_prog_option_head_lights_enable(0U);
    set_prog_option_left_slide_enable(0U);
    set_prog_option_right_slide_enable(0U);
    set_prog_option_sunroof_closure_enable(0U);
    set_prog_option_tail_glass_enable(0U);
    set_prog_option_trunk_enable(0U);
    set_prog_option_windows_closure_enable(0U);
    set_prog_option_windows_vent_enable(0U);
    set_prog_option_audio_enable(0U);
    set_prog_option_timed_enable(0U);
    set_prog_option_latched_enable(0U);
    set_prog_option_pulsed_enable(0U);
    set_prog_option_garage_door_enable(0U);

    /* Smart OEM alarm control */
    set_prog_secure_lock_smart_enable(1U);
}

/*--------------------------------------------------------------------------*/

void fw_identity_init(void)
{
    core_contract_db3_firmware_config_init_defaults();
    core_contract_db3_pts_ctl_config_init_defaults();
    set_fw_name(BUILD_FW_NAME);
    set_app_version(BUILD_FW_VERSION);
    set_fw_info(BUILD_FW_INFO);

    set_can_dev_0_ft_sw(1);
    set_can_dev_0_speed(6);
    set_can_dev_0_rx_buffer_size(8);
    set_can_dev_0_tx_buffer_size(8); 
}

/*--------------------------------------------------------------------------*/

void fw_init( void )
{	

	TRACE("\r FW INIT!");

	if( detect_car_search() )
    {
        TRACE("\r\n CAR DETECTED \r\n");

		car.bits.can0_detected = TRUE;
        
        if( car.bits.can0_detected ) func_cfg.can_dev_id = CAN_DEV_0 ;

		func_init();

        prog_set_exit_callback((Prog_Func)fw_options_set);

        fw_options_set();
    }

}

/*--------------------------------------------------------------------------*/

void fw_options_set( void )
{
    UInt8 takeover_option;

    fw_prog_vehicle_specific_enable_apply();

    func_cfg.comfort = (UInt8)get_prog_feat_comfort_closure_value();

    takeover_option = (UInt8)get_prog_feat_custom1_default_value();

    if (get_prog_feat_custom1_enable() == 0U)
    {
        tsk_rs_update_cfg(TSK_RS_TAKEOVER_DISABLE);
    }
    else
    {
        switch (takeover_option)
        {
            case PROG_TAKEOVER_ENGINE_SHUTDOWN_DOOR_OPEN:
                tsk_rs_update_cfg(TSK_RS_TAKEOVER_DISABLE);
                break;
            case PROG_TAKEOVER_ENABLED_BY_OEM_REMOTE:
                tsk_rs_update_cfg(TSK_RS_TAKEOVER_OEM);
                break;
            case PROG_TAKEOVER_ENABLED_BY_OEM_OR_AFTERMARKET:
                tsk_rs_update_cfg(TSK_RS_TAKEOVER_AFTERMARKET | TSK_RS_TAKEOVER_OEM);
                break;
            case PROG_TAKEOVER_SKIP_DOOR_SHUTDOWN:
                tsk_rs_update_cfg(TSK_RS_SKIP_DOOR_SHUTDOWN);
                break;
            default:
                tsk_rs_update_cfg(TSK_RS_TAKEOVER_DISABLE);
                break;
        }
    }

    TRACE("\r\n[FW][PROG] custom1_en=%u custom1_val=%u\r\n",
          (unsigned int)get_prog_feat_custom1_enable(),
          (unsigned int)takeover_option);
        
    func_cfg.aux1    = (UInt8)get_prog_feat_custom2_value();
    func_cfg.aux2    = (UInt8)get_prog_feat_custom3_value();
    func_cfg.aux3    = (UInt8)get_prog_feat_custom4_value();
}	

/*--------------------------------------------------------------------------*/

static void fw_prog_vehicle_specific_enable_apply( void )
{
#define APPLY_VS_ENABLE_STATE(field) \
    do { \
        if (get_prog_##field() != 0U) { \
            enable_prog_##field(); \
        } else { \
            disable_prog_##field(); \
        } \
    } while (0);

    PROG_CONFIG_ENABLE_FIELDS(APPLY_VS_ENABLE_STATE);

#undef APPLY_VS_ENABLE_STATE
}

/*--------------------------------------------------------------------------*/



