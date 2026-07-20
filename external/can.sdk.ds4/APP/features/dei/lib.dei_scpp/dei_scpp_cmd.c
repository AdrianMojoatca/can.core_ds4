/*==========================================================================*/
// $Id: dei_scpp_cmd.c 33405 2015-08-26 21:21:53Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_sc_private.h"
#include "d2d.h"
#include "status_system.h"
#include "dei_ss.h"
#include "dei_led.h"
#include "dei_siren.h"
#include "aid_dhp.h"
#include "sensor_proto.h"
#include "dei_system.h" //dei_system_prg_mode_get()

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define DEI_SC_BUTTON_XPRESS_RELEASED             DEI_SC_BUTTON_NOT_USED_D0
#define DEI_RS_VRTC_TIMEOUT_SENSOR_ADJUST_MODE    VRTC_1SEC*60  							//Timeout for sensor adjust mode, if no activity from HHU or control button on IVU

#define DEI_SC_CMD_RELEASE_TIMEOUT  1500     // 1.5 sec up to extend press which will send xpress release
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_sc_cmd_release_tmr_rewind(UInt16 delta);
static void dei_sc_cmd_release_tmr_stop(void);
static void dei_sc_cmd_sensor_adjust_mode(UInt8 cmd, Cmd_Rf_Pld *btn_pld);
static Boolean is_aux_command(UInt8 cmd);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Boolean btn_hold_prev;
static UInt8  last_btn_cmd;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void dei_sc_pagging_rewind( unsigned ms )
{
    timeout_update ( dei_sc_timeout_pagging , ms );
    timeout_start  ( dei_sc_timeout_pagging );
}


