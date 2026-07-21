#ifndef __CORE_CONTRACT_DS4_API_TABLE_H__
#define __CORE_CONTRACT_DS4_API_TABLE_H__

#include <stdint.h>

#include "core_contract_ds4_abi.h"
#include "core_contract_ds4_capabilities.h"
#include "core_contract_ds4_image_layout.h"
#include "core_contract_ds4_firmware_config.h"
#include "core_contract_ds4_can_config.h"
#include "core_contract_ds4_pts_ctl_config.h"
#include "core_contract_ds4_prog_config.h"
#include "core_contract_ds4_status_config.h"
#include "core_contract_ds4_dbg_config.h"

#define CORE_DS4_API_TABLE_MAGIC             (0x41505334UL)   /* 'APS4' */
#define CORE_DS4_API_TABLE_RESERVED_SLOTS    (16U)

typedef enum
{
    CORE_DS4_API_FN_GPIO_HANDBRAKE_SET = 0,
    CORE_DS4_API_FN_GPIO_DOORS_SET,
    CORE_DS4_API_FN_GPIO_LIGHTS_SET,
    CORE_DS4_API_FN_GPIO_TRUNK_SET,
    CORE_DS4_API_FN_GPIO_BRAKE_SET,
    CORE_DS4_API_FN_GPIO_PTS1_SET,
    CORE_DS4_API_FN_GPIO_PTS2_SET,
    CORE_DS4_API_FN_GPIO_RAP_SET,
    CORE_DS4_API_FN_GPIO_RF_LOOP_SET,
    CORE_DS4_API_FN_GPIO_IGNITION_SET,
    CORE_DS4_API_FN_GPIO_HOOD_SET,
    CORE_DS4_API_FN_GPIO_KEY_WRAP_SET,
    CORE_DS4_API_FN_COUNT
} core_ds4_api_function_id_t;

#define CORE_DS4_API_TABLE_FUNCTION_COUNT    ((uint32_t)CORE_DS4_API_FN_COUNT)

typedef void (*core_ds4_api_gpio_state_set_fn_t)(uint32_t state_value);
typedef void (*core_ds4_api_gpio_toggle_fn_t)(void);
typedef const core_ds4_abi_info_t* (*core_ds4_api_get_abi_info_fn_t)(void);

/*
 * Canonical function-pointer typedefs, reused across API groups so the table
 * stays compact and the (auto-generated) FW shim has a small type vocabulary.
 * Widths are fixed (uint32_t; Boolean carried as uint32_t) so CORE and FW agree
 * regardless of enum-int settings. Add new canonical shapes here as needed.
 */
