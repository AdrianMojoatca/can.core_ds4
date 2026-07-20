/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: trace_print.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "trace_private.h"
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

#if USE_TRACE == 1

Boolean trace_print(  const char *str , ... )
{
    Trace_Q_Msg* trace_q_msg ; 

    va_list arg ; 
        
    trace_q_msg = (Trace_Q_Msg *)q_remove_c( trace_vars.q_free ) ; 

    if( !trace_q_msg ) return FALSE ;
    
        
    trace_q_msg->str = (void *)str ; 

    va_start( arg , str );
    {		           
        #if TRACE_ARG_SIZE == 1
			trace_q_msg->arg[0] = (void *)va_arg( arg , VARG );
		#endif

		#if TRACE_ARG_SIZE == 2
			trace_q_msg->arg[0] = (void *)va_arg( arg , VARG );
			trace_q_msg->arg[1] = (void *)va_arg( arg , VARG );
		#endif

		#if TRACE_ARG_SIZE == 3
			trace_q_msg->arg[0] = (void *)va_arg( arg , VARG );
			trace_q_msg->arg[1] = (void *)va_arg( arg , VARG );
			trace_q_msg->arg[2] = (void *)va_arg( arg , VARG );
		#endif

		#if TRACE_ARG_SIZE == 4
			trace_q_msg->arg[0] = (void *)va_arg( arg , VARG );
			trace_q_msg->arg[1] = (void *)va_arg( arg , VARG );
			trace_q_msg->arg[2] = (void *)va_arg( arg , VARG );
			trace_q_msg->arg[3] = (void *)va_arg( arg , VARG );
		#endif						
    }
    va_end( arg );

    
    q_insert( trace_vars.q_pending , (Q_Msg *)trace_q_msg );
    
    return TRUE ;
}

#endif

/*--------------------------------------------------------------------------*/
