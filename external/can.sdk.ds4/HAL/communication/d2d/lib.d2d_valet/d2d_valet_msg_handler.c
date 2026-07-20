/*! 
 * @file   d2d1_msg_handler.c
 * @brief  
 * @author  DIRECTED ELECTRONICS
 * @date   8/13/2015
 */

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2d_valet_private.h"
#include "intl_port.h"  //d2d_cmd_enabled
#include "dei_led.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define LED_OFF 		1
#define LED_ON			0

// truth table for led pattern with inversion bit and button press
                            //================================//
UInt8 led_table[8]= {				//  pattern 		inver			buttn  	//
	LED_OFF,									//		0						0					0			//
	LED_OFF,									//		0						0					1			//	
	LED_OFF,									//		0						1					0			//
	LED_ON,										//		0						1					1			//
	LED_ON,										//		1						0					0			//
	LED_ON,										//		1						0					1			//
	LED_ON,										//		1						1					0			//
	LED_OFF,									//		1						1					1			//
};													//================================//	

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void butn_press(D2d_Valet_Port *port);
void butn_release(D2d_Valet_Port *port);
void send_switch_pattern( D2d_Valet_Port *port, UInt8 tap_cnt, UInt8 hold_status);
void butn_timeout(D2d_Valet_Port *port);
void update_led(D2d_Valet_Port *port, UInt8 led_state);
static void idrive_led(void);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
UInt8 led_patt_byte[4];    //maximum multiple LED pattern sent from DS4
UInt8 led_patt_byte_idx=0;
UInt8 led_patt_len=0;
UInt8 led_multi_patt_running=0;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void alloc_d2d_valet_port_rxmsg(D2d_Valet_Port *port)
{
    if ((port->rxmsg = smsg_alloc(port->smsgbase.number)) != 0)
        port->rxbuf = smsg_getptr(port->rxmsg);
    else
        port->rxbuf = NULL; // no msg means no buffer pointer
}

void check_valet_switch_state(void)
{
    if(valet_switch_unstable == 0)
    {
        valet_switch_unstable = 1;
        valet_switch_tstamp_pre = T1TC;
    }
}

void butn_press(D2d_Valet_Port *port)
{
    d2d_valet_butn_state = BUTN_DEBOUNCE;
    port_timer_rewind(port,PORT_TIMER_MS(100));
}

void butn_release(D2d_Valet_Port *port)
{
UInt8 old;

    old = d2d_valet_butn_state;
	d2d_valet_butn_state = BUTN_RELEASED; // new state will be this.

  if(old != BUTN_PRESSED)
	{
	  if(d2d_valet_butn_tap_cnt)
	    port_timer_rewind(port,PORT_TIMER_MS(1450)); // still have pending taps?
	}
    port_timer_rewind(port,PORT_TIMER_MS(1450));
	  d2d_valet_led_table_index &= ~(1u<<0);		// button bit update
		update_led(port, d2d_valet_led_table_index);

}


void send_switch_pattern(D2d_Valet_Port *port, UInt8 tap_cnt, UInt8 hold_status)
{
  #define VALET_SWITCH_MS_SCALE  (72*1000)
  UInt32 btn_tstamp_sec;
	UInt8 butn_event;

  btn_tstamp_sec = valet_switch_tstamp /VALET_SWITCH_MS_SCALE;
  btn_tstamp_sec <<=8;
  btn_tstamp_sec /= 1000; 
	
	alloc_d2d_valet_port_rxmsg(port);
	
 	butn_event = tap_cnt<<1;
    if(hold_status)
       butn_event |= 1;

		port->rxbuf[0] = (0xFE - port->smsgbase.number);
		port->rxbuf[1] = PROTO_SCPP;
		port->rxbuf[2] = 0x0;      //no traffic/broadcast/RSVP
		port->rxbuf[3] = 0xEA;
		port->rxbuf[4] = butn_event;
    port->rxbuf[5] = btn_tstamp_sec >> 8;
    port->rxbuf[6] = btn_tstamp_sec;

    // ask for buffer to do submit
		port->rxlen = (5+2);
    smsg_setlen(port->rxmsg, port->rxlen);

    smsg_submit(port->rxmsg);
		port->smsgbase.pre_wake = FALSE;
		wake_leave_flagged(WAKE_D2D_VALET);
}


