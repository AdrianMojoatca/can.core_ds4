#include "core_contract_ds4_nvfs_entry_name.h"
#include "core_contract_ds4_firmware_config.h"

#include <string.h>

char* nvfs_usr_entry_name_from_contract(char* dest, size_t dest_size, const char* prefix)
{
    const char* fw_name;
    size_t prefix_len;
    size_t fw_name_len;
    size_t max_copy_fw;

    if (dest == NULL || dest_size == 0U)
    {
        return NULL;
    }

    if (prefix == NULL)
    {
        prefix = "";
    }

    fw_name = get_fw_name();
    if (fw_name == NULL)
    {
        fw_name = "";
    }

    prefix_len = strlen(prefix);
    fw_name_len = strlen(fw_name);

    if (prefix_len >= dest_size)
    {
        memcpy(dest, prefix, dest_size - 1U);
        dest[dest_size - 1U] = '\0';
        return dest;
    }

    memcpy(dest, prefix, prefix_len);

    max_copy_fw = dest_size - prefix_len - 1U;
    if (fw_name_len > max_copy_fw)
    {
        fw_name_len = max_copy_fw;
    }

    memcpy(dest + prefix_len, fw_name, fw_name_len);
    dest[prefix_len + fw_name_len] = '\0';

    return dest;
}
