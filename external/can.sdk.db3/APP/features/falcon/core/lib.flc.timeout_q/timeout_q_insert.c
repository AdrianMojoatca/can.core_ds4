/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: timeout_q_insert.c 14626 2012-05-24 11:26:47Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "timeout_q_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Boolean _insert_( Timeout_Q_Func timeout_q_func , Timeout_Q_Arg timeout_q_arg , UInt32 offset ,  UInt32 repeat_period , UInt8  repeat_count );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

Boolean timeout_q_insert( Timeout_Q_Func timeout_q_func , Timeout_Q_Arg timeout_q_arg , UInt32 offset ,  UInt32 repeat_period , UInt8  repeat_count )
{
    Boolean ret ; 
    
    PER_EXEC_ATOMIC(ret =  _insert_( timeout_q_func , timeout_q_arg , offset , repeat_period , repeat_count ); );

    return ret ;
}

//--------------------------------------------------------------------------//

Boolean _insert_( Timeout_Q_Func timeout_q_func , Timeout_Q_Arg timeout_q_arg , UInt32 offset ,  UInt32 repeat_period , UInt8  repeat_count )
{	
	Boolean ret = FALSE ;

    Timeout_Q_Node *timeout_q_node  = NULL ;
    
    timeout_q_init( TIMEOUT_Q_DEFAULT_SIZE );
	
	timeout_q_node = (Timeout_Q_Node*)fifo_remove( timeout_q_fifo_free ); 	
			
	insert_in_pennding:
	{
		if( timeout_q_node )
		{			                        
            timeout_q_node->timeout_q_obj.req_done = FALSE ; 
            
            timeout_q_node->timeout_q_obj.timeout_q_func = timeout_q_func ; 
            timeout_q_node->timeout_q_obj.timeout_q_arg  = timeout_q_arg  ;
			
			fifo_insert( timeout_q_fifo_exec , (Fifo_Node*) timeout_q_node ); 

            #define REPEAT      (repeat_period != 1) ? repeat_period-1 : 1
													
			out_exec_start( (Out)(&(timeout_q_node->timeout_q_obj))  ,offset , REPEAT , 1 , repeat_count );

            #undef  REPEAT
	
			ret = TRUE ;
		}
		else
		{
			timeout_q_node = timeout_q_free_create(); 
	
			if( timeout_q_node ) goto insert_in_pennding ;             
		}
	}
					
	return ret ;
}

//--------------------------------------------------------------------------//
