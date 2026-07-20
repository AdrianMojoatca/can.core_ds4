/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_rf_trunk.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_system_private.h"
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

void dei_system_rf_trunk( void *remote )
{		
	if( dei_rs_mts_ready_for_activation() )
	{
		cmd_q_rs_insert( INPUT_RF_START_STOP_TOOGLE , 100 , 0 );

		return  ; 
	}
	
	if( dei_feature_get( DEI_FEAT_CH2_OUTPUT_TYPE ) == DEI_CH2_OUTPUT_VALIDITY )
    {
        dei_system_rf_range_check();		
    }
}

//--------------------------------------------------------------------------//

void dei_system_rf_trunk_ext( void *remote )
{
  
  if( get_ss_panic()  || get_ss_trigger()  ) return ;
	
	if( dei_feature_get( DEI_FEAT_CH2_OUTPUT_TYPE ) == DEI_CH2_OUTPUT_VALIDITY )
	{
			if( dei_feature_get( DEI_FEAT_OEM_ALARM_DISARM_W_CH2 )==DEI_OEM_DISARM_CH2_ON )
			{
				cmd_q_rs_insert( INPUT_DISARM            , 100 ) ; 												  
				cmd_q_rs_insert( INPUT_TRUNK  |INPUT_ON  , 100  , (void*)CMD_TRUNK_WITH_DISARM ) ; 	
			}
			else
			{
				cmd_q_rs_insert( INPUT_TRUNK | INPUT_ON  , 100  , NULL ) ; 			
			}	
			
			notify_send( NOTIFY_TRUNK_RELEASE_OPEN );
	}	
}

//--------------------------------------------------------------------------//

void dei_system_rf_trunk_release( void *remote )
{  
		if( get_ss_panic()  || get_ss_trigger()  ) return ;
	
		if( dei_feature_get( DEI_FEAT_CH2_OUTPUT_TYPE ) == DEI_CH2_OUTPUT_VALIDITY )
		{
			cmd_q_rs_insert( INPUT_TRUNK , 100 , NULL );
		}
}

//--------------------------------------------------------------------------//
