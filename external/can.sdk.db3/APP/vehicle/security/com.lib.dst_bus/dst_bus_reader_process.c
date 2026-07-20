/*==========================================================================*/
// $Id: dst_bus_reader_process.c 34922 2015-11-02 21:11:00Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dst_bus_private.h"
#include "dbg.h"

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

void dst_bus_reader_process (Dst_Bus_Msg msg)
{
  UInt8 i;
  #if DBG_DST_BUS == 1
    
    dst_bus_dbg_print("\nDst Bus Reader Msg Size  : " , &msg.size , 1) ;
    dst_bus_dbg_print(" Data : "                      , &msg.data[0] , msg.size+1) ;
  #endif
  
  //TRACE("\r\nDST_BUS_READER_PROCESS: ");
  //for ( i = 0; i < msg.size+1; i++ ) TRACE("%02X ", msg.data[i]);
  //TRACE("DST_CMD WILL FOLLOW\r\n");

  dst_bus_vars.trans_resp = dst_cmd(msg.data[0]  , &msg.data[1] , msg.size) ;

  //TRACE("\r\nDST_BUS_READER_PROCESS TRANS RESP: ");
  //for ( i = 0; i < msg.size; i++ ) TRACE("%02X ", dst_bus_vars.trans_resp.buff[i]);
  //TRACE("\r\n"); 

  if(dst_bus_vars.flg_gwr)
    {
      dst_bus_build_tx_msg(dst_bus_vars.trans_resp) ;
    }

}
