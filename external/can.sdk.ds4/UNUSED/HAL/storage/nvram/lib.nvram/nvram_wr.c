/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvram_wr.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include <string.h>

#include "nvram_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Nvram_Op_Args_Wr // : public Nvram_Op_Args
{
  Nvram_Op_Args _base_          ; // !!! Must Be First Field !!!

  UInt16       nvram_addr ;
  const void * data_addr  ;
  UInt16       size       ;

} Nvram_Op_Args_Wr ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void nvram_server_executed_wr (Nvram_Op_Args_Wr * op_args_wr) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//
//! \desc  This function will write a block of user data to the internal
//!        NVRAM RAM cache.

//! \assume -none-

//! \notes  -none-

//! \param  nvram_addr The logical destination address in NVRAM to write to. \n
//! \param  data_addr  The physical destination address to read from.\n
//! \param  size       The total numberof bytes to transfer.              

//! \return \b Boolean \b TRUE, if write was ok.\n
//!                    \b FALSE, if there was an error.

//! \throws -none-
//--------------------------------------------------------------------------//

Boolean nvram_wr (UInt16 nvram_addr , const void * data_addr , UInt16 size)
{
  if (nvram_vars.is_initialized)
    {
      Nvram_Op_Args_Wr op_args_wr ;
    
      NVRAM_OP_ARGS_CONTRUCT (op_args_wr._base_ , nvram_server_executed_wr) ;
    
      op_args_wr.nvram_addr = nvram_addr ;
      op_args_wr.data_addr  = data_addr  ;
      op_args_wr.size       = size       ;
    
      nvram_server_call ((Nvram_Op_Args *) & op_args_wr) ;
    
      return op_args_wr._base_.status == NVRAM_OK ;
    }

  return FALSE ;
}

//--------------------------------------------------------------------------//

static void nvram_server_executed_wr (Nvram_Op_Args_Wr * op_args_wr)
{
  op_args_wr->_base_.status = NVRAM_OK ;

  NvramTry
    {
      nvram_check_range  (op_args_wr->nvram_addr , op_args_wr->size) ;

      memcpy (IMAGE_DATA (nvram_image_cache) + op_args_wr->nvram_addr , op_args_wr->data_addr , op_args_wr->size) ;

      nvram_vars.is_cache_modified = TRUE ;
    }

  NvramCatch (op_args_wr->_base_.status)
    {
    }
}

