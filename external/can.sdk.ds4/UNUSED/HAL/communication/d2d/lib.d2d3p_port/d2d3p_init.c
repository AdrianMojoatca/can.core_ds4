/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d1_init.c 32240 2015-06-12 12:31:31Z simon.demers $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "d2d3p_private.h"
#include "isr.h"
#include "intl_port.h"
#include "bitfield.h"
#include "nvfs_usr.h"

#include <string.h>
#include "timeout_f.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define D2D3P_TASK_TIMEOUT 1
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Boolean d2d_init_table (void);
static Boolean d2d_init_error (void);
void d2d3p_task                (void * tsk_arg);
static Q dummy_wait;
Timeout d2d3p_task_timeout      ;
void function_d2d3p_task_timeout(void * tsk_arg);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
void d2d3p_init (UInt8 port_no)
{
    UInt8 i;
    D2d3p_Port *port = &port3p;
    Tsk tsk;
		static Boolean do_once = TRUE;
    
    dummy_wait = q_create();

	
    //init physical port    
    if(port_no == UART_PORT0)
			PCONP   |= (1u << 3) ;     // Turn on UART0 clock
		else if(port_no == UART_PORT1)
			PCONP   |= (1u << 4) ;     // Turn on UART1 clock
		else if(port_no == UART_PORT3)
			PCONP   |= (1u << 25) ;     // Turn on UART3 clock
	
    gpio_pl_uart_cfg(uart_pl_tx[port_no], 0);
    gpio_pl_select(uart_pl_tx[port_no]);
    gpio_pl_select(uart_pl_rx[port_no]);
	
    if(port_no == UART_PORT0)  //under dash port
    {
        PCLKSEL0 = (PCLKSEL0 & ~(3U << 6)) | (1u << 6) ;       // CPU_CLK / 1 = 72 MHz
        isr_set_vector(ISR_ID_UART_0 , ISR_PRI_14 , ISR_TYPE_IRQ , d2d3p_irq0_service) ;
    }
    else if(port_no == UART_PORT1)
    {
        PCLKSEL0 = (PCLKSEL0 & ~(3U << 8)) | (1u << 8) ;       // CPU_CLK / 1 = 72 MHz
        isr_set_vector(ISR_ID_UART_1 , ISR_PRI_14 , ISR_TYPE_IRQ , d2d3p_irq1_service) ;
    }
    else if(port_no == UART_PORT3)
    {
        PCLKSEL1 = (PCLKSEL1 & ~(3U << 18)) | (1u << 18) ;       // CPU_CLK / 1 = 72 MHz
        isr_set_vector(ISR_ID_UART_3 , ISR_PRI_14 , ISR_TYPE_IRQ , d2d3p_irq3_service) ;
    }
    
    port->smsgbase.rxirq   = (VP2A *)d2d3p_uart_rx_irq;
    port->smsgbase.txirq   = (VP1A *)d2d3p_uart_tx_irq;
    port->smsgbase.number = port_no;//smsg_register(port_no, port);
    port->smsgbase.timeoutfunc = (VP1A *)d2d3p_msg_timeout_handler;
    port->rxofs = 1;
          
    d2d3p_port_no = port_no;  //0: internal port
		
    wake_register      (d2d3p_wake);
		
		if(do_once)
		{
			wake_register_ready(d2d3p_wake_ready);
			do_once = FALSE;
		}

    
    d2d_switch_port_rx_irq_ctrl(port, 1);
    port->smsgbase.busstate = BUS_SLEEP;

//    d2d3p_task_timeout = timeout_f_create(D2D3P_TASK_TIMEOUT,      function_d2d3p_task_timeout);
//		
//		timeout_start(d2d3p_task_timeout);

		tsk = os_tsk_create(d2d3p_task, TASK_D2D3P_PRI, TASK_D2D3P_STK, 0, TASK_D2D3P_NAME);

    os_tsk_start(tsk, 0);
		
}

/**
* @brief  task to handle message from d2d3p ports 
* @param
* @return
*/

void d2d3p_task(void * tsk_arg)
{

    for(; ;)
    {
			q_remove(dummy_wait, 1);

			if(port3p.smsgbase.itimer)
                {
									if(--(port3p.smsgbase.itimer) == 0)
									{
                        d2d3p_msg_timeout_handler(&port3p);
//							          wake_enter_flagged(WAKE_D2D3P_TSK) ;
									}
                }

    }
}

/**
* @brief  timer to handle message from d2d3p ports 
* @param
* @return
*/

//void function_d2d3p_task_timeout(void * tsk_arg)
//{
//			if(port3p.smsgbase.itimer)
//                {
//									if(--(port3p.smsgbase.itimer) == 0)
//									{
//                        d2d3p_msg_timeout_handler(&port3p);
////							          wake_enter_flagged(WAKE_D2D3P_TSK) ;
//									}
//                }
//			timeout_start(d2d3p_task_timeout);
//}



