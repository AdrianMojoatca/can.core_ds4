/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hw_adc_init.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "hw_adc_private.h"
#include "gpio.h"

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

void hw_adc_init (void)
{	
	CRITICAL ;
		
	C_ENTER ;			    
	{
		gpio_pl_mode(GPIO_PL_A_D_INP);
		gpio_pl_select( GPIO_PL_A_D_INP);

		gpio_pl_mode(GPIO_PL_A_D_BATTERY);
		gpio_pl_select( GPIO_PL_A_D_BATTERY);
		
#ifdef DEV_LPC_236x	
		gpio_pl_mode(GPIO_PL_HW_VERSION);
		gpio_pl_select( GPIO_PL_HW_VERSION);
#endif
		
/*		
		PINMODE1 = (PINMODE1 & ~(3u << 14)) | (2u << 14) ;       // Disable P0.23 PULL UP-DOWN
		PINMODE1 = (PINMODE1 & ~(3u << 16)) | (2u << 16) ;       // Disable P0.24 PULL UP-DOWN
		PINMODE1 = (PINMODE1 & ~(3u << 18)) | (2u << 18) ;       // Disable P0.25 PULL UP-DOWN

		PINSEL1  = (PINSEL1  & ~(3u << 14)) | (1u << 14) ;       // Configure P0.23 as AD0.0
		PINSEL1  = (PINSEL1  & ~(3u << 16)) | (1u << 16) ;       // Configure P0.24 as AD0.1
		PINSEL1  = (PINSEL1  & ~(3u << 18)) | (1u << 18) ;       // Configure P0.25 as AD0.2
*/
		PCONP |= (1u << 12) ;       // Enable ADC clock

		AD0INTEN = 0 ;

        #ifdef	DEV_LPC_236x	
		AD0DR0	 = 0 ;
		AD0DR1	 = 0 ;
		AD0DR2	 = 0 ;
        #endif

		// Clock divider to bring ADC clock to 1MHz
		// if the freq is too high, I had bad data like 000, 200, 100
		AD0CR  = 0x00000400 ;		
		AD0CR |= (1u << 21) ;		// Enable ADC converter	
	}
	C_LEAVE ;

	wake_register (hw_adc_wake) ;
}
