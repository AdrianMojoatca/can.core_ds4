/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: adc_convert_raw.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "isr.h"
#include "adc_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/


#define	ADC_CHANNEL_SETTLING_TIME	1	//1 ms

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

UInt16 adc_convert_raw (Adc_Channel adc_channel)
{
  UInt32 selected_channel_mask =  ((UInt32) 1    << adc_channel) ;
  
  CRITICAL ;

  if (selected_channel_mask != AD0CR & 0xFF)
  {
	if (! (PCONP & (1u << 12)))
		adc_wake (s_WAKE) ;	// we MUST activate the ADC before usage. 	
	AD0CR &= ~ ((UInt32) 0xFF << 0)           ;  // Select a single channel
	AD0CR |=  selected_channel_mask;
	
    os_tsk_wait(ADC_CHANNEL_SETTLING_TIME) ;
  }
  
  adc_channel_selected = adc_channel ;         // Let IRQ handler know which channel is selected
  
  C_ENTER ;
    {
      AD0INTEN |= ((UInt32) 1 <<  8) ;         // Enable global ADC interrupt
  
      AD0CR    |= ((UInt32) 1 << 24) ;         // Start conversion
  
      os_irq_wait (adc_irq_done , TRUE , 0) ;
    }
  C_LEAVE ;

		#ifdef DEV_LPC_176x
			return ((UInt16) (& AD0DR0) [adc_channel] >> 4 &  (UInt16) 0xFFF);
		#else
			return (UInt16) (& AD0DR0) [adc_channel] >> 6 & (UInt16) 0x3FF ;
		#endif
}

