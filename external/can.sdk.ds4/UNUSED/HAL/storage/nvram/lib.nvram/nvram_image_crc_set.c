/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvram_image_crc_set.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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
//! \desc Calculates both the data and header CRC check values and writes
//!       them into the image info segment of the NVRAM header.
//!
//!       First the data CRC is computed and written to the image_info field.
//!       Then the entire image_info CRC is computed and written to the
//!       header block.

//! \assume -none-

//! \notes  -none-

//! \param  nvram_image : Base address of a NVRAM image.

//! \return \b void

//! \throws -none-

void nvram_image_crc_set (UInt8 * const nvram_image)
{
  Nvram_Header * const image_header = IMAGE_HEADER   (nvram_image) ;
  UInt8        * const image_data   = IMAGE_DATA     (nvram_image) ;

  image_header->image_info.data_crc = nvram_calc_crc (  image_data               , NVRAM_SIZE               ) ;
  image_header->image_info_crc      = nvram_calc_crc (& image_header->image_info , sizeof (Nvram_Image_Info)) ;
}

