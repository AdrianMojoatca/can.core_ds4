/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: trace_tsk.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "trace_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#if USE_TRACE == 1

typedef void (* Trace_Func )( void* , ...) ; 

#endif

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

#if USE_TRACE == 1

void trace_tsk( void )
{
    #define ARG(x)  trace_q_msg->arg[x]

    Trace_Q_Msg *trace_q_msg ; 

    Boolean wake = FALSE ; 
        
    for( ; ; )
    {                       
        trace_q_msg = (Trace_Q_Msg *)q_remove( trace_vars.q_pending, 0 ) ;             
        
        wake_ck_enter();
        {        
        	if( trace_q_msg->str )
            {
            	#if TRACE_ARG_SIZE == 1
					t_printf( trace_q_msg->str , ARG(0)) ;                
				#endif
			
				#if TRACE_ARG_SIZE == 2
					t_printf( trace_q_msg->str , ARG(0) , ARG(1) ) ;                
				#endif			
				
				#if TRACE_ARG_SIZE == 3
					t_printf( trace_q_msg->str , ARG(0) , ARG(1), ARG(2) ) ;                
				#endif	
				
				#if TRACE_ARG_SIZE == 4
					t_printf( trace_q_msg->str , ARG(0) , ARG(1), ARG(2), ARG(3) ) ;                
				#endif	
            }					    
            else
            {
            	if( ARG(0) )
                {
                	#if TRACE_ARG_SIZE == 1
						((Trace_Func)( ARG(0)))() ;                
					#endif
			
					#if TRACE_ARG_SIZE == 2
						((Trace_Func)( ARG(0)))( ARG(1)) ;
					#endif			
				
					#if TRACE_ARG_SIZE == 3
						((Trace_Func)( ARG(0)))( ARG(1) , ARG(2)) ;
					#endif	
					
					#if TRACE_ARG_SIZE == 4
						((Trace_Func)( ARG(0)))( ARG(1) , ARG(2) , ARG(3)) ;
					#endif	
                }                
            }    
									                        
            q_return( (Q_Msg *) trace_q_msg ) ;
        
        }
        wake_ck_leave(); 
    }

    #undef ARG
}

#endif

/*--------------------------------------------------------------------------*/
