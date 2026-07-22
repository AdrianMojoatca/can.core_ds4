#include "core_framework.h"
#include "core_contract_ds4_fw_api.h"
#include "core_contract_ds4_gpio_output.h"   /* pulls gpio_pl.h with the FW redirect (see that header) */
#include "core_contract_ds4_firmware_config.h"
#include "core_contract_ds4_can_config.h"
#include "core_contract_ds4_pts_ctl_config.h"
#include "core_contract_ds4_prog_config.h"
#include "core_contract_ds4_status_config.h"

#if defined(CORE_DS4_BUILD)

#include "os.h"         /* SS4 Slice 4: os_evt + os_tsk_wait (CORE only) */
#include "timeout.h"    /* SS4 Slice 5: timeout start/stop (CORE only)  */
#include "timeout_f.h"  /* SS4 Slice 5: timeout_f_create (CORE only)    */
#include "nvfs.h"       /* SS4 Slice 6: nvfs_rd/wr/... (CORE only)      */
#include "nvfs_usr.h"   /* SS4 Slice 6: nvfs_fw_data entry name         */
#include "can_usr.h"    /* SS4 Slice 7a: can_usr_* (CORE only)          */
#include "can_sync.h"   /* SS4 Slice 7a: can_sync_* (CORE only)         */
#include "can_per.h"    /* SS4 Slice 7a: can_per_init (CORE only)       */
#include "process.h"    /* SS4 Slice 7b: process_* (CORE only)          */
#include "tsk_rs.h"     /* SS4 Slice 7b: tsk_rs_* (CORE only)           */
#include "hazard.h"     /* SS4 Slice 7b: hazard_* (CORE only)           */
#include "lf.h"         /* SS4 Slice 7b: lf_init (CORE only)            */
#include "sense_rs_can.h"  /* SS4 Slice 8 (CORE only) */
#include "status_system.h" /* SS4 Slice 8 */
#include "d2d.h"           /* SS4 Slice 8 */
#include "gpio_pl.h"       /* SS4 Slice 8 */
#include "reset.h"         /* SS4 Slice 8 */
#include "wdog_v.h"        /* SS4 Slice 8 */
#include "timeout_q.h"     /* SS4 Slice 8 */
#include "led_detect.h"    /* SS4 Slice 8 */
#include "cmd.h"           /* SS4 Slice 8 (cmd_lut_insert_table1) */
#include "prog.h"          /* SS4 Slice 8d (prog_set_exit_callback) */

#if defined(__CC_ARM)
#define CORE_DS4_SHARED_ABI_INFO_PLACEMENT __attribute__((at(CORE_DS4_SHARED_ABI_INFO_ADDR), used))
#define CORE_DS4_SHARED_API_TABLE_PLACEMENT __attribute__((at(CORE_DS4_SHARED_API_TABLE_ADDR), used))
#elif defined(__GNUC__)
#define CORE_DS4_SHARED_ABI_INFO_PLACEMENT __attribute__((section("CORE_DS4_SHARED_ABI_INFO"), used))
#define CORE_DS4_SHARED_API_TABLE_PLACEMENT __attribute__((section("CORE_DS4_SHARED_API_TABLE"), used))
#else
#define CORE_DS4_SHARED_ABI_INFO_PLACEMENT
#define CORE_DS4_SHARED_API_TABLE_PLACEMENT
#endif

static const core_ds4_abi_info_t* core_contract_ds4_shared_abi_info_get(void);
extern Boolean dbg_print(const char *str, ...);   /* SS4 Slice 3: routed to FW via table. DS4 trace fn is dbg_print (DB3 used trace_print); TRACE()->dbg_print via dbgcli.h */

const core_ds4_abi_info_t g_core_ds4_shared_abi_info CORE_DS4_SHARED_ABI_INFO_PLACEMENT =
{
    CORE_DS4_ABI_MAGIC,
    CORE_DS4_ABI_VERSION_MAJOR,
    CORE_DS4_ABI_VERSION_MINOR,
    CORE_DS4_API_TABLE_VERSION,
    CORE_DS4_CAPABILITY_DEFAULT_MASK,
    0U
};

