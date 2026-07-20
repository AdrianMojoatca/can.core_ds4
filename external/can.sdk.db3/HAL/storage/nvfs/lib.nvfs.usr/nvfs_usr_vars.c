/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs_usr_vars.c 33406 2015-08-26 21:25:10Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

 #include "nvfs_usr.h"
 #include "core_contract_db3_nvfs_entry_name.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define NVFS_FW_NAME_ENTRY_BUFFER_SIZE   48

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void nvfs_usr_init_entry_names_from_contract(void);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

//--------------------------------------------------------------------------//

const NVFS_Entry_Name nvfs_platform        = "PLATFORM"                        ; // platform name/number
const NVFS_Entry_Name nvfs_hw_ver          = "HW_VER"                          ; // hardware version
const NVFS_Entry_Name nvfs_hw_id           = "HW_ID"                           ; // hardware id
const NVFS_Entry_Name nvfs_boot_ver        = "BOOT_VER"                        ; // boot version 
const NVFS_Entry_Name nvfs_sernum          = "SERNUM"                          ; // serial number send to xpress vip for encryption
                                                                              
const NVFS_Entry_Name nvfs_id              = "ID"                              ; // set at production for tracking
                                          
const NVFS_Entry_Name nvfs_d2d_ena         = "D2D"                             ; // d2d enable/disable

static char nvfs_vin_name[NVFS_FW_NAME_ENTRY_BUFFER_SIZE]             = "VIN,";
static char nvfs_fw_data_name[NVFS_FW_NAME_ENTRY_BUFFER_SIZE]         = "DATA,";
static char nvfs_feat_fw_name[NVFS_FW_NAME_ENTRY_BUFFER_SIZE]         = "FEAT_FW_";
static char nvfs_fw_error_name[NVFS_FW_NAME_ENTRY_BUFFER_SIZE]        = "FW_ERR_";
static char nvfs_os_stk_error_name[NVFS_FW_NAME_ENTRY_BUFFER_SIZE]    = "OS_STK_ERROR";
static char nvfs_os_heap_error_name[NVFS_FW_NAME_ENTRY_BUFFER_SIZE]   = "OS_HEAP_ERROR";

const NVFS_Entry_Name nvfs_vin             = nvfs_vin_name                    ; // vin for current software , if you want to save previous vin copy to different name
const NVFS_Entry_Name nvfs_fw_data         = nvfs_fw_data_name                ; // specific firmware data ( bypass data , keyless data....)
                                          
const NVFS_Entry_Name nvfs_brand           = "BRAND"                           ; // set at production Rf ,ss ,Rs 

const NVFS_Entry_Name nvfs_feat_rs             = "FEAT_RS"                     ; // Feature name
const NVFS_Entry_Name nvfs_feat_rs_ver         = "FEAT_RS_VER"                 ; // Feature version
 
const NVFS_Entry_Name nvfs_feat_dei            = "FEAT_DEI"                    ; // Feature name
const NVFS_Entry_Name nvfs_feat_dei_ver        = "FEAT_DEI_VER"                ; // Feature version
const NVFS_Entry_Name nvfs_feat_astrostart     = "FEAT_ASTRO"                  ; // Feature name
const NVFS_Entry_Name nvfs_feat_astrostart_ver = "FEAT_ASTRO_VER"              ; // Feature version
const NVFS_Entry_Name nvfs_feat_autostart      = "FEAT_AUTO"                   ; // Feature name
const NVFS_Entry_Name nvfs_feat_autostart_ver  = "FEAT_AUTO_VER"               ; // Feature version

const NVFS_Entry_Name nvfs_ss_stat         = "SS_STATUS"                       ; // Security system status
const NVFS_Entry_Name nvfs_ss_ver          = "SS_VER"                          ; // Security system version

const NVFS_Entry_Name nvfs_rs_stat         = "RS_STATUS"                       ; // Remote start status
const NVFS_Entry_Name nvfs_rs_ver          = "RS_VER"                          ; // Security system version

