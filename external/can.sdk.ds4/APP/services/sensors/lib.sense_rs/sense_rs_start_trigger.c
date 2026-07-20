/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs_start_trigger.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs_private.h"
#if CM800_DEI_BRAND == 1
#include "dei_feature.h"
#endif

#if CM800_AUTOSTART_BRAND == 1
#include "as_feature.h"
#endif
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
Boolean sense_rs_idle_turbo_enabled( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

void sense_rs_start_trigger( Boolean do_init  , Boolean status, UInt8 from)
{	
	static Boolean old_status = FALSE;

    if( do_init )    //STATUS = LOW  : handbrake active
                     //STATUS = HIGH : handbrake not active
    {
        ATOMIC( old_status = status );
    }
    else
    {    
        trigger_change_detect:
    	{				
    		if( old_status != status )
    		{
    			if( status == FALSE )
                {                
                    if( get_ign_status() && !get_rs_gwr() )
	                {		                                        
                        if( sense_rs_input_config->start_input_config==SENSE_RS_START_INPUT_MAPPED_TO_TURBO)
                        {
                            cmd_q_rs_insert( INPUT_RF_TIMER_MODE_TOOGLE , TIMEOUT_Q_INSERT );
                        }
                        else
                        {                            
                            cmd_q_rs_insert( INPUT_RF_START_STOP_TOOGLE , TIMEOUT_Q_INSERT );
                        }
                    }
                    else
                    {
                      #if	CM800_AUTOSTART_BRAND == 1										
                        if(as_feature_get(AS_FEAT_MODE4_FUNC2_EXTERNAL_TRIG) == AS_OPTION42_EXTERNAL_TRIG_ENABLE_SINGLE_PUSLE )
						  {
						    cmd_q_rs_insert( INPUT_RF_START_STOP_TOOGLE , TIMEOUT_Q_INSERT );
						  }							  													
                      #elif CM800_DEI_BRAND == 1																		
                        cmd_q_rs_insert( INPUT_RF_START_STOP_TOOGLE , TIMEOUT_Q_INSERT );
                      #endif												
							        sense_rs_trig_cmd_arrive = TRUE;
                    }    
                }
    
    			TRACE_DEI_SENSE_RS("\n\r[TRIGGER_RS_WIRE_%s]\n\r" , status ? "ON":"OFF" );				
    		    
                ATOMIC( old_status = status );
            }            
    	}
    }
}

/*--------------------------------------------------------------------------*/

Boolean sense_rs_idle_turbo_enabled( void )
{
    /*Boolean ret = FALSE;
    
    #if CM800_DEI_BRAND == 1
        ret = dei_feature_get(DEI_FEAT_TURBO_MODE_RUNTIME);
    #endif

    #if AS_BRAND == 1
        if(get_gearbox_type()==GEARBOX_AUTOMATIC)
        {
            ret = (as_feature_get(AS_FEAT_MODE2_FUNC4_IDLE_AND_TURBO_MODE)==AS_OPTION24_AUTO_TURBO_ENABLED_MANUAL_TURBO_DISABLED)||
                  (as_feature_get(AS_FEAT_MODE2_FUNC4_IDLE_AND_TURBO_MODE)==AS_OPTION24_AUTO_TURBO_ENABLED_MANUAL_TURBO_ENABLED );
        }
        else
        {
            ret = (as_feature_get(AS_FEAT_MODE2_FUNC4_IDLE_AND_TURBO_MODE)==AS_OPTION24_AUTO_TURBO_ENABLED_MANUAL_TURBO_ENABLED );                  
        }            
             
    #endif
    
    return ret;*/

    return TRUE;
}

/*--------------------------------------------------------------------------*/

    
