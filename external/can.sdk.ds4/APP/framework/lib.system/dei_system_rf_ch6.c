/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_rf_ch6.c 17934 2012-10-29 19:23:36Z martin.bouchard $
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

void dei_system_rf_ch6_on( void *remote )
{			
	if( dei_feature_get( DEI_FEAT_AUX_4_OUTPUT_TYPE ) !=  DEI_AUX_OUTPUT_OFF ) //do not notify when AUX out is OFF
	{	
	  //if( !ss.ch6_status )
		if( !get_ss_ch6()  )
		{			
			notify_send( NOTIFY_AUX_CH6_ON );      

			TRACE_SYS("\n\r[DEI_SYSTEM_AUX_CH6_RF_ON]\n\r");

			cmd_q_rs_insert( INPUT_AUX4  | INPUT_ON , 100  , (void*)CMD_AUX_FROM_RF ) ; 			                                  
		}
		else
		{
			notify_send( NOTIFY_AUX_CH6_OFF );      

			TRACE_SYS("\n\r[DEI_SYSTEM_AUX_CH6_RF_OFF]\n\r");

			cmd_q_rs_insert( INPUT_AUX4  , 100  , (void*)CMD_AUX_FROM_RF ) ; 			                                  
		}				
	}	
}

//--------------------------------------------------------------------------//

void dei_system_ch6_reset( void )
{
	if( dei_feature_get( DEI_FEAT_AUX_4_OUTPUT_TYPE ) == DEI_AUX_OUTPUT_LATCH_RESET_IGN ) //latch with ignition reset
	{
	  //if( ss.ch6_status )
		if( get_ss_ch6()  )
		{
			cmd_q_rs_insert( INPUT_AUX4 , 100  , (void*)CMD_AUX_RESET ) ; 
		
			TRACE_SYS("\n\r[DEI_SYSTEM_AUX_CH6_RESET]\n\r");			                                  
		}
	}	
}	

//--------------------------------------------------------------------------//

void dei_system_rf_ch6_off( void *remote )
{
	if( dei_feature_get( DEI_FEAT_AUX_4_OUTPUT_TYPE )== DEI_AUX_OUTPUT_VALIDITY ) 
	{		
		cmd_q_rs_insert( INPUT_AUX4 , 100  , (void*)CMD_AUX_FROM_RF ) ; 			                                  

		TRACE_SYS("\n\r[DEI_SYSTEM_AUX_CH6_RF_OFF( release )]\n\r");
	}
}

//--------------------------------------------------------------------------//

void dei_system_ch6_link_arm( void )
{
	#define AUX4_LINK_UNLOCK	(((dei_feature_get(DEI_FEAT_AUX_4_LINKING    )==DEI_AUX_LINK_TO_DISARM    ) ||\
						  		  (dei_feature_get(DEI_FEAT_AUX_4_LINKING    )==DEI_AUX_LINK_TO_ARM_DISARM))&&\
						  		  (dei_feature_get(DEI_FEAT_AUX_4_OUTPUT_TYPE)!=DEI_AUX_OUTPUT_OFF    ))
	
	#define AUX4_LINK_LOCK		(((dei_feature_get(DEI_FEAT_AUX_4_LINKING    )==DEI_AUX_LINK_TO_ARM       ) ||\
								  (dei_feature_get(DEI_FEAT_AUX_4_LINKING    )==DEI_AUX_LINK_TO_ARM_DISARM))&&\
								  (dei_feature_get(DEI_FEAT_AUX_4_OUTPUT_TYPE)!=DEI_AUX_OUTPUT_OFF))
		
  //if( AUX4_LINK_UNLOCK && ss.ch6_status )
	if( AUX4_LINK_UNLOCK && get_ss_ch6()  )
	{		
		TRACE_SYS("\n\r[DEI_SYSTEM_AUX_CH6_OFF - reset LINK ARM]\n\r");
		
		cmd_q_rs_insert( INPUT_AUX4 , 100 , (void*)CMD_AUX_WITH_ARM );
	}
	
	if( AUX4_LINK_LOCK )
	{
		TRACE_SYS("\n\r[DEI_SYSTEM_AUX_CH6_ON - LINK ARM]\n\r");
		
		cmd_q_rs_insert( INPUT_AUX4            , 100 , (void*)CMD_AUX_WITH_ARM );
		cmd_q_rs_insert( INPUT_AUX4 | INPUT_ON , 100 , (void*)CMD_AUX_WITH_ARM );
	}
	
	#undef AUX4_LINK_LOCK			
	#undef AUX4_LINK_UNLOCK	
}

