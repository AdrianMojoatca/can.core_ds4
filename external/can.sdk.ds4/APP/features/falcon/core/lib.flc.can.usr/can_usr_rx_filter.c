/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_usr_rx_reg.c 19497 2013-02-25 13:41:22Z martin.bouchard $
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

void can_usr_rx_filter_add(Can_Dev_Id dev_id , UInt32 can_id)
{
    if( can_usr_rx_filter_check_id(dev_id,can_id)==FALSE)
    {
        TRACE_CAN_USR("\n\rSW_FILTER_CAN_DEV_%d_ID_ADDED - ",dev_id==CAN_DEV_0 ? 0:1);
        TRACE_CAN_USR("%04x\n\r",can_id);

        TRACE_ZONE(os_tsk_wait(10));
        
        if( can_usr[dev_id].filter.size < CAN_USR_FILTER_TABLE_SIZE )
        {        
            ATOMIC(can_usr[dev_id].filter.table[can_usr[dev_id].filter.size++]=can_id);
        }
        else
        {
            TRACE_CAN_USR("\n\rSW_FILTER_CAN_DEV_%d_FILTER_FULL\n\r",dev_id==CAN_DEV_0 ? 0:1);            

            os_tsk_wait(100);
            
            while(1);
        }
    }
    else
    {
        //TRACE_CAN_USR("\n\rSW_FILTER_CAN_DEV_%d_ID_FOUND - ",dev_id==CAN_DEV_0 ? 0:1);
        //TRACE_CAN_USR("%04x\n\r",can_id);

        TRACE_ZONE(os_tsk_wait(10));
    }

}

//--------------------------------------------------------------------------//

Boolean can_usr_rx_filter_check_id(Can_Dev_Id dev_id , UInt32 id)
{
    UInt8 i=0;
    
    if(can_usr[dev_id].filter.enable==FALSE) return TRUE;
    
    for( i = 0 ; i < can_usr[dev_id].filter.size ; i++ )
    {
        if(id==can_usr[dev_id].filter.table[i])return TRUE;
    }

    return FALSE;
}

//--------------------------------------------------------------------------//

void can_usr_rx_filter_enable(Can_Dev_Id dev_id )
{
    ATOMIC(can_usr[dev_id].filter.enable=TRUE);
} 

//--------------------------------------------------------------------------//

void can_usr_rx_filter_disable(Can_Dev_Id dev_id )
{
    ATOMIC(can_usr[dev_id].filter.enable=FALSE);
} 

//--------------------------------------------------------------------------//
