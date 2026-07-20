#ifndef __CORE_CONTRACT_DS4_FW_DESCRIPTOR_H__
#define __CORE_CONTRACT_DS4_FW_DESCRIPTOR_H__

/*
 * FW image descriptor (single-boot / thin-FW model).
 *
 * In the thin-FW model CORE boots the RTOS once and STAYS LIVE; it then invokes
 * the FW image through this descriptor instead of BX-jumping into an FW re-boot.
 * The descriptor is published by the FW image at its flash base
 * (CORE_DS4_FLASH_FW_START_ADDR = 0x00028000) so CORE can find + validate it.
 *
 * CORE calls, in order:  fw_ram_init_fn  -> fw_identity_init_fn -> core_init()
 *                        -> [io test] -> fw_init_reset_fn -> fw_init_fn.
 * fw_identity_init MUST precede core_init (core_init consumes the config it writes).
 *
 * A CORE that finds no valid descriptor at 0x28000 falls back to the legacy
 * re-boot handoff (core_contract_ds4_fw_handoff.h) -> full backward compatibility.
 */

#include <stdint.h>

#include "core_contract_ds4_abi.h"
#include "core_contract_ds4_image_layout.h"

#define CORE_DS4_FW_DESC_MAGIC   (0x34445746UL)   /* 'FWD4' */

typedef void (*core_ds4_fw_entry_fn_t)(void);

typedef struct
{
    uint32_t               magic;               /* == CORE_DS4_FW_DESC_MAGIC          */
    uint32_t               abi_version;         /* (ABI_MAJOR << 16) | ABI_MINOR      */
    uint32_t               struct_size;         /* sizeof(core_ds4_fw_descriptor_t)   */
    uint32_t               flags;               /* reserved feature flags, 0 for now  */

    core_ds4_fw_entry_fn_t fw_ram_init_fn;      /* init FW RW/ZI window; run FIRST    */
    core_ds4_fw_entry_fn_t fw_identity_init_fn; /* populate config; before core_init  */
    core_ds4_fw_entry_fn_t fw_init_fn;          /* vehicle init; registers RTOS work  */
    core_ds4_fw_entry_fn_t fw_init_reset_fn;    /* optional reset registration (or 0) */

    uint32_t               reserved[7];
    uint32_t               crc32;               /* optional integrity (0 = unused)    */
} core_ds4_fw_descriptor_t;                     /* 16 words = 0x40 bytes              */

/* Strict validator (magic-gated; unlike the legacy is_valid which accepts any
 * non-empty first word). Returns 1 if the descriptor is a usable thin-FW image. */
static uint32_t core_contract_ds4_fw_descriptor_is_valid(const core_ds4_fw_descriptor_t* d)
{
    uint32_t lo;
    uint32_t hi;
    uint8_t  i;
    core_ds4_fw_entry_fn_t fns[3];

    if (d == 0)
    {
        return 0U;
    }
    if (d->magic != CORE_DS4_FW_DESC_MAGIC)
    {
        return 0U;
    }
    if ((d->abi_version >> 16) != (uint32_t)CORE_DS4_ABI_VERSION_MAJOR)
    {
        return 0U;
    }
    if (d->struct_size < sizeof(core_ds4_fw_descriptor_t))
    {
        return 0U;
    }

    lo = (uint32_t)CORE_DS4_FLASH_FW_START_ADDR;
    hi = (uint32_t)CORE_DS4_FLASH_FW_END_ADDR;
    fns[0] = d->fw_ram_init_fn;
    fns[1] = d->fw_identity_init_fn;
    fns[2] = d->fw_init_fn;
    for (i = 0U; i < 3U; i++)
    {
        uint32_t a = (uint32_t)fns[i];
        if ((a & 1U) == 0U)          /* must be a Thumb entry */
        {
            return 0U;
        }
        a &= ~1U;
        if ((a < lo) || (a > hi))    /* must live inside the FW flash window */
        {
            return 0U;
        }
    }

    return 1U;
}

#endif /* __CORE_CONTRACT_DS4_FW_DESCRIPTOR_H__ */
