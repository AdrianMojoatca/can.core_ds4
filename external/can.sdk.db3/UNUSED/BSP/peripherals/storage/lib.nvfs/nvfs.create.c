/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs.create.c 33258 2015-08-19 12:46:35Z martin.bouchard $
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

static void _create_   (NVFS_Entry_Name const entry_name , UInt16 size) ;

static void wr_name    (UInt16 * const mem_free , NVFS_Entry_Link * const nvfs_entry_link , NVFS_Entry_Name const entry_name ) ;

static void wr_data_00 (UInt16 * const mem_free , NVFS_Entry_Link * const nvfs_entry_link , UInt16                entry_size ) ;

static void wr_entry   (UInt16 * const mem_free , NVFS_Entry      * const nvfs_entry      , NVFS_Header   * const nvfs_header) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

NVFS_Status nvfs_create (NVFS_Entry_Name const entry_name , UInt16 entry_size)
{
  int status = NVFS_IRQ_CALL_FAILED ;

  if (nvfs_atomic_request ())
    {
      if ((status = setjmp (nvfs_vars.throw_vec)) == 0)
        {
          _create_ (entry_name , entry_size) ;
        }

      nvfs_atomic_release () ;
    }

  return (NVFS_Status) status ;
}

//--------------------------------------------------------------------------//

static void _create_ (NVFS_Entry_Name const entry_name , UInt16 entry_size)
{
  NVFS_Entry  nvfs_entry  ;
  NVFS_Header nvfs_header ;

  if (!entry_name || entry_name[0] == 0 || entry_size == 0)
    {
      nvfs_throw (NVFS_ERROR) ;
    }
  
  if (nvfs_find_entry (entry_name , 0 , 0))
    {
      nvfs_throw (NVFS_NAME_DUPLICATE) ;
    }

  nvfs_rd_header (& nvfs_header) ;

  /*write_elements :*/
    {
      UInt16 mem_free = nvfs_header.mem.free ;

      wr_name    (& mem_free , & nvfs_entry.link.name ,   entry_name ) ;
      wr_data_00 (& mem_free , & nvfs_entry.link.data ,   entry_size ) ;
      wr_entry   (& mem_free , & nvfs_entry           , & nvfs_header) ;
    }
}

//--------------------------------------------------------------------------//

static void wr_name (UInt16 * const mem_free , NVFS_Entry_Link * const nvfs_entry_link , NVFS_Entry_Name const entry_name)
{
  UInt8 index = 0 ;

  nvfs_entry_link->addr = *mem_free ;
  nvfs_entry_link->size =         0 ;

  do
    {
      nvfs_wr_nvram ((*mem_free)++ , entry_name [index++]) ;

      nvfs_entry_link->size ++ ;
    }
  while (entry_name [index]) ;
}

//--------------------------------------------------------------------------//

static void wr_data_00 (UInt16 * const mem_free , NVFS_Entry_Link * const nvfs_entry_link , UInt16 entry_size)
{
  nvfs_entry_link->addr = *mem_free   ;
  nvfs_entry_link->size =  entry_size ;

  do
    {
      nvfs_wr_nvram ((*mem_free)++ , 0) ;
    }
  while (--entry_size) ;
}

//--------------------------------------------------------------------------//

static void wr_entry (UInt16 * const mem_free , NVFS_Entry * const nvfs_entry , NVFS_Header * const nvfs_header)
{
  UInt16 entry_addr = *mem_free ;

  nvfs_entry->link.next       = 0 ;
  nvfs_entry->link.attributes = 0 ;

  nvfs_wr_entry (nvfs_entry , entry_addr) ;

  nvfs_header->mem.free = entry_addr + sizeof *nvfs_entry ;

  if (nvfs_header->mem.list.root)
    {
      nvfs_rd_entry (nvfs_entry , nvfs_header->mem.list.last) ;

      nvfs_entry->link.next = entry_addr ;

      nvfs_wr_entry (nvfs_entry , nvfs_header->mem.list.last) ;
    }
  else
    {
      nvfs_header->mem.list.root = entry_addr ;
    }

  nvfs_header->mem.list.last = entry_addr ;

  nvfs_wr_header (nvfs_header) ;
}

