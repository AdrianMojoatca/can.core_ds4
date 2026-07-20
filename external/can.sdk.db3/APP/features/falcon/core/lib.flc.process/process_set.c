/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: process_set.c 14626 2012-05-24 11:26:47Z martin.bouchard $
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

void  process_set ( Process_Node *process , Process_Set set , void *set_param )
{
    #pragma push
    #pragma diag_suppress 767
    
    ATOMIC
    (    
        if( process )
        {    
            
            switch( set )
            {
                case PROCESS_SET_START           : process->start           = (Process_Exec)set_param   ; break ;
                case PROCESS_SET_EXEC            : process->exec            = (Process_Exec)set_param   ; break ;
                case PROCESS_SET_STOP            : process->stop            = (Process_Exec)set_param   ; break ;                
                case PROCESS_SET_ID              : process->id              = (Process_Id  )set_param   ; break ;
                case PROCESS_SET_BLOCKED_BY_ID   : process->is_blocked_by   = (Process_Id  )set_param   ; break ;
                case PROCESS_SET_SUSPENDED_BY_ID : process->is_suspended_by = (Process_Id  )set_param   ; break ;

                case PROCESS_SET_TIMEOUT         :
                { 
                    process->timeout    = (UInt16  )set_param/10;                     
                    process->no_timeout = !process->timeout     ; 
                }                           
                break ;
        
                default : break ;
            }
        }
    )
        
    #pragma pop
}

/*--------------------------------------------------------------------------*/

