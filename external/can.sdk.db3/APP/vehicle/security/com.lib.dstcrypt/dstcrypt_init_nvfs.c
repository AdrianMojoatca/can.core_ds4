/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dstcrypt_init_nvfs.c 34930 2015-11-03 16:27:11Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dstcrypt_private.h"
#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Boolean init_isk (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
Boolean dstcrypt_init_nvfs (void)
{  
  return (init_isk()) ;
}

//--------------------------------------------------------------------------//

static Boolean init_isk (void)
{  
  memset(&dstcrypt_nvfs_entry , NULL , sizeof(DstCrypt_Nvfs_Entry)) ;
  
  dstcrypt_vars.nvfs_entry_done = (nvfs_rd (nvfs_dst_isk , &dstcrypt_nvfs_entry) == NVFS_OK) ;

  #if DBG_DSTCRYPT == 1

    if(dstcrypt_vars.nvfs_entry_done)
      {
        dstcrypt_dbg_print("\nDstCrypt Isk Found: " , &dstcrypt_nvfs_entry.isk[0] , sizeof_array(dstcrypt_nvfs_entry.isk)) ;
      }
    else
      {
        dstcrypt_dbg_print("\nDstCrypt Isk Not Found" , NULL , NULL) ;
      }
  #endif

  return dstcrypt_vars.nvfs_entry_done;
}
