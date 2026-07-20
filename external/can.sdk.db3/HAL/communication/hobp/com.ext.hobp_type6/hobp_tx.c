/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hobp_tx.c 44556 2016-11-29 13:03:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "hobp_private.h"
#include "hobp_bus.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static Hobp_Msg broadcast_msg ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
void hobp_tx(Hobp_Msg * msg)
{  
  timeout_stop(hobp_vars.tx_timeout)  ;
  
  hobp_bus_tx(&msg->data[0] , msg->size) ;

  #if DBG_HOBP == 1
    hobp_dbg_print("\nHoBp Tx:         " , &msg->data[0] , msg->size) ;
  #endif
}

//--------------------------------------------------------------------------//
void hobp_tx_broadcast(Hobp_Msg * msg , UInt32 timeout)
{
  timeout_stop(hobp_vars.tx_timeout)  ;
  
  broadcast_msg = *msg ;
  
  timeout_update(hobp_vars.tx_timeout , timeout) ;
  
  hobp_tx_timeout(NULL) ; 
}
//--------------------------------------------------------------------------//
void hobp_tx_timeout (void * arg )
{    
  if(hobp_vars.gwr && hobp_vars.ign)
    {
      hobp_tx(&broadcast_msg) ;

      timeout_start(hobp_vars.tx_timeout) ; 
    }

//  #if DBG_HOBP == 1
//    if(arg)
//      {
//        hobp_dbg_print("\nTx Timeout:" , &broadcast_msg.data[0] , broadcast_msg.size) ;
//      }
//  #endif



//  if(hobp_vars.ign && (hobp_vars.gwr || !hobp_vars.log_is_done))
//    {
//      hobp_bus_tx(&tx_msg.data[0] , tx_msg.size) ;  
//    
//      #if DBG_HOBP == 1
//        hobp_dbg_print("\nTx             :" , &tx_msg.data[0] , tx_msg.size) ;
//      #endif
//    
//      if(hobp_vars.state == HOBP_IMO_STATUS_PART1)
//        {
//          tx_msg  = hobp_msg[HOBP_MSG_IMO_STATUS2] ;
//    
//          hobp_vars.state = HOBP_IMO_STATUS_PART2 ;
//        }
//      //timeout_start (hobp_vars.tx_timeout)  ;
//    }
}
