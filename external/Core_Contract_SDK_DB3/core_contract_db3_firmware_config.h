/*<#======================================================================#>*/
/*              CONTRACT WRAPPER PENTRU CONFIG_FIRMWARE.H (DB3)              */
/*<#======================================================================#>*/

#ifndef __CORE_CONTRACT_DB3_FIRMWARE_CONFIG_H__.
#define __CORE_CONTRACT_DB3_FIRMWARE_CONFIG_H__.

#include <stdint.h>

typedef struct {
    const char* fw_name;
    const char* app_version;
    const char* fw_info;
    uint32_t dei_ss_ignition_checking_timeout;
} firmware_config_contract_t;

extern firmware_config_contract_t g_firmware_config;

#ifdef FIRMWARE_CONFIG_DEFINE_STORAGE
firmware_config_contract_t g_firmware_config;
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

void core_contract_db3_firmware_config_init_defaults(void);
void core_contract_db3_firmware_config_init_vehicle_specific(void);

#endif /* __CORE_CONTRACT_DB3_FIRMWARE_CONFIG_H__.Core_Contract_SDK_DB3Core_Contract_SDK_DB3 */
