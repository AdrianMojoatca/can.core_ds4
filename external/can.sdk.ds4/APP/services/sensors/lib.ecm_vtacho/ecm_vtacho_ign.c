/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: ecm_vtacho_ign.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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


Ecm_Vtacho_Evt ecm_vtacho_ign( void )
{
	if( ecm_vtacho_vars.time ++ == ECT_VTACHO_TIMEOUT_READ_VREF )
	{
		ecm_vtacho_vars.v_ref = get_voltage() ; 

		//TRACE_DEI_ECM("[ECM_VTACHO_VREF_VOLTAGE : %d[V] ]\n\r" , (UInt8)get_voltage() );	//use accurate ecm_vtacho_trace
#if DEBUG_ECM_TACHO == 1
				TRACE_DEI_ECM("ECM_VTACHO_VREF_VOLTAGE[V]:\r");
        ecm_vtacho_trace(get_voltage());
#endif		
		
		if( ecm_vtacho_vars.running )
        {
					  os_tsk_wait(200);   //avoid glitch on VBAT_IN
            ecm_vtacho_vars.time  = 0 ; 
            ecm_vtacho_vars.state = ecm_vtacho_check ;
        }
        else
        {                        
            ecm_vtacho_vars.state = NULL;	
        }
	} 

	return ECM_VTACHO_EVT_NULL ; 
}

/*--------------------------------------------------------------------------*/

void ecm_vtacho_vign_on( void )
{
    ecm_vtacho_vars.v_ign_on = get_voltage();
	
#if DEBUG_ECM_TACHO == 1
				TRACE_DEI_ECM("ECM_VTACHO_V_IGN_ON_VOLTAGE_REF[V]:\r");
        ecm_vtacho_trace(get_voltage());
#endif
}

/*--------------------------------------------------------------------------*/

