/*<#======================================================================#>*/
/*              CONTRACT WRAPPER PENTRU CONFIG_STATUS.H (DS4)              */
/*<#======================================================================#>*/

#ifndef __STATUS_CONFIG_WRAPPER_H__
#define __STATUS_CONFIG_WRAPPER_H__

#include <stdint.h>

#define __CONFIG_STATUS_H__

#define CORE_CONFIG_STATUS_VERSION_2_00

#define CORE_STATUS_VIN_ENABLE               (1U)
#define CORE_STATUS_RKE_ENABLE               (1U)
#define CORE_STATUS_KEY_ENABLE               (1U)
#define CORE_STATUS_RPM_ENABLE               (1U)
#define CORE_STATUS_DOOR_ENABLE              (1U)
#define CORE_STATUS_SPEED_ENABLE             (1U)
#define CORE_STATUS_BRAKE_ENABLE             (1U)
#define CORE_STATUS_TRANS_ENABLE             (1U)
#define CORE_STATUS_ALARM_ENABLE             (1U)
#define CORE_STATUS_ENGINE_ENABLE            (1U)
#define CORE_STATUS_COMFORT_ENABLE           (1U)
#define CORE_STATUS_TEMPERATURE_ENABLE       (1U)
#define CORE_STATUS_REQ_INPUT_ENABLE         (1U)
#define CORE_STATUS_DTC_ENABLE               (1U)
#define CORE_STATUS_MILEAGE_ENABLE           (1U)
#define CORE_STATUS_TPMS_ENABLE              (1U)
#define CORE_STATUS_DOORLOCK_ENABLE          (1U)

#define CORE_STATUS_OIL_ALARM_ENABLE         (1U)
#define CORE_STATUS_TIRE_ALARM_ENABLE        (1U)
#define CORE_STATUS_SEAT_BELTS_ENABLE        (1U)
#define CORE_STATUS_SEAT_WEIGHTS_ENABLE      (1U)
#define CORE_STATUS_EV_BATTERY_LVL_ENABLE    (1U)
#define CORE_STATUS_EV_RANGE_ENABLE          (1U)
#define CORE_STATUS_OIL_LIFE_ENABLE          (1U)
#define CORE_STATUS_FUEL_LVL_ENABLE          (1U)
#define CORE_STATUS_FUEL_CAP_ENABLE          (1U)
#define CORE_STATUS_CABIN_TEMP_ENABLE        (1U)
#define CORE_STATUS_TIRE_PRESSURE_ENABLE     (1U)
#define CORE_STATUS_ODOMETER_ENABLE          (1U)
#define CORE_STATUS_BATTERY_VOLTAGE_ENABLE   (1U)

#define CORE_FUNCTION_HONKS_ENABLE           (1U)
#define CORE_FUNCTION_HEADLIGHTS_ENABLE      (1U)
#define CORE_FUNCTION_RIB_ENABLE             (1U)

#define STATUS_VIN_ENABLE              CORE_STATUS_VIN_ENABLE
#define STATUS_RKE_ENABLE              CORE_STATUS_RKE_ENABLE
#define STATUS_KEY_ENABLE              CORE_STATUS_KEY_ENABLE
#define STATUS_RPM_ENABLE              CORE_STATUS_RPM_ENABLE
#define STATUS_DOOR_ENABLE             CORE_STATUS_DOOR_ENABLE
#define STATUS_SPEED_ENABLE            CORE_STATUS_SPEED_ENABLE
#define STATUS_BRAKE_ENABLE            CORE_STATUS_BRAKE_ENABLE
#define STATUS_TRANS_ENABLE            CORE_STATUS_TRANS_ENABLE
#define STATUS_ALARM_ENABLE            CORE_STATUS_ALARM_ENABLE
#define STATUS_ENGINE_ENABLE           CORE_STATUS_ENGINE_ENABLE
#define STATUS_COMFORT_ENABLE          CORE_STATUS_COMFORT_ENABLE
#define STATUS_TEMPERATURE_ENABLE      CORE_STATUS_TEMPERATURE_ENABLE
#define STATUS_REQ_INPUT_ENABLE        CORE_STATUS_REQ_INPUT_ENABLE
#define STATUS_DTC_ENABLE              CORE_STATUS_DTC_ENABLE
#define STATUS_MILEAGE_ENABLE          CORE_STATUS_MILEAGE_ENABLE
#define STATUS_TPMS_ENABLE             CORE_STATUS_TPMS_ENABLE
#define STATUS_DOORLOCK_ENABLE         CORE_STATUS_DOORLOCK_ENABLE

