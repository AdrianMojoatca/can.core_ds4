
/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dst_log_update.c 17998 2012-11-01 15:10:11Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dst_private.h"
#include "nvram_usr.h"
#include "bitfield.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define TRANS_DATA_SIZE 6

typedef void    (* Log_Func) (UInt8 * data , UInt8 size) ;

typedef struct
{           
  UInt8    page  ;
  Log_Func func  ;

}Log_Entry ;

//--------------------------------------------------------------------------//
typedef struct
{
  Boolean rx_chall                      ;
  
  UInt8 challenge [DST_CHALLENGE_SIZE]  ;
  UInt8 response  [DST_SIGNATURE_SIZE]  ;
  
  Dst_Memory      key                   ;

}Key_Log ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void update_page1_3  (UInt8 * data , UInt8 size) ;
static void update_page4    (UInt8 * data , UInt8 size) ;
static void update_page16   (UInt8 * data , UInt8 size) ;
static void update_page30   (UInt8 * data , UInt8 size) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static  Key_Log key_log ;

//--------------------------------------------------------------------------//
 
Log_Entry log_table[] = 
{
  {1  , update_page1_3  },
  {2  , update_page1_3  },
  {3  , update_page1_3  },
  {4  , update_page4    },
//  {6  , update_page6  },
//  {7  , update_page7  },
//  {8  , update_page8  },
//  {9  , update_page9  },
//  {10 , update_page10 },
//  {11 , update_page11 },
//  {12 , update_page12 },
//  {13 , update_page13 },
//  {14 , update_page14 },
//  {15 , update_page15 },
  {16 , update_page16   },
//  {17 , update_page17 },
//  {18 , update_page18 },
//  {19 , update_page19 },
  {30 , update_page30   },
} ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

void dst_log_update_page(UInt8 page , UInt8 * data , UInt8 size)
{
  UInt8 n ;
  
  #if DBG_DST == 1
    dst_dbg_print("\nDst Log Update Page ", &page, 1);
    dst_dbg_print("\nDst Log Page Data   ",  data, size);
   
  #endif
          
  if(size == TRANS_DATA_SIZE)
    {
      TRACE("\r\nDST_LOG_UPDATE_PAGE\r\n");
	  for(n = 0 ; n < sizeof_array(log_table) ; n ++)
        {
          if(log_table[n].page == page)
            {              
              log_table[n].func(data, size) ;
              return ;
            }
        }

  TRACE("\r\nDst Log Page Not Support\r\n");
  #if DBG_DST == 1
    dst_dbg_print("\nDst Log Page Not Support", NULL, NULL);
  #endif    

    }
}

//--------------------------------------------------------------------------//
static void update_page1_3    (UInt8 * data , UInt8 size) 
{  
  key_log.key.sel_addr      = data[0] ;
  key_log.key.usr_data1     = data[1] ;
  key_log.key.uid.item.ma   = data[2] ;

//  #if DBG_DST == 1
//    dbg_tx_text("\r\nSel Addr = ") ;
//    dbg_tx_h8(key_log.key.sel_addr) ; 
//    dbg_tx_text("\r\nUsr Data1 = ") ;
//    dbg_tx_h8(key_log.key.usr_data1) ; 
//    dbg_tx_text("\r\nManu Code = ") ;
//    dbg_tx_h8(key_log.key.uid.item.ma) ; 

//  #endif
}

//--------------------------------------------------------------------------//
static void update_page4    (UInt8 * data , UInt8 size) 
{
  memcpy(&key_log.key.uid.item.serial[0] , data , DST_UID_SIZE_NO_MA);
}
//--------------------------------------------------------------------------//
static void update_page16  (UInt8 * data , UInt8 size) 
{
  memcpy(&key_log.key.tic[0]       , data                , DST_TIC_SIZE) ;
  memcpy(&key_log.key.rc[0]        , data + DST_TIC_SIZE , DST_RC_SIZE)  ;

  memcpy(&dst_vars.key_mem->tic[0] , data                , DST_TIC_SIZE) ;
  memcpy(&dst_vars.key_mem->rc[0]  , data + DST_TIC_SIZE , DST_RC_SIZE)  ;

}
//--------------------------------------------------------------------------//
static void update_page30   (UInt8 * data , UInt8 size) 
{
  memcpy(key_log.key.config.byte , data , DST_CONFIG_SIZE ) ;
}

