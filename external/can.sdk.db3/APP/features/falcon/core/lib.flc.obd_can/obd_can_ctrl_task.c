/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: obd_can_ctrl_task.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static Boolean obd_can_check_disconnected_bus ( void );
static void    obd_can_update_schedule        ( Obd_Can_Req_Enum request, UInt32 now );
static void    obd_can_wait_request_complete  ( Obd_Can_Req_Enum request );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void obd_can_ctrl_task(void)
{
  UInt8                 i;
  SInt32                now;
  SInt32                when;
  Boolean               initial_requests_done;
  Obd_Can_Req_Type_Enum req_type;
  Obd_Can_Req_Enum      waiting_req = (Obd_Can_Req_Enum)0;  
  // The task performs time-based requests and event-based requests

  obd_can_vars.schedule_timer = 0; 

  for(;;)
  {
  	// Every second wake up and check the schedule
	os_tsk_wait(1000);
	if (!obd_can_vars.flg_request) continue;

    now = ++obd_can_vars.schedule_timer;

    // Initial request (at beginning of IGN cycle) must be done first
	initial_requests_done = FALSE;
	for (i=0; i<sizeof_array(obd_can_schedule); i++)
	{
	  if (obd_can_vars.flg_ign && obd_can_is_req_enabled((Obd_Can_Req_Enum)i) && obd_can_schedule[i].status == OBD_CAN_INITIAL_REQUEST_NOT_DONE && i != OBD_CAN_DTC_CLR)
    {
      initial_requests_done = TRUE;
      obd_can_wait_request_complete(waiting_req);
      if(obd_can_vars.retry_tx)
      {
        obd_can_vars.retry_tx = FALSE;
        i = 0;
        continue;
      }
      if(obd_can_vars.is_active)
      { 
        obd_can_tx((Obd_Can_Req_Enum)i, OBD_CAN_REQ_TYPE_INITIAL, OBD_CAN_TX_FIRST_TRY);       
        obd_can_update_schedule((Obd_Can_Req_Enum)i, now);
        waiting_req = (Obd_Can_Req_Enum)i;
        obd_can_schedule[i].status = OBD_CAN_ONGOING;
      }
	  }
	}
	if (!obd_can_vars.flg_ign) continue;

 	// If all initial requests failed, signal disconnected bus
	if (initial_requests_done)
	{
      obd_can_wait_request_complete(waiting_req);
	  obd_can_check_disconnected_bus();
	}

  // Loop through the schedule and determine if a request must be performed
	for (i=0; i<sizeof_array(obd_can_schedule); i++)
	{
      when = obd_can_schedule[i].when;
	  if (now >= when && when != 0)
	  {
		obd_can_wait_request_complete((Obd_Can_Req_Enum)i);
		req_type = OBD_CAN_REQ_TYPE_ON_DEMAND;
		if (obd_can_cfg[i].time_based_period != 0) req_type = OBD_CAN_REQ_TYPE_TIME_BASED;
		if (obd_can_cfg[i].event_based_cmd != OBD_CAN_NO_EVENT) req_type = OBD_CAN_REQ_TYPE_EVENT_BASED;
	    obd_can_tx((Obd_Can_Req_Enum)i, req_type, OBD_CAN_TX_FIRST_TRY);
		obd_can_update_schedule((Obd_Can_Req_Enum)i, now);
	  }
	}

  }

}

//--------------------------------------------------------------------------//

static void obd_can_update_schedule(Obd_Can_Req_Enum request, UInt32 now)
{
  // Update the schedule
  // Time-based requests must be rescheduled according to config, unless they are one-shot
  // Event-based requests must be cleared - they will be scheduled again when their configured event triggers
  
    obd_can_schedule[request].when = 0;
    obd_can_schedule[request].status = OBD_CAN_INITIAL_REQUEST_NOT_DONE;
    if (obd_can_cfg[request].time_based_period != 0)
	{
	    obd_can_schedule[request].when = now + obd_can_cfg[request].time_based_period;
	}
	if (obd_can_cfg[request].event_based_cmd != OBD_CAN_NO_EVENT)
	{
	  obd_can_config_internal ( request, 0, obd_can_cfg[request].event_based_cmd, obd_can_cfg[request].event_based_delay, FALSE);
	}
  
}

//--------------------------------------------------------------------------//

static void obd_can_wait_request_complete(Obd_Can_Req_Enum request)
{
  UInt8   wait_loops;
  UInt8   wait_loop_cnt;

  // Wait for the reception of the responses for the previous request
  // Wait at most OBD_CAN_TIMEOUT but stop waiting immediately when response has arrived or timed-out
  wait_loops = OBD_CAN_TIMEOUT / OBD_CAN_P2_TIMEOUT;
  for (wait_loop_cnt=0; wait_loop_cnt<wait_loops; wait_loop_cnt++)
  {
    os_tsk_wait(OBD_CAN_P2_TIMEOUT);
    if (obd_can_schedule[request].status != OBD_CAN_ONGOING) break;
  } 
}

