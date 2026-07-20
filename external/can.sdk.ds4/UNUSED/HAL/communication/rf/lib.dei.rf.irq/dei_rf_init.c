/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rf_init.c 33268 2015-08-19 15:40:18Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rf_private.h"
#include "fiq.h"
#include "irq_user.h"
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

//--------------------------------------------------------------------------//

void dei_rf_init( void  )	
{	
	ATOMIC
    (    
        irq_user_init() ; 
    
        dei_rf.rx_irq = os_irq_create() ; 
        dei_rf.tx_irq = os_irq_create() ; 
        
        dei_rf.tx_tmr = tmr_irq_create((Tmr_Irq_Func)dei_rf_tx_irq);
        
                
        irq_user_register( (Irq_User_Func) dei_rf_usr_irq) ;     
        
        wake_register    ( (Wake_Func)     dei_rf_wake   ) ;
    
        
    	dei_rf.time_rx_high_min = DEI_RF_CK_72MHZ * DEI_RF_RX_HIGH_MIN;
    	dei_rf.time_rx_high_max = DEI_RF_CK_72MHZ * DEI_RF_RX_HIGH_MAX;
    	dei_rf.time_rx_low_max  = DEI_RF_CK_72MHZ * DEI_RF_RX_LOW_MAX ;
        
        dei_rf.antenna_type     = DEI_RF_CLEAN_ANTENNA ;      
        dei_rf.rx_delay         = 0 ;
        
        dei_rf.rx_id_table[0] = dei_rf.rx_id_table[1] = dei_rf.rx_id_table[2] = dei_rf.rx_id_table[3] = 0;
        
        ext_init:
    	{
    		EXTMODE  = 0x02       ; 
    		EXTPOLAR = 0x02       ;
				#ifdef DEV_LPC_236x
				INTWAKE |= 2          ;
				#endif

    		isr_set_vector (ISR_ID_EINT_1, ISR_PRI_15 , ISR_TYPE_IRQ_FAST , dei_rf_rx_irq );
    	}
    
        DEI_RF_TX_OFF();    
    
        dei_rf_mode(DEI_RF_MODE_NORMAL_RX);
    )    
}

//--------------------------------------------------------------------------//

