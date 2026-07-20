#include "core_contract_ds4_init.h"
#include "core_contract_ds4_can_config.h"
#include "core_contract_ds4_dbg_config.h"
#include "core_contract_ds4_firmware_config.h"
#include "core_contract_ds4_gpio_config.h"
#include "core_contract_ds4_obd_can_config.h"
#include "core_contract_ds4_prog_config.h"
#include "core_contract_ds4_pts_ctl_config.h"
#include "core_contract_ds4_status_config.h"

void core_contract_ds4_init_vehicle_specific_all(void)
{
    core_contract_ds4_can_config_init_vehicle_specific();
    core_contract_ds4_dbg_config_init_vehicle_specific();
    core_contract_ds4_firmware_config_init_vehicle_specific();
    core_contract_ds4_obd_can_config_init_vehicle_specific();
    core_contract_ds4_prog_config_init_vehicle_specific();
    core_contract_ds4_pts_ctl_config_init_vehicle_specific();
    core_contract_ds4_status_config_init_vehicle_specific();
}

void core_contract_ds4_init_defaults_all(void)
{
    core_contract_ds4_can_config_init_defaults();
    core_contract_ds4_dbg_config_init_defaults();
    core_contract_ds4_firmware_config_init_defaults();
    core_contract_ds4_obd_can_config_init_defaults();
    core_contract_ds4_prog_config_init_defaults();
    core_contract_ds4_pts_ctl_config_init_defaults();
    core_contract_ds4_status_config_init_defaults();
}
