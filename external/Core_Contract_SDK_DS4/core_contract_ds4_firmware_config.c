#define FIRMWARE_CONFIG_DEFINE_STORAGE
#include "core_contract_ds4_firmware_config.h"

void core_contract_ds4_firmware_config_init_vehicle_specific(void)
{
}

void core_contract_ds4_firmware_config_init_defaults(void)
{
    //firmware_config_set_defaults_from_defines();
    core_contract_ds4_firmware_config_init_vehicle_specific();
}
