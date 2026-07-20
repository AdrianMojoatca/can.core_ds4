/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_sync_execute.c 19497 2013-02-25 13:41:22Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "can_sync_private.h"
#include "fifo_private.h"
#include "wake.h"
#include "trace.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void execute     ( Can_Msg *can_msg , Can_Dev_Id dev_id );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void  can_sync_execute_dev0( Can_Msg *can_msg )
{
    execute( can_msg , CAN_DEV_0 );
}    

//--------------------------------------------------------------------------//

void  can_sync_execute_dev1( Can_Msg *can_msg )
{
    execute( can_msg , CAN_DEV_1 );
}    

//--------------------------------------------------------------------------//

void execute( Can_Msg *can_msg , Can_Dev_Id dev_id )
{
    #define CAN_SYNC_OBJ    (((Can_Sync_Node*)curr)->can_sync_obj)
    #define SYNC_FREE       can_sync[dev_id].free
    #define SYNC_EXEC       can_sync[dev_id].exec   
    #define RES             can_sync[dev_id].res     

	Fifo_Node *curr , *temp ;
    
    if( os_res_request( RES , 200 ) )
    {    
        can_sync[dev_id].can_msg = can_msg ; 
    	
    	for( curr = SYNC_EXEC->rhs ; curr != SYNC_EXEC ; curr = curr->rhs )
    	{    		            
            if( CAN_SYNC_OBJ.can_id == can_msg->id )
            {                
                if( out_exec( NULL , (Out)(&CAN_SYNC_OBJ)) || CAN_SYNC_OBJ.req_done )
        		{
        			temp = curr->rhs ; 
        			
        			fifo_unlink( curr ) ; 
        
        			fifo_insert( SYNC_FREE , curr ) ; 
        			
        			curr = temp->lhs ; 
        		}
                else
                {
                    can_sync_tx( dev_id , &CAN_SYNC_OBJ );                    
                }        		
            }
    	}
        
        os_res_release( RES );	
    }
    
    	
    #undef CAN_SYNC_OBJ	
    #undef SYNC_FREE   
    #undef SYNC_EXEC   
    #undef RES   
}
        
//--------------------------------------------------------------------------//