void d2d_valet_reset_request(void)
{
D2d_Valet_Port *port = d2d_valet_port;

	
   alloc_d2d_valet_port_rxmsg(port);
	
		port->rxbuf[0] = (0xFE - port->smsgbase.number);
		port->rxbuf[1] = PROTO_INTL;
		port->rxbuf[2] = 0xEA;      // request reset

    // ask for buffer to do submit
		port->rxlen = (3);
    smsg_setlen(port->rxmsg, port->rxlen);

    smsg_submit(port->rxmsg);
}


void butn_timeout(D2d_Valet_Port *port)
{
    if(d2d_valet_butn_state == BUTN_DEBOUNCE)
    {
      d2d_valet_butn_tap_cnt++;
      d2d_valet_butn_state = BUTN_PRESSED; // ok, its really pressed now.

	    d2d_valet_debounce_state = DEBOUNCE_TIMEOUT;
      port_timer_rewind(port,PORT_TIMER_MS(1400)); // BUTN_TIME_HOLD - BUTN_TIME_DEBOUNCE wait to decide if this is a hold or not.
			d2d_valet_led_table_index |= (1u<<0);		// button bit update
			update_led(port, d2d_valet_led_table_index);
    }
    else // BUTN_PRESSED or BUTN_RELEASED
    {
	  if(d2d_valet_butn_state == BUTN_PRESSED)
	    d2d_valet_butn_tap_cnt--;  // don't count lastmost press

      valet_switch_tstamp = T1TC - valet_switch_tstamp_pre;
      send_switch_pattern( port, d2d_valet_butn_tap_cnt, (d2d_valet_butn_state - BUTN_RELEASED)); // =0 if released, else non-zero
			d2d_valet_butn_tap_cnt = 0;
      valet_switch_unstable = 0;
    }
}

void update_led(D2d_Valet_Port *port, UInt8 led_state)
{
			gpio_pl_pin_write(d2d_valet_port->led_pin_label, led_table[led_state]);
}


void d2d_led_msg_timeout_handler( Intl_Port *port)
{
	switch( d2d_valet_led_state )
	{
		case LED_STAT_ON:
		{
			if(led_patt_byte_idx > 0 && led_patt_byte_idx < led_patt_len
				  && !led_multi_patt_running)
			{
				led_multi_patt_running = 1;
				idrive_led();
				return;
			}
			else if(led_patt_byte_idx == led_patt_len)
			{
				led_patt_byte_idx = 0;
				led_multi_patt_running = 0;
				idrive_led();
				return;
			}
			
			d2d_valet_led_table_index &= ~(1u<<2);
			d2d_valet_led_state = LED_STAT_OFF;
			update_led(d2d_valet_port, d2d_valet_led_table_index);

			port_timer_rewind(port,PORT_TIMER_MS(_d2d_valet_pulse_time));
		}
		break;
		case LED_STAT_OFF:
		{
			d2d_valet_led_flash_cnt++;
			d2d_valet_led_table_index |= (1u<<2);
			d2d_valet_led_state = LED_STAT_ON;
			update_led(d2d_valet_port,d2d_valet_led_table_index);
			
			if((LED_NUM_FLASH(d2d_valet_led_pattern) > d2d_valet_led_flash_cnt) && (LED_NUM_FLASH(d2d_valet_led_pattern) != LED_FLASH_FOREVER))
			{
				port_timer_rewind(port,PORT_TIMER_MS(_d2d_valet_pulse_time));
			}
			else
			{
				d2d_valet_led_flash_cnt = 0;
				port_timer_rewind(port,PORT_TIMER_MS(d2d_valet_offset_time + _d2d_valet_pulse_time));
				led_patt_byte_idx++;
				led_multi_patt_running = 0;
			}
		}
		break;
		default:
		break;
	}

}
/**
* @brief  called by task
* @param
* @return
*/
void d2d_valet_msg_timeout_handler(D2d_Valet_Port *port)
{
  if(  d2d_valet_debounce_state == DEBOUNCE_PRESS
    || d2d_valet_debounce_state == DEBOUNCE_RELEASE)	
    check_valet_switch_state();
  
	switch(d2d_valet_debounce_state)
	{
		case DEBOUNCE_PRESS:
		case DEBOUNCE_RELEASE:      
		{
			if(!port->smsgbase.pre_wake)
			{
				port->smsgbase.pre_wake = TRUE;
					wake_enter_flagged(WAKE_D2D_VALET);
			}

			d2d_valet_debounce_state = DEBOUNCE_TIMEOUT;
      if(gpio_pl_pin_read(port->butn_pin_label))
      {// DEBOUNCE_RELEASE
        			butn_release(port);
      }
      else  
      {  //DEBOUNCE_PRESS
			  d2d_valet_butn_state = BUTN_DEBOUNCE;
			  butn_timeout(port);
			  //butn_press(port);
      }
		}
		break;
		case DEBOUNCE_TIMEOUT:
		{
			d2d_valet_debounce_state = DEBOUNCE_NONE;
			butn_timeout(port);
		}
		break;
		default:
		break;
	}
	
}


