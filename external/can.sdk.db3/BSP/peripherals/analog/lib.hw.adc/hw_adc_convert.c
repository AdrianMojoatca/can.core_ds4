/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hw_adc_convert.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "hw_adc_private.h"

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

SInt16 hw_adc_convert (Adc_Channel adc_channel)
{
  CRITICAL ;
  
  C_ENTER ;
  {	
	if (! (PCONP & (1u << 12)))
		hw_adc_wake (s_WAKE) ;	// we MUST activate the ADC before usage. 
								// not doing it can cause adc to stuck in the status while loop, bugging all the soft

	AD0CR &= 0xFFFFFF00  ;	// clear last channel use for conversion
	AD0CR |= adc_channel ;	// enable the current channel

	AD0CR |= (1u << 24) ;	// start conversion

	while (!(AD0STAT & adc_channel));

	AD0CR &= ~(1u << 24) ;	// clear start conversion bit
  }
  C_LEAVE ;

	#ifdef DEV_LPC_236x
	switch (adc_channel)
	{
		case 0x01:
			return (AD0DR0 >> 6) & 0x3FF;
		case 0x02:
			return (AD0DR1 >> 6) & 0x3FF;
		case 0x04:
			return (AD0DR2 >> 6) & 0x3FF;
	}
	#else
	switch (adc_channel)
	{
		case 0x02:
			return (AD0DR1 >> 4) & 0xFFF;
		case 0x04:
			return (AD0DR2 >> 4) & 0xFFF;
	}
	#endif

	return -1;
}
