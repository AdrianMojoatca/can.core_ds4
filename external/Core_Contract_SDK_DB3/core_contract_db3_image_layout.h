#ifndef __CORE_CONTRACT_DB3_IMAGE_LAYOUT_H__
#define __CORE_CONTRACT_DB3_IMAGE_LAYOUT_H__

#include <stdint.h>

/*
 * DB3 flash map for split-image rollout.
 *
 * Full app flash window (from current app.sct):
 *   0x00008100 .. 0x0003FEFF
 *
 * Split proposal used by build split tooling:
 *   CORE image: 0x00008100 .. 0x00027FFF
 *   FW image  : 0x00028000 .. 0x0003FEFF
 */

#define CORE_DB3_FLASH_APP_START_ADDR         (0x00008100UL)
#define CORE_DB3_FLASH_APP_END_ADDR           (0x0003FEFFUL)

#define CORE_DB3_FLASH_CORE_START_ADDR        (0x00008100UL)
#define CORE_DB3_FLASH_CORE_END_ADDR          (0x00027FFFUL)

#define CORE_DB3_FLASH_FW_START_ADDR          (0x00028000UL)
#define CORE_DB3_FLASH_FW_END_ADDR            (0x0003FEFFUL)

#define CORE_DB3_FLASH_CORE_SIZE_BYTES        (CORE_DB3_FLASH_CORE_END_ADDR - CORE_DB3_FLASH_CORE_START_ADDR + 1UL)
#define CORE_DB3_FLASH_FW_SIZE_BYTES          (CORE_DB3_FLASH_FW_END_ADDR - CORE_DB3_FLASH_FW_START_ADDR + 1UL)

#define CORE_DB3_IMAGE_LAYOUT_VERSION          (1U)
#define CORE_DB3_IMAGE_LAYOUT_FREEZE_DATE      (20260702UL)

/*
 * Reserved addresses for ABI metadata and API table publication in Core image.
 * These are contract addresses for split-image finalization phases.
 */
#define CORE_DB3_SHARED_ABI_INFO_ADDR          (0x00008120UL)
#define CORE_DB3_SHARED_API_TABLE_ADDR         (0x00008200UL)

#endif /* __CORE_CONTRACT_DB3_IMAGE_LAYOUT_H__ */