typedef void     (*core_ds4_fn_void_void)(void);
typedef void     (*core_ds4_fn_void_u32)(uint32_t);
typedef uint32_t (*core_ds4_fn_u32_void)(void);
typedef uint32_t (*core_ds4_fn_u32_u32)(uint32_t);
typedef void*    (*core_ds4_fn_ptr_void)(void);
typedef void     (*core_ds4_fn_void_ptr)(void*);
typedef void     (*core_ds4_fn_trace_t)(const char*, ...);   /* variadic (trace_print) */
typedef void     (*core_ds4_fn_evt_clr_t)(void* evt, uint32_t flags);
typedef uint32_t (*core_ds4_fn_evt_v_t)(void* evt, ...);     /* variadic (os_evt_signal_v/wait_v) */
typedef void*    (*core_ds4_fn_timeout_f_t)(uint32_t timeout_ms, core_ds4_fn_void_ptr cb); /* timeout_f_create */
typedef uint32_t (*core_ds4_fn_nvfs_rw_t)(const char* name, void* buf);                    /* nvfs_rd / nvfs_wr */
typedef uint32_t (*core_ds4_fn_nvfs_wrc_t)(const char* name, const void* buf, uint32_t size); /* nvfs_wr_create */
typedef uint32_t (*core_ds4_fn_nvfs_del_t)(const char* name);                              /* nvfs_delete */
typedef void     (*core_ds4_fn_void_u32_ptr)(uint32_t, void*);                             /* can_usr_rx_reg_fast */
typedef void     (*core_ds4_fn_void_u32_ptr_u32)(uint32_t, void*, uint32_t);               /* can_usr_rx_reg_slow */
typedef uint32_t (*core_ds4_fn_can_tx_t)(uint32_t dev, void* msg, uint32_t fast, uint32_t timeout); /* can_usr_tx */
typedef void     (*core_ds4_fn_void_u32_u32)(uint32_t, uint32_t);                          /* can_sync_init */
typedef uint32_t (*core_ds4_fn_can_sync_add_t)(uint32_t, void*, void*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t); /* can_sync_add (8 args) */
typedef uint32_t (*core_ds4_fn_u32_u32_ptr)(uint32_t, void*);                              /* can_sync_remove */
typedef void*    (*core_ds4_fn_ptr_ptr)(void*);                                            /* process_create_cfg */
typedef void     (*core_ds4_fn_void_ptr_u32)(void*, uint32_t);                             /* process_launch, hazard_init */
typedef uint32_t (*core_ds4_fn_timeout_q_ins_t)(void*, void*, uint32_t, uint32_t, uint32_t); /* timeout_q_insert */
typedef uint32_t (*core_ds4_fn_u32_ptr)(void*);                                            /* timeout_q_remove */
typedef void     (*core_ds4_fn_void_ptr_u32_u32)(void*, uint32_t, uint32_t);               /* cmd_lut_insert_table1 */
typedef uint32_t (*core_ds4_fn_cmd_q_t)(uint32_t, uint32_t, ...);                           /* cmd_q_insert (variadic) */
typedef uint32_t (*core_ds4_fn_can_per_add_t)(void*, void*, uint32_t, uint32_t, uint32_t);  /* can_per_add (5 args) */

/*
 * CORE->FW API table.  ABI RULE: APPEND-ONLY. Never reorder or remove a field
 * (field position == ABI). New services are appended just before reserved_slots
 * and consume reserved headroom; bump CORE_DS4_API_TABLE_VERSION on every change
 * and CORE_DS4_ABI_VERSION_MINOR on additive ones. A consumer must range-check
 * (table_size / capabilities_mask) before dereferencing newer fields.
 *
 * Group build-out (thin-FW slices): [gpio] exists today; [can] [sense] [rsr]
 * [process] [os] [nvfs] [timeout] [misc] service pointers and the config DATA
 * pointers are appended in later slices.
 */
