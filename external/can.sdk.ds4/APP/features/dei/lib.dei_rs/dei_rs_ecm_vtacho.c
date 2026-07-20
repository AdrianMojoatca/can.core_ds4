/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_ecm_vtacho.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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

void dei_rs_ecm_vtacho( void )
{	      	
	Boolean vtacho_req_init = FALSE;
    Boolean engine_running  = FALSE ;
	  
	if( !get_ign_status() )
	{	  
		vtacho_req_init = (get_rs_vtacho_ref() == ECM_VTACHO_REF_NOT_PROGRAMMED);

		TRACE_DEI_RS("\n\r[DEI_VTACHO_%s_INIT]\n\r" , vtacho_req_init ? "REQUEST" : "NOT_REQUEST" );
	}
    else
    {
        engine_running = get_ign_status();
    }
    
	     	
	while( dei_rs_state != NULL )
	{	
		os_tsk_wait(10);

        if( (dei_rs_state == dei_rs_gwr       )||\
            (dei_rs_state == dei_rs_acc       )||\
            (dei_rs_state == dei_rs_ign       )||\
            (dei_rs_state == dei_rs_wait_start))
			{			  
				if( dei_rs_state == dei_rs_acc )
                {
                    engine_running = get_ign_status();

                    ecm_vtacho_engine_status( engine_running );
                }
                                
                if( engine_running )
				{					
					dei_rs_evt_signal( DEI_RS_EVT_ENGINE_RUNNING ); 

                    if( dei_rs_state == dei_rs_wait_start )
                    {
                        if( get_rs_start_successful() == FALSE )
                        {
                            set_rs_start_successful( TRUE );
                    
                            notify_send( NOTIFY_RS_SUCCESSFULLY_STARTED );
														if( brand_id != BRAND_VP_SC )
															notify_send( NOTIFY_RS_START );

			
			                if( dei_feature_get(DEI_FEAT_PARKING_LIGHT_OUTPUT) != 3 ) //parking light off
                            {
                                dei_prk_flash( (dei_feature_get( DEI_FEAT_PARKING_LIGHT_OUTPUT)==1) ? DEI_PRK_RS_CONSTANT : DEI_PRK_RS_PULSED );
                            }

                            dei_rs_auto_defogger();
                        }
                    }                        
				}				
			}	

				
		switch( ecm_vtacho_looping() )
		{
			case ECM_VTACHO_EVT_CRANK_RELEASE :
			{
				dei_rs_evt_signal( DEI_RS_EVT_ENGINE_RUNNING );			
			}
			break;
			
			case ECM_VTACHO_EVT_ENGINE_STOPPED:
			{				
				dei_rs_evt_signal( DEI_RS_EVT_LOW_BATT );
				
                engine_running = FALSE ;

                ecm_vtacho_engine_status( engine_running );

                set_rs_start_successful( FALSE );
			}
			break;			 	

            case ECM_VTACHO_EVT_ENGINE_RUNNING:
			{				
				if( get_rs_start_successful() == FALSE )
                {
                    set_rs_start_successful( TRUE );
                    
                    notify_send( NOTIFY_RS_SUCCESSFULLY_STARTED );
										if( brand_id != BRAND_VP_SC )
											notify_send( NOTIFY_RS_START );
			
			        if( dei_feature_get(DEI_FEAT_PARKING_LIGHT_OUTPUT) != 3 ) //parking light off
                    {
                        dei_prk_flash( (dei_feature_get( DEI_FEAT_PARKING_LIGHT_OUTPUT)==1) ? DEI_PRK_RS_CONSTANT : DEI_PRK_RS_PULSED );
                    }
                }
			}
			break;			 	
			
			default : break;
		}
	}
		
	
	if( vtacho_req_init && (ecm_vtacho_ref() != ECM_VTACHO_REF_NOT_PROGRAMMED) )
	{		 
		dei_feature_set( DEI_FEAT_VTACHO_REF             , ecm_vtacho_ref() );
		dei_feature_set( DEI_FEAT_VIRTUAL_TACH_FINE_TUNE , 10               );
		
		TRACE_DEI_RS("\n\r[DEI_RS_VTACHO_PROGRAMMED %d ]\n\r" , ecm_vtacho_ref());

		nvram_usr_commit();
	}		
		
    os_tsk_stop();
}
				
//--------------------------------------------------------------------------//

