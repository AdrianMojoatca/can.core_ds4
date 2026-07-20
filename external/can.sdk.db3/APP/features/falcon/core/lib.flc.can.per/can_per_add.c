/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_per_add.c 19497 2013-02-25 13:41:22Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "can_per_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Boolean per_add( Can_Per_Func can_per_func , Can_Per_Arg can_per_arg , UInt32 offset ,  UInt32 repeat_period , UInt8  repeat_count );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

Boolean can_per_add( Can_Per_Func can_per_func , Can_Per_Arg can_per_arg , UInt32 offset ,  UInt32 repeat_period , UInt8  repeat_count )
{
    Boolean ret ; 
    
    PER_EXEC_ATOMIC(ret =  per_add( can_per_func , can_per_arg , offset , repeat_period , repeat_count ); );

    return ret ;
}

//--------------------------------------------------------------------------//

Boolean per_add( Can_Per_Func can_per_func , Can_Per_Arg can_per_arg , UInt32 offset ,  UInt32 repeat_period , UInt8  repeat_count )
{	
	Boolean ret = FALSE ;
	
	Can_Per_Node *can_per_node = (Can_Per_Node*)fifo_remove( can_per_fifo_free ); 	
			
	insert_in_pennding:
	{
		if( can_per_node )
		{			                        
            can_per_node->can_per_obj.req_done = FALSE ; 
            
            can_per_node->can_per_obj.can_per_func = can_per_func ; 
            can_per_node->can_per_obj.can_per_arg  = can_per_arg  ;
			
			fifo_insert( can_per_fifo_exec , (Fifo_Node*) can_per_node ); 

            #define REPEAT      (repeat_period != 1) ? repeat_period-1 : 1
													
			out_exec_start( (Out)(&(can_per_node->can_per_obj))  ,offset , REPEAT , 1 , repeat_count );

            #undef  REPEAT
	
			ret = TRUE ;
		}
		else
		{
			can_per_node = can_per_node_free_create(); 
	
			if( can_per_node ) goto insert_in_pennding ;             
		}
	}
					
	return ret ;
}

//--------------------------------------------------------------------------//
