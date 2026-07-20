/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d1_init.c 32240 2015-06-12 12:31:31Z simon.demers $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "d2d1_private.h"
#include "isr.h"
#include "intl_port.h"
#include "bitfield.h"
#include "nvfs_usr.h"

#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Boolean d2d_init_table (void);
static Boolean d2d_init_error (void);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
void d2d1_init (UInt8 port_no)
{
    UInt8 i;
    D2d1_Port *port;
		static Boolean do_once = TRUE;
    
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
        isr_set_vector(ISR_ID_UART_0 , ISR_PRI_14 , ISR_TYPE_IRQ , d2d1_irq0_service) ;
    }
    else if(port_no == UART_PORT1)
    {
        PCLKSEL0 = (PCLKSEL0 & ~(3U << 8)) | (1u << 8) ;       // CPU_CLK / 1 = 72 MHz
        isr_set_vector(ISR_ID_UART_1 , ISR_PRI_14 , ISR_TYPE_IRQ , d2d1_irq1_service) ;
    }
    else if(port_no == UART_PORT3)
    {
        PCLKSEL1 = (PCLKSEL1 & ~(3U << 18)) | (1u << 18) ;       // CPU_CLK / 1 = 72 MHz
        isr_set_vector(ISR_ID_UART_3 , ISR_PRI_14 , ISR_TYPE_IRQ , d2d1_irq3_service) ;
    }
    
    //initialize swmsg q of d2d1 port
    port = os_mem_request(sizeof(D2d1_Port));
    memset(port, 0, sizeof(D2d1_Port));
    port->smsgbase.sendfunc = (VP2A *)d2d1_txmsg;
    port->smsgbase.rxirq   = (VP2A *)d2d1_uart_rx_irq;
    port->smsgbase.txirq   = (VP1A *)d2d1_uart_tx_irq;
    port->smsgbase.number = smsg_register(port_no, port);
    port->smsgbase.timeoutfunc = (VP1A *)d2d1_msg_timeout_handler;
    port->smsgbase.filterfunc = (BP2A *)d2d1_send_filter;
          
    d2d1_port_no = port_no;  //0: internal port
		
    wake_register      (d2d1_wake);
		//init reset
		d2d1_init_reset() ;
		
		if(do_once)
		{
			wake_register_ready(d2d1_wake_ready);
			do_once = FALSE;
		}

    
    d2d_switch_port_rx_irq_ctrl(port, 1);
}

//legacy D2D config

/**
* @brief  d2d_init_table
* @param
* @return
*/
static Boolean d2d_init_table (void)
{
  UInt16 size = 0 ;

  if ((nvfs_rd_size(nvfs_d2d_ena , &size) == NVFS_OK))
    {
      if((size == sizeof(d2d_ena_table)) && (nvfs_rd_s(nvfs_d2d_ena , &d2d_ena_table, sizeof(d2d_ena_table)) == NVFS_OK))
      {
          return (TRUE) ;
      }
    }

  nvfs_delete(nvfs_d2d_ena);

  switch (nvfs_wr_create(nvfs_d2d_ena, &d2d_ena_default , sizeof(d2d_ena_default)))
    {
      case NVFS_OK :  
        {
           return d2d_init_table() ;
        }  
    }      

  return FALSE ;
}

/**
* @brief  d2d_init_error
* @param
* @return
*/
static Boolean d2d_init_error (void)
{
  UInt16 size = 0 ;

  if ((nvfs_rd_size(nvfs_d2d_error , &size) == NVFS_OK))
  {
      if((size == sizeof(d2d_err)) && (nvfs_rd_s(nvfs_d2d_error , &d2d_err, sizeof(d2d_err)) == NVFS_OK))
      {
          return (TRUE) ;
      }
  }
	return FALSE;
}


/**
* @brief  d2d_init_nvfs
* @param
* @return
*/
Boolean d2d_init_nvfs(void)
{
    UInt8 index;
    //validate D2D mask in NVFS matches the fw's default value
  if (nvfs_rd_s(nvfs_d2d_ena , &d2d_ena_table, sizeof(d2d_ena_table)) == NVFS_OK)
  {
    // Loop through the D2D mask from NVFS and if a discrepancy is found with the firmware's default mask (support bits), reset the D2D NVFS variables
    for (index = 0 ; index < sizeof_array (d2d_ena_table.d2d_mask) ; index++)
    {
      if (d2d_ena_table.d2d_mask[index] ^ d2d_ena_default.d2d_mask[index])
      {
        // Reset the D2D NVFS variables since the mask is different than the default one
        d2d1_reset() ;
      
        // Exit
        break ;
      }
    }
  }
  else
  {
    // Reset the (other) D2D NVFS variables since the mask doesn't even exist
    d2d1_reset() ;
  }

  return (d2d_init_table() & d2d_init_error());
}



