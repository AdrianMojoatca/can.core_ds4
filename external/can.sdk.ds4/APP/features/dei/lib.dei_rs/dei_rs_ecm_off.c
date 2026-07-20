/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_ecm_off.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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

void dei_rs_ecm_off( void )
{
   Boolean engine_running = get_ign_status();
    
    while( dei_rs_state != NULL )
	{			
		check_rs_state :
		{
			os_tsk_wait(10);
	
			if( (dei_rs_state == dei_rs_gwr       )||\
                (dei_rs_state == dei_rs_acc       )||\
                (dei_rs_state == dei_rs_ign       )||\
                (dei_rs_state == dei_rs_wait_start)
                )
			{			  
				if( dei_rs_state == dei_rs_acc )
                {
                    engine_running = get_ign_status();
                }
                
                if( engine_running )
				{					
					dei_rs_evt_signal( DEI_RS_EVT_ENGINE_RUNNING ); 
				}
				
				goto check_rs_state ; 
			}	

					
			if( dei_rs_state == dei_rs_start )
			{
				static const TskTimeout cranking_time[] = 
				{
					600 , 800 , 1000 , 1200 , 1400 , 1600 , 1800 , 2000 , 4000 
				}; 
					
				os_tsk_wait( cranking_time[ dei_feature_get( DEI_FEAT_CRANKING_TIME )-1 ] );
	
				dei_rs_evt_signal( DEI_RS_EVT_ENGINE_RUNNING );				
	
				goto check_rs_state ;
			}			


            if( dei_rs_state == dei_rs_running )
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

	os_tsk_stop();
}
				
//--------------------------------------------------------------------------//

