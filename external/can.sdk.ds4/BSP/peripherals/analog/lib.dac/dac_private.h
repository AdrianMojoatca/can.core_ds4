/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dac_private.h 33268 2015-08-19 15:40:18Z suchita.yadav $
/*==========================================================================*/

#ifndef __DAC_PRIVATE_H__
#define __DAC_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
 
#include "dac.h"
#include "config_gpio.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define	DAC_BITS_RESOLUTION 		10				// DAC's number of bits of resolution
#define	DAC_REG_MAX_VALUE			(((UInt16)1 << (DAC_BITS_RESOLUTION))-1u) // Maximum value that we can put in the DAC converter register

#define	DAC_VDD_REF					3.3				// Voltage put by design as the Vref
#define	DAC_DESIGN_MULTIPLIER		3				// Design's multiply factor to output on the wire
#define	DAC_MAX_WIRE_VOLTAGE		9.9				// DAC_VDD_REF * DAC_DESIGN_MULTIPLIER --> Maximum voltage we can output on the wire
#define DAC_MAX_REQUEST             9.0             // Maximum value reached by DAC output was 9V, as measured on oscilloscope

#ifdef DEV_LPC_236x
	#define	ADC_CHANNEL_TO_READ_DAC		ADC_CHANNEL_0	// ADC channel through which we read the DAC output
#else
	#define	ADC_CHANNEL_TO_READ_DAC		ADC_CHANNEL_2	// ADC channel through which we read the DAC output
#endif	

#define DAC_MAX_VALUE               0x400

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void    dac_set_relay_ctl (Boolean set_or_clr) ;
Boolean dac_get_relay_ctl (void)               ;
void    dac_convert    (UInt16 dac_value)   ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


