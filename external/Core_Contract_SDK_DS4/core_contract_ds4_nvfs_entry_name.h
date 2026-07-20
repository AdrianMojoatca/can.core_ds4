#ifndef __CORE_CONTRACT_DS4_NVFS_ENTRY_NAME_H__
#define __CORE_CONTRACT_DS4_NVFS_ENTRY_NAME_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

char* nvfs_usr_entry_name_from_contract(char* dest, size_t dest_size, const char* prefix);

#ifdef __cplusplus
}
#endif

#endif /* __CORE_CONTRACT_DS4_NVFS_ENTRY_NAME_H__ */