//--------------------------------------------------------------------------//
void dei_scpp_cmd(UInt8 *cmd_msg, UInt8 msg_size)
{
	  UInt8 hdr, cmd, pld;
	  UInt16 t_stamp = 0;
	  UInt8 idx;
    Boolean press_type;
    Cmd_Rf_Pld     btn_pld;
    btn_pld.pld = 0 ; 
	
		pld = 0;
		idx=1;   //cmd_msg[0] is proto_id
		hdr = cmd_msg[idx++];
		cmd = cmd_msg[idx++];
    if(msg_size >= 4)
	    pld = cmd_msg[idx];  //idx==3
		if(msg_size == 4 || msg_size == 6)
		{
			if(msg_size == 6)
			{
				t_stamp = (cmd_msg[idx+1] <<8) | cmd_msg[idx+2];  //pass the button press time stamp
				
				t_stamp++;  //DEBUG: need to compare with ignition on starting time
			}
		}
    
	
    if(cmd == (DEI_SC_BUTTON_SWITCH_PATTERN + 0xA0))
    {
        pld += INPUT_PUSH_VALET_RELEASE_;
        if(pld >= INPUT_PUSH_VALET_RELEASE_ && pld <= INPUT_BUTTON_TAP5_HOLD)  //pld is in order
        {
            if(pld == INPUT_PUSH_VALET_RELEASE_ && btn_hold_prev)
                cmd_q_rs_insert(INPUT_EXT_PUSH_VALET_RELEASE, 100, (void *) t_stamp);
            else
               cmd_q_rs_insert(pld, 100, (void*) t_stamp);
            
            if(pld & 0x01)
                btn_hold_prev = TRUE;
            else
                btn_hold_prev = FALSE;
        }
		if(dei_sc_mode == DEI_SC_MODE_SENSOR_ADJUST)
			{
				vrtc_stop( dei_sc_sensor_adjust_mode_vrtc );
				vrtc_update(  dei_sc_sensor_adjust_mode_vrtc, DEI_RS_VRTC_TIMEOUT_SENSOR_ADJUST_MODE	);
				vrtc_start( dei_sc_sensor_adjust_mode_vrtc );
			}

        return;
    }
    else if(cmd == (DEI_SC_BUTTON_LEARNING_BEGIN + 0xA0) && (dei_sc_mode == DEI_SC_MODE_PROGRAMMING))
    {
        cmd_q_rs_insert(INPUT_RF_PROGRAMMING_DONE, 100);
        return;
    }
    else if(cmd == (DEI_SC_BUTTON_XPRESS_RELEASED + 0xA0))   //notification of release of HHU extend press
    {
       dei_sc_cmd_release_tmr_stop();

       if( is_aux_command(last_btn_cmd))
         cmd_q_rs_insert( (Cmd)((last_btn_cmd - 0xA0 + INPUT_RF_ENTRY + 1)|INPUT_RF_RELEASE) , 100 , btn_pld.pld );

       return;
    }
    else if( (cmd == (DEI_SC_BUTTON_SENSOR1_ADJUST + 0xA0)) 
            && ((scpp_system_type & SYS_TYPE_SS) == SYS_TYPE_SS) )
    {
			if( dei_sc_mode == DEI_SC_MODE_SENSOR_ADJUST )
				return;
			dei_sc_sensor_adjust_hhu = (hdr & 0x03);
			dei_sc_mode = DEI_SC_MODE_SENSOR_ADJUST;
			wake_enter_flagged(WAKE_SCPP_CMD);		// added for sensor adjust mode to be system awake
			dei_ss_sensor_curr_aid_reset();
			dei_ss_sensor_curr_chanl_reset();

			sensadj_sort();
			
			vrtc_update(  dei_sc_sensor_adjust_mode_vrtc, DEI_RS_VRTC_TIMEOUT_SENSOR_ADJUST_MODE	);
			vrtc_start( dei_sc_sensor_adjust_mode_vrtc );
    }
    else if ( cmd == (DEI_SC_BUTTON_EXIT_PROGRAMMING_MODE + 0xA0))
    {
		if( dei_sc_mode == DEI_SC_MODE_SENSOR_ADJUST)
			dei_led_flash(DEI_LED_RESET, TRUE);
		
      dei_sc_mode = DEI_SC_MODE_NORMAL;
    }

    press_type = !!(hdr & 0x08);
		
		// if new aux cmd?
		if(!press_type && is_aux_command(cmd) && is_aux_command(last_btn_cmd)) 
		{
				if(dei_sc_cmd_release_tmr_cnt && cmd != last_btn_cmd)  // we own a RF_RELEASE
					cmd_q_rs_insert( (Cmd)((last_btn_cmd - 0xA0 + INPUT_RF_ENTRY + 1)|INPUT_RF_RELEASE) , 100 , btn_pld.pld );
		}

    last_btn_cmd = cmd;

    // store current HHU number
    dei_sc_current_sn = (hdr & 0x03);
    
    dei_sc_is_silent = FALSE ; 

    if( (cmd == (DEI_SC_BUTTON_SILENT_LOCK + 0xA0)) || (cmd == (DEI_SC_BUTTON_SILENT_UNLOCK + 0xA0)))
    {
			dei_sc_is_silent = TRUE ; 
    }
		
		if((cmd == (DEI_SC_BUTTON_ONE_BUTTON_FUNCTION + 0xA0)) || (cmd == (DEI_SC_BUTTON_STOP + 0xA0)))
		{
			onebutton_cmd_arrive = TRUE;
		}
		else
			onebutton_cmd_arrive = FALSE;

		dei_sc_pagging_rewind( DEI_SC_TIMEOUT_PAGGING );
		
    btn_pld.bits.id      = hdr & 0x03;
    btn_pld.bits.batt_low = ((hdr & 0x04) >> 2);

    if( dei_sc_mode == DEI_SC_MODE_SENSOR_ADJUST )
    {
			btn_pld.bits.pld     = pld;

			dei_sc_cmd_sensor_adjust_mode( cmd , &btn_pld);
    }
		else
    {
//			if( (cmd == (DEI_SC_BUTTON_SILENT_LOCK + 0xA0)) || (cmd == (DEI_SC_BUTTON_SILENT_UNLOCK + 0xA0)) || ( cmd == ( DEI_SC_BUTTON_LOCK + 0xA0)) || (cmd == ( DEI_SC_BUTTON_UNLOCK + 0xA0)))
//			{
				// store the hhu number for 
				dei_sc_set_hhu_num( dei_sc_current_sn );
//			}
			//build the button 
			btn_pld.bits.pld     = pld;
																							
			#define BUTTON_CMD  (press_type ? (Cmd)(((cmd -0xA0) + INPUT_RF_ENTRY + 1) | INPUT_RF_EXT) :  (cmd -0xA0) + INPUT_RF_ENTRY + 1)
			
			if(
				(dei_sc_mode == DEI_SC_MODE_NORMAL)
			  &&( (cmd == DEI_SC_BUTTON_LOCK + 0xA0 && dei_system_prg_mode_get() == FALSE)
				  || cmd == DEI_SC_BUTTON_SILENT_LOCK + 0xA0
				  || cmd == DEI_SC_BUTTON_SENSOR_SILENT_ARM + 0xA0
				  || cmd == DEI_SC_BUTTON_FULL_SILENT_ARM + 0xA0
			    )
			  &&(
					!(get_ss_panic() || get_ss_trigger()) // Car is screaming
					&& !get_rs_mts_progress()  // Ready mode started
					&& get_ign_status()!=0 && get_rs_gwr()==0 // Ignition on and we didn't do it! (driving)
					)
			  )
			  {
          //notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_FEATURE_NA ); 
				  if(!press_type) 
						return; // Block all commands except unlock and start when user ignition is on
				}
			
			cmd_q_rs_insert( BUTTON_CMD , 100 , btn_pld.pld );
			
			#undef BUTTON_CMD       

			if(!press_type && is_aux_command( last_btn_cmd))   //press_type will have DEI_SC_BUTTON_XPRESS_RELEASED later
			{
					dei_sc_cmd_release_tmr_rewind(DEI_SC_CMD_RELEASE_TIMEOUT);
			}
			else
					dei_sc_cmd_release_tmr_stop();
    }

}

//--------------------------------------------------------------------------//

