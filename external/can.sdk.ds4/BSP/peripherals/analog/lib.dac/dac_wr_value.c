/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dac_wr_value.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dac_private.h"
#include "os.h"


/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define	MAX_DAC_VALUE_TO_CORRECT		0x200
#define DAC_N_ADC_SETTLE_TIME			3	    // in milli seconds
#define MAX_VALUE_IF_ADC_DISCONNECTED	16		// (16/1023)*5v = ~78mV
#define ADC_FILTER_SIZE                 5

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/** Description: dac_wr_value accepts uint16 input and set the DAC output using function
    dac_convert. If input is more than or equal 0x400, input is rejected and FALSE is returned.
	The function also reads back the DAC output using ADC (if dac_value is less than 0x200 ~ 5v) and
	does a correction based on value read from ADC. 
*/

Boolean dac_wr_value (UInt16 write_value, Boolean adjust) 
{
 	UInt16 read_value = 0 ;

	if(write_value >= DAC_MAX_VALUE)
	    return FALSE;

	// Write the DAC value and make sure the DAC relay is on
	dac_convert    (write_value) ;

	if(adjust)
	{
		// ADC reading is limited to 0-5V by design (translated to 0 - 3.3V on CPU pin).
		// So if DAC value is higher than 5V, it's useless to try to correct it
		if (write_value < MAX_DAC_VALUE_TO_CORRECT)
		{	
			// Thus we want to correct the DAC output based on the voltage we read from the ADC
			os_tsk_wait (DAC_N_ADC_SETTLE_TIME) ;
		
			read_value = adc_convert_min_max_avg (ADC_CHANNEL_TO_READ_DAC , ADC_FILTER_SIZE) ;

			if (read_value < MAX_VALUE_IF_ADC_DISCONNECTED)	// protection if adc wire is not connected to dac wire , the value will be near 0
				return TRUE;			// then this will avoid bad dac correction
	
			write_value -= ((read_value + 2) / 2) - write_value ;
	
			dac_convert (write_value) ;
	
		}
	}
	return TRUE;	
}
