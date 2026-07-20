/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_sync_private.h 19497 2013-02-25 13:41:22Z martin.bouchard $
/*==========================================================================*/
#ifndef __CAN_SYNC_PRIVATE_H__
#define __CAN_SYNC_PRIVATE_H__
/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "fifo_private.h"
#include "out_private.h"
#include "can_sync.h"
#include "trace.h"
#include "can_usr.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#include "config_dbg.h"
#ifndef DBG_CAN_SYNC
#define DBG_CAN_SYN		0
#endif

#if DBG_CAN_SYNC == 1
#define TRACE_CAN_SYNC		TRACE
#else
#define TRACE_CAN_SYNC(...)   ;
#endif


//--------------------------------------------------------------------------//

typedef struct s_Can_Sync_Obj
{
    struct s_Out    out            ;     
    Can_Sync_Func   can_sync_func  ;     
    Can_Sync_Arg    can_sync_arg   ;
    UInt32          can_id         ;
    Boolean         req_done       ;
    Boolean         tx_status      ;
    
}Can_Sync_Obj ;

//--------------------------------------------------------------------------//

typedef struct s_Can_Sync_Node
{
	Fifo_Node node           ; 
	
    Can_Sync_Obj  can_sync_obj ;     
	
}Can_Sync_Node ; 

//--------------------------------------------------------------------------//

typedef struct s_Can_Sync
{
    Fifo_List free     ; 
    Fifo_List exec     ;

    Res       res      ;
    Can_Msg*  can_msg  ;

}Can_Sync ;

//--------------------------------------------------------------------------//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
Can_Sync_Node* can_sync_node_free_create( void );
void           can_sync_execute_dev0    ( Can_Msg *can_msg );
void           can_sync_execute_dev1    ( Can_Msg *can_msg );
void           can_sync_tx              ( Can_Dev_Id dev_id , Can_Sync_Obj *can_sync_obj );
void           can_sync_tx_status       ( Boolean execute   , Can_Sync_Obj *can_sync_obj );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Can_Sync can_sync[CAN_DEV_COUNT];
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