typedef struct
{
    /* ---- header ---- */
    uint32_t table_magic;
    uint32_t table_size;
    uint16_t abi_version_major;
    uint16_t abi_version_minor;
    uint32_t api_table_version;
    core_ds4_capabilities_mask_t capabilities_mask;

    core_ds4_api_get_abi_info_fn_t get_abi_info;

    /* ---- GROUP: gpio ---- */
    core_ds4_api_gpio_state_set_fn_t gpio_handbrake_set;
    core_ds4_api_gpio_state_set_fn_t gpio_doors_set;
    core_ds4_api_gpio_state_set_fn_t gpio_lights_set;
    core_ds4_api_gpio_state_set_fn_t gpio_trunk_set;
    core_ds4_api_gpio_state_set_fn_t gpio_brake_set;
    core_ds4_api_gpio_state_set_fn_t gpio_pts1_set;
    core_ds4_api_gpio_state_set_fn_t gpio_pts2_set;
    core_ds4_api_gpio_state_set_fn_t gpio_rap_set;
    core_ds4_api_gpio_state_set_fn_t gpio_rf_loop_set;
    core_ds4_api_gpio_state_set_fn_t gpio_ignition_set;
    core_ds4_api_gpio_state_set_fn_t gpio_hood_set;
    core_ds4_api_gpio_state_set_fn_t gpio_key_wrap_set;

    core_ds4_api_gpio_toggle_fn_t gpio_rf_loop_toggle;

    /* ---- GROUP: config (DATA pointers to CORE's single config instances; SS4 Slice 2) ---- */
    firmware_config_contract_t*  firmware_config;
    can_config_contract_t*       can_config;
    pts_ctl_config_contract_t*   pts_ctl_config;
    prog_config_contract_t*      prog_config;
    status_config_contract_t*    status_config;

    /* ---- GROUP: diag (SS4 Slice 3) ---- */
    core_ds4_fn_trace_t trace_print;

    /* ---- GROUP: os/rtos (SS4 Slice 4); Evt handled opaque as void* ---- */
    core_ds4_fn_ptr_void  os_evt_create_fn;   /* Evt os_evt_create(void)          */
    core_ds4_fn_evt_clr_t os_evt_clr_fn;      /* void os_evt_clr(Evt, UInt16)     */
    core_ds4_fn_evt_v_t   os_evt_signal_fn;   /* Boolean os_evt_signal_v(Evt,...) */
    core_ds4_fn_evt_v_t   os_evt_wait_fn;     /* UInt16  os_evt_wait_v(Evt,...)    */
    core_ds4_fn_void_u32  os_tsk_wait_fn;     /* void os_tsk_wait(TskTimeout)     */

    /* ---- GROUP: timeout (SS4 Slice 5); Timeout handled opaque as void* ---- */
    core_ds4_fn_timeout_f_t timeout_f_create_fn; /* Timeout timeout_f_create(UInt32, Exec_Func) */
    core_ds4_fn_void_ptr    timeout_start_fn;     /* void timeout_start(Timeout) */
    core_ds4_fn_void_ptr    timeout_stop_fn;      /* void timeout_stop(Timeout)  */

    /* ---- GROUP: nvfs (SS4 Slice 6); NVFS_Status as uint32_t ---- */
    core_ds4_fn_nvfs_rw_t   nvfs_rd_fn;           /* NVFS_Status nvfs_rd(name, dst)          */
    core_ds4_fn_nvfs_rw_t   nvfs_wr_fn;           /* NVFS_Status nvfs_wr(name, buf)          */
    core_ds4_fn_nvfs_wrc_t  nvfs_wr_create_fn;    /* NVFS_Status nvfs_wr_create(name,buf,sz) */
    core_ds4_fn_nvfs_del_t  nvfs_delete_fn;       /* NVFS_Status nvfs_delete(name)           */
    core_ds4_fn_u32_void    nvram_usr_commit_fn;  /* Boolean nvram_usr_commit(void)          */
    /* NOTE: nvfs_fw_data (DATA entry name) deferred to Slice 8 - see routing via &address */

    /* ---- GROUP: can (SS4 Slice 7a); Can_Dev_Id/Can_Usr_Setup as uint32_t, Can_Msg* as void* ---- */
    core_ds4_fn_void_u32          can_per_init_fn;        /* void can_per_init(UInt8)                       */
    core_ds4_fn_void_u32          can_usr_init_fn;        /* void can_usr_init(Can_Usr_Setup)               */
    core_ds4_fn_void_u32_ptr      can_usr_rx_reg_fast_fn; /* void (Can_Dev_Id, Reg_Func) - ISR context!     */
    core_ds4_fn_void_u32_ptr_u32  can_usr_rx_reg_slow_fn; /* void (Can_Dev_Id, Reg_Func, UInt32 can_id)     */
    core_ds4_fn_can_tx_t          can_usr_tx_fn;          /* Boolean can_usr_tx(dev, Can_Msg*, fast, to)    */
    core_ds4_fn_void_u32_u32      can_sync_init_fn;       /* void can_sync_init(Can_Dev_Id, UInt8)          */
    core_ds4_fn_can_sync_add_t    can_sync_add_fn;        /* Boolean can_sync_add(8 args)                   */
    core_ds4_fn_u32_u32_ptr       can_sync_remove_fn;     /* Boolean can_sync_remove(Can_Dev_Id, void*)     */

    /* ---- GROUP: process/task/hazard (SS4 Slice 7b); Process_Node/cfg via void*, enums as uint32_t ---- */
    core_ds4_fn_ptr_ptr       process_create_cfg_fn;   /* Process_Node* process_create_cfg(Process_Cfg*)  */
    core_ds4_fn_void_ptr_u32  process_launch_fn;        /* void process_launch(Process_Node*, UInt32)      */
    core_ds4_fn_void_void     tsk_rs_init_fn;           /* void tsk_rs_init(void)                          */
    core_ds4_fn_void_u32_ptr  tsk_rs_register_fn;       /* void tsk_rs_register(Tsk_Rs_Evt, Tsk_Rs_Func)   */
    core_ds4_fn_void_u32      tsk_rs_update_cfg_fn;     /* void tsk_rs_update_cfg(Tsk_Rs_Takeover)         */
    core_ds4_fn_void_ptr_u32  hazard_init_fn;           /* void hazard_init(Hazard_Pulse_Func, Hazard_Cfg) */
    core_ds4_fn_void_u32      hazard_status_update_fn;  /* void hazard_status_update(Boolean)              */
    core_ds4_fn_void_void     lf_init_fn;               /* void lf_init(void)                              */

    /* ---- GROUP: sense_rs (SS4 Slice 8); Boolean(Boolean)=u32_u32, cfg via void* ---- */
    core_ds4_fn_void_ptr  sense_rs_can_init_fn;              /* void sense_rs_can_init(Sense_Rs_Can_Cfg*) */
    core_ds4_fn_u32_u32   sense_rs_ign_can_fn;              /* Boolean(Boolean) */
    core_ds4_fn_u32_u32   sense_rs_brake_can_fn;
    core_ds4_fn_u32_u32   sense_rs_driver_door_can_fn;
    core_ds4_fn_u32_u32   sense_rs_other_doors_can_fn;
    core_ds4_fn_u32_u32   sense_rs_handbrake_can_fn;
    core_ds4_fn_u32_u32   sense_rs_hood_can_fn;
    core_ds4_fn_u32_u32   sense_rs_oem_lock_can_fn;
    core_ds4_fn_u32_u32   sense_rs_oem_trunk_can_fn;
    core_ds4_fn_u32_u32   sense_rs_oem_unlock_can_fn;
    core_ds4_fn_void_u32  sense_rs_rpm_can_fn;              /* void(UInt16) */
    core_ds4_fn_u32_u32   sense_rs_skd_can_fn;
    core_ds4_fn_void_u32  sense_rs_speed_can_fn;            /* void(UInt32) */
    core_ds4_fn_u32_u32   sense_rs_trunk_can_fn;
    core_ds4_fn_void_void sense_rs_can_brake_skip_enter_fn;
    core_ds4_fn_void_void sense_rs_can_brake_skip_leave_fn;

    /* ---- GROUP: status_system (SS4 Slice 8); setters void(enum)=void_u32, getters Boolean(void)=u32_void ---- */
    core_ds4_fn_void_u32  set_brake_source_fn;
    core_ds4_fn_void_u32  set_door_source_fn;
    core_ds4_fn_void_u32  set_handbrake_source_fn;
    core_ds4_fn_void_u32  set_ign_source_fn;
    core_ds4_fn_void_u32  set_rpm_source_fn;
    core_ds4_fn_void_u32  set_speed_source_fn;
    core_ds4_fn_void_u32  set_trunk_source_fn;
    core_ds4_fn_void_u32  set_rke_lock_use_fn;
    core_ds4_fn_void_u32  set_rke_trunk_use_fn;
    core_ds4_fn_void_u32  set_rke_unlock_use_fn;
    core_ds4_fn_void_u32  set_rs_gwr_fn;
    core_ds4_fn_void_u32  set_rs_ign_fn;
    core_ds4_fn_void_u32  set_rs_start_fn;
    core_ds4_fn_u32_void  get_ign_status_fn;
    core_ds4_fn_u32_void  get_rs_gwr_fn;
    core_ds4_fn_u32_void  get_ss_lock_fn;
    core_ds4_fn_u32_void  get_trunk_status_fn;

    /* ---- GROUP: misc (SS4 Slice 8) ---- */
    core_ds4_fn_void_void        led_detect_started_fn;
    core_ds4_fn_void_void        led_detect_done_fn;
    core_ds4_fn_void_void        led_detect_in_progress_fn;
    core_ds4_fn_void_void        led_detect_err_can0_fn;
    core_ds4_fn_void_void        led_detect_err_invalid_ignition_fn;
    core_ds4_fn_void_u32         d2d_tx_fn;                 /* void d2d_tx(UInt8)                 */
    core_ds4_fn_void_u32         gpio_pl_set_fn;            /* void gpio_pl_set(Gpio_Pl_T)       */
    core_ds4_fn_void_u32         gpio_pl_clr_fn;            /* void gpio_pl_clr(Gpio_Pl_T)       */
    core_ds4_fn_void_ptr         reset_register_fn;         /* void reset_register(Reset_Func)   */
    core_ds4_fn_void_u32_ptr_u32 wdog_v_force_reset_fn;     /* void(e_Wdog_v_Reset_Type,char*,UInt16) */
    core_ds4_fn_void_ptr_u32_u32 cmd_lut_insert_table1_fn;  /* void(const Cmd_Array1*,UInt8,Exec_Mode) */
    core_ds4_fn_timeout_q_ins_t  timeout_q_insert_fn;       /* Boolean(Timeout_Q_Func,Timeout_Q_Arg,u32,u32,u8) */
    core_ds4_fn_u32_ptr          timeout_q_remove_fn;       /* Boolean timeout_q_remove(void*)   */
    char* const *                nvfs_fw_data_addr;         /* DATA: &nvfs_fw_data (entry name)  */

    /* ---- GROUP: prog (SS4 Slice 8d) ---- */
    core_ds4_fn_void_ptr          prog_set_exit_callback_fn; /* void prog_set_exit_callback(Prog_Func) */

    /* ---- GROUP: dbg config (v1.1): DATA pointer to CORE's single dbg config instance.
           Appended at the TAIL (consumes a reserved slot) so all v1.0 field offsets are
           preserved -> an old (v1.0) FW on a v1.1 CORE still reads the table correctly. ---- */
    dbg_config_contract_t* dbg_config;

    /* ---- GROUP: DS4-added routing (symbols the DS4 vehicle calls that DB3 did not route) ----
       Populate in the shim's CORE_DS4_BUILD block (S5) with the real CORE functions;
       until then these are 0 -> the FW must not call them at runtime. ---- */
    core_ds4_fn_u32_u32          prog_retrieve_option_fn;   /* SInt8 prog_retrieve_option(Feature_Name)      */
    core_ds4_fn_cmd_q_t          cmd_q_insert_fn;           /* Boolean cmd_q_insert(UInt16, TskTimeout, ...) */
    core_ds4_fn_can_per_add_t    can_per_add_fn;            /* Boolean can_per_add(func, arg, off, per, cnt) */
    core_ds4_fn_u32_ptr          can_per_remove_fn;         /* Boolean can_per_remove(void*)                 */

    /* ---- future groups appended here (append-only), consuming reserved headroom ---- */

    void* reserved_slots[CORE_DS4_API_TABLE_RESERVED_SLOTS];
} core_ds4_api_table_t;

