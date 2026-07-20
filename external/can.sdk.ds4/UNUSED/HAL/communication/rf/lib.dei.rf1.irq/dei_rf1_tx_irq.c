/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rf1_tx_irq.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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
static void dei_rf1_tx_bit_start   ( Boolean bit );
static void dei_rf1_tx_bit_complete( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------------------//

void dei_rf1_tx_irq( void )
{
	if( tmr_irq_int_sta( dei_rf1_vars.tx_tmr) )
	{
		tmr_irq_dis( dei_rf1_vars.tx_tmr );		

		if( dei_rf1_vars.tx_bit_complete )
		{
			dei_rf1_tx_bit_complete() ; 
		}
		else
		{
			
			if( dei_rf1_vars.tx_idx == 0 )
			{
				dei_rf1_tx_bit_start(TRUE);		 //The first  is HIGH

				dei_rf1_vars.tx_idx ++ ; 
			}
			else
			{									
				if( dei_rf1_vars.tx_idx < 32+1 )
				{
					dei_rf1_vars.tx_idx ++ ; 

					dei_rf1_tx_bit_start( !!(dei_rf1_vars.tx_data & 0x80000000) );

					dei_rf1_vars.tx_data = dei_rf1_vars.tx_data << 1;
				}
				else
				{
					if(FALSE)// dei_rf1_vars.tx_idx == 32 + 1 )
					{
						dei_rf1_vars.tx_idx ++ ; 

						dei_rf1_tx_bit_start(TRUE);
					}
					else
					{	
						OUT_TX_RF(FALSE);
						
						//we done with 1 frame	, so check if we have other frames for TX
						if( (--dei_rf1_vars.tx_frame_count) )
						{
							tmr_irq_dis   ( dei_rf1_vars.tx_tmr );
							tmr_irq_update( dei_rf1_vars.tx_tmr , DEI_RF1_TX_NEXT_FRAME_TIMEOUT, TRUE);
	
							tmr_irq_ena   ( dei_rf1_vars.tx_tmr );
	
							dei_rf1_vars.tx_data = dei_rf1_vars.tx_copy_data ; 
							dei_rf1_vars.tx_idx  = 0;
						}
						else
						{	
							dei_rf1_vars.tx_complete = TRUE ; 
							
							ATOMIC_FIQ(irq_user_trigger()); 
						}
					}
				}
			}				
		}
	}	
} 

//--------------------------------------------------------------------------------------//

static void dei_rf1_tx_bit_start( Boolean bit )
{
	tmr_irq_dis( dei_rf1_vars.tx_tmr );
	
	OUT_TX_RF(bit);
	
	dei_rf1_vars.tx_bit_complete = TRUE; 
	dei_rf1_vars.tx_bit          = bit ;
	
	tmr_irq_update( dei_rf1_vars.tx_tmr , DEI_RF1_TX_HALF_BIT_TIMEOUT , TRUE );
	tmr_irq_ena(dei_rf1_vars.tx_tmr);	
}
																		 
//--------------------------------------------------------------------------------------//

static void dei_rf1_tx_bit_complete( void )
{
	tmr_irq_dis( dei_rf1_vars.tx_tmr );
	
	OUT_TX_RF(!dei_rf1_vars.tx_bit );
	
	dei_rf1_vars.tx_bit_complete = FALSE; 

	tmr_irq_update( dei_rf1_vars.tx_tmr , DEI_RF1_TX_HALF_BIT_TIMEOUT , TRUE );
	tmr_irq_ena   (dei_rf1_vars.tx_tmr);	
}

//--------------------------------------------------------------------------------------//
