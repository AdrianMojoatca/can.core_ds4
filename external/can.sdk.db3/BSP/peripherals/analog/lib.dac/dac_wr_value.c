/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dac_wr_value.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dac.h"
#include "hw_dac.h"
#include "hw_adc.h"
#include "dbg.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define DELAY_1MS	(CPU_CLOCK_HZ / 1000)
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void dac_wr_value (UInt16 write_value, Boolean adjust)
{
 	UInt16 read_value = 0 ;
	UInt32 delay	  = 0 ;

	hw_dac_convert (write_value) ;
	
  #if DEBUG_DAC == 1
    dbg_tx_text ("1ST WRITE :") ;
    dbg_tx_h16 (write_value) ;
    dbg_tx_crlf () ;
  #endif

  if (adjust)
 {
	if (write_value < 0x200)	// middle of DAC range output is 5V so correct value below that
								// because ADC cannot convert 5V and up
	{
		// delay 1ms. not using timeout, pte, tsk wait, etc because can't do 1ms delay precise
		while (delay ++ < DELAY_1MS * 2) ;	// we need more than 1ms because of higher resistor + cap time constant

		#ifdef DEV_LPC_176x
		read_value = hw_adc_convert (CHANNEL2) ;
		#else
		read_value = hw_adc_convert (CHANNEL0) ;
		#endif
		

		 #if DEBUG_DAC == 1
		   dbg_tx_text ("READ :") ;
		   dbg_tx_h16 (read_value) ;
		   dbg_tx_crlf () ;
	#endif

		if (read_value < 16)	// protection if adc wire is not connected to dac wire , the value will be near 0
			return ;			// then this will avoid bad dac correction

		write_value -= ((read_value + 2) / 2) - write_value ;

		hw_dac_convert (write_value) ;

	     #if DEBUG_DAC == 1
		   dbg_tx_text ("2ND WRITE :") ;
		   dbg_tx_h16 (write_value) ;
		   dbg_tx_crlf () ;
	#endif
	}
 }
}
