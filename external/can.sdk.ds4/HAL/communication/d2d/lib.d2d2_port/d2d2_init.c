/*! 
 * @file   d2d2_init.c
 * @brief  
 * @author  DIRECTED ELECTRONICS
 * @date   7/20/2015
 */

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2d2_private.h"
#include "isr.h"

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
void d2d2_init (UInt8 port_no)
{
    UInt8 i;
    D2d2_Port *port;

    //init physical port    
    if(port_no == UART_PORT0)  //under dash port
    {
        gpio_pl_func_cfg_gpio(GPIO_PL_D2D_TX) ;
        PCONP    &= ~(1u << 3) ;             //--- Turn off UART0 clock                 
        OUT_TX_D2D(0);       //long break to exit maintenance mode
        os_tsk_wait(10);
        OUT_TX_D2D(1);
        os_tsk_wait(10);
        PCONP   |= (1u << 3) ;     // Turn on UART0 clock
    }
    else if(port_no == UART_PORT1)
    {
        gpio_pl_func_cfg_gpio(GPIO_PL_RF_D2D_TX);
        PCONP   &= ~(1u << 4) ;     // Turn off UART1 clock
        OUT_TX_RF(0);   //long break to exit maintenance mode
        os_tsk_wait(10);
        OUT_TX_RF(1);
        os_tsk_wait(10);
        PCONP   |= (1u << 4) ;     // Turn on UART1 clock
    }
    else if(port_no == UART_PORT3)
    {
        gpio_pl_func_cfg_gpio(GPIO_PL_DBG_PORT_TX);
        PCONP   &= ~(1u << 25) ;     // Turn off UART3 clock
        gpio_pl_clr(GPIO_PL_DBG_PORT_TX);   //long break to exit maintenance mode
        os_tsk_wait(10);
        gpio_pl_set(GPIO_PL_DBG_PORT_TX);
        os_tsk_wait(10);
        PCONP   |= (1u << 25) ;     // Turn on UART3 clock
    }

    gpio_pl_uart_cfg(uart_pl_tx[port_no] ,1);    //D2D2: 115200bps
    gpio_pl_select(uart_pl_tx[port_no]);
    gpio_pl_select(uart_pl_rx[port_no]);

    if(port_no == UART_PORT0)  //under dash port
    {
        PCLKSEL0 = (PCLKSEL0 & ~(3U << 6)) | (1u << 6) ;       // CPU_CLK / 1 = 72 MHz
        isr_set_vector(ISR_ID_UART_0 , ISR_PRI_14 , ISR_TYPE_IRQ_FAST , d2d2_irq0_service) ;
    }
    else if(port_no == UART_PORT1)
    {
        PCLKSEL0 = (PCLKSEL0 & ~(3U << 8)) | (1u << 8) ;       // CPU_CLK / 1 = 72 MHz
        isr_set_vector(ISR_ID_UART_1 , ISR_PRI_14 , ISR_TYPE_IRQ_FAST , d2d2_irq1_service) ;
    }
    else if(port_no == UART_PORT3)
    {
        PCLKSEL1 = (PCLKSEL1 & ~(3U << 18)) | (1u << 18) ;       // CPU_CLK / 1 = 72 MHz
        isr_set_vector(ISR_ID_UART_3 , ISR_PRI_14 , ISR_TYPE_IRQ_FAST , d2d2_irq3_service) ;
    }
        
    //initialize swmsg q of d2d2 port
    port = os_mem_request(sizeof(D2d2_Port));

    memset(port, 0, sizeof(D2d2_Port));
    port->smsgbase.sendfunc = (VP2A *)d2d2_txmsg;             //pass tx function
    port->smsgbase.rxirq   = (VP2A *)d2d2_uart_rx_irq;
    port->smsgbase.txirq   = (VP1A *)d2d2_uart_tx_irq;
    port->smsgbase.number = smsg_register(port_no, port);   //0: INTERNAL PORT 1: UNDER_DASH
    port->smsgbase.timeoutfunc = (VP1A *)d2d2_msg_timeout_handler;
    port->smsgbase.filterfunc = NULL;

    d2d_switch_port_rx_irq_ctrl(port, 1);
    
    wake_register      (d2d2_wake);
    
		
    port_timer_rewind(port, 10);
    port->smsgbase.busstate = BUS_SLEEP;
}

