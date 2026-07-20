/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: obd_can_vars.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "obd_can_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
                                      
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

const NVFS_Entry_Name  nvfs_obd_can_config  = "OBD_CAN_CFG" ;
const NVFS_Entry_Name  nvfs_obd_can_ver     = "OBD_CAN_VER" ;
const NVFS_Entry_Name  nvfs_obd_can_err     = "OBD_CAN_ERR" ;

Obd_Can_Vars        obd_can_vars ;
Obd_Can_Err         obd_can_err ;
Obd_Can_Config   	  obd_can_cfg         [OBD_CAN_REQ_COUNT] ;   
Obd_Can_ECU_Map  	  obd_can_ecu_map     [OBD_CAN_REQ_COUNT] ;
Obd_Can_ECU_Ans_Map obd_can_ecu_ans_map [OBD_CAN_REQ_COUNT] ;
Obd_Can_Schedule 	  obd_can_schedule    [OBD_CAN_REQ_COUNT] ;

Can_Msg obd_can_req_std_msg          = {0x7E0     , 0, 0, 8, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} ;
Can_Msg obd_can_req_std_msg_req_next = {0x7E0     , 0, 0, 8, 0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} ;

Can_Msg obd_can_od_std_msg          = {0x7E0     , 0, 0, 8, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} ;
Can_Msg obd_can_od_std_msg_req_next = {0x7E0     , 0, 0, 8, 0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} ;

Can_Msg obd_can_req_ext_msg          = {0x18DB33F1, 1, 0, 8, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} ;
Can_Msg obd_can_req_ext_msg_req_next = {0x18DB33F1, 1, 0, 8, 0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} ;

Can_Msg obd_can_od_ext_msg          = {0x18DB33F1, 1, 0, 8, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} ;
Can_Msg obd_can_od_ext_msg_req_next = {0x18DB33F1, 1, 0, 8, 0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} ;

const Obd_Can_Config obd_can_config_default[OBD_CAN_REQ_COUNT] = 
{
  // support mask             period     event               event delay
  {OBD_CAN_VIN_MASK         , 0,         OBD_CAN_NO_EVENT,    0,        } , 
  {OBD_CAN_ENG_COOLANT_MASK , 0,         OBD_CAN_NO_EVENT,    0,        } , 
  {OBD_CAN_RPM_MASK         , 0,         OBD_CAN_NO_EVENT,    0,        } , 
  {OBD_CAN_SPEED_MASK       , 0,         OBD_CAN_NO_EVENT,    0,        } , 
  {OBD_CAN_ENG_RUN_MASK     , 0,         OBD_CAN_NO_EVENT,    0,        } , 
  {OBD_CAN_FUEL_LEVEL_MASK  , 0,         OBD_CAN_NO_EVENT,    0,        } , 
  {OBD_CAN_AIR_AMB_MASK     , 0,         OBD_CAN_NO_EVENT,    0,        } , 
  {OBD_CAN_INTAKE_TEMP_MASK , 0,         OBD_CAN_NO_EVENT,    0,        } , 
  {OBD_CAN_BAT_VOLT_MASK    , 0,         OBD_CAN_NO_EVENT,    0,        } , 
  {OBD_CAN_DTC_CODE_MASK    , 0,         OBD_CAN_NO_EVENT,    0,        } , 
  {OBD_CAN_DTC_CLR_MASK     , 0,         OBD_CAN_NO_EVENT,    0,        } , 
};

