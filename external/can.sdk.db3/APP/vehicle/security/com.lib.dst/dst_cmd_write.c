
/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dst_cmd_write.c 24713 2014-03-05 18:48:51Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dst_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef Boolean (* Dst_Write_Callback) (Dst_Trans_Resp * resp , UInt8 * data , UInt8 size) ;

typedef struct 
{
  UInt8              page     ;
  Dst_Write_Callback callback ;  

}Write_Table ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Boolean write_pwd         (Dst_Trans_Resp * resp , UInt8 * data , UInt8 size)  ;
static Boolean write_usr_data_1  (Dst_Trans_Resp * resp , UInt8 * data , UInt8 size)  ;
static Boolean write_isk         (Dst_Trans_Resp * resp , UInt8 * data , UInt8 size)  ;   
static Boolean write_config      (Dst_Trans_Resp * resp , UInt8 * data , UInt8 size)  ;
                              
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

const Write_Table write_table[] =
{
 {DST_ISK_PAGE          , write_isk         } ,
 {DST_CONFIG_PAGE       , write_config      } ,
} ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
Dst_Trans_Resp dst_cmd_write(UInt8 page , UInt8 * data , UInt8 size)
{  
  UInt8 i  ;

  Dst_Trans_Resp resp ; 

  TRACE("\r\nDST_CMD_WRITE\r\n");
  memset(&resp , 0 , sizeof(Dst_Trans_Resp)) ;

  size-= DST_BCC_SIZE ;

   #if DBG_DST == 1
     dst_dbg_print("\nDst Write Page ", &page, 1);
     dst_dbg_print(" Data "           , data , size);   
   #endif

  if(dst_get_lock_status(page) == DST_PAGE_STATUS_UNLOCK)
    {
      for( i = 0 ; i < sizeof_array(write_table) ; i++)
        {
          if((write_table[i].page == page))
            {
              if(!write_table[i].callback(&resp , data , size))
                {
                  dst_err_add(DST_ERR_WRITE) ;// error write
                }
            }
        }
    }
  return (resp) ;
}
//--------------------------------------------------------------------------//

static Boolean write_pwd(Dst_Trans_Resp * resp , UInt8 * data , UInt8 size)
{
  if(size > DST_PWD_SIZE && dst_sel_addr_is_valid(data))
    {
      return write_pwd(resp , (data + DST_PWD_SIZE) , (size - DST_PWD_SIZE)) ;
    }  
  else if(size == DST_PWD_SIZE)
    {
      dst_vars.key_mem->sel_addr    = *data ;
    }
  else 
    {
      // log the error
      return FALSE ;
    }

  resp->item.data[0]            = dst_vars.key_mem->sel_addr  ;
  resp->item.data[1]            = dst_vars.key_mem->usr_data1 ;
  
  memcpy(&resp->item.data[2] , dst_vars.key_mem->uid.byte , DST_UID_SIZE_WITH_MA) ;
  
  return TRUE ;
}

//--------------------------------------------------------------------------//

static Boolean write_usr_data_1(Dst_Trans_Resp * resp , UInt8 * data , UInt8 size)
{
  if(size > DST_USR_DATA_SIZE && dst_sel_addr_is_valid(data))
    {
      return write_usr_data_1(resp , (data + DST_PWD_SIZE) , (size - DST_PWD_SIZE)) ;
    }
  else if(size == DST_USR_DATA_SIZE)
    {
      dst_vars.key_mem->usr_data1 = *data ;
    }
  else
    {
      return FALSE ;
    }

  resp->item.data[0] = dst_vars.key_mem->sel_addr  ;
  resp->item.data[1] = dst_vars.key_mem->usr_data1 ;
  
  memcpy(&resp->item.data[2] , dst_vars.key_mem->uid.byte , DST_UID_SIZE_WITH_MA) ;

  return TRUE ;
}

//--------------------------------------------------------------------------//

static Boolean write_isk(Dst_Trans_Resp * resp , UInt8 * data , UInt8 size)
{  
  Boolean is_80bits = !!((size == DSTCRYPT_KEY80_SIZE) || (size == (DSTCRYPT_KEY80_SIZE + 1))) ;

  if(dst_vars.key_mem->config.bit.enc_80 != is_80bits)
    {
      dst_vars.key_mem->config.bit.enc_80 = is_80bits ;

      dst_err_add(DST_ERR_ISK_MISTMATCH) ;
    }

  if(dst_vars.key_mem->config.bit.enc_80)
    {
      if(size < DSTCRYPT_KEY80_SIZE)
        {
          return FALSE ;
        }
      else if(size > DSTCRYPT_KEY80_SIZE)
        {          
          if(!dst_sel_addr_is_valid(data))
            {
              return FALSE ;
            }
          data++ ;
        }

      size   = DSTCRYPT_KEY80_SIZE ;

    }
  else
    {
      if(size < DSTCRYPT_KEY40_SIZE)
        {
          return FALSE ;
        }
      else if(size > DSTCRYPT_KEY40_SIZE)
        {          
          if(!dst_sel_addr_is_valid(data))
            {
              return FALSE ;
            }
          data++ ;
        }

      size = DSTCRYPT_KEY40_SIZE ;
    }

  if(dstcrypt_write_isk(data , size))
    {
      dst_log_update_isk(data , size) ;

      return TRUE ;
    }  

//  memset(dst_vars.key_mem->uid.item.serial , 0 , DST_UID_SIZE_NO_MA ) ;


  return FALSE ;  

}

//--------------------------------------------------------------------------//

static Boolean write_config(Dst_Trans_Resp * resp , UInt8 * data , UInt8 size)
{ 
  if(size > DST_CONFIG_SIZE && dst_sel_addr_is_valid(data))
    {
      return write_config(resp , (data + DST_PWD_SIZE) , DST_CONFIG_SIZE) ;
    }
  else if(size == DST_CONFIG_SIZE)
    {
      memcpy(&dst_vars.key_mem->config , data , size) ;
    }
  else
    {
      return FALSE ;
    }
  
  memcpy(&resp->item.data[0]    , data , size) ; 
  memcpy(&resp->item.data[size] , &dst_vars.key_mem->uid.byte[0] , DST_UID_SIZE_WITH_MA) ;     

  return TRUE ;

}

