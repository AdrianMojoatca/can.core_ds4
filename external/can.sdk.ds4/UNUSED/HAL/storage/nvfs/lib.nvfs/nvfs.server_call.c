/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs.server_call.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//
//! \desc   This function acts as a common entry point for all NVRAM
//!         operations.
//!         
//!         Depending on the value of NVRAM_USE_OS, it will either execute
//!         the callers operation in the callers thread or on a separate
//!         dedicated server thread.

//! \assume -none-

//! \notes -none-

//! \param op_args : Pointer to operation arguments as initialized by caller.
//!        op_func : Pointer to function to execute with arguments.

//! \return -none-

//! \throws -none-
//--------------------------------------------------------------------------//

void nvfs_server_call (Nvfs_Op_Args * const op_args , Nvfs_Op_Func const op_func)
{
  nvfs_atomic_request () ;
    {
      #if NVFS_USE_OS == 0
    
        ATOMIC (op_func (op_args)) ;
    
      #else
    
        op_args->op_func = op_func ;
    
        os_msg_send (nvfs_vars.tsk_server , op_args , 0) ;
    
      #endif
    }
  nvfs_atomic_release () ;
}

