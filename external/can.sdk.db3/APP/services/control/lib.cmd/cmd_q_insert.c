/*==========================================================================*/
// $Id: cmd_q_insert.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "cmd_private.h"
#if	defined PLATFORM_CM800 || defined PLATFORM_933		//def PLATFORM_CM800
	#include "evt.h"
	#include "d2d.h"
#endif

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

/*--------------------------------------------------------------------------*/
#if	defined PLATFORM_CM800 || defined PLATFORM_933				//def PLATFORM_CM800 and PLATFORM_933
Boolean cmd_q_rs_insert ( Cmd cmd, TskTimeout timeout , ...)
#else
Boolean cmd_q_insert ( Cmd cmd, TskTimeout timeout , ...)
#endif
{	
	Cmd_Q_Msg * cmd_q_msg;

	insert_cmd_q_msg:
	{
		cmd_q_msg = (Cmd_Q_Msg *) q_remove (cmd_q_free , timeout) ;
			
		if (cmd_q_msg)
		{
			va_list args ;
	
			cmd_q_msg->cmd = (Cmd_List)cmd ;
			
			va_start( args , timeout ) ;
			{
				cmd_q_msg->param = (Cmd_Pld)va_arg( args , VARG ) ; 
			}
			va_end( args ) ; 
			
			q_insert (cmd_q_pending , (Q_Msg *) cmd_q_msg) ;
			
			return TRUE ;
		}
		else
		{
			if( exec_q_free_add( cmd_q_free , sizeof( Cmd_Q_Msg) ) )
			{
                cmd_error (ERR_CMD_INSERT_TIMEOUT) ;
                goto insert_cmd_q_msg ; 
			}		
		}
	}
		
	return FALSE ;		
}

#if	defined PLATFORM_CM800 || defined PLATFORM_933		//def PLATFORM_CM800

Boolean cmd_q_insert (Cmd cmd, TskTimeout timeout)
{	
	switch(cmd)
	{
		case INPUT_BRAKE_STATUS_PRESS: evt_notify(SET_BRAKE_ON); return TRUE;
		case INPUT_BRAKE_STATUS_RELEASE: evt_notify(SET_BRAKE_OFF); return TRUE;
		case INPUT_PRK_NEUTRAL_STATUS_ON: evt_notify(SET_PRK_NTRL_ACTIVE); return TRUE;
		case INPUT_PRK_NEUTRAL_STATUS_OFF: evt_notify(SET_PRK_NTRL_NOT_ACTIVE); return TRUE;
		case INPUT_HANDBRAKE_STATUS_ON: evt_notify(SET_HANDBRAKE_ON); return TRUE;
		case INPUT_HANDBRAKE_STATUS_OFF: evt_notify(SET_HANDBRAKE_OFF); return TRUE;
		case INPUT_HOOD_STATUS_OPEN: evt_notify(SET_HOOD_OPEN); return TRUE;
		case INPUT_HOOD_STATUS_CLOSE: evt_notify(SET_HOOD_CLOSE); return TRUE;
		case INPUT_TRUNK_STATUS_OPEN: evt_notify(SET_TRUNK_OPEN); return TRUE;
		case INPUT_TRUNK_STATUS_CLOSE: evt_notify(SET_TRUNK_CLOSE); return TRUE;
		case INPUT_IGN_STATUS_ON: evt_notify(SET_IGN_ON); return TRUE;
		case INPUT_IGN_STATUS_OFF: evt_notify(SET_IGN_OFF); return TRUE;
		case INPUT_ALL_DOOR_STATUS_OPEN: 
			evt_notify(SET_OTHER_OPEN); 
			cmd_q_rs_insert(cmd, timeout, 0); // Also send to other systems (Falcon takeover)
			return TRUE;
		case INPUT_ALL_DOOR_STATUS_CLOSE: evt_notify(SET_OTHER_CLOSE); return TRUE;
		case INPUT_GLOWPLUG_STATUS_ON: evt_notify(SET_GLOWPLUG_ON); return TRUE;
		case INPUT_GLOWPLUG_STATUS_OFF: evt_notify(SET_GLOWPLUG_OFF); return TRUE;
		case INPUT_SENSOR_TRIGGER: evt_notify(FN_ALARM_TRIGGERED); return TRUE;
		case INPUT_KEY_IN_STATUS_ON: evt_notify(SET_KEY_IN); return TRUE;
		case INPUT_KEY_IN_STATUS_OFF: evt_notify(SET_KEY_OUT); return TRUE;
		case INPUT_OEM_LOCK: evt_notify(SET_KEYLESS_LOCK); return TRUE;
		case INPUT_OEM_UNLOCK: evt_notify(SET_KEYLESS_UNLOCK); return TRUE;
		case INPUT_OEM_ARM: evt_notify(SET_KEYLESS_ARM); return TRUE;
		case INPUT_OEM_DISARM: evt_notify(SET_KEYLESS_DISARM); return TRUE;
		case INPUT_OEM_TRUNK: evt_notify(SET_KEYLESS_TRUNK); return TRUE;
		case INPUT_OEM_PANIC: evt_notify(SET_KEYLESS_PANIC); return TRUE;
		case INPUT_OEM_AUX1: evt_notify(SET_KEYLESS_AUX1); return TRUE;
		case INPUT_OEM_AUX2: evt_notify(SET_KEYLESS_AUX2); return TRUE;
		case INPUT_OEM_AUX3: evt_notify(SET_KEYLESS_AUX3); return TRUE;
		case INPUT_OEM_AUX4: evt_notify(SET_KEYLESS_AUX4); return TRUE;
		case INPUT_OEM_RS_ON: evt_notify(SET_KEYLESS_START); return TRUE;
		case INPUT_OEM_RS_OFF: evt_notify(SET_KEYLESS_STOP); return TRUE;
        default: cmd_q_rs_insert(cmd, timeout,0); return TRUE;
	}
		
}


/*--------------------------------------------------------------------------*/

void cmd_q_rs_insert_hhu_num( UInt8 hhu_num , Cmd cmd, TskTimeout timeout , UInt32 payload )
{
	if( hhu_num == 0 )
		cmd_q_rs_insert( cmd , timeout , ( void* )payload );
	else
		cmd_q_rs_insert( (Cmd) (cmd+1) , timeout , ( void* )payload );
}

#endif
/*--------------------------------------------------------------------------*/



