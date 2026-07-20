/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_mts_smart_timer_disable.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rs_private.h"
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

void dei_rs_mts_smart_timer_disable( void )
{			
	if( dei_rs_state == NULL )
    {            
	    if( get_rs_smart() ) notify_send( NOTIFY_SMART_MODE_OFF );    
	    if( get_rs_timer() ) notify_send( NOTIFY_TIMER_MODE_OFF );
                
        if( !get_ss_trigger() )
	    {			
		    if( get_rs_mts() || dei_rs_mts_door_checking_timeout_status() )
	        {
						set_rs_mts  ( FALSE );
	    	    notify_send( NOTIFY_MTS_DISABLE );
#ifdef PLATFORM_933            
						cmd_q_rs_insert( INPUT_GND_WHEN_READY_MODE_OFF, 100 );
#endif            
                    
                if( dei_rs_mts_door_checking_timeout_status() )
                {
                    //--- the timeout for MTS is in progress
                    //--- stop the timer

                    dei_rs_mts_door_checking_timeout_stop();
                }
                
                dei_prk_flash( DEI_PRK_MTS_OFF ); 

                notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_RS_PROGRAMMING ); 
	        }
	        else
	        {	      
			    if( get_rs_smart() || get_rs_timer() )
	    	    {	    		 
				    if( get_rs_smart() ) dei_prk_flash( DEI_PRK_SMART_OFF ); 	    		
				    if( get_rs_timer() ) dei_prk_flash( DEI_PRK_TMR_OFF   ); 
	            }
	        }
	    }

        set_rs_smart( FALSE );
	    set_rs_timer( FALSE );
	    set_rs_mts  ( FALSE );
    }    	    			    				    
}

//--------------------------------------------------------------------------//

