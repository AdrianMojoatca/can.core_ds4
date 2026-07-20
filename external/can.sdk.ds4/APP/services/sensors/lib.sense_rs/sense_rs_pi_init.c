/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs_pi_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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
Boolean sense_rs_input_glowplug_mapped (UInt8 i);
Boolean sense_rs_input_safety_sw_mapped(UInt8 i);
Boolean sense_rs_input_valet_switch_mapped(UInt8 i);
void sense_rs_set_input_source( UInt8 i );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/
#ifdef PLATFORM_CM800

void sense_rs_pi_init( void )
{
    UInt8 i  ;

    Boolean input_glowplug  = FALSE ;
    Boolean input_safety_sw = FALSE ;

    os_tsk_wait(100);
    
    for( i = 0 ; i < SENSE_RS_IN_COUNT; i++ )
    {
        Sense_Rs_Input *input   = (Sense_Rs_Input*)os_mem_request(sizeof *input);        
        
        Sense_Rs_Input_Cfg *cfg = sense_rs_pi_cfg(i);

        input->ied = ied_create(cfg->time_0_to_1 , cfg->time_1_to_0 , sense_rs_pi_status(i) );
        input->selector = i ;

        sense_rs_pi_executor(i,TRUE,sense_rs_pi_status(i));
        
        per_exec_register((Per_Exec_Func)sense_rs_pi_loop , (Per_Exec_Arg)input );

        if(sense_rs_input_glowplug_mapped(i))
        {
            input_glowplug = TRUE ;
        }

        if(sense_rs_input_safety_sw_mapped(i))
        {
            input_safety_sw = TRUE ;
        }
    }

    if( !input_glowplug )
    {
        //No wire mapped to glowplug , force the glowplug light status to OFF
        //otherwise the car will not start in Diesel Wait to Start  mode
        set_glowplug_off_wire_status( TRUE ) ;
    }

    if( !input_safety_sw )
    {
        //No wire mapped to safety sw , force the safety switch to SAFETY_RS_ENABLE
        //otherwise the car will not start
        
        set_safety_sw_status( SAFETY_RS_ENABLE );
    }
}


//--------------------------------------------------------------------------//

Boolean sense_rs_input_glowplug_mapped(UInt8 i)
{    
    return (sense_rs_input_config->inputs[i] == SENSE_RS_IN_FUNCTION_GLOWPLUG);    
}

//--------------------------------------------------------------------------//

Boolean sense_rs_input_safety_sw_mapped(UInt8 i)
{
    return (sense_rs_input_config->inputs[i] == SENSE_RS_IN_FUNCTION_SAFETY_SW);                
}

//--------------------------------------------------------------------------//

#else
	
Boolean ext_sensor_trigger_configured = FALSE;

void sense_rs_pi_init( void )
{
  UInt8 i  ;
	
	ext_sensor_trigger_configured = FALSE;
  os_tsk_wait(100);
	
	sense_rs_door_defined = 0;
    
    for( i = 0 ; i < SENSE_RS_IN_COUNT; i++ )
    {
        if(!sense_rs_input_valet_switch_mapped(i))
        {
					Sense_Rs_Input *input   = (Sense_Rs_Input*)os_mem_request(sizeof *input);        
					
					Sense_Rs_Input_Cfg *cfg = sense_rs_pi_cfg((Sense_Rs_Input_Selector)i);

					input->ied = ied_create(cfg->time_0_to_1 , cfg->time_1_to_0 , sense_rs_pi_status((Sense_Rs_Input_Selector)i) );
					input->selector = (Sense_Rs_Input_Selector)i ;
					
					sense_rs_set_input_source(i);   // set before pi_executor in which needs input source
					sense_rs_pi_executor((Sense_Rs_Input_Selector)i,TRUE,sense_rs_pi_status((Sense_Rs_Input_Selector)i));
					
					per_exec_register((Per_Exec_Func)sense_rs_pi_loop , (Per_Exec_Arg)input );
				}
				
				if(sense_rs_input_config->inputs[i] == SENSE_RS_IN_FUNCTION_EXT_SENSOR_TRIGGER)
					ext_sensor_trigger_configured = TRUE;
    }
		sense_ext_trigger_timer = timeout_f_create(SENSE_EXT_PRESS_TIMEOUT, (Exec_Func)sense_ext_trigger_check_timeout);
		
		TRACE_DEI_SENSE_RS("defined doors: %d ", sense_rs_door_defined);
}


//--------------------------------------------------------------------------//

Boolean sense_rs_input_valet_switch_mapped(UInt8 i)
{    
    return (sense_rs_input_config->inputs[i] == SENSE_RS_IN_FUNCTION_VALET_SW);    
}

//--------------------------------------------------------------------------//

void sense_rs_set_input_source( UInt8 i )
{
	switch( sense_rs_input_config->inputs[i] )
	{
		case SENSE_RS_IN_FUNCTION_DOOR: 
		{
			if(get_door_source() == SOURCE_NONE)
				set_door_source (SOURCE_WIRE);
			else
				set_door_source(SOURCE_AUTO);
		}
		break;
		case SENSE_RS_IN_FUNCTION_IN_HOOD:
		{
			if(get_hood_source() == SOURCE_NONE)
				set_hood_source (SOURCE_WIRE);
			else
				set_hood_source(SOURCE_AUTO);
		}
		break;
		case SENSE_RS_IN_FUNCTION_HANDBRAKE:
		{
			if(get_handbrake_source() == SOURCE_NONE)
				set_handbrake_source (SOURCE_WIRE);
			else
				set_handbrake_source(SOURCE_AUTO);
		}
		break;
		case SENSE_RS_IN_FUNCTION_HEADLIGHT_DETECT:
		{
			if(get_headlight_source() == SOURCE_NONE)
				set_headlight_source (SOURCE_WIRE);
			else
				set_headlight_source(SOURCE_AUTO);
		}
		break;
		case SENSE_RS_IN_FUNCTION_IN_TRUNK:
		{
			if(get_trunk_source() == SOURCE_NONE)
				set_trunk_source (SOURCE_WIRE);
			else
				set_trunk_source(SOURCE_AUTO);
		}
		break;
		case SENSE_RS_IN_FUNCTION_IGNITION:
		{
			if(get_ign_source() == SOURCE_NONE)
				set_ign_source (SOURCE_WIRE);
			else
				set_ign_source(SOURCE_AUTO);
		}
		break;
		case SENSE_RS_IN_FUNCTION_BRAKE:
		{
			if(get_brake_source() == SOURCE_NONE)
				set_brake_source (SOURCE_WIRE);
			else
				set_brake_source(SOURCE_AUTO);
		}
		break;
	}
}
#endif

