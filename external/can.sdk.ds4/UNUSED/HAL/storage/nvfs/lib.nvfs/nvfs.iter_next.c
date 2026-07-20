/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs.iter_next.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "nvfs_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Nvfs_Op_Args_Iter_Next    // : public Op_Args
{
  Nvfs_Op_Args _base_ ;                    // !!! Must Be First Field !!!

  NVFS_Iterator * iter ;

} Nvfs_Op_Args_Iter_Next ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void nvfs_server_executed_iter_next (Nvfs_Op_Args_Iter_Next * op_args_iter_next) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

NVFS_Status nvfs_iter_next (NVFS_Iterator * const iter)
{
  Nvfs_Op_Args_Iter_Next op_args_iter_next = { { NVFS_ERR_UNINITIALIZED } } ;
  
  #if NVFS_USE_OS == 1
  
  if(os_irq_executing)
  { 
	NVFS_EXCEPTION_REPORT(NVFS_ERR_INVALID_CALL);
  	
  	return NVFS_ERR_INVALID_CALL;
  }
  
  #endif

  if (nvfs_vars.is_initialized)
    {
      op_args_iter_next.iter = iter ;

      NVFS_SERVER_CALL  (op_args_iter_next , nvfs_server_executed_iter_next) ;
    }

  NVFS_EXCEPTION_REPORT (op_args_iter_next._base_.status) ;

  return op_args_iter_next._base_.status ;
}

//--------------------------------------------------------------------------//

static void nvfs_server_executed_iter_next (Nvfs_Op_Args_Iter_Next * op_args_iter_next)
{
  NvfsTry
    {
      nvfs_server_iter_next (op_args_iter_next->iter) ;

      op_args_iter_next->_base_.status = NVFS_OK ;
    }

  NvfsCatch (op_args_iter_next->_base_.status)
    {
    }
}

