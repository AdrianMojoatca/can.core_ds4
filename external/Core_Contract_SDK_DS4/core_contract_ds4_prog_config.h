/*<#======================================================================#>*/
/*               CONTRACT WRAPPER PENTRU CONFIG_PROG.H (DS4)             */
/*<#======================================================================#>*/

#ifndef __CORE_CONTRACT_DS4_PROG_CONFIG_H__
#define __CORE_CONTRACT_DS4_PROG_CONFIG_H__

#include <stdint.h>

typedef struct {
    uint32_t ctl_doorlock_brake_enable;
    uint32_t ctl_doorlock_ignition_enable;
    uint32_t ctl_doorlock_speed_enable;
    uint32_t custom1_opt2_enable;
    uint32_t custom1_opt3_enable;
    uint32_t custom1_opt4_enable;
    uint32_t custom1_opt5_enable;
    uint32_t custom2_opt2_enable;
    uint32_t custom2_opt3_enable;
    uint32_t custom2_opt4_enable;
    uint32_t custom2_opt5_enable;
    uint32_t custom3_opt2_enable;
    uint32_t custom3_opt3_enable;
    uint32_t custom3_opt4_enable;
    uint32_t custom3_opt5_enable;
    uint32_t custom4_opt2_enable;
    uint32_t custom4_opt3_enable;
    uint32_t custom4_opt4_enable;
    uint32_t custom4_opt5_enable;
    uint32_t feat_auto_lock_enable;
    uint32_t feat_auto_lock_value;
    uint32_t feat_auto_unlock_enable;
    uint32_t feat_auto_unlock_value;
    uint32_t feat_aux1_default;
    uint32_t feat_aux1_enable;
    uint32_t feat_aux2_default;
    uint32_t feat_aux2_enable;
    uint32_t feat_aux3_default;
    uint32_t feat_aux3_enable;
    uint32_t feat_aux4_default;
    uint32_t feat_aux4_enable;
    uint32_t feat_comfort_closure_enable;
    uint32_t feat_comfort_closure_value;
    uint32_t feat_ctl_doorlock_enable;
    uint32_t feat_ctl_doorlock_value;
    uint32_t feat_custom1_default_value;
    uint32_t feat_custom1_enable;
    uint32_t feat_custom2_default_value;
    uint32_t feat_custom2_enable;
    uint32_t feat_custom3_default_value;
    uint32_t feat_custom3_enable;
    uint32_t feat_custom4_default_value;
    uint32_t feat_custom4_enable;
    uint32_t feat_defroster_enable;
    uint32_t feat_defroster_value;
    uint32_t feat_ext_light_supervision_enable;
    uint32_t feat_ext_light_supervision_value;
    uint32_t feat_heated_seats_enable;
    uint32_t feat_heated_seats_value;
    uint32_t feat_horn_confirmation_enable;
    uint32_t feat_horn_confirmation_value;
    uint32_t feat_key_ign_arm_defeat_enable;
    uint32_t feat_key_ign_arm_defeat_value;
    uint32_t feat_light_control_enable;
    uint32_t feat_light_control_value;
    uint32_t feat_oem_security_enable;
    uint32_t feat_oem_security_value;
    uint32_t feat_relock_enable;
    uint32_t feat_relock_value;
    uint32_t feat_rf_output_enable;
    uint32_t feat_rf_output_value;
    uint32_t feat_seats_temp_control_enable;
    uint32_t feat_seats_temp_control_value;
    uint32_t feat_seats_temp_level_enable;
    uint32_t feat_seats_temp_level_value;
    uint32_t feat_secure_lock_enable;
    uint32_t feat_secure_lock_value;
    uint32_t feat_trunk_default;
    uint32_t feat_trunk_enable;
    uint32_t feat_unlock_driver_enable;
    uint32_t feat_unlock_driver_value;
    uint32_t heated_seats_level_high_enable;
    uint32_t heated_seats_level_low_enable;
    uint32_t heated_seats_level_med_enable;
    uint32_t option_gas_cap_enable;
    uint32_t option_head_lights_enable;
    uint32_t option_left_slide_enable;
    uint32_t option_right_slide_enable;
    uint32_t option_sunroof_closure_enable;
    uint32_t option_tail_glass_enable;
    uint32_t option_trunk_enable;
    uint32_t option_windows_closure_enable;
    uint32_t option_windows_vent_enable;
    uint32_t option_audio_enable;
    uint32_t option_timed_enable;
    uint32_t option_latched_enable;
    uint32_t option_pulsed_enable;
    uint32_t option_garage_door_enable;
    uint32_t secure_lock_smart_enable;
} prog_config_contract_t;

