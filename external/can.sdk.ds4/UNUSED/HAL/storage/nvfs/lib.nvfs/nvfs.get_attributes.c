/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs.get_attributes.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "nvfs_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Nvfs_Op_Args_Get_Attr // : public Op_Args
{
  Nvfs_Op_Args _base_ ;                // !!! Must Be First Field !!!

  NVFS_Entry_Name   entry_name ;
  NVFS_Attributes * attributes ;

} Nvfs_Op_Args_Get_Attr ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void nvfs_server_executed_get_attr (Nvfs_Op_Args_Get_Attr * op_args_get_attr) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

NVFS_Status nvfs_get_attributes (NVFS_Entry_Name const entry_name , NVFS_Attributes * const attributes)
{
  Nvfs_Op_Args_Get_Attr op_args_get_attr = { { NVFS_ERR_UNINITIALIZED } } ;
  
  #if NVFS_USE_OS == 1
  
  if(os_irq_executing)
  { 
	NVFS_EXCEPTION_REPORT(NVFS_ERR_INVALID_CALL);
  	
  	return NVFS_ERR_INVALID_CALL;
  }
  
  #endif

  if (nvfs_vars.is_initialized)
    {
       op_args_get_attr.entry_name = entry_name ;
       op_args_get_attr.attributes = attributes ;
       
       NVFS_SERVER_CALL (op_args_get_attr , nvfs_server_executed_get_attr) ;
    }

  NVFS_EXCEPTION_REPORT (op_args_get_attr._base_.status) ;

  return op_args_get_attr._base_.status ;
}

//--------------------------------------------------------------------------//

static void nvfs_server_executed_get_attr (Nvfs_Op_Args_Get_Attr * op_args_get_attr)
{
  NvfsTry
    {
      UInt16     entry_addr ;
    
      NVFS_Entry nvfs_entry ;
    
      if (! op_args_get_attr->entry_name || op_args_get_attr->entry_name[0] == 0)
        {
          NvfsThrow NVFS_ERROR ;
        }
      
      if (! nvfs_find_entry (op_args_get_attr->entry_name , & entry_addr , 0))
        {
          NvfsThrow NVFS_NAME_NOT_FOUND ;
        }
    
      nvfs_rd_entry (& nvfs_entry , entry_addr) ;
    
    * op_args_get_attr->attributes = nvfs_entry.link.attributes ;

      op_args_get_attr->_base_.status = NVFS_OK ;
    }

  NvfsCatch (op_args_get_attr->_base_.status)
    {
    }
}

