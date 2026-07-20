/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_retry.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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

void dei_rs_retry( void )
{
	Dei_Rs_Evt evt ;
		
	if( get_rs_start() )
	{
		set_rs_start( FALSE );

		cmd_q_rs_insert( INPUT_RS_STARTER_OFF , 100 ); 
	}
	
	if( get_rs_ign() )
	{
		set_rs_ign( FALSE );

		cmd_q_rs_insert( INPUT_RS_IGNITION_OFF , 100 );     	
	}
	
	if( get_rs_acc() )
	{
		set_rs_acc( FALSE ) ; 

		cmd_q_rs_insert( INPUT_RS_ACCESSORY_OFF , 100 ); 
	}
	
	set_rs_retry_count( get_rs_retry_count() + 1 ); 
		
	if( get_rs_retry_count() == 3 )
	{
		dei_rs_shutdown();
	}
	else
	{
		
		dei_rs_runtime_pause();		//save remaining runtim	
		
		#define EVT_MSK_RETRY	(1<<DEI_RS_EVT_TRANSMITTER_SHUTDOWN     )|\
	                    		(1<<DEI_RS_EVT_HOOD                		)|\
								(1<<DEI_RS_EVT_BRAKE               	    )|\
								(1<<DEI_RS_EVT_NEUTRAL             		)|\
                        		(1<<DEI_RS_EVT_HANDBRAKE            	)|\
								(1<<DEI_RS_EVT_TRIGGER             		)
		
		dei_prk_flash( DEI_PRK_RS_STOP	 );				

		evt = dei_rs_evt_wait( EVT_MSK_RETRY , 3000 );
	
		if(evt  == DEI_RS_EVT_TIMEOUT )
		{
			ATOMIC( dei_rs_state = (Dei_Rs_State)dei_rs_acc ) ;				
		}
		else
		{			
			set_rs_error_shutdown( evt );

			dei_rs_shutdown();
		}		

		#undef EVT_MSK_RETRY
	}
}

//--------------------------------------------------------------------------//

