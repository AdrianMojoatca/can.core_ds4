/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_rf_smart_toogle.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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

void dei_rs_rf_smart_toogle( void *remote )
{			
	if(  get_ss_trigger()  || get_ss_panic() )
    {
        notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_ALARM_TRIGGER  );

        return ;
    }
		
		if( get_rs_valet() )
    {
        notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_RS_VALET  );

        return ;
    }
	
		
	if( get_ign_status() && !dei_rs_state )
	{
		notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_FEATURE_NA );	
		
		return ;
	}


	if( !get_rs_smart() )
    {
        Dei_Rs_Evt error_start;

        error_start = dei_rs_error_start() ; 

        if( error_start )
        {
            notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_RS_PROGRAMMING );
			
            return ;
        }
    }

#ifdef PLATFORM_933

	dei_rs_smart_stop();


	set_rs_smart( !get_rs_smart() );
	set_rs_timer( FALSE );
	
	notify_send( get_rs_smart() ? NOTIFY_SMART_MODE_ON : NOTIFY_SMART_MODE_OFF );
		 
	dei_prk_flash( get_rs_smart() ? DEI_PRK_SMART_ON : DEI_PRK_SMART_OFF ) ;
	
	dei_siren_chirp(get_rs_smart() ? DEI_SIREN_LOW_TEMP_MODE_ON : DEI_SIREN_LOW_TEMP_MODE_OFF);
		
	dei_rs_smart_start();
	
	TRACE_DEI_RS("\n\r[DEI_RS SMART MODE ACTIVATED && STARTED]\n\r"); 

#else    
        
	set_rs_smart( !get_rs_smart() );
	set_rs_timer( FALSE );
	

	notify_send( get_rs_smart() ? NOTIFY_SMART_MODE_ON : NOTIFY_SMART_MODE_OFF );
		
	dei_prk_flash( get_rs_smart() ? DEI_PRK_SMART_ON : DEI_PRK_SMART_OFF ) ;

	dei_rs_timer_stop();
	
	if( get_rs_smart() )
	{
		if( get_ss_lock() )
		{
			dei_rs_smart_start() ;

			TRACE_DEI_RS("\n\r[DEI_RS SMART MODE ACTIVATED && STARTED]\n\r"); 
		}
		else
		{
			TRACE_DEI_RS("\n\r[DEI_RS SMART MODE ACTIVATED && NOT STARTED ( CAR UNLOCKED ) ]\n\r"); 
		}
	}
	else
	{
		dei_rs_smart_stop();

		TRACE_DEI_RS("\n\r[DEI_RS SMART MODE DEACTIVATED]\n\r"); 
	}
#endif
}
	

//--------------------------------------------------------------------------//

