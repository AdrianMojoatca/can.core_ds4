/*<#======================================================================#>*/
/*              CONTRACT WRAPPER PENTRU CONFIG_OBD_CAN.H (DS4)              */
/*<#======================================================================#>*/

#ifndef __OBD_CAN_CONFIG_WRAPPER_H__
#define __OBD_CAN_CONFIG_WRAPPER_H__

#include <stdint.h>

#define __CONFIG_OBD_CAN_H__

#define CORE_CONFIG_OBD_CAN_VERSION_DEFAULT          (2U)
#define CORE_OBD_CAN_VIN_MASK_DEFAULT                (1U)
#define CORE_OBD_CAN_ENG_COOLANT_MASK_DEFAULT        (1U)
#define CORE_OBD_CAN_RPM_MASK_DEFAULT                (0U)
#define CORE_OBD_CAN_SPEED_MASK_DEFAULT              (1U)
#define CORE_OBD_CAN_ENG_RUN_MASK_DEFAULT            (1U)
#define CORE_OBD_CAN_FUEL_LEVEL_MASK_DEFAULT         (1U)
#define CORE_OBD_CAN_AIR_AMB_MASK_DEFAULT            (1U)
#define CORE_OBD_CAN_INTAKE_TEMP_MASK_DEFAULT        (1U)
#define CORE_OBD_CAN_BAT_VOLT_MASK_DEFAULT           (1U)
#define CORE_OBD_CAN_DTC_CODE_MASK_DEFAULT           (1U)
#define CORE_OBD_CAN_DTC_CLR_MASK_DEFAULT            (1U)
#define CORE_OBD_CAN_REQ_RETRIES_DEFAULT             (5U)
#define CORE_OBD_CAN_BAUDRATE_DEFAULT                (500U)
#define CORE_OBD_CAN_IGN_DELAY_DEFAULT               (6U)

#define CONFIG_OBD_CAN_VERSION   CORE_CONFIG_OBD_CAN_VERSION_DEFAULT
#define OBD_CAN_VIN_MASK         CORE_OBD_CAN_VIN_MASK_DEFAULT
#define OBD_CAN_ENG_COOLANT_MASK CORE_OBD_CAN_ENG_COOLANT_MASK_DEFAULT
#define OBD_CAN_RPM_MASK         CORE_OBD_CAN_RPM_MASK_DEFAULT
#define OBD_CAN_SPEED_MASK       CORE_OBD_CAN_SPEED_MASK_DEFAULT
#define OBD_CAN_ENG_RUN_MASK     CORE_OBD_CAN_ENG_RUN_MASK_DEFAULT
#define OBD_CAN_FUEL_LEVEL_MASK  CORE_OBD_CAN_FUEL_LEVEL_MASK_DEFAULT
#define OBD_CAN_AIR_AMB_MASK     CORE_OBD_CAN_AIR_AMB_MASK_DEFAULT
#define OBD_CAN_INTAKE_TEMP_MASK CORE_OBD_CAN_INTAKE_TEMP_MASK_DEFAULT
#define OBD_CAN_BAT_VOLT_MASK    CORE_OBD_CAN_BAT_VOLT_MASK_DEFAULT
#define OBD_CAN_DTC_CODE_MASK    CORE_OBD_CAN_DTC_CODE_MASK_DEFAULT
#define OBD_CAN_DTC_CLR_MASK     CORE_OBD_CAN_DTC_CLR_MASK_DEFAULT
#define OBD_CAN_REQ_RETRIES      CORE_OBD_CAN_REQ_RETRIES_DEFAULT
#define OBD_CAN_BAUDRATE         CORE_OBD_CAN_BAUDRATE_DEFAULT
#define OBD_CAN_IGN_DELAY        CORE_OBD_CAN_IGN_DELAY_DEFAULT

