/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: cmd_init_nvfs.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "cmd_private.h"
#include "nvram.h"
#include "nvfs_usr.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Boolean cmd_init_error               (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
Boolean cmd_init_nvfs (void)
{
  return(cmd_init_error()) ;
}

//--------------------------------------------------------------------------//

static Boolean cmd_init_error (void)
{
  static Cmd_Error cur_err ;

  UInt16 size = 0 ;

  if ((nvfs_rd_size(nvfs_cmd_error , &size) == NVFS_OK))
  {
      if((size == sizeof(cur_err)) && (nvfs_rd_s(nvfs_cmd_error , &cmd_err, sizeof(cmd_err)) == NVFS_OK))
      {
          #if DBG_CMD == 1
              dbg_tx_text ("\n nvfs_cmd_error correct read \n\r") ;
          #endif
          return (TRUE) ;
      }
  }

  nvfs_delete(nvfs_cmd_error);
  
  cmd_err = cur_err ;

  switch (nvfs_wr_create(nvfs_cmd_error, &cmd_err , sizeof(Cmd_Error)))
    {
      case NVFS_OK :  
        {         
            #if DBG_CMD == 1
              dbg_tx_text ("\n nvfs_cmd_error re-created") ;
            #endif
           return cmd_init_error() ;
        }  
    }      
  return FALSE ;
}


