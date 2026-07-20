/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs.find_entry.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "nvfs_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static UInt16  nvfs_find_entry_fetch_list_root (void) ;

static Boolean nvfs_find_entry_test_match (NVFS_Entry_Name const entry_name , NVFS_Entry_Link * const nvfs_entry_link) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean nvfs_find_entry (NVFS_Entry_Name const entry_name , UInt16 * const entry_addr , UInt16 * const entry_addr_prev)
{
  UInt16 loc_entry_addr      = nvfs_find_entry_fetch_list_root () ; 
  UInt16 loc_entry_addr_prev = 0                  ;

  while (loc_entry_addr)
    {
      NVFS_Entry nvfs_entry ;

      nvfs_rd_entry (& nvfs_entry , loc_entry_addr) ;

      if (nvfs_find_entry_test_match (entry_name , & nvfs_entry.link.name))
        {
          if (entry_addr     ) *entry_addr      = loc_entry_addr      ;
          if (entry_addr_prev) *entry_addr_prev = loc_entry_addr_prev ;

          return TRUE ;
        }

      loc_entry_addr_prev = loc_entry_addr       ;
      loc_entry_addr      = nvfs_entry.link.next ;
    }

  return FALSE ;
}

//--------------------------------------------------------------------------//

static UInt16 nvfs_find_entry_fetch_list_root (void)
{
  NVFS_Header nvfs_header ;

  nvfs_rd_header (& nvfs_header) ;

  return nvfs_header.mem.list.root ;
}

//--------------------------------------------------------------------------//

static Boolean nvfs_find_entry_test_match (NVFS_Entry_Name const entry_name , NVFS_Entry_Link * const nvfs_entry_link)
{
  UInt16 size, index ;

  for (index = 0 , size = nvfs_entry_link->size ; size && entry_name [index] ; size-- , index++)
    {
      if (nvfs_rd_nvram (nvfs_entry_link->addr + index) != entry_name [index])
        {
          return FALSE ;
        }
    }

  return ! (size || entry_name [index]) ;
}

