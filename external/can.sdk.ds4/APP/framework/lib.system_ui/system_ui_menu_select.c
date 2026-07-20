/*==========================================================================*/
// $Id: dei_sc_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "system_ui_private.h"
#include "ngmm_feature.h"
#include "cmd.h"
#include "prg.h"
#include "reset.h"   //reset delete_all_nvfs_vars
#include "status_system.h"   //get_ss_arm/lock
#include "dei_system.h"
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

//--------------------------------------------------------------------------//
void system_ui_menu_select( void )
{
	UInt8 i;
	typedef struct s_cmd_state_map
	{
		UInt32 state;
		UInt32 cmd;
	}cmd_state_map;
	
	static cmd_state_map cmd_num[15]=
	{
		{TACH_LEARN_STATE, 			INPUT_TACH_LEARN			},
		{TACH_LEARN_DONE, 			INPUT_TACH_LEARN_DONE	},
		{RS_SHUT_DOWN_STATE,		INPUT_RS_SHUTDOWN_HISTORY	},
		{TRIG_HIST_STATE, 			INPUT_ALARM_TRIG_HISTORY	},
		//{ALARM_VALET_STATE, 		INPUT_ALARM_VALET			},
		{TACH_LEARN_DONE_SECOND, 		INPUT_TACH_LEARN_DONE	},
		
		{REMOTE_PAIRING_STATE, 				INPUT_REMOTE_PAIRING		},
		{REMOTE_DELETE_STATE, 				INPUT_REMOTE_DELETE			},
		{ZAP_STATE, 									INPUT_ZAP					},
		{TACH_DELETE_STATE, 					INPUT_TACH_DELETE			},
		{TAP_HOLD_STATE   , 					INPUT_TAP_HOLD   			},
		{QUICK_TAP_HOLD_STATE   , 		INPUT_TAP_HOLD   			},
		{TWO_TAP_HOLD_STATE   ,				INPUT_TWO_TAP_HOLD   			},
		{QUICK_TWO_TAP_HOLD_STATE,		INPUT_TWO_TAP_HOLD   			},
		{THREE_TAP_HOLD_STATE  ,			INPUT_THREE_TAP_HOLD   	},
		{QUICK_THREE_TAP_HOLD_STATE,	INPUT_THREE_TAP_HOLD   	},
		
	};

	TRACE_SYS_UI("[UI_BTN_CNT: %d], [IGN_CNT: %d]\n\r", system_ui_btn_cnt, system_ui_ign_cnt);
	TRACE_SYS_UI("[1st_action: %d]\n\r", system_ui_first_action);	
	
	system_ui_sel_func = (system_ui_btn_cnt) | (system_ui_ign_cnt <<4);
	if( system_ui_first_action == IGN_FIRST )
		system_ui_sel_func |= 0x80;

	TRACE_SYS_UI("SEL_FUNC: 0x%2X\n\r", system_ui_sel_func);
	
	if( system_ui_sel_func == OVERRIDE_STATE 
		 || ( get_ss_trigger() 
	         && ( system_ui_sel_func == TACH_LEARN_STATE || system_ui_sel_func == (IGN_ON_STATE | HOLD_5_SEC_STATE) 
	             || system_ui_sel_func == (IGN_ON_STATE | 0x02)))  // accept hold for override
	  )
	{
		cmd_q_rs_insert( INPUT_EMERGENCY_OVERRIDE, 100 );

		//fall through, because TACH_LEARN_DONE == OVERRIDE_STATE (0x90 | 1*2)!
	}

	if( !tap_while_ign_on && (system_ui_sel_func == ALARM_VALET_STATE) 
		 && !system_ui_hold_status )
	{
		cmd_q_rs_insert (INPUT_ALARM_VALET , 100);
		system_ui_hold_status = FALSE;
	}
		
	if( system_ui_sel_func == HOLD_5_SEC_STATE && (system_ui_state & 0x04))
	{
		system_ui_cmd = system_ui_sel_func;
		//start 3 sec timer
		system_ui_hold_timeout = TRUE;
		vrtc_stop(system_ui_cmd_vrtc);
		vrtc_update(system_ui_cmd_vrtc, HHU_ALARM_TRIG_TIMEOUT);
		vrtc_start( system_ui_cmd_vrtc );
	}
	
	if( system_ui_sel_func == RS_VALET_STATE )
	{
		system_ui_cmd = system_ui_sel_func;
		//start 5 sec timer
		system_ui_hold_timeout = TRUE;
		vrtc_stop(system_ui_cmd_vrtc);
		vrtc_update(system_ui_cmd_vrtc, RS_VALET_TIMEOUT);
		vrtc_start( system_ui_cmd_vrtc );
	}

	for(i=0; i<15; i++)
	{
		if( system_ui_sel_func == cmd_num[i].state )
		{
			cmd_q_rs_insert(cmd_num[i].cmd, 100);
			break;
		}
	}
	
	if((system_ui_state & 0x01) )		//if last state timer running
	{
		vrtc_stop( system_ui_vrtc );
		switch ( system_ui_sel_func )
		{
			case TACH_LEARN_STATE:
			{
				vrtc_update(system_ui_vrtc, TACH_LEARN_TIMEOUT);
				vrtc_start( system_ui_vrtc );
			}
			break;
			
			case REMOTE_PAIRING_STATE:
			case REMOTE_DELETE_STATE:
			case ZAP_STATE:
			case TACH_DELETE_STATE:
			{
				system_ui_reset();
				system_ui_program_menu = TRUE;
				vrtc_update(system_ui_vrtc, PRG_MENU_TIMEOUT);
				vrtc_start( system_ui_vrtc );
			}
			break;
			
			case TAP_HOLD_STATE:
			case TWO_TAP_HOLD_STATE:
			case THREE_TAP_HOLD_STATE:
			case QUICK_TAP_HOLD_STATE:
			case QUICK_TWO_TAP_HOLD_STATE:
			case QUICK_THREE_TAP_HOLD_STATE:
			{
				system_ui_reset();
				system_ui_program_menu = TRUE;

				vrtc_update(system_ui_vrtc, PRG_MENU_NEXT_TIMEOUT);
				vrtc_start( system_ui_vrtc );
			}
			break;
			
			case IGN_ON_STATE:
			{
				if( system_ui_first_action == NOT_DETECTED)
				{
					vrtc_update(system_ui_vrtc, PRG_MENU_NEXT_TIMEOUT);
					vrtc_start( system_ui_vrtc );
				}
			}
			break;
			
			default:
			{
					vrtc_update(system_ui_vrtc, BUTN_PRESS_TIMEOUT);
					vrtc_start( system_ui_vrtc );
			}
			break;
		}
	}
	else
	{
		system_ui_reset();
	}
	
}