//--------------------------------------------------------------------------//
void dst_log_update_sel_addr(UInt8 * data)
{
	/* use this as in the precedent version of dst ? */
	/*
	if(key_log.key.sel_addr  != *data)
    {
      key_log.key.sel_addr   = *data ;

      #if DBG_DST_TRACE == 2
        dbg_tx_text("\r\nSel Addr  = ") ; 
        dbg_tx_h8(key_log.key.sel_addr) ; 
    
      #endif
    }*/
	/* end use this as in precedent version of dst ? */
}
//--------------------------------------------------------------------------//
void dst_log_update_challenge(UInt8 * data)
{    
  TRACE("\r\nDST_LOG_UPDATE_SEL_CHALLENGE\r\n");
  if(memcmp(&key_log.challenge[0] , data , DST_CHALLENGE_SIZE))
    {
      key_log.rx_chall = TRUE ;
      
      memcpy(&key_log.challenge[0] , data , DST_CHALLENGE_SIZE) ;

      #if DBG_DST == 1
        dst_dbg_print("\nDst New Challenge " , data , DST_CHALLENGE_SIZE) ;
      #endif 
    }
  else
    {
      #if DBG_DST == 1
        dst_dbg_print("\nDst Challenge Skip" , NULL, NULL) ;
      #endif 
    }

}
//--------------------------------------------------------------------------//

void dst_log_update_response(UInt8 * data)
{  
  #define BLANK_KEY(x) !memcmp(&dst_vars.log[x].uid[0], &uid.item.serial[0] , DST_UID_SIZE_NO_MA )
  
  #define LOG(x)       dst_vars.log[x] 
  #define CUR_KEY      key_log.key

  if(key_log.rx_chall)
    {
      static const Uid uid = {0};

      UInt8 z ;
      
      memcpy(&key_log.response[0] , data , DST_SIGNATURE_SIZE) ;
      
      #define COUNT z //dst_vars.log_count

      for(COUNT = 0 ; COUNT < DST_LOG_SIZE ; COUNT ++)
        {
          if(BLANK_KEY(COUNT))
            {
              LOG(COUNT).sel_addr  = CUR_KEY.sel_addr   ;
              LOG(COUNT).user_data = CUR_KEY.usr_data1  ;
              LOG(COUNT).ma        = CUR_KEY.uid.item.ma;
              
              memcpy(LOG(COUNT).uid       , CUR_KEY.uid.item.serial , sizeof_array(LOG(COUNT).uid))       ;
              memcpy(LOG(COUNT).challenge , key_log.challenge       , sizeof_array(LOG(COUNT).challenge)) ;
              memcpy(LOG(COUNT).response  , key_log.response        , sizeof_array(LOG(COUNT).response))  ;
              
              break ;            
            }
        } 
      
      dst_vars.log_count = COUNT + 1 ;
      
      if(nvfs_wr_create(nvfs_dst_log , &dst_vars.log , sizeof(Dst_Log) * DST_LOG_SIZE) == NVFS_OK) 
        {
		  TRACE("\r\nINSIDE DST_LOG_UPDATE_RESPONSE- wr_create: %02X \r\n", DST_SIGNATURE_SIZE);
          #if DBG_DST == 1
            {          
              dst_dbg_print("\r\nResponse  = " , &key_log.response[0] , DST_SIGNATURE_SIZE) ;

              dst_dbg_print("\r\n\n***Log# " , &dst_vars.log_count,1) ;
              
             if(dst_vars.log_done)
               {
                 dst_dbg_print("\r\n***Log Done***" , NULL, NULL) ;
				 TRACE("\r\n LOG_DONE\r\n");
               }
            }
          #endif          
          if(dst_vars.log_count == (DST_LOG_SIZE))
            {
              dst_vars.log_done = TRUE ;
			  TRACE("\r\nDST_CARS.LOG_IS_DONE\r\n");
			  nvram_cache_commit();
              #if DBG_DST == 1
                dst_dbg_print("\nDst Log Is Done" , NULL, NULL) ;
              #endif              
            }          
		  else
		  {
		  	  TRACE("\r\ndst_vars.log_count: %d vs DST_LOG_SIZE: %d\r\n", dst_vars.log_count, DST_LOG_SIZE);
		  }
        }     
    }

  dst_log_update_reset() ;

  #undef COUNT

}
//--------------------------------------------------------------------------//
void dst_log_update_isk(UInt8 * data , UInt8 size)
{
  memcpy(&key_log.key.isk          , data , size) ;

  memcpy(&dst_vars.key_mem->isk[0] , data , size) ;

  #if DBG_DST == 1
    {     
      dst_dbg_print("\r\nSecret Key= ", &key_log.key.isk[0] , size) ;
    }

  #endif
}
//--------------------------------------------------------------------------//
void dst_log_update_reset(void)
{
  key_log.rx_chall   = FALSE ;
}
