#include "core_contract_db3_init.h"
#include "core_contract_db3_can_config.h"
#include "core_contract_db3_dbg_config.h"
#include "core_contract_db3_firmware_config.h"
#include "core_contract_db3_gpio_config.h"
#include "core_contract_db3_obd_can_config.h"
#include "core_contract_db3_prog_config.h"
#include "core_contract_db3_pts_ctl_config.h"
#include "core_contract_db3_status_config.h"

void core_contract_db3_init_vehicle_specific_all(void)
{
    core_contract_db3_can_config_init_vehicle_specific();
    core_contract_db3_dbg_config_init_vehicle_specific();
    core_contract_db3_firmware_config_init_vehicle_specific();
    core_contract_db3_obd_can_config_init_vehicle_specific();
    core_contract_db3_prog_config_init_vehicle_specific();
    core_contract_db3_pts_ctl_config_init_vehicle_specific();
    core_contract_db3_status_config_init_vehicle_specific();
}

void core_contract_db3_init_defaults_all(void)
{
    core_contract_db3_can_config_init_defaults();
    core_contract_db3_dbg_config_init_defaults();
    core_contract_db3_firmware_config_init_defaults();
    core_contract_db3_obd_can_config_init_defaults();
    core_contract_db3_prog_config_init_defaults();
    core_contract_db3_pts_ctl_config_init_defaults();
    core_contract_db3_status_config_init_defaults();
}
