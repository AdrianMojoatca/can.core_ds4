/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d_wake.c 32240 2015-06-12 12:31:31Z simon.demers $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2d1_private.h"
#include "wake.h"
#include "config_gpio.h"
#include "irq_user.h"
#include "d2d2.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//#define D2D1_DBG_PIN

#define CPU_WAKE_CLOCK_MULT        4                                       // 4 Mhz clock

#define D2D_BAUDRATE              (CPU_WAKE_CLOCK_MULT*1000000/1200)
#define CPU_LATENCY_COMP          (CPU_WAKE_CLOCK_MULT* 400)    // latency measured from DS4 with analog, ford3, and tl6
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Boolean D2D1_PIN(UInt8 port);
static void d2d1_delay( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean D2D1_PIN(UInt8 port)
{
	Boolean rd_val;
	
   if(port == 3)
		 rd_val = gpio_pl_pin_read(GPIO_PL_UART3_WAKE);
	 else if(port == 2)
		 rd_val = gpio_pl_pin_read(GPIO_PL_RF_D2D_RX);
	 else
		 rd_val = gpio_pl_pin_read(GPIO_PL_D2D_RX);
	 
	 return rd_val;
}

/*--------------------------------------------------------------------------*/

void d2d1_wake (Wake_Status wake_status)
{
  struct s_Smsg_Port *port = NULL;
	static UInt8 d2d1_port_count = 0;
	UInt8 i;

	switch(wake_status)
    {
        case s_SLEEP:
        {
					d2d1_port_count++;
					d2d1_port_no = find_port(D2D_TYPE1,d2d1_port_count);

					if(d2d1_port_no == UART_PORT0)
					{
						gpio_pl_func_cfg_gpio(GPIO_PL_D2D_RX) ;
						PCONP    &= ~(1u << 3) ;             //--- Turn off UART0 clock                 
					}
					else if(d2d1_port_no ==  UART_PORT1)
					{
						gpio_pl_func_cfg_gpio(GPIO_PL_RF_D2D_RX) ;
						PCONP    &= ~(1u << 4) ;             //--- Turn off UART1 clock                 
					}
					else if(d2d1_port_no ==  UART_PORT3)
					{
						gpio_pl_func_cfg_gpio(GPIO_PL_DBG_PORT_RX) ;
						PCONP    &= ~(1u << 25) ;             //--- Turn off UART3 clock                 
					}
        }
        break;

        case s_WAKE:
        {
					d2d1_port_no = find_port(D2D_TYPE1, d2d1_port_count);
						
					d2d1_port_count--;
						
					if ((d2d1_port_no == UART_PORT0) || (d2d1_port_no == UART_PORT1) || (d2d1_port_no == UART_PORT3))
						port = smsg_port[d2d1_port_no];


					if(d2d1_port_no == UART_PORT0)
					{
						gpio_pl_select(GPIO_PL_D2D_RX) ;
						PCONP    |= (1u << 3) ;             //--- Turn off UART0 clock                 
					}
					else if(d2d1_port_no ==  UART_PORT1)
					{
						gpio_pl_select(GPIO_PL_RF_D2D_RX) ;
						PCONP    |= (1u << 4) ;             //--- Turn off UART1 clock                 
					}
					else if(d2d1_port_no ==  UART_PORT3)
					{
						gpio_pl_select(GPIO_PL_DBG_PORT_RX) ;
						PCONP    |= (1u << 25) ;             //--- Turn off UART1 clock                 
					}

					if ((d2d_first_byte.validity == TRUE) & (d2d1_port_no == wake_uart_no))
						{
							d2d_rda_rx = d2d_first_byte.byte ;  // Put read data into d2d irq rx structure
							
							(* port->rxirq)(port, d2d_rda_rx);   // receive data available

							d2d_first_byte.validity = FALSE ;   // Clear byte validity
						}
        }
        break;

        default : break ;
    }
}

/*--------------------------------------------------------------------------*/
static unsigned last_bit_time;
Boolean d2d1_wake_ready(void)
{
  UInt8 i ;
#ifdef D2D1_DBG_PIN
  Boolean test = TRUE;
#endif
	
    if( (!D2D1_PIN(wake_uart_no)) &&  (wake_uart_no == d2d1_port_no) )
    {                
#ifdef D2D1_DBG_PIN
			  OUT_1(test);
#endif
			  T2TC += CPU_LATENCY_COMP;
			  last_bit_time = D2D_BAUDRATE/2; // To achieve sample starting at moment of 1.5 bit
			 
        for( i = 0 ;  i < 8 ; i++ )
        {
            d2d1_delay();      //--- sampling 8 bits					
			
            d2d_first_byte.byte >>= 1   ;
            d2d_first_byte.byte  |=((UInt8)D2D1_PIN(wake_uart_no)<<7);
#ifdef D2D1_DBG_PIN
					  test = !test;
					  OUT_1(test);
#endif
        }

				d2d1_delay();
        //--- sampling the stop bit here         
        d2d_first_byte.validity = D2D1_PIN(wake_uart_no);
        
        d2d_first_byte.d2d_rx_swi = TRUE ; 
#ifdef D2D1_DBG_PIN
				OUT_1(0);
#endif
				irq_user_trigger();
    }
    
	   return TRUE ; 
}

/*--------------------------------------------------------------------------*/

void d2d1_delay(void)
{
    while((T2TC - last_bit_time) < D2D_BAUDRATE);
		last_bit_time += D2D_BAUDRATE;
}

/*--------------------------------------------------------------------------*/

