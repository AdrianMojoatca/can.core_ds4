/*==========================================================================*/
// $Id: adc_wake.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "adc_private.h"

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

void adc_wake (Wake_Status wake_status)
{
	if (wake_status == s_WAKE)
	{
		PCONP |= (1u << 12) ;       // Enable ADC clock

		// Clock divider to bring ADC clock to 1MHz
		// if the freq is too high, I had bad data like 000, 200, 100
		// if this line is not present i have a lag in the value ...
		// ...convert to last value even if the applied voltage has changed
#ifdef DEV_LPC_236x
		AD0CR  = 0x00000400 ;	
#endif	  
 	    AD0CR |= (1u << 21) ;		// Enable ADC converter
	}
	else  			// SLEEP
	{
		AD0CR &= ~(7<<24);
		AD0CR &= ~(1u << 21) ;		// Disable ADC converter
		PCONP &= ~(1u << 12) ;  	// Disable ADC clock
#ifdef DEV_LPC_236x
		AD0CR &= ~((UInt32) 0xFF << 0);  // Select no channel	
#endif
	}
}

