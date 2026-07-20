/*==========================================================================*/
// $Id: dst_bus_trans_process.c 34684 2015-10-26 10:42:57Z martin.bouchard $
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

void dst_bus_trans_process (Dst_Bus_Msg msg)
{  
  #if DBG_DST_BUS == 1
    dst_bus_dbg_print("\nDst Bus Trans Msg Size   : " , &msg.size , 1) ;
    dst_bus_dbg_print(" Data : "                     , &msg.data[0] , msg.size) ;
  #endif

  //TRACE("\r\nDST_BUS_TRANS_PROCESS\r\n");
  
  if(msg.data[0] == DST_BUS_TAG_ID)
    {
      #define RD_ADDR ((msg.data[7] >> 2) & 0x3F)

       if(!dst_bus_vars.flg_gwr)
         {
           dst_trans(RD_ADDR ,&msg.data[1] , msg.size) ;
         }            
    }
  else if(msg.data[0] == DST_BUS_TAG_SYNC)
    {
      if(!dst_bus_vars.nvfs_entry.is_prog)
        {
          dst_bus_vars.nvfs_entry.reader_data_level   = !DST_BUS_READER_RX  ;
          dst_bus_vars.nvfs_entry.is_prog             = TRUE                ;

          nvfs_wr (nvfs_dst_bus_data, &dst_bus_vars.nvfs_entry) ;

          #if DBG_DST_BUS == 1
            dst_bus_dbg_print("\nDst Bus Reader Data Level: " , &dst_bus_vars.nvfs_entry.reader_data_level , 1) ;
          #endif
        }
    }
}
