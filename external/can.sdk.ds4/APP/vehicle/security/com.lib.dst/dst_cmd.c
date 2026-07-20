
/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dst_cmd.c 15918 2012-07-12 18:57:41Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dst_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define GEN_READ 0
#define PROGRAM  1
#define LOCK     2
#define SEL_READ 3

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
                                    
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

Dst_Trans_Resp dst_cmd(UInt8 cmd  , UInt8 * data , UInt8 size)
{  
  #define WR_CMD  (cmd & 0x03) 
  #define PAGE    (cmd >> 2)
  #define RD_ADDR (cmd & 0xFC) 
  
  Dst_Trans_Resp trans_resp ;
  UInt8 i;

  //TRACE("\r\nDST_CMD FUNCTION\r\n");

  memset(&trans_resp , 0 , sizeof(Dst_Trans_Resp)) ;
  
 #if DBG_DST == 1
   dst_dbg_print("\nDst Reader Cmd ", &cmd, 1);
   dst_dbg_print("\nDst Cmd Data   ", data , size);   
 #endif
 	//TRACE("\r\nDST READER CMD: %02X\r\n", cmd);
	//TRACE("\r\n DST READER DATA: ");
	for ( i = 0; i < size; i++ )
	{
		TRACE("%02X ", data[i]);
	}  
	//TRACE("\r\nCMD FOLLOWS: %02X", WR_CMD);

  switch(WR_CMD)
    {
      case GEN_READ : trans_resp = dst_cmd_gen_read  (PAGE , data , size) ;                                      break ;
      case PROGRAM  : trans_resp = dst_cmd_write     (PAGE , data , size) ;  trans_resp.item.rd_addr = PROGRAM ; break ;
      case LOCK     : trans_resp = dst_cmd_lock      (PAGE)               ;                                      break ;
      case SEL_READ : trans_resp = dst_cmd_sel_read  (PAGE , data , size) ;                                      break ;
      default       :                                                        return (trans_resp);
    }

  {
    Boolean status = dst_get_lock_status(PAGE) & 1 ;
    
    trans_resp.item.rd_addr |= RD_ADDR  | status << 1 ;
  }
  

  return trans_resp ;

}
