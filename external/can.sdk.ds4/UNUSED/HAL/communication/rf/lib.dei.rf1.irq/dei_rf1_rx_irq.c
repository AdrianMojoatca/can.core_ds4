/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rf1_rx_irq.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rf1_private.h"
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

//--------------------------------------------------------------------------------------//

void dei_rf1_rx_irq( void )
{
 	Dei_Rf1_Bit rf_bit ;
	
	dei_rf1_vars.rx_time_stamp = T3TC ;	

	rf_bit = dei_rf1_rx_bit();

	
	if( rf_bit == DEI_RF1_START_BIT )
	{
		dei_rf1_vars.rx_data = 0 ; 
		dei_rf1_vars.rx_idx  = 0 ; 
		
		dei_rf1_vars.rx_start = TRUE ;
	}
	
	if( rf_bit == DEI_RF1_ERROR_BIT )
	{
		dei_rf1_vars.rx_data = 0 ; 
		dei_rf1_vars.rx_idx  = 0 ; 

		dei_rf1_vars.rx_start=FALSE;
	}

	if( dei_rf1_vars.rx_start )
	{
		if( rf_bit == DEI_RF1_LOW_BIT )
		{
			dei_rf1_vars.rx_data  = dei_rf1_vars.rx_data << 1;			
			dei_rf1_vars.rx_data &= ~1 ;
			
			dei_rf1_vars.rx_idx ++ ; 
		}
		
		if( rf_bit == DEI_RF1_HIGH_BIT )
		{
			dei_rf1_vars.rx_data  = dei_rf1_vars.rx_data << 1;			
			dei_rf1_vars.rx_data |= 1 ;
			
			dei_rf1_vars.rx_idx ++ ; 
		}
	} 

												   
	if( dei_rf1_vars.rx_idx == 32 )
	{
		dei_rf1_vars.rx_start=FALSE;

		
		if( dei_rf1_vars.rx_last_data != dei_rf1_vars.rx_data )
		{			 					
            tmr_irq_update( dei_rf1_vars.rx_tmr , DEI_RF1_RX_TIMEOUT , FALSE );
		    tmr_irq_ena   ( dei_rf1_vars.rx_tmr );
		            
            dei_rf1_ext_disable();

            dei_rf1_vars.rx_complete = TRUE ; 

			dei_rf1_vars.rx_last_data = dei_rf1_vars.rx_data ;

			ATOMIC_FIQ(irq_user_trigger()); 
		}
		else
		{
			dei_rf1_vars.rx_data = 0 ; 
			dei_rf1_vars.rx_idx  = 0 ; 
		}			
	}	

	EXTINT |= (1u << 1) ;

} 

//--------------------------------------------------------------------------------------//

void dei_rf1_rx_tmr_irq( void )
{
	if( tmr_irq_int_sta( dei_rf1_vars.rx_tmr ) )
	{
		tmr_irq_dis(dei_rf1_vars.rx_tmr );
				
		dei_rf1_vars.rx_last_data = 0; 		
	}
}

//--------------------------------------------------------------------------------------//

