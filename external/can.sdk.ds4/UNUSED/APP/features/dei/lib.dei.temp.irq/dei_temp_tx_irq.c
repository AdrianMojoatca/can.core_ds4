/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_temp_tx_irq.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_temp_private.h"
#include "gpio.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_temp_tx_bit_data    ( Boolean bit );
static void dei_temp_tx_bit_start   ( void );
static void dei_temp_tx_bit_complete( void );
static void dei_temp_tx_bit_preamble( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------------------//

void dei_temp_tx_irq( void )
{
	if( tmr_irq_int_sta( dei_temp_vars.tx_tmr) )
    {	
    	tmr_irq_dis( dei_temp_vars.tx_tmr );		
    
    	if( dei_temp_vars.tx_bit_complete )
    	{
    		dei_temp_tx_bit_complete() ; 
    
            return ;
    	}
    		
            
        switch( dei_temp_vars.tx_state )
        {        
            case DEI_TEMP_STATE_PREAMBLE :
            {
                if( --dei_temp_vars.tx_idx )
                {
                    dei_temp_tx_bit_preamble();
                }
                else
                {
                    tmr_irq_update( dei_temp_vars.tx_tmr , DEI_TEMP_TX_START_BIT_PERIOD , TRUE );
    
    			    tmr_irq_ena( dei_temp_vars.tx_tmr );
                          
                    dei_temp_vars.tx_state = DEI_TEMP_STATE_TX_START_BIT ;                                              
                }                
            }
            break;
    
            case DEI_TEMP_STATE_TX_START_BIT :
            {
                dei_temp_tx_bit_start();
    
                dei_temp_vars.tx_state = DEI_TEMP_STATE_TX_DATA ; 
                
                dei_temp_vars.tx_idx = DEI_TEMP_TX_BIT_COUNT + 1 ;                                             
            }
            break;
    
            case DEI_TEMP_STATE_TX_DATA :
            {
                if( --dei_temp_vars.tx_idx )
                {
                    dei_temp_tx_bit_data( !!(dei_temp_vars.tx_data & 0x80) );
    	
    			    dei_temp_vars.tx_data = dei_temp_vars.tx_data << 1;   
                }
                else
                {
                    dei_temp_ext_enable();  //Switch To Rx Mode
    
                    tmr_irq_update( dei_temp_vars.tx_tmr , DEI_TEMP_RX_FRAME_TIMEOUT, TRUE);	
    				tmr_irq_ena   ( dei_temp_vars.tx_tmr );
    
                    dei_temp_vars.tx_state = DEI_TEMP_STATE_RX_TIMEOUT ;                                              
                }
            }
            break;
    
            case DEI_TEMP_STATE_RX_TIMEOUT :
            {                    
                dei_temp_ext_disable();
                        
                if( (--dei_temp_vars.tx_frame_count) )
    			{				
    				dei_temp_tx_bit_preamble();
                    
                    dei_temp_vars.tx_idx = DEI_TEMP_TX_PREAMBLE_BIT_COUNT ;     
                    
                    dei_temp_vars.tx_data = dei_temp_vars.tx_copy_data ; 
    
                    dei_temp_vars.tx_state = DEI_TEMP_STATE_PREAMBLE ;
                }
                else
                {
                    dei_temp_vars.rx_error = TRUE ; 

					if(dei_temp_vars.tempHandshakeStatus < DEI_TEMP_HANDSHAKE_PASS)
					{
					    if(dei_temp_vars.tempHandshakeStatus < DEI_TEMP_HANDSHAKE_INIT2)
					        dei_temp_vars.tempHandshakeStatus++;
					    else
					        dei_temp_vars.tempHandshakeStatus = DEI_TEMP_HANDSHAKE_FAIL;
					}
						    
    									
    				ATOMIC_FIQ( irq_user_trigger() );        
                }
             }
             break;
        }
    }
} 

//--------------------------------------------------------------------------------------//

static void dei_temp_tx_bit_start( void )
{
	tmr_irq_dis( dei_temp_vars.tx_tmr );
	
	DEI_TEMP_OUT(TRUE);
	
	dei_temp_vars.tx_bit_complete = TRUE; 	
	
	tmr_irq_update( dei_temp_vars.tx_tmr , DEI_TEMP_TX_START_BIT_PERIOD , TRUE );
	tmr_irq_ena   ( dei_temp_vars.tx_tmr);	
}

//--------------------------------------------------------------------------------------//

static void dei_temp_tx_bit_data( Boolean bit )
{
	tmr_irq_dis( dei_temp_vars.tx_tmr );
	
	DEI_TEMP_OUT(TRUE);
	
	dei_temp_vars.tx_bit_complete = TRUE; 
	dei_temp_vars.tx_bit          = bit ;
	
	tmr_irq_update( dei_temp_vars.tx_tmr , dei_temp_vars.tx_bit ? DEI_TEMP_TX_BIT_HIGH_PERIOD : DEI_TEMP_TX_BIT_LOW_PERIOD , TRUE );
	tmr_irq_ena   ( dei_temp_vars.tx_tmr);	
}

//--------------------------------------------------------------------------------------//


static void dei_temp_tx_bit_preamble(void)
{
	tmr_irq_dis( dei_temp_vars.tx_tmr );
	
	DEI_TEMP_OUT(TRUE);
	
	dei_temp_vars.tx_bit_complete = TRUE; 
		
	tmr_irq_update( dei_temp_vars.tx_tmr , DEI_TEMP_TX_BIT_PREAMBLE_PERIOD , TRUE );
	tmr_irq_ena   ( dei_temp_vars.tx_tmr);	
}

//--------------------------------------------------------------------------------------//


static void dei_temp_tx_bit_complete( void )
{
	tmr_irq_dis( dei_temp_vars.tx_tmr );
	
	DEI_TEMP_OUT(FALSE);               
	
	dei_temp_vars.tx_bit_complete = FALSE; 
	
    tmr_irq_update( dei_temp_vars.tx_tmr , DEI_TEMP_TX_BIT_LOW_PERIOD , TRUE );
	tmr_irq_ena   ( dei_temp_vars.tx_tmr );	
}

//--------------------------------------------------------------------------------------//
