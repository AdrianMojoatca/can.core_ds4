/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dbgcli_init.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "isr.h"
#include "dbgcli_private.h"
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

// !!! WARNING - DO NOT USE FRACTIONAL DIVIDE !!!
//
// Enabling of fractional divide and using UART IRQs seems to cause
// some kind of hardware errata to appear.  This needs to be confirmed
// with errata sheets.


void dbg_init (void)
{
  CRITICAL ;
	UInt8 i;
  /// Initialize IRQ synchronization points ///

  dbg_irq_tx = os_irq_create () ;
  dbg_irq_rx = os_irq_create () ;

  dbg_tx_res = os_res_create () ;
	dbg_str_res = os_res_create () ;

	dbg_vars.q_free    = q_create()   ; 
	dbg_vars.q_pending = q_create()   ;	

	for( i = 0 ; i < DBG_Q_SIZE ; i ++ )
	{
		 void *dbg_q_msg ; 
		 
	   dbg_q_msg = os_mem_request( sizeof( struct s_Dbg_Q_Msg ) );
										
		 q_insert( dbg_vars.q_free , (Q_Msg*)dbg_q_msg  );            
		 q_assign( dbg_vars.q_free , (Q_Msg*)dbg_q_msg  );
	}

  if(dbg_uart_avail)
  {  
    /// Configure UART-3 as debug port ///
    C_ENTER ;
      {
        /// NOTE : Hardcoded UART-3 at 75 Mhz clock source - 115200 N 8 1 ///

        PCONP |= (1u << 25) ;     // Turn on UART clock

        gpio_pl_uart_cfg(GPIO_PL_DBG_PORT_TX, 1);
        PCLKSEL1 = (PCLKSEL1 & ~(3U << 18)) | (1u << 18) ;       // CPU_CLK / 1 = 72 MHz

        gpio_pl_select(GPIO_PL_DBG_PORT_TX);
        gpio_pl_select(GPIO_PL_DBG_PORT_RX);

        /// Connect UART-3 interrupt vector in VIC controller and enable it ///

        isr_set_vector (ISR_ID_UART_3 , ISR_PRI_15 , ISR_TYPE_IRQ , dbg_irq_service) ;
      }
     C_LEAVE ;
   }
    
		dbgtx_fifo = (UInt8 *) os_mem_request( DBGTX_FIFO_SIZE );
		
		init_dbg_task:
		{
			Tsk tsk;
			tsk = os_tsk_create ( dbg_tsk , DBG_TSK_PRI , DBG_TSK_STK , 0 , DBG_TSK_NAME );
			os_tsk_start (tsk , 0) ;
		}
}

