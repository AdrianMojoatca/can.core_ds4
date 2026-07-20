/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dac_set_relay_ctl.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dac_private.h"
#include "os.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

// DAC turn on and turnoff time in milli seconds
#define	DAC_RELAY_TURN_ON_TIME		3	// Relay CPC1008N: max turn on time is 3ms
#define	DAC_RELAY_TURN_OFF_TIME		2	    // Relay CPC1008N: max turn off time is 2ms

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/** Description: Depending on the input boolean value, the DAC relay is either 
    turned ON or turned OFF. After the change in DAC relay, a settling time is 
	needed for the DAC output to reach expected value. The settling time is handled
	using os_tsk_wait().
*/

void dac_set_relay_ctl (Boolean set_or_clr)
{
	if (dac_get_relay_ctl())
	{	// DAC relay is set (ON)
		
		if (set_or_clr) return;		// If we want to turn on the DAC relay --> return, it's already on
		
		DAC_RLY_CTL_CLR;			// Clear (turn OFF) DAC relay
		
		/* settling time for DAC relay */
		os_tsk_wait (DAC_RELAY_TURN_OFF_TIME) ;
	}
	else
	{	// DAC relay is cleared (OFF)
		
		if (!set_or_clr) return;	// If we want to turn off the DAC relay --> return, it's already off
		
		DAC_RLY_CTL_SET;			// Set (turn ON) DAC relay
		
		/* settling time for DAC relay */
		os_tsk_wait (DAC_RELAY_TURN_ON_TIME) ;
	}
}