//--------------------------------------------------------------------------//

void dei_system_ch6_link_disarm( void )
{
	#define AUX4_LINK_UNLOCK	(((dei_feature_get(DEI_FEAT_AUX_4_LINKING    )==DEI_AUX_LINK_TO_DISARM    ) ||\
						  		  (dei_feature_get(DEI_FEAT_AUX_4_LINKING    )==DEI_AUX_LINK_TO_ARM_DISARM))&&\
						  		  (dei_feature_get(DEI_FEAT_AUX_4_OUTPUT_TYPE)!=DEI_AUX_OUTPUT_OFF    ))
	
	#define AUX4_LINK_LOCK		(((dei_feature_get(DEI_FEAT_AUX_4_LINKING    )==DEI_AUX_LINK_TO_ARM       ) ||\
								  (dei_feature_get(DEI_FEAT_AUX_4_LINKING    )==DEI_AUX_LINK_TO_ARM_DISARM))&&\
								  (dei_feature_get(DEI_FEAT_AUX_4_OUTPUT_TYPE)!=DEI_AUX_OUTPUT_OFF))
	
	
  //if( AUX4_LINK_LOCK && ss.ch6_status )
	if( AUX4_LINK_LOCK && get_ss_ch6()  )
	{		
		TRACE_SYS("\n\r[DEI_SYSTEM_AUX_CH6_OFF - reset LINK DISARM]\n\r");

		cmd_q_rs_insert( INPUT_AUX4 , 100 , (void*)CMD_AUX_WITH_DISARM );
	}
	
	if( AUX4_LINK_UNLOCK )
	{
		TRACE_SYS("\n\r[DEI_SYSTEM_AUX_CH6_ON - LINK DISARM]\n\r");

		cmd_q_rs_insert( INPUT_AUX4            , 100 , (void*)CMD_AUX_WITH_DISARM );
		cmd_q_rs_insert( INPUT_AUX4 | INPUT_ON , 100 , (void*)CMD_AUX_WITH_DISARM );
	}
	
	#undef AUX4_LINK_LOCK			
	#undef AUX4_LINK_UNLOCK				
}

//--------------------------------------------------------------------------//


void dei_system_ch6_link_rs_on( void )
{
	#define AUX4_LINK_RS	(((dei_feature_get(DEI_FEAT_AUX_4_LINKING    )==DEI_AUX_LINK_TO_REMOTE_START))&&\
							  (dei_feature_get(DEI_FEAT_AUX_4_OUTPUT_TYPE)!=DEI_AUX_OUTPUT_OFF))
			
	if( AUX4_LINK_RS )
	{
		TRACE_SYS("\n\r[DEI_SYSTEM_AUX_CH6_ON - LINK RS]\n\r");
		
		cmd_q_rs_insert( INPUT_AUX4            , 100 , (void*)CMD_AUX_WITH_RS );
		cmd_q_rs_insert( INPUT_AUX4 | INPUT_ON , 100 , (void*)CMD_AUX_WITH_RS );
	}
	
	#undef AUX4_LINK_RS	
}

//--------------------------------------------------------------------------//

void dei_system_ch6_link_rs_off( void )
{
	#define AUX4_LINK_RS	(((dei_feature_get(DEI_FEAT_AUX_4_LINKING    )==DEI_AUX_LINK_TO_REMOTE_START))&&\
							  (dei_feature_get(DEI_FEAT_AUX_4_OUTPUT_TYPE)!=DEI_AUX_OUTPUT_OFF))
			
  //if( AUX4_LINK_RS && ss.ch6_status )
	if( AUX4_LINK_RS && get_ss_ch6()  )
	{
		TRACE_SYS("\n\r[DEI_SYSTEM_AUX_CH6_OFF - reset LINK RS]\n\r");
		
		cmd_q_rs_insert( INPUT_AUX4            , 100 , (void*)CMD_AUX_WITH_RS );		
	}
	
	#undef AUX4_LINK_RS	
}

//--------------------------------------------------------------------------//