static hard_reset_action(void)
{
	  OUT_LED_RED(TRUE);      //indicate something important
	  reset_service();
	  reset_long_service();
	  nvram_usr_commit();
	  os_tsk_wait(1000);
	  OUT_LED_RED(FALSE);
	  wdog_v_force_reset(WDOG_V_RESET_FIRMWARE,(char*)__func__,__LINE__);
}	


//--------------------------------------------------------------------------//
void system_ui_prg_via_ble(UInt8 mode)
{
	UInt8 umode = mode & 0xF;
  if(umode && umode <= 4)   //program mode only
  {
    prg_menu_set(umode);
    
    prg_exit_set(TRUE);
		
		
		if(!dei_system_prg_mode_get())
			cmd_q_rs_insert(INPUT_BLE_PRG_ENTER, 100);//trigger BLE program menu
		
    vrtc_stop(system_ui_ble_evt_vrtc);
		vrtc_update(system_ui_ble_evt_vrtc, VRTC_1SEC); //send event in 1sec
		vrtc_start(system_ui_ble_evt_vrtc);
  }
	else if(mode && (mode == 0xC0))
	{   //hard reset request from phone
			if( !(get_ss_arm() || get_ss_lock()))
			{
			  hard_reset_action();    //this function never returns
				// return;                // implied 
			}
	    mainp_status(0);            //forbid hard reset
	}
  else 
  {
		// umode == 0
    //if(prg_exit_get())
    {
      prg_menu_set(0);
			ble_prg_evt_exit();
    }
  }
}

//--------------------------------------------------------------------------//
void system_ui_ble_evt(void)
{
    ble_prg_evt_enter();
}