const NVFS_Entry_Name nvfs_rem_dei         = "REM_DEI"                         ; // Remote data     for Dei
const NVFS_Entry_Name nvfs_rem_as          = "REM_AS"                          ; // Remote data     for autostart
const NVFS_Entry_Name nvfs_rem_af          = "REM_AF"                          ; // Remote data     for Astrostart


const NVFS_Entry_Name nvfs_error           = "ERROR"                           ; // Error Code
const NVFS_Entry_Name nvfs_d2d_error       = "D2D_ERROR"                       ; // D2D Error Code                                                                                    
const NVFS_Entry_Name nvfs_exec_error      = "EXEC_ERROR"                      ; // EXEC Error Code 
const NVFS_Entry_Name nvfs_feat_error      = "FEAT_ERROR"                      ; // Feat Error Code 


const NVFS_Entry_Name nvfs_xpresskit_id    = "XPRESSKIT_ID"                    ; // Fw Id & Vehicle ID 

const NVFS_Entry_Name nvfs_fob_ctrl        = "FOB_CTRL"                        ; // Fob control configuration
   
//--------------------------------------------------------------------------//

const NVFS_Entry_Name nvfs_feat_fw         = nvfs_feat_fw_name                 ; // feature for interface
const NVFS_Entry_Name nvfs_w2w_ena         = "W2W"                             ; // W2W enable/disable
const NVFS_Entry_Name nvfs_fw_error        = nvfs_fw_error_name                ;

//--------------------------------------------------------------------------//

const NVFS_Entry_Name nvfs_os_stk_error    = nvfs_os_stk_error_name            ; // os stack error
const NVFS_Entry_Name nvfs_os_heap_error   = nvfs_os_heap_error_name           ; // os heap error

//--------------------------------------------------------------------------//
const NVFS_Entry_Name nvfs_d2d_type        = "D2D_TYPE"                        ; // D2D_TYPE: D2D_1_2, D2D_1_5, and D2D_I
//--------------------------------------------------------------------------//
const NVFS_Entry_Name nvfs_config_port     = "CONFIG_PORT"     ;  //BYTE 0: UNDER DASH; BYTE 01: UPPER DASH; 02: SECRET PORT
                                                                  // each port could be: 00: OFF; 01: D2D1; 02: D2D2; 
                                                                  // 03: D2D0 (VALET SWITCH); 04: D2D_i (compustar)

//--------------------------------------------------------------------------//
const NVFS_Entry_Name nvfs_auto_manual		= "_MANUAL"	;		// To get the device is Auto/Manual transmission type
//--------------------------------------------------------------------------//

const NVFS_Entry_Name nvfs_tempsens		= "TEMPSENS"	;		// To get the source for temp start
                                                                // 00 NONE;  01: analog sensor; 
                                                                // 02: NGSS; 03: D2D IVU

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void nvfs_usr_init_entry_names_from_contract(void)
{
    nvfs_usr_entry_name_from_contract(nvfs_vin_name, sizeof(nvfs_vin_name), "VIN,");
    nvfs_usr_entry_name_from_contract(nvfs_fw_data_name, sizeof(nvfs_fw_data_name), "DATA,");
    nvfs_usr_entry_name_from_contract(nvfs_feat_fw_name, sizeof(nvfs_feat_fw_name), "FEAT_FW_");
    nvfs_usr_entry_name_from_contract(nvfs_fw_error_name, sizeof(nvfs_fw_error_name), "FW_ERR_");
    nvfs_usr_entry_name_from_contract(nvfs_os_stk_error_name, sizeof(nvfs_os_stk_error_name), "OS_STK_ERROR");
    nvfs_usr_entry_name_from_contract(nvfs_os_heap_error_name, sizeof(nvfs_os_heap_error_name), "OS_HEAP_ERROR");
}
