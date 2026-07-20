/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_start.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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
static void dei_rs_start_execute( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean get_rs_progress() {
	
	return rs_progress; 
	}

//--------------------------------------------------------------------------//
	
Boolean dei_rs_is_active(void)
{
	return (dei_rs_state != 0);
}

//--------------------------------------------------------------------------//

void dei_rs_start( void )
{
	VIRTUAL_WATCH_WITH_CUSTOM_TIMEOUT(dei_rs_start_execute(),12);
}

//--------------------------------------------------------------------------//

static void dei_rs_start_execute( void )
{
    #define EVT_MSK_START	(1<<DEI_RS_EVT_TRANSMITTER_SHUTDOWN    )|\
	                    	(1<<DEI_RS_EVT_HOOD                    )|\
							(1<<DEI_RS_EVT_BRAKE                   )|\
							(1<<DEI_RS_EVT_NEUTRAL                 )|\
                        	(1<<DEI_RS_EVT_HANDBRAKE               )|\
							(1<<DEI_RS_EVT_TRIGGER                 )|\
							(1<<DEI_RS_EVT_ENGINE_RUNNING          )    
    Dei_Rs_Evt evt ;													
	
	rs_progress=TRUE;
	set_rs_takeover(FALSE);
	switch( evt = dei_rs_evt_wait( EVT_MSK_START , DEI_RS_START_TIMEOUT ) )
	{
		case DEI_RS_EVT_TIMEOUT : //event is possible only in tacho and vtacho mode
		{			
			set_rs_start( FALSE );
			
			cmd_q_rs_insert( INPUT_RS_STARTER_OFF  , 100 , (void*)CMD_RS_START_ERROR ); 
			
            if( dei_feature_get( DEI_FEAT_ENGINE_CHECKING_MODE ) == DEI_ECM_VIRTUAL_TACHO )
            {
                set_rs_error_shutdown( DEI_RS_EVT_LOW_TACH ); //OR LOW_BATT ?
            }
            else
            {
			    set_rs_error_shutdown( DEI_RS_EVT_LOW_TACH );
            }

			dei_rs_retry();
		}
		break;
	
		
		case DEI_RS_EVT_ENGINE_RUNNING:
		{
			dei_rs_runtime_start();
			
			ATOMIC( dei_rs_state = (Dei_Rs_State)dei_rs_running  );
						
			set_rs_acc( TRUE );
			
			set_rs_start( FALSE );
			
			cmd_q_rs_insert( INPUT_RS_STARTER_OFF  , 100 , (void*)CMD_RS_START_SUCCESSFUL ); 
			
			cmd_q_rs_insert( INPUT_RS_ACCESSORY_ON , 100 ); 
			
//			if( brand_id != BRAND_VP_SC)
//				notify_send( NOTIFY_RS_START );

			dei_rs_secure_lock_start_off();			
		}
		break;

		default :
		{			
			//rs.error_shutdown = evt ;
			set_rs_error_shutdown( evt );

			dei_rs_shutdown();
		}
		break;
	};


	#undef EVT_MSK_START
}

//--------------------------------------------------------------------------//


