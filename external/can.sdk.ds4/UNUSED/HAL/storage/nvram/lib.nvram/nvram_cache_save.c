/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvram_cache_save.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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
//! \desc This function is called as part of the commit & commit_sync
//!       operations.  It will age the current image and then write it to
//!       the oldest FLASH page.  If mirroring is requested, it will then
//!       overwrite the most recent page as well in order to syncronize the
//!       two FLASH pages.

//! \assume There is a valid FLASH image already loaded into RAM cache.\n
//!         Called from within a 'NvramTry' block.

//! \notes  The write(s) will occur regardless of whether the cache was
//!         modified or not.

//! \param  do_mirror If \b TRUE, both FLASH pages will be updated.

//! \return \b Boolean \b TRUE if commit was ok. \n
//!                    \b FALSE if write failed.

//! \throws -none-
//--------------------------------------------------------------------------//

void nvram_cache_save (Boolean do_mirror)
{
  switch (nvram_vars.image_flash_recent)
    {
      case IMAGE_FLASH_0 :
      case IMAGE_FLASH_1 : break ;

      default            : NvramThrow NVRAM_ERR_INTERNAL ;
    }

  perform_wr :
    {
      typedef struct
        {
          UInt8 * const image_wr_1 ;
          UInt8 * const image_wr_2 ;
    
          Nvram_Image_Flash_ID const image_flash_recent_new ;
    
        } Wr_Order ;
    
      static const Wr_Order wr_order_table [] = // !!! Order Critical !!!
        {
          { nvram_image_flash_1 , nvram_image_flash_0 , IMAGE_FLASH_1 } ,
          { nvram_image_flash_0 , nvram_image_flash_1 , IMAGE_FLASH_0 } ,
        } ;
    
      const Wr_Order * const wr_order = & wr_order_table [nvram_vars.image_flash_recent] ;
      
      nvram_cache_age      () ;
        
      nvram_image_crc_set  (nvram_image_cache) ;
    
      nvram_image_cache_wr (wr_order->image_wr_1) ;
      
      if (do_mirror)
        {
          nvram_image_cache_wr (wr_order->image_wr_2) ;
        }
    
      nvram_vars.image_flash_recent = wr_order->image_flash_recent_new ;
    }
}

