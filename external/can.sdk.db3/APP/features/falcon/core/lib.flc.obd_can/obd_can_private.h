/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

//             <<< Use Configuration Wizard in Context Menu >>>             //

/*==========================================================================*/
// $Id: obd_can_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __OBD_CAN_PRIVATE_H__
#define __OBD_CAN_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "obd_can.h"
#include "core_contract_db3_obd_can_config.h"
#include "can_usr.h"
#include "timeout_f.h"
#include "cmd.h"
#include "status.h"
#include "nvfs_usr.h"
#include "can_usr.h"
#include "dbg.h"

#if DBG_OBD_CAN == 1
  #warning "DBG_OBD_CAN is Enabled"
#endif
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

enum 
{
  // Add a new version in the enum whenever the NFVS data (config, etc...) changes.
  // init_nvfs() can detect a version change and reset the configuration data if necessary.
  OBD_CAN_VER_100, //
  OBD_CAN_VER_2 
};
// Versioning of the library
#define OBD_CAN_CUR_VER 2

#if CONFIG_OBD_CAN_VERSION != OBD_CAN_CUR_VER   // must be changed when config file is modified
    #error config_obd_can does not match
#endif

//--------------------------------------------------------------------------//
// <h>  Task 'obd_can_ctrl' settings
// <s0> Name
// <o0> Priority           <0-65535>
// <i>  Lowest  priority = 0
// <i>  Highest priority = 65535
// <o1> Stack Size (bytes) <0x0000-0xFFFF:4>
// <i>  Must be a multiple of 4.
// </h>
#define TASK_OBD_CTRL_NAME          "obd_can_ctrl"
#define TASK_OBD_CTRL_PRI           5001
#define TASK_OBD_CTRL_STK           0x0100

#define OBD_CAN_ERR_BUFF_SIZE       6

#define OBD_CAN_MAX_RESPONSE_DATA   30

#define TXDATA(z)    request_info->can_tx_msg->data._8_[z]

#define TX_STD_SIZE   TXDATA(0)
#define TX_STD_SIDRQ  TXDATA(1)
#define TX_STD_INFTYP TXDATA(2)
    
// OBD CAN timeouts as per specification
#define OBD_CAN_P2_TIMEOUT          60 
#define OBD_CAN_TIMEOUT             10000

// OBD CAN modes as per specification
#define OBD_CAN_MODE1               0x01
#define OBD_CAN_MODE2               0x02
#define OBD_CAN_MODE3               0x03
#define OBD_CAN_MODE4               0x04
#define OBD_CAN_MODE9               0x09

#define OBD_CAN_NEGATIVE_RESPONSE   0x7F
#define OBD_CAN_NEGATIVE_PENDING    0x78

#define OBD_CAN_TX_FIRST_TRY        FALSE
#define OBD_CAN_TX_RETRY            TRUE

#define OBD_CAN_ON_DEMAND           -1

#define DTC_CLR_SUCCESS             0
#define DTC_CLR_FAIL                1

#define BEFORE_DISCONNECT_MAX       5

//--------------------------------------------------------------------------//

// CAN IDs for the ECUs

// For 11-bit OBD-CAN, this is simple:
//                      requests  7E0 - 707
//                      responses 7E8 - 7EF
// For 29-bit OBD-CAN, it gets more complicated: 
//                      requests  18DBxxF1
//                      responses 18DAF1xx
//                      There are 256 possible addresses for 8 ECUs overall. 
//                      Typically, addresses 10-17 are used for engine ECUs and that's what 
//                      is implemented in this library.
//                      If we want to support other types of ECUs in the future, the range
//                      becomes huge (256) so we need to change the way we do ECU discovery.

#define OBD_CAN_REQ_ALL_ECUS    (obd_can_vars.is_ext ? 0x18DB33F1 : 0x7DF)
#define OBD_CAN_REQ_FIRST_ECU   (obd_can_vars.is_ext ? 0x18DA10F1 : 0x7E0)
#define OBD_CAN_REQ_LAST_ECU    (obd_can_vars.is_ext ? 0x18DA17F1 : 0x7E7)
#define OBD_CAN_REQ_NO_ECU      (obd_can_vars.is_ext ? 0xFFFFFFFF : 0xFFFF)

#define OBD_CAN_RESP_FIRST_ECU  (obd_can_vars.is_ext ? 0x18DAF10E : 0x7E8)
#define OBD_CAN_RESP_LAST_ECU   (obd_can_vars.is_ext ? 0x18DAF117 : 0x7EF)

enum 
{
  OBD_CAN_STD_ID_7DF , // Requests

  OBD_CAN_STD_ID_7E0 ,
  OBD_CAN_STD_ID_7E1 ,
  OBD_CAN_STD_ID_7E2 ,
  OBD_CAN_STD_ID_7E3 ,
  OBD_CAN_STD_ID_7E4 ,
  OBD_CAN_STD_ID_7E5 ,
  OBD_CAN_STD_ID_7E6 ,
  OBD_CAN_STD_ID_7E7 ,

  OBD_CAN_STD_ID_7E8 , // Responses
  OBD_CAN_STD_ID_7E9 ,
  OBD_CAN_STD_ID_7EA ,
  OBD_CAN_STD_ID_7EB ,
  OBD_CAN_STD_ID_7EC ,
  OBD_CAN_STD_ID_7ED ,
  OBD_CAN_STD_ID_7EE ,
  OBD_CAN_STD_ID_7EF ,

  OBD_CAN_STD_ID_COUNT

} ;

enum 
{
  OBD_CAN_EXT_ID_18DB33F1 , // Request

  OBD_CAN_EXT_ID_18DA10F1 , //
  OBD_CAN_EXT_ID_18DA11F1 , //
  OBD_CAN_EXT_ID_18DA12F1 , // 
  OBD_CAN_EXT_ID_18DA13F1 , // 
  OBD_CAN_EXT_ID_18DA14F1 , // 
  OBD_CAN_EXT_ID_18DA15F1 , // 
  OBD_CAN_EXT_ID_18DA16F1 , // 
  OBD_CAN_EXT_ID_18DA17F1 , // 

  OBD_CAN_EXT_ID_18DAF10E , // Responses
  OBD_CAN_EXT_ID_18DAF10F , //
  OBD_CAN_EXT_ID_18DAF110 , //
  OBD_CAN_EXT_ID_18DAF111 , // 
  OBD_CAN_EXT_ID_18DAF112 , // 
  OBD_CAN_EXT_ID_18DAF113 , // 
  OBD_CAN_EXT_ID_18DAF114 , // 
  OBD_CAN_EXT_ID_18DAF115 , // 
  OBD_CAN_EXT_ID_18DAF116 , // 
  OBD_CAN_EXT_ID_18DAF117 , // 

  OBD_CAN_EXT_ID_COUNT

} ;

//--------------------------------------------------------------------------//

typedef struct s_Obd_Can_Config
{
  Boolean           is_enabled ;  
  SInt8             time_based_period ;
  UInt8             event_based_cmd ;
  SInt8             event_based_delay ;

} Obd_Can_Config;

//--------------------------------------------------------------------------//

typedef struct s_Obd_Can_ECU_Map
{
  UInt32            ecu ;
  UInt32            potential_ecu ;

} Obd_Can_ECU_Map;

//--------------------------------------------------------------------------//

typedef struct s_Obd_Can_ECU__Ans_Map
{
  UInt32            ecu ;

} Obd_Can_ECU_Ans_Map;

//--------------------------------------------------------------------------//

typedef struct s_Obd_Can_Schedule
{
  SInt32                when    ;
  Obd_Can_Req_Status    status  ;

} Obd_Can_Schedule;

//--------------------------------------------------------------------------//

typedef struct s_Pid_to_Req_Enum
{
  UInt8             pid     ;
  Obd_Can_Req_Enum  request ;

} Pid_to_Req_Enum;

//--------------------------------------------------------------------------//

typedef struct s_Obd_Can_Req_Info
{
    UInt8                   req_retry_cnt     ;
    Obd_Can_Req_Enum        current_req       ;
    Obd_Can_Req_Type_Enum   current_req_type  ;
    Timeout                 rx_timeout        ;
    Can_Msg       *         can_tx_msg        ;
  
} Obd_Can_Req_Info ;

//--------------------------------------------------------------------------//

typedef struct s_Obd_Can
{
  SInt32           schedule_timer ;
  
  Can_Dev_Id       dev_id     ;

  UInt8            disconnected_bus_cnt ;

  Tsk              rx_tsk     ;
  Tsk              ctrl_tsk   ;

  Boolean          flg_ign    ;

  Boolean          is_ext    ;

  Boolean          is_active  ; 

  Boolean          scan_tool_detected ;

  Boolean          flg_request ;            //to activate request schedule

  Timeout          ign_timeout   ;

  Boolean          is_connected ;
  
  Obd_Can_Req_Info  schedule_info ;
  Obd_Can_Req_Info  on_demand_info ;
  
  UInt8             req_retries ;
  Boolean           retry_tx ;

} Obd_Can_Vars ;

//--------------------------------------------------------------------------//

typedef struct
{
  UInt8  size    ;
  UInt8  sidpr   ; // reponse
  UInt8  data[OBD_CAN_MAX_RESPONSE_DATA];

} Obd_Can_Rx_Msg ;

//--------------------------------------------------------------------------//

