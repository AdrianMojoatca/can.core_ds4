/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: process_looping_start.c 14626 2012-05-24 11:26:47Z martin.bouchard $
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
Process_Node_Status process_start_analyse_suspend_list( Process_Node *new );
Process_Node_Status process_start_analyse_execute_list( Process_Node *new );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

void process_looping_start( Process_Node *new )
{    
    Process_Node_Status status_exec ; 
    Process_Node_Status status_suspend ; 
    
    if( !process_q_node_valid( &process_exec_list , new ) )
    {
        TRACE_PROCESS("\n\r[process start :  %s already in executing list]\n\r", new->name );    

        return ; 
    }

    if( !process_q_node_valid( &process_suspend_list , new ) )
    {
        TRACE_PROCESS("\n\r[process start :  %s already in suspending list]\n\r", new->name );    

        return ; 
    }
    

    TRACE_PROCESS("\n\r[---------------------process start : evaluate the new process %s]\n\r", new->name );

    status_exec    = process_start_analyse_execute_list ( new );
    status_suspend = process_start_analyse_suspend_list ( new ); 
    
    if( status_exec == PROCESS_NODE_READY && status_suspend == PROCESS_NODE_READY )
    {
        // --> start the process and insert in exec
        TRACE_PROCESS("\n\r[process start : the new process %s is started]\n\r", new->name );
        
        process_state_start( new ) ; 
        
        process_q_link( &process_exec_list , new );   
    }
    else
    if( (status_exec&PROCESS_NODE_BLOCKED) || (status_suspend&PROCESS_NODE_BLOCKED ) )
    {
        // --> the new process is blocked
        TRACE_PROCESS("\n\r[process start : the new process %s is blocked]\n\r", new->name );
    }
    else
    {
        // --> the new process is suspended
        TRACE_PROCESS("\n\r[process start : the new process %s is suspended]\n\r", new->name );
        
        process_q_link( &process_suspend_list , new  );
    }               
}

/*--------------------------------------------------------------------------*/

