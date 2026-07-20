/*==========================================================================*/
// $Id: dei_sc_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "system_ui_private.h"
#include "ngmm_feature.h"
#include "cmd.h"
#include "dei_system.h"
#include "status_system.h"   //get_ss_arm
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define SECX8_TO_TICKS_CONV_MULTIPLIER		(CPU_CLOCK_HZ>>8)

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void system_ui_menu_select( void );
void system_ui_butn_cnt( UInt8 cnt, void* payload );
void system_ui_timeout_reset( void );
void system_in_used_check(void);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void  system_ui_init    ( void ) 
{
	cmd_register :
    {
        static const Cmd_Array1 cmd_array[] = 
        {                
					{INPUT_EXT_PUSH_VALET_RELEASE		, (Exec_Func) system_ui_butn_release },
					{INPUT_PUSH_VALET_PRESS_				, (Exec_Func) system_ui_butn_press},
					{INPUT_BUTTON_TAP1							, (Exec_Func) system_ui_butn_1_tap},
					{INPUT_BUTTON_TAP1_HOLD					, (Exec_Func) system_ui_butn_1_tap_hold},
					{INPUT_BUTTON_TAP2							, (Exec_Func) system_ui_butn_2_tap},
					{INPUT_BUTTON_TAP2_HOLD					, (Exec_Func) system_ui_butn_2_tap_hold},
					{INPUT_BUTTON_TAP3   						, (Exec_Func) system_ui_butn_3_tap},
					{INPUT_BUTTON_TAP3_HOLD   			, (Exec_Func) system_ui_butn_3_tap_hold},
					{INPUT_BUTTON_TAP4   						, (Exec_Func) system_ui_butn_4_tap},
					{INPUT_BUTTON_TAP4_HOLD   			, (Exec_Func) system_ui_butn_4_tap_hold},
					{INPUT_BUTTON_TAP5   						, (Exec_Func) system_ui_butn_5_tap},
					{INPUT_BUTTON_TAP5_HOLD   			, (Exec_Func) system_ui_butn_5_tap_hold},
					{INPUT_IGN_STATUS_ON  					,	(Exec_Func) system_ui_ign_on},
					{INPUT_IGN_STATUS_OFF						, (Exec_Func) system_ui_ign_off},
					{INPUT_PRG_MODE_EXIT						, (Exec_Func) system_ui_timeout_reset},
					//DS4 is in used?
					{INPUT_ARM                      , (Exec_Func) system_in_used_check },
					{INPUT_GWR_ON                   , (Exec_Func) system_in_used_check },
	  
       };

        CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL | EXEC_MODE_PROG);
    }
		
	system_ui_program_menu 			= FALSE;
	system_ui_first_action			= NOT_DETECTED;
	system_ui_hold_timeout      = FALSE;
  system_ui_flg_ign           = FALSE;
		
	system_ui_vrtc      				= vrtc_f_create( (Exec_Func) system_ui_execute ) ; 
	system_ui_cmd_vrtc					= vrtc_f_create( (Exec_Func) system_ui_cmd_execute );
		
  system_ui_ble_evt_vrtc      = vrtc_f_create((Exec_Func) system_ui_ble_evt);		

}

//--------------------------------------------------------------------------//
void system_ui_first_action_check(void* payload)
{
	UInt32 tstamp_x8;
	if(system_ui_first_action == NOT_DETECTED)
	{
		system_ui_tstamp_curr = T1TC;		// ignition curr time stamp time stamp

		tstamp_x8 = (BUTN_PRESS_TIMESTAMP(payload) * SECX8_TO_TICKS_CONV_MULTIPLIER);
		
//		TRACE_SYS_UI("tstamp_ign: [%8X]\n\r", system_ui_tstamp_ign);
		TRACE_SYS_UI("tdiff_cur_ign(dec): [%d]\n\r", (SInt32)((system_ui_tstamp_curr - system_ui_tstamp_ign) / SECX8_TO_TICKS_CONV_MULTIPLIER));
		TRACE_SYS_UI("tstamp_x8(dec): [%d]\n\r", (tstamp_x8 / SECX8_TO_TICKS_CONV_MULTIPLIER));
		
		if ((system_ui_tstamp_ign != 0) && ((system_ui_tstamp_curr - system_ui_tstamp_ign) > tstamp_x8))
		{
			// ignition first
			system_ui_first_action = IGN_FIRST;
		}
		else
		{
			system_ui_first_action = BUTN_PRESS_FIRST;
		}
	}
}

