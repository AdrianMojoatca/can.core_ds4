/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_sync_tx.c 19497 2013-02-25 13:41:22Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "can_sync_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void copy_can_msg( Can_Msg *can_msg_source , Can_Msg *can_msg_dest );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void can_sync_tx( Can_Dev_Id dev_id , Can_Sync_Obj *can_sync_obj )
{
    Can_Msg        can_msg ; 
    
    Can_Sync_Flags flags ; 

    if( !can_sync_obj->tx_status )return ;

    copy_can_msg( can_sync[dev_id].can_msg , &can_msg ); 
    
    if( can_sync_obj->can_sync_func )
    {
        flags = (can_sync_obj->can_sync_func)( &can_msg , can_sync_obj->can_sync_arg ) ;
        
        if( flags&CAN_SYNC_COMPLETE )  can_sync_obj->req_done = TRUE ;                  

        if( flags&CAN_SYNC_TX       )  can_usr_tx( dev_id , &can_msg , TRUE , 10 );                                 
    }
}

//--------------------------------------------------------------------------//


void copy_can_msg( Can_Msg *can_msg_source , Can_Msg *can_msg_dest )
{
   can_msg_dest->id           = can_msg_source->id; 
   can_msg_dest->is_rtr       = can_msg_source->is_rtr; 
   can_msg_dest->is_ext       = can_msg_source->is_ext;
   can_msg_dest->length       = can_msg_source->length;
   can_msg_dest->data._64_[0] = can_msg_source->data._64_[0];
}         

//--------------------------------------------------------------------------//

