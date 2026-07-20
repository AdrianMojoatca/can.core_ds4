#ifndef __CORE_CONTRACT_DB3_CAPABILITIES_H__
#define __CORE_CONTRACT_DB3_CAPABILITIES_H__

#include <stdint.h>

typedef uint32_t core_db3_capabilities_mask_t;

#define CORE_DB3_CAPABILITY_GPIO             (1UL << 0)
#define CORE_DB3_CAPABILITY_CAN              (1UL << 1)
#define CORE_DB3_CAPABILITY_D2D              (1UL << 2)
#define CORE_DB3_CAPABILITY_DST              (1UL << 3)
#define CORE_DB3_CAPABILITY_TACH             (1UL << 4)
#define CORE_DB3_CAPABILITY_MISC             (1UL << 5)
#define CORE_DB3_CAPABILITY_RSR              (1UL << 6)
#define CORE_DB3_CAPABILITY_NVFS             (1UL << 7)
#define CORE_DB3_CAPABILITY_STATUS           (1UL << 8)

#define CORE_DB3_CAPABILITY_DEFAULT_MASK     (CORE_DB3_CAPABILITY_GPIO     | \
                                              CORE_DB3_CAPABILITY_CAN      | \
                                              CORE_DB3_CAPABILITY_D2D      | \
                                              CORE_DB3_CAPABILITY_DST      | \
                                              CORE_DB3_CAPABILITY_TACH     | \
                                              CORE_DB3_CAPABILITY_MISC     | \
                                              CORE_DB3_CAPABILITY_RSR      | \
                                              CORE_DB3_CAPABILITY_NVFS     | \
                                              CORE_DB3_CAPABILITY_STATUS)

static uint32_t core_contract_db3_capability_is_supported(
    core_db3_capabilities_mask_t capabilities_mask,
    core_db3_capabilities_mask_t required_capability)
{
    return ((capabilities_mask & required_capability) == required_capability) ? 1U : 0U;
}

#endif /* __CORE_CONTRACT_DB3_CAPABILITIES_H__ */
