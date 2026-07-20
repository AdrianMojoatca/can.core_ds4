/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvram_rd.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include <string.h>

#include "nvram_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Nvram_Op_Args_Rd // : public Nvram_Op_Args
{
  Nvram_Op_Args _base_    ;       // !!! Must Be First Field !!!

  UInt16 nvram_addr ;
  void * data_addr  ;
  UInt16 size       ;

} Nvram_Op_Args_Rd ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void nvram_server_executed_rd (Nvram_Op_Args_Rd * op_args_rd) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//
//! \desc  This function will read a block of user data from the internal
//!        NVRAM RAM cache and copy it to the users specified destination.

//! \assume -none-

//! \notes  -none-

//! \param  nvram_addr The logical source address in NVRAM to read from. \n
//! \param  data_addr  The physical destination address to write to.\n
//! \param  size       The total numberof bytes to transfer.              

//! \return \b Boolean \b TRUE, if read was ok.\n
//!                    \b FALSE, if there was an error.

//! \throws -none-
//--------------------------------------------------------------------------//

Boolean nvram_rd (UInt16 nvram_addr , void * data_addr , UInt16 size)
{
  if (nvram_vars.is_initialized)
    {
      Nvram_Op_Args_Rd op_args_rd ;
    
      NVRAM_OP_ARGS_CONTRUCT (op_args_rd._base_ , nvram_server_executed_rd) ;
    
      op_args_rd.nvram_addr = nvram_addr ;
      op_args_rd.data_addr  = data_addr  ;
      op_args_rd.size       = size       ;
    
      nvram_server_call ((Nvram_Op_Args *) & op_args_rd) ;
    
      return op_args_rd._base_.status == NVRAM_OK ;
    }

  return FALSE ;
}

//--------------------------------------------------------------------------//

static void nvram_server_executed_rd (Nvram_Op_Args_Rd * op_args_rd)
{
  op_args_rd->_base_.status = NVRAM_OK ;

  NvramTry
    {
      nvram_check_range  (op_args_rd->nvram_addr , op_args_rd->size) ;

      memcpy (op_args_rd->data_addr , IMAGE_DATA (nvram_image_cache) + op_args_rd->nvram_addr , op_args_rd->size) ;
    }

  NvramCatch (op_args_rd->_base_.status)
    {
    }
}