#if defined(FW_DS4_BUILD)
/* SS4 Slice 2 single-instance: FW reads/writes CORE's single config instance
   through the API-table DATA pointer (no separate FW storage). */
prog_config_contract_t* core_ds4_cfg_prog(void);
#define g_prog_config (*core_ds4_cfg_prog())
#else
extern prog_config_contract_t g_prog_config;

#ifdef PROG_CONFIG_DEFINE_STORAGE
prog_config_contract_t g_prog_config;
#endif
#endif

#define PROG_CONFIG_FIELDS(X) \
    X(ctl_doorlock_brake_enable) \
    X(ctl_doorlock_ignition_enable) \
    X(ctl_doorlock_speed_enable) \
    X(custom1_opt2_enable) \
    X(custom1_opt3_enable) \
    X(custom1_opt4_enable) \
    X(custom1_opt5_enable) \
    X(custom2_opt2_enable) \
    X(custom2_opt3_enable) \
    X(custom2_opt4_enable) \
    X(custom2_opt5_enable) \
    X(custom3_opt2_enable) \
    X(custom3_opt3_enable) \
    X(custom3_opt4_enable) \
    X(custom3_opt5_enable) \
    X(custom4_opt2_enable) \
    X(custom4_opt3_enable) \
    X(custom4_opt4_enable) \
    X(custom4_opt5_enable) \
    X(feat_auto_lock_enable) \
    X(feat_auto_lock_value) \
    X(feat_auto_unlock_enable) \
    X(feat_auto_unlock_value) \
    X(feat_aux1_default) \
    X(feat_aux1_enable) \
    X(feat_aux2_default) \
    X(feat_aux2_enable) \
    X(feat_aux3_default) \
    X(feat_aux3_enable) \
    X(feat_aux4_default) \
    X(feat_aux4_enable) \
    X(feat_comfort_closure_enable) \
    X(feat_comfort_closure_value) \
    X(feat_ctl_doorlock_enable) \
    X(feat_ctl_doorlock_value) \
    X(feat_custom1_default_value) \
    X(feat_custom1_enable) \
    X(feat_custom2_default_value) \
    X(feat_custom2_enable) \
    X(feat_custom3_default_value) \
    X(feat_custom3_enable) \
    X(feat_custom4_default_value) \
    X(feat_custom4_enable) \
    X(feat_defroster_enable) \
    X(feat_defroster_value) \
    X(feat_ext_light_supervision_enable) \
    X(feat_ext_light_supervision_value) \
    X(feat_heated_seats_enable) \
    X(feat_heated_seats_value) \
    X(feat_horn_confirmation_enable) \
    X(feat_horn_confirmation_value) \
    X(feat_key_ign_arm_defeat_enable) \
    X(feat_key_ign_arm_defeat_value) \
    X(feat_light_control_enable) \
    X(feat_light_control_value) \
    X(feat_oem_security_enable) \
    X(feat_oem_security_value) \
    X(feat_relock_enable) \
    X(feat_relock_value) \
    X(feat_rf_output_enable) \
    X(feat_rf_output_value) \
    X(feat_seats_temp_control_enable) \
    X(feat_seats_temp_control_value) \
    X(feat_seats_temp_level_enable) \
    X(feat_seats_temp_level_value) \
    X(feat_secure_lock_enable) \
    X(feat_secure_lock_value) \
    X(feat_trunk_default) \
    X(feat_trunk_enable) \
    X(feat_unlock_driver_enable) \
    X(feat_unlock_driver_value) \
    X(heated_seats_level_high_enable) \
    X(heated_seats_level_low_enable) \
    X(heated_seats_level_med_enable) \
    X(option_gas_cap_enable) \
    X(option_head_lights_enable) \
    X(option_left_slide_enable) \
    X(option_right_slide_enable) \
    X(option_sunroof_closure_enable) \
    X(option_tail_glass_enable) \
    X(option_trunk_enable) \
    X(option_windows_closure_enable) \
    X(option_windows_vent_enable) \
    X(option_audio_enable) \
    X(option_timed_enable) \
    X(option_latched_enable) \
    X(option_pulsed_enable) \
    X(option_garage_door_enable) \
    X(secure_lock_smart_enable)

