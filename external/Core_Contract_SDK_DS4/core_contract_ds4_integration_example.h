#ifndef __CORE_CONTRACT_DS4_INTEGRATION_EXAMPLE_LEGACY_H__
#define __CORE_CONTRACT_DS4_INTEGRATION_EXAMPLE_LEGACY_H__

/* Legacy integration example only. Not part of active DS4 runtime flow. */

#ifdef CONTRACT_CONFIG_EXAMPLE_DEFINE_STORAGE
#define CAN_CONFIG_DEFINE_STORAGE
#define DBG_CONFIG_DEFINE_STORAGE
#define FIRMWARE_CONFIG_DEFINE_STORAGE
#define OBD_CAN_CONFIG_DEFINE_STORAGE
#define PTS_CTL_CONFIG_DEFINE_STORAGE
#define STATUS_CONFIG_DEFINE_STORAGE
#endif

#include "can_config_wrapper.h"
#include "dbg_config_wrapper.h"
#include "firmware_config_wrapper.h"
#include "gpio_config_wrapper.h"
#include "obd_can_config_wrapper.h"
#include "pts_ctl_config_wrapper.h"
#include "status_config_wrapper.h"

static inline void contract_config_load_defaults_all(void)
{
    can_config_set_defaults_from_defines();
    dbg_config_set_defaults_from_defines();
    firmware_config_set_defaults_from_defines();
    gpio_config_set_defaults_from_defines();
    obd_can_config_set_defaults_from_defines();
    pts_ctl_config_set_defaults_from_defines();
    status_config_set_defaults_from_defines();
}

static inline void contract_config_apply_vehicle_specific_example(void)
{
    set_status_vin_enable(STATUS_VIN_ENABLE);
    set_status_rke_enable(STATUS_RKE_ENABLE);
    set_fw_name(FW_NAME);
    set_app_version(APP_VERSION);
}

static inline void contract_config_init_all_example(void)
{
    contract_config_load_defaults_all();
    contract_config_apply_vehicle_specific_example();
}

#endif /* __CORE_CONTRACT_DS4_INTEGRATION_EXAMPLE_LEGACY_H__ */
