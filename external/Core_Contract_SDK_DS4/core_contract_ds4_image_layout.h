#ifndef __CORE_CONTRACT_DS4_IMAGE_LAYOUT_H__
#define __CORE_CONTRACT_DS4_IMAGE_LAYOUT_H__

#include <stdint.h>

/*
 * DS4 flash map for split-image rollout.
 *
 * Full app flash window (from current app.sct):
 *   0x00008100 .. 0x0003FEFF   (bootloader comment in boot_1700.sct: "max address is 0x40000")
 *
 * Split layout used by build split tooling:
 *   CORE image: 0x00008100 .. 0x00027FFF   (code capped by scatter at ABI addr 0x00027E00)
 *   FW image  : 0x00028000 .. 0x0003FEFF   (FROZEN base 0x28000 -- do NOT move; every
 *                                            shipped thin FW is linked against it)
 *
 * Whole-chip context (LPC176x, 256 KB flash 0x00000..0x3FFFF):
 *   0x00000000 .. 0x00005FFF  bootloader code            (boot_1700.sct ER_CODE_0, 0x6000)
 *   0x00006000 .. 0x00007FFF  NVRAM/NVFS flash "Block #0" (boot_1700.sct, 8 KB, 4k-aligned,
 *                                                           bootloader-owned via BootService())
 *   0x00008000 .. 0x000080FF  ABOUT block
 *   0x00008100 .. 0x00027E00  CORE code
 *   0x00027E00 / 0x00027E40   ABI info / API table (shared, __AT-fixed)
 *   0x00028000 .. 0x0003FEFF  FW image
 *
 * BOUNDARY / GROWTH POLICY (frozen 2026-07-15):
 *   - NVFS is NOT in the application window. It lives at 0x6000 (below the app) and is
 *     reached only through bootloader BootService() traps; neither CORE nor FW hold its
 *     sector addresses. FW grows UPWARD (away from NVFS), so FW growth cannot corrupt NVFS.
 *   - CORE<->FW is protected by hard linker caps: CORE code region ends at 0x27E00 and the
 *     FW region ends at 0x3FEFF; exceeding either is a LINK error (region overflow), never a
 *     silent overlap. GAP between CORE and FW is intentionally 0 (adjacent) -- the ABI+table
 *     occupy 0x27E00..0x27FFF between CORE code and the FW base.
 *   - SOFT CEILING (policy, not linker-enforced): keep CORE code below
 *     CORE_DS4_FLASH_CORE_SOFT_CEILING_ADDR, i.e. a 4 KB reserve under the 0x27E00 hard cap.
 *     ArmLink cannot "warn and continue", so enforce this with a post-build map check:
 *       ER_IROM_APP1 (Base 0x8100) + Size  must stay < 0x00026E00.
 *     If it creeps past the soft ceiling, plan CORE reduction BEFORE hitting 0x27E00 --
 *     do not move the frozen FW base 0x28000.
 */

#define CORE_DS4_FLASH_APP_START_ADDR         (0x00008100UL)
#define CORE_DS4_FLASH_APP_END_ADDR           (0x0003FEFFUL)

#define CORE_DS4_FLASH_CORE_START_ADDR        (0x00008100UL)
#define CORE_DS4_FLASH_CORE_END_ADDR          (0x00027FFFUL)

#define CORE_DS4_FLASH_FW_START_ADDR          (0x00028000UL)
#define CORE_DS4_FLASH_FW_END_ADDR            (0x0003FEFFUL)

#define CORE_DS4_FLASH_GAP_START_ADDR         (CORE_DS4_FLASH_CORE_END_ADDR + 1UL)
#define CORE_DS4_FLASH_GAP_END_ADDR           (CORE_DS4_FLASH_FW_START_ADDR - 1UL)
#define CORE_DS4_FLASH_GAP_SIZE_BYTES         (CORE_DS4_FLASH_FW_START_ADDR - CORE_DS4_FLASH_CORE_END_ADDR - 1UL)

/*
 * Minimum reserved growth gap between CORE and FW images.
 * Frozen at 0: CORE and FW are adjacent by design; the hard linker caps (CORE code end
 * 0x27E00, FW end 0x3FEFF) make a physical gap unnecessary for overlap safety.
 */
#define CORE_DS4_FLASH_MIN_GAP_BYTES          (0UL)

/*
 * CORE code hard cap: the CORE scatter (app.sct ER_IROM_APP1, Base 0x8100, Max 0x1FD00)
 * ends exactly at the ABI info address. Growing past it is a link error.
 */
#define CORE_DS4_FLASH_CORE_CODE_CAP_ADDR     (CORE_DS4_SHARED_ABI_INFO_ADDR)

