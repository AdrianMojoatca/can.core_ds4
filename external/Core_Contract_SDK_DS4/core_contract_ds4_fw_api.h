#ifndef __CORE_CONTRACT_DS4_FW_API_H__
#define __CORE_CONTRACT_DS4_FW_API_H__

#include <stdint.h>

#include "core_contract_ds4_boot_handshake.h"

static void core_contract_ds4_fw_gpio_handbrake_on(void)
{
    const core_ds4_api_table_t* api_table = core_contract_ds4_boot_api_table_get();
    if ((core_contract_ds4_api_table_is_valid(api_table) != 0U) && (api_table->gpio_handbrake_set != 0))
    {
        api_table->gpio_handbrake_set(1U);
    }
}

static void core_contract_ds4_fw_gpio_handbrake_off(void)
{
    const core_ds4_api_table_t* api_table = core_contract_ds4_boot_api_table_get();
    if ((core_contract_ds4_api_table_is_valid(api_table) != 0U) && (api_table->gpio_handbrake_set != 0))
    {
        api_table->gpio_handbrake_set(0U);
    }
}

static void core_contract_ds4_fw_gpio_doors_open(void)
{
    const core_ds4_api_table_t* api_table = core_contract_ds4_boot_api_table_get();
    if ((core_contract_ds4_api_table_is_valid(api_table) != 0U) && (api_table->gpio_doors_set != 0))
    {
        api_table->gpio_doors_set(1U);
    }
}

static void core_contract_ds4_fw_gpio_doors_closed(void)
{
    const core_ds4_api_table_t* api_table = core_contract_ds4_boot_api_table_get();
    if ((core_contract_ds4_api_table_is_valid(api_table) != 0U) && (api_table->gpio_doors_set != 0))
    {
        api_table->gpio_doors_set(0U);
    }
}

static void core_contract_ds4_fw_gpio_lights_on(void)
{
    const core_ds4_api_table_t* api_table = core_contract_ds4_boot_api_table_get();
    if ((core_contract_ds4_api_table_is_valid(api_table) != 0U) && (api_table->gpio_lights_set != 0))
    {
        api_table->gpio_lights_set(1U);
    }
}

static void core_contract_ds4_fw_gpio_lights_off(void)
{
    const core_ds4_api_table_t* api_table = core_contract_ds4_boot_api_table_get();
    if ((core_contract_ds4_api_table_is_valid(api_table) != 0U) && (api_table->gpio_lights_set != 0))
    {
        api_table->gpio_lights_set(0U);
    }
}

static void core_contract_ds4_fw_gpio_trunk_open(void)
{
    const core_ds4_api_table_t* api_table = core_contract_ds4_boot_api_table_get();
    if ((core_contract_ds4_api_table_is_valid(api_table) != 0U) && (api_table->gpio_trunk_set != 0))
    {
        api_table->gpio_trunk_set(1U);
    }
}

static void core_contract_ds4_fw_gpio_trunk_closed(void)
{
    const core_ds4_api_table_t* api_table = core_contract_ds4_boot_api_table_get();
    if ((core_contract_ds4_api_table_is_valid(api_table) != 0U) && (api_table->gpio_trunk_set != 0))
    {
        api_table->gpio_trunk_set(0U);
    }
}

static void core_contract_ds4_fw_gpio_brake_set(Boolean state)
{
    const core_ds4_api_table_t* api_table = core_contract_ds4_boot_api_table_get();
    if ((core_contract_ds4_api_table_is_valid(api_table) != 0U) && (api_table->gpio_brake_set != 0))
    {
        api_table->gpio_brake_set((state == TRUE) ? 1U : 0U);
    }
}

static void core_contract_ds4_fw_gpio_pts1_set(Boolean state)
{
    const core_ds4_api_table_t* api_table = core_contract_ds4_boot_api_table_get();
    if ((core_contract_ds4_api_table_is_valid(api_table) != 0U) && (api_table->gpio_pts1_set != 0))
    {
        api_table->gpio_pts1_set((state == TRUE) ? 1U : 0U);
    }
}

