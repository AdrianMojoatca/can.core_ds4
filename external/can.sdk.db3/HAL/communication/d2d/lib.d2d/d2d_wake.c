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
#include "core_contract_db3_d2d_gpio.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define D2D_DBG_PIN     0

#if  D2D_DBG_PIN == 1

    #define   D2D_PIN_TOGGLE()       do { if(++toggle & 1) NEG_OUT3_SET; else NEG_OUT3_CLR; } while(0)
    #define   DELAY_TOGGLE_PIN       (20 * CPU_WAKE_CLOCK_MULT)           
#else
    #define   D2D_PIN_TOGGLE()       ;
    #define   DELAY_TOGGLE_PIN       0
#endif

#ifdef PLATFORM_404
    #define D2D_PIN                   core_contract_db3_d2d_rx_read(TRUE)
#else
    #define D2D_PIN                   core_contract_db3_d2d_rx_read(FALSE)
#endif

#define CPU_WAKE_CLOCK_MULT        4                                       // 4 Mhz clock
#define D2D_BAUDRATE              (((CPU_WAKE_CLOCK_MULT * 1000000) / 1200))
#define CPU_WAKEUP_DELTA          (200 * CPU_WAKE_CLOCK_MULT)              // 200 usec to wakeup cpu
#define CPU_LATENCY_COMP          ( 50 * CPU_WAKE_CLOCK_MULT)
#define D2D_FIRST_SAMPLING_POINT  (((3*D2D_BAUDRATE)/2 - CPU_LATENCY_COMP) - CPU_WAKEUP_DELTA - (T2TC-wake_tc()))-DELAY_TOGGLE_PIN  

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void d2d_delay( UInt32 delay_us );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

UInt8 d2d_echo_idx, d2d_echo_cnt;
UInt8 d2d_echo_buf[D2D_ECHO_SIZE];

UInt8 toggle = 0;
static UInt32 last_bit_time = 0 ;


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
                        core_contract_db3_d2d_sleep_gpio_mode();

						PCONP    &= ~(1u << 3) ;             //--- Turn off UART clock                 
        }
        break;

        case s_WAKE:
        {
                                                core_contract_db3_d2d_select_main_uart_pins();
           
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

void d2dtxpin(UInt8 val)
{
    core_contract_db3_d2d_tx_write(val);
}

#define OVERSAMPLE_RATE 8  // when checking for next start bit
Boolean d2d_wake_ready(void)
{
    UInt8 i,echo ;    
	
	  d2d_echo_idx = 0;
    d2d_echo_cnt = 0;    
	  d2d_first_byte.validity = 0;
    if( !D2D_PIN )
    {
        last_bit_time = T2TC;			
        d2d_delay(D2D_FIRST_SAMPLING_POINT); //---we get the start bit , wait for first bit sampling point

			for(;;)
			{
        D2D_PIN_TOGGLE();

        for( i = 0 ;  i < 8 ; i++ )
        {
            d2d_first_byte.byte >>= 1   ;
            d2d_first_byte.byte  |=((UInt8)D2D_PIN<<7);
					  if(d2d_first_byte.validity)
						{
							d2dtxpin(echo & 1);  // echo next bit
							echo>>=1;
						}
            
            d2d_delay(D2D_BAUDRATE);      //--- sampling 8 bits

            D2D_PIN_TOGGLE();
        }

				if(d2d_first_byte.validity)
					d2dtxpin(1);  // write my own stop bit
				
        //--- sampling the stop bit here         
        d2d_first_byte.validity = D2D_PIN ;  // remember we have a character here
				echo = d2d_first_byte.byte;          // save for potential echo
				
				// check 1 more bit time in case another start bit arrives
				for(i=0;i<OVERSAMPLE_RATE;i++)
				{
					d2d_delay(D2D_BAUDRATE/OVERSAMPLE_RATE);
					D2D_PIN_TOGGLE();
					if(!D2D_PIN) // new starrt bit started?
						break;
				}
				if(i==OVERSAMPLE_RATE)  // line was continuously hi the whole time?
				{
					// everything in d2d_echo_cnt waa already echoed.
					// the LAST byte of the string is in d2d_first_byte.byte
					break; // main module stopped talking
				}
				// we are at first edge of start bit
				d2d_delay(D2D_BAUDRATE/2 - (D2D_BAUDRATE/OVERSAMPLE_RATE)/2);  // delay to somewhere near the half-bit
				
				//begin by sending our own start bit now
				if(d2d_first_byte.validity)
				{
					d2dtxpin(0);
					d2d_echo_buf[d2d_echo_cnt++] = echo;  // this is a byte we will echo
				}
				d2d_delay(D2D_BAUDRATE);
			}
        
        d2d_first_byte.d2d_rx_swi = TRUE ; 
				irq_user_trigger();
    }
    
    return TRUE ; 
}

/*--------------------------------------------------------------------------*/

void d2d_delay( UInt32 delay_ticks )
{
    if(delay_ticks > 3*D2D_BAUDRATE/2)  //--- sort of protection added because the delay_us in case of D2D_FIRST_SAMPLING_POINT 
                                     //    is based on a wake_tc() value
                                     //    if other functions are registered to ready_for_wake list from LIB.WAKE the wake_tc() value
                                     //    is too large and D2D_FIRST_SAMPLING_POINT value is wrong computed
                                     //    We ensure that the maximum delay generated by d2d_delay() function is 3*D2D_BAUD_RATE/2
    {
        delay_ticks = 3*D2D_BAUDRATE/2;
    }

    
    
    while((T2TC - last_bit_time) < (delay_ticks));
		
		last_bit_time += delay_ticks;
		
}

/*--------------------------------------------------------------------------*/

