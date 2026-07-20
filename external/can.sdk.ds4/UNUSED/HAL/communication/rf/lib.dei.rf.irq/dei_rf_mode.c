/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rf_mode.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rf_private.h"
#include <stdarg.h>
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef void (*Dei_Rf_Mode_Func)( UInt16 , UInt32 ) ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void dei_rf_rx_normal               ( void );
static void dei_rf_rx_programming          ( void ); 
static void dei_rf_tx_normal               ( UInt16 hhu_sn , UInt32 payload  );
static void dei_rf_tx_programming          ( UInt16 car_sn , UInt16 hhu_sn   );	
static void dei_rf_tx_pagging              ( UInt16 car_sn , UInt32 payload  );
static void dei_rf_tx_pagging_stop         ( void ); 	
static void dei_rf_load_data_tx_normal     ( UInt16 hhu_sn , UInt32 payload  );
static void dei_rf_load_data_tx_programming( UInt16 car_sn , UInt16 hhu_sn   );

	 
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_rf_mode( Dei_Rf_Mode mode , ... )
{
    const Dei_Rf_Mode_Func mode_func[] = 
    {
        (Dei_Rf_Mode_Func)dei_rf_rx_normal ,(Dei_Rf_Mode_Func)dei_rf_rx_programming , 
        (Dei_Rf_Mode_Func)dei_rf_tx_normal ,(Dei_Rf_Mode_Func)dei_rf_tx_programming , 
        (Dei_Rf_Mode_Func)dei_rf_tx_pagging,(Dei_Rf_Mode_Func)dei_rf_tx_pagging_stop 
    };

    va_list args ;
    
    va_start( args , mode );
    {        
        ATOMIC_FIQ
        (
            ATOMIC( (mode_func[mode])( (UInt16)va_arg (args,VARG) , (UInt32)va_arg (args,VARG) ) ) ; 
        )        
    }
    va_end( args );  
}

//--------------------------------------------------------------------------//

void dei_rf_rx_normal( void  )	
{	
    DEI_RF_EXT_DISABLE();
    {    
        dei_rf.rx_bit_count   = DEI_RF_RX_NORMAL_BIT_COUNT ;    
        dei_rf.rx_data0 = 0 ; 
        dei_rf.rx_data1 = 0 ;    
        
        dei_rf.rx_bit_index = 0 ;          
        
        dei_rf.rx_complete = FALSE ; 
    
        dei_rf.rx_state = (Dei_Rf_State )dei_rf_rx_state_start ; 
    }               
    DEI_RF_EXT_ENABLE();
}

//--------------------------------------------------------------------------//

void dei_rf_rx_programming( void )
{
    DEI_RF_EXT_DISABLE();
    {        
        dei_rf.rx_bit_count   = DEI_RF_RX_PROGRAMMING_BIT_COUNT ;    
        dei_rf.rx_data0 = 0 ; 
        dei_rf.rx_data1 = 0 ;    
        
        dei_rf.rx_bit_index = 0 ;          
        
        dei_rf.rx_complete = FALSE ; 
    
        dei_rf.rx_state = (Dei_Rf_State )dei_rf_rx_state_start ; 
    }       
    DEI_RF_EXT_ENABLE();
}

//--------------------------------------------------------------------------//				

