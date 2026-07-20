/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d_wake.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2d_private.h"
#include "wake.h"
#include "gpio.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//#define D2D1_DBG_PIN

#define D2D_PIN                   gpio_pl_pin_read(GPIO_PL_D2D_RX)

#define CPU_WAKE_CLOCK_MULT        4                                       // 4 Mhz clock

#define D2D_BAUDRATE              (CPU_WAKE_CLOCK_MULT*1000000/1200)
#define CPU_LATENCY_COMP          (CPU_WAKE_CLOCK_MULT* 400)    // latency measured from DS4 with analog, ford3, and tl6
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void d2d_delay( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

void d2d_wake (Wake_Status wake_status)
{
    switch(wake_status)
    {
        case s_SLEEP:
        {
            gpio_pl_func_cfg_gpio(GPIO_PL_D2D_RX) ;

	        PCONP    &= ~(1u << 3) ;             //--- Turn off UART clock                 
        }
        break;

        case s_WAKE:
        {
			gpio_pl_select(GPIO_PL_D2D_RX);
            
            PCONP   |= (1u << 3) ;                            //--- Turn on UART clock

             if (d2d_first_byte.validity == TRUE)
             {
                d2d_rda_rx = d2d_first_byte.byte ;  // Put read data into d2d irq rx structure
                
                os_irq_set(d2d_irq_rx);             // Signal task that data has arrived
                
                d2d_first_byte.validity = FALSE ;   // Clear byte validity
            }
        }
        break;

        default : break ;
    }
}


/*--------------------------------------------------------------------------*/
static unsigned last_bit_time;
Boolean d2d_wake_ready(void)
{
    UInt8 i ;         
#ifdef D2D1_DBG_PIN
  Boolean test = TRUE;
#endif
        
    if( !D2D_PIN )
    {                
#ifdef D2D1_DBG_PIN
			  OUT_1(test);
#endif
			  T2TC += CPU_LATENCY_COMP;
			  last_bit_time = D2D_BAUDRATE/2; // To achieve sample starting at moment of 1.5 bit


        for( i = 0 ;  i < 8 ; i++ )
        {
           d2d_delay();      //--- sampling 8 bits					
            d2d_first_byte.byte >>= 1   ;
            d2d_first_byte.byte  |=((UInt8)D2D_PIN<<7);
#ifdef D2D1_DBG_PIN
					  test = !test;
					  OUT_1(test);
#endif
        }

           d2d_delay();
        //--- sampling the stop bit here         
        d2d_first_byte.validity = D2D_PIN ;
        
        d2d_first_byte.d2d_rx_swi = TRUE ; 
#ifdef D2D1_DBG_PIN
					  OUT_1(0);
#endif
				irq_user_trigger();
    }
    
    return TRUE ; 
}

/*--------------------------------------------------------------------------*/

void d2d_delay( void )
{
        while((T2TC - last_bit_time) < D2D_BAUDRATE);
		last_bit_time += D2D_BAUDRATE;
}

/*--------------------------------------------------------------------------*/

