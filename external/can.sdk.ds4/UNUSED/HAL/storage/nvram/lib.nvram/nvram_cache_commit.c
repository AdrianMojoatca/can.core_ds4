/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvram_cache_commit.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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

static void nvram_server_executed_commit (Nvram_Op_Args * op_args) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//
//! \desc This function will write the current image in the RAM cache to 
//!       the oldest FLASH page.  It does not syncronize the two FLASH
//!       images.
//!
//!       If there have been no writes since the last cache load, no action
//!       is taken.

//! \assume - There is a valid FLASH image already loaded into RAM cache.\n

//! \notes  -none-

//  \param  -none-

//! \return \b Boolean \b TRUE if commit was ok. \n
//!                    \b FALSE if write failed.
//!

//! \throws -none-
//--------------------------------------------------------------------------//

Boolean nvram_cache_commit (void)
{
  if (nvram_vars.is_initialized)
    {
      Nvram_Op_Args op_args ;
    
      NVRAM_OP_ARGS_CONTRUCT (op_args , nvram_server_executed_commit) ;
    
      nvram_server_call (& op_args) ;
    
      return op_args.status == NVRAM_OK ;
    }

  return FALSE ;
}

//--------------------------------------------------------------------------//

static void nvram_server_executed_commit (Nvram_Op_Args * op_args)
{
  op_args->status = NVRAM_OK ;

  NvramTry
    {
      if (nvram_vars.is_cache_modified)
        {
          nvram_cache_save (FALSE) ;

          nvram_vars.is_cache_modified = FALSE ;
        }
    }

  NvramCatch (op_args->status)
    {
    }
}