typedef enum
{
   NO_OBD_CAN_ERR              ,

   OBD_CAN_ERR_NOT_AVAILABLE   ,
   OBD_CAN_ERR_PENDING         ,
   OBD_CAN_ERR_BUS_DISCONNECTED,
   OBD_CAN_ERR_UNSUPPORTED_MODE,
   OBD_CAN_ERR_UNKNOWN_REQ     ,

   OBD_CAN_ERR_COUNT 

} Obd_Can_Err_Code ;

//--------------------------------------------------------------------------//

typedef struct s_Obd_Can_Err
{
  UInt8            count                        ;

  Obd_Can_Err_Code code [OBD_CAN_ERR_BUFF_SIZE] ;
  
} Obd_Can_Err ;


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

EXTERN_C_ENTER

void    obd_can_display_status  (void) ;
void    obd_can_display_support (void) ;

Boolean obd_can_init_nvfs       (void) ;

void obd_can_reset              (void) ;    

void obd_can_tx_with_ecu        (Obd_Can_Req_Enum request, Obd_Can_Req_Type_Enum request_type, Boolean retrying, UInt32 ecu);
void obd_can_tx_reset_sidrq     (Obd_Can_Req_Info * request_info) ;

void obd_can_rx                 (void * q_msg) ;

Boolean obd_can_config_internal (Obd_Can_Req_Enum request, SInt8 period, UInt16 cmd, SInt8 delay, Boolean save_nvfs);
Boolean obd_can_is_req_enabled  (Obd_Can_Req_Enum request) ;

Boolean obd_can_rx_vin          (Obd_Can_Rx_Msg * obd_can_rx_msg) ;    
Boolean obd_can_rx_air_ambiant  (Obd_Can_Rx_Msg * obd_can_rx_msg) ;    
Boolean obd_can_rx_fuel_level   (Obd_Can_Rx_Msg * obd_can_rx_msg) ;    
Boolean obd_can_rx_intake_temp  (Obd_Can_Rx_Msg * obd_can_rx_msg) ;    
Boolean obd_can_rx_eng_run      (Obd_Can_Rx_Msg * obd_can_rx_msg) ;    
Boolean obd_can_rx_eng_coolant  (Obd_Can_Rx_Msg * obd_can_rx_msg) ;    
Boolean obd_can_rx_speed        (Obd_Can_Rx_Msg * obd_can_rx_msg) ;    
Boolean obd_can_rx_rpm          (Obd_Can_Rx_Msg * obd_can_rx_msg) ;    
Boolean obd_can_rx_bat_volt     (Obd_Can_Rx_Msg * obd_can_rx_msg) ;    
Boolean obd_can_rx_dtc          (Obd_Can_Rx_Msg * obd_can_rx_msg) ;
Boolean obd_can_rx_dtc_clr      (Obd_Can_Rx_Msg * obd_can_rx_msg) ;

void    obd_can_rx_dtc_clr_tx_d2d  (UInt8 status) ;

void    obd_can_clr_dtc            (void * arg);

void    obd_can_scan_tool_detected (void * q_msg) ;

void    obd_can_cmd                (void *arg);

void    obd_can_rx_timeout_on_demand  (void * arg) ;
void obd_can_rx_timeout_schedule      (void * arg) ;
void    obd_can_rx_timeout            (Obd_Can_Req_Info * request_info) ;
void    obd_can_ign_timeout           (void * arg) ;

void    obd_can_ctrl_task          (void);

void init_ecu_map				   ( UInt32 first_potential_ecu	) ;

Boolean obd_can_err_add            (Obd_Can_Err_Code code);

EXTERN_C_LEAVE

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern const NVFS_Entry_Name          nvfs_obd_can_config  ;
extern const NVFS_Entry_Name          nvfs_obd_can_ver     ;
extern const NVFS_Entry_Name          nvfs_obd_can_err     ;

extern Obd_Can_Vars             obd_can_vars  ;
extern Obd_Can_Err              obd_can_err ;
extern Obd_Can_Config           obd_can_cfg           [OBD_CAN_REQ_COUNT] ;    
extern Obd_Can_ECU_Map          obd_can_ecu_map       [OBD_CAN_REQ_COUNT] ;
extern Obd_Can_ECU_Ans_Map      obd_can_ecu_ans_map   [OBD_CAN_REQ_COUNT] ;    
extern Obd_Can_Schedule         obd_can_schedule      [OBD_CAN_REQ_COUNT] ;

extern Can_Msg obd_can_req_std_msg          ;
extern Can_Msg obd_can_req_std_msg_req_next ;

extern Can_Msg obd_can_od_std_msg           ;
extern Can_Msg obd_can_od_std_msg_req_next  ;

extern Can_Msg obd_can_req_ext_msg          ;
extern Can_Msg obd_can_req_ext_msg_req_next ;

extern Can_Msg obd_can_od_ext_msg           ;
extern Can_Msg obd_can_od_ext_msg_req_next  ;






extern const Obd_Can_Config obd_can_config_default [OBD_CAN_REQ_COUNT] ;        

#endif

