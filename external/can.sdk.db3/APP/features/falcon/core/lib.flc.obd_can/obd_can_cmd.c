/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: obd_can_cmd.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

void obd_can_cmd(void *arg)
{
  UInt8 i;
  UInt8 cmd;

  // We received a command which is used as a trigger for event-based requests
  // Let's determine which command from arg
  Exec_Q_Msg * exec_q_msg = (Exec_Q_Msg *) arg;
  Cmd_Q_Msg  * cmd_q_msg  = (Cmd_Q_Msg *) exec_q_msg;
  cmd = cmd_q_msg->cmd;

  if (!obd_can_vars.flg_ign) return;

#if DBG_OBD_CAN == 1
  dbg_tx_text("*** obd_can_cmd: cmd=");
  dbg_tx_h8(cmd);
  dbg_tx_text("\r\n");
#endif

  // Let's find out which request(s) uses the command, and schedule the request
  for (i=0; i<sizeof_array(obd_can_schedule); i++)
  {
    if (obd_can_cfg[i].event_based_cmd == cmd)
 	{
	  // Update the schedule
	     
	    	obd_can_schedule[i].when = obd_can_vars.schedule_timer + obd_can_cfg[i].event_based_delay;
		
	}
  }

}