Boolean d2d_valet_filter(D2d_Valet_Port *port,Smsg msg)
{
  		UInt8 *ptr;
      ptr = smsg_getptr(msg);
  
      return (ptr[0] == 0xFE && ptr[1] == (PROTO_SCPP|PROTO_AUTH) && ptr[3] == 0xD4);
}


static void idrive_led(void)
{
	    d2d_valet_led_pattern = led_patt_byte[led_patt_byte_idx];
	
			if((d2d_valet_led_pattern == 0) || (d2d_valet_led_pattern == 0xFF))
			{
				if(d2d_valet_led_pattern == 0)
				{
					d2d_valet_led_table_index |= (1u<<2);		// LED OFF for LED bit position

					d2d_valet_led_state = LED_STAT_OFF;
				}
				else if (d2d_valet_led_pattern == 0xff)
				{
					d2d_valet_led_table_index &= ~(1u<<2);		// LED ON for LED bit position
					d2d_valet_led_state = LED_STAT_ON;
				}
				update_led(d2d_valet_port, d2d_valet_led_table_index);
				d2d_valet_led_state = LED_STAT_NONE;
			}
			else
			{
				d2d_valet_led_state = LED_STAT_ON;
				
				if( LED_OFFSET(d2d_valet_led_pattern) == LONG_OFFSET)
				{
					d2d_valet_offset_time = 1500;
				}
				else if(LED_OFFSET(d2d_valet_led_pattern) == MED_OFFSET)
				{
					d2d_valet_offset_time = 1000;
				}
				else if(LED_OFFSET(d2d_valet_led_pattern) == SHORT_OFFSET)
				{
					d2d_valet_offset_time = 700;
				}
				else if(LED_OFFSET(d2d_valet_led_pattern) == SPEC_OFFSET)
				{
					if(LED_PULSE(d2d_valet_led_pattern) == LONG_PULSE)
						d2d_valet_offset_time = 500;
					else
						d2d_valet_offset_time = 0;
				}

				_d2d_valet_pulse_time = 300;
				
				d2d_led_msg_timeout_handler(&intl_port);
			}
}


/**
* @brief  send msg to hardware
* @param
* @return
*/

void d2d_valet_txmsg(D2d_Valet_Port *port,Smsg msg)
{
		UInt8 *ptr;
    UInt8 msg_id = 0;
	  UInt8 len, i;
    ptr = smsg_getptr(msg);
    len = smsg_getlen(msg);
	  if(ptr[1] & PROTO_AUTH)
      len -= 4;   //excluding auth data (AUTH_SIZE: 4)
	
    if(d2d_valet_filter(port, msg))
    {
				msg_id = ptr[3];
    }
    
    smsg_free(msg);
		
	// start LED flash if it is a D4 message
		if( msg_id == 0xD4 && len >= 7 && len < 10) //valid msg has 7~9 bytes)
		{
			 d2d_valet_led_flash_cnt = 1;		// starting the first flash
			
			 led_patt_len = 0;
			 led_multi_patt_running = 0;
       for(i = 5; i < len; i++)
       {
				 if(ptr[i])
					 led_patt_byte[led_patt_len++] = ptr[i];
			 }
			 
			 if(led_patt_len == 0)  //LED off
				 led_patt_byte[led_patt_len++] = ptr[5];
			 
			 led_patt_byte_idx = 0; //start LED pattern

 	     d2d_valet_led_pattern = led_patt_byte[led_patt_byte_idx];
	
			 if(d2d_valet_led_pattern)  //D4 LED flashing
			 {
				 if(valet_led_in_use == VALET_LED_IN_USE_ENABLED
				 	 || valet_led_in_use == VALET_LED_IN_USE_INACTIVE)
				 {
					 valet_led_in_use = VALET_LED_IN_USE_ACTIVE;
				   wake_enter_flagged(WAKE_D2D_VALET);
				 }
			 }
			 else   //D4 LED off
			 {
				 if(valet_led_in_use == VALET_LED_IN_USE_ACTIVE)
				 {
					 valet_led_in_use = VALET_LED_IN_USE_INACTIVE;
				   wake_leave_flagged(WAKE_D2D_VALET);
				 }
			 }
			 
       idrive_led();
		}
}

