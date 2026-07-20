
/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dst_cmd_read.c 15918 2012-07-12 18:57:41Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dst_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef void  (* Dst_Read_Callback) (Dst_Trans_Resp * resp , UInt8 * data) ;

typedef struct 
{
  UInt8             page     ;
  Dst_Read_Callback callback ;  

}Read_Table ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void read_page_1_to_3 (Dst_Trans_Resp * resp , UInt8 * data);
static void read_page_4      (Dst_Trans_Resp * resp , UInt8 * data);
static void read_page_7      (Dst_Trans_Resp * resp , UInt8 * data);
static void read_page_8_to_15(Dst_Trans_Resp * resp , UInt8 * data);
static void read_page_16     (Dst_Trans_Resp * resp , UInt8 * data);
static void read_page_17     (Dst_Trans_Resp * resp , UInt8 * data);
static void read_page_18     (Dst_Trans_Resp * resp , UInt8 * data);
static void read_page_30     (Dst_Trans_Resp * resp , UInt8 * data);
                                 
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

const Read_Table read_table[] =
{
 {1 , read_page_1_to_3  },
 {2 , read_page_1_to_3  },
 {3 , read_page_1_to_3  },
 {4 , read_page_4       },
 {7 , read_page_7       },
 {8 , read_page_8_to_15 },
 {9 , read_page_8_to_15 },
 {10, read_page_8_to_15 },
 {11, read_page_8_to_15 },
 {12, read_page_8_to_15 },
 {13, read_page_8_to_15 },
 {14, read_page_8_to_15 },
 {15, read_page_8_to_15 },
 {16, read_page_16      },
 {17, read_page_17      },
 {18, read_page_18      },
 {30, read_page_30      },
} ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
Dst_Trans_Resp dst_cmd_gen_read(UInt8 page , UInt8 * data , UInt8 size)
{
  UInt8 i, j  ;

  Dst_Trans_Resp resp ; 

 // TRACE("\r\nDST_CMD_READ: WILL DO DS_CMD_GEN_READ\r\n");

 #if DBG_DST == 1
   dst_dbg_print("\nDst Gen Read Page ", &page, 1);
   dst_dbg_print(" Data "    , data , size);   
 #endif

   
  
  for( i = 0 ; i < sizeof_array(read_table) ; i++)
    {
      if(read_table[i].page == page)
        {
          read_table[i].callback(&resp , data) ;

		  //for ( j = 0; j < size; j++ ) TRACE("%02X ", resp.buff[j]);
          //TRACE("\r\n");
          return(resp) ;
        }
    }

  memset(&resp , 0 , sizeof(Dst_Trans_Resp)) ;

  return (resp) ;
  
}
//--------------------------------------------------------------------------//

Dst_Trans_Resp dst_cmd_sel_read(UInt8 page , UInt8 * data , UInt8 size)
{
   #if DBG_DST == 1
     dst_dbg_print("\nDst Sel Read Page ", &page, 1);
     dst_dbg_print(" Data "    , data , size);   
   #endif
  if(!dst_log_is_done())
    {
	  TRACE("\r\nDST_CMD_READ: WILL DO DST_LOG_UPDATE_SEL_ADDR\r\n");
      dst_log_update_sel_addr(data) ;
    }
   
  return (dst_cmd_gen_read       (page , (data + DST_PWD_SIZE) , size - DST_PWD_SIZE)) ;
}

//--------------------------------------------------------------------------//

static void read_page_1_to_3 (Dst_Trans_Resp * resp , UInt8 * data)
{
  resp->item.data[0] = dst_vars.key_mem->sel_addr  ;
  resp->item.data[1] = dst_vars.key_mem->usr_data1 ;
  
  memcpy(&resp->item.data[2] , dst_vars.key_mem->uid.byte , DST_UID_SIZE_WITH_MA) ;
}

//--------------------------------------------------------------------------//
static void read_page_4 (Dst_Trans_Resp * resp , UInt8 * data)
{
  UInt8 challenge[DST_CHALLENGE_SIZE] ;

  TRACE("\r\nREAD_PAGE_4\r\n");

  memcpy(&challenge[0] , data , DST_CHALLENGE_SIZE) ;
  
  dstcrypt                (&challenge[0] , (dst_vars.key_mem->config.bit.enc_80? DST_80 : DST_40)) ;

  memcpy(&resp->item.data[0]                  , &dst_vars.key_mem->uid.item.serial[0] , DST_UID_SIZE_NO_MA) ;
  
  memcpy(&resp->item.data[DST_UID_SIZE_NO_MA] , &challenge[0]             , DST_SIGNATURE_SIZE) ;

  if(!dst_log_is_done())
    {
      dst_log_update_challenge(data) ;
    }
}
//--------------------------------------------------------------------------//

static void read_page_7 (Dst_Trans_Resp * resp , UInt8 * data)
{

}
//--------------------------------------------------------------------------//

static void read_page_8_to_15 (Dst_Trans_Resp * resp , UInt8 * data)
{

}
//--------------------------------------------------------------------------//

static void read_page_16 (Dst_Trans_Resp * resp , UInt8 * data)
{
  memcpy(&resp->item.data[0]            , dst_vars.key_mem->tic , DST_TIC_SIZE) ;
  
  memcpy(&resp->item.data[DST_TIC_SIZE] , dst_vars.key_mem->rc  , DST_RC_SIZE ) ;
}
//--------------------------------------------------------------------------//

static void read_page_17 (Dst_Trans_Resp * resp , UInt8 * data)
{

}
//--------------------------------------------------------------------------//

static void read_page_18 (Dst_Trans_Resp * resp , UInt8 * data)
{

}
//--------------------------------------------------------------------------//
static void read_page_30 (Dst_Trans_Resp * resp , UInt8 * data)
{
  memcpy(&resp->item.data[0]               , &dst_vars.key_mem->config , DST_CONFIG_SIZE)     ;
  
  resp->item.data[2] = dst_vars.key_mem->uid.item.ma ;
   
  memcpy(&resp->item.data[3] , dst_vars.key_mem->uid.item.serial, DST_UID_SIZE_NO_MA);
}



