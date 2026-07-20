/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: ecm_vtacho_start.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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
static Ecm_Vtacho_Evt ecm_vtacho_check_for_voltage_min( void );
static void 		  ecm_vtacho_compute_cranking_time( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

Ecm_Vtacho_Evt ecm_vtacho_start( void )
{
	ecm_vtacho_vars.v_min     = get_voltage() ; 
	ecm_vtacho_vars.time      = 0;
	
	ecm_vtacho_vars.state = ecm_vtacho_check_for_voltage_min ;
	
	TRACE_DEI_ECM("[ECM_VTACHO_VMIN_VOLTAGE : %d [V} ]\n\r" , (UInt8)get_voltage() );	
#if DEBUG_ECM_TACHO == 1
        ecm_vtacho_trace(get_voltage());
#endif 
     
	voltage_set_sampling_period( 1 );	//10 ms sampling period

	return ECM_VTACHO_EVT_NULL ; 
} 

/*--------------------------------------------------------------------------*/

Ecm_Vtacho_Evt ecm_vtacho_check_for_voltage_min( void )
{
	if( get_voltage() < ecm_vtacho_vars.v_min )
	{
		ecm_vtacho_vars.v_min = get_voltage() ; 
		
		if( ecm_vtacho_vars.time != ECM_VTACHO_VMIN_COUNT )
		{
			ecm_vtacho_vars.time  ++ ;
		}		
	}
	else
	{
		if( get_voltage() > (ecm_vtacho_vars.v_min + ECM_VTACHO_VOLTAGE_CRANKING_DIFF ) )
		{
			if( ecm_vtacho_vars.time == ECM_VTACHO_VMIN_COUNT )
			{
				ecm_vtacho_vars.state = ecm_vtacho_delay ;
				
				ecm_vtacho_compute_cranking_time();

				ecm_vtacho_vars.time = 0 ;

				TRACE_DEI_ECM("[ECM_VTACHO_VOLTAGE_MIN_FOUND : %d[V] ]\n\r" , (UInt8)ecm_vtacho_vars.v_min );	
			}
		}
	} 											
	
	return ECM_VTACHO_EVT_NULL ; 
} 

/*--------------------------------------------------------------------------*/

void ecm_vtacho_compute_cranking_time( void )
{						
	if( ecm_vtacho_vars.ref == ECM_VTACHO_REF_NOT_PROGRAMMED )
	{
		/*cranking time when vtacho is not programmed */
		
		ecm_vtacho_vars.cranking_time = ECM_VTACHO_CRANKING_TIME_MIN + ecm_vtacho_vars.retry_count * ECM_VTACHO_CRANKING_TIME_STEP ;
	}
	else
	{
		/*cranking time when vtacho is programmed */

		ecm_vtacho_vars.cranking_time = ECM_VTACHO_CRANKING_TIME_MIN + ((ecm_vtacho_vars.ref + ecm_vtacho_vars.retry_count) * ECM_VTACHO_CRANKING_TIME_STEP); 
	
		if( ecm_vtacho_vars.fine_tune < ECM_VTACHO_FINE_TUNE_DISABLE )
		{
			ecm_vtacho_vars.cranking_time = ecm_vtacho_vars.cranking_time - ecm_vtacho_vars.fine_tune*ECM_VTACHO_CRANKING_TIME_ADJUST ;
		}
		
		if( ecm_vtacho_vars.fine_tune >  ECM_VTACHO_FINE_TUNE_DISABLE )
		{
			ecm_vtacho_vars.cranking_time = ecm_vtacho_vars.cranking_time + (ecm_vtacho_vars.fine_tune-ECM_VTACHO_FINE_TUNE_DISABLE)*ECM_VTACHO_CRANKING_TIME_ADJUST ;
		}
			
	}

	TRACE_DEI_ECM("\n\r[ECM_VTACHO_CRANKING_TIME %d [ms] ]\n\r" , ecm_vtacho_vars.cranking_time * 10);
}

/*--------------------------------------------------------------------------*/

