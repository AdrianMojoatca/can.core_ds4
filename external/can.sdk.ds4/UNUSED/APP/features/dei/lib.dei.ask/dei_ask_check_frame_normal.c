/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ask_check_frame_normal.c 31299 2015-05-08 14:32:26Z martin.bouchard $
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

UInt8 dei_ask_check_frame_normal_mode( void )
{
    UInt8 i ;
    
    if( (dei_ask.data_fix.bits.sn == 0xFFFFFFFF) || (dei_ask.data_fix.bits.sn == 0x00000000) )
    {    
        return DEI_ASK_MAX_REMOTES; //invalid frame
    }

    if( (dei_ask.data_enc.data == dei_ask.last_data_enc) && (dei_ask.data_fix.data == dei_ask.last_data_fix))
    {      
      for(i=0;i<DEI_ASK_MAX_REMOTES;i++)
       {
         if(dei_ask.database.remote[i].serial_number == dei_ask.data_fix.bits.sn)
           {               
             return i;
           }
       }
    }
    
    for(i=0;i<DEI_ASK_MAX_REMOTES;i++)
    {
        if(dei_ask.database.remote[i].serial_number == dei_ask.data_fix.bits.sn)
        {
            if(dei_ask.database.remote[i].disc == dei_ask.data_enc.bits.disc)
            {                
                if( sync_count_check( dei_ask.sync_count_obj[i] , dei_ask.data_enc.bits.sc))
                {
                    dei_ask.database.remote[i].sync_count = dei_ask.data_enc.bits.sc;
                    
                    return i ;
                }
                else
                {
                    TRACE_DEI_ASK("\n\r[LIB.DEI.ASK] Invalid Sync Count %04x \n\r" , dei_ask.data_enc.bits.sc );
                    
                    return DEI_ASK_MAX_REMOTES;
                }
            }
            else
            {
                TRACE_DEI_ASK("\n\r[LIB.DEI.ASK] Invalid Disc %04x \n\r" , dei_ask.data_enc.bits.disc );
                
                return DEI_ASK_MAX_REMOTES;
            }
        }        
    }
    
    TRACE_DEI_ASK("\n\r[LIB.DEI.ASK] Remote not found in database %08x \n\r" , dei_ask.data_fix.bits.sn );
    
    return DEI_ASK_MAX_REMOTES;
}

//--------------------------------------------------------------------------//    
