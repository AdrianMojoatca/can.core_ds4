/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_usr_rx_tsk.c 19497 2013-02-25 13:41:22Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "can_usr_private.h"
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

//--------------------------------------------------------------------------//

void can_usr_rx_tsk( void* dev_id )
{
    #pragma push
    #pragma diag_suppress 767
        UInt8 DEV_ID = (UInt8)dev_id;
    #pragma pop
    
    Can_Msg can_msg ;     
    
    for( ; ; )
    {
        can_rx( can_usr[DEV_ID].rx_can , &can_msg , 0 ); 
        
        //TRACE_CAN_USR("[CAN_USR_RX_TSK %s] - %08x (%d)\n\r" , DEV_ID == CAN_DEV_1 ? "CAN_DEV_1" : "CAN_DEV_0",can_msg.id,DEV_ID);
        
        if(can_usr_rx_filter_check_id((Can_Dev_Id)DEV_ID,can_msg.id)==TRUE)
        {
            wake_enter();
            {                
                reg_execute( can_usr[DEV_ID].rx_reg_fast , 0 , &can_msg );
                
                if( can_usr[DEV_ID].rx_use_buffer )
                {                
                    if( !pipe_insert( can_usr[DEV_ID].rx_pipe , &can_msg , 10 ) )
                    {
                        TRACE_CAN_USR("[CAN_USR_RX_TSK_PIPE_FULL %s]\n\r" , DEV_ID == CAN_DEV_1 ? "CAN_DEV_1" : "CAN_DEV_0");
        
                        can_usr_err_add( (DEV_ID == CAN_DEV_0) ? CAN_USR_ERR_DEV0_RX_PIPE_FULL : CAN_USR_ERR_DEV1_RX_PIPE_FULL );
                    }
                }
                else
                {
                    reg_execute( can_usr[DEV_ID].rx_reg_slow , can_msg.id , &can_msg );
                }                    
            }
            wake_leave();
        }
    }        
}

//--------------------------------------------------------------------------//

void can_usr_rx_handler_tsk( void *dev_id )
{
    #pragma push
    #pragma diag_suppress 767
        UInt8 DEV_ID = (UInt8)dev_id;
    #pragma pop
    
    Can_Msg can_msg ; 
    
    for( ; ; )
    {
        pipe_remove( can_usr[DEV_ID].rx_pipe , &can_msg , 0 ) ; 
        
        //TRACE_CAN_USR("[CAN_USR_RX_PIPE %s] - %08x (%d)\n\r" , DEV_ID == CAN_DEV_1 ? "CAN_DEV_1" : "CAN_DEV_0",can_msg.id,DEV_ID);

        wake_enter();
        {
            reg_execute( can_usr[DEV_ID].rx_reg_slow , can_msg.id , &can_msg );
        }
        wake_leave();
    }    
}

//--------------------------------------------------------------------------//

