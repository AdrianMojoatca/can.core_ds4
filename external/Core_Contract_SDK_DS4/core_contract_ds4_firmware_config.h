/*<#======================================================================#>*/
/*              CONTRACT WRAPPER PENTRU CONFIG_FIRMWARE.H (DS4)              */
/*<#======================================================================#>*/

#ifndef __CORE_CONTRACT_DS4_FIRMWARE_CONFIG_H__
#define __CORE_CONTRACT_DS4_FIRMWARE_CONFIG_H__

#include <stdint.h>

typedef struct {
    const char* fw_name;
    const char* app_version;
    const char* fw_info;
    uint32_t dei_ss_ignition_checking_timeout;
} firmware_config_contract_t;

#if defined(FW_DS4_BUILD)
/* SS4 Slice 2 single-instance: FW reads/writes CORE's single config instance
   through the API-table DATA pointer (no separate FW storage). */
firmware_config_contract_t* core_ds4_cfg_firmware(void);
#define g_firmware_config (*core_ds4_cfg_firmware())
#else
extern firmware_config_contract_t g_firmware_config;

#ifdef FIRMWARE_CONFIG_DEFINE_STORAGE
firmware_config_contract_t g_firmware_config;
#endif
#endif

#define set_fw_name(value) (g_firmware_config.fw_name = (const char*)(value))
#define get_fw_name() (g_firmware_config.fw_name)
#define set_app_version(value) (g_firmware_config.app_version = (const char*)(value))
#define get_app_version() (g_firmware_config.app_version)
#define set_fw_info(value) (g_firmware_config.fw_info = (const char*)(value))
#define get_fw_info() (g_firmware_config.fw_info)
#define set_dei_ss_ignition_checking_timeout(value) (g_firmware_config.dei_ss_ignition_checking_timeout = (uint32_t)(value))
#define get_dei_ss_ignition_checking_timeout() (g_firmware_config.dei_ss_ignition_checking_timeout)

#define firmware_config_set_defaults_from_defines() do { } while(0)

#define FIRMWARE_CFG_FW_NAME (get_fw_name())
#define FIRMWARE_CFG_APP_VERSION (get_app_version())
#define FIRMWARE_CFG_FW_INFO (get_fw_info())
#define FIRMWARE_CFG_DEI_SS_IGNITION_CHECKING_TIMEOUT (get_dei_ss_ignition_checking_timeout())

void core_contract_ds4_firmware_config_init_defaults(void);
void core_contract_ds4_firmware_config_init_vehicle_specific(void);

#endif /* __CORE_CONTRACT_DS4_FIRMWARE_CONFIG_H__ */
