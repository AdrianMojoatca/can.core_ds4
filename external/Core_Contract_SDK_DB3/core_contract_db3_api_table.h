#ifndef __CORE_CONTRACT_DB3_API_TABLE_H__
#define __CORE_CONTRACT_DB3_API_TABLE_H__

#include <stdint.h>

#include "core_contract_db3_abi.h"
#include "core_contract_db3_capabilities.h"

#define CORE_DB3_API_TABLE_MAGIC             (0x41504233UL)
#define CORE_DB3_API_TABLE_RESERVED_SLOTS    (16U)

typedef enum
{
    CORE_DB3_API_FN_GPIO_HANDBRAKE_SET = 0,
    CORE_DB3_API_FN_GPIO_DOORS_SET,
    CORE_DB3_API_FN_GPIO_LIGHTS_SET,
    CORE_DB3_API_FN_GPIO_TRUNK_SET,
    CORE_DB3_API_FN_GPIO_BRAKE_SET,
    CORE_DB3_API_FN_GPIO_PTS1_SET,
    CORE_DB3_API_FN_GPIO_PTS2_SET,
    CORE_DB3_API_FN_GPIO_RAP_SET,
    CORE_DB3_API_FN_GPIO_RF_LOOP_SET,
    CORE_DB3_API_FN_GPIO_IGNITION_SET,
    CORE_DB3_API_FN_GPIO_HOOD_SET,
    CORE_DB3_API_FN_GPIO_KEY_WRAP_SET,
    CORE_DB3_API_FN_COUNT
} core_db3_api_function_id_t;

#define CORE_DB3_API_TABLE_FUNCTION_COUNT    ((uint32_t)CORE_DB3_API_FN_COUNT)

typedef void (*core_db3_api_gpio_state_set_fn_t)(uint32_t state_value);
typedef void (*core_db3_api_gpio_toggle_fn_t)(void);
typedef const core_db3_abi_info_t* (*core_db3_api_get_abi_info_fn_t)(void);

typedef struct
{
    uint32_t table_magic;
    uint32_t table_size;
    uint16_t abi_version_major;
    uint16_t abi_version_minor;
    uint32_t api_table_version;
    core_db3_capabilities_mask_t capabilities_mask;

    core_db3_api_get_abi_info_fn_t get_abi_info;

    core_db3_api_gpio_state_set_fn_t gpio_handbrake_set;
    core_db3_api_gpio_state_set_fn_t gpio_doors_set;
    core_db3_api_gpio_state_set_fn_t gpio_lights_set;
    core_db3_api_gpio_state_set_fn_t gpio_trunk_set;
    core_db3_api_gpio_state_set_fn_t gpio_brake_set;
    core_db3_api_gpio_state_set_fn_t gpio_pts1_set;
    core_db3_api_gpio_state_set_fn_t gpio_pts2_set;
    core_db3_api_gpio_state_set_fn_t gpio_rap_set;
    core_db3_api_gpio_state_set_fn_t gpio_rf_loop_set;
    core_db3_api_gpio_state_set_fn_t gpio_ignition_set;
    core_db3_api_gpio_state_set_fn_t gpio_hood_set;
    core_db3_api_gpio_state_set_fn_t gpio_key_wrap_set;

    core_db3_api_gpio_toggle_fn_t gpio_rf_loop_toggle;

    void* reserved_slots[CORE_DB3_API_TABLE_RESERVED_SLOTS];
} core_db3_api_table_t;

static uint32_t core_contract_db3_api_table_is_valid(const core_db3_api_table_t* api_table)
{
    if (api_table == 0)
    {
        return 0U;
    }

    if (api_table->table_magic != CORE_DB3_API_TABLE_MAGIC)
    {
        return 0U;
    }

    if (api_table->table_size < sizeof(core_db3_api_table_t))
    {
        return 0U;
    }

    if (api_table->abi_version_major != CORE_DB3_ABI_VERSION_MAJOR)
    {
        return 0U;
    }

    if (api_table->api_table_version != CORE_DB3_API_TABLE_VERSION)
    {
        return 0U;
    }

    if (api_table->abi_version_minor < CORE_DB3_ABI_VERSION_MINOR)
    {
        return 0U;
    }

    return 1U;
}

#endif /* __CORE_CONTRACT_DB3_API_TABLE_H__ */
