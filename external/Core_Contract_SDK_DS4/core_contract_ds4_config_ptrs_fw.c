/*<#======================================================================#>*/
/*  SS4 Slice 2 - FW-side config accessors (single-instance).               */
/*  Return CORE's single config instances, published as DATA pointers in    */
/*  the API table at 0x27E40. The config headers redefine g_*_config to     */
/*  (*core_ds4_cfg_*()) under FW_DS4_BUILD, so every FW set_/get_ macro      */
/*  touches CORE's instance. Compiled only in the FW image (empty in CORE). */
/*<#======================================================================#>*/

#if defined(FW_DS4_BUILD)

#include "core_contract_ds4_boot_handshake.h"   /* core_contract_ds4_boot_api_table_get */
#include "core_contract_ds4_api_table.h"        /* table layout + config struct types  */
#include "gpio_pl.h"                            /* Gpio_Pl_T for the gpio_pl wrappers   */

/* SS4 Slice 8: real FW wrappers for gpio_pl_set/clr (routed to CORE via table).
   Real functions (not macros) so gpio.h/gpio_pl.h declarations stay valid and
   gpio_output.h inlines / vehicle code link against these. */
void gpio_pl_set(Gpio_Pl_T pl)
{
    core_contract_ds4_boot_api_table_get()->gpio_pl_set_fn((uint32_t)pl);
}

void gpio_pl_clr(Gpio_Pl_T pl)
{
    core_contract_ds4_boot_api_table_get()->gpio_pl_clr_fn((uint32_t)pl);
}

firmware_config_contract_t* core_ds4_cfg_firmware(void)
{
    return core_contract_ds4_boot_api_table_get()->firmware_config;
}

can_config_contract_t* core_ds4_cfg_can(void)
{
    return core_contract_ds4_boot_api_table_get()->can_config;
}

pts_ctl_config_contract_t* core_ds4_cfg_pts_ctl(void)
{
    return core_contract_ds4_boot_api_table_get()->pts_ctl_config;
}

prog_config_contract_t* core_ds4_cfg_prog(void)
{
    return core_contract_ds4_boot_api_table_get()->prog_config;
}

status_config_contract_t* core_ds4_cfg_status(void)
{
    return core_contract_ds4_boot_api_table_get()->status_config;
}

/* v1.1: FW-owned runtime debug config. Null-safe against a pre-v1.1 CORE whose
   table has no dbg_config slot (api_table_version < 12). */
dbg_config_contract_t* core_ds4_cfg_dbg(void)
{
    const core_ds4_api_table_t* t = core_contract_ds4_boot_api_table_get();
    if (t->api_table_version < 12U) { return 0; }
    return t->dbg_config;
}

#endif /* FW_DS4_BUILD */
