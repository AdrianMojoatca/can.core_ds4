#ifndef __CORE_CONTRACT_DS4_BOOT_HANDSHAKE_H__
#define __CORE_CONTRACT_DS4_BOOT_HANDSHAKE_H__

#include <stdint.h>

#include "core_contract_ds4_abi.h"
#include "core_contract_ds4_api_table.h"
#include "core_contract_ds4_capabilities.h"
#include "core_contract_ds4_image_layout.h"
#include "core_contract_ds4_gpio_output.h"

#if defined(CORE_DS4_BUILD)
extern const core_ds4_abi_info_t g_core_ds4_shared_abi_info;
extern const core_ds4_api_table_t g_core_ds4_shared_api_table;
#endif

typedef enum
{
    CORE_DS4_BOOT_HANDSHAKE_FAIL = 0,
    CORE_DS4_BOOT_HANDSHAKE_OK = 1
} core_ds4_boot_handshake_result_t;

static void core_contract_ds4_api_gpio_handbrake_set(uint32_t state_value)
{
    if (state_value != 0U)
    {
        core_contract_ds4_gpio_handbrake_on();
    }
    else
    {
        core_contract_ds4_gpio_handbrake_off();
    }
}

static void core_contract_ds4_api_gpio_doors_set(uint32_t state_value)
{
    if (state_value != 0U)
    {
        core_contract_ds4_gpio_doors_open();
    }
    else
    {
        core_contract_ds4_gpio_doors_closed();
    }
}

static void core_contract_ds4_api_gpio_lights_set(uint32_t state_value)
{
    if (state_value != 0U)
    {
        core_contract_ds4_gpio_lights_on();
    }
    else
    {
        core_contract_ds4_gpio_lights_off();
    }
}

static void core_contract_ds4_api_gpio_trunk_set(uint32_t state_value)
{
    if (state_value != 0U)
    {
        core_contract_ds4_gpio_trunk_open();
    }
    else
    {
        core_contract_ds4_gpio_trunk_closed();
    }
}

static void core_contract_ds4_api_gpio_brake_set(uint32_t state_value)
{
    core_contract_ds4_gpio_brake_set((state_value != 0U) ? TRUE : FALSE);
}

static void core_contract_ds4_api_gpio_pts1_set(uint32_t state_value)
{
    core_contract_ds4_gpio_pts_1_set((state_value != 0U) ? TRUE : FALSE);
}

static void core_contract_ds4_api_gpio_pts2_set(uint32_t state_value)
{
    core_contract_ds4_gpio_pts_2_set((state_value != 0U) ? TRUE : FALSE);
}

static void core_contract_ds4_api_gpio_rap_set(uint32_t state_value)
{
    core_contract_ds4_gpio_rap_set((state_value != 0U) ? TRUE : FALSE);
}

static void core_contract_ds4_api_gpio_rf_loop_set(uint32_t state_value)
{
    core_contract_ds4_gpio_rf_loop_set((state_value != 0U) ? TRUE : FALSE);
}

static void core_contract_ds4_api_gpio_ignition_set(uint32_t state_value)
{
    if (state_value != 0U)
    {
        core_contract_ds4_gpio_ignition_on();
    }
    else
    {
        core_contract_ds4_gpio_ignition_off();
    }
}

static void core_contract_ds4_api_gpio_hood_set(uint32_t state_value)
{
    if (state_value != 0U)
    {
        core_contract_ds4_gpio_hood_open();
    }
    else
    {
        core_contract_ds4_gpio_hood_closed();
    }
}

static void core_contract_ds4_api_gpio_key_wrap_set(uint32_t state_value)
{
    if (state_value != 0U)
    {
        core_contract_ds4_gpio_key_wrap_on();
    }
    else
    {
        core_contract_ds4_gpio_key_wrap_off();
    }
}

