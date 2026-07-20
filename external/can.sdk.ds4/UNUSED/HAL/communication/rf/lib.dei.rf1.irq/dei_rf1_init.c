/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rf1_init.c 33268 2015-08-19 15:40:18Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rf1_private.h"
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

//---------------------------------------------------------------------------//

void dei_rf1_init (void)
{    
	ATOMIC
	(
		OUT_TX_RF( FALSE ) ; // RF OFF
		
		if( dei_rf1_nvfs_init() )
		{
			TRACE_DEI_RF1("[DEI_RF1_NVFS_INIT_OK]\n\r");
		}
		else
		{
			TRACE_DEI_RF1("[DEI_RF1_NVFS_INIT_ERROR]\n\r");
		}
				
		irq_user_init() ; 
        
		dei_rf1_vars.rx_time     = 0 ;
		dei_rf1_vars.rx_data     = 0 ;
		dei_rf1_vars.rx_last_data= 0 ;
		dei_rf1_vars.rx_idx      = 0 ;
		dei_rf1_vars.rx_complete = FALSE ;
		dei_rf1_vars.rx_start    = FALSE ;

		dei_rf1_vars.tx_data      = 0;
		dei_rf1_vars.tx_copy_data = 0;
		
		dei_rf1_vars.tx_idx = 0;
		dei_rf1_vars.tx_complete     = FALSE ; 
		dei_rf1_vars.tx_in_progress  = FALSE ;
		dei_rf1_vars.tx_bit_complete = FALSE ;
		dei_rf1_vars.tx_request      = FALSE ;

		dei_rf1_vars.rx_time_low_min  = DEI_RF1_RX_LOW_MIN_WAKE  ;
		dei_rf1_vars.rx_time_low_max  = DEI_RF1_RX_LOW_MAX_WAKE  ;
		dei_rf1_vars.rx_time_high_max = DEI_RF1_RX_HIGH_MAX_WAKE ;
		dei_rf1_vars.rx_time_start_min= DEI_RF1_RX_START_MIN_WAKE;
		dei_rf1_vars.rx_time_start_max= DEI_RF1_RX_START_MAX_WAKE;

		dei_rf1_vars.exec_q_free = exec_q_free_create(2);

		dei_rf1_vars.rx_tmr = tmr_irq_create((Tmr_Irq_Func)dei_rf1_rx_tmr_irq);
		dei_rf1_vars.tx_tmr = tmr_irq_create((Tmr_Irq_Func)dei_rf1_tx_irq    );
		
		isr_set_vector (ISR_ID_EINT_1, ISR_PRI_15 , ISR_TYPE_IRQ_FAST , dei_rf1_rx_irq );
    
		    	
		dei_rf1_notify_init();

		EXTMODE  = 2;
		gpio_pl_select(GPIO_PL_RF_RX);
		
		INTWAKE |= 2;

		irq_user_register( (Irq_User_Func) dei_rf1_usr_irq   ) ; 
		
		dei_rf1_ext_enable_rising();
	)      
} 


//---------------------------------------------------------------------------//