volatile UInt32 tstamp_never;
//--------------------------------------------------------------------------//
Boolean system_ui_tap_cnt_before_ign(void* payload)
{
	UInt32 tstamp_frac, tstamp_sec, tstamp ;
	tstamp_never = BUTN_PRESS_TIMESTAMP(payload);
	//tstamp_sec = ((tstamp_never>>8) - 0x01)
	//tstamp_frac = ((tstamp_never && 0xff) - 0x2D);
	
	
		tstamp = ((tstamp_never - 0x12D) * SECX8_TO_TICKS_CONV_MULTIPLIER);
	
		system_ui_tstamp_curr = T1TC;		// ignition curr time stamp time stamp

	if( ((system_ui_tstamp_curr - system_ui_tstamp_ign) > tstamp) && ( system_ui_tstamp_ign != 0) )
		return FALSE;
	else
		return TRUE;
}

//--------------------------------------------------------------------------//

void system_ui_reset( void )
{
		system_ui_first_action = NOT_DETECTED;
		system_ui_btn_cnt = 0;		// reset button press count after sending the command
		system_ui_ign_cnt = 0;
		system_ui_tstamp_curr = 0;
		system_ui_tstamp_ign = 0;
		system_ui_program_menu = FALSE;
		system_ui_hold_status = FALSE;
}

//--------------------------------------------------------------------------//
void system_ui_execute(void)
{
	system_ui_state &=  (~0x01);		// turn OFF timer

	vrtc_stop( system_ui_vrtc );
	
	if(!system_ui_state) // system_ui_state is 0 mean going to IDLE state
	{
		system_ui_reset();
	}
}

//--------------------------------------------------------------------------//

void system_ui_cmd_execute( void )
{
	vrtc_stop( system_ui_cmd_vrtc );
	
	if((system_ui_cmd == HOLD_5_SEC_STATE) && system_ui_hold_timeout)
	{
		cmd_q_rs_insert( INPUT_CONVENIENCE_ALERT, 100 );
		system_ui_state = 0;
	}
	
	if( (system_ui_cmd == RS_VALET_STATE) & system_ui_hold_timeout )
	{
		cmd_q_rs_insert( INPUT_RS_VALET, 100 );
		system_ui_state = 0;
	}
	
	system_ui_hold_timeout =  FALSE;
}

//--------------------------------------------------------------------------//
void system_ui_butn_release(void)
{
	system_ui_state &= (~0x04);		// button release
	
	system_ui_hold_timeout = FALSE;		// reset the hold timeout if release happen in between
	
	if( system_ui_btn_cnt )
	{
		system_ui_btn_cnt++;
	
		system_ui_menu_select();
	}

}

//--------------------------------------------------------------------------//
void system_ui_butn_press(void* payload)
{
	if(!system_ui_state)		// check for coming out of IDLE mode of state machine
	  system_ui_state |= 0x01;		// turn ON timer

	system_ui_state |= 0x04;		// button pressed
	system_ui_btn_cnt++;
	
	system_ui_hold_status = TRUE;
	
	timestamp_payload = BUTN_PRESS_TIMESTAMP(payload);

	system_ui_first_action_check( payload );
	
	system_ui_menu_select();
}

//--------------------------------------------------------------------------//
void system_ui_butn_1_tap(void* payload)
{
	UInt32 tstamp_from,tstamp ;
	UInt32 tstamp_now;
	tstamp_from = BUTN_PRESS_TIMESTAMP(payload);
	
	tstamp = ((tstamp_from - 0x12D) * SECX8_TO_TICKS_CONV_MULTIPLIER);	
	tstamp_now = T1TC;
	
	if(get_ign_status() && (tstamp_now - system_ui_tstamp_ign) > tstamp)
		tap_while_ign_on = 1;
	else
		tap_while_ign_on = 0;
	system_ui_butn_cnt(2, payload);
	system_ui_state &= (~0x04); //go back to release state and increment the btn count
}

//--------------------------------------------------------------------------//
void system_ui_butn_1_tap_hold(void* payload)
{
	system_ui_butn_cnt(3, payload);
	system_ui_state |= 0x04;
}

//--------------------------------------------------------------------------//
void system_ui_butn_2_tap(void* payload)
{
	system_ui_butn_cnt(4, payload);
	system_ui_state &= (~0x04); //go back to release state and increment the btn count
}

//--------------------------------------------------------------------------//
void system_ui_butn_2_tap_hold(void* payload)
{
	system_ui_butn_cnt(5, payload);
	system_ui_state |= 0x04;
}

//--------------------------------------------------------------------------//
void system_ui_butn_3_tap(void* payload)
{
	system_ui_butn_cnt(6, payload);
	system_ui_state &= (~0x04); //go back to release state and increment the btn count

}

//--------------------------------------------------------------------------//
void system_ui_butn_3_tap_hold(void* payload)
{
	system_ui_butn_cnt(7 , payload);
	system_ui_state |= 0x04;
}

//--------------------------------------------------------------------------//
void system_ui_butn_4_tap(void* payload)
{
	system_ui_butn_cnt(8 , payload);
	system_ui_state &= (~0x04); //go back to release state and increment the btn count

}

//--------------------------------------------------------------------------//
void system_ui_butn_4_tap_hold(void* payload)
{
	system_ui_butn_cnt(9, payload);
	system_ui_state |= 0x04;
}

//--------------------------------------------------------------------------//
void system_ui_butn_5_tap(void* payload)
{
	system_ui_butn_cnt(10 , payload);
	system_ui_state &= (~0x04); //go back to release state and increment the btn count

}

//--------------------------------------------------------------------------//
void system_ui_butn_5_tap_hold(void* payload)
{
	system_ui_butn_cnt(11, payload);
	system_ui_state |= 0x04;
}

//--------------------------------------------------------------------------//
void system_ui_ign_on(void)
{
  if(system_ui_flg_ign == FALSE)
  {
    system_ui_flg_ign = TRUE;
    
    if(!system_ui_state)		// check for coming out of IDLE mode of state machine
      system_ui_state |=  0x01;		// turn ON timer
    
    system_ui_hold_timeout = FALSE;		// pager message send state cancelled

    system_ui_state |= 0x02;		// ignition ON
    system_ui_ign_cnt++;
		system_ui_ign_cnt &= 0x01;  //ignition transistion starts from ign on
    
    system_ui_tstamp_ign = T1TC;		// ignition satrt time stamp

    system_ui_menu_select();
  }
}

//--------------------------------------------------------------------------//
void system_ui_ign_off(void)
{
  if(system_ui_flg_ign == TRUE)
  {
    system_ui_flg_ign = FALSE;		
		
		if(get_ss_arm())  //start over to retry override
			system_ui_btn_cnt = 0;
    
    system_ui_state &= (~0x02);		// ignition off
    system_ui_ign_cnt++;

    //system_ui_tstamp_ign = T1TC;		// ignition satrt time stamp
    
    system_ui_menu_select();
  }
}

//--------------------------------------------------------------------------//
void system_ui_butn_cnt( UInt8 cnt, void* payload )
{
	UInt32 i;
	if( system_ui_state & 0x04 )
	{
		system_ui_btn_cnt++;
	}

	if( !system_ui_tap_cnt_before_ign( payload ) || system_ui_program_menu )
	{
		for(i=0; i<cnt; i++)
			system_ui_btn_cnt++;
		system_ui_first_action_check( payload );
	}

	system_ui_menu_select();
}

//--------------------------------------------------------------------------//
void system_ui_timeout_reset( void )
{
	vrtc_stop( system_ui_vrtc );
	vrtc_update(system_ui_vrtc, PRG_MENU_EXIT);
	vrtc_start( system_ui_vrtc );
	
	system_ui_reset();
}


//--------------------------------------------------------------------------//
void system_in_used_check(void)
{
	UInt16 size = 0;
	
	if(!system_in_used_status)
	{
		system_in_used_status = 1;
		
		nvfs_wr_create(nvfs_sys_in_used, &system_in_used_status, sizeof(system_in_used_status));

//		TRACE_SYS_UI("[SYS_IN_USE: %d]\n\r", system_in_used_status);
		
		if((mainp_last_staus_get() & 0x80 ) && !dei_system_prg_mode_get())  //installer allowed mode, and not in programming
		{
			mainp_status(get_ss_arm() ? MAINP_ARMED : MAINP_DISARMED);
		}
	}
}


//--------------------------------------------------------------------------//
void system_ui_nvfs_init(void)
{
	UInt16 size = 0;
	
	system_in_used_status = 0;
	if(nvfs_rd_size(nvfs_sys_in_used, &size) == NVFS_OK)
		system_in_used_status = 1;

//	TRACE_SYS_UI("[SYS_IN_USE: %d]\n\r", system_in_used_status);
}

//API to report in_used status
//--------------------------------------------------------------------------//
UInt8 system_in_used_status_get(void)
{
	return system_in_used_status;
}
