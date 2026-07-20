/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs.server_iter_next.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "nvfs_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define MIN(lhs,rhs) ((lhs) < (rhs) ? (lhs) : (rhs))

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void nvfs_server_copy_name (const NVFS_Entry_Link * const name , NVFS_Iterator * const iter) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void nvfs_server_iter_next (NVFS_Iterator * const iter)
{
  if (iter->is_valid = !! iter->entry_addr_next)
    {
      NVFS_Entry nvfs_entry ;

      nvfs_rd_entry (& nvfs_entry , iter->entry_addr_next) ;

      iter->entry_addr_next = nvfs_entry.link.next ;

      nvfs_server_copy_name     (& nvfs_entry.link.name , iter) ;
    }
}

//--------------------------------------------------------------------------//

static void nvfs_server_copy_name (const NVFS_Entry_Link * const name , NVFS_Iterator * const iter)
{
  UInt16      index ;

  UInt16      srce = name->addr      ;
  NVFS_Char * dest = iter->name_buff ;
  UInt16      size = MIN  (name->size , iter->name_buff_size) ;

  for (index = 0 ; index < size ; index++)
    {
      *dest++ = nvfs_rd_nvram (srce++) ;
    }

  iter->is_truncated = name->size >= iter->name_buff_size ;
  
  dest [iter->is_truncated ? -1 : 0] = 0 ;
}

#undef MIN

