
/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dst_isk_search.c 16978 2012-09-01 18:03:05Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dst_private.h"
#include "dst_isk_search_v2.c"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define IS_02(z) ((z & 0x0F) == 0x02)
#define IS_34(z) ( z         == 0x34) 
#define IS_94(z) ( z         == 0x94) 
#define IS_00(z) ((z & 0x0F) == 0x00)

//--------------------------------------------------------------------------//

typedef struct
{
  UInt64 lsb64 ;
  UInt8  msb8  ;
 
}Mask ;

//--------------------------------------------------------------------------//

typedef struct 
{
  UInt8 isk[DSTCRYPT_KEY80_SIZE] ;

  Dst_Log log  ;

  Mask    mask ;

}Search_Entry ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Mask build_mask(UInt8 pwd)            ;

static void build_isk (Search_Entry * entry) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
    
Boolean dst_isk_search(Dst_Log * log, UInt8 isk[DSTCRYPT_KEY80_SIZE])
{  
  #define PWD       search_entry.log.sel_addr
  
  Search_Entry search_entry ;
  
  memset(&search_entry , NULL , sizeof(search_entry)) ;

  memcpy(&search_entry.log , log , sizeof(Dst_Log)) ;
   
  do
    {
      if(IS_02(PWD) || IS_34(PWD) || IS_94(PWD) || IS_00(PWD))
        {
          search_entry.mask = build_mask (PWD) ;
    
          do
            {
              UInt8 challenge [DST_CHALLENGE_SIZE] ;
              
              memcpy(&challenge[0] , &search_entry.log.challenge[0] , DST_CHALLENGE_SIZE) ;
              
              build_isk (&search_entry) ;
    
              dstcrypt_set_isk(&search_entry.isk[0] , DSTCRYPT_KEY80_SIZE) ;
    
              dstcrypt(&challenge[0] , (dst_vars.key_mem->config.bit.enc_80? DST_80 : DST_40)) ;
                  
              if(!memcmp(&challenge[0] , &log->response[0] , DST_SIGNATURE_SIZE) && dstcrypt_write_isk(&search_entry.isk[0] , DSTCRYPT_KEY80_SIZE))
                {
                  memcpy(&isk[0] , &search_entry.isk[0] , DSTCRYPT_KEY80_SIZE) ;
                  
                  *log = search_entry.log ;
 
                  return TRUE ;
                }          
            }
          while(++search_entry.log.user_data != log->user_data) ;
        }
    }
  while ((PWD -=2) , (PWD != log->sel_addr)) ;
   
  return NULL ;

}
//--------------------------------------------------------------------------//

static Mask build_mask(UInt8 pwd)
{
  Mask mask = {0} ;
  
  #if DBG_DST == 1  
   dst_dbg_print("\nDst Build Mask Pwd: " , &pwd , 1) ;
  #endif

  if(IS_02(pwd))
    {
      mask.lsb64   =  0x466A466A;
      mask.msb8    =  0x6A; 
    }
  else if(IS_00(pwd))
    {
      mask.lsb64   =  0x20062006;
      mask.msb8    =  0x06;
    }
  else if(IS_34(pwd))
    {
      mask.lsb64   =  0x8AAE8AAE;
      mask.msb8    =  0xAE;
    }
  else if(IS_94(pwd))
    {
      mask.lsb64   =  0x46904624;
      mask.msb8    =  0x06;
    }
  
  return mask;
}
//--------------------------------------------------------------------------//

static void build_isk(Search_Entry * search_entry)
{
  #define ID     search_entry->log.user_data
  #define ISK(z) search_entry->isk[z]

    // Declaration Variables
     UInt64 tempo    =0  ;
     UInt64 tempo_2  =0  ;
     UInt64 page4    =0  ;
     UInt64 page4_2  =0  ;
     UInt32 buffer   =0  ;
     UInt8  buffer_2 =0  ; 

    UInt64  mask  = search_entry->mask.lsb64 ;
    UInt8   mask2 = search_entry->mask.msb8  ;
    
    
    // Instructions
    page4_2 = (ID & 0xF0);
    page4_2 = (ID & 0x0F) | page4_2 ;
        
    page4   = ((ID & 0xF0)  << 24)  ;
    page4   = ((ID & 0x0F)  << 24)| page4  ;
    page4   = ((ID & 0xF0)  << 16)| page4  ;
    page4   = ((ID & 0x0F)  << 16)| page4  ;   
    page4   = ((ID & 0xF0)  << 8) | page4  ;
    page4   = ((ID & 0x0F)  << 8) | page4  ;
    page4   =  (ID & 0XF0)        | page4  ; 
    page4   =  (ID & 0x0F)        | page4  ;
 
    buffer   = search_entry->log.uid[1] ;
    buffer   = (buffer << 8) | search_entry->log.uid[0]   ;
    buffer   = (buffer << 8) | search_entry->log.sel_addr ;
    buffer_2 = search_entry->log.uid[2] & 0xFF   ;

    // formule
    tempo   = (((page4    <<1) & 0x001FF) ^   (buffer << 9) & (0xFFFFFF00));
    tempo_2 =  ((buffer_2 <<1) & (0x0FF)) |  ((search_entry->log.uid[1] >>7) & (0x01)) ;
        
    page4   = page4 ^ tempo;
    page4   = page4 ^ mask; 
        
    page4_2 = page4_2 ^ tempo_2 ;
    page4_2 = page4_2 ^ mask2;
        
    // Transform page4 UInt64 into page4 Uint8 [10]
    ISK(4) =  (page4_2      & 0xFF);
    ISK(3) =  ((page4 >>24) & 0xFF);
    ISK(2) =  ((page4 >>16) & 0xFF);
    ISK(1) =  ((page4 >>8)  & 0xFF);
    ISK(0) =  (page4        & 0xFF);
}

