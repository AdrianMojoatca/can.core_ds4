/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_sync_add.c 19497 2013-02-25 13:41:22Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "can_sync_private.h"
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

Boolean can_sync_add( Can_Dev_Id dev_id , Can_Sync_Func can_sync_func , Can_Sync_Arg can_sync_arg , Can_Sync_Id can_id , UInt32 sync_offset ,  UInt32 sync_on , UInt32 sync_off , UInt8  sync_repeat )
{	
	#define SYNC_FREE       can_sync[dev_id].free
    #define SYNC_EXEC       can_sync[dev_id].exec    
    #define RES             can_sync[dev_id].res     
    
    Boolean ret = FALSE ;

    if( os_res_request( RES , 200 ) )
    {	    	        
        Can_Sync_Node *can_sync_node = (Can_Sync_Node*)fifo_remove( SYNC_FREE ); 	

        can_usr_rx_filter_add( dev_id,can_id);
    			
    	insert_in_pennding:
    	{
    		if( can_sync_node )
    		{			                                                
                can_sync_node->can_sync_obj.can_sync_func = can_sync_func ; 
                can_sync_node->can_sync_obj.can_sync_arg  = can_sync_arg  ;
                can_sync_node->can_sync_obj.can_id        = can_id        ;
                can_sync_node->can_sync_obj.req_done      = FALSE         ;
                can_sync_node->can_sync_obj.tx_status     = FALSE         ;
    			
    			fifo_insert( SYNC_EXEC , (Fifo_Node*) can_sync_node ); 
                													
    			out_exec_start( (Out)(&(can_sync_node->can_sync_obj))  , sync_offset , sync_on , sync_off , sync_repeat );
                
    			ret = TRUE ;
    		}
    		else
    		{
    			can_sync_node = can_sync_node_free_create(); 
    	
    			if( can_sync_node ) goto insert_in_pennding ;             
    		}
    	}
        
        os_res_release( RES );
    }

    					
	return ret ;
}

//--------------------------------------------------------------------------//
