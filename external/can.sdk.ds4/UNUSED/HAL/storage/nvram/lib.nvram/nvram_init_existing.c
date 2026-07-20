/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvram_init_existing.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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
//! \desc   This routine will attempt to read the contents of FLASH memory
//!         into the on-chip cache RAM.  There are three possible scenarios: \n
//!
//!         \li (1) Both pages are valid:\n
//!                 Read the oldest one into the RAM cache.
//!
//!         \li (2) Only one page is valid:\n
//!                 Read the valid page into RAM and copy it over teh invalid page.
//!
//!         \li (3) No valid pages are found:\n
//!                 Return an error status.

//! \assume \li Linker scatter file has correctly located cache, iap_wr, and flash variables.

//! \notes -none- 

//  \param -void-

//! \return \li \b NVRAM_OK           : Initiallization was completed correctly.
//!         \li \b NVRAM_OK_RECOVERED : Only one FLASH page was valid, the invalid one was forced to mirror the valid one.
//!         \li \b NVRAM_ERR_WR_FAIL  : Unable to update FLASH.
//!         \li \b NVRAM_ERR_INTERNAL : Unexpected internal error occured.

//! \throws Nvram_Status 

//--------------------------------------------------------------------------//

Nvram_Status nvram_init_existing (void)
{
  Nvram_Status status = NVRAM_OK ;

  Boolean const is_flash_0_valid = nvram_image_crc_verify (nvram_image_flash_0) ;
  Boolean const is_flash_1_valid = nvram_image_crc_verify (nvram_image_flash_1) ;

  if (!is_flash_0_valid && !is_flash_1_valid)
    {
      NvramThrow NVRAM_ERR_CORRUPTED ;
    }
  else
    {
      typedef struct
        {
          UInt8              * const image                  ;
          Nvram_Image_Flash_ID const image_flash_recent_new ;
      
        } Order ;
      
      static const Order order [] = // !!! Order Critical !!!
        {
          { nvram_image_flash_0 , IMAGE_FLASH_0 } ,
          { nvram_image_flash_1 , IMAGE_FLASH_1 } ,
        } ;
    
      SInt8 index_rd      ;
      SInt8 index_wr = -1 ;

      if (is_flash_0_valid && is_flash_1_valid)
        {
          #define AGE_0 (IMAGE_HEADER (nvram_image_flash_0)->image_info.data_age)
          #define AGE_1 (IMAGE_HEADER (nvram_image_flash_1)->image_info.data_age)
    
          index_rd = AGE_0 < AGE_1 ;  // 0 -> FLASH_0 , 1 -> FLASH_1 : Oldest

          #undef  AGE_1
          #undef  AGE_0
        }
      else
        {
          index_rd = ! is_flash_0_valid ;
          index_wr = ! is_flash_1_valid ;

          status   = NVRAM_OK_RECOVERED ;
        }

      nvram_image_cache_rd           (order[index_rd].image) ;
    
      nvram_vars.image_flash_recent = order[index_rd].image_flash_recent_new ;

      if (index_wr >= 0)
        {
          nvram_image_cache_wr (order[index_wr].image) ;
        }
    }

  return status ;
}

