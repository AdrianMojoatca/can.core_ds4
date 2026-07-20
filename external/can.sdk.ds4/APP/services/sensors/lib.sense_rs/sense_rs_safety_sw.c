/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs_safety_sw.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs_private.h"
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

void sense_rs_safety_sw( Boolean do_init , Boolean status )
{	
	if( do_init )      //STATUS = HIGH  : remote start disable(switch not connected or in off position )
                       //STATUS = LOW   : remote start enable(switch connected or in on position )
    {
         set_safety_sw_status( status ? SAFETY_RS_DISABLE : SAFETY_RS_ENABLE );
    }
    else
    {
        set_safety_sw_status( status ? SAFETY_RS_DISABLE : SAFETY_RS_ENABLE );
    
    	cmd_q_rs_insert( (get_safety_sw_status() == SAFETY_RS_ENABLE)  ? INPUT_PRK_NEUTRAL_STATUS_ON : INPUT_PRK_NEUTRAL_STATUS_OFF , TIMEOUT_Q_INSERT );
    		
    	TRACE_DEI_SENSE_RS("\n\r[SAFETY_SWITCH_WIRE_%s]\n\r" , (get_safety_sw_status() == SAFETY_RS_ENABLE) ? "RS_ENABLE":"RS_DISABLE" );		
    }
}

/*--------------------------------------------------------------------------*/

