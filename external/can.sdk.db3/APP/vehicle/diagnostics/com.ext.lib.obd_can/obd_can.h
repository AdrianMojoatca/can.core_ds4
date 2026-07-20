/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

// KNOWN LIMITATIONS:
// - New requests cannot be added via configuration; programming is required.

/*==========================================================================*/
// $Id: obd_can.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __OBD_CAN_H__
#define __OBD_CAN_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"
#include "can.h"
#include "core_contract_db3_obd_can_config.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

// Enums used as indexes in configuration and schedule tables
// Not to be confused with the actual OBD CAN PIDs 
typedef enum
{
  OBD_CAN_VIN          ,
  OBD_CAN_ENG_COOLANT  ,
  OBD_CAN_RPM          ,
  OBD_CAN_SPEED        ,
  OBD_CAN_ENG_RUN      ,
  OBD_CAN_FUEL_LEVEL   ,
  OBD_CAN_AIR_AMB      ,
  OBD_CAN_INTAKE_TEMP  ,
  OBD_CAN_BAT_VOLT     ,
  OBD_CAN_DTC_CODE     ,

  OBD_CAN_DTC_CLR      ,

  OBD_CAN_REQ_COUNT

} Obd_Can_Req_Enum;

// Enums for request types
typedef enum
{
  OBD_CAN_REQ_TYPE_NONE       ,
  OBD_CAN_REQ_TYPE_INITIAL    ,
  OBD_CAN_REQ_TYPE_ON_DEMAND  ,
  OBD_CAN_REQ_TYPE_TIME_BASED ,
  OBD_CAN_REQ_TYPE_EVENT_BASED,

  OBD_CAN_REQ_TYPE_COUNT

} Obd_Can_Req_Type_Enum;

// Request status
typedef enum
{
  OBD_CAN_INITIAL_REQUEST_NOT_DONE  ,
  OBD_CAN_ONGOING                   ,
  OBD_CAN_NOT_SUPPORTED             ,
  OBD_CAN_SUCCESS                   ,  
  OBD_CAN_NOT_ENABLED               ,

} Obd_Can_Req_Status;

typedef enum
{
  ALL_ECUS       ,
  SPECIFIC_ECUS   ,
  DEFAULT_ECUS 

} All_Ecu_State;

// Use this value when disabling event-based requests
#define OBD_CAN_NO_EVENT    0xFF

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

EXTERN_C_ENTER

// This is the public interface for the OBD CAN library
// Callers should *** NOT *** use any other functions

// Initialization
// This is the order in which these functions should be called
void               obd_can_init_reset          (void) ;
void               obd_can_init_can            (Can_Dev_Id can_dev_id , Boolean is_std , Boolean do_init_can) ;
void               obd_can_init                (void) ;
void               obd_can_init_ecu            (All_Ecu_State state) ; // function call after init to change type of request
void               obd_can_activate            (void) ;

// Configuration (support, time-based requests, event-based requests)
Boolean            obd_can_config_set_enabled  (Obd_Can_Req_Enum request, Boolean is_enabled);
Boolean            obd_can_config              (Obd_Can_Req_Enum request, SInt8 period, UInt16 cmd, SInt8 delay);

// On-demand requests
void               obd_can_ctrl_request        (Obd_Can_Req_Enum request) ;
Obd_Can_Req_Status obd_can_ctrl_request_status (Obd_Can_Req_Enum request) ;

void               obd_can_deactivate          (void) ;

Boolean            obd_can_is_active           (void) ;
Boolean            obd_can_is_connected        (void) ;

void obd_can_ign_on             (void * arg) ;  // made global for lib.detect in analog product
void obd_can_ign_off            (void * arg) ;  // made global for lib.detect in analog product

void obd_can_tx            (Obd_Can_Req_Enum request, Obd_Can_Req_Type_Enum request_type, Boolean retrying);

Boolean obd_can_rx_value_is_valid(void * value_to_validate, UInt8 number_of_bytes) ;


EXTERN_C_LEAVE

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

#endif