#define STATUS_OIL_ALARM_ENABLE        CORE_STATUS_OIL_ALARM_ENABLE
#define STATUS_TIRE_ALARM_ENABLE       CORE_STATUS_TIRE_ALARM_ENABLE
#define STATUS_SEAT_BELTS_ENABLE       CORE_STATUS_SEAT_BELTS_ENABLE
#define STATUS_SEAT_WEIGHTS_ENABLE     CORE_STATUS_SEAT_WEIGHTS_ENABLE
#define STATUS_EV_BATTERY_LVL_ENABLE   CORE_STATUS_EV_BATTERY_LVL_ENABLE
#define STATUS_EV_RANGE_ENABLE         CORE_STATUS_EV_RANGE_ENABLE
#define STATUS_OIL_LIFE_ENABLE         CORE_STATUS_OIL_LIFE_ENABLE
#define STATUS_FUEL_LVL_ENABLE         CORE_STATUS_FUEL_LVL_ENABLE
#define STATUS_FUEL_CAP_ENABLE         CORE_STATUS_FUEL_CAP_ENABLE
#define STATUS_CABIN_TEMP_ENABLE       CORE_STATUS_CABIN_TEMP_ENABLE
#define STATUS_TIRE_PRESSURE_ENABLE    CORE_STATUS_TIRE_PRESSURE_ENABLE
#define STATUS_ODOMETER_ENABLE         CORE_STATUS_ODOMETER_ENABLE
#define STATUS_BATTERY_VOLTAGE_ENABLE  CORE_STATUS_BATTERY_VOLTAGE_ENABLE

#define FUNCTION_HONKS_ENABLE          CORE_FUNCTION_HONKS_ENABLE
#define FUNCTION_HEADLIGHTS_ENABLE     CORE_FUNCTION_HEADLIGHTS_ENABLE
#define FUNCTION_RIB_ENABLE            CORE_FUNCTION_RIB_ENABLE

typedef struct {
    uint32_t status_vin_enable;
    uint32_t status_rke_enable;
    uint32_t status_key_enable;
    uint32_t status_rpm_enable;
    uint32_t status_door_enable;
    uint32_t status_speed_enable;
    uint32_t status_brake_enable;
    uint32_t status_trans_enable;
    uint32_t status_alarm_enable;
    uint32_t status_engine_enable;
    uint32_t status_comfort_enable;
    uint32_t status_temperature_enable;
    uint32_t status_req_input_enable;
    uint32_t status_dtc_enable;
    uint32_t status_mileage_enable;
    uint32_t status_tpms_enable;
    uint32_t status_doorlock_enable;
    uint32_t status_oil_alarm_enable;
    uint32_t status_tire_alarm_enable;
    uint32_t status_seat_belts_enable;
    uint32_t status_seat_weights_enable;
    uint32_t status_ev_battery_lvl_enable;
    uint32_t status_ev_range_enable;
    uint32_t status_oil_life_enable;
    uint32_t status_fuel_lvl_enable;
    uint32_t status_fuel_cap_enable;
    uint32_t status_cabin_temp_enable;
    uint32_t status_tire_pressure_enable;
    uint32_t status_odometer_enable;
    uint32_t status_battery_voltage_enable;
    uint32_t function_honks_enable;
    uint32_t function_headlights_enable;
    uint32_t function_rib_enable;
} status_config_contract_t;

#if defined(FW_DS4_BUILD)
/* SS4 Slice 2 single-instance: FW reads/writes CORE's single config instance
   through the API-table DATA pointer (no separate FW storage). */
status_config_contract_t* core_ds4_cfg_status(void);
#define g_status_config (*core_ds4_cfg_status())
#else
extern status_config_contract_t g_status_config;

#ifdef STATUS_CONFIG_DEFINE_STORAGE
status_config_contract_t g_status_config;
#endif
#endif

