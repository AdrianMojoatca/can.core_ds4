/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_error_start.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rs_private.h"
#ifdef PLATFORM_933
#include "sensor_proto.h"
#endif
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

Dei_Rs_Evt dei_rs_error_start( void )
{		
	
#ifdef PLATFORM_CM800
	if( (dei_feature_get( DEI_FEAT_TRANSMISSION_MODE) == 1) && ( get_gearbox_type() == GEARBOX_MANUAL ) ) //supported : automatic only , selected : manual
    {
		TRACE_DEI_RS("\n\r[ DEI_ERR_RS_START_GEARBOX_NOT_SUPPORTED ]\n\r");

        return DEI_RS_EVT_MTS ;
    }

	if( (dei_feature_get( DEI_FEAT_TRANSMISSION_MODE) == 2) && ( get_gearbox_type() == GEARBOX_AUTOMATIC ) ) //supported : manual only , selected : automatic
    {
        TRACE_DEI_RS("\n\r[ DEI_ERR_RS_START_GEARBOX_NOT_SUPPORTED ]\n\r");
		
		return DEI_RS_EVT_MTS ;
    }                
    
	if( get_safety_sw_status() == SAFETY_RS_DISABLE )
    {
		TRACE_DEI_RS("\n\r[ DEI_ERR_RS_START_RS_VALET_SWITCH ]\n\r");

        return DEI_RS_EVT_NEUTRAL;
    }
#else  //PLATFORM_933
    //Idle/Pitstop option		
  if(get_ign_status() && (dei_feature_get(DEI_FEAT_PITSTOP_MODE_ENABLE) == DEI_PITSTOP_MODE_OFF) && (get_gearbox_type() != GEARBOX_MANUAL))  //engine is running, 
      {
        return DEI_RS_EVT_PITSTOP_NOT_AVAIL;  //TODO: add plight flashing pattern 
      }    
	
#endif
	if( get_hood_status() )
    {
		TRACE_DEI_RS("\n\r[ DEI_ERR_RS_START_HOOD ]\n\r");

        return DEI_RS_EVT_HOOD   ; 
    }
	
  	if( get_brake_status() )
    {
		TRACE_DEI_RS("\n\r[ DEI_ERR_RS_START_BRAKE ]\n\r");

        return DEI_RS_EVT_BRAKE  ; 
    }
	
	if( !get_rs_rpm_ref() && (dei_feature_get( DEI_FEAT_ENGINE_CHECKING_MODE )==4))
    {
		TRACE_DEI_RS("\n\r[ DEI_ERR_RS_START_TACHO_NOT_PROGRAMMED ]\n\r");

        return DEI_RS_EVT_LOW_TACH ; 
    }
#ifdef PLATFORM_933
	if( sensor_missing_safety() )
	{
		TRACE_DEI_RS("\n\r[ DEI_ERR_RS_START_MISSING_SAFETY_PERIPHERAL ]\n\r");
			return DEI_RS_EVT_NEUTRAL;
	}
#endif		
	if(  get_gearbox_type() == GEARBOX_AUTOMATIC )                                                          
    {
        return DEI_RS_EVT_TIMEOUT ;
    }
        
	if( (get_gearbox_type() == GEARBOX_MANUAL) && !get_handbrake_status() )
    {
		TRACE_DEI_RS("\n\r[ DEI_ERR_RS_START_HANDBRAKE ]\n\r");
        
        return DEI_RS_EVT_MTS ; 
    }
				
  	if( (get_gearbox_type() == GEARBOX_MANUAL) && (dei_feature_get( DEI_FEAT_ENGINE_CHECKING_MODE )!=4))
    {
		TRACE_DEI_RS("\n\r[ DEI_ERR_RS_START_UNIT_NOT_IN_TACHO_MODE ]\n\r");

        return DEI_RS_EVT_MTS ; 
    }
			
	if( (get_gearbox_type() == GEARBOX_MANUAL) && !get_rs_rpm_ref() )	                                       
    {
		TRACE_DEI_RS("\n\r[ DEI_ERR_RS_START_TACHO_NOT_PROGRAMMED ]\n\r");
                
        return DEI_RS_EVT_MTS ; 
    }
				  
	if( (get_gearbox_type() == GEARBOX_MANUAL)  && get_rs_mts_progress()  && ( get_rpm() > DEI_RS_RPM_MIN ) ) return DEI_RS_EVT_TIMEOUT ;
      
	if( (get_gearbox_type() == GEARBOX_MANUAL) && get_rs_mts() && (! get_door_status()|| (get_rpm() > DEI_RS_RPM_MIN)) )  return DEI_RS_EVT_TIMEOUT  ;
	

	TRACE_DEI_RS("\n\r[ DEI_ERR_RS_START_MTS_NOT_INIT ]\n\r");

	return DEI_RS_EVT_MTS ;	
}

//--------------------------------------------------------------------------//
	

