/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: obd_can_config.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
			
Boolean obd_can_config(Obd_Can_Req_Enum request, SInt8 period, UInt16 cmd, SInt8 delay)
{
	return obd_can_config_internal(request, period, cmd, delay, TRUE);
}

//--------------------------------------------------------------------------//

Boolean obd_can_config_set_enabled(Obd_Can_Req_Enum request, Boolean is_enabled)
{
	if (request >= OBD_CAN_REQ_COUNT) return FALSE;
	obd_can_cfg[request].is_enabled = is_enabled;
    return nvfs_wr(nvfs_obd_can_config, &obd_can_cfg) == NVFS_OK ;  
}

//--------------------------------------------------------------------------//
Boolean obd_can_config_internal(Obd_Can_Req_Enum request, SInt8 period, UInt16 cmd, SInt8 delay, Boolean save_nvfs)
{

#if DBG_OBD_CAN == 1
  dbg_tx_request () ;
  dbg_tx_text ("\r\n");
  dbg_tx_text ("*** obd_can_config: req="); dbg_tx_h8(request);
  dbg_tx_text (" period="); dbg_tx_h8(period);
  dbg_tx_text (" cmd="); dbg_tx_h8(cmd);
  dbg_tx_text (" delay="); dbg_tx_h8(delay);
  dbg_tx_text ("\r\n");
  dbg_tx_release () ;
#endif
	// Sanity checks
	if (request >= OBD_CAN_REQ_COUNT) return FALSE;
	if (!obd_can_cfg[request].is_enabled) return FALSE;

	// Configure the request

   	obd_can_schedule[request].status = OBD_CAN_INITIAL_REQUEST_NOT_DONE;

	
	    // Configure event-based request
		if (obd_can_cfg[request].event_based_cmd != OBD_CAN_NO_EVENT)
		{
	      cmd_lut_remove((Cmd)obd_can_cfg[request].event_based_cmd, obd_can_cmd);
		}
		obd_can_cfg[request].event_based_cmd = cmd;
	    if (cmd != OBD_CAN_NO_EVENT)
	    {
		  cmd_lut_insert((Cmd)cmd, obd_can_cmd, EXEC_MODE_NORMAL);
		}
		obd_can_cfg[request].event_based_delay = delay;

	    // Configure time-based request
		if (period == OBD_CAN_ON_DEMAND)
		{
          obd_can_schedule[request].when = obd_can_vars.schedule_timer;
		}
		else
		{
          obd_can_schedule[request].when = period != 0 ? obd_can_vars.schedule_timer + period : 0;
		  obd_can_cfg[request].time_based_period = period;
		}
	

    return save_nvfs ? nvfs_wr(nvfs_obd_can_config, &obd_can_cfg) == NVFS_OK : TRUE;  
}

