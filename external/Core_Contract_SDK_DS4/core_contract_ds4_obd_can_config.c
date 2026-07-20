#define OBD_CAN_CONFIG_DEFINE_STORAGE
#include "core_contract_ds4_obd_can_config.h"

void core_contract_ds4_obd_can_config_init_vehicle_specific(void)
{
}

void core_contract_ds4_obd_can_config_init_defaults(void)
{
    obd_can_config_set_defaults_from_defines();
    core_contract_ds4_obd_can_config_init_vehicle_specific();
}
