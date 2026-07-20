#ifndef __CORE_CONTRACT_DB3_ABI_H__
#define __CORE_CONTRACT_DB3_ABI_H__

#include <stdint.h>

#define CORE_DB3_ABI_MAGIC                   (0x43444233UL)
#define CORE_DB3_ABI_VERSION_MAJOR           (1U)
#define CORE_DB3_ABI_VERSION_MINOR           (0U)
#define CORE_DB3_API_TABLE_VERSION           (1U)
#define CORE_DB3_ABI_FREEZE_DATE             (20260702UL)

#define CORE_DB3_ABI_REQUIRED_MAJOR          CORE_DB3_ABI_VERSION_MAJOR
#define CORE_DB3_ABI_REQUIRED_MINOR          CORE_DB3_ABI_VERSION_MINOR

typedef enum
{
    CORE_DB3_ABI_COMPAT_INVALID = 0,
    CORE_DB3_ABI_COMPAT_OK = 1,
    CORE_DB3_ABI_COMPAT_MAJOR_MISMATCH = 2,
    CORE_DB3_ABI_COMPAT_MINOR_TOO_OLD = 3
} core_db3_abi_compat_result_t;

typedef struct
{
    uint32_t abi_magic;
    uint16_t abi_version_major;
    uint16_t abi_version_minor;
    uint32_t api_table_version;
    uint32_t capabilities_mask;
    /* Reserved for future ABI extensions. Must remain 0 in ABI v1. */
    uint32_t reserved_0;
} core_db3_abi_info_t;

typedef const core_db3_abi_info_t* (*core_db3_get_abi_info_fn_t)(void);

static core_db3_abi_compat_result_t core_contract_db3_abi_check_compatibility(
    uint16_t required_major,
    uint16_t required_minor,
    const core_db3_abi_info_t* abi_info)
{
    if (abi_info == 0)
    {
        return CORE_DB3_ABI_COMPAT_INVALID;
    }

    if (abi_info->abi_magic != CORE_DB3_ABI_MAGIC)
    {
        return CORE_DB3_ABI_COMPAT_INVALID;
    }

    if (abi_info->abi_version_major != required_major)
    {
        return CORE_DB3_ABI_COMPAT_MAJOR_MISMATCH;
    }

    if (abi_info->abi_version_minor < required_minor)
    {
        return CORE_DB3_ABI_COMPAT_MINOR_TOO_OLD;
    }

    return CORE_DB3_ABI_COMPAT_OK;
}

#endif /* __CORE_CONTRACT_DB3_ABI_H__ */