/* Ceiling guard: the table (published at 0x37040) must not reach the FW image
   base (0x38000). Fails to compile if the table grows past the 0xFC0 (4032 B)
   window. DS4 is greenfield: reserved_slots=16 and the surface still evolves, so
   there is NO frozen-size pin yet (added at DS6 freeze). */
typedef char core_ds4_api_table_fits_assert[
    (sizeof(core_ds4_api_table_t) <= (CORE_DS4_FLASH_FW_START_ADDR - CORE_DS4_SHARED_API_TABLE_ADDR)) ? 1 : -1];

static uint32_t core_contract_ds4_api_table_is_valid(const core_ds4_api_table_t* api_table)
{
    if (api_table == 0)
    {
        return 0U;
    }

    if (api_table->table_magic != CORE_DS4_API_TABLE_MAGIC)
    {
        return 0U;
    }

    if (api_table->table_size < sizeof(core_ds4_api_table_t))
    {
        return 0U;
    }

    if (api_table->abi_version_major != CORE_DS4_ABI_VERSION_MAJOR)
    {
        return 0U;
    }

    if (api_table->api_table_version != CORE_DS4_API_TABLE_VERSION)
    {
        return 0U;
    }

    if (api_table->abi_version_minor < CORE_DS4_ABI_VERSION_MINOR)
    {
        return 0U;
    }

    return 1U;
}

#endif /* __CORE_CONTRACT_DS4_API_TABLE_H__ */
