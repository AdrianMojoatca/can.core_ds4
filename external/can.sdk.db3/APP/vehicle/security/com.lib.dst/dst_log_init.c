
/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dst_log_init.c 21825 2013-07-25 18:42:00Z louis-philippe.rispoli $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dst_private.h"

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
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

void dst_log_init(void)
{
  UInt16 log_size ;

  dst_vars.log_done  = FALSE ;
  dst_vars.log_count = FALSE ;
  memset(&dst_vars.log[0] , NULL , sizeof(Dst_Log) * DST_LOG_SIZE) ;

  TRACE("\r\nDST_LOG_INIT\r\n");

  if((nvfs_rd_size(nvfs_dst_log , &log_size) == NVFS_OK))
    {       
       dst_vars.log_count = (log_size /(sizeof(Dst_Log))) ;
	   

      if(dst_vars.log_count == DST_LOG_SIZE)
        {
          #if DBG_DST == 1
            dst_dbg_print("\nDst log Size Ok" , NULL , NULL);
          #endif
		  TRACE("\r\nDst log Size Ok\r\n");
          if(nvfs_rd(nvfs_dst_log , dst_vars.log) == NVFS_OK)
            {
              dst_vars.log_done  = TRUE ;
			  TRACE("\r\n  dst_vars.log_done set!!!\r\n");
            }
        }
      else
        {
          dst_log_delete() ;
          
          #if DBG_DST == 1
            dst_dbg_print("\nDst log Size Not Match", NULL, NULL);
          #endif
		  TRACE("\r\nDst log Size Not Match\r\n");
        }       
    }
  else
    {
      #if DBG_DST == 1
        dst_dbg_print("\nDst Log Not Found" , NULL, NULL);
      #endif
	  TRACE("\r\nDST LOG NOT FOUND\r\n");
    }
  #if DBG_DST == 1
    dst_dbg_print_log () ;  
  #endif  
}
