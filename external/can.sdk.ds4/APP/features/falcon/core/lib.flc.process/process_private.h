/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: process_private.h 16559 2012-08-12 21:50:08Z florin.olariu $
/*==========================================================================*/

#ifndef __PROCESS_PRIVATE_H__
#define __PROCESS_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "process.h"
#include "per_exec.h"
#include "timeout_q.h"
#include "trace.h"
#include "config_dbg.h" 
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

typedef struct s_Process_Node
{
    struct s_Process_Node  *lhs   ; 
    struct s_Process_Node  *rhs   ;
    
    UInt8*  name                  ;
    
    Process_Exec   start          ; 
    Process_Exec   stop           ;        
    Process_Exec   exec           ;
    
    UInt32         timeout        ;
    Boolean        no_timeout     ;
    
    UInt32         tmr            ;    
    Boolean        start_done     ;    

    Process_Id    id              ;    
    Process_Id    is_blocked_by   ;
    Process_Id    is_suspended_by ;

}Process_Node ;
        
/*--------------------------------------------------------------------------*/    

typedef struct s_Process_List
{
    Process_Node *lhs ; 
    Process_Node *rhs ; 

}Process_List ;  

/*--------------------------------------------------------------------------*/

typedef enum
{
    PROCESS_NODE_READY     = 0     , 
    PROCESS_NODE_BLOCKED   = 1u<<0 , 
    PROCESS_NODE_SUSPENDED = 1u<<1 

}Process_Node_Status ;   

/*--------------------------------------------------------------------------*/

#ifndef DEBUG_PROCESS
#define DEBUG_PROCESS			0
#endif

#if DEBUG_PROCESS == 1
#define TRACE_PROCESS			TRACE
#else
#define TRACE_PROCESS(...)		;
#endif

/*--------------------------------------------------------------------------*/

#define PROCESS_START_GUARD_TIMEOUT             30  // --> 300ms guard time for start

/*--------------------------------------------------------------------------*/    

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void            process_init         ( void );

void            process_q_init       ( Process_List* process_list );
Process_Node*   process_q_head       ( Process_List* process_list );
void            process_q_unlink     ( Process_Node* process_node );
void            process_q_link       ( Process_List* process_list   , Process_Node* process_node );
void            process_q_move       ( Process_List* source         , Process_List* dest         );
Boolean         process_q_node_valid ( Process_List *list_for_check , Process_Node *new          );



void            process_looping_start( Process_Node *new );
Per_Exec_Wake   process_looping_exec ( void );
void            process_looping      ( void );

void            process_state_start  ( Process_Node* process );
Boolean         process_state_exec   ( Process_Node* process );
void            process_state_stop   ( Process_Node* process );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Process_List   process_exec_list   ; 
extern Process_List   process_suspend_list;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif



