/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvram_cache_commit.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include <string.h>

#include "iap.h"
#include "nvram_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Boolean sector_erase    (void) ;
static Boolean sector_write    (void) ;
static Boolean sector_prepare  (void) ;

static Boolean is_write_needed (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static SInt16 sector ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean nvram_cache_commit (void)
{
  Boolean status = TRUE ;

  ATOMIC
    (
      if (nvram_do_cache_save && is_write_needed ())
        {
          sector = iap_sector_map_addr (NVRAM_FLASH_BASE) ;
    
          status = sector != -1 && sector_erase () && sector_write () ;
    
          if (status)
            {
              nvram_do_cache_save = FALSE ;
            }
        }
    )

  return status ;
}

//--------------------------------------------------------------------------//

static Boolean sector_erase (void)
{
  return sector_prepare () && iap_sector_erase (sector , sector) == IAP_CMD_SUCCESS ;
}

//--------------------------------------------------------------------------//

static Boolean sector_write (void)
{
  const UInt16  move_size  = 256 ;

  UInt16        data_size  = NVRAM_SIZE ;

  const UInt8 * data_srce  = NVRAM_FLASH_BUFF ;

  const UInt8 * flash_dest = NVRAM_FLASH_BASE ;

  for (;;)
    {
      memcpy (NVRAM_RAM_256 , data_srce , move_size) ;

      if (! (sector_prepare () && iap_sector_write (flash_dest , NVRAM_RAM_256 , IAP_SIZE_256) == IAP_CMD_SUCCESS))
        {
          return FALSE ;
        }

      data_size  -= move_size ;

      if (data_size == 0)
        {
          return TRUE ;
        }

      data_srce  += move_size ;
    
      flash_dest += move_size ;
    }  
}

//--------------------------------------------------------------------------//

static Boolean sector_prepare (void)
{
  return iap_sector_prepare (sector , sector) == IAP_CMD_SUCCESS ;
}

//--------------------------------------------------------------------------//

static Boolean is_write_needed (void)
{
  auto  UInt16  data_size  = NVRAM_SIZE ;

  const UInt8 * data_srce  = NVRAM_FLASH_BUFF ;

  const UInt8 * flash_dest = NVRAM_FLASH_BASE ;

  do
    {
      if (*flash_dest++ != *data_srce++)
        {
          return TRUE ;
        }
    }
  while (--data_size) ;

  return FALSE ;
}