static const core_ds4_abi_info_t* core_contract_ds4_local_abi_info_get(void)
{
    static const core_ds4_abi_info_t local_abi_info =
    {
        CORE_DS4_ABI_MAGIC,
        CORE_DS4_ABI_VERSION_MAJOR,
        CORE_DS4_ABI_VERSION_MINOR,
        CORE_DS4_API_TABLE_VERSION,
        CORE_DS4_CAPABILITY_DEFAULT_MASK,
        0U
    };

    return &local_abi_info;
}

static const core_ds4_api_table_t* core_contract_ds4_local_api_table_get(void)
{
    static const core_ds4_api_table_t local_api_table =
    {
        CORE_DS4_API_TABLE_MAGIC,
        sizeof(core_ds4_api_table_t),
        CORE_DS4_ABI_VERSION_MAJOR,
        CORE_DS4_ABI_VERSION_MINOR,
        CORE_DS4_API_TABLE_VERSION,
        CORE_DS4_CAPABILITY_DEFAULT_MASK,

        core_contract_ds4_local_abi_info_get,

        core_contract_ds4_api_gpio_handbrake_set,
        core_contract_ds4_api_gpio_doors_set,
        core_contract_ds4_api_gpio_lights_set,
        core_contract_ds4_api_gpio_trunk_set,
        core_contract_ds4_api_gpio_brake_set,
        core_contract_ds4_api_gpio_pts1_set,
        core_contract_ds4_api_gpio_pts2_set,
        core_contract_ds4_api_gpio_rap_set,
        core_contract_ds4_api_gpio_rf_loop_set,
        core_contract_ds4_api_gpio_ignition_set,
        core_contract_ds4_api_gpio_hood_set,
        core_contract_ds4_api_gpio_key_wrap_set,

        core_contract_ds4_gpio_rf_loop_toggle,

        {0}
    };

    return &local_api_table;
}

static const core_ds4_api_table_t* core_contract_ds4_boot_api_table_get(void)
{
#if defined(CORE_DS4_BUILD)
    return &g_core_ds4_shared_api_table;
#elif defined(FW_DS4_BUILD)
    return (const core_ds4_api_table_t*)(CORE_DS4_SHARED_API_TABLE_ADDR);
#else
    return core_contract_ds4_local_api_table_get();
#endif
}

static core_ds4_boot_handshake_result_t core_contract_ds4_boot_handshake_validate(
    uint16_t required_abi_minor,
    core_ds4_capabilities_mask_t required_capabilities,
    const core_ds4_api_table_t* api_table)
{
    const core_ds4_api_table_t* selected_api_table = api_table;
    const core_ds4_abi_info_t* abi_info;
    core_ds4_abi_compat_result_t compatibility_result;

    if (selected_api_table == 0)
    {
        selected_api_table = core_contract_ds4_boot_api_table_get();
    }

    if (core_contract_ds4_api_table_is_valid(selected_api_table) == 0U)
    {
        return CORE_DS4_BOOT_HANDSHAKE_FAIL;
    }

    if (selected_api_table->get_abi_info == 0)
    {
        return CORE_DS4_BOOT_HANDSHAKE_FAIL;
    }

    abi_info = selected_api_table->get_abi_info();
    compatibility_result = core_contract_ds4_abi_check_compatibility(
        CORE_DS4_ABI_VERSION_MAJOR,
        required_abi_minor,
        abi_info);

    if (compatibility_result != CORE_DS4_ABI_COMPAT_OK)
    {
        return CORE_DS4_BOOT_HANDSHAKE_FAIL;
    }

    if (core_contract_ds4_capability_is_supported(
            abi_info->capabilities_mask,
            required_capabilities) == 0U)
    {
        return CORE_DS4_BOOT_HANDSHAKE_FAIL;
    }

    return CORE_DS4_BOOT_HANDSHAKE_OK;
}

#endif /* __CORE_CONTRACT_DS4_BOOT_HANDSHAKE_H__ */
