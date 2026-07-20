/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sync_count_create.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sync_count_private.h" 
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

/*--------------------------------------------------------------------------*/

Sync_Count_Obj sync_count_create( Sync_Count open_window , Sync_Count resync_window , UInt8 resync_index_max , TskTimeout timeout_resync_ms )
{
    Sync_Count_Obj sync_count = (Sync_Count_Obj)timeout_build (timeout_resync_ms, (Timeout_Func)sync_count_reset , sizeof ( struct s_Sync_Count_Obj) ) ;    
    
    sync_count->open_window   = open_window ;     
    sync_count->resync_window = resync_window ; 

    sync_count->current_count      = 0 ;    
    sync_count->resync_count       = 0 ;
    sync_count->resync_index       = 0 ;
    
    sync_count->is_locked          = FALSE ; 

    sync_count->resync_index_max   = resync_index_max ;

    TRACE_DEI_SYNC("\n\r[LIB.SYNC_COUNT] 0x%08X %s \n\r" , sync_count , sync_count ? "INIT OK" : "INIT ERROR" ) ; 
    
    return sync_count ;        
}

/*--------------------------------------------------------------------------*/

