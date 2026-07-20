/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: prog_init_nvfs.c 19675 2013-03-06 16:09:50Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "prog_private.h"
#include "nvram.h"
#include "nvfs_usr.h"
#include "dbg.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Boolean prog_init_table              (void) ;
static Boolean prog_init_error              (void) ;
static Boolean prog_feat_table_content_match (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
Boolean prog_init_nvfs (void)
{
  return(prog_init_table() & prog_init_error()) ;
}

//--------------------------------------------------------------------------//

static Boolean prog_init_table (void)
{
  UInt16 size = 0 ;

  if(nvfs_rd_size(nvfs_feat_fw , &size) == NVFS_OK)
    {
      if(size == PROG_FEAT_SIZE)
        {
          if (nvfs_rd(nvfs_feat_fw , &feature_table) == NVFS_OK)
            {            
              if (prog_feat_table_content_match())
                {
                  #if DBG_PROG == 1
                    dbg_tx_text ("\r\nProg Feat Load") ; 
                  #endif
                  return TRUE ;
                }   
            }    
        }
    }   
  
  nvfs_delete(nvfs_feat_fw)  ;

  if(nvfs_wr_create(nvfs_feat_fw, &feature_table_default , PROG_FEAT_SIZE) == NVFS_OK)
    {         
      #if DBG_PROG == 1
        dbg_tx_text ("\r\nProg Feat Create") ;
      #endif
      return prog_init_table() ;
    }      
  #if DBG_PROG == 1
    dbg_tx_text ("\r\nProg Feat FAIL") ;
  #endif
  return FALSE ;
}

//--------------------------------------------------------------------------//

static Boolean prog_init_error (void)
{
  static Prog_Error cur_err ;
  
  UInt16 size = 0 ;

  if(nvfs_rd_size(nvfs_feat_error , &size) == NVFS_OK)
  {
      if((nvfs_rd(nvfs_feat_error , &prog_err) == NVFS_OK) && (size == sizeof(cur_err)))
      {
          #if DBG_PROG == 1
              dbg_tx_text ("\r\nProg Error Load") ;
          #endif
          return (TRUE) ;
      }
  }

  nvfs_delete(nvfs_feat_error)  ;

  switch (nvfs_wr_create(nvfs_feat_error, &cur_err , sizeof(Prog_Error)))
    {
      case NVFS_OK :  
        {         
           #if DBG_PROG == 1
                dbg_tx_text ("\r\nProg Error Create") ;
           #endif
           return prog_init_error() ;
        }  
    }      


  #if DBG_PROG == 1
    dbg_tx_text ("\r\nProg Error FAIL") ;
  #endif
  return FALSE ;
}

//--------------------------------------------------------------------------//
static Boolean prog_feat_table_content_match (void)
{     
  UInt8 lfeat = 0 ;
  UInt8 dfeat = 0 ;

  do
  {   
    if(feature_table[lfeat].feature == feature_table_default[dfeat].feature)
      {
        if(feature_table[lfeat].option_number == feature_table_default[dfeat].option_number)
          {
            lfeat++ ;
            if(lfeat == FEATURE_NUM)
              {
                return TRUE ;
              }
            dfeat = 0 ;
          }
        else
          {
            return FALSE ;
          }
      }
    else
      {
        dfeat++ ;
      }
  }
  while(dfeat < FEATURE_NUM) ;
  
  return FALSE ;  
}