#define DECLARE_PROG_CONFIG_ACCESSORS(field) \
    static void set_prog_##field(uint32_t value) { g_prog_config.field = value; } \
    static uint32_t get_prog_##field(void) { return g_prog_config.field; }

PROG_CONFIG_FIELDS(DECLARE_PROG_CONFIG_ACCESSORS)

#define PROG_CONFIG_ENABLE_FIELDS(X) \
    X(ctl_doorlock_brake_enable) \
    X(ctl_doorlock_ignition_enable) \
    X(ctl_doorlock_speed_enable) \
    X(custom1_opt2_enable) \
    X(custom1_opt3_enable) \
    X(custom1_opt4_enable) \
    X(custom1_opt5_enable) \
    X(custom2_opt2_enable) \
    X(custom2_opt3_enable) \
    X(custom2_opt4_enable) \
    X(custom2_opt5_enable) \
    X(custom3_opt2_enable) \
    X(custom3_opt3_enable) \
    X(custom3_opt4_enable) \
    X(custom3_opt5_enable) \
    X(custom4_opt2_enable) \
    X(custom4_opt3_enable) \
    X(custom4_opt4_enable) \
    X(custom4_opt5_enable) \
    X(feat_auto_lock_enable) \
    X(feat_auto_unlock_enable) \
    X(feat_aux1_enable) \
    X(feat_aux2_enable) \
    X(feat_aux3_enable) \
    X(feat_aux4_enable) \
    X(feat_comfort_closure_enable) \
    X(feat_ctl_doorlock_enable) \
    X(feat_custom1_enable) \
    X(feat_custom2_enable) \
    X(feat_custom3_enable) \
    X(feat_custom4_enable) \
    X(feat_defroster_enable) \
    X(feat_ext_light_supervision_enable) \
    X(feat_heated_seats_enable) \
    X(feat_horn_confirmation_enable) \
    X(feat_key_ign_arm_defeat_enable) \
    X(feat_light_control_enable) \
    X(feat_oem_security_enable) \
    X(feat_relock_enable) \
    X(feat_rf_output_enable) \
    X(feat_seats_temp_control_enable) \
    X(feat_seats_temp_level_enable) \
    X(feat_secure_lock_enable) \
    X(feat_trunk_enable) \
    X(feat_unlock_driver_enable) \
    X(heated_seats_level_high_enable) \
    X(heated_seats_level_low_enable) \
    X(heated_seats_level_med_enable) \
    X(option_gas_cap_enable) \
    X(option_head_lights_enable) \
    X(option_left_slide_enable) \
    X(option_right_slide_enable) \
    X(option_sunroof_closure_enable) \
    X(option_tail_glass_enable) \
    X(option_trunk_enable) \
    X(option_windows_closure_enable) \
    X(option_windows_vent_enable) \
    X(option_audio_enable) \
    X(option_timed_enable) \
    X(option_latched_enable) \
    X(option_pulsed_enable) \
    X(option_garage_door_enable) \
    X(secure_lock_smart_enable)

#define DECLARE_PROG_CONFIG_ENABLE_DISABLE(field) \
    static void enable_prog_##field(void) { set_prog_##field(1U); } \
    static void disable_prog_##field(void) { set_prog_##field(0U); }

