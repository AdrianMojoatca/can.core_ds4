/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_wait_start.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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
static void dei_rs_wait_start_glowplug( void );
static void dei_rs_wait_start_delayed ( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_rs_wait_start( void )
{
	#define EVT_MSK_WAIT_START     (1<<DEI_RS_EVT_TRANSMITTER_SHUTDOWN    )|\
	                    		   (1<<DEI_RS_EVT_HOOD                    )|\
								   (1<<DEI_RS_EVT_BRAKE                   )|\
								   (1<<DEI_RS_EVT_NEUTRAL                 )|\
                        		   (1<<DEI_RS_EVT_HANDBRAKE               )|\
								   (1<<DEI_RS_EVT_TRIGGER                 )|\
                                   (1<<DEI_RS_EVT_ENGINE_RUNNING          )
                                       
	UInt8 evt ;
	
	TRACE_DEI_RS("\n\r[RS_WAIT_TO_START]\n\r");
    
    switch( evt = dei_rs_evt_wait( EVT_MSK_WAIT_START , DEI_RS_WAIT_TIMEOUT ) )
    {
        case DEI_RS_EVT_TIMEOUT :
        {
					#ifdef PLATFORM_CM800
            (dei_feature_get( DEI_FEAT_DIESEL_START_DELAY )==0) ? dei_rs_wait_start_glowplug() : dei_rs_wait_start_delayed();		
					#else
					  dei_rs_wait_start_delayed();
					#endif
        }
        break;

        case DEI_RS_EVT_ENGINE_RUNNING:
		{
			dei_rs_runtime_start();
			
			ATOMIC( dei_rs_state = (Dei_Rs_State)dei_rs_running );

			if( !get_rs_acc() )
			{
				set_rs_acc( TRUE );

				cmd_q_rs_insert( INPUT_RS_ACCESSORY_ON , 100 , 0 ); 
			}
			if( brand_id != BRAND_VP_SC )
				notify_send( NOTIFY_RS_START );


			set_rs_start_successful( TRUE );

			dei_prk_flash( (dei_feature_get( DEI_FEAT_PARKING_LIGHT_OUTPUT)==1) ? DEI_PRK_RS_CONSTANT : DEI_PRK_RS_PULSED );
			
			dei_rs_secure_lock_start_off();  //start off implied
		}
		break;

		default:
		{
			set_rs_error_shutdown( evt );

			dei_rs_shutdown();
		}
		break;
    } 
}

//--------------------------------------------------------------------------//

void dei_rs_wait_start_glowplug( void )
{
	#define EVT_MSK_WAIT_GLOWPLUG  (1<<DEI_RS_EVT_TRANSMITTER_SHUTDOWN    )|\
	                    		   (1<<DEI_RS_EVT_HOOD                    )|\
								   (1<<DEI_RS_EVT_BRAKE                   )|\
								   (1<<DEI_RS_EVT_NEUTRAL                 )|\
                        		   (1<<DEI_RS_EVT_HANDBRAKE               )|\
								   (1<<DEI_RS_EVT_TRIGGER                 )|\
								   (1<<DEI_RS_EVT_ENGINE_RUNNING          )|\
								   (1<<DEI_RS_EVT_GLOWPLUG_OFF            )
	UInt8 evt ;
	
    TRACE_DEI_RS("\n\r[RS_WAIT_TO_START_GLOWPLUG]\n\r");
	
    switch( evt = dei_rs_evt_wait( EVT_MSK_WAIT_GLOWPLUG , get_glowplug_off_status() ? 500 : DEI_RS_WAIT_GLOWPLUG_TIMEOUT ) )
	{
		case DEI_RS_EVT_TIMEOUT      :
        case DEI_RS_EVT_GLOWPLUG_OFF :
		{
			if( !get_glowplug_off_status() )
			{
				set_rs_error_shutdown( DEI_RS_EVT_GLOWPLUG_TIMEOUT );

				dei_rs_shutdown(); 
			}
			else
			{
								
                if( (get_rpm() == 0) || ( dei_feature_get( DEI_FEAT_ENGINE_CHECKING_MODE ) != DEI_ECM_TACHOMETER)  )
                {
                    ATOMIC( dei_rs_state = (Dei_Rs_State)dei_rs_start ); 
                    
                    if( get_rs_acc() )
				    {					    
					    set_rs_acc( FALSE );

					    cmd_q_rs_insert( INPUT_RS_ACCESSORY_OFF  , 100 ); 
				    }
				                                        
					set_rs_start( TRUE );

				    cmd_q_rs_insert( INPUT_RS_STARTER_ON  , 100 ); 
                }
                else
                {
                    //the remote starter is in tachometer mode and rpm > 0 
                    
                    dei_rs_runtime_start();
			
			        ATOMIC( dei_rs_state = (Dei_Rs_State)dei_rs_running );
			
			        if( !get_rs_acc() )
			        {
				        set_rs_acc( TRUE );

				        cmd_q_rs_insert( INPUT_RS_ACCESSORY_ON , 100 ); 
			        }
							dei_rs_secure_lock_start_off();  //start off implied
                }                    
			}
		}
		break;

		case DEI_RS_EVT_ENGINE_RUNNING:
		{
			dei_rs_runtime_start();
			
			ATOMIC( dei_rs_state = (Dei_Rs_State)dei_rs_running );

			if( !get_rs_acc() )
			{
				set_rs_acc( TRUE );

				cmd_q_rs_insert( INPUT_RS_ACCESSORY_ON , 100 ); 
			}			
			dei_rs_secure_lock_start_off();  //start off implied
		}
		break;

		default:
		{
			set_rs_error_shutdown( evt );

			dei_rs_shutdown();
		}
		break;
	};


	#undef EVT_MSK_WAIT_GLOWPLUG
}

//--------------------------------------------------------------------------//

void dei_rs_wait_start_delayed( void )
{
	#define EVT_MSK_WAIT_DELAYED   (1<<DEI_RS_EVT_TRANSMITTER_SHUTDOWN    )|\
	                    		   (1<<DEI_RS_EVT_HOOD                    )|\
								   (1<<DEI_RS_EVT_BRAKE                   )|\
								   (1<<DEI_RS_EVT_NEUTRAL                 )|\
                        		   (1<<DEI_RS_EVT_HANDBRAKE               )|\
								   (1<<DEI_RS_EVT_TRIGGER                 )|\
								   (1<<DEI_RS_EVT_ENGINE_RUNNING          )
								 
	UInt8 evt ;	

	TRACE_DEI_RS("\n\r[RS_WAIT_TO_START_DELAYED]\n\r");

#ifdef PLATFORM_CM800
	switch( evt = dei_rs_evt_wait( EVT_MSK_WAIT_DELAYED , dei_feature_get( DEI_FEAT_DIESEL_START_DELAY )*1000))
#else
	switch( evt = dei_rs_evt_wait( EVT_MSK_WAIT_DELAYED , (dei_feature_get( DEI_FEAT_DIESEL_START_DELAY )==0) ? 500 : (dei_feature_get( DEI_FEAT_DIESEL_START_DELAY )*1000)))
#endif
	{
		case DEI_RS_EVT_TIMEOUT      :
		{
			if( (get_rpm() == 0) || ( dei_feature_get( DEI_FEAT_ENGINE_CHECKING_MODE ) != DEI_ECM_TACHOMETER)  )
            {
                ATOMIC( dei_rs_state = (Dei_Rs_State)dei_rs_start ); 
                    
                if( get_rs_acc() )
				{					    
			        set_rs_acc( FALSE );

					cmd_q_rs_insert( INPUT_RS_ACCESSORY_OFF  , 100 ); 
				}
				                                        
				set_rs_start( TRUE );

				cmd_q_rs_insert( INPUT_RS_STARTER_ON  , 100 ); 
             }
             else
             {
                //the remote starter is in tachometer mode and rpm > 0 
                    
                dei_rs_runtime_start();
			
			    ATOMIC( dei_rs_state = (Dei_Rs_State)dei_rs_running );
			
			    if( !get_rs_acc() )
			    {				        
				    set_rs_acc( TRUE );

				    cmd_q_rs_insert( INPUT_RS_ACCESSORY_ON , 100 ); 
			    }			    
					dei_rs_secure_lock_start_off();  //start off implied
             }                    
		}
		break;

		case DEI_RS_EVT_ENGINE_RUNNING:
		{
			dei_rs_runtime_start();
			
			ATOMIC( dei_rs_state = (Dei_Rs_State)dei_rs_running );
			
			if( !get_rs_acc() )
			{				
				set_rs_acc( TRUE );

				cmd_q_rs_insert( INPUT_RS_ACCESSORY_ON , 100 ); 
			}
			dei_rs_secure_lock_start_off();  //start off implied
		}
		break;

		default:
		{			
			set_rs_error_shutdown( evt );

			dei_rs_shutdown();
		}
		break;
	};

	#undef EVT_MSK_WAIT_DELAYED

}

//--------------------------------------------------------------------------//
