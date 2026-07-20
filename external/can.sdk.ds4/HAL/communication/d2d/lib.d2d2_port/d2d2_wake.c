/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d2_wake.c  $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2d2_private.h"
#include "gpio.h"
#include "wake.h"


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

void d2d2_wake (Wake_Status wake_status)
{
  D2d2_Port *port = NULL;
	static UInt8 d2d2_port_count = 0;
    UInt8 d2d2_port_no;
    static UInt32 tmr1_start = 0 ;
    static UInt32 tmr1_cur = 0 ;


    switch(wake_status)
    {
        case s_SLEEP:
        {
					d2d2_port_count++;
					d2d2_port_no = find_port(D2D_TYPE2, d2d2_port_count );
					
					if((d2d2_port_no == UART_PORT0) | (d2d2_port_no == UART_PORT1) | (d2d2_port_no == UART_PORT3))
					{
						port = (D2d2_Port *) smsg_port[d2d2_port_no];
						ATOMIC(
						port->smsgbase.busstate = BUS_SLEEP;
						d2d_switch_port_tx(port, CHAR_ERROR);
						d2d_switch_port_tx(port, CHAR_SLEEP);
						)

            tmr1_start = T1TC;
						//os_tsk_wait(1);
            do
            {
               tmr1_cur = T1TC;
            }while ((tmr1_cur - tmr1_start) < 1*1000*72);

					}

					if(d2d2_port_no == UART_PORT0)
					{
						gpio_pl_func_cfg_gpio(GPIO_PL_D2D_RX) ;
						PCONP    &= ~(1u << 3) ;             //--- Turn off UART0 clock                 
					}
					else if(d2d2_port_no ==  UART_PORT1)
					{
						gpio_pl_func_cfg_gpio(GPIO_PL_RF_D2D_RX) ;
						PCONP    &= ~(1u << 4) ;             //--- Turn off UART1 clock                 
					}
					else if(d2d2_port_no ==  UART_PORT3)
					{
						gpio_pl_func_cfg_gpio(GPIO_PL_DBG_PORT_RX) ;
						PCONP    &= ~(1u << 25) ;             //--- Turn off UART1 clock                 
						
					}
        }
        break;

        case s_WAKE:
        {
					d2d2_port_no = find_port( D2D_TYPE2, d2d2_port_count);
					if((d2d2_port_no == UART_PORT0) || (d2d2_port_no == UART_PORT1) || (d2d2_port_no == UART_PORT3))
					{
							port = (D2d2_Port *) smsg_port[d2d2_port_no];
					}
					d2d2_port_count--;

					if(d2d2_port_no == UART_PORT0)
					{
						gpio_pl_select(GPIO_PL_D2D_RX) ;
						PCONP    |= (1u << 3) ;             //--- Turn off UART0 clock                 
					}
					else if(d2d2_port_no ==  UART_PORT1)
					{
						gpio_pl_select(GPIO_PL_RF_D2D_RX) ;
						PCONP    |= (1u << 4) ;             //--- Turn off UART1 clock                 
					}
					else if(d2d2_port_no ==  UART_PORT3)
					{
						gpio_pl_select(GPIO_PL_DBG_PORT_RX) ;
						PCONP    |= (1u << 25) ;             //--- Turn off UART1 clock                 
					}
					
					port->givechance = 1;
					
					if(wake_reason)   // don't clear this now, we have to repeat on all ports
					{
						d2d_switch_port_tx(port, wake_reason);
					}
					to_master(port);
        }
        break;

        default : break ;
    }
}


/*--------------------------------------------------------------------------*/

UInt8 find_port( UInt8 type, UInt8 num )
{
	UInt8 i, match = 0;
	for(i=0; i<4; i++)
	{
		if(uart_config[i] == type)
			match++;
		
		if(match == num)
		{
			break;
		}
	}
	return (i+1);
}