#define set_status_vin_enable(value) (g_status_config.status_vin_enable = (uint32_t)(value))
#define get_status_vin_enable() (g_status_config.status_vin_enable)
#define set_status_rke_enable(value) (g_status_config.status_rke_enable = (uint32_t)(value))
#define get_status_rke_enable() (g_status_config.status_rke_enable)
#define set_status_key_enable(value) (g_status_config.status_key_enable = (uint32_t)(value))
#define get_status_key_enable() (g_status_config.status_key_enable)
#define set_status_rpm_enable(value) (g_status_config.status_rpm_enable = (uint32_t)(value))
#define get_status_rpm_enable() (g_status_config.status_rpm_enable)
#define set_status_door_enable(value) (g_status_config.status_door_enable = (uint32_t)(value))
#define get_status_door_enable() (g_status_config.status_door_enable)
#define set_status_speed_enable(value) (g_status_config.status_speed_enable = (uint32_t)(value))
#define get_status_speed_enable() (g_status_config.status_speed_enable)
#define set_status_brake_enable(value) (g_status_config.status_brake_enable = (uint32_t)(value))
#define get_status_brake_enable() (g_status_config.status_brake_enable)
#define set_status_trans_enable(value) (g_status_config.status_trans_enable = (uint32_t)(value))
#define get_status_trans_enable() (g_status_config.status_trans_enable)
#define set_status_alarm_enable(value) (g_status_config.status_alarm_enable = (uint32_t)(value))
#define get_status_alarm_enable() (g_status_config.status_alarm_enable)
#define set_status_engine_enable(value) (g_status_config.status_engine_enable = (uint32_t)(value))
#define get_status_engine_enable() (g_status_config.status_engine_enable)
#define set_status_comfort_enable(value) (g_status_config.status_comfort_enable = (uint32_t)(value))
#define get_status_comfort_enable() (g_status_config.status_comfort_enable)
#define set_status_temperature_enable(value) (g_status_config.status_temperature_enable = (uint32_t)(value))
#define get_status_temperature_enable() (g_status_config.status_temperature_enable)
#define set_status_req_input_enable(value) (g_status_config.status_req_input_enable = (uint32_t)(value))
#define get_status_req_input_enable() (g_status_config.status_req_input_enable)
#define set_status_dtc_enable(value) (g_status_config.status_dtc_enable = (uint32_t)(value))
#define get_status_dtc_enable() (g_status_config.status_dtc_enable)
#define set_status_mileage_enable(value) (g_status_config.status_mileage_enable = (uint32_t)(value))
#define get_status_mileage_enable() (g_status_config.status_mileage_enable)
#define set_status_tpms_enable(value) (g_status_config.status_tpms_enable = (uint32_t)(value))
#define get_status_tpms_enable() (g_status_config.status_tpms_enable)
#define set_status_doorlock_enable(value) (g_status_config.status_doorlock_enable = (uint32_t)(value))
#define get_status_doorlock_enable() (g_status_config.status_doorlock_enable)
#define set_status_oil_alarm_enable(value) (g_status_config.status_oil_alarm_enable = (uint32_t)(value))
#define get_status_oil_alarm_enable() (g_status_config.status_oil_alarm_enable)
#define set_status_tire_alarm_enable(value) (g_status_config.status_tire_alarm_enable = (uint32_t)(value))
#define get_status_tire_alarm_enable() (g_status_config.status_tire_alarm_enable)
#define set_status_seat_belts_enable(value) (g_status_config.status_seat_belts_enable = (uint32_t)(value))
#define get_status_seat_belts_enable() (g_status_config.status_seat_belts_enable)
#define set_status_seat_weights_enable(value) (g_status_config.status_seat_weights_enable = (uint32_t)(value))
#define get_status_seat_weights_enable() (g_status_config.status_seat_weights_enable)
#define set_status_ev_battery_lvl_enable(value) (g_status_config.status_ev_battery_lvl_enable = (uint32_t)(value))
#define get_status_ev_battery_lvl_enable() (g_status_config.status_ev_battery_lvl_enable)
#define set_status_ev_range_enable(value) (g_status_config.status_ev_range_enable = (uint32_t)(value))
#define get_status_ev_range_enable() (g_status_config.status_ev_range_enable)
#define set_status_oil_life_enable(value) (g_status_config.status_oil_life_enable = (uint32_t)(value))
#define get_status_oil_life_enable() (g_status_config.status_oil_life_enable)
#define set_status_fuel_lvl_enable(value) (g_status_config.status_fuel_lvl_enable = (uint32_t)(value))
#define get_status_fuel_lvl_enable() (g_status_config.status_fuel_lvl_enable)
#define set_status_fuel_cap_enable(value) (g_status_config.status_fuel_cap_enable = (uint32_t)(value))
#define get_status_fuel_cap_enable() (g_status_config.status_fuel_cap_enable)
#define set_status_cabin_temp_enable(value) (g_status_config.status_cabin_temp_enable = (uint32_t)(value))
#define get_status_cabin_temp_enable() (g_status_config.status_cabin_temp_enable)
#define set_status_tire_pressure_enable(value) (g_status_config.status_tire_pressure_enable = (uint32_t)(value))
#define get_status_tire_pressure_enable() (g_status_config.status_tire_pressure_enable)
#define set_status_odometer_enable(value) (g_status_config.status_odometer_enable = (uint32_t)(value))
#define get_status_odometer_enable() (g_status_config.status_odometer_enable)
#define set_status_battery_voltage_enable(value) (g_status_config.status_battery_voltage_enable = (uint32_t)(value))
#define get_status_battery_voltage_enable() (g_status_config.status_battery_voltage_enable)
#define set_function_honks_enable(value) (g_status_config.function_honks_enable = (uint32_t)(value))
#define get_function_honks_enable() (g_status_config.function_honks_enable)
#define set_function_headlights_enable(value) (g_status_config.function_headlights_enable = (uint32_t)(value))
#define get_function_headlights_enable() (g_status_config.function_headlights_enable)
#define set_function_rib_enable(value) (g_status_config.function_rib_enable = (uint32_t)(value))
#define get_function_rib_enable() (g_status_config.function_rib_enable)

