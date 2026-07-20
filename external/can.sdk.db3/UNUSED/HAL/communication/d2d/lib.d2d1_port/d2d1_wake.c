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


#define D2D_DBG_PIN     0

#if  D2D_DBG_PIN == 1

#include "config_gpio.h"
    #define   D2D_PIN_TOGGLE()       OUT_2(TRUE);OUT_2(FALSE);
    #define   DELAY_TOGGLE_PIN       (20 * CPU_WAKE_CLOCK_MULT)           
#else
    #define   D2D_PIN_TOGGLE()       ;
    #define   DELAY_TOGGLE_PIN       0
#endif

#if DBG_JTAG == 0
	#define CPU_WAKE_CLOCK_MULT        4                                       // 4 Mhz clock
#else
	#define CPU_WAKE_CLOCK_MULT        4                                       // 72 Mhz clock
#endif

#define D2D_BAUDRATE              ((CPU_WAKE_CLOCK_MULT * 1000000) / 1200)-DELAY_TOGGLE_PIN
#define CPU_WAKEUP_DELTA          (200 * CPU_WAKE_CLOCK_MULT)              // 200 usec to wakeup cpu
#define CPU_LATENCY_COMP          ( 50 * CPU_WAKE_CLOCK_MULT)
#define D2D_FIRST_SAMPLING_POINT  (((3*D2D_BAUDRATE)/2 + CPU_LATENCY_COMP) - CPU_WAKEUP_DELTA - (T2TC-wake_tc()))-DELAY_TOGGLE_PIN  

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Boolean D2D1_PIN(UInt8 port);
static void d2d1_delay( UInt32 delay_us );
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

Boolean d2d1_wake_ready(void)
{
  UInt8 i ;
        
    if( !D2D1_PIN(wake_uart_no) )
    {                
        d2d1_delay(D2D_FIRST_SAMPLING_POINT); //---we get the start bit , wait for first bit sampling point

        D2D_PIN_TOGGLE();

        for( i = 0 ;  i < 8 ; i++ )
        {
            d2d_first_byte.byte >>= 1   ;
            d2d_first_byte.byte  |=((UInt8)D2D1_PIN(wake_uart_no)<<7);
            
            d2d1_delay(D2D_BAUDRATE);      //--- sampling 8 bits

            D2D_PIN_TOGGLE();
        }

        //--- sampling the stop bit here         
        d2d_first_byte.validity = D2D1_PIN(wake_uart_no);
        
        d2d_first_byte.d2d_rx_swi = TRUE ; 
				irq_user_trigger();
    }
    
    return TRUE ; 
}

/*--------------------------------------------------------------------------*/

void d2d1_delay( UInt32 delay_us )
{
    if(delay_us > 3*D2D_BAUDRATE/2)  //--- sort of protection added because the delay_us in case of D2D_FIRST_SAMPLING_POINT 
                                     //    is based on a wake_tc() value
                                     //    if other functions are registered to ready_for_wake list from LIB.WAKE the wake_tc() value
                                     //    is too large and D2D_FIRST_SAMPLING_POINT value is wrong computed
                                     //    We ensure that the maximum delay generated by d2d_delay() function is 3*D2D_BAUD_RATE/2
    {
        delay_us = 3*D2D_BAUDRATE/2;
    }

    
    T2TC = 0 ;    //--- we can do this because we running in sleep mode 
                  //--- and timer is not used in this case
    while(T2TC < (delay_us));
}

/*--------------------------------------------------------------------------*/

