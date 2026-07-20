/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_ecm_tacho.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rs_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define DELAY_CHECK_OVER_REV   100     //How many consecutive readings of OVER REV RPM
                                       //will trigger the OVER REV shutdown

#define DELAY_CHECK_RPM        200     //How many consecutive readings of RPM
                                       //will trigger engine running conditions in wait to start state
#ifndef	DELAY_CHECK_RPM_LOW								   
#define DELAY_CHECK_RPM_LOW   800     // --- After 8000ms from starting , we will check the RPM
                                       // --- If is lower than theshold we abort the cranking
#else 
#warning "DELAY CHECK_RPM_LOW has been reconfigured in the firmware"
#endif

#ifndef	DELAY_CHECK_RPM_ZERO								   
#define DELAY_CHECK_RPM_ZERO   200     // --- After 2000ms from starting , we will check the RPM
                                       // --- If is zero we abort the cranking
#else 
#warning "DELAY CHECK_RPM_ZERO has been reconfigured in the firmware"
#endif

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

void dei_rs_ecm_tacho( void )
{
	#define TACHO_FINE_TUNE 6

	UInt16 rpm , rpm_ref ; 

    UInt16  delay_check_over_rev = DELAY_CHECK_OVER_REV;
    UInt16  delay_check_rpm      = DELAY_CHECK_RPM ;
    UInt16  delay_check_rpm_zero = DELAY_CHECK_RPM_LOW ;

    
    Boolean do_idle = !!get_rpm();

	rpm_ref = get_rs_rpm_ref();
	
	while( dei_rs_state != NULL )
	{							
		rs_check_state :
		{		
			rpm = get_rpm();
			
			os_tsk_wait( 10 );

            if( (dei_rs_state == dei_rs_gwr       )||\
                (dei_rs_state == dei_rs_acc       )||\
                (dei_rs_state == dei_rs_ign       )||\
                (dei_rs_state == dei_rs_wait_start)
                )
			{			  
				delay_check_rpm_zero = DELAY_CHECK_RPM_LOW;
                
                if( !do_idle )
                {
                    if( rpm )
    				{					
    					delay_check_over_rev = DELAY_CHECK_OVER_REV;
                        
                        if( !(--delay_check_rpm) )
                        {
                            dei_rs_evt_signal( DEI_RS_EVT_ENGINE_RUNNING );				
                        }
    				}
                    else
                    {
                        set_rs_start_successful( FALSE );
                        
                        delay_check_rpm = DELAY_CHECK_RPM ;
                    }
                    
                    if( get_gearbox_type() == GEARBOX_MANUAL )
                    {
                        //--- if the idle mode is not active and the door or trunk is opening during the wait to start
                        //--- period we force a shutdown simulating the trigger
                        //--- available only for manual gearbox
                        if( get_door_status() || get_trunk_status() )
                        {
                            dei_rs_evt_signal( DEI_RS_EVT_TRIGGER );	                        							
                        }	
                    }
                }
                else
                {
                    dei_rs_evt_signal( DEI_RS_EVT_ENGINE_RUNNING );				
                }                    
			}	
            
                        			
			if( dei_rs_state == dei_rs_start )
			{			  
				UInt16 rpm_crank_release  = (5 * rpm_ref * ( TACHO_FINE_TUNE + 4 + 2 *( get_rs_retry_count())))/100 ;
                
               
                if( rpm > rpm_crank_release )
				{
					delay_check_over_rev = DELAY_CHECK_OVER_REV ;
                    
                    dei_rs_evt_signal( DEI_RS_EVT_ENGINE_RUNNING );	
				}
                else
                {
                    if( delay_check_rpm_zero == 0 )
                    {
                        if(rpm > 0 && rpm < ((rpm_crank_release*20)/100) )
                        {
                            dei_rs_evt_signal( DEI_RS_EVT_ENGINE_RUNNING );	                    
                        }
                    }
                    else
                    {                        
                        delay_check_rpm_zero  -- ; 
                    }
										
                    if( delay_check_rpm_zero < (DELAY_CHECK_RPM_LOW - DELAY_CHECK_RPM_ZERO)  )
                    {
                        if( rpm == 0 )
                        {
                            //dei_rs_evt_signal( DEI_RS_EVT_ENGINE_RUNNING );	//Is it true?
							delay_check_rpm_zero = 0;                    
                        }
                    }
                }                								
			}
				
			if( dei_rs_state == dei_rs_running )
			{				  
				if( get_rs_start_successful() == FALSE )
                {
                    if( rpm != 0 )
                    {
                        set_rs_start_successful( TRUE );
                        
                        notify_send( NOTIFY_RS_SUCCESSFULLY_STARTED );    //update smart start status only
											
												if( brand_id != BRAND_VP_SC )
												  notify_send( NOTIFY_RS_START );
    			
    			        if( dei_feature_get(DEI_FEAT_PARKING_LIGHT_OUTPUT) != 3 ) //parking light off
                        {
                            dei_prk_flash( (dei_feature_get( DEI_FEAT_PARKING_LIGHT_OUTPUT)==1) ? DEI_PRK_RS_CONSTANT : DEI_PRK_RS_PULSED );
                        }
    
                        dei_rs_auto_defogger();
                    }

                    delay_check_rpm_zero = DELAY_CHECK_RPM_ZERO ; // --- restart the rpm zero timeout for next start
                }
                                                
                if( rpm < rpm_ref / 5 )
				{				  					                     
                     dei_rs_evt_signal( DEI_RS_EVT_LOW_TACH  );                                                                                                           
				}
				
			  
				if( rpm  > 3*rpm_ref )
				{									                    
					if( delay_check_over_rev == 0 )
                    {                    
                        set_rs_retry_count( 2 );
                      
                        dei_rs_evt_signal( DEI_RS_EVT_OVERREV   );	
                                                    
                    }
                    else
                    {
                        delay_check_over_rev -- ;
                    }
				}
                else
                {
                    delay_check_over_rev = DELAY_CHECK_OVER_REV ;
                }    										
			}
		}
	}

	os_tsk_stop();

	#undef TACH_FINE_TUNE
}
				
//--------------------------------------------------------------------------//

#undef DELAY_CHECK_OVER_REV   
#undef DELAY_CHECK_RPM        
#undef DELAY_CHECK_RPM_ZERO 
#undef DELAY_CHECK_RPM_LOW 