#define status_config_set_defaults_from_defines() do { \
    set_status_vin_enable((uint32_t)(STATUS_VIN_ENABLE)); \
    set_status_rke_enable((uint32_t)(STATUS_RKE_ENABLE)); \
    set_status_key_enable((uint32_t)(STATUS_KEY_ENABLE)); \
    set_status_rpm_enable((uint32_t)(STATUS_RPM_ENABLE)); \
    set_status_door_enable((uint32_t)(STATUS_DOOR_ENABLE)); \
    set_status_speed_enable((uint32_t)(STATUS_SPEED_ENABLE)); \
    set_status_brake_enable((uint32_t)(STATUS_BRAKE_ENABLE)); \
    set_status_trans_enable((uint32_t)(STATUS_TRANS_ENABLE)); \
    set_status_alarm_enable((uint32_t)(STATUS_ALARM_ENABLE)); \
    set_status_engine_enable((uint32_t)(STATUS_ENGINE_ENABLE)); \
    set_status_comfort_enable((uint32_t)(STATUS_COMFORT_ENABLE)); \
    set_status_temperature_enable((uint32_t)(STATUS_TEMPERATURE_ENABLE)); \
    set_status_req_input_enable((uint32_t)(STATUS_REQ_INPUT_ENABLE)); \
    set_status_dtc_enable((uint32_t)(STATUS_DTC_ENABLE)); \
    set_status_mileage_enable((uint32_t)(STATUS_MILEAGE_ENABLE)); \
    set_status_tpms_enable((uint32_t)(STATUS_TPMS_ENABLE)); \
    set_status_doorlock_enable((uint32_t)(STATUS_DOORLOCK_ENABLE)); \
    set_status_oil_alarm_enable((uint32_t)(STATUS_OIL_ALARM_ENABLE)); \
    set_status_tire_alarm_enable((uint32_t)(STATUS_TIRE_ALARM_ENABLE)); \
    set_status_seat_belts_enable((uint32_t)(STATUS_SEAT_BELTS_ENABLE)); \
    set_status_seat_weights_enable((uint32_t)(STATUS_SEAT_WEIGHTS_ENABLE)); \
    set_status_ev_battery_lvl_enable((uint32_t)(STATUS_EV_BATTERY_LVL_ENABLE)); \
    set_status_ev_range_enable((uint32_t)(STATUS_EV_RANGE_ENABLE)); \
    set_status_oil_life_enable((uint32_t)(STATUS_OIL_LIFE_ENABLE)); \
    set_status_fuel_lvl_enable((uint32_t)(STATUS_FUEL_LVL_ENABLE)); \
    set_status_fuel_cap_enable((uint32_t)(STATUS_FUEL_CAP_ENABLE)); \
    set_status_cabin_temp_enable((uint32_t)(STATUS_CABIN_TEMP_ENABLE)); \
    set_status_tire_pressure_enable((uint32_t)(STATUS_TIRE_PRESSURE_ENABLE)); \
    set_status_odometer_enable((uint32_t)(STATUS_ODOMETER_ENABLE)); \
    set_status_battery_voltage_enable((uint32_t)(STATUS_BATTERY_VOLTAGE_ENABLE)); \
    set_function_honks_enable((uint32_t)(FUNCTION_HONKS_ENABLE)); \
    set_function_headlights_enable((uint32_t)(FUNCTION_HEADLIGHTS_ENABLE)); \
    set_function_rib_enable((uint32_t)(FUNCTION_RIB_ENABLE));  \
} while(0)

