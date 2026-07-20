/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_sync_remove.c 19497 2013-02-25 13:41:22Z martin.bouchard $
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

Boolean can_sync_remove( Can_Dev_Id dev_id , void *func_or_arg )
{
    #define CAN_SYNC_OBJ     ((Can_Sync_Node*)curr)->can_sync_obj
    #define SYNC_FREE       can_sync[dev_id].free
    #define SYNC_EXEC       can_sync[dev_id].exec   
    #define RES             can_sync[dev_id].res     


    Boolean ret = FALSE ;
    
    Fifo_Node *curr , *temp ;

    if( os_res_request(RES , 200) )
    {		
    	for( curr = SYNC_EXEC->rhs ; curr != SYNC_EXEC ; curr = curr->rhs )
    	{		
    		if( (CAN_SYNC_OBJ.can_sync_func == func_or_arg) || (CAN_SYNC_OBJ.can_sync_arg == func_or_arg) )
    		{
    			temp = curr->rhs;
    			
    			fifo_unlink( curr ) ; 
    
    			fifo_insert( SYNC_FREE , curr ) ; 
    			
    			curr = temp->lhs ; 
    
                ret = TRUE ; 
    		}					 
    	}
    
        os_res_release(RES);
    }
    else
    {
        TRACE_CAN_SYNC("\n\r[CAN_SYNC_REMOVE] : TIMEOUT\n\r");
    }
        
    return ret ; 

    #undef CAN_SYNC_OBJ	
    #undef SYNC_FREE   
    #undef SYNC_EXEC   
    #undef RES         

}

//--------------------------------------------------------------------------//
