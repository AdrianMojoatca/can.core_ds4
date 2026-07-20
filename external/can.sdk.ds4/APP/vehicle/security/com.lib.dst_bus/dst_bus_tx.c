/*==========================================================================*/
// $Id: dst_bus_tx.c 34683 2015-10-25 16:42:09Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dst_bus_private.h"

#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define STATE_TAG_ID   0
#define STATE_TAG_DATA 1
#define STATE_TAG_BCC  2

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void tx_process (UInt8 data) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
                   
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void dst_bus_build_tx_msg(Dst_Trans_Resp trans_resp)
{
  UInt16 bcc   ;

  memset(&dst_bus_vars.tx_msg , 0 , sizeof(Dst_Bus_Msg)) ;

  dst_bus_vars.tx_msg.size    = 10 ;
  dst_bus_vars.tx_msg.data[0] = DST_BUS_TAG_ID ;
  
  memcpy(&dst_bus_vars.tx_msg.data[1] , &trans_resp.buff[0] , sizeof_array(trans_resp.buff )) ;

  dst_bcc(&bcc , &dst_bus_vars.trans_resp.buff[0] , sizeof_array(dst_bus_vars.trans_resp.buff)) ;

  dst_bus_vars.tx_msg.data[8] = (bcc    & 0xFF) ;
  dst_bus_vars.tx_msg.data[9] = (bcc>>8 & 0xFF) ; 
 
  #if DBG_DST_BUS == 1
    dst_bus_dbg_print("\nDst Bus Trans Tx Msg Size: " , &dst_bus_vars.tx_msg.size    , 1) ;
    dst_bus_dbg_print(" Data : "                      , &dst_bus_vars.tx_msg.data[0] , dst_bus_vars.tx_msg.size) ;
  #endif
}