Process_Node_Status process_start_analyse_suspend_list( Process_Node *new )
{
    static Process_List  tmp_list     ;

    Process_Node* curr                ;    
    
    Boolean   curr_blocked           ;
    Boolean   curr_suspended         ;

    Boolean   new_blocked  = FALSE   ;
    Boolean   new_suspended= FALSE   ; 
        
    process_q_init( &tmp_list );

    TRACE_PROCESS("\n\r[process start : analyse the suspending list]\n\r") ;

    process_q_init( &tmp_list );

    #pragma push
    #pragma diag_suppress 1293
    
    while( curr = process_q_head( &process_suspend_list ) )
    {
        
    #pragma pop
        process_q_unlink( curr ) ;
                        
        curr_blocked   = !!(curr->is_blocked_by     & new->id);
        curr_suspended = !!(curr->is_suspended_by   & new->id);

        if( !new_blocked )
        {
            new_blocked = !!(new->is_blocked_by     & curr->id);
        }

        if( !new_suspended )
        {
            new_suspended = !!(new->is_suspended_by & curr->id);
        }

        
        TRACE_PROCESS("-- process from suspending %s : "  , curr->name) ;
        TRACE_PROCESS(" %s is blocked && is %s suspended \n\r" , curr_blocked ? " " : "not" , curr_suspended ? " " : "not") ;
        TRACE_PROCESS("-- by the new process %s  "   , new->name) ;
        TRACE_PROCESS(" %s is blocked && is %s suspended \n\r" , new_blocked  ? " " : "not" , new_suspended  ? " " : "not") ;
                 

        switch( new_blocked*8+new_suspended*4+curr_blocked*2 + curr_suspended )
        {
            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =0 , new_suspended = 0 , curr_blocked = 0 , curr_suspended = 0 */
            /*--------------------------------------------------------------------------------------------------*/
            case 0 : 
            {                
                process_q_link( &tmp_list , curr );   // no change for current process , keep it in suspend list
            }
            break ; 

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =0 , new_suspended = 0 , curr_blocked = 0 , curr_suspended = 1 */
            /*--------------------------------------------------------------------------------------------------*/
            case 1 :
            {                
                process_q_link( &tmp_list , curr );    // no change for current process , keep it in suspend list
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =0 , new_suspended = 0 , curr_blocked = 1 , curr_suspended = 0                       */
            /*--------------------------------------------------------------------------------------------------*/
            case 2 :
            {                
                //remove the process from suspend list
            }
            break;
            
            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =0 , new_suspended = 0 , curr_blocked = 1 , curr_suspended = 1                       */
            /*--------------------------------------------------------------------------------------------------*/
            case 3 :
            {                
                //remove the process from suspend list
            }
            break;
             
            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =0 , new_suspended = 1 , curr_blocked = 0 , curr_suspended =0                        */
            /*--------------------------------------------------------------------------------------------------*/
            case 4 :
            {
               process_q_link( &tmp_list , curr );    // no change for current process , keep it in suspend list
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =0 , new_suspended = 1 , curr_blocked = 0 , curr_suspended =1                        */
            /*--------------------------------------------------------------------------------------------------*/
            case 5 :
            {
                process_q_link( &tmp_list , curr ); //keep the  curent process in suspend list   
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =0 , new_suspended = 1 , curr_blocked = 1 , curr_suspended =0                        */
            /*--------------------------------------------------------------------------------------------------*/
            case 6 :
            {
                //remove the process from suspend list              
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =0 , new_suspended = 1 , curr_blocked = 1 , curr_suspended =1                        */
            /*--------------------------------------------------------------------------------------------------*/
            case 7 :
            {
                //remove the process from suspend list                
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =1 , new_suspended = 0 , curr_blocked = 0 , curr_suspended = 0                       */
            /*--------------------------------------------------------------------------------------------------*/
            case 8 :
            {                
                process_q_link( &tmp_list , curr ); //keep the  curent process in suspend list      
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =1 , new_suspended = 0 , curr_blocked = 0 , curr_suspended = 1                       */
            /*--------------------------------------------------------------------------------------------------*/
            case 9 :
            {                
                process_q_link( &tmp_list , curr ); //keep the  curent process in suspend list  
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =1 , new_suspended = 0 , curr_blocked = 1 , curr_suspended = 0                       */
            /*--------------------------------------------------------------------------------------------------*/
            case 10 :
            {                
                TRACE_PROCESS("\n\r[!! ILEGAL CONDITION - cross blocking!!]\n\r") ;                                                                
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =1 , new_suspended = 0 , curr_blocked = 1 , curr_suspended = 1                       */
            /*--------------------------------------------------------------------------------------------------*/
            case 11 :
            {                                
                TRACE_PROCESS("\n\r[!! ILEGAL CONDITION - cross blocking!!]\n\r") ;                
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =1 , new_suspended = 1 , curr_blocked = 0 , curr_suspended = 0                       */
            /*--------------------------------------------------------------------------------------------------*/
            case 12 :
            {                
                process_q_link( &tmp_list , curr ); //keep the  curent process in suspend list                
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =1 , new_suspended = 1 , curr_blocked = 0 , curr_suspended = 1 */
            /*--------------------------------------------------------------------------------------------------*/
            case 13 :
            {                
                process_q_link( &tmp_list , curr ); //keep the  curent process in suspend list                               
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =1 , new_suspended = 1 , curr_blocked = 1 , curr_suspended = 0 */
            /*--------------------------------------------------------------------------------------------------*/
            case 14 :
            {                
                //keep the current process running 
                TRACE_PROCESS("\n\r[!! ILEGAL CONDITION - cross blocking!!]\n\r") ;                                               
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =1 , new_suspended = 1 , curr_blocked = 1 , curr_suspended = 1 */
            /*--------------------------------------------------------------------------------------------------*/
            case 15 :
            {                                
                TRACE_PROCESS("\n\r[!! ILEGAL CONDITION - cross blocking!!]\n\r") ;            
            }
            break;

            default : break;
        }

        TRACE_PROCESS("\n\r[process start : the state for analyse suspending list %d]\n\r", new_blocked*8+new_suspended*4+curr_blocked*2 + curr_suspended);                 
                        
   }

   process_q_move( &tmp_list , &process_suspend_list );
   
   return (Process_Node_Status)(2*new_suspended + new_blocked);   
}

/*--------------------------------------------------------------------------*/

Process_Node_Status process_start_analyse_execute_list( Process_Node *new )
{
    static Process_List  tmp_list    ;

    Process_Node* curr               ;    
    
    Boolean   curr_blocked           ;
    Boolean   curr_suspended         ;

    Boolean   new_blocked  = FALSE   ;
    Boolean   new_suspended= FALSE   ; 
    
    TRACE_PROCESS("\n\r[process start : analyse the executing list]\n\r") ;

    process_q_init( &tmp_list );
    
    #pragma push
    #pragma diag_suppress 1293

    while( (curr = process_q_head( &process_exec_list )) )
    {
        
    #pragma pop
        
        process_q_unlink( curr ) ;
                        
        curr_blocked   = !!(curr->is_blocked_by     & new->id);
        curr_suspended = !!(curr->is_suspended_by   & new->id);

        if( !new_blocked )
        {
            new_blocked = !!(new->is_blocked_by     & curr->id);
        }

        if( !new_suspended )
        {
            new_suspended = !!(new->is_suspended_by & curr->id);
        }

        
        TRACE_PROCESS("--process from execute list %s  :"  , curr->name) ;
        TRACE_PROCESS(" %s is blocked && is %s suspended ]\n\r" , curr_blocked ? " " : "not" , curr_suspended ? " " : "not") ;
        TRACE_PROCESS("--the new process %s  :"   , new->name) ;
        TRACE_PROCESS(" is %s blocked && is %s suspended ]\n\r" , new_blocked  ? " " : "not" , new_suspended  ? " " : "not") ;
                 

        switch( new_blocked*8+new_suspended*4+curr_blocked*2 + curr_suspended )
        {
            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =0 , new_suspended = 0 , curr_blocked = 0 , curr_suspended = 0 */
            /*--------------------------------------------------------------------------------------------------*/
            case 0 : 
            {                
                process_q_link( &tmp_list , curr );   // no change for current process , keep it in exec list
            }
            break ; 

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =0 , new_suspended = 0 , curr_blocked = 0 , curr_suspended = 1 */
            /*--------------------------------------------------------------------------------------------------*/
            case 1 :
            {
                process_state_stop( curr );                      // put the running process in suspend list    
                
                process_q_link( &process_suspend_list  , curr ) ;     
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =0 , new_suspended = 0 , curr_blocked = 1 , curr_suspended = 0                       */
            /*--------------------------------------------------------------------------------------------------*/
            case 2 :
            {                
                process_state_stop( curr );         // stop the current process because is blocked
            }
            break;
            
            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =0 , new_suspended = 0 , curr_blocked = 1 , curr_suspended = 1                       */
            /*--------------------------------------------------------------------------------------------------*/
            case 3 :
            {                
                process_state_stop( curr );          
                // stop the current process because is blocked and suspended                                                     
                // the dominating state in this case is blocking state
            }
            break;
             
            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =0 , new_suspended = 1 , curr_blocked = 0 , curr_suspended =0                        */
            /*--------------------------------------------------------------------------------------------------*/
            case 4 :
            {
                process_q_link( &tmp_list , curr );   // no change for current process , keep it in exec list    
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =0 , new_suspended = 1 , curr_blocked = 0 , curr_suspended =1                        */
            /*--------------------------------------------------------------------------------------------------*/
            case 5 :
            {
                TRACE_PROCESS("\n\r[!! ILEGAL CONDITION - cross suspending!!]\n\r") ;
                
                new_blocked = TRUE ;                   //ilegal condition , force the new blocked 
                
                process_q_link( &tmp_list , curr );   // no change for current process , keep it in exec list          
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =0 , new_suspended = 1 , curr_blocked = 1 , curr_suspended =0                        */
            /*--------------------------------------------------------------------------------------------------*/
            case 6 :
            {
                process_state_stop( curr );                                                  
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =0 , new_suspended = 1 , curr_blocked = 1 , curr_suspended =1                        */
            /*--------------------------------------------------------------------------------------------------*/
            case 7 :
            {
                process_state_stop( curr );                  
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =1 , new_suspended = 0 , curr_blocked = 0 , curr_suspended = 0                       */
            /*--------------------------------------------------------------------------------------------------*/
            case 8 :
            {                
                process_q_link( &tmp_list , curr );   // no change for current process , keep it in exec list    
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =1 , new_suspended = 0 , curr_blocked = 0 , curr_suspended = 1                       */
            /*--------------------------------------------------------------------------------------------------*/
            case 9 :
            {                
                //nothing to do for new , just keep the state
                process_q_link( &tmp_list , curr );   // no change for current process , keep it in exec list
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =1 , new_suspended = 0 , curr_blocked = 1 , curr_suspended = 0                       */
            /*--------------------------------------------------------------------------------------------------*/
            case 10 :
            {                
                TRACE_PROCESS("\n\r[!! ILEGAL CONDITION - cross blocking!!]\n\r") ;
                
                process_state_stop( curr );                                                  
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =1 , new_suspended = 0 , curr_blocked = 1 , curr_suspended = 1                       */
            /*--------------------------------------------------------------------------------------------------*/
            case 11 :
            {                                
                process_q_link( &tmp_list , curr );   // no change for current process , keep it in exec list
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =1 , new_suspended = 1 , curr_blocked = 0 , curr_suspended = 0                       */
            /*--------------------------------------------------------------------------------------------------*/
            case 12 :
            {                                
                process_q_link( &tmp_list , curr );   // no change for current process , keep it in exec list              
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =1 , new_suspended = 1 , curr_blocked = 0 , curr_suspended = 1 */
            /*--------------------------------------------------------------------------------------------------*/
            case 13 :
            {                
                process_q_link( &tmp_list , curr );   // no change for current process , keep it in exec list                              
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =1 , new_suspended = 1 , curr_blocked = 1 , curr_suspended = 0 */
            /*--------------------------------------------------------------------------------------------------*/
            case 14 :
            {                
                TRACE_PROCESS("\n\r[!! ILEGAL CONDITION - cross blocking!!]\n\r") ;
                
                process_state_stop( curr );                                                                                                    
            }
            break;

            /*--------------------------------------------------------------------------------------------------*/
            /* new_blocked =1 , new_suspended = 1 , curr_blocked = 1 , curr_suspended = 1 */
            /*--------------------------------------------------------------------------------------------------*/
            case 15 :
            {                
                //keep the current process running     

                TRACE_PROCESS("\n\r[!! ILEGAL CONDITION - cross blocking!!]\n\r") ;
                
                process_state_stop( curr );                                                                                                 
            }
            break;

            default : break;
        }
        
        TRACE_PROCESS("\n\r[process start : the state for analyse execurte list %d]\n\r", new_blocked*8+new_suspended*4+curr_blocked*2 + curr_suspended);                 
   }

   process_q_move( &tmp_list , &process_exec_list );
   
   return (Process_Node_Status)(2*new_suspended + new_blocked);   
}

/*--------------------------------------------------------------------------*/
