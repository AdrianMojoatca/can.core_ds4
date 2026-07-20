/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rf_rx_state.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rf_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef enum
{
    DEI_RF_BIT_LOW   = 0, 
    DEI_RF_BIT_HIGH  = 1,
    DEI_RF_BIT_ERROR = 2,
    DEI_RF_BIT_NOISE = 3
}Dei_Rf_Bit_Type ; 

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_rf_rx_state_data( void );
static void dei_rf_rx_state_crc0( void );
static void dei_rf_rx_state_crc1( void );

static Dei_Rf_Bit_Type dei_rf_rx_state_check_bit( void );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_rf_rx_state_start( void )
{
    Dei_Rf_Bit_Type bit_type ; 

    bit_type = dei_rf_rx_state_check_bit() ; 

    if( bit_type == DEI_RF_BIT_NOISE )  return ; 

    if( (bit_type == DEI_RF_BIT_ERROR) || (bit_type == DEI_RF_BIT_LOW) )
    {
        dei_rf.rx_bit_index = 0 ; 
      
        return ;
    };

    if( ++dei_rf.rx_bit_index == 2 )
    {
        dei_rf.rx_bit_index = 0 ; 
        
        dei_rf.rx_data0 = dei_rf.rx_data1 = 0 ; 
        dei_rf.rx_crc0  = dei_rf.rx_crc1  = 0 ;

        dei_rf.rx_state = (Dei_Rf_State)dei_rf_rx_state_data ; 
    };
}

//--------------------------------------------------------------------------//

void dei_rf_rx_state_data( void )
{
    Dei_Rf_Bit_Type bit_type ;     
    Boolean tmp ;

    bit_type = dei_rf_rx_state_check_bit() ; 

    if( bit_type == DEI_RF_BIT_NOISE )  return ; 

    if( bit_type == DEI_RF_BIT_ERROR )
    {
        dei_rf.rx_bit_index = 0 ; 

        dei_rf.rx_state = (Dei_Rf_State)dei_rf_rx_state_start ; 
      
        return ;
    };

    
    dei_rf.rx_data0 = dei_rf.rx_data0 >> 1 ; 
    
    if( dei_rf.rx_data1 & 1u ) dei_rf.rx_data0 |= (1u<<31) ; 

    dei_rf.rx_data1 = dei_rf.rx_data1 >> 1 ;
    
    if( bit_type )                     
    {               
        dei_rf.rx_data1 |= (1u<<31) ;  
    };
    
    tmp = dei_rf.rx_crc1;
    
    dei_rf.rx_crc1 = dei_rf.rx_crc0 ^ bit_type ;
    dei_rf.rx_crc0 = dei_rf.rx_crc1 ^ tmp      ;    

    if( ++dei_rf.rx_bit_index == dei_rf.rx_bit_count )
    {
        dei_rf.rx_state = (Dei_Rf_State)dei_rf_rx_state_crc0 ; 
    };
}

//--------------------------------------------------------------------------//

void dei_rf_rx_state_crc0( void )
{
    Dei_Rf_Bit_Type bit_type ;         

    bit_type = dei_rf_rx_state_check_bit() ; 

    if( bit_type == DEI_RF_BIT_NOISE )  return ; 

    if( (bit_type == DEI_RF_BIT_ERROR) || (bit_type != dei_rf.rx_crc0 ) )
    {
        dei_rf.rx_bit_index = 0 ; 

        dei_rf.rx_state = (Dei_Rf_State)dei_rf_rx_state_start ; 
      
        return ;
    };

    dei_rf.rx_state = (Dei_Rf_State)dei_rf_rx_state_crc1 ; 
}

//--------------------------------------------------------------------------//

void dei_rf_rx_state_crc1( void )
{
    Dei_Rf_Bit_Type bit_type ;         

    bit_type = dei_rf_rx_state_check_bit() ; 

    if( bit_type == DEI_RF_BIT_NOISE )  return ; 

    if( (bit_type == DEI_RF_BIT_ERROR) || (bit_type != dei_rf.rx_crc1 ) )
    {
        dei_rf.rx_bit_index = 0 ; 

        dei_rf.rx_state = (Dei_Rf_State)dei_rf_rx_state_start ; 
      
        return ;
    };
    
    if( dei_rf.rx_bit_count == DEI_RF_RX_NORMAL_BIT_COUNT )
    {
        UInt8 i ;                     
        
        for( i = 0 ; i < DEI_RF_ID_COUNT ; i ++ )
        {                
             if( ((dei_rf.rx_data0 >> 14)&0x000000FF) == dei_rf.rx_id_table[i] )break;
        };    

        if( i == DEI_RF_ID_COUNT )
        {
            dei_rf.rx_bit_index = 0 ; 

            dei_rf.rx_state = (Dei_Rf_State)dei_rf_rx_state_start ; 
      
            return ;            
        };
    };
    
    DEI_RF_EXT_DISABLE() ;                 
        
    dei_rf.rx_complete = TRUE ; 
    
    dei_rf.rx_delay    = T1TC ;        
        
    ATOMIC_FIQ(	irq_user_trigger() ); 
}

//--------------------------------------------------------------------------//

Dei_Rf_Bit_Type dei_rf_rx_state_check_bit( void )
{
    UInt32 time_fiq   ;
    UInt32 time_diff  ;    
    
    time_fiq = dei_rf.rx_time_stamp ;

    time_diff = ( time_fiq > dei_rf.rx_time ) ?  time_fiq-dei_rf.rx_time : (0xFFFFFFFF - dei_rf.rx_time)+time_fiq ; 

    if( time_diff < dei_rf.time_rx_high_min ) 
    {
        return DEI_RF_BIT_NOISE ; 
    }
        
    dei_rf.rx_time = time_fiq ;
                
    if( time_diff <  dei_rf.time_rx_high_max )
    { 
        return DEI_RF_BIT_HIGH  ;
    }
    
    if( time_diff <= dei_rf.time_rx_low_max  ) 
    {
        return DEI_RF_BIT_LOW   ;                
    }

    return DEI_RF_BIT_ERROR ;    
}

//--------------------------------------------------------------------------//
