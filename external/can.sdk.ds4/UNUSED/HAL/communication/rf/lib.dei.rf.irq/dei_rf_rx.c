/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rf_rx.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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
static Dei_Rf_Frame *dei_rf_cast_frame( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//----------------------------------------------------------------------------//

Dei_Rf_Frame *dei_rf_rx( TskTimeout timeout )
{
    if( os_irq_wait( dei_rf.rx_irq , FALSE , timeout ) )
    {            
        return dei_rf_cast_frame() ; 
    };
    
    TRACE_DEI_RF("[LIB.DEI_RF] IRQ RX TIMEOUT n\r");
    
    return NULL ; 
}

//----------------------------------------------------------------------------//        

Dei_Rf_Frame *dei_rf_cast_frame( void )
{
    Dei_Rf_Frame copy ; 

    if( dei_rf.rx_bit_count == DEI_RF_RX_PROGRAMMING_BIT_COUNT )
    {
        copy.programming.sn                                     =((dei_rf.rx_data0 >> 8 )& 0x00FFFFFF) |((dei_rf.rx_data1 << 24)& 0xFF000000) ;
        copy.programming.sync_count                             =(dei_rf.rx_data1 >> 8 )& 0xFFFF;
        copy.programming.pl                                     =(dei_rf.rx_data1 >> 24)& 0xFF  ;
    
       ((Dei_Rf_Frame*)&dei_rf)->programming.sn             = copy.programming.sn;
       ((Dei_Rf_Frame*)&dei_rf)->programming.sync_count     = copy.programming.sync_count;
       ((Dei_Rf_Frame*)&dei_rf)->programming.pl             = copy.programming.pl;    

       TRACE_DEI_RF("\n\r[LIB.DEI_RF] IRQ RX PROGRAMMING FRAME  |%08X %04X %02X|\n\r" , ((Dei_Rf_Frame*)&dei_rf)->programming.sn          , \
                                                                                 ((Dei_Rf_Frame*)&dei_rf)->programming.sync_count  , \
                                                                                 ((Dei_Rf_Frame*)&dei_rf)->programming.pl  ) ;  
    }
    else
    {
        copy.normal.sn0                                    =(dei_rf.rx_data0 >> 14    ) ;
        copy.normal.pl0                                    =(dei_rf.rx_data0 >> 22    ) ;
        copy.normal.encrypt                                =((dei_rf.rx_data0 >> 30)&0x00000003)|((dei_rf.rx_data1 << 2 )&0xFFFFFFFC) ;
        copy.normal.press_type                             =(dei_rf.rx_data1 >> 30)&0x01;
        copy.normal.batt_low                               =(dei_rf.rx_data1 >> 31)&0x01;
        
        
        
       ((Dei_Rf_Frame*)&dei_rf)->normal.sn0               = copy.normal.sn0;
       ((Dei_Rf_Frame*)&dei_rf)->normal.pl0               = copy.normal.pl0;
       ((Dei_Rf_Frame*)&dei_rf)->normal.encrypt           = copy.normal.encrypt  ;
       ((Dei_Rf_Frame*)&dei_rf)->normal.batt_low          = copy.normal.batt_low ;
       ((Dei_Rf_Frame*)&dei_rf)->normal.press_type        = copy.normal.press_type;   

       TRACE_DEI_RF("\n\r[LIB.DEI_RF] IRQ RX NORMAL FRAME  |%02X %02X %08X %02X|\n\r" , ((Dei_Rf_Frame*)&dei_rf)->normal.sn0          , \
                                                                                    ((Dei_Rf_Frame*)&dei_rf)->normal.pl0          , \
                                                                                    ((Dei_Rf_Frame*)&dei_rf)->normal.encrypt      , \
                                                                                    ((Dei_Rf_Frame*)&dei_rf)->normal.press_type   ) ;                                                                               
    };


    return (Dei_Rf_Frame *)&dei_rf;
}                                          

//----------------------------------------------------------------------------//        