const core_ds4_api_table_t g_core_ds4_shared_api_table CORE_DS4_SHARED_API_TABLE_PLACEMENT =
{
    CORE_DS4_API_TABLE_MAGIC,
    sizeof(core_ds4_api_table_t),
    CORE_DS4_ABI_VERSION_MAJOR,
    CORE_DS4_ABI_VERSION_MINOR,
    CORE_DS4_API_TABLE_VERSION,
    CORE_DS4_CAPABILITY_DEFAULT_MASK,

    core_contract_ds4_shared_abi_info_get,

    core_contract_ds4_api_gpio_handbrake_set,
    core_contract_ds4_api_gpio_doors_set,
    core_contract_ds4_api_gpio_lights_set,
    core_contract_ds4_api_gpio_trunk_set,
    core_contract_ds4_api_gpio_brake_set,
    core_contract_ds4_api_gpio_pts1_set,
    core_contract_ds4_api_gpio_pts2_set,
    core_contract_ds4_api_gpio_rap_set,
    core_contract_ds4_api_gpio_rf_loop_set,
    core_contract_ds4_api_gpio_ignition_set,
    core_contract_ds4_api_gpio_hood_set,
    core_contract_ds4_api_gpio_key_wrap_set,

    core_contract_ds4_gpio_rf_loop_toggle,

    /* GROUP: config - DATA pointers to CORE's single config instances (SS4 Slice 2) */
    &g_firmware_config,
    &g_can_config,
    &g_pts_ctl_config,
    &g_prog_config,
    &g_status_config,

    /* GROUP: diag (SS4 Slice 3) */
    (core_ds4_fn_trace_t)dbg_print,

    /* GROUP: os/rtos (SS4 Slice 4) */
    (core_ds4_fn_ptr_void)os_evt_create,
    (core_ds4_fn_evt_clr_t)os_evt_clr,
    (core_ds4_fn_evt_v_t)os_evt_signal_v,
    (core_ds4_fn_evt_v_t)os_evt_wait_v,
    (core_ds4_fn_void_u32)os_tsk_wait,

    /* GROUP: timeout (SS4 Slice 5) */
    (core_ds4_fn_timeout_f_t)timeout_f_create,
    (core_ds4_fn_void_ptr)timeout_start,
    (core_ds4_fn_void_ptr)timeout_stop,

    /* GROUP: nvfs (SS4 Slice 6) */
    (core_ds4_fn_nvfs_rw_t)nvfs_rd,
    (core_ds4_fn_nvfs_rw_t)nvfs_wr,
    (core_ds4_fn_nvfs_wrc_t)nvfs_wr_create,
    (core_ds4_fn_nvfs_del_t)nvfs_delete,
    (core_ds4_fn_u32_void)nvram_usr_commit,

    /* GROUP: can (SS4 Slice 7a) */
    (core_ds4_fn_void_u32)can_per_init,
    (core_ds4_fn_void_u32)can_usr_init,
    (core_ds4_fn_void_u32_ptr)can_usr_rx_reg_fast,
    (core_ds4_fn_void_u32_ptr_u32)can_usr_rx_reg_slow,
    (core_ds4_fn_can_tx_t)can_usr_tx,
    (core_ds4_fn_void_u32_u32)can_sync_init,
    (core_ds4_fn_can_sync_add_t)can_sync_add,
    (core_ds4_fn_u32_u32_ptr)can_sync_remove,

    /* GROUP: process/task/hazard (SS4 Slice 7b) */
    (core_ds4_fn_ptr_ptr)process_create_cfg,
    (core_ds4_fn_void_ptr_u32)process_launch,
    (core_ds4_fn_void_void)tsk_rs_init,
    (core_ds4_fn_void_u32_ptr)tsk_rs_register,
    (core_ds4_fn_void_u32)tsk_rs_update_cfg,
    (core_ds4_fn_void_ptr_u32)hazard_init,
    (core_ds4_fn_void_u32)hazard_status_update,
    (core_ds4_fn_void_void)lf_init,

    /* GROUP: sense_rs (SS4 Slice 8) */
    (core_ds4_fn_void_ptr)sense_rs_can_init,
    (core_ds4_fn_u32_u32)sense_rs_ign_can,
    (core_ds4_fn_u32_u32)sense_rs_brake_can,
    (core_ds4_fn_u32_u32)sense_rs_driver_door_can,
    (core_ds4_fn_u32_u32)sense_rs_other_doors_can,
    (core_ds4_fn_u32_u32)sense_rs_handbrake_can,
    (core_ds4_fn_u32_u32)sense_rs_hood_can,
    (core_ds4_fn_u32_u32)sense_rs_oem_lock_can,
    (core_ds4_fn_u32_u32)sense_rs_oem_trunk_can,
    (core_ds4_fn_u32_u32)sense_rs_oem_unlock_can,
    (core_ds4_fn_void_u32)sense_rs_rpm_can,
    (core_ds4_fn_u32_u32)sense_rs_skd_can,
    (core_ds4_fn_void_u32)sense_rs_speed_can,
    (core_ds4_fn_u32_u32)sense_rs_trunk_can,
    (core_ds4_fn_void_void)sense_rs_can_brake_skip_enter,
    (core_ds4_fn_void_void)sense_rs_can_brake_skip_leave,

    /* GROUP: status_system (SS4 Slice 8) */
    (core_ds4_fn_void_u32)set_brake_source,
    (core_ds4_fn_void_u32)set_door_source,
    (core_ds4_fn_void_u32)set_handbrake_source,
    (core_ds4_fn_void_u32)set_ign_source,
    (core_ds4_fn_void_u32)set_rpm_source,
    (core_ds4_fn_void_u32)set_speed_source,
    (core_ds4_fn_void_u32)set_trunk_source,
    (core_ds4_fn_void_u32)set_rke_lock_use,
    (core_ds4_fn_void_u32)set_rke_trunk_use,
    (core_ds4_fn_void_u32)set_rke_unlock_use,
    (core_ds4_fn_void_u32)set_rs_gwr,
    (core_ds4_fn_void_u32)set_rs_ign,
    (core_ds4_fn_void_u32)set_rs_start,
    (core_ds4_fn_u32_void)get_ign_status,
    (core_ds4_fn_u32_void)get_rs_gwr,
    (core_ds4_fn_u32_void)get_ss_lock,
    (core_ds4_fn_u32_void)get_trunk_status,

    /* GROUP: misc (SS4 Slice 8) */
    (core_ds4_fn_void_void)led_detect_started,
    (core_ds4_fn_void_void)led_detect_done,
    (core_ds4_fn_void_void)led_detect_in_progress,
    (core_ds4_fn_void_void)led_detect_err_can0,
    (core_ds4_fn_void_void)led_detect_err_invalid_ignition,
    (core_ds4_fn_void_u32)d2d_tx,
    (core_ds4_fn_void_u32)gpio_pl_set,
    (core_ds4_fn_void_u32)gpio_pl_clr,
    (core_ds4_fn_void_ptr)reset_register,
    (core_ds4_fn_void_u32_ptr_u32)wdog_v_force_reset,
    (core_ds4_fn_void_ptr_u32_u32)cmd_lut_insert_table1,
    (core_ds4_fn_timeout_q_ins_t)timeout_q_insert,
    (core_ds4_fn_u32_ptr)timeout_q_remove,
    &nvfs_fw_data,

    /* GROUP: prog: prog_set_exit_callback is a no-op macro on PLATFORM_933 (DS4) -
       there is no real function to route, and the FW-side call also expands to `;`.
       Slot left NULL on DS4. (DB3 routed a real prog_set_exit_callback here.) */
    0,

    /* GROUP: dbg config (v1.1) - DATA pointer to CORE's single dbg config instance */
    &g_dbg_config,

    /* GROUP: DS4-added routing (S5) - real CORE fns for the 4 DS4-only symbols the
       vehicle calls (DB3 did not route these). Order matches the struct fields. */
    (core_ds4_fn_u32_u32)prog_retrieve_option,
    (core_ds4_fn_cmd_q_t)cmd_q_insert,
    (core_ds4_fn_can_per_add_t)can_per_add,
    (core_ds4_fn_u32_ptr)can_per_remove,

    {0}
};

