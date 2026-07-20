/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: adc_init.c 33397 2015-08-26 16:42:57Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "isr.h"
#include "wake.h"
#include "adc_private.h"
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

void adc_init (Adc_CCLK_Divider cclk_divider , UInt8 pclk_divider , Adc_Enable_Mask adc_enable_mask)
{
  CRITICAL ;

	static Boolean do_adc_init_once = TRUE ;

	if( do_adc_init_once )
	{
  init_adc_hw :
    {
      C_ENTER ;
        {
          PCONP |=   ((UInt32) 1 << 12) ;           // Enable  ADC unit
          AD0CR |=   ((UInt32) 1 << 21) ;           // Disable ADC power-down
          AD0CR &= ~ ((UInt32) 1 << 16) ;           // Software mode (not burst)

          PCLKSEL0 &= ~ ((UInt32) 3            << 24) ;
          PCLKSEL0 |=   ((UInt32) cclk_divider << 24) ; 

          pclk_divider -= 1 ;

          AD0CR &= ~ ((UInt32) 0xFF         << 8) ;
          AD0CR |=   ((UInt32) pclk_divider << 8) ;
        }
      C_LEAVE ;
    }

	}
  select_adc_pins :
    {
			
#ifdef DEV_LPC_176x
      static const Gpio_Pl_T adc_pin_setup [] =
        {
					GPIO_PL_AD0_NU,
					GPIO_PL_AD1_NU,
          GPIO_PL_AD_INP , // AD-2
          GPIO_PL_DAC_OUT , // DAC - OUT
          GPIO_PL_A_D_BATTERY , // AD-4
          GPIO_PL_A_D_TEMP_SENSOR , // AD-5
					GPIO_PL_AD6_NU,
					GPIO_PL_AD7_NU
        } ;
#else
      static const Gpio_Pl_T adc_pin_setup [] =
        {
					GPIO_PL_AD_INP,
					GPIO_PL_AD_HW_VER,
          GPIO_PL_A_D_BATTERY , // AD-2
          GPIO_PL_DAC_OUT , // DAC - OUT
          GPIO_PL_AD4_NU , // AD-4
          GPIO_PL_AD5_NU  // AD-5
        } ;
#endif
				UInt8 pin = 0 ;

      do
        {
          if (adc_enable_mask & 1)
            {
              C_ENTER ;
                {
									gpio_pl_select( adc_pin_setup[pin] );
									gpio_pl_mode( adc_pin_setup[pin] ); 
                }
              C_LEAVE ;
            }

          adc_enable_mask >>= 1 ;
        }
      while (++pin < sizeof_array (adc_pin_setup)) ;
    }

		if(do_adc_init_once)
		{
			do_adc_init_once = FALSE;
			setup_resource_management :
				{
					adc_res = os_res_create () ;
				}

			setup_irq_handler :
				{
					adc_irq_done = os_irq_create () ;

					C_ENTER ;
						{
							isr_set_vector (ISR_ID_ADC_0  , ISR_PRI_15 , ISR_TYPE_IRQ , adc_irq_service) ;
						}
					C_LEAVE ;
				}

			wake_register (adc_wake) ;
	}
}

