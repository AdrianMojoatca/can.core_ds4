/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: obd_can_timeout.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

void obd_can_rx_timeout_on_demand (void * arg)
{
    Obd_Can_Req_Info * request_info = &obd_can_vars.on_demand_info ;
    obd_can_rx_timeout(request_info);
}

void obd_can_rx_timeout_schedule (void * arg)
{
  Obd_Can_Req_Info * request_info = &obd_can_vars.schedule_info;
  obd_can_rx_timeout(request_info);
}

void obd_can_rx_timeout (Obd_Can_Req_Info * request_info)
{
  if(request_info == NULL)
    return ;
  if (request_info->req_retry_cnt++ < obd_can_vars.req_retries && obd_can_schedule[request_info->current_req].status != OBD_CAN_SUCCESS)
  {
    obd_can_tx(request_info->current_req, request_info->current_req_type, OBD_CAN_TX_RETRY);
  }
  else
  {
    // Currently searching for the ECU, don't give up yet, try the next ECU
    if (obd_can_vars.is_active && obd_can_ecu_map[request_info->current_req].ecu == OBD_CAN_REQ_NO_ECU)
    {
      #if DBG_OBD_CAN == 1
        dbg_tx_text("*** obd_can_rx_timeout() - trying next ECU\r\n");
      #endif
      // Have tried all ECUs?
      if (obd_can_ecu_map[request_info->current_req].potential_ecu == OBD_CAN_REQ_LAST_ECU || obd_can_ecu_map[request_info->current_req].potential_ecu == OBD_CAN_REQ_ALL_ECUS)
      {
        #if DBG_OBD_CAN == 1
          dbg_tx_text("*** obd_can_rx_timeout() - tried all ECUs\r\n");
        #endif
        obd_can_ecu_map[request_info->current_req].potential_ecu = obd_can_ecu_map[request_info->current_req].potential_ecu == OBD_CAN_REQ_ALL_ECUS ? OBD_CAN_REQ_ALL_ECUS : OBD_CAN_REQ_FIRST_ECU;
        obd_can_schedule[request_info->current_req].status = OBD_CAN_NOT_SUPPORTED;  
        timeout_stop(request_info->rx_timeout) ;
        obd_can_tx_reset_sidrq(request_info);
      }
      else
      {
        // More ECUs to try...
        if (obd_can_vars.is_ext)
        {
          obd_can_ecu_map[request_info->current_req].potential_ecu += 0x0100;
        }
        else
        {
          obd_can_ecu_map[request_info->current_req].potential_ecu++;
        }
        // Transmit again with another ECU
        obd_can_tx(request_info->current_req, request_info->current_req_type, OBD_CAN_TX_FIRST_TRY);
      }
    }		  
    else
    {
      #if DBG_OBD_CAN == 1
        dbg_tx_text("*** obd_can_rx_timeout() - no more responses expected\r\n");
      #endif
      
      // Give up on response
      timeout_stop(request_info->rx_timeout) ;
      if(TX_STD_SIDRQ == OBD_CAN_MODE4)
      {
        obd_can_rx_dtc_clr_tx_d2d(DTC_CLR_FAIL);
      }
      obd_can_tx_reset_sidrq(request_info);
    }
  }
}

//--------------------------------------------------------------------------//

void obd_can_ign_timeout (void * arg)
{
#if DBG_OBD_CAN == 1
  dbg_tx_text("*** obd_can: ign cycle begins\r\n");
#endif

  obd_can_vars.flg_request = TRUE ;
  timeout_stop(obd_can_vars.ign_timeout) ;
}