typedef struct {
    uint32_t config_obd_can_version;
    uint32_t obd_can_vin_mask;
    uint32_t obd_can_eng_coolant_mask;
    uint32_t obd_can_rpm_mask;
    uint32_t obd_can_speed_mask;
    uint32_t obd_can_eng_run_mask;
    uint32_t obd_can_fuel_level_mask;
    uint32_t obd_can_air_amb_mask;
    uint32_t obd_can_intake_temp_mask;
    uint32_t obd_can_bat_volt_mask;
    uint32_t obd_can_dtc_code_mask;
    uint32_t obd_can_dtc_clr_mask;
    uint32_t obd_can_req_retries;
    uint32_t obd_can_baudrate;
    uint32_t obd_can_ign_delay;
} obd_can_config_contract_t;

extern obd_can_config_contract_t g_obd_can_config;

#ifdef OBD_CAN_CONFIG_DEFINE_STORAGE
obd_can_config_contract_t g_obd_can_config;
#endif

#define set_config_obd_can_version(value) (g_obd_can_config.config_obd_can_version = (uint32_t)(value))
#define get_config_obd_can_version() (g_obd_can_config.config_obd_can_version)
#define set_obd_can_vin_mask(value) (g_obd_can_config.obd_can_vin_mask = (uint32_t)(value))
#define get_obd_can_vin_mask() (g_obd_can_config.obd_can_vin_mask)
#define set_obd_can_eng_coolant_mask(value) (g_obd_can_config.obd_can_eng_coolant_mask = (uint32_t)(value))
#define get_obd_can_eng_coolant_mask() (g_obd_can_config.obd_can_eng_coolant_mask)
#define set_obd_can_rpm_mask(value) (g_obd_can_config.obd_can_rpm_mask = (uint32_t)(value))
#define get_obd_can_rpm_mask() (g_obd_can_config.obd_can_rpm_mask)
#define set_obd_can_speed_mask(value) (g_obd_can_config.obd_can_speed_mask = (uint32_t)(value))
#define get_obd_can_speed_mask() (g_obd_can_config.obd_can_speed_mask)
#define set_obd_can_eng_run_mask(value) (g_obd_can_config.obd_can_eng_run_mask = (uint32_t)(value))
#define get_obd_can_eng_run_mask() (g_obd_can_config.obd_can_eng_run_mask)
#define set_obd_can_fuel_level_mask(value) (g_obd_can_config.obd_can_fuel_level_mask = (uint32_t)(value))
#define get_obd_can_fuel_level_mask() (g_obd_can_config.obd_can_fuel_level_mask)
#define set_obd_can_air_amb_mask(value) (g_obd_can_config.obd_can_air_amb_mask = (uint32_t)(value))
#define get_obd_can_air_amb_mask() (g_obd_can_config.obd_can_air_amb_mask)
#define set_obd_can_intake_temp_mask(value) (g_obd_can_config.obd_can_intake_temp_mask = (uint32_t)(value))
#define get_obd_can_intake_temp_mask() (g_obd_can_config.obd_can_intake_temp_mask)
#define set_obd_can_bat_volt_mask(value) (g_obd_can_config.obd_can_bat_volt_mask = (uint32_t)(value))
#define get_obd_can_bat_volt_mask() (g_obd_can_config.obd_can_bat_volt_mask)
#define set_obd_can_dtc_code_mask(value) (g_obd_can_config.obd_can_dtc_code_mask = (uint32_t)(value))
#define get_obd_can_dtc_code_mask() (g_obd_can_config.obd_can_dtc_code_mask)
#define set_obd_can_dtc_clr_mask(value) (g_obd_can_config.obd_can_dtc_clr_mask = (uint32_t)(value))
#define get_obd_can_dtc_clr_mask() (g_obd_can_config.obd_can_dtc_clr_mask)
#define set_obd_can_req_retries(value) (g_obd_can_config.obd_can_req_retries = (uint32_t)(value))
#define get_obd_can_req_retries() (g_obd_can_config.obd_can_req_retries)
#define set_obd_can_baudrate(value) (g_obd_can_config.obd_can_baudrate = (uint32_t)(value))
#define get_obd_can_baudrate() (g_obd_can_config.obd_can_baudrate)
#define set_obd_can_ign_delay(value) (g_obd_can_config.obd_can_ign_delay = (uint32_t)(value))
#define get_obd_can_ign_delay() (g_obd_can_config.obd_can_ign_delay)

