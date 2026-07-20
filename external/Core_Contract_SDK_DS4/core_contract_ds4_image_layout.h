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
 *   CORE image: 0x00008100 .. 0x00037FFF   (code capped by scatter at ABI addr 0x00037000)
 *   FW image  : 0x00038000 .. 0x0003FEFF   (base 0x38000, 32 KB-sector aligned = LPC176x
 *                                            sector 21 start, so the FW is one eraseable
 *                                            sector and can be flashed independently of CORE)
 *
 * WHY 0x38000 (not the DB3-inherited 0x28000): DS4 is a much larger image than DB3
 * (NGMM + DEI + AS features stay in CORE). Measured CORE ~170 KB, which overflows a
 * 0x28000 base (only ~124 KB below it). 0x38000 is the lowest 32 KB-aligned base that
 * still fits CORE (~188 KB budget, ~18 KB margin) while leaving ~32 KB for the thin FW.
 * DS4 is GREENFIELD: this base is NOT frozen until the first DS4 thin FW ships.
 *
 * Whole-chip context (LPC176x, 256 KB flash 0x00000..0x3FFFF):
 *   0x00000000 .. 0x00005FFF  bootloader code            (boot_1700.sct ER_CODE_0, 0x6000)
 *   0x00006000 .. 0x00007FFF  NVRAM/NVFS flash "Block #0" (boot_1700.sct, 8 KB, 4k-aligned,
 *                                                           bootloader-owned via BootService())
 *   0x00008000 .. 0x000080FF  ABOUT block
 *   0x00008100 .. 0x00037000  CORE code
 *   0x00037000 / 0x00037040   ABI info / API table (shared, __AT-fixed)
 *   0x00038000 .. 0x0003FEFF  FW image                   (flash sector 21)
 *
 * BOUNDARY / GROWTH POLICY:
 *   - NVFS is NOT in the application window. It lives at 0x6000 (below the app) and is
 *     reached only through bootloader BootService() traps; neither CORE nor FW hold its
 *     sector addresses. FW grows UPWARD (away from NVFS), so FW growth cannot corrupt NVFS.
 *   - CORE<->FW is protected by hard linker caps: CORE code region ends at 0x37000 and the
 *     FW region ends at 0x3FEFF; exceeding either is a LINK error (region overflow), never a
 *     silent overlap. GAP between CORE and FW is intentionally 0 (adjacent) -- the ABI+table
 *     occupy 0x37000..0x37FFF between CORE code and the FW base.
 *   - SOFT CEILING (policy, not linker-enforced): keep CORE code below
 *     CORE_DS4_FLASH_CORE_SOFT_CEILING_ADDR, i.e. a 4 KB reserve under the 0x37000 hard cap.
 *     ArmLink cannot "warn and continue", so enforce this with a post-build map check:
 *       ER_IROM_APP1 (Base 0x8100) + Size  must stay < 0x00036000.
 *     If it creeps past the soft ceiling, plan CORE reduction (or, while still greenfield,
 *     revisit the FW base) BEFORE hitting the 0x37000 hard cap.
 */

#define CORE_DS4_FLASH_APP_START_ADDR         (0x00008100UL)
#define CORE_DS4_FLASH_APP_END_ADDR           (0x0003FEFFUL)

#define CORE_DS4_FLASH_CORE_START_ADDR        (0x00008100UL)
#define CORE_DS4_FLASH_CORE_END_ADDR          (0x00037FFFUL)

#define CORE_DS4_FLASH_FW_START_ADDR          (0x00038000UL)
#define CORE_DS4_FLASH_FW_END_ADDR            (0x0003FEFFUL)

#define CORE_DS4_FLASH_GAP_START_ADDR         (CORE_DS4_FLASH_CORE_END_ADDR + 1UL)
#define CORE_DS4_FLASH_GAP_END_ADDR           (CORE_DS4_FLASH_FW_START_ADDR - 1UL)
#define CORE_DS4_FLASH_GAP_SIZE_BYTES         (CORE_DS4_FLASH_FW_START_ADDR - CORE_DS4_FLASH_CORE_END_ADDR - 1UL)

/*
 * Minimum reserved growth gap between CORE and FW images.
 * Frozen at 0: CORE and FW are adjacent by design; the hard linker caps (CORE code end
 * 0x37000, FW end 0x3FEFF) make a physical gap unnecessary for overlap safety.
 */
#define CORE_DS4_FLASH_MIN_GAP_BYTES          (0UL)

/*
 * CORE code hard cap: the CORE scatter (app.sct ER_IROM_APP1, Base 0x8100, Size 0x2EF00)
 * ends exactly at the ABI info address 0x37000. Growing past it is a link error.
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

#define CORE_DS4_IMAGE_LAYOUT_VERSION          (3U)
#define CORE_DS4_IMAGE_LAYOUT_FREEZE_DATE      (20260720UL)

/*
 * Reserved addresses for ABI metadata and API table publication in Core image.
 * NOTE:
 *   - Do not place shared contract data inside 0x00008100..startup region.
 *   - Keep this window in upper CORE flash and reserve it explicitly in CORE scatter.
 *   - ABI info 0x37000, API table 0x37040 (0x40 gap); FW base 0x38000 -> table window
 *     0x38000 - 0x37040 = 0xFC0 = 4032 bytes (unchanged from the original design).
 */
#define CORE_DS4_SHARED_ABI_INFO_ADDR          (0x00037000UL)
#define CORE_DS4_SHARED_API_TABLE_ADDR         (0x00037040UL)

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
