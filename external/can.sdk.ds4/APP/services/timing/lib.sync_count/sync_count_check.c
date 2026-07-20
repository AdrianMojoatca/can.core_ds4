/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sync_count_check.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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

Boolean sync_count_check( Sync_Count_Obj sync , Sync_Count new_count )
{
    UInt16 distance      ; 

    Boolean ret  = FALSE ; 

	if( sync->current_count < new_count )
	{
		distance = new_count - sync->current_count; 
	}
	else
	{
		distance = 0xFFFF - sync->current_count + new_count ; 
	};
		
    TRACE_DEI_SYNC("\n\r[LIB.SYNC_COUNT] 0x%08X CHECK\n\r", sync );  
    	
	if( (distance <= sync->open_window) && distance  )
	{
		sync->current_count = new_count ; 

        timeout_stop( (Timeout)sync ) ; 

        sync->is_locked = FALSE ; 

        sync->window_type = SYNC_WINDOW_OPEN ; 
        
        ret = TRUE ; 

        goto exit_sync_count_check ;
	};
	
        
    if( (distance > sync->open_window ) && (distance <= sync->resync_window) )
	{
        if( !sync->is_locked )
		{
			sync->resync_count = new_count ; 
                
            sync->is_locked = TRUE ; 

            sync->resync_index = 1 ; 

            timeout_start( (Timeout)sync ) ;

            sync->window_type = SYNC_WINDOW_RESYNC ; 
                      
            ret = FALSE ; 

            goto exit_sync_count_check ;
		}
		else
		{
		    if( (sync->resync_count + 1 ) == new_count  )
			{
				 if( ++sync->resync_index == sync->resync_index_max )
                 {
                    timeout_stop( (Timeout)sync ); 
                    
                    sync->current_count = new_count ; 
                        
                    sync->is_locked = FALSE ; 

                    sync->window_type = SYNC_WINDOW_OPEN ; 
                           
                    ret = TRUE ; 

                    goto exit_sync_count_check ;
                 }
                 else
                 {
                    sync->resync_count   ++ ; 

                    sync->window_type = SYNC_WINDOW_RESYNC ; 

                    ret = FALSE ; 

                    goto exit_sync_count_check ;
                 };                	
			}
            else
            {
                sync->window_type = SYNC_WINDOW_RESYNC ; 
                
                ret = FALSE ; 

                goto exit_sync_count_check ;
            };
		};
			
	};
			
	sync->window_type = SYNC_WINDOW_BLOCK ; 
    
    exit_sync_count_check :

    TRACE_DEI_SYNC(" - Windows type %d\n\r - Current count %d\n\r - Resync count %d\n\r - Resync index %d \n\r" , \
             sync->window_type , sync->current_count , sync->resync_count , sync->resync_index ) ; 

    return ret ; 
}

/*--------------------------------------------------------------------------*/

