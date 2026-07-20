/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ask_check_frame_programming.c 31193 2015-05-06 17:14:56Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ask_private.h"
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

UInt8 dei_ask_check_frame_programming_mode( void )
{
    UInt8 i ;
    
    if( (dei_ask.data_enc.data == dei_ask.last_data_enc) &&
        (dei_ask.data_fix.data == dei_ask.last_data_fix))
    {    
        return DEI_ASK_MAX_REMOTES ; //programming not done
    }
    
    if( (dei_ask.data_fix.bits.sn == 0xFFFFFFFF) || 
        (dei_ask.data_fix.bits.sn == 0x00000000) )
    {    
        TRACE_DEI_ASK("\n\r[LIB.DEI.ASK] Programming : invalid SN %08X\n\r" , dei_ask.data_fix.bits.sn);
                
        return DEI_ASK_MAX_REMOTES;  //programming not done
    }
                            
    for(i=0;i<DEI_ASK_MAX_REMOTES;i++)
    {
        if(dei_ask.database.remote[i].serial_number == dei_ask.data_fix.bits.sn)
        {
            dei_ask.database.remote[i].sync_count = dei_ask.data_enc.bits.sc  ;
            dei_ask.database.remote[i].disc       = dei_ask.data_enc.bits.disc;
            
            sync_count_update( dei_ask.sync_count_obj[i] , dei_ask.database.remote[i].sync_count); 
            
            TRACE_DEI_ASK("\n\r[LIB.DEI.ASK] Programming : sn already stored %08X\n\r" , dei_ask.database.remote[i].serial_number);
            
            return i ;// programming done
        }
    }
    
    TRACE_DEI_ASK("\n\r[LIB.DEI.ASK] Programming : new remote stored at %02X\n\r" , dei_ask.database.index );
    
    dei_ask.database.remote[dei_ask.database.index].sync_count    = dei_ask.data_enc.bits.sc  ;
    dei_ask.database.remote[dei_ask.database.index].disc          = dei_ask.data_enc.bits.disc;
    dei_ask.database.remote[dei_ask.database.index].serial_number = dei_ask.data_fix.bits.sn  ;
    dei_ask.database.remote[dei_ask.database.index].is_1button    = (dei_ask.data_fix.bits.button == DEI_ASK_BUTTON_START_STOP);
    
    
    sync_count_update( dei_ask.sync_count_obj[dei_ask.database.index],dei_ask.database.remote[dei_ask.database.index].sync_count); 
    
    i = dei_ask.database.index;
    
    dei_ask.database.index = ( ++ dei_ask.database.index) % DEI_ASK_MAX_REMOTES;
        
    return i;    
}

//--------------------------------------------------------------------------//    
