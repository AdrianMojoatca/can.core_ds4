/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dac_set_output.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dac_private.h"

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

/** Description: Calculates the UInt16 dac_value from the input voltage requested. 
    Max input voltage that can be requested is 9v. If more than 9.0v is passed as input, 
	function returns FALSE and DAC relay is opened. 
*/
Boolean dac_set_output (double voltage)
{
	UInt16 write_value = 0 ;

	if (voltage > DAC_MAX_REQUEST)
	{	// Voltage is too high so don't output it and open the DAC relay
		dac_set_relay_ctl (FALSE) ;
		return FALSE ;
	}
	
	if (voltage < 5)	
		write_value = (UInt16)((voltage / DAC_MAX_WIRE_VOLTAGE) * DAC_REG_MAX_VALUE) ;
	else
		write_value = (UInt16)((voltage / 10.0) * DAC_REG_MAX_VALUE) ;

 	return dac_wr_value (write_value, FALSE) ;
}



