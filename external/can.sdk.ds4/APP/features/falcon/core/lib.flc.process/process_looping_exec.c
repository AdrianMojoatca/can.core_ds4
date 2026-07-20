/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: process_looping_exec.c 14626 2012-05-24 11:26:47Z martin.bouchard $
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
static Boolean process_node_ready_for_exec      ( Process_Node *new );
static void    process_exec_analyse_suspend_list( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/                 

Per_Exec_Wake process_looping_exec( void )
{
    static Process_List tmp_list ; 

    Process_Node* process_exec = NULL ; 
    
    Boolean  wake_active = FALSE ;     

    process_q_init( &tmp_list  );
    
    #pragma push
    #pragma diag_suppress 1293
    
    while( !!(process_exec = process_q_head( &process_exec_list )) )
    {
        
    #pragma pop
        process_q_unlink( process_exec ) ;

        if( !process_state_exec(process_exec) )
        {
            process_q_link(&tmp_list , process_exec);
        }
        else
        {
            process_state_stop( process_exec );

            TRACE_PROCESS("\n\r[process execute : the process %s is stopped]\n\r", process_exec->name ); 
        }
    }
        
    process_q_move( &tmp_list , &process_exec_list  );


    process_exec_analyse_suspend_list() ; 

    
    return wake_active ? PER_EXEC_WAKE : PER_EXEC_SLEEP ;                  
}

/*--------------------------------------------------------------------------*/                 

void process_exec_analyse_suspend_list( void )
{
    Process_Node* curr = NULL ; 
    
    static Process_List tmp_list         ;    
    static Process_List exec_ready_list  ; 
    
    process_q_init( &tmp_list );
    process_q_init( &exec_ready_list  );
    
    #pragma push
    #pragma diag_suppress 1293
    
    while( curr = process_q_head( &process_suspend_list ) )
    {
       
    #pragma pop
        
        process_q_unlink( curr );

        if( process_node_ready_for_exec( curr ) )
        {
            TRACE_PROCESS("\n\r[process execute : the process %s is exit from suspending list]\n\r", curr->name ); 
            
            process_q_link( &exec_ready_list , curr );

            process_state_start( curr );

            break ; 
        }
        else
        {
            process_q_link( &tmp_list , curr );
        }
    }
                    
    process_q_move( &tmp_list        , &process_suspend_list );
    process_q_move( &exec_ready_list , &process_exec_list    );                     
}

/*--------------------------------------------------------------------------*/                 

Boolean process_node_ready_for_exec( Process_Node *new )
{
    static Process_List  tmp_list ;
    
    Boolean ready_for_exec = TRUE ; 

    Process_Node *curr            ; 
    
    
    process_q_init( &tmp_list );
    
    #pragma push
    #pragma diag_suppress 1293
    
    while( curr = process_q_head( &process_exec_list ) )
    {
        
    #pragma pop
        
        process_q_unlink( curr );
        
        if( new->is_suspended_by & curr->id  )
        {
            ready_for_exec = FALSE ; 
        }

        process_q_link( &tmp_list , curr );
    }
    
    process_q_move( &tmp_list , &process_exec_list );
    
    
    return ready_for_exec ; 
}    
            
/*--------------------------------------------------------------------------*/                 

