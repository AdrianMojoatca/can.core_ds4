/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs.delete.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "nvfs_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct
{
  UInt16 entry_size      ;

  UInt16 entry_addr      ;  // To be deleted
  UInt16 entry_addr_prev ;  // Zero, if root
  UInt16 entry_addr_next ;  // Zero, if last

} Entry_Info ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void _delete_       (NVFS_Entry_Name const entry_name) ;

static void update_header  (Entry_Info * const entry_info)                                    ;
static void shift_entries  (Entry_Info * const entry_info)                                    ;
static void get_entry_info (Entry_Info * const entry_info , NVFS_Entry_Name const entry_name) ;

static void mv_link        (NVFS_Entry_Link * const entry_link , UInt16 shift_distance)                ;
static void mv_entry       (NVFS_Entry * const nvfs_entry , UInt16 entry_addr , UInt16 shift_distance) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

NVFS_Status nvfs_delete (NVFS_Entry_Name const entry_name)
{
  int status = NVFS_IRQ_CALL_FAILED ;

  if (nvfs_atomic_request ())
    {
      if ((status = setjmp (nvfs_vars.throw_vec)) == 0)
        {
          _delete_ (entry_name) ;
        }

      nvfs_atomic_release () ;
    }

  return (NVFS_Status) status ;
}

//--------------------------------------------------------------------------//

static void _delete_ (NVFS_Entry_Name const entry_name)
{
  Entry_Info entry_info ;

  get_entry_info (& entry_info , entry_name) ;

  shift_entries  (& entry_info) ;

  update_header  (& entry_info) ;
}

//--------------------------------------------------------------------------//

static void get_entry_info (Entry_Info * const entry_info , NVFS_Entry_Name const entry_name)
{
  NVFS_Entry nvfs_entry  ;

  if (!nvfs_find_entry (entry_name , & entry_info->entry_addr , & entry_info->entry_addr_prev))
    {
      nvfs_throw (NVFS_NAME_NOT_FOUND) ;
    }

  nvfs_rd_entry (& nvfs_entry , entry_info->entry_addr) ;

  if (nvfs_entry.link.attributes & NVFS_ATTRIB_WR_PROTECTED)
    {
      nvfs_throw (NVFS_WR_PROTECTED) ;
    }

  entry_info->entry_addr_next = nvfs_entry.link.next ;

  entry_info->entry_size = entry_info->entry_addr + sizeof (NVFS_Entry) - nvfs_entry.link.name.addr ;
}

//--------------------------------------------------------------------------//

static void shift_entries (Entry_Info * const entry_info)
{
  UInt16 entry_addr      ;
  UInt16 entry_addr_next ;

  NVFS_Entry nvfs_entry  ;

  for (entry_addr = entry_info->entry_addr_next ; entry_addr ; entry_addr = entry_addr_next)
    {
      nvfs_rd_entry  (& nvfs_entry , entry_addr) ;

      entry_addr_next = nvfs_entry.link.next ;

      mv_entry       (& nvfs_entry , entry_addr , entry_info->entry_size) ;

      nvfs_wr_entry  (& nvfs_entry , entry_addr - entry_info->entry_size) ;
    }
}

//--------------------------------------------------------------------------//

static void mv_entry (NVFS_Entry * const nvfs_entry , UInt16 entry_addr , UInt16 shift_distance)
{
  mv_link (& nvfs_entry->link.name , shift_distance) ;
  mv_link (& nvfs_entry->link.data , shift_distance) ;

  if (nvfs_entry->link.next)
    {
      nvfs_entry->link.next -= shift_distance ;
    }
}

//--------------------------------------------------------------------------//

static void mv_link (NVFS_Entry_Link * const entry_link , UInt16 shift_distance)
{
  UInt16 move_size = entry_link->size                  ; 

  UInt16 addr_srce = entry_link->addr                  ;
  UInt16 addr_dest = entry_link->addr - shift_distance ; 

  entry_link->addr = addr_dest ;

  do
    {
      nvfs_wr_nvram (addr_dest++ , nvfs_rd_nvram (addr_srce++)) ;
    }
  while (--move_size) ;
}

//--------------------------------------------------------------------------//

static void update_header (Entry_Info * const entry_info)
{
  NVFS_Header nvfs_header ;

  nvfs_rd_header (& nvfs_header) ;

  if (entry_info->entry_addr_prev == 0)
    {
      if (entry_info->entry_addr_next == 0)
        {
          nvfs_header.mem.list.root = 0 ;
          nvfs_header.mem.list.last = 0 ;
        }
      else
        {
          nvfs_header.mem.list.root  = entry_info->entry_addr_next - entry_info->entry_size ;
          nvfs_header.mem.list.last -= entry_info->entry_size                               ;
        }
    }
  else
    {
      NVFS_Entry nvfs_entry ;

      nvfs_rd_entry (& nvfs_entry , entry_info->entry_addr_prev) ;

      if (entry_info->entry_addr_next == 0)
        {
          nvfs_entry.link.next = 0 ;
    
          nvfs_header.mem.list.last = entry_info->entry_addr_prev ;
        }
      else
        {
          nvfs_entry.link.next = entry_info->entry_addr_next - entry_info->entry_size ;
    
          nvfs_header.mem.list.last -= entry_info->entry_size ;
        }

      nvfs_wr_entry (& nvfs_entry , entry_info->entry_addr_prev) ;
    }

  nvfs_header.mem.free -= entry_info->entry_size ;

  nvfs_wr_header (& nvfs_header) ;
}

