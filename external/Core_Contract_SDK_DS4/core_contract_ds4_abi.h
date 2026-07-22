#ifndef __CORE_CONTRACT_DS4_ABI_H__
#define __CORE_CONTRACT_DS4_ABI_H__

#include <stdint.h>

#define CORE_DS4_ABI_MAGIC                   (0x43445334UL)   /* 'CDS4' */
#define CORE_DS4_ABI_VERSION_MAJOR           (1U)
#define CORE_DS4_ABI_VERSION_MINOR           (1U)   /* S5: +4 DS4-only routed fields (additive) */
#define CORE_DS4_API_TABLE_VERSION           (2U)

/*
 * DS4 contract is GREENFIELD / NOT frozen yet. The API-table window is sized
 * generously up front (see image_layout: table 0x37040 .. FW base 0x38000 = 0xFC0),
 * reserved_slots is large, and the surface is still evolving through the DS slices.
 * Freeze at DS6: pin the version constants, add freeze asserts + FROZEN.md.
 */
#define CORE_DS4_ABI_FROZEN                  (0U)

#define CORE_DS4_ABI_REQUIRED_MAJOR          CORE_DS4_ABI_VERSION_MAJOR
#define CORE_DS4_ABI_REQUIRED_MINOR          CORE_DS4_ABI_VERSION_MINOR

typedef enum
{
    CORE_DS4_ABI_COMPAT_INVALID = 0,
    CORE_DS4_ABI_COMPAT_OK = 1,
    CORE_DS4_ABI_COMPAT_MAJOR_MISMATCH = 2,
    CORE_DS4_ABI_COMPAT_MINOR_TOO_OLD = 3
} core_ds4_abi_compat_result_t;

typedef struct
{
    uint32_t abi_magic;
    uint16_t abi_version_major;
    uint16_t abi_version_minor;
    uint32_t api_table_version;
    uint32_t capabilities_mask;
    /* Reserved for future ABI extensions. Must remain 0 in ABI v1. */
    uint32_t reserved_0;
} core_ds4_abi_info_t;

typedef const core_ds4_abi_info_t* (*core_ds4_get_abi_info_fn_t)(void);

static core_ds4_abi_compat_result_t core_contract_ds4_abi_check_compatibility(
    uint16_t required_major,
    uint16_t required_minor,
    const core_ds4_abi_info_t* abi_info)
{
    if (abi_info == 0)
    {
        return CORE_DS4_ABI_COMPAT_INVALID;
    }

    if (abi_info->abi_magic != CORE_DS4_ABI_MAGIC)
    {
        return CORE_DS4_ABI_COMPAT_INVALID;
    }

    if (abi_info->abi_version_major != required_major)
    {
        return CORE_DS4_ABI_COMPAT_MAJOR_MISMATCH;
    }

    if (abi_info->abi_version_minor < required_minor)
    {
        return CORE_DS4_ABI_COMPAT_MINOR_TOO_OLD;
    }

    return CORE_DS4_ABI_COMPAT_OK;
}

#endif /* __CORE_CONTRACT_DS4_ABI_H__ */
