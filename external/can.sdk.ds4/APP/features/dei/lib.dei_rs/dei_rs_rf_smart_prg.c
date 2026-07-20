/*==========================================================================*/
// $Id: dei_rs_rf_smart_prg.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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

/*--------------------------------------------------------------------------*/

void dei_rs_rf_smart_prg_set( void *remote )
{        
    UInt8 temp_low =0 , temp_high = 0 ;
	
	if( !get_temp_sensor_present() )
	{
		notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_FEATURE_NA );
	
		return ;
	}
	
	temp_high  = ((Cmd_Rf_Pld*)cmd_get_pld(remote))->bits.pld&0x0F;
    temp_low   = (((Cmd_Rf_Pld*)cmd_get_pld(remote))->bits.pld>>4)&0x0F ;                        
	
	    
    notify_send( NOTIFY_LOW_TEMP_UPDATE  , temp_low  ) ; 
    notify_send( NOTIFY_HIGH_TEMP_UPDATE , temp_high ) ;

	dei_feature_set( DEI_FEAT_SMART_START_LOW_TEMP  , temp_low + 1 );
	dei_feature_set( DEI_FEAT_SMART_START_HIGH_TEMP , temp_high+ 1 );
		 
    notify_send( NOTIFY_TEMP_ADJUST_ENTER ) ; 

	TRACE_DEI_RS("\n\r[ DEI_RS_SMART_PRG_SET LOW_TEMP %d  HIGH_TEMP  %d ]\n\r"  ,\
								dei_feature_get(DEI_FEAT_SMART_START_LOW_TEMP),\
								dei_feature_get(DEI_FEAT_SMART_START_HIGH_TEMP)) ; 
}

/*--------------------------------------------------------------------------*/

void dei_rs_rf_smart_prg_enter( void *remote )
{			
	//if( !temperature.bit.in_sensor_present )
	if( !get_temp_sensor_present() )
	{
		notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_FEATURE_NA );
	
		return ;
	}

	
	notify_send( NOTIFY_LOW_TEMP_UPDATE  , dei_feature_get( DEI_FEAT_SMART_START_LOW_TEMP  )-1) ; 
    notify_send( NOTIFY_HIGH_TEMP_UPDATE , dei_feature_get( DEI_FEAT_SMART_START_HIGH_TEMP )-1) ; 
    
	notify_send( NOTIFY_TEMP_ADJUST_ENTER ) ; 

	TRACE_DEI_RS("\n\r[ DEI_RS_SMART_PRG_ENTER LOW_TEMP %d  HIGH_TEMP  %d ]\n\r"  ,\
								dei_feature_get(DEI_FEAT_SMART_START_LOW_TEMP),\
								dei_feature_get(DEI_FEAT_SMART_START_HIGH_TEMP)) ; 
}
	
/*--------------------------------------------------------------------------*/		

