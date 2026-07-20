/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs_usr.h 33406 2015-08-26 21:25:10Z suchita.yadav $
/*==========================================================================*/

#ifndef __NVFS_USR_H__
#define __NVFS_USR_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "nvfs.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef union s_Id_Entry
{
// 123456 ABC 12 A 1234567
// DIGITS FORMAT INFORMATION
  struct
    {
  
      UInt8 upc     [6] ;  // First 6 digits Numeric Last 6 digits of UPC
      UInt8 assembly[3] ;  // Next  3 digits Alpha Numeric Assembly code    
      UInt8 year    [2] ;  // Next  2 digits Numeric Year code              
      UInt8 month       ;  // Next  1 digit Alpha numeric Month code        
      UInt8 serial  [7] ;  // Next  7 digits Numeric Serial Number Sequence 
  
    } item ;

  UInt8 buff[19] ;

}Id_Entry ;

//--------------------------------------------------------------------------//  

typedef struct s_Xpresskit_Entry
{

  UInt16  fw_id      ;
  UInt16  vehicle_id ;
   
}Xpresskit_Id_Entry ;


//--------------------------------------------------------------------------//  

typedef enum e_Hw_Id
{
  NO_HW_ID        = 0 ,
  HW_ID_CANMAX400     ,
  HW_ID_DBALL         ,
  HW_ID_CANCRUISE     ,
  HW_ID_CM800         ,

  HW_ID_COUNT

}Hw_Id ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern const NVFS_Entry_Name nvfs_platform        ; // platform name/number
extern const NVFS_Entry_Name nvfs_hw_ver          ; // hardware version
extern const NVFS_Entry_Name nvfs_hw_id           ; // hardware id
extern const NVFS_Entry_Name nvfs_boot_ver        ; // boot version 
extern const NVFS_Entry_Name nvfs_sernum          ; // serial number send to xpress vip for encryption

extern const NVFS_Entry_Name nvfs_id              ; // set at production for tracking
                                         
extern const NVFS_Entry_Name nvfs_d2d_ena         ; // d2d enable/disable
                                         
extern const NVFS_Entry_Name nvfs_vin             ; // vin for current software , if you want to save previous vin copy to different name
extern const NVFS_Entry_Name nvfs_fw_data         ; // specific firmware data ( bypass data , keyless data....)
                                          
extern const NVFS_Entry_Name nvfs_brand           ; // set at production Rf ,ss ,Rs
extern const NVFS_Entry_Name nvfs_auth_brand      ; // set at production protected RF 
extern const NVFS_Entry_Name nvfs_auth_sys_type   ; // set at production protected RS, SS, Hybride


extern const NVFS_Entry_Name nvfs_feat_rs             ; // Feature name
extern const NVFS_Entry_Name nvfs_feat_rs_ver         ; // Feature version
                                          
extern const NVFS_Entry_Name nvfs_feat_dei            ; // Feature name
extern const NVFS_Entry_Name nvfs_feat_dei_ver        ; // Feature version
extern const NVFS_Entry_Name nvfs_feat_astrostart     ; // Feature name
extern const NVFS_Entry_Name nvfs_feat_astrostart_ver ; // Feature version
extern const NVFS_Entry_Name nvfs_feat_autostart      ; // Feature name
extern const NVFS_Entry_Name nvfs_feat_autostart_ver  ; // Feature version
 
extern const NVFS_Entry_Name nvfs_ss_stat         ; // Security system status
extern const NVFS_Entry_Name nvfs_ss_ver          ; // Security system version

extern const NVFS_Entry_Name nvfs_rs_stat         ; // Remote start status
extern const NVFS_Entry_Name nvfs_rs_ver          ; // Security system version
 
extern const NVFS_Entry_Name nvfs_rem_dei         ; // Remote data     for Dei
extern const NVFS_Entry_Name nvfs_rem_as          ; // Remote data     for autostart
extern const NVFS_Entry_Name nvfs_rem_af          ; // Remote data     for Astrostart

extern const NVFS_Entry_Name nvfs_error           ; // Error Code
extern const NVFS_Entry_Name nvfs_d2d_error       ; // D2D Error Code                                                                                  
extern const NVFS_Entry_Name nvfs_exec_error      ; // EXEC Error Code   
extern const NVFS_Entry_Name nvfs_feat_error      ; // FEAT Error Code  

extern const NVFS_Entry_Name nvfs_xpresskit_id    ; // Fw Id & Vehicle ID

extern const NVFS_Entry_Name nvfs_fob_ctrl        ; // FOB control configuration

//--------------------------------------------------------------------------//

extern const NVFS_Entry_Name nvfs_feat_fw         ; // feature for interface
extern const NVFS_Entry_Name nvfs_w2w_ena         ; // w2w enable/disable
extern const NVFS_Entry_Name nvfs_fw_error        ; // software error


//--------------------------------------------------------------------------//
extern const NVFS_Entry_Name nvfs_os_stk_error    ; // os stack error
extern const NVFS_Entry_Name nvfs_os_heap_error   ; // os heap error

//--------------------------------------------------------------------------//
extern const NVFS_Entry_Name nvfs_d2d_type        ; // D2D_TYPE: D2D_1_2, D2D_1_5, and D2D_I
//--------------------------------------------------------------------------//
extern const NVFS_Entry_Name nvfs_config_port ; //BYTE 0: UNDER DASH; BYTE 01: UPPER DASH; 
                                                //BYTE 02:  SECRET  PORT
                                                //each port could be: 00: OFF; 01: D2D1; 02: D2D2; 
                                                // 03: D2D0 (VALET SWITCH); 04: D2D_i (compustar)

//--------------------------------------------------------------------------//
extern const NVFS_Entry_Name nvfs_auto_manual;		// To get the device is Auto/Manual transmission type
//--------------------------------------------------------------------------//
extern const NVFS_Entry_Name nvfs_tempsens;		// To get the source for temp start

extern const NVFS_Entry_Name nvfs_hard_reset_req;  //request hard reset from web via nvfs
extern const NVFS_Entry_Name nvfs_sys_in_used;      //if system in used (received arm/start)
extern const NVFS_Entry_Name nvfs_ble_info;       //BLE firmware info from proto_info
// debug
extern const NVFS_Entry_Name nvfs_dbg_msg1;
extern const NVFS_Entry_Name nvfs_dbg_msg2;
extern const NVFS_Entry_Name nvfs_dbg_enc_dis;  // debug flag to disable encryption of hex files
extern const NVFS_Entry_Name nvfs_validation_flag;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

