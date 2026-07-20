/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs.delete.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "nvfs_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Nvfs_Op_Args_Delete // : public Op_Args
{
  Nvfs_Op_Args _base_ ;              // !!! Must Be First Field !!!

  NVFS_Entry_Name entry_name ;

} Nvfs_Op_Args_Delete ;

//--------------------------------------------------------------------------//

typedef struct
{
  UInt16 entry_size      ;

  UInt16 entry_addr      ;  // To be deleted
  UInt16 entry_addr_prev ;  // Zero, if root
  UInt16 entry_addr_next ;  // Zero, if last

} Nvfs_Delete_Entry_Info ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void nvfs_server_executed_delete (Nvfs_Op_Args_Delete * op_args_delete) ;

static void nvfs_delete_update_header  (Nvfs_Delete_Entry_Info * const entry_info)                                    ;
static void nvfs_delete_shift_entries  (Nvfs_Delete_Entry_Info * const entry_info)                                    ;
static void nvfs_delete_get_entry_info (Nvfs_Delete_Entry_Info * const entry_info , NVFS_Entry_Name const entry_name) ;

static void nvfs_delete_mv_link        (NVFS_Entry_Link * const entry_link , UInt16 shift_distance)                     ;
static void nvfs_delete_mv_entry       (NVFS_Entry      * const nvfs_entry , UInt16 entry_addr , UInt16 shift_distance) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

NVFS_Status nvfs_delete (NVFS_Entry_Name const entry_name)
{
  Nvfs_Op_Args_Delete op_args_delete = { { NVFS_ERR_UNINITIALIZED } } ;
  
  #if NVFS_USE_OS == 1
  
  if(os_irq_executing)
  { 
	NVFS_EXCEPTION_REPORT(NVFS_ERR_INVALID_CALL);
  	
  	return NVFS_ERR_INVALID_CALL;
  }
  
  #endif

  if (nvfs_vars.is_initialized)
    {
      op_args_delete.entry_name = entry_name ;

      NVFS_SERVER_CALL  (op_args_delete , nvfs_server_executed_delete) ;
    }

  NVFS_EXCEPTION_REPORT (op_args_delete._base_.status) ;

  return op_args_delete._base_.status ;
}

//--------------------------------------------------------------------------//

static void nvfs_server_executed_delete (Nvfs_Op_Args_Delete * op_args_delete)
{
  NvfsTry
    {
      Nvfs_Delete_Entry_Info entry_info ;
    
      nvfs_delete_get_entry_info (& entry_info , op_args_delete->entry_name) ;
    
      nvfs_delete_shift_entries  (& entry_info) ;
    
      nvfs_delete_update_header  (& entry_info) ;

      op_args_delete->_base_.status = NVFS_OK ;
    }
  
  NvfsCatch (op_args_delete->_base_.status)
    {
    }
}

//--------------------------------------------------------------------------//

static void nvfs_delete_get_entry_info (Nvfs_Delete_Entry_Info * const entry_info , NVFS_Entry_Name const entry_name)
{
  NVFS_Entry nvfs_entry  ;

  if (!nvfs_find_entry (entry_name , & entry_info->entry_addr , & entry_info->entry_addr_prev))
    {
      NvfsThrow NVFS_NAME_NOT_FOUND ;
    }

  nvfs_rd_entry (& nvfs_entry , entry_info->entry_addr) ;

  if (nvfs_entry.link.attributes & NVFS_ATTRIB_WR_PROTECTED)
    {
      NvfsThrow NVFS_WR_PROTECTED ;
    }

  entry_info->entry_addr_next = nvfs_entry.link.next ;

  entry_info->entry_size = entry_info->entry_addr + sizeof (NVFS_Entry) - nvfs_entry.link.name.addr ;
}

//--------------------------------------------------------------------------//

static void nvfs_delete_shift_entries (Nvfs_Delete_Entry_Info * const entry_info)
{
  UInt16 entry_addr      ;
  UInt16 entry_addr_next ;

  NVFS_Entry nvfs_entry  ;

  for (entry_addr = entry_info->entry_addr_next ; entry_addr ; entry_addr = entry_addr_next)
    {
      nvfs_rd_entry  (& nvfs_entry , entry_addr) ;

      entry_addr_next = nvfs_entry.link.next ;

      nvfs_delete_mv_entry       (& nvfs_entry , entry_addr , entry_info->entry_size) ;

      nvfs_wr_entry  (& nvfs_entry , entry_addr - entry_info->entry_size) ;
    }
}

//--------------------------------------------------------------------------//

static void nvfs_delete_mv_entry (NVFS_Entry * const nvfs_entry , UInt16 entry_addr , UInt16 shift_distance)
{
  nvfs_delete_mv_link (& nvfs_entry->link.name , shift_distance) ;
  nvfs_delete_mv_link (& nvfs_entry->link.data , shift_distance) ;

  if (nvfs_entry->link.next)
    {
      nvfs_entry->link.next -= shift_distance ;
    }
}

//--------------------------------------------------------------------------//

static void nvfs_delete_mv_link (NVFS_Entry_Link * const entry_link , UInt16 shift_distance)
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

static void nvfs_delete_update_header (Nvfs_Delete_Entry_Info * const entry_info)
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