PROG_CONFIG_ENABLE_FIELDS(DECLARE_PROG_CONFIG_ENABLE_DISABLE)

#undef DECLARE_PROG_CONFIG_ENABLE_DISABLE

#define set_prog_feat_custom2_value(value) set_prog_feat_custom2_default_value((uint32_t)(value))
#define get_prog_feat_custom2_value() get_prog_feat_custom2_default_value()
#define set_prog_feat_custom3_value(value) set_prog_feat_custom3_default_value((uint32_t)(value))
#define get_prog_feat_custom3_value() get_prog_feat_custom3_default_value()
#define set_prog_feat_custom4_value(value) set_prog_feat_custom4_default_value((uint32_t)(value))
#define get_prog_feat_custom4_value() get_prog_feat_custom4_default_value()

#undef DECLARE_PROG_CONFIG_ACCESSORS

#define PROG_CFG_CTL_DOORLOCK_BRAKE_ENABLE (get_prog_ctl_doorlock_brake_enable())
#define PROG_CFG_CTL_DOORLOCK_IGNITION_ENABLE (get_prog_ctl_doorlock_ignition_enable())
#define PROG_CFG_CTL_DOORLOCK_SPEED_ENABLE (get_prog_ctl_doorlock_speed_enable())
#define PROG_CFG_CUSTOM1_OPT2_ENABLE (get_prog_custom1_opt2_enable())
#define PROG_CFG_CUSTOM1_OPT3_ENABLE (get_prog_custom1_opt3_enable())
#define PROG_CFG_CUSTOM1_OPT4_ENABLE (get_prog_custom1_opt4_enable())
#define PROG_CFG_CUSTOM1_OPT5_ENABLE (get_prog_custom1_opt5_enable())
#define PROG_CFG_CUSTOM2_OPT2_ENABLE (get_prog_custom2_opt2_enable())
#define PROG_CFG_CUSTOM2_OPT3_ENABLE (get_prog_custom2_opt3_enable())
#define PROG_CFG_CUSTOM2_OPT4_ENABLE (get_prog_custom2_opt4_enable())
#define PROG_CFG_CUSTOM2_OPT5_ENABLE (get_prog_custom2_opt5_enable())
#define PROG_CFG_CUSTOM3_OPT2_ENABLE (get_prog_custom3_opt2_enable())
#define PROG_CFG_CUSTOM3_OPT3_ENABLE (get_prog_custom3_opt3_enable())
#define PROG_CFG_CUSTOM3_OPT4_ENABLE (get_prog_custom3_opt4_enable())
#define PROG_CFG_CUSTOM3_OPT5_ENABLE (get_prog_custom3_opt5_enable())
#define PROG_CFG_CUSTOM4_OPT2_ENABLE (get_prog_custom4_opt2_enable())
#define PROG_CFG_CUSTOM4_OPT3_ENABLE (get_prog_custom4_opt3_enable())
#define PROG_CFG_CUSTOM4_OPT4_ENABLE (get_prog_custom4_opt4_enable())
#define PROG_CFG_CUSTOM4_OPT5_ENABLE (get_prog_custom4_opt5_enable())
#define PROG_CFG_FEAT_AUTO_LOCK_ENABLE (get_prog_feat_auto_lock_enable())
#define PROG_CFG_FEAT_AUTO_LOCK_VALUE (get_prog_feat_auto_lock_value())
#define PROG_CFG_FEAT_AUTO_UNLOCK_ENABLE (get_prog_feat_auto_unlock_enable())
#define PROG_CFG_FEAT_AUTO_UNLOCK_VALUE (get_prog_feat_auto_unlock_value())
#define PROG_CFG_FEAT_AUX1_DEFAULT (get_prog_feat_aux1_default())
#define PROG_CFG_FEAT_AUX1_ENABLE (get_prog_feat_aux1_enable())
#define PROG_CFG_FEAT_AUX2_DEFAULT (get_prog_feat_aux2_default())
#define PROG_CFG_FEAT_AUX2_ENABLE (get_prog_feat_aux2_enable())
#define PROG_CFG_FEAT_AUX3_DEFAULT (get_prog_feat_aux3_default())
#define PROG_CFG_FEAT_AUX3_ENABLE (get_prog_feat_aux3_enable())
#define PROG_CFG_FEAT_AUX4_DEFAULT (get_prog_feat_aux4_default())
#define PROG_CFG_FEAT_AUX4_ENABLE (get_prog_feat_aux4_enable())
#define PROG_CFG_FEAT_COMFORT_CLOSURE_ENABLE (get_prog_feat_comfort_closure_enable())
#define PROG_CFG_FEAT_COMFORT_CLOSURE_VALUE (get_prog_feat_comfort_closure_value())
#define PROG_CFG_FEAT_CTL_DOORLOCK_ENABLE (get_prog_feat_ctl_doorlock_enable())
#define PROG_CFG_FEAT_CTL_DOORLOCK_VALUE (get_prog_feat_ctl_doorlock_value())
#define PROG_CFG_FEAT_CUSTOM1_DEFAULT_VALUE (get_prog_feat_custom1_default_value())
#define PROG_CFG_FEAT_CUSTOM1_ENABLE (get_prog_feat_custom1_enable())
#define PROG_CFG_FEAT_CUSTOM2_DEFAULT_VALUE (get_prog_feat_custom2_default_value())
#define PROG_CFG_FEAT_CUSTOM2_ENABLE (get_prog_feat_custom2_enable())
#define PROG_CFG_FEAT_CUSTOM3_DEFAULT_VALUE (get_prog_feat_custom3_default_value())
#define PROG_CFG_FEAT_CUSTOM3_ENABLE (get_prog_feat_custom3_enable())
#define PROG_CFG_FEAT_CUSTOM4_DEFAULT_VALUE (get_prog_feat_custom4_default_value())
#define PROG_CFG_FEAT_CUSTOM4_ENABLE (get_prog_feat_custom4_enable())
#define PROG_CFG_FEAT_DEFROSTER_ENABLE (get_prog_feat_defroster_enable())
#define PROG_CFG_FEAT_DEFROSTER_VALUE (get_prog_feat_defroster_value())
#define PROG_CFG_FEAT_EXT_LIGHT_SUPERVISION_ENABLE (get_prog_feat_ext_light_supervision_enable())
#define PROG_CFG_FEAT_EXT_LIGHT_SUPERVISION_VALUE (get_prog_feat_ext_light_supervision_value())
#define PROG_CFG_FEAT_HEATED_SEATS_ENABLE (get_prog_feat_heated_seats_enable())
#define PROG_CFG_FEAT_HEATED_SEATS_VALUE (get_prog_feat_heated_seats_value())
#define PROG_CFG_FEAT_HORN_CONFIRMATION_ENABLE (get_prog_feat_horn_confirmation_enable())
#define PROG_CFG_FEAT_HORN_CONFIRMATION_VALUE (get_prog_feat_horn_confirmation_value())
#define PROG_CFG_FEAT_KEY_IGN_ARM_DEFEAT_ENABLE (get_prog_feat_key_ign_arm_defeat_enable())
#define PROG_CFG_FEAT_KEY_IGN_ARM_DEFEAT_VALUE (get_prog_feat_key_ign_arm_defeat_value())
#define PROG_CFG_FEAT_LIGHT_CONTROL_ENABLE (get_prog_feat_light_control_enable())
#define PROG_CFG_FEAT_LIGHT_CONTROL_VALUE (get_prog_feat_light_control_value())
#define PROG_CFG_FEAT_OEM_SECURITY_ENABLE (get_prog_feat_oem_security_enable())
#define PROG_CFG_FEAT_OEM_SECURITY_VALUE (get_prog_feat_oem_security_value())
#define PROG_CFG_FEAT_RELOCK_ENABLE (get_prog_feat_relock_enable())
#define PROG_CFG_FEAT_RELOCK_VALUE (get_prog_feat_relock_value())
#define PROG_CFG_FEAT_RF_OUTPUT_ENABLE (get_prog_feat_rf_output_enable())
#define PROG_CFG_FEAT_RF_OUTPUT_VALUE (get_prog_feat_rf_output_value())
#define PROG_CFG_FEAT_SEATS_TEMP_CONTROL_ENABLE (get_prog_feat_seats_temp_control_enable())
#define PROG_CFG_FEAT_SEATS_TEMP_CONTROL_VALUE (get_prog_feat_seats_temp_control_value())
#define PROG_CFG_FEAT_SEATS_TEMP_LEVEL_ENABLE (get_prog_feat_seats_temp_level_enable())
#define PROG_CFG_FEAT_SEATS_TEMP_LEVEL_VALUE (get_prog_feat_seats_temp_level_value())
#define PROG_CFG_FEAT_SECURE_LOCK_ENABLE (get_prog_feat_secure_lock_enable())
#define PROG_CFG_FEAT_SECURE_LOCK_VALUE (get_prog_feat_secure_lock_value())
#define PROG_CFG_FEAT_TRUNK_DEFAULT (get_prog_feat_trunk_default())
#define PROG_CFG_FEAT_TRUNK_ENABLE (get_prog_feat_trunk_enable())
#define PROG_CFG_FEAT_UNLOCK_DRIVER_ENABLE (get_prog_feat_unlock_driver_enable())
#define PROG_CFG_FEAT_UNLOCK_DRIVER_VALUE (get_prog_feat_unlock_driver_value())
#define PROG_CFG_HEATED_SEATS_LEVEL_HIGH_ENABLE (get_prog_heated_seats_level_high_enable())
#define PROG_CFG_HEATED_SEATS_LEVEL_LOW_ENABLE (get_prog_heated_seats_level_low_enable())
#define PROG_CFG_HEATED_SEATS_LEVEL_MED_ENABLE (get_prog_heated_seats_level_med_enable())
#define PROG_CFG_OPTION_GAS_CAP_ENABLE (get_prog_option_gas_cap_enable())
#define PROG_CFG_OPTION_HEAD_LIGHTS_ENABLE (get_prog_option_head_lights_enable())
#define PROG_CFG_OPTION_LEFT_SLIDE_ENABLE (get_prog_option_left_slide_enable())
#define PROG_CFG_OPTION_RIGHT_SLIDE_ENABLE (get_prog_option_right_slide_enable())
#define PROG_CFG_OPTION_SUNROOF_CLOSURE_ENABLE (get_prog_option_sunroof_closure_enable())
#define PROG_CFG_OPTION_TAIL_GLASS_ENABLE (get_prog_option_tail_glass_enable())
#define PROG_CFG_OPTION_TRUNK_ENABLE (get_prog_option_trunk_enable())
#define PROG_CFG_OPTION_WINDOWS_CLOSURE_ENABLE (get_prog_option_windows_closure_enable())
#define PROG_CFG_OPTION_WINDOWS_VENT_ENABLE (get_prog_option_windows_vent_enable())
#define PROG_CFG_OPTION_AUDIO_ENABLE (get_prog_option_audio_enable())
#define PROG_CFG_OPTION_TIMED_ENABLE (get_prog_option_timed_enable())
#define PROG_CFG_OPTION_LATCHED_ENABLE (get_prog_option_latched_enable())
#define PROG_CFG_OPTION_PULSED_ENABLE (get_prog_option_pulsed_enable())
#define PROG_CFG_OPTION_GARAGE_DOOR_ENABLE (get_prog_option_garage_door_enable())
#define PROG_CFG_SECURE_LOCK_SMART_ENABLE (get_prog_secure_lock_smart_enable())

void core_contract_ds4_prog_config_init_defaults(void);
void core_contract_ds4_prog_config_init_vehicle_specific(void);

#endif /* __CORE_CONTRACT_DS4_PROG_CONFIG_H__ */
