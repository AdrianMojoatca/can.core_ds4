/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs.create.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "nvfs_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Nvfs_Op_Args_Create // : public Op_Args
{
  Nvfs_Op_Args _base_ ;              // !!! Must Be First Field !!!

  UInt16          entry_size ;
  NVFS_Entry_Name entry_name ;

} Nvfs_Op_Args_Create ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void nvfs_server_executed_create (Nvfs_Op_Args_Create * op_args_create) ;

static void nvfs_create_wr_name    (UInt16 * const mem_free , NVFS_Entry_Link * const nvfs_entry_link , NVFS_Entry_Name const entry_name ) ;

static void nvfs_create_wr_data_00 (UInt16 * const mem_free , NVFS_Entry_Link * const nvfs_entry_link , UInt16                entry_size ) ;

static void nvfs_create_wr_entry   (UInt16 * const mem_free , NVFS_Entry      * const nvfs_entry      , NVFS_Header   * const nvfs_header) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

NVFS_Status nvfs_create (NVFS_Entry_Name const entry_name , UInt16 entry_size)
{
  Nvfs_Op_Args_Create op_args_create = { { NVFS_ERR_UNINITIALIZED } } ;
  
  #if NVFS_USE_OS == 1
  
  if(os_irq_executing)
  { 
	NVFS_EXCEPTION_REPORT(NVFS_ERR_INVALID_CALL);
  	
  	return NVFS_ERR_INVALID_CALL;
  }
  
  #endif

  if (nvfs_vars.is_initialized)
    {
      op_args_create.entry_name = entry_name ;
      op_args_create.entry_size = entry_size ;

      NVFS_SERVER_CALL  (op_args_create , nvfs_server_executed_create) ;
    }

  NVFS_EXCEPTION_REPORT (op_args_create._base_.status) ;

  return op_args_create._base_.status ;
}

//--------------------------------------------------------------------------//

static void nvfs_server_executed_create (Nvfs_Op_Args_Create * op_args_create)
{
  NvfsTry
    {
      NVFS_Entry  nvfs_entry  ;
      NVFS_Header nvfs_header ;
    
      if (!op_args_create->entry_name || op_args_create->entry_name[0] == 0 || op_args_create->entry_size == 0)
        {
          NvfsThrow NVFS_ERROR ;
        }
      
      if (nvfs_find_entry (op_args_create->entry_name , 0 , 0))
        {
          NvfsThrow NVFS_NAME_DUPLICATE ;
        }
    
      nvfs_rd_header (& nvfs_header) ;
    
      write_elements :
        {
          UInt16 mem_free = nvfs_header.mem.free ;
    
          nvfs_create_wr_name    (& mem_free , & nvfs_entry.link.name ,   op_args_create->entry_name) ;
          nvfs_create_wr_data_00 (& mem_free , & nvfs_entry.link.data ,   op_args_create->entry_size) ;
          nvfs_create_wr_entry   (& mem_free , & nvfs_entry           , & nvfs_header               ) ;
        }

      op_args_create->_base_.status = NVFS_OK ;
    }
  
  NvfsCatch (op_args_create->_base_.status)
    {
    }
}

//--------------------------------------------------------------------------//

static void nvfs_create_wr_name (UInt16 * const mem_free , NVFS_Entry_Link * const nvfs_entry_link , NVFS_Entry_Name const entry_name)
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

static void nvfs_create_wr_data_00 (UInt16 * const mem_free , NVFS_Entry_Link * const nvfs_entry_link , UInt16 entry_size)
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

static void nvfs_create_wr_entry (UInt16 * const mem_free , NVFS_Entry * const nvfs_entry , NVFS_Header * const nvfs_header)
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

