/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvram_image_cache_wr.c 33268 2015-08-19 15:40:18Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include <string.h>

#ifdef DEV_LPC_236x
	#include "iap.h"
#endif
#include "nvram_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void nvram_sector_write_block (UInt8 sector_num , void * const dest , void const * const srce) ;

static void nvram_sector_execute_cmd (UInt8 sector_num , Iap_Status (* iap_sector_operation)(const UInt8 , const UInt8)) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//
//! \desc Will write an NVRAM image from cache to the specified destinateion
//!       FLASH address.

//! \assume The destination address is a valid FLASH page as defined in the scatter file.
//!         Has been called from within a 'NvramTry' block. 

//! \notes  -none-

//! \param  nvram_image_flash Addess of a valid NVRAM image.

//! \return \b void

//! \throws -none-
//--------------------------------------------------------------------------//

void nvram_image_cache_wr (UInt8 * const nvram_image_flash)
{
  /*--------------------------------------------------------------------------*\
  | Fetch sector number for specified FLASH memory region.  No need to check   |
  | result since it is validated during 'nvram_init' and never changes because |
  | it is static at link time.                                                 |
  \*--------------------------------------------------------------------------*/

  UInt8 sector_num = (UInt8) iap_sector_map_addr (nvram_image_flash) ;

  /*---------------------------------------------------------------------------------*\
  | Before a sector can be erased, it must first be prepared.  We call IAP 'prepare'  |
  | to do this and then follow it with IAP 'erase'.                                   |
  \*---------------------------------------------------------------------------------*/

  nvram_sector_execute_cmd (sector_num , iap_sector_prepare) ;
  nvram_sector_execute_cmd (sector_num , iap_sector_erase  ) ;

  /*-----------------------------------------------------------------------------------------*\
  | Now write the RAM cache to the FLASH page to make it permanent.                           |
  |                                                                                           |
  | Write the cache as a sequence of 256-byte blocks using the special                        |
  | 256-byte buffer mapped in the scatter file that is gauranteed to be located               |
  | in on-chip RAM.  We need to do it this way as the IAP write routine requires              |
  | source memory to be located in on-chip RAM.  This allows us to reserve a small            |
  | amount of on-chip RAM and let the main cache RAM be located anywhere in the memory space. |
  |                                                                                           |
  | Note that we still have to do an IAP 'prepare' prior to each IAP write                    |
  | even though the write block is only a sub-set of the entire FLASH page.                   |
  \*-----------------------------------------------------------------------------------------*/

  write_cache :
    {
      UInt8   block = 0 ;

      UInt8 * srce  = nvram_image_cache ;
      UInt8 * dest  = nvram_image_flash ;

      do
        {
          nvram_sector_write_block (sector_num , dest , srce) ;

          srce += NVRAM_IAP_WR_BUFF_SIZE ;
          dest += NVRAM_IAP_WR_BUFF_SIZE ;
        }
      while (++block < NVRAM_IMAGE_SIZE / NVRAM_IAP_WR_BUFF_SIZE) ;
    }
}

//--------------------------------------------------------------------------//

static void nvram_sector_execute_cmd (UInt8 sector_num , Iap_Status (* iap_sector_operation)(const UInt8 , const UInt8))
{
  if (iap_sector_operation (sector_num , sector_num) != IAP_CMD_SUCCESS)
    {
      NvramThrow NVRAM_ERR_WR_FAIL ;
    }
}

//--------------------------------------------------------------------------//

static void nvram_sector_write_block (UInt8 sector_num , void * const dest , void const * const srce)
{
  memcpy (nvram_iap_wr_buff , srce , NVRAM_IAP_WR_BUFF_SIZE) ;

  nvram_sector_execute_cmd (sector_num , iap_sector_prepare) ;

  if (iap_sector_write (dest , nvram_iap_wr_buff , IAP_SIZE_256) != IAP_CMD_SUCCESS)
    {
      NvramThrow NVRAM_ERR_WR_FAIL ;
    }
}