static const core_ds4_abi_info_t* core_contract_ds4_shared_abi_info_get(void)
{
    return &g_core_ds4_shared_abi_info;
}

#endif

void gpio_iface_doors_open(void)
{
    core_contract_ds4_fw_gpio_doors_open();
}

void gpio_iface_doors_closed(void)
{
    core_contract_ds4_fw_gpio_doors_closed();
}

void gpio_iface_trunk_open(void)
{
    core_contract_ds4_fw_gpio_trunk_open();
}

void gpio_iface_trunk_closed(void)
{
    core_contract_ds4_fw_gpio_trunk_closed();
}

void gpio_iface_handbrake_on(void)
{
    core_contract_ds4_fw_gpio_handbrake_on();
}

void gpio_iface_handbrake_off(void)
{
    core_contract_ds4_fw_gpio_handbrake_off();
}

void gpio_iface_lights_on(void)
{
    core_contract_ds4_fw_gpio_lights_on();
}

void gpio_iface_lights_off(void)
{
    core_contract_ds4_fw_gpio_lights_off();
}

void gpio_iface_brake(Boolean state)
{
    core_contract_ds4_fw_gpio_brake_set(state);
}

void gpio_iface_rf_loop(Boolean state)
{
    core_contract_ds4_fw_gpio_rf_loop_set(state);
}

