#define STATUS_CONFIG_DEFINE_STORAGE
#include "core_contract_ds4_status_config.h"

__weak void core_contract_ds4_status_config_apply_vehicle_specific(void)
{
}

void core_contract_ds4_status_config_init_vehicle_specific(void)
{
    core_contract_ds4_status_config_apply_vehicle_specific();
}

void core_contract_ds4_status_config_init_defaults(void)
{
    status_config_set_defaults_from_defines();
    core_contract_ds4_status_config_init_vehicle_specific();
}
