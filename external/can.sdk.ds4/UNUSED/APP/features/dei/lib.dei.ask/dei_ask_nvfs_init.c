/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ask_nvfs_init.c 31193 2015-05-06 17:14:56Z martin.bouchard $
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
static void trace_stored_remotes( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

Boolean dei_ask_nvfs_init(void)
{
    Boolean status = TRUE ;
    
    if( (nvfs_size_is_valid(nvfs_rem_dei,sizeof(dei_ask.database))!= NVFS_OK) || 
        (nvfs_rd           (nvfs_rem_dei,&dei_ask.database      ) != NVFS_OK))            
    {              
        nvfs_delete(nvfs_rem_dei);
        
        TRACE_DEI_ASK("\n\r [LIB.DEI.ASK] --- remote database is empty\n\r");
        
        status = (nvfs_wr_create( nvfs_rem_dei,&dei_ask.database,sizeof(dei_ask.database)) == NVFS_OK);              
        
        if(status)
        {
            status = nvram_usr_commit() ;
        }
    }
    else
    {
        trace_stored_remotes();
    }
    
    return status;
}

//--------------------------------------------------------------------------//    

void trace_stored_remotes( void )
{
    UInt8 i = 0 ;
    
    TRACE_DEI_ASK("\n\r[LIB.DEI.ASK] -- remote list (%04x) \n\r" , dei_ask.database.index);
    
    for(i=0;i<DEI_ASK_MAX_REMOTES;i++)
    {
        TRACE_DEI_ASK(" - Remote %d\n\r" , i);
        TRACE_DEI_ASK("      * Serial number  %08x\n\r" ,dei_ask.database.remote[i].serial_number);
        TRACE_DEI_ASK("      * Sync count     %04x\n\r" ,dei_ask.database.remote[i].sync_count   );
        TRACE_DEI_ASK("      * Disc           %04x\n\r" ,dei_ask.database.remote[i].disc         );
        TRACE_DEI_ASK("      * One button     %01x\n\r" ,dei_ask.database.remote[i].is_1button   );
        
        TRACE_ZONE(os_tsk_wait(10));
    }
}

//--------------------------------------------------------------------------//    