/*
 * CORE code SOFT ceiling (policy reserve, 4 KB below the hard cap). Not linker-enforced --
 * verify after each CORE build via the map:  Base(0x8100) + ER_IROM_APP1 Size < this addr.
 */
#define CORE_DS4_FLASH_CORE_SOFT_RESERVE_BYTES (0x1000UL)   /* 4 KB early-warning reserve */
#define CORE_DS4_FLASH_CORE_SOFT_CEILING_ADDR  (CORE_DS4_FLASH_CORE_CODE_CAP_ADDR - \
                                                CORE_DS4_FLASH_CORE_SOFT_RESERVE_BYTES)

#define CORE_DS4_FLASH_CORE_SIZE_BYTES        (CORE_DS4_FLASH_CORE_END_ADDR - CORE_DS4_FLASH_CORE_START_ADDR + 1UL)
#define CORE_DS4_FLASH_FW_SIZE_BYTES          (CORE_DS4_FLASH_FW_END_ADDR - CORE_DS4_FLASH_FW_START_ADDR + 1UL)

#define CORE_DS4_IMAGE_LAYOUT_VERSION          (2U)
#define CORE_DS4_IMAGE_LAYOUT_FREEZE_DATE      (20260715UL)

/*
 * Reserved addresses for ABI metadata and API table publication in Core image.
 * NOTE:
 *   - Do not place shared contract data inside 0x00008100..startup region.
 *   - Keep this window in upper CORE flash and reserve it explicitly in CORE scatter.
 */
#define CORE_DS4_SHARED_ABI_INFO_ADDR          (0x00027000UL)
#define CORE_DS4_SHARED_API_TABLE_ADDR         (0x00027040UL)

#define CORE_DS4_LAYOUT_ASSERT(name, expr) typedef char core_ds4_layout_assert_##name[(expr) ? 1 : -1]

CORE_DS4_LAYOUT_ASSERT(app_starts_with_core_start,
	CORE_DS4_FLASH_APP_START_ADDR == CORE_DS4_FLASH_CORE_START_ADDR);
CORE_DS4_LAYOUT_ASSERT(core_before_fw,
	CORE_DS4_FLASH_CORE_END_ADDR < CORE_DS4_FLASH_FW_START_ADDR);
CORE_DS4_LAYOUT_ASSERT(app_ends_with_fw_end,
	CORE_DS4_FLASH_APP_END_ADDR == CORE_DS4_FLASH_FW_END_ADDR);
CORE_DS4_LAYOUT_ASSERT(gap_policy_respected,
	CORE_DS4_FLASH_GAP_SIZE_BYTES >= CORE_DS4_FLASH_MIN_GAP_BYTES);
CORE_DS4_LAYOUT_ASSERT(shared_abi_in_core_window,
	(CORE_DS4_SHARED_ABI_INFO_ADDR >= CORE_DS4_FLASH_CORE_START_ADDR) &&
	(CORE_DS4_SHARED_ABI_INFO_ADDR <= CORE_DS4_FLASH_CORE_END_ADDR));
CORE_DS4_LAYOUT_ASSERT(shared_api_in_core_window,
	(CORE_DS4_SHARED_API_TABLE_ADDR >= CORE_DS4_FLASH_CORE_START_ADDR) &&
	(CORE_DS4_SHARED_API_TABLE_ADDR <= CORE_DS4_FLASH_CORE_END_ADDR));

/* CORE code hard cap sits exactly at the ABI addr, strictly below the API table. */
CORE_DS4_LAYOUT_ASSERT(core_code_cap_at_abi,
	CORE_DS4_FLASH_CORE_CODE_CAP_ADDR == CORE_DS4_SHARED_ABI_INFO_ADDR);
CORE_DS4_LAYOUT_ASSERT(core_code_cap_below_table,
	CORE_DS4_FLASH_CORE_CODE_CAP_ADDR < CORE_DS4_SHARED_API_TABLE_ADDR);
/* Soft ceiling is a positive reserve below the hard cap and above CORE start. */
CORE_DS4_LAYOUT_ASSERT(soft_ceiling_below_cap,
	CORE_DS4_FLASH_CORE_SOFT_CEILING_ADDR < CORE_DS4_FLASH_CORE_CODE_CAP_ADDR);
CORE_DS4_LAYOUT_ASSERT(soft_ceiling_above_start,
	CORE_DS4_FLASH_CORE_SOFT_CEILING_ADDR > CORE_DS4_FLASH_CORE_START_ADDR);

#undef CORE_DS4_LAYOUT_ASSERT

#endif /* __CORE_CONTRACT_DS4_IMAGE_LAYOUT_H__ */
