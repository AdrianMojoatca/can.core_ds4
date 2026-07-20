/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs.init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "nvfs_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#ifndef  NVFS_SERVER_STK
#define  NVFS_SERVER_STK 0x290
#warning NVFS Server stack size defaulted to 0x290 !
#endif

//--------------------------------------------------------------------------//

#ifndef  NVFS_SERVER_PRIORITY
#define  NVFS_SERVER_PRIORITY OS_PRI_MAX
#warning NVFS Server priority defaulted to OS maximum !
#endif

//--------------------------------------------------------------------------//

typedef struct s_Nvfs_Op_Args_Init // : public Op_Args
{
  Nvfs_Op_Args _base_    ;         // !!! Must Be First Field !!!

  UInt16  fs_base   ;
  UInt16  fs_size   ;
  Boolean do_format ;

} Nvfs_Op_Args_Init ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void nvfs_server_executed_init (Nvfs_Op_Args_Init * op_args_init) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

NVFS_Status nvfs_init (UInt16 fs_base , UInt16 fs_size , Boolean do_format)
{
  /*------------------------------------------------------------------------------------------------*\
  | If OS is used, create an actual server task.  If not, the callers thread will act as the server. |
  \*------------------------------------------------------------------------------------------------*/

  #if NVFS_USE_OS == 1
  
  if(os_irq_executing)
  { 
  	return NVFS_ERR_INVALID_CALL;
  }
  
  if (nvfs_vars.tsk_server == 0)
    {
      nvfs_vars.tsk_server = os_tsk_create (nvfs_tsk_server , NVFS_SERVER_PRIORITY , NVFS_SERVER_STK , 0 , "nvfs_server") ;
    
      os_tsk_start (nvfs_vars.tsk_server , 0) ;
    }

  #endif

  nvfs_atomic_init () ;

  initialize :
    {
      Nvfs_Op_Args_Init op_args_init ;

      op_args_init.fs_base   = fs_base   ;
      op_args_init.fs_size   = fs_size   ;
      op_args_init.do_format = do_format ;
    
      NVFS_SERVER_CALL      (op_args_init , nvfs_server_executed_init) ;
      
      NVFS_EXCEPTION_REPORT (op_args_init._base_.status) ;
    
      return op_args_init._base_.status ;
    }
}

//--------------------------------------------------------------------------//

static void nvfs_server_executed_init (Nvfs_Op_Args_Init * op_args_init)
{
  NvfsTry
    {
      if (op_args_init->fs_base + op_args_init->fs_size > NVRAM_SIZE || op_args_init->fs_size <= sizeof (NVFS_Header))
        {
          NvfsThrow NVFS_ERROR ;
        }
    
      nvfs_vars.nvram.beg = op_args_init->fs_base                             ;
      nvfs_vars.nvram.end = op_args_init->fs_base + op_args_init->fs_size - 1 ;
  
      (op_args_init->do_format ? nvfs_init_format : nvfs_init_existing) ()    ;

      nvfs_vars.is_initialized    = TRUE    ;

      op_args_init->_base_.status = NVFS_OK ;
    }
  
  NvfsCatch (op_args_init->_base_.status)
    {
    }
}

