/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_temp_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_temp_private.h"
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

void dei_temp_init (void)
{    
	ATOMIC_FIQ
	(				
        dei_temp_vars.tempHandshakeStatus = DEI_TEMP_HANDSHAKE_INIT0;
		PINMODE1 &= ~(0x00000002);			// need to check
		
		irq_user_init() ; 
        		
		dei_temp_vars.tx_data      = 0;
		dei_temp_vars.tx_copy_data = 0;
		
		dei_temp_vars.tx_idx = 0;		
		dei_temp_vars.tx_bit_complete = FALSE ;
        dei_temp_vars.rx_error        = FALSE ;
				
		dei_temp_vars.rx_data       = 0 ; 
		dei_temp_vars.rx_idx        = 0 ;
		dei_temp_vars.rx_complete   = FALSE;
        dei_temp_vars.rx_err_count  = 0 ;

		dei_temp_vars.edge = DEI_TEMP_NO_EDGE ; 

		dei_temp_vars.tx_tmr = tmr_irq_create((Tmr_Irq_Func)dei_temp_tx_irq);

		fiq_register     ( (Fiq_Func)      dei_temp_rx_fiq    ) ;        
        irq_user_register( (Irq_User_Func) dei_temp_usr_irq   ) ; 
		
		dei_temp_ext_disable();      
		
		dei_temp_vars.timeout_request = 1 ;
        
        dei_temp_vars.timeout_sensor_update = vrtc_f_create((Exec_Func)dei_temp_wake_for_read); //Use only to wakeup the CPU for temp sensor update
		        
        wake_register    ( (Wake_Func    )dei_temp_wake    );
		per_exec_register( (Per_Exec_Func)dei_temp_request );

	)      
} 


//---------------------------------------------------------------------------//

void dei_temp_wake_for_read( void )
{
    TRACE_DEI_TEMP("[Dei Temp Wake Cpu]\n\r");
}

//---------------------------------------------------------------------------//