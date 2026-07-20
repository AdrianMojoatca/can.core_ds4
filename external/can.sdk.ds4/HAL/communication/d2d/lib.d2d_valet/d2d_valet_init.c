/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d1_init.c 32240 2015-06-12 12:31:31Z simon.demers $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "d2d_valet_private.h"
#include <string.h>
#include "fiq.h"
#include "ngmm_feature.h"


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

void d2d_valet_init (void)
{
  D2d_Valet_Port *port;
	UInt8 i;
  
	Gpio_Pl_T btn_pin, led_pin;
	static const Gpio_Pl_T valet_butn_pin_label[] = 
	{
		GPIO_PL_IN_NEG1 , GPIO_PL_IN_NEG3 , GPIO_PL_IN_NEG4  , GPIO_PL_IN_NEG5 , GPIO_PL_IN_NEG7, GPIO_PL_IN_POS1, GPIO_PL_IN_POS2, GPIO_PL_IN_POS3, GPIO_PL_IN_DOOR_POS1
	};

	static const Gpio_Pl_T led_out_pin_label[] = 
	{
		GPIO_PL_NEG_OUT1 , GPIO_PL_NEG_OUT2 , GPIO_PL_NEG_OUT3  , GPIO_PL_NEG_OUT4 , GPIO_PL_NEG_OUT5, GPIO_PL_POS_OUT6, GPIO_PL_POS_OUT7, GPIO_PL_NEG_OUT8, GPIO_PL_NEG_OUT9,\
		GPIO_PL_NEG_OUT10, GPIO_PL_NEG_OUT11, GPIO_PL_NEG_OUT12, GPIO_PL_NEG_OUT13, GPIO_PL_OUT_NEG14, GPIO_PL_RLY1_CTL, GPIO_PL_RLY2_CTL, GPIO_PL_RLY3_CTL
	};
	
	static Boolean do_once = TRUE;

  port = (D2d_Valet_Port *) smsg_port[VALET_PORT];  
  
	btn_pin = 0;
	led_pin = 0;
	
		for( i = 0 ; i < DEI_PROG_IN_COUNT; i++ )
    {
        if(dei_feature_get(DEI_FEAT_INPUT_BASE(i)) == DEI_IN_PRG_VALET_SWITCH)
        {
					btn_pin = valet_butn_pin_label[i];
					break;
        }
    }
		
		
		for( i = 0 ; i < DEI_PROG_OUT_COUNT ; i ++ )
		{
			if( dei_feature_get(DEI_FEAT_OUTPUT_BASE(i)) == DEI_OUT_PRG_LED_FLASH ) //feature is enable
			{
				led_pin = led_out_pin_label[i];
				valet_led_in_use = VALET_LED_IN_USE_ENABLED;
				break;
			}
		}


	if (do_once && (btn_pin || led_pin))
	{
			// init Port Pins
			d2d_valet_butn_state = BUTN_RELEASED;
			d2d_valet_debounce_state = DEBOUNCE_NONE;
			
      if(btn_pin)
      {
        port->butn_pin_label = btn_pin;
        gpio_pl_init(port->butn_pin_label);
        fiq_register( d2d_valet_butn_p4_service);
        // enable interrupt for receive pin
        gpio_pl_falling_irq_ena( port->butn_pin_label );
        gpio_pl_rising_irq_ena( port->butn_pin_label );
        gpio_pl_int_clr(port->butn_pin_label);
      }
      
      if(led_pin)
      {
        port->led_pin_label = led_pin;
        gpio_pl_init(port->led_pin_label);
      }

			wake_register      (d2d_valet_wake);
		  do_once = FALSE;
		}
}


void d2d_valet_pre_init(void)
{
			//initialize swmsg q of d2d_valet port
			d2d_valet_port = os_mem_request(sizeof(D2d_Valet_Port));
			memset(d2d_valet_port, 0, sizeof(D2d_Valet_Port));

		   d2d_valet_port->smsgbase.filterfunc = (BP2A *)d2d_valet_filter;
			d2d_valet_port->smsgbase.sendfunc = (VP2A *)d2d_valet_txmsg;
			d2d_valet_port->smsgbase.number = smsg_register(VALET_PORT, d2d_valet_port);
			d2d_valet_port->smsgbase.timeoutfunc = (VP1A *)d2d_valet_msg_timeout_handler;
						
			d2d_valet_port_no = VALET_PORT;  //4: valet port
			d2d_valet_port->smsgbase.pre_wake = TRUE;
}
