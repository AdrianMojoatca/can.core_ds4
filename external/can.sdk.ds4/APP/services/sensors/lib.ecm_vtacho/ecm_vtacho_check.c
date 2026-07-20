/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: ecm_vtacho_check.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "ecm_vtacho_private.h"
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

Ecm_Vtacho_Evt ecm_vtacho_check( void )
{
	if( ecm_vtacho_vars.time ++  == ECM_VATCHO_TIMEOUT_CHECK_RUNNING )
	{
		ecm_vtacho_vars.time = 0 ;
		
		if(!ecm_vtacho_vars.running )
        {
            if( get_voltage() >= (ecm_vtacho_vars.v_ref + ECM_VTACHO_VOLTAGE_RUNNING_DIFF) )
    		{							
    			if( ecm_vtacho_vars.ref == ECM_VTACHO_REF_NOT_PROGRAMMED )
    			{
    				ecm_vtacho_vars.ref = (Ecm_Vtacho_Ref)ecm_vtacho_vars.retry_count ;
    
#if DEBUG_ECM_TACHO == 1
    				TRACE_DEI_ECM("[ECM_VTACHO_REF_PROGRAMMED : %d ]\r" , ecm_vtacho_vars.ref );	
            ecm_vtacho_trace(ecm_vtacho_vars.ref);
#endif
    			}
    
    
    			//TRACE_DEI_ECM("[ECM_VTACHO_VOLTAGE_ENGINE_RUNNING1 : %d ]\n\r" , (UInt8)get_voltage() );	//use accurate ecm_vtacho_trace
#if DEBUG_ECM_TACHO == 1
					TRACE_DEI_ECM("ECM_VTACHO_VOLTAGE_ENGINE_RUNNING1:\r");
					ecm_vtacho_trace(get_voltage());
#endif
    			
    			return ECM_VTACHO_EVT_ENGINE_RUNNING ; 
    		}
            
        //TRACE_DEI_ECM("[ECM_VTACHO_VOLTAGE_ENGINE_ENGINE_STOPPED1 : %d ]\n\r" , (UInt8)get_voltage() );	//use accurate ecm_vtacho_trace
#if DEBUG_ECM_TACHO == 1
				TRACE_DEI_ECM("ECM_VTACHO_VOLTAGE_ENGINE_ENGINE_STOPPED1 :\r");
        ecm_vtacho_trace(get_voltage());
#endif

		    ecm_vtacho_vars.retry_count ++ ;
		    ecm_vtacho_vars.time = 0 ;
		
		    return ECM_VTACHO_EVT_ENGINE_STOPPED ;
        }
        else
        {
            if( get_voltage() >= (ecm_vtacho_vars.v_ref - ECM_VTACHO_VOLTAGE_RUNNING_DIFF) )
    		{							
    			//TRACE_DEI_ECM("[ECM_VTACHO_VOLTAGE_ENGINE_RUNNING2 : %d ]\n\r" , (UInt8)get_voltage() );	//use accurate ecm_vtacho_trace
#if DEBUG_ECM_TACHO == 1
					TRACE_DEI_ECM("ECM_VTACHO_VOLTAGE_ENGINE_RUNNING2 :\r");
          ecm_vtacho_trace(get_voltage());
#endif
    			
    			return ECM_VTACHO_EVT_ENGINE_RUNNING ; 
    		}
            
            //TRACE_DEI_ECM("[ECM_VTACHO_VOLTAGE_ENGINE_ENGINE_STOPPED2 : %d ]\n\r" , (UInt8)get_voltage() );	//use accurate ecm_vtacho_trace
#if DEBUG_ECM_TACHO == 1
				TRACE_DEI_ECM("ECM_VTACHO_VOLTAGE_ENGINE_ENGINE_STOPPED2 :\r");
        ecm_vtacho_trace(get_voltage());
#endif

		    ecm_vtacho_vars.retry_count ++ ;
		    ecm_vtacho_vars.time = 0 ;
		
		    return ECM_VTACHO_EVT_ENGINE_STOPPED ;            
        }		
	}

	return ECM_VTACHO_EVT_NULL ; 
}

/*--------------------------------------------------------------------------*/
