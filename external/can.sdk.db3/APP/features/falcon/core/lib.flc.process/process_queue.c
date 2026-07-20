/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: process_queue.c 14626 2012-05-24 11:26:47Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "process_private.h" 
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

void process_q_init (Process_List *process_list )
{
  process_list->rhs = process_list->lhs = (Process_Node*)process_list ;
}

/*--------------------------------------------------------------------------*/

void process_q_unlink (Process_Node * process_node)
{
  process_node->lhs->rhs = process_node->rhs ;
  process_node->rhs->lhs = process_node->lhs ;
}

/*--------------------------------------------------------------------------*/

void process_q_link (Process_List * process_list , Process_Node * process_node)
{
  process_node->lhs      = (Process_Node*)process_list  ;
  process_node->rhs      = process_list->rhs        ;
  process_node->rhs->lhs = process_node             ;

  process_list->rhs      = process_node             ;
}

/*--------------------------------------------------------------------------*/

Process_Node* process_q_head (Process_List * process_list)
{
  return process_list->lhs == (Process_Node*)process_list ? NULL : process_list->lhs ;
}

/*--------------------------------------------------------------------------*/

void process_q_move( Process_List *source , Process_List *dest )
{
    Process_Node *curr ; 
    
    #pragma push
    #pragma diag_suppress 1293
    
    while( curr = process_q_head( source ) )
    {
        
    #pragma pop
        process_q_unlink( curr );
        
        process_q_link( dest , curr );        
    }
}    

/*--------------------------------------------------------------------------*/

Boolean process_q_node_valid( Process_List *list_for_check , Process_Node *new )
{
    Process_List tmp_list       ; 
    Process_Node *curr          ;    

    Boolean is_valid = TRUE ;

    process_q_init( &tmp_list ) ;
    
    #pragma push
    #pragma diag_suppress 1293

    while( (curr = process_q_head( list_for_check)) )// --> check if already exist in list
    {
        
    #pragma pop
        
        process_q_unlink( curr );
        process_q_link  ( &tmp_list , curr );

        if( curr == new )
        {
            is_valid = FALSE ;
        }
    } 
    
    process_q_move( &tmp_list , list_for_check );    

    return is_valid ; 
}

/*--------------------------------------------------------------------------*/
        


