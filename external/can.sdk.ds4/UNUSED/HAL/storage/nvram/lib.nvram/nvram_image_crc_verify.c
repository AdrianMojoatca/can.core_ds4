/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvram_image_crc_verify.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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
//! \desc  Determines if the specified NVRAM image is valid by computing
//!        both the header and data CRCs and comparing them to the ones
//!        present in the image.

//! \assume -none-

//! \notes  -none-

//! \param  nvram_image The NVRAM image to check.

//! \return \b void

//! \throws -none-
//--------------------------------------------------------------------------//

Boolean nvram_image_crc_verify (UInt8 const * const nvram_image)
{
  Nvram_Header * const image_header = IMAGE_HEADER (nvram_image) ;
  UInt8        * const image_data   = IMAGE_DATA   (nvram_image) ;

  #define TEST_HEADER (nvram_calc_crc (& image_header->image_info , sizeof (Nvram_Image_Info)) == image_header->image_info_crc     )
  #define TEST_DATA   (nvram_calc_crc (  image_data               , NVRAM_SIZE               ) == image_header->image_info.data_crc)

  return  TEST_HEADER && TEST_DATA ;

  #undef  TEST_DATA
  #undef  TEST_HEADER 
}