void dei_sc_sensor_adjust_mode_timeout(void)
{
	UInt8 tx_hhu_no;
	UInt32 tx_pld;

 	UInt8 pld[2] = {0,0};		// intialized for Testmode and alarm bit set sensor device
	
	pld[0] = get_shock_sensor_alert_bits();

	sensor_proto_tx(PLD_E1, dei_ss_sensor_curr_aid(), dei_ss_sensor_curr_chanl() , pld, 2 );
	sensors_status_invalidate();

	dei_sc_mode = DEI_SC_MODE_NORMAL;
	dei_led_flash(DEI_LED_RESET, TRUE);
	vrtc_stop(dei_sc_sensor_adjust_mode_vrtc);
	
	dei_siren_chirp( DEI_SIREN_SENSOR_PROGRAMMING_EXIT ); 				// to give long siren pulse for exit Sensor Adjust mode

	notify_send(NOTIFY_SENSOR_PROG_MODE_EXIT);
	
	wake_leave_flagged(WAKE_SCPP_CMD);		// sensor adjust mode exit

}

//--------------------------------------------------------------------------//
Per_Exec_Wake dei_sc_cmd_release_tmr_update(void)
{
    if(dei_sc_cmd_release_tmr_cnt == 0 || --dei_sc_cmd_release_tmr_cnt != 0)
        return PER_EXEC_SLEEP;

    //btn released
    cmd_q_rs_insert( (Cmd)(((last_btn_cmd - 0xA0) + INPUT_RF_ENTRY + 1)|INPUT_RF_RELEASE) , 100);
		return PER_EXEC_WAKE;
}

//--------------------------------------------------------------------------//
void dei_sc_cmd_release_tmr_rewind(UInt16 delta)
{
    #define RELEASE_HZ    100
    dei_sc_cmd_release_tmr_cnt = (delta * RELEASE_HZ + 999)/1000;
}

//--------------------------------------------------------------------------//
void dei_sc_cmd_release_tmr_stop(void)
{
    dei_sc_cmd_release_tmr_cnt = 0;
}

//--------------------------------------------------------------------------//

void dei_sc_cmd_sensor_adjust_mode(UInt8 cmd, Cmd_Rf_Pld *btn_pld)
{
	UInt8 pld_byte;
	
	if( dei_sc_current_sn == dei_sc_sensor_adjust_hhu )
	{
		if( cmd != (DEI_SC_BUTTON_EXIT_PROGRAMMING_MODE + 0xA0) )
		{
			vrtc_stop( dei_sc_sensor_adjust_mode_vrtc );
			vrtc_update(  dei_sc_sensor_adjust_mode_vrtc, DEI_RS_VRTC_TIMEOUT_SENSOR_ADJUST_MODE    );
			vrtc_start( dei_sc_sensor_adjust_mode_vrtc );
		}

		if(cmd == (DEI_SC_BUTTON_LOCK + 0xA0))
		{
			dei_ss_sensor_adjust_src = DEI_SS_LED_REMOTE_INCREASE;
			pld_byte = get_shock_sensor_level();

			if( pld_byte < 0x0F)
			{
				pld_byte += 1;
				set_shock_sensor_level( pld_byte );
			}

		}
		else if(cmd == (DEI_SC_BUTTON_UNLOCK + 0xA0))
		{
			dei_ss_sensor_adjust_src = DEI_SS_LED_REMOTE_DECREASE;
			pld_byte = get_shock_sensor_level();
			if( pld_byte > 0 )
			{
				pld_byte -= 1;
				set_shock_sensor_level( pld_byte );
			}
		}
		
		if( (cmd == (DEI_SC_BUTTON_LOCK + 0xA0)) || (cmd == (DEI_SC_BUTTON_UNLOCK + 0xA0)) || (cmd == (DEI_SC_BUTTON_START_STOP_TOOLGE+ 0xA0)) )
		{
			if(cmd == (DEI_SC_BUTTON_START_STOP_TOOLGE + 0xA0))
			{
				dei_ss_sensor_adjust_src = DEI_SS_LED_REMOTE_DEFAULT;
			}

			btn_pld->bits.pld     = pld_byte;
			cmd_q_rs_insert( INPUT_RF_PUT_NEW_SENSOR1_VALUE , 100 , btn_pld->pld );
		}
		else
		{
			//build the button 
			#define BUTTON_CMD  (cmd -0xA0) + INPUT_RF_ENTRY + 1

			dei_ss_sensor_adjust_src = DEI_SS_LCD_REMOTE;
			cmd_q_rs_insert( BUTTON_CMD , 100 , btn_pld->pld );

			#undef BUTTON_CMD       
		}
	}
}


//--------------------------------------------------------------------------//
static Boolean is_aux_command(UInt8 cmd)
{
	Boolean ret_val;
	
	ret_val = FALSE;
	
	if(  cmd == 0xA5 || cmd == 0xAC || cmd == 0xAD
    || cmd == 0xAE || cmd == 0xAF)
		ret_val = TRUE;
	
	return ret_val;
}