#define STATUS_CFG_STATUS_VIN_ENABLE (get_status_vin_enable())
#define STATUS_CFG_STATUS_RKE_ENABLE (get_status_rke_enable())
#define STATUS_CFG_STATUS_KEY_ENABLE (get_status_key_enable())
#define STATUS_CFG_STATUS_RPM_ENABLE (get_status_rpm_enable())
#define STATUS_CFG_STATUS_DOOR_ENABLE (get_status_door_enable())
#define STATUS_CFG_STATUS_SPEED_ENABLE (get_status_speed_enable())
#define STATUS_CFG_STATUS_BRAKE_ENABLE (get_status_brake_enable())
#define STATUS_CFG_STATUS_TRANS_ENABLE (get_status_trans_enable())
#define STATUS_CFG_STATUS_ALARM_ENABLE (get_status_alarm_enable())
#define STATUS_CFG_STATUS_ENGINE_ENABLE (get_status_engine_enable())
#define STATUS_CFG_STATUS_COMFORT_ENABLE (get_status_comfort_enable())
#define STATUS_CFG_STATUS_TEMPERATURE_ENABLE (get_status_temperature_enable())
#define STATUS_CFG_STATUS_REQ_INPUT_ENABLE (get_status_req_input_enable())
#define STATUS_CFG_STATUS_DTC_ENABLE (get_status_dtc_enable())
#define STATUS_CFG_STATUS_MILEAGE_ENABLE (get_status_mileage_enable())
#define STATUS_CFG_STATUS_TPMS_ENABLE (get_status_tpms_enable())
#define STATUS_CFG_STATUS_DOORLOCK_ENABLE (get_status_doorlock_enable())
#define STATUS_CFG_STATUS_OIL_ALARM_ENABLE (get_status_oil_alarm_enable())
#define STATUS_CFG_STATUS_TIRE_ALARM_ENABLE (get_status_tire_alarm_enable())
#define STATUS_CFG_STATUS_SEAT_BELTS_ENABLE (get_status_seat_belts_enable())
#define STATUS_CFG_STATUS_SEAT_WEIGHTS_ENABLE (get_status_seat_weights_enable())
#define STATUS_CFG_STATUS_EV_BATTERY_LVL_ENABLE (get_status_ev_battery_lvl_enable())
#define STATUS_CFG_STATUS_EV_RANGE_ENABLE (get_status_ev_range_enable())
#define STATUS_CFG_STATUS_OIL_LIFE_ENABLE (get_status_oil_life_enable())
#define STATUS_CFG_STATUS_FUEL_LVL_ENABLE (get_status_fuel_lvl_enable())
#define STATUS_CFG_STATUS_FUEL_CAP_ENABLE (get_status_fuel_cap_enable())
#define STATUS_CFG_STATUS_CABIN_TEMP_ENABLE (get_status_cabin_temp_enable())
#define STATUS_CFG_STATUS_TIRE_PRESSURE_ENABLE (get_status_tire_pressure_enable())
#define STATUS_CFG_STATUS_ODOMETER_ENABLE (get_status_odometer_enable())
#define STATUS_CFG_STATUS_BATTERY_VOLTAGE_ENABLE (get_status_battery_voltage_enable())
#define STATUS_CFG_FUNCTION_HONKS_ENABLE (get_function_honks_enable())
#define STATUS_CFG_FUNCTION_HEADLIGHTS_ENABLE (get_function_headlights_enable())
#define STATUS_CFG_FUNCTION_RIB_ENABLE (get_function_rib_enable())

void core_contract_ds4_status_config_init_defaults(void);
void core_contract_ds4_status_config_init_vehicle_specific(void);

#endif /* __STATUS_CONFIG_WRAPPER_H__ */
