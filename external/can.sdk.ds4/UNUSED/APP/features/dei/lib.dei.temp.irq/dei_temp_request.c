/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_temp_request.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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

Per_Exec_Wake dei_temp_request( void )
{    
	Boolean do_request ; 

	ATOMIC( do_request = !(--dei_temp_vars.timeout_request) );
	
	if( !do_request )return PER_EXEC_SLEEP ;
		
	ATOMIC_FIQ
	(
		ATOMIC
		(
            dei_temp_ext_disable();

            if((dei_temp_vars.tempHandshakeStatus == DEI_TEMP_HANDSHAKE_INIT0)||\
               (dei_temp_vars.tempHandshakeStatus == DEI_TEMP_HANDSHAKE_INIT1)||\
               (dei_temp_vars.tempHandshakeStatus == DEI_TEMP_HANDSHAKE_INIT2))
                dei_temp_vars.tx_data         = DEI_TEMP_REQUEST_HANDSHAKE ;
			else
                dei_temp_vars.tx_data         = DEI_TEMP_REQUEST ;

			dei_temp_vars.tx_copy_data    = dei_temp_vars.tx_data ; 			
			dei_temp_vars.rx_error        = FALSE ; 
			dei_temp_vars.tx_bit_complete = FALSE ;
			dei_temp_vars.tx_frame_count  = DEI_TEMP_TX_FRAME_COUNT ;
            dei_temp_vars.tx_idx          = DEI_TEMP_TX_PREAMBLE_BIT_COUNT;
            dei_temp_vars.tx_state        = DEI_TEMP_STATE_PREAMBLE ;
			
		
			tmr_irq_dis   ( dei_temp_vars.tx_tmr );
			tmr_irq_update( dei_temp_vars.tx_tmr , DEI_TEMP_TX_BIT_PREAMBLE_PERIOD , FALSE );

			tmr_irq_ena   ( dei_temp_vars.tx_tmr ); 

			dei_temp_vars.timeout_request = DEI_TEMP_TIMEOUT_TEMP_REQUEST;

			wake_ck_enter();
		)
	)
	
	return PER_EXEC_SLEEP ;      
} 


//---------------------------------------------------------------------------//

