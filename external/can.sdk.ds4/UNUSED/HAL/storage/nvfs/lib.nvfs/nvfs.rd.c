/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs.rd.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "nvfs_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Nvfs_Op_Args_Rd    // : public Op_Args
{
  Nvfs_Op_Args _base_ ;             // !!! Must Be First Field !!!

  NVFS_Entry_Name entry_name ;

  void          * ram_dest   ;

} Nvfs_Op_Args_Rd ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void nvfs_server_executed_rd (Nvfs_Op_Args_Rd * op_args_rd) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

NVFS_Status nvfs_rd (NVFS_Entry_Name const entry_name , void * const ram_dest)
{
  Nvfs_Op_Args_Rd op_args_rd = { { NVFS_ERR_UNINITIALIZED } } ;
  
  #if NVFS_USE_OS == 1
  
  if(os_irq_executing)
  { 
	NVFS_EXCEPTION_REPORT(NVFS_ERR_INVALID_CALL);
  	
  	return NVFS_ERR_INVALID_CALL;
  }
  
  #endif

  if (nvfs_vars.is_initialized)
    {
       op_args_rd.entry_name = entry_name ;
       op_args_rd.ram_dest   = ram_dest   ;
       
       NVFS_SERVER_CALL (op_args_rd , nvfs_server_executed_rd) ;
    }

  NVFS_EXCEPTION_REPORT (op_args_rd._base_.status) ;

  return op_args_rd._base_.status ;
}

//--------------------------------------------------------------------------//

static void nvfs_server_executed_rd (Nvfs_Op_Args_Rd * op_args_rd)
{
  NvfsTry
    {
      UInt16     entry_addr ;
    
      NVFS_Entry nvfs_entry ;
    
      if (! op_args_rd->entry_name || op_args_rd->entry_name[0] == 0 || op_args_rd->ram_dest == 0)
        {
          NvfsThrow NVFS_ERROR ;
        }
      
      if (! nvfs_find_entry (op_args_rd->entry_name , & entry_addr , 0))
        {
          NvfsThrow NVFS_NAME_NOT_FOUND ;
        }
    
      nvfs_rd_entry (& nvfs_entry , entry_addr) ;
    
      if (! nvram_rd (nvfs_entry.link.data.addr , op_args_rd->ram_dest , nvfs_entry.link.data.size))
        {
          NvfsThrow NVFS_ERROR ;
        }

      op_args_rd->_base_.status = NVFS_OK ;
    }

  NvfsCatch (op_args_rd->_base_.status)
    {
    }
}