void dei_rf_tx_normal( UInt16 hhu_sn , UInt32 payload  )	
{	    
    UInt32 time_diff ;

    tmr_irq_dis( dei_rf.tx_tmr ) ; 
    {            
        dei_rf.tx_bit_count    = DEI_RF_TX_NORMAL_BIT_COUNT    ; 
        dei_rf.tx_frame_count  = DEI_RF_TX_NORMAL_FRAME_COUNT  ;

        dei_rf.tx_bit_index    = 0 ;
        dei_rf.tx_frame_index  = 0 ; 

        dei_rf.tx_pagging      = FALSE ; 

        dei_rf_load_data_tx_normal( hhu_sn , payload ) ;
        
        dei_rf.tx_complete = FALSE ; 

        dei_rf.tx_state = (Dei_Rf_State)dei_rf_tx_state_offset ;

        time_diff = ( T1TC > dei_rf.rx_delay ) ?  T1TC-dei_rf.rx_delay : (0xFFFFFFFF - dei_rf.rx_delay)+T1TC ;
        
        if( time_diff < (DEI_RF_TX_OFFSET_START - DEI_RF_TX_1MS) )
        {        
            tmr_irq_update( dei_rf.tx_tmr ,  DEI_RF_TX_OFFSET_START-time_diff , FALSE );//adjust the offfset

            TRACE_DEI_RF("[DEI_RF_TX_NORMAL_OFFSET %d ]\n\r",(DEI_RF_TX_OFFSET_START-time_diff)/(72*1000));
        }
        else
        {
            tmr_irq_update( dei_rf.tx_tmr ,  DEI_RF_TX_1MS , FALSE );//start imediattly

            TRACE_DEI_RF("[DEI_RF_TX_NORMAL_OFFSET_1MS]\n\r");
        }
          
    }
    tmr_irq_ena( dei_rf.tx_tmr ) ;     
}

//--------------------------------------------------------------------------//

void dei_rf_tx_programming ( UInt16 car_sn , UInt16 hhu_sn  )	
{    
    UInt32 time_diff ;

    tmr_irq_dis( dei_rf.tx_tmr ) ; 
    {            
        dei_rf.tx_bit_count    = DEI_RF_TX_PROGRAMMING_BIT_COUNT    ; 
        dei_rf.tx_frame_count  = DEI_RF_TX_PROGRAMMING_FRAME_COUNT  ;

        dei_rf.tx_bit_index    = 0 ;
        dei_rf.tx_frame_index  = 0 ; 

        dei_rf.tx_pagging      = FALSE ; 

        dei_rf_load_data_tx_programming( car_sn , hhu_sn ) ;

        dei_rf.tx_complete = FALSE ; 

        dei_rf.tx_state = (Dei_Rf_State)dei_rf_tx_state_offset ;

        time_diff = ( T1TC > dei_rf.rx_delay ) ?  T1TC-dei_rf.rx_delay : (0xFFFFFFFF - dei_rf.rx_delay)+T1TC ;
        
        if( time_diff < (DEI_RF_TX_OFFSET_START - DEI_RF_TX_1MS) )
        {        
            tmr_irq_update( dei_rf.tx_tmr ,  DEI_RF_TX_OFFSET_START-time_diff , FALSE );//adjust the offfset
        }
        else
        {
            tmr_irq_update( dei_rf.tx_tmr ,  DEI_RF_TX_1MS , FALSE );//start imediattly
        }
                        
    }
    tmr_irq_ena( dei_rf.tx_tmr ) ;     
}    

//--------------------------------------------------------------------------//

void dei_rf_tx_pagging( UInt16 car_sn , UInt32 payload  )	
{
    tmr_irq_dis( dei_rf.tx_tmr ) ; 
    {            
        dei_rf.tx_bit_count    = DEI_RF_TX_NORMAL_BIT_COUNT    ; 
        
        dei_rf.tx_frame_count  = DEI_RF_TX_PAGGING_FRAME_COUNT ;
        dei_rf.tx_page_count   = DEI_RF_TX_PAGGING_PAGE_COUNT  ;

        dei_rf.tx_bit_index    = 0 ;
        dei_rf.tx_frame_index  = 0 ; 

        dei_rf_load_data_tx_normal( car_sn , payload ) ;

        tmr_irq_update( dei_rf.tx_tmr ,  DEI_RF_TX_OFFSET_START , FALSE );

        dei_rf.tx_complete = FALSE ; 

        dei_rf.tx_pagging = TRUE ; 

        dei_rf.tx_state = (Dei_Rf_State)dei_rf_tx_state_offset ;
    }
    tmr_irq_ena( dei_rf.tx_tmr ) ;     
}    

//----------------------------------------------------------------------------//

void dei_rf_tx_pagging_stop( void )
{
    if( dei_rf.tx_pagging )
    {
        tmr_irq_dis( dei_rf.tx_tmr ) ; 
    
        dei_rf.tx_complete = FALSE ; 
        dei_rf.tx_pagging  = FALSE ;		
        
        DEI_RF_TX_OFF() ;		

        os_irq_set( dei_rf.tx_irq ) ;        
    }
}