static void core_contract_ds4_fw_gpio_pts2_set(Boolean state)
{
    const core_ds4_api_table_t* api_table = core_contract_ds4_boot_api_table_get();
    if ((core_contract_ds4_api_table_is_valid(api_table) != 0U) && (api_table->gpio_pts2_set != 0))
    {
        api_table->gpio_pts2_set((state == TRUE) ? 1U : 0U);
    }
}

static void core_contract_ds4_fw_gpio_rap_set(Boolean state)
{
    const core_ds4_api_table_t* api_table = core_contract_ds4_boot_api_table_get();
    if ((core_contract_ds4_api_table_is_valid(api_table) != 0U) && (api_table->gpio_rap_set != 0))
    {
        api_table->gpio_rap_set((state == TRUE) ? 1U : 0U);
    }
}

static void core_contract_ds4_fw_gpio_rf_loop_set(Boolean state)
{
    const core_ds4_api_table_t* api_table = core_contract_ds4_boot_api_table_get();
    if ((core_contract_ds4_api_table_is_valid(api_table) != 0U) && (api_table->gpio_rf_loop_set != 0))
    {
        api_table->gpio_rf_loop_set((state == TRUE) ? 1U : 0U);
    }
}

static void core_contract_ds4_fw_gpio_ignition_on(void)
{
    const core_ds4_api_table_t* api_table = core_contract_ds4_boot_api_table_get();
    if ((core_contract_ds4_api_table_is_valid(api_table) != 0U) && (api_table->gpio_ignition_set != 0))
    {
        api_table->gpio_ignition_set(1U);
    }
}

static void core_contract_ds4_fw_gpio_ignition_off(void)
{
    const core_ds4_api_table_t* api_table = core_contract_ds4_boot_api_table_get();
    if ((core_contract_ds4_api_table_is_valid(api_table) != 0U) && (api_table->gpio_ignition_set != 0))
    {
        api_table->gpio_ignition_set(0U);
    }
}

static void core_contract_ds4_fw_gpio_hood_open(void)
{
    const core_ds4_api_table_t* api_table = core_contract_ds4_boot_api_table_get();
    if ((core_contract_ds4_api_table_is_valid(api_table) != 0U) && (api_table->gpio_hood_set != 0))
    {
        api_table->gpio_hood_set(1U);
    }
}

static void core_contract_ds4_fw_gpio_hood_closed(void)
{
    const core_ds4_api_table_t* api_table = core_contract_ds4_boot_api_table_get();
    if ((core_contract_ds4_api_table_is_valid(api_table) != 0U) && (api_table->gpio_hood_set != 0))
    {
        api_table->gpio_hood_set(0U);
    }
}

static void core_contract_ds4_fw_gpio_key_wrap_on(void)
{
    const core_ds4_api_table_t* api_table = core_contract_ds4_boot_api_table_get();
    if ((core_contract_ds4_api_table_is_valid(api_table) != 0U) && (api_table->gpio_key_wrap_set != 0))
    {
        api_table->gpio_key_wrap_set(1U);
    }
}

static void core_contract_ds4_fw_gpio_key_wrap_off(void)
{
    const core_ds4_api_table_t* api_table = core_contract_ds4_boot_api_table_get();
    if ((core_contract_ds4_api_table_is_valid(api_table) != 0U) && (api_table->gpio_key_wrap_set != 0))
    {
        api_table->gpio_key_wrap_set(0U);
    }
}

static void core_contract_ds4_fw_gpio_rf_loop_toggle(void)
{
    const core_ds4_api_table_t* api_table = core_contract_ds4_boot_api_table_get();
    if ((core_contract_ds4_api_table_is_valid(api_table) != 0U) && (api_table->gpio_rf_loop_toggle != 0))
    {
        api_table->gpio_rf_loop_toggle();
    }
}

#endif /* __CORE_CONTRACT_DS4_FW_API_H__ */