void gpio_iface_pts_1(Boolean state)
{
    core_contract_ds4_fw_gpio_pts1_set(state);
}

void gpio_iface_pts_2(Boolean state)
{
    core_contract_ds4_fw_gpio_pts2_set(state);
}

void gpio_iface_rap(Boolean state)
{
    core_contract_ds4_fw_gpio_rap_set(state);
}

void gpio_iface_ignition_on(void)
{
    core_contract_ds4_fw_gpio_ignition_on();
}

void gpio_iface_ignition_off(void)
{
    core_contract_ds4_fw_gpio_ignition_off();
}

void gpio_iface_hood_open(void)
{
    core_contract_ds4_fw_gpio_hood_open();
}

void gpio_iface_hood_closed(void)
{
    core_contract_ds4_fw_gpio_hood_closed();
}

void gpio_iface_key_wrap_on(void)
{
    core_contract_ds4_fw_gpio_key_wrap_on();
}

void gpio_iface_key_wrap_off(void)
{
    core_contract_ds4_fw_gpio_key_wrap_off();
}

void gpio_iface_led_red_on(void)
{
    core_contract_ds4_gpio_led_red_on();
}

void gpio_iface_led_red_off(void)
{
    core_contract_ds4_gpio_led_red_off();
}

void gpio_iface_led_green_on(void)
{
    core_contract_ds4_gpio_led_green_on();
}

void gpio_iface_led_green_off(void)
{
    core_contract_ds4_gpio_led_green_off();
}

void gpio_iface_led_blue_on(void)
{
    core_contract_ds4_gpio_led_blue_on();
}

void gpio_iface_led_blue_off(void)
{
    core_contract_ds4_gpio_led_blue_off();
}

void gpio_iface_rsr_lock_on(void)
{
    core_contract_ds4_gpio_rsr_lock_on();
}

void gpio_iface_rsr_lock_off(void)
{
    core_contract_ds4_gpio_rsr_lock_off();
}

void gpio_iface_rsr_unlock_on(void)
{
    core_contract_ds4_gpio_rsr_unlock_on();
}

void gpio_iface_rsr_unlock_off(void)
{
    core_contract_ds4_gpio_rsr_unlock_off();
}

void gpio_iface_rsr_trunk_on(void)
{
    core_contract_ds4_gpio_rsr_trunk_on();
}

void gpio_iface_rsr_trunk_off(void)
{
    core_contract_ds4_gpio_rsr_trunk_off();
}

void gpio_iface_rf_loop_toggle(void)
{
    core_contract_ds4_fw_gpio_rf_loop_toggle();
}
