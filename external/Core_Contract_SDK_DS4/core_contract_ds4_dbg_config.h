/*<#======================================================================#>*/
/*              CONTRACT WRAPPER PENTRU CONFIG_DBG.H (DS4)              */
/*<#======================================================================#>*/

#ifndef __DBG_CONFIG_WRAPPER_H__
#define __DBG_CONFIG_WRAPPER_H__

#include <stdint.h>
#include "config_dbg.h"

typedef struct {
    uint32_t debug_text;
    uint32_t use_trace;
    uint32_t dbg_print_mem;
    uint32_t dbg_detect;
    uint32_t dbg_sense;
    uint32_t dbg_function;
    uint32_t dbg_cmd;
    uint32_t dbg_ctl_dl;
    uint32_t dbg_d2d;
    uint32_t dbg_eips;
    uint32_t dbg_obd_can;
    uint32_t dbg_prog;
    uint32_t dbg_reset;
    uint32_t dbg_rsr_feat;
    uint32_t dbg_rsr;
    uint32_t dbg_rxt;
    uint32_t dbg_secure_lock;
    uint32_t dbg_sts_r;
    uint32_t dbg_vin;
    uint32_t dbg_w2w;
    uint32_t dbg_wdog_v;
    uint32_t dbg_wake;
    uint32_t dbg_gpio;
    uint32_t dbg_pts_ctl;
} dbg_config_contract_t;

#if defined(FW_DS4_BUILD)
/* v1.1 single-instance: FW reads/writes CORE's single dbg config instance through
   the API-table DATA pointer (no separate FW storage). Mirrors firmware_config. */
dbg_config_contract_t* core_ds4_cfg_dbg(void);
#define g_dbg_config (*core_ds4_cfg_dbg())
#else
extern dbg_config_contract_t g_dbg_config;

#ifdef DBG_CONFIG_DEFINE_STORAGE
dbg_config_contract_t g_dbg_config;
#endif
#endif

#define set_debug_text(value) (g_dbg_config.debug_text = (uint32_t)(value))
#define get_debug_text() (g_dbg_config.debug_text)
#define set_use_trace(value) (g_dbg_config.use_trace = (uint32_t)(value))
#define get_use_trace() (g_dbg_config.use_trace)
#define set_dbg_print_mem(value) (g_dbg_config.dbg_print_mem = (uint32_t)(value))
#define get_dbg_print_mem() (g_dbg_config.dbg_print_mem)
#define set_dbg_detect(value) (g_dbg_config.dbg_detect = (uint32_t)(value))
#define get_dbg_detect() (g_dbg_config.dbg_detect)
#define set_dbg_sense(value) (g_dbg_config.dbg_sense = (uint32_t)(value))
#define get_dbg_sense() (g_dbg_config.dbg_sense)
#define set_dbg_function(value) (g_dbg_config.dbg_function = (uint32_t)(value))
#define get_dbg_function() (g_dbg_config.dbg_function)
#define set_dbg_cmd(value) (g_dbg_config.dbg_cmd = (uint32_t)(value))
#define get_dbg_cmd() (g_dbg_config.dbg_cmd)
#define set_dbg_ctl_dl(value) (g_dbg_config.dbg_ctl_dl = (uint32_t)(value))
#define get_dbg_ctl_dl() (g_dbg_config.dbg_ctl_dl)
#define set_dbg_d2d(value) (g_dbg_config.dbg_d2d = (uint32_t)(value))
#define get_dbg_d2d() (g_dbg_config.dbg_d2d)
#define set_dbg_eips(value) (g_dbg_config.dbg_eips = (uint32_t)(value))
#define get_dbg_eips() (g_dbg_config.dbg_eips)
#define set_dbg_obd_can(value) (g_dbg_config.dbg_obd_can = (uint32_t)(value))
#define get_dbg_obd_can() (g_dbg_config.dbg_obd_can)
#define set_dbg_prog(value) (g_dbg_config.dbg_prog = (uint32_t)(value))
#define get_dbg_prog() (g_dbg_config.dbg_prog)
#define set_dbg_reset(value) (g_dbg_config.dbg_reset = (uint32_t)(value))
#define get_dbg_reset() (g_dbg_config.dbg_reset)
#define set_dbg_rsr_feat(value) (g_dbg_config.dbg_rsr_feat = (uint32_t)(value))
#define get_dbg_rsr_feat() (g_dbg_config.dbg_rsr_feat)
#define set_dbg_rsr(value) (g_dbg_config.dbg_rsr = (uint32_t)(value))
#define get_dbg_rsr() (g_dbg_config.dbg_rsr)
#define set_dbg_rxt(value) (g_dbg_config.dbg_rxt = (uint32_t)(value))
#define get_dbg_rxt() (g_dbg_config.dbg_rxt)
#define set_dbg_secure_lock(value) (g_dbg_config.dbg_secure_lock = (uint32_t)(value))
#define get_dbg_secure_lock() (g_dbg_config.dbg_secure_lock)
#define set_dbg_sts_r(value) (g_dbg_config.dbg_sts_r = (uint32_t)(value))
#define get_dbg_sts_r() (g_dbg_config.dbg_sts_r)
#define set_dbg_vin(value) (g_dbg_config.dbg_vin = (uint32_t)(value))
#define get_dbg_vin() (g_dbg_config.dbg_vin)
#define set_dbg_w2w(value) (g_dbg_config.dbg_w2w = (uint32_t)(value))
#define get_dbg_w2w() (g_dbg_config.dbg_w2w)
#define set_dbg_wdog_v(value) (g_dbg_config.dbg_wdog_v = (uint32_t)(value))
#define get_dbg_wdog_v() (g_dbg_config.dbg_wdog_v)
#define set_dbg_wake(value) (g_dbg_config.dbg_wake = (uint32_t)(value))
#define get_dbg_wake() (g_dbg_config.dbg_wake)
#define set_dbg_gpio(value) (g_dbg_config.dbg_gpio = (uint32_t)(value))
#define get_dbg_gpio() (g_dbg_config.dbg_gpio)
#define set_dbg_pts_ctl(value) (g_dbg_config.dbg_pts_ctl = (uint32_t)(value))
#define get_dbg_pts_ctl() (g_dbg_config.dbg_pts_ctl)

