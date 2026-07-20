/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvram_init_format.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include <string.h>

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
//! \desc Performs an explicit format of both FLASH pages and loads them \n
//!       with $FF default values.  Cache memory is also loaded with $FF.

//! \assume -none-

//! \notes  -none-

//  \param  -void-

//! \return \li \b NVRAM_OK           : Format was completed correctly.
//!         \li \b NVRAM_ERR_WR_FAIL  : Unable to update FLASH.
//!         \li \b NVRAM_ERR_INTERNAL : Unexpected internal error occured.

//! \throws Nvram_Status 

//--------------------------------------------------------------------------//

Nvram_Status nvram_init_format (void)
{
  Nvram_Header * const image_header = IMAGE_HEADER (nvram_image_cache) ;
  UInt8        * const image_data   = IMAGE_DATA   (nvram_image_cache) ;

  /*--------------------------------*\
  | Initialize cache to virgin state |
  \*--------------------------------*/

  image_header->image_info.data_age = 0   ;
  
  memset (image_data , 0xFF , NVRAM_SIZE) ;

  /*--------------------------------------------------------------------------------------------*\
  | Update cache crc info and write cache to both flash buffers to do initial syncronized commit |
  \*--------------------------------------------------------------------------------------------*/

  nvram_image_crc_set  (nvram_image_cache  ) ;

  nvram_image_cache_wr (nvram_image_flash_0) ;
  nvram_image_cache_wr (nvram_image_flash_1) ;

  /*----------------------------------------------------------*\
  | Update local status/control variables to reflect new state |
  \*----------------------------------------------------------*/

  nvram_vars.image_flash_recent = IMAGE_FLASH_0 ;

  return NVRAM_OK ;
}

