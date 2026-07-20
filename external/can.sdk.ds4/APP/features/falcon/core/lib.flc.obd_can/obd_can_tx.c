/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: obd_can_tx.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "obd_can_private.h"
#include "wake.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void obd_can_tx(Obd_Can_Req_Enum request, Obd_Can_Req_Type_Enum request_type, Boolean retrying)
{
  Obd_Can_Req_Info * request_info;
    
  if(request_type == OBD_CAN_REQ_TYPE_ON_DEMAND)
  {
    request_info = &obd_can_vars.on_demand_info ;
  }
  else
  {
    request_info = &obd_can_vars.schedule_info ;
  }
  
    timeout_stop(request_info->rx_timeout) ;

    if ( !obd_can_vars.flg_ign || !obd_can_is_req_enabled(request) )
    {
      obd_can_schedule[request].status = OBD_CAN_INITIAL_REQUEST_NOT_DONE ;
      return;
    }

    // Prepare reception of response(s)
    timeout_update(request_info->rx_timeout,OBD_CAN_P2_TIMEOUT) ;
    timeout_start(request_info->rx_timeout) ;
    
    obd_can_schedule[request].status = OBD_CAN_ONGOING; 

    if (!retrying)
    { 
      request_info->req_retry_cnt = 0;
      request_info->current_req = request;
      request_info->current_req_type = request_type;
    }

    // Prepare transmission of request
    switch ( request )
      {
        case OBD_CAN_VIN          : TX_STD_SIZE = 0x02 ; TX_STD_SIDRQ = OBD_CAN_MODE9 ; TX_STD_INFTYP = 0x02 ;  break ;  //state_VIN         () ; break ;
        case OBD_CAN_ENG_COOLANT  : TX_STD_SIZE = 0x02 ; TX_STD_SIDRQ = OBD_CAN_MODE1 ; TX_STD_INFTYP = 0x05 ;  break ;  //state_eng_coolant () ; break ;
        case OBD_CAN_RPM          : TX_STD_SIZE = 0x02 ; TX_STD_SIDRQ = OBD_CAN_MODE1 ; TX_STD_INFTYP = 0x0C ;  break ;  //state_rpm         () ; break ;
        case OBD_CAN_SPEED        : TX_STD_SIZE = 0x02 ; TX_STD_SIDRQ = OBD_CAN_MODE1 ; TX_STD_INFTYP = 0x0D ;  break ;  //state_speed       () ; break ;
        case OBD_CAN_ENG_RUN      : TX_STD_SIZE = 0x02 ; TX_STD_SIDRQ = OBD_CAN_MODE1 ; TX_STD_INFTYP = 0x1F ;  break ;  //state_eng_run     () ; break ;
        case OBD_CAN_FUEL_LEVEL   : TX_STD_SIZE = 0x02 ; TX_STD_SIDRQ = OBD_CAN_MODE1 ; TX_STD_INFTYP = 0x2F ;  break ;  //state_fuel_level  () ; break ;
        case OBD_CAN_AIR_AMB      : TX_STD_SIZE = 0x02 ; TX_STD_SIDRQ = OBD_CAN_MODE1 ; TX_STD_INFTYP = 0x46 ;  break ;  //state_air_amb     () ; break ;
        case OBD_CAN_INTAKE_TEMP  : TX_STD_SIZE = 0x02 ; TX_STD_SIDRQ = OBD_CAN_MODE1 ; TX_STD_INFTYP = 0x0F ;  break ;  //state_intake_temp () ; break ;
        case OBD_CAN_BAT_VOLT     : TX_STD_SIZE = 0x02 ; TX_STD_SIDRQ = OBD_CAN_MODE1 ; TX_STD_INFTYP = 0x42 ;  break ;  //state_bat_volt    () ; break ;
        case OBD_CAN_DTC_CODE     : TX_STD_SIZE = 0x01 ; TX_STD_SIDRQ = OBD_CAN_MODE3 ; TX_STD_INFTYP = 0x00 ;  break ;  //state_dtc_code    () ; break ;
        case OBD_CAN_DTC_CLR      : TX_STD_SIZE = 0x01 ; TX_STD_SIDRQ = OBD_CAN_MODE4 ; TX_STD_INFTYP = 0x00 ;  break ;  //state_clr_dtc     () ; break ;
      
        default:
          {          
          if (obd_can_vars.flg_ign) obd_can_err_add ( OBD_CAN_ERR_UNKNOWN_REQ );
            return ;
          }
      }

    // Select the ECU
    
      if (obd_can_ecu_map[request].ecu != OBD_CAN_REQ_NO_ECU)
      {
        // It's already known
          request_info->can_tx_msg->id = obd_can_ecu_map[request].ecu;
      }
      else

      {
        // It's unknown for now - let's try the next potential ECU
        // If we receive a response, we will log this potential ECU as the desired ECU for future requests
          request_info->can_tx_msg->id = obd_can_ecu_map[request].potential_ecu;
      }
    

  #if DBG_OBD_CAN == 1
    dbg_tx_text("--- obd_can_tx():\r\n");
    dbg_tx_text("    ECU        = ");    dbg_tx_h32(request_info->can_tx_msg->id); dbg_tx_text("\r\n");
    dbg_tx_text("    TX_STD_SIDRQ  = "); dbg_tx_h8(TX_STD_SIDRQ);    dbg_tx_text("\r\n");
    dbg_tx_text("    TX_STD_INFTYP = "); dbg_tx_h8(TX_STD_INFTYP);   dbg_tx_text("\r\n");
  #endif

    // Transmit the request
    wake_enter_flagged(WAKE_OBD_CAN_TX) ;
    can_usr_tx (obd_can_vars.dev_id , request_info->can_tx_msg ,FALSE,1) ;
    wake_leave_flagged(WAKE_OBD_CAN_TX) ;
}
//--------------------------------------------------------------------------// 
void obd_can_tx_reset_sidrq (Obd_Can_Req_Info * request_info)
{
  if(request_info != NULL)
  {
        #if DBG_OBD_CAN == 1
          dbg_tx_text("***Clearing TX_STD_SIDRQ\r\n");
        #endif
        
            TX_STD_SIDRQ = 0x00;
        
  }
}

//--------------------------------------------------------------------------// 

void obd_can_tx_with_ecu(Obd_Can_Req_Enum request, Obd_Can_Req_Type_Enum request_type, Boolean retrying, UInt32 ecu)
{
    obd_can_ecu_map[request].ecu = ecu;
    obd_can_tx(request, request_type, retrying);
}