#define obd_can_config_set_defaults_from_defines() do { \
    set_config_obd_can_version((uint32_t)(CORE_CONFIG_OBD_CAN_VERSION_DEFAULT)); \
    set_obd_can_vin_mask((uint32_t)(CORE_OBD_CAN_VIN_MASK_DEFAULT)); \
    set_obd_can_eng_coolant_mask((uint32_t)(CORE_OBD_CAN_ENG_COOLANT_MASK_DEFAULT)); \
    set_obd_can_rpm_mask((uint32_t)(CORE_OBD_CAN_RPM_MASK_DEFAULT)); \
    set_obd_can_speed_mask((uint32_t)(CORE_OBD_CAN_SPEED_MASK_DEFAULT)); \
    set_obd_can_eng_run_mask((uint32_t)(CORE_OBD_CAN_ENG_RUN_MASK_DEFAULT)); \
    set_obd_can_fuel_level_mask((uint32_t)(CORE_OBD_CAN_FUEL_LEVEL_MASK_DEFAULT)); \
    set_obd_can_air_amb_mask((uint32_t)(CORE_OBD_CAN_AIR_AMB_MASK_DEFAULT)); \
    set_obd_can_intake_temp_mask((uint32_t)(CORE_OBD_CAN_INTAKE_TEMP_MASK_DEFAULT)); \
    set_obd_can_bat_volt_mask((uint32_t)(CORE_OBD_CAN_BAT_VOLT_MASK_DEFAULT)); \
    set_obd_can_dtc_code_mask((uint32_t)(CORE_OBD_CAN_DTC_CODE_MASK_DEFAULT)); \
    set_obd_can_dtc_clr_mask((uint32_t)(CORE_OBD_CAN_DTC_CLR_MASK_DEFAULT)); \
    set_obd_can_req_retries((uint32_t)(CORE_OBD_CAN_REQ_RETRIES_DEFAULT)); \
    set_obd_can_baudrate((uint32_t)(CORE_OBD_CAN_BAUDRATE_DEFAULT)); \
    set_obd_can_ign_delay((uint32_t)(CORE_OBD_CAN_IGN_DELAY_DEFAULT));  \
} while(0)

#define OBD_CAN_CFG_CONFIG_OBD_CAN_VERSION (get_config_obd_can_version())
#define OBD_CAN_CFG_OBD_CAN_VIN_MASK (get_obd_can_vin_mask())
#define OBD_CAN_CFG_OBD_CAN_ENG_COOLANT_MASK (get_obd_can_eng_coolant_mask())
#define OBD_CAN_CFG_OBD_CAN_RPM_MASK (get_obd_can_rpm_mask())
#define OBD_CAN_CFG_OBD_CAN_SPEED_MASK (get_obd_can_speed_mask())
#define OBD_CAN_CFG_OBD_CAN_ENG_RUN_MASK (get_obd_can_eng_run_mask())
#define OBD_CAN_CFG_OBD_CAN_FUEL_LEVEL_MASK (get_obd_can_fuel_level_mask())
#define OBD_CAN_CFG_OBD_CAN_AIR_AMB_MASK (get_obd_can_air_amb_mask())
#define OBD_CAN_CFG_OBD_CAN_INTAKE_TEMP_MASK (get_obd_can_intake_temp_mask())
#define OBD_CAN_CFG_OBD_CAN_BAT_VOLT_MASK (get_obd_can_bat_volt_mask())
#define OBD_CAN_CFG_OBD_CAN_DTC_CODE_MASK (get_obd_can_dtc_code_mask())
#define OBD_CAN_CFG_OBD_CAN_DTC_CLR_MASK (get_obd_can_dtc_clr_mask())
#define OBD_CAN_CFG_OBD_CAN_REQ_RETRIES (get_obd_can_req_retries())
#define OBD_CAN_CFG_OBD_CAN_BAUDRATE (get_obd_can_baudrate())
#define OBD_CAN_CFG_OBD_CAN_IGN_DELAY (get_obd_can_ign_delay())

void core_contract_ds4_obd_can_config_init_defaults(void);
void core_contract_ds4_obd_can_config_init_vehicle_specific(void);

#endif /* __OBD_CAN_CONFIG_WRAPPER_H__ */
