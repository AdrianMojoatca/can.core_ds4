/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs_pi_executor.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs_private.h"
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

void sense_rs_pi_executor( Sense_Rs_Input_Selector selector , Boolean do_init , Boolean status )
{    
#ifdef PLATFORM_CM800
    static const Sense_Rs_Input_Change_Executor in_change_executor[SENSE_RS_IN_FUNCTION_COUNT] = 
    {
        sense_rs_handbrake    ,  
        sense_rs_safety_sw    ,
        sense_rs_hood         ,
        sense_rs_trunk        ,
        sense_rs_door         ,
        sense_rs_brake        ,
        sense_rs_glowplug     ,
        sense_rs_ign          ,
        sense_rs_start_trigger,
    
        sense_rs_custom_neg1  ,
        sense_rs_custom_neg2  ,
        sense_rs_custom_neg3  ,
        sense_rs_custom_pos1  ,
        sense_rs_custom_pos2    
    };
#else

    static const Sense_Rs_Input_Change_Executor in_change_executor[SENSE_RS_IN_FUNCTION_COUNT] = 
    {
        sense_rs_not_configured   ,			
        sense_rs_start_trigger    ,
        sense_rs_brake            ,
        sense_rs_door             ,
        sense_rs_hood             ,
        sense_rs_inst_trig        ,
        sense_rs_handbrake        ,
        sense_rs_headlight_detect ,
        sense_rs_trunk            , 
        sense_rs_valet_switch     ,
        sense_rs_ign              ,
        sense_rs_custom_pos1      ,
        sense_rs_ext_sensor_trigger,
	      sense_rs_tamper           ,
			  sense_rs_ptim_stop        ,
			  sense_rs_ext_open         ,
			  sense_rs_ext_close        ,
				sense_rs_ext_trunk        ,
    };

#endif
    
    UInt8 function = sense_rs_input_config->inputs[selector] ; 
    
    if( function < SENSE_RS_IN_FUNCTION_COUNT )
    {                
        #if USE_TRACE == 1
        {
#ifdef PLATFORM_CM800
            const char *str[] = 
            {
                "[IN_NEG1_%s ]\n\r",
                "[IN_NEG2_%s ]\n\r",
                "[IN_NEG3_%s ]\n\r",
                "[IN_NEG4_%s ]\n\r",
                "[IN_NEG5_%s ]\n\r",
                "[IN_NEG6_%s ]\n\r",
                "[IN_NEG7_%s ]\n\r",
                "[IN_POS1_%s ]\n\r",
                "[IN_POS2_%s ]\n\r",
                "[IN_POS3_%s ]\n\r",
                "[IN_NP1_%s ]\n\r"
            };

						TRACE_DEI_SENSE_RS(str[selector] , status  ? "HIGH":"LOW" );
#else
            const char *str[] = 
            {
                "[IN_NEG1_%s ]\n\r",
                "[IN_NEG3_%s ]\n\r",
                "[IN_NEG4_%s ]\n\r",
                "[IN_NEG5_%s ]\n\r",
                "[IN_NEG7_%s ]\n\r",
                "[IN_POS1_%s ]\n\r",
                "[IN_POS2_%s ]\n\r",
                "[IN_POS3_%s ]\n\r",
                "[IN_NP1_%s ]\n\r"
            };
						
						if( function != SENSE_RS_IN_FUNCTION_NOT_CONFIGURED)
						{
							TRACE_DEI_SENSE_RS("SRC: %d FUNC: %d ", selector, function);
						  TRACE_DEI_SENSE_RS(str[selector] , status  ? "HIGH":"LOW" );
						}
						else
						{
							TRACE_DEI_SENSE_RS("[NOT CONFIGURED ] \n\r" );
						}

#endif
            
            if( do_init )
            {
                os_tsk_wait(10);
							
							// check the defined doors during initialization, debug info only
							if(function == (SENSE_RS_IN_FUNCTION_DOOR -1))  // door
							{
								sense_rs_door_defined++;
							}
            }
        }
        #endif
				
				status = !status;   // correct to 1: ON, 0: OFF because all OFF are HIGH by definition of gpio_pl.
				if( (function == (SENSE_RS_IN_FUNCTION_DOOR) && get_door_close_type() == DOOR_NORMALLY_CLOSE)
          || (function == (SENSE_RS_IN_FUNCTION_IN_HOOD) && get_hood_close_type() == HOOD_NORMALLY_CLOSE)
			    || (function == (SENSE_RS_IN_FUNCTION_IN_TRUNK) && get_trunk_close_type() == TRUNK_NORMALLY_CLOSE)  // check CLOSE TYPE of wired door, hood, trunk
				  || (function == (SENSE_RS_IN_FUNCTION_TAMPER))
			  )
				  status = !status;
				
				(in_change_executor[function])(do_init , status, selector); 
    }         
}


/*--------------------------------------------------------------------------*/

