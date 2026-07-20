/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: tach_init.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "tach_private.h"
#include "timeout_f.h"
#include "core_contract_db3_tach_gpio.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/


#define TACH_TIMEOUT 50

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void tach_init_pwm(void) ;
Boolean tach_init_opt(UInt32 timeout);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void tach_init (void)
{	
  tach_init_opt(TACH_TIMEOUT) ;
}
//--------------------------------------------------------------------------//
Boolean tach_init_opt(UInt32 timeout)
{
  if(timeout != NULL)  
    {
      tach_vars.timeout = timeout_f_create(TACH_TIMEOUT, tach_update_timeout);
	
      tach_init_pwm() ;

      wake_register (tach_wake) ;
	
      tach_cmd_lut_ena () ;
      
      #if DBG_TACH
	{	
          UInt8 tmp[2]  = {TACH_TIMEOUT >> 8 , TACH_TIMEOUT};
          
          tach_dbg_print("\nTach Init Timeout:" , &tmp[0] , 2) ;
        }
        
      #endif      
      
      return (!!tach_vars.timeout) ;
    }

  return FALSE ;
}
//--------------------------------------------------------------------------//
static void tach_init_pwm(void)
{
	UInt32 chan_num;
  tach_vars.is_enabled = FALSE ;

  /// Configure PWM 1 and its timer ///
  
		core_contract_db3_tach_pwm_select();

		PCONP |= (1u << 6) ;		// Power PWM1

		PCLKSEL0 &= ~(3u << 12) ;
		PCLKSEL0 |=  (1u << 12) ;	// Divide CPU clock by 1

		PWM1PR   = 0x00000007 ;		// Set prescaler to divide by 8

		PWM1MCR |= 0x002 ;			// On match 0, reset timer counter
		
		PWM1MR0  = 0x01  ;			// Init MR0 with low value with no MR2 value so no pulse on PWM, otherwise it is not working
	
	#ifdef DEV_LPC_236x
		PWM1MR2  = 0x00  ;
		PWM1LER |= 0x05  ;
	#else
		PWM1MR3 = 0x00;
		PWM1LER |= 0x09  ;
	#endif
	
	
		//PWM1PCR |= 0x404 ;			// Enabled PWM1[2] output and put it single edge.

		chan_num = core_contract_db3_tach_pwm_channel();
		PWM1PCR |= (1u<<chan_num); // select correct channel number
		PWM1PCR |= (0x100 << chan_num); // enable correct output channel 
		

		PWM1TCR |= 0x09 ;			// Enable the PWM MODE. OUTPUT IS NOT ACTIVATED YET. IT WILL IN TACH UPDATE
  
	}