#define dbg_config_set_defaults_from_defines() do { \
    set_debug_text((uint32_t)(DEBUG_TEXT)); \
    set_use_trace((uint32_t)(USE_TRACE)); \
    set_dbg_print_mem((uint32_t)(DBG_PRINT_MEM)); \
    set_dbg_detect((uint32_t)(DBG_DETECT)); \
    set_dbg_sense((uint32_t)(DBG_SENSE)); \
    set_dbg_function((uint32_t)(DBG_FUNCTION)); \
    set_dbg_cmd((uint32_t)(DBG_CMD)); \
    set_dbg_ctl_dl((uint32_t)(DBG_CTL_DL)); \
    set_dbg_d2d((uint32_t)(DBG_D2D)); \
    set_dbg_eips((uint32_t)(DBG_EIPS)); \
    set_dbg_obd_can((uint32_t)(DBG_OBD_CAN)); \
    set_dbg_prog((uint32_t)(DBG_PROG)); \
    set_dbg_reset((uint32_t)(DBG_RESET)); \
    set_dbg_rsr_feat((uint32_t)(DBG_RSR_FEAT)); \
    set_dbg_rsr((uint32_t)(DBG_RSR)); \
    set_dbg_rxt((uint32_t)(DBG_RXT)); \
    set_dbg_secure_lock((uint32_t)(DBG_SECURE_LOCK)); \
    set_dbg_sts_r((uint32_t)(DBG_STS_R)); \
    set_dbg_vin((uint32_t)(DBG_VIN)); \
    set_dbg_w2w((uint32_t)(DBG_W2W)); \
    set_dbg_wdog_v((uint32_t)(DBG_WDOG_V)); \
    set_dbg_wake((uint32_t)(DBG_WAKE)); \
    set_dbg_gpio((uint32_t)(DBG_GPIO)); \
    set_dbg_pts_ctl((uint32_t)(DBG_PTS_CTL));\
} while(0)

#define DBG_CFG_DEBUG_TEXT (get_debug_text())
#define DBG_CFG_USE_TRACE (get_use_trace())
#define DBG_CFG_DBG_PRINT_MEM (get_dbg_print_mem())
#define DBG_CFG_DBG_DETECT (get_dbg_detect())
#define DBG_CFG_DBG_SENSE (get_dbg_sense())
#define DBG_CFG_DBG_FUNCTION (get_dbg_function())
#define DBG_CFG_DBG_CMD (get_dbg_cmd())
#define DBG_CFG_DBG_CTL_DL (get_dbg_ctl_dl())
#define DBG_CFG_DBG_D2D (get_dbg_d2d())
#define DBG_CFG_DBG_EIPS (get_dbg_eips())
#define DBG_CFG_DBG_OBD_CAN (get_dbg_obd_can())
#define DBG_CFG_DBG_PROG (get_dbg_prog())
#define DBG_CFG_DBG_RESET (get_dbg_reset())
#define DBG_CFG_DBG_RSR_FEAT (get_dbg_rsr_feat())
#define DBG_CFG_DBG_RSR (get_dbg_rsr())
#define DBG_CFG_DBG_RXT (get_dbg_rxt())
#define DBG_CFG_DBG_SECURE_LOCK (get_dbg_secure_lock())
#define DBG_CFG_DBG_STS_R (get_dbg_sts_r())
#define DBG_CFG_DBG_VIN (get_dbg_vin())
#define DBG_CFG_DBG_W2W (get_dbg_w2w())
#define DBG_CFG_DBG_WDOG_V (get_dbg_wdog_v())
#define DBG_CFG_DBG_WAKE (get_dbg_wake())
#define DBG_CFG_DBG_GPIO (get_dbg_gpio())
#define DBG_CFG_DBG_PTS_CTL (get_dbg_pts_ctl())

void core_contract_ds4_dbg_config_init_defaults(void);
void core_contract_ds4_dbg_config_init_vehicle_specific(void);

#endif /* __DBG_CONFIG_WRAPPER_H__ */