//----------------------------------------------------------------------------//

void dei_rf_load_data_tx_normal( UInt16 hhu_sn , UInt32 payload )
{
	UInt16 crc = 0 ; 
	
	/*compute the crc*/

	crc  = 0 ;
	crc += (UInt8)(hhu_sn);
	crc += (UInt8)(hhu_sn >> 8);
	crc += (UInt8)(payload) ; 
	crc += (UInt8)(payload>>8);
	crc += (UInt8)(payload>>16);
	crc += (UInt8)(payload>>24);

	crc &= 0x00FF ; 

	/*prepare the data*/
	dei_rf.tx_copy_data0  = dei_rf.tx_copy_data1 = 0 ; 	
	dei_rf.tx_copy_data0  = 0x00000000C ; 
	dei_rf.tx_copy_data0 |= ((UInt32)hhu_sn << 4 ); 
	dei_rf.tx_copy_data0 |= ((UInt32)payload<< 20);
	dei_rf.tx_copy_data1 |= ((UInt32)payload>> 12);
	dei_rf.tx_copy_data1 |= ((UInt32)crc    << 20);	

	dei_rf.tx_data0 = dei_rf.tx_copy_data0 ; 
    dei_rf.tx_data1 = dei_rf.tx_copy_data1 ; 
}

//----------------------------------------------------------------------------//				

void dei_rf_load_data_tx_programming( UInt16 car_sn , UInt16 hhu_sn )
{
	UInt16 crc = 0 ; 
	
	/*compute the crc*/
	crc  = 0 ;
	crc += (UInt8)(car_sn);
	crc += (UInt8)(car_sn >> 8);
	crc += (UInt8)(hhu_sn) ; 
	crc += (UInt8)(hhu_sn >> 8);
	
	crc &= 0x00FF ; 

	/*prepare the data*/
	dei_rf.tx_copy_data0  = dei_rf.tx_copy_data1 = 0 ; 
	dei_rf.tx_copy_data0  = 0x0000000C ;
	dei_rf.tx_copy_data0 |= ((UInt32)car_sn << 4 ); 
	dei_rf.tx_copy_data0 |= ((UInt32)hhu_sn << 20);
	dei_rf.tx_copy_data1 |= ((UInt32)hhu_sn >> 12);
	dei_rf.tx_copy_data1 |= ((UInt32)crc    << 4 );	

	dei_rf.tx_data0 = dei_rf.tx_copy_data0 ; 
    dei_rf.tx_data1 = dei_rf.tx_copy_data1 ; 

// DATA 0                                                                                                                                                     first tx
// |b31 |b30 |b29 |b28 |b27 |b26 |b25 |b24 |b23 |b22 |b21 |b20 |b19 |b18 |b17 |b16 |b15 |b14 |b13 |b12 |b11 |b10 |b09 |b08 |b07 |b06 |b05 |b04 |b03 |b02 |b01 |b00 | 
// |HS11|HS10|HS09|HS08|HS07|HS06|HS05|HS04|HS03|HS02|HS01|HS00|CS15|CS14|CS13|CS12|CS11|CS10|CS09|CS08|CS07|CS06|CS05|CS04|CS03|CS02|CS01|CS00| 1  | 1  | 0  | 0  |

// DATA1
// |b31 |b30 |b29 |b28 |b27 |b26 |b25 |b24 |b23 |b22 |b21 |b20 |b19 |b18 |b17 |b16 |b15 |b14 |b13 |b12 |b11 |b10 |b09 |b08 |b07 |b06 |b05 |b04 |b03 |b02 |b01 |b00 | 
// | x  | x  | x  | x  | x  | x  | x  | x  | x  | x  | x  | x  | x  | x  | x  | x  | x  | x  | x  |STOP|CK07|CK06|CK05|CK04|CK03|CK02|CK01|CK00|HS15|HS14|HS13|HS12|
}

//----------------------------------------------------------------------------//

