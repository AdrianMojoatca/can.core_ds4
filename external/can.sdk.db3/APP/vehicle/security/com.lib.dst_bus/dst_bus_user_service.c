/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dst_bus_user_service.c 34965 2015-11-04 11:27:14Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dst_bus_private.h"
#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void process_msg(Dst_Bus_Msg msg , Task_Callback  callback) ;

static void tag_rx_reset            (void) ;

static void tag_rx_data             (UInt8 data) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static Dst_Bus_Msg irq_tag_msg ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void dst_bus_user_service (void)
{  
  if(!!(dst_bus_vars.flag & DST_BUS_FLAG_READER))
    {
      //TRACE("\r\nDST_BUS_SERVICE: DST_BUS_FLAG_READER active\r\n");
	  dst_bus_clr_flag(DST_BUS_FLAG_READER) ;

      process_msg(dst_bus_vars.reader_msg , dst_bus_reader_process) ;
    }

  if(!!(dst_bus_vars.flag & DST_BUS_FLAG_TAG_RX_BYTE))
    {
      //TRACE("\r\nDST_BUS_SERVICE: DST_BUS_FLAG_TAG_RX_BYTE active\r\n");
	  dst_bus_clr_flag(DST_BUS_FLAG_TAG_RX_BYTE) ;

      //if(dst_bus_vars.fiq_rx_data)
        {
          tag_rx_data(dst_bus_vars.fiq_rx_data) ;
        }
      //else
//        {
//          dst_bus_err_add(DST_BUS_ERR_RX_FRAMING) ;
//        }
      
    }

  if(!!(dst_bus_vars.flag & DST_BUS_FLAG_TAG_RX_DONE))
    {
	  //TRACE("\r\nDST_BUS_SERVICE: DST_BUS_FLAG_TAG_RX_DONE active\r\n");
      dst_bus_clr_flag(DST_BUS_FLAG_TAG_RX_DONE) ;

      process_msg(dst_bus_vars.tag_msg , dst_bus_trans_process) ;
    }
}

//--------------------------------------------------------------------------//
static void process_msg(Dst_Bus_Msg msg , Task_Callback  callback)
{
  Dst_Bus_Q_Msg * rx_q_msg = (Dst_Bus_Q_Msg *) q_remove_c (dst_bus_vars.q_free);
  UInt8 i;

  if (rx_q_msg)
  {
	  //TRACE("\r\nPROCESS MSG RX: ");
	  //for ( i = 0; i < 30; i++) TRACE("%02X ", msg.data[i]);
	  //TRACE("\r\n");

      rx_q_msg->msg      = msg;

      rx_q_msg->callback = callback ;

      q_insert (dst_bus_vars.q_pending , (Q_Msg *) rx_q_msg) ;
  }
 else
    {
      dst_bus_err_add(DST_BUS_ERR_READER_NO_Q_FREE) ;
    }
}
//--------------------------------------------------------------------------//

static void  tag_rx_data (UInt8 data)
{        
  if(irq_tag_msg.size < sizeof_array(irq_tag_msg.data))
    {
      irq_tag_msg.data[irq_tag_msg.size] = data   ;
    
      irq_tag_msg.size++ ;
 
    }
  else
    {
      tag_rx_reset() ;
    }

  if(irq_tag_msg.size >= DST_BUS_TAG_DATA_SIZE)
    {
      dst_bus_tag_rx_done() ;
    }
  else
    {
      tmr_update(dst_bus_vars.tmr_tag_rx_timeout , DST_BUS_TAG_RX_DATA_TIMEOUT , FALSE) ; // keep as FALSE because is set from interupt not in fiq
      
      tmr_ena   (dst_bus_vars.tmr_tag_rx_timeout) ;
    }
}
//--------------------------------------------------------------------------//

void  dst_bus_tag_rx_done (void)
{
  tmr_dis(dst_bus_vars.tmr_tag_rx_timeout)        ;
    
  if(irq_tag_msg.size != 0)
    {
      dst_bus_vars.tag_msg = irq_tag_msg ;

      dst_bus_set_flag(DST_BUS_FLAG_TAG_RX_DONE) ;
    }
  tag_rx_reset() ;
}
//--------------------------------------------------------------------------//

static void  tag_rx_reset (void)
{
  memset (&irq_tag_msg , 0 , sizeof(irq_tag_msg)) ;    
}



