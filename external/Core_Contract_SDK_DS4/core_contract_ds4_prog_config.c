#define PROG_CONFIG_DEFINE_STORAGE
#include "core_contract_ds4_prog_config.h"
#include <string.h>

__weak void core_contract_ds4_prog_config_apply_vehicle_specific(void)
{
}

void core_contract_ds4_prog_config_init_vehicle_specific(void)
{
    core_contract_ds4_prog_config_apply_vehicle_specific();
}

void core_contract_ds4_prog_config_init_defaults(void)
{
    memset(&g_prog_config, 0, sizeof(g_prog_config));
    core_contract_ds4_prog_config_init_vehicle_specific();
}
