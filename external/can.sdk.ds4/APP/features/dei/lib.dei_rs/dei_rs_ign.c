/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_ign.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rs_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#ifdef PLATFORM_CM800
	#define DEI_RS_ACC_DIESEL_START_DELAY			dei_feature_get(DEI_FEAT_ACCESSORY_DURING_DIESEL_START_DELAY  )
#else
	#define DEI_RS_ACC_DIESEL_START_DELAY			1
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

void dei_rs_ign( void )
{
	
	#define EVT_MSK_IGN	(1<<DEI_RS_EVT_TRANSMITTER_SHUTDOWN)|\
	                    (1<<DEI_RS_EVT_HOOD                )|\
						(1<<DEI_RS_EVT_BRAKE               )|\
						(1<<DEI_RS_EVT_NEUTRAL             )|\
                        (1<<DEI_RS_EVT_HANDBRAKE           )|\
						(1<<DEI_RS_EVT_TRIGGER             )|\
						(1<<DEI_RS_EVT_ENGINE_RUNNING      )
						

	Dei_Rs_Evt evt = dei_rs_evt_wait( EVT_MSK_IGN , DEI_RS_ACC_TO_IGN_TIMEOUT );
	
	if( (evt  == DEI_RS_EVT_TIMEOUT) || (evt == DEI_RS_EVT_ENGINE_RUNNING) )
	{
		set_rs_ign( TRUE );

		cmd_q_rs_insert( INPUT_RS_IGNITION_ON , 100 ); 
    	
		if( evt == DEI_RS_EVT_TIMEOUT )
		{
			dei_rs_state = (Dei_Rs_State)dei_rs_wait_start ;
		
		
			if( DEI_RS_ACC_DIESEL_START_DELAY == 2 ) //no acc
			{
				set_rs_acc( FALSE );
			
				cmd_q_rs_insert( INPUT_RS_ACCESSORY_OFF , 100 ) ; 
			}
		}
		else
		{			
			dei_rs_runtime_start();
			
			ATOMIC( dei_rs_state = (Dei_Rs_State)dei_rs_running );			
		}
					
	}
	else
	{
		set_rs_error_shutdown( evt );

		dei_rs_shutdown();
	}



	#undef EVT_MSK_IGN
}

//--------------------------------------------------------------------------//