//--------------------------------------------------------------------------//

static Boolean obd_can_check_disconnected_bus ( void )
{
	Boolean at_least_one_succeeded;
	UInt8 i;

	if (!obd_can_vars.is_active) return FALSE;

	at_least_one_succeeded = FALSE;
	for (i=0; i<sizeof_array(obd_can_schedule); i++)
	{
	  if (obd_can_is_req_enabled((Obd_Can_Req_Enum)i)) 
	  {
	    if (obd_can_schedule[i].status == OBD_CAN_SUCCESS) at_least_one_succeeded = TRUE;
	  }
	}

	if (!at_least_one_succeeded)
	{
//	  obd_can_vars.is_connected = FALSE ;
#if DBG_OBD_CAN == 1
  	  dbg_tx_text("obd_can_ctrl_tsk: disconnected bus detected\r\n");
#endif

	  // All requests have failed in this ignition cycle
	  if (obd_can_vars.disconnected_bus_cnt < BEFORE_DISCONNECT_MAX)
	  {
	  	obd_can_vars.disconnected_bus_cnt++;
		obd_can_err_add(OBD_CAN_ERR_BUS_DISCONNECTED);
	  }
	  obd_can_deactivate();
	  for (i=0; i<sizeof_array(obd_can_schedule); i++)
	  {
        obd_can_schedule[i].status = OBD_CAN_INITIAL_REQUEST_NOT_DONE;
	  }
	  return TRUE;
	}

	return FALSE;
}

//--------------------------------------------------------------------------//

#if DBG_OBD_CAN == 1

void obd_can_debug_scheduler ( void )
{
	UInt8 i;
    Obd_Can_Req_Status status;
	dbg_tx_text("*** obd_can_schedule: now=0x" );
	dbg_tx_h32(obd_can_vars.schedule_timer);
	dbg_tx_text("\r\n");

	for (i=0; i<sizeof_array(obd_can_schedule); i++)
	{
	    dbg_tx_text("    ");
		dbg_tx_h8(i);
		dbg_tx_text(": when=0x");
		dbg_tx_h32(obd_can_schedule[i].when);
        status = obd_can_ctrl_request_status((Obd_Can_Req_Enum)i);
        
        switch(status)
        {
            case(OBD_CAN_INITIAL_REQUEST_NOT_DONE):
                dbg_tx_text("OBD_CAN_INITIAL_REQUEST_NOT_DONE"); break;
            case(OBD_CAN_ONGOING):
                dbg_tx_text("OBD_CAN_ONGOING\r\n");              break;
            case(OBD_CAN_NOT_SUPPORTED):
                dbg_tx_text("OBD_CAN_NOT_SUPPORTED\r\n");        break;
            case(OBD_CAN_SUCCESS):
                dbg_tx_text("OBD_CAN_SUCCESS\r\n");              break;
            case(OBD_CAN_NOT_ENABLED):
                dbg_tx_text("OBD_CAN_NOT_ENABLED\r\n");          break;
        }
	}
}

void obd_can_debug_config ( void )
{
	UInt8 i;

	dbg_tx_text("*** obd_can_cfg:\r\n");

    dbg_tx_text("    flg_ign=");
	dbg_tx_h8(obd_can_vars.flg_ign);
	dbg_tx_text("\r\n");
    dbg_tx_text("    activated=");
	dbg_tx_h8(obd_can_vars.is_active);
	dbg_tx_text("\r\n");
    dbg_tx_text("    rx_complete=");
	//dbg_tx_h8(obd_can_vars.rx_complete);
	dbg_tx_text("\r\n");

	for (i=0; i<sizeof_array(obd_can_cfg); i++)
	{
	    dbg_tx_request () ;
		dbg_tx_text("    ");
		dbg_tx_h8(i);
		dbg_tx_text(": is_sup=0x");
		dbg_tx_h8(obd_can_cfg[i].is_enabled);
		dbg_tx_text(" period=0x");
		dbg_tx_h8(obd_can_cfg[i].time_based_period);
		dbg_tx_text(" event=0x");
		dbg_tx_h8(obd_can_cfg[i].event_based_cmd);
		dbg_tx_text(" delay=0x");
		dbg_tx_h8(obd_can_cfg[i].event_based_delay);
		dbg_tx_text("\r\n");
		dbg_tx_release () ;
	}
}

#endif
