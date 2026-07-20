/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvram_check_range.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//
//! \desc  Checks to see that all elements in the NVRAM memory block
//!        specified by the start adress and size are in the valid range
//!        in the NVRAM logical memory map.

//! \assume Has been called from within a 'NvramTry' block.

//! \notes  -none-

//! \param  nvram_addr Is a logical address within the range of 0 to NVRAM_SIZE\n
//! \param  size       The size of a block within the NVRAM logical memory map.

//! \return \b void
//!

//! \throws \b Nvram_Status : \b NVRAM_ERR_RANGE If error detected.
//--------------------------------------------------------------------------//

void nvram_check_range (UInt16 nvram_addr , UInt16 size)
{
  if (nvram_addr >= NVRAM_SIZE || size > NVRAM_SIZE - nvram_addr)
    {
      NvramThrow NVRAM_ERR_RANGE ;
    }
}

