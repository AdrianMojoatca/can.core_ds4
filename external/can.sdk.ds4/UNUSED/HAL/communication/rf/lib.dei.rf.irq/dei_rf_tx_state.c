/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rf_tx_state.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rf_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_rf_tx_state_preamble( void );
static void dei_rf_tx_state_wakeup  ( void );
static void dei_rf_tx_state_bit     ( void );

static void dei_rf_tx_state_pagging_frame_offset( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Boolean dei_rf_tx_bit_done ; 
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//-------------------------------------------------------------------------//

void dei_rf_tx_state_offset( void )
{    
    DEI_RF_TX_ON() ; 
        
    dei_rf_tx_bit_done = FALSE ; 

    dei_rf.tx_bit_index = 0 ; 

    DEI_RF_TMR( (dei_rf.tx_pagging) ? DEI_RF_TX_WAKEUP_ON_PERIOD : DEI_RF_TX_PREAMBLE_ON_PERIOD1 ) ; 
        
    dei_rf.tx_state = (dei_rf.tx_pagging ) ? (Dei_Rf_State)dei_rf_tx_state_wakeup : (Dei_Rf_State)dei_rf_tx_state_preamble ;                  
}

//-------------------------------------------------------------------------//

void dei_rf_tx_state_pagging_frame_offset( void )
{    
    DEI_RF_TX_ON() ; 
        
    dei_rf_tx_bit_done = FALSE ; 

    dei_rf.tx_bit_index = 0 ; 

    DEI_RF_TMR( DEI_RF_TX_PREAMBLE_ON_PERIOD1 ) ; 
        
    dei_rf.tx_state = (Dei_Rf_State)dei_rf_tx_state_preamble ;                  
}

//-------------------------------------------------------------------------//


void dei_rf_tx_state_preamble( void )
{
    if( dei_rf_tx_bit_done   )
    {
        if( ++dei_rf.tx_bit_index != 2 )
        {
            DEI_RF_TMR( DEI_RF_TX_PREAMBLE_ON_PERIOD2 ) ; 
        }
        else
        {
            DEI_RF_TMR( DEI_RF_TX_BIT_PERIOD ) ;
            
            dei_rf.tx_state = (Dei_Rf_State)dei_rf_tx_state_bit ;
        };

        DEI_RF_TX_ON() ;         
    }
    else
    {
        DEI_RF_TX_OFF() ; 
        
        DEI_RF_TMR( DEI_RF_TX_PREAMBLE_OFF_PERIOD ) ; 
                     
    };

    dei_rf_tx_bit_done = !dei_rf_tx_bit_done  ;
}        

//-------------------------------------------------------------------------//

void dei_rf_tx_state_wakeup( void )
{
    if( dei_rf_tx_bit_done )
    {        
        if( ++ dei_rf.tx_bit_index == DEI_RF_TX_WAKEUP_BIT_COUNT )
        {
            DEI_RF_TMR(  DEI_RF_TX_PREAMBLE_ON_PERIOD1 ) ;                
            
            dei_rf.tx_bit_index = 0 ; 

            dei_rf.tx_state = (Dei_Rf_State)dei_rf_tx_state_preamble ;
        }
        else
        {
            DEI_RF_TMR( DEI_RF_TX_WAKEUP_ON_PERIOD ) ;         
        };
                    
        DEI_RF_TX_ON();        
    }
    else
    {
        DEI_RF_TX_OFF() ; 
        
        DEI_RF_TMR( DEI_RF_TX_WAKEUP_OFF_PERIOD ) ;         
    };

    dei_rf_tx_bit_done = !dei_rf_tx_bit_done ;
}

//-------------------------------------------------------------------------//

void dei_rf_tx_state_bit( void )
{        
    #define TX_NORMAL         ( dei_rf.tx_bit_count == DEI_RF_TX_NORMAL_BIT_COUNT )
    #define TX_BIT_TYPE       ( dei_rf.tx_data0&1u )
         
    if( dei_rf_tx_bit_done )
    {        
        if( ++dei_rf.tx_bit_index == dei_rf.tx_bit_count )
        {
            dei_rf.tx_bit_index = 0 ;                 
            
            dei_rf.tx_data0 = dei_rf.tx_copy_data0 ; 
            
            dei_rf.tx_data1 = dei_rf.tx_copy_data1 ; 
            
            if( --dei_rf.tx_frame_count )
            {                
                if( dei_rf.tx_pagging )
                {
                    dei_rf.tx_state = (Dei_Rf_State)dei_rf_tx_state_pagging_frame_offset ;

                    DEI_RF_TMR( DEI_RF_TX_PAGGING_INTERFRAME_OFFSET ) ;
                }
                else
                {
                    dei_rf.tx_state = (Dei_Rf_State)dei_rf_tx_state_offset ; 

                    DEI_RF_TMR( TX_NORMAL ? DEI_RF_TX_INTERFRAME_SPACE_NORMAL : DEI_RF_TX_INTERFRAME_SPACE_PROGRAMMING );
                };
                                
                return ;                 

            }
            else
            {
                if( dei_rf.tx_pagging )
                {
                    if( --dei_rf.tx_page_count )
                    {
                        dei_rf.tx_frame_count = DEI_RF_TX_PAGGING_FRAME_COUNT ; 
                        
                        DEI_RF_TMR( DEI_RF_TX_PAGGING_INTERPAGE_OFFSET   ) ;
                         
                        dei_rf.tx_state = (Dei_Rf_State)dei_rf_tx_state_offset ;
                       
                        return ;
                    }                    
                };
                
                dei_rf.tx_complete    =  TRUE ; 

                dei_rf.tx_pagging = FALSE ; 

                ATOMIC_FIQ(irq_user_trigger());  

            };
        }
        else
        {
            DEI_RF_TMR(  DEI_RF_TX_BIT_PERIOD ) ; 

            DEI_RF_TX_ON();        
        };                           
    }
    else
    {
        DEI_RF_TX_OFF();
        
        DEI_RF_TMR( (TX_BIT_TYPE) ? DEI_RF_TX_BIT_HIGH_PERIOD : DEI_RF_TX_BIT_LOW_PERIOD ) ; 
        
        dei_rf.tx_data0 = dei_rf.tx_data0>>1 ; 
        
        (dei_rf.tx_data1&1u) ? (dei_rf.tx_data0 |= (1u<<31)) : (dei_rf.tx_data0 &= ~(1u<<31)) ;

        dei_rf.tx_data1 = dei_rf.tx_data1>>1 ;         
    };

    dei_rf_tx_bit_done = !dei_rf_tx_bit_done ; 

    #undef TX_NORMAL
    #undef TX_BIT_TYPE
}

//-------------------------------------------------------------------------//    

