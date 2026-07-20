/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvram_cache_commit_sync.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "nvram_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void nvram_server_executed_commit_sync (Nvram_Op_Args * op_args) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//
//! \desc This function will write the current RAM cache contents to both
//!       FLASH pages, ensuring that they are both synced to the same data
//!       and age count.

//! \assume - There is a valid FLASH image already loaded into RAM cache.\n

//! \notes  -none-

//  \param  -none-

//! \return \b Boolean \b TRUE  if commit was ok. \n
//!                    \b FALSE if write failed.
//!

//! \throws -none-
//--------------------------------------------------------------------------//

Boolean nvram_cache_commit_sync (void)
{
  if (nvram_vars.is_initialized)
    {
      Nvram_Op_Args op_args ;
    
      NVRAM_OP_ARGS_CONTRUCT (op_args , nvram_server_executed_commit_sync) ;
    
      nvram_server_call (& op_args) ;
    
      return op_args.status == NVRAM_OK ;
    }

  return FALSE ;
}

//--------------------------------------------------------------------------//

static void nvram_server_executed_commit_sync (Nvram_Op_Args * op_args)
{
  op_args->status = NVRAM_OK ;

  NvramTry
    {
      nvram_cache_save (TRUE) ;

      nvram_vars.is_cache_modified = FALSE ;
    }

  NvramCatch (op_args->status)
    {
    }
}

