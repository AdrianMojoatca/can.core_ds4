/*<#======================================================================#>*/
/*              CONTRACT WRAPPER PENTRU CONFIG_PTS_CTL.H (DB3)              */
/*<#======================================================================#>*/

#ifndef __PTS_CTL_CONFIG_WRAPPER_H__
#define __PTS_CTL_CONFIG_WRAPPER_H__

#include <stdint.h>

#define __CONFIG_PTS_CTL_H__

#define CORE_CONFIG_PTS_CTL_VERSION_DEFAULT   (1U)
#define CORE_PTS_CTL_TIMEOUT_DEFAULT          (750U)
#define CORE_PTS_CTL_PULSE_TIMEOUT_DEFAULT    (4000U)

#define CONFIG_PTS_CTL_VERSION  CORE_CONFIG_PTS_CTL_VERSION_DEFAULT
#define PTS_CTL_TIMEOUT         CORE_PTS_CTL_TIMEOUT_DEFAULT
#define PTS_CTL_PULSE_TIMEOUT   CORE_PTS_CTL_PULSE_TIMEOUT_DEFAULT

typedef struct {
    uint32_t config_pts_ctl_version;
    uint32_t pts_ctl_timeout;
    uint32_t pts_ctl_pulse_timeout;
} pts_ctl_config_contract_t;

extern pts_ctl_config_contract_t g_pts_ctl_config;

#ifdef PTS_CTL_CONFIG_DEFINE_STORAGE
pts_ctl_config_contract_t g_pts_ctl_config;
#endif

#define set_config_pts_ctl_version(value) (g_pts_ctl_config.config_pts_ctl_version = (uint32_t)(value))
#define get_config_pts_ctl_version() (g_pts_ctl_config.config_pts_ctl_version)
#define set_pts_ctl_timeout(value) (g_pts_ctl_config.pts_ctl_timeout = (uint32_t)(value))
#define get_pts_ctl_timeout() (g_pts_ctl_config.pts_ctl_timeout)
#define set_pts_ctl_pulse_timeout(value) (g_pts_ctl_config.pts_ctl_pulse_timeout = (uint32_t)(value))
#define get_pts_ctl_pulse_timeout() (g_pts_ctl_config.pts_ctl_pulse_timeout)

#define pts_ctl_config_set_defaults_from_defines() do { \
    set_config_pts_ctl_version((uint32_t)(CORE_CONFIG_PTS_CTL_VERSION_DEFAULT)); \
    set_pts_ctl_timeout((uint32_t)(CORE_PTS_CTL_TIMEOUT_DEFAULT)); \
    set_pts_ctl_pulse_timeout((uint32_t)(CORE_PTS_CTL_PULSE_TIMEOUT_DEFAULT)); \
} while(0)

#define PTS_CTL_CFG_CONFIG_PTS_CTL_VERSION (get_config_pts_ctl_version())
#define PTS_CTL_CFG_PTS_CTL_TIMEOUT (get_pts_ctl_timeout())
#define PTS_CTL_CFG_PTS_CTL_PULSE_TIMEOUT (get_pts_ctl_pulse_timeout())

void core_contract_db3_pts_ctl_config_init_defaults(void);
void core_contract_db3_pts_ctl_config_init_vehicle_specific(void);

#endif /* __PTS_CTL_CONFIG_WRAPPER_H__ */
