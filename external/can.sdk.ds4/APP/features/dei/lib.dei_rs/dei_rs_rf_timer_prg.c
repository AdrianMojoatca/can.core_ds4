/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_rf_timer_prg.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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

void dei_rs_rf_timer_prg_adjust( void *remote )
{		
	UInt8 start , interval ; 
	
	start       = ((Cmd_Rf_Pld*)(cmd_get_pld(remote)))->bits.pld&0x0F;
    interval    = (((Cmd_Rf_Pld*)(cmd_get_pld(remote)))->bits.pld>>4)&0x0F;

    TRACE_DEI_RS("\n\r[ DEI_RS : TIMER MODE ADJUST PUT STARTS %02d   INTERVAL %02d* ]\n\r" , start , interval) ; 

	dei_feature_set( DEI_FEAT_TIMER_MODE_START     , start   );
	dei_feature_set( DEI_FEAT_TIMER_MODE_INTERVAL  , interval);

  //if( rs.timer_on    && ss.locked )
	if( get_rs_timer() && get_ss_lock() )
	{
		dei_rs_timer_stop(); 
			
		dei_rs_timer_start();
	}
    
    notify_send( NOTIFY_TIMER_SET_START_COUNT  , start     ) ; 
    notify_send( NOTIFY_TIMER_SET_TIME_START   , interval  ) ; 

    notify_send( NOTIFY_TIMER_ADJUST_ENTER ) ; 
}


//--------------------------------------------------------------------------//

void dei_rs_rf_timer_prg_enter( void *remote )
{
	UInt8 start , interval ; 

	start    = dei_feature_get( DEI_FEAT_TIMER_MODE_START    ); 
	interval = dei_feature_get( DEI_FEAT_TIMER_MODE_INTERVAL ); 
		
	TRACE_DEI_RS("\n\r[ DEI_RS : TIMER MODE ADJUST ENTER STARTS %02d   INTERVAL %02d* ]\n\r" , start , interval) ; 
	
    
    notify_send( NOTIFY_TIMER_SET_START_COUNT  , start     ) ; 
    notify_send( NOTIFY_TIMER_SET_TIME_START   , interval  ) ; 

    notify_send( NOTIFY_TIMER_ADJUST_ENTER ) ; 	
}

//--------------------------------------------------------------------------//

