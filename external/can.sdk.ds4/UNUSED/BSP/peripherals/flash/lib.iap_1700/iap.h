/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: iap.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __IAP_H__
#define __IAP_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef enum
{
  IAP_DEV_ID_LPC1769        = 0x26113F37 ,
  IAP_DEV_ID_LPC1768        = 0x26013F37 ,
  IAP_DEV_ID_LPC1767        = 0x26012837 ,
  IAP_DEV_ID_LPC1766        = 0x26013F33 ,
  IAP_DEV_ID_LPC1765        = 0x26013733 ,
  IAP_DEV_ID_LPC1764        = 0x26011922 ,
  IAP_DEV_ID_LPC1759        = 0x25113737 ,
  IAP_DEV_ID_LPC1758        = 0x25013F37 ,
  IAP_DEV_ID_LPC1756        = 0x25011723 ,
  IAP_DEV_ID_LPC1754        = 0x25011722 ,
  IAP_DEV_ID_LPC1752        = 0x25001121 ,
  IAP_DEV_ID_LPC1751        = 0x25001118 ,
  IAP_DEV_ID_LPC1751_NO_CRP = 0x25001110 ,

} Iap_Dev_ID ;

//--------------------------------------------------------------------------//

typedef enum  // !!! Order important - do not change !!!
{
  IAP_SIZE_256  = 256  ,
  IAP_SIZE_512  = 512  ,
  IAP_SIZE_1024 = 1024 ,
  IAP_SIZE_4096 = 4096 ,

} Iap_Sector_Wr_Size ; // In bytes

//--------------------------------------------------------------------------//

/// These status codes are defined in the LPC2300 datasheet IAP section ///

typedef enum  // !!! Order important - do not change !!!
{
  IAP_CMD_SUCCESS                             =  0u ,
  IAP_INVALID_COMMAND                         =  1u ,
  IAP_SRC_ADDR_ERROR                          =  2u ,
  IAP_DST_ADDR_ERROR                          =  3u ,
  IAP_SRC_ADDR_NOT_MAPPED                     =  4u ,
  IAP_DST_ADDR_NOT_MAPPED                     =  5u ,
  IAP_COUNT_ERROR                             =  6u ,
  IAP_INVALID_SECTOR                          =  7u ,
  IAP_SECTOR_NOT_BLANK                        =  8u ,
  IAP_SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION =  9u ,
  IAP_COMPARE_ERROR                           = 10u ,
  IAP_BUSY                                    = 11u

} Iap_Status ;

//--------------------------------------------------------------------------//

typedef UInt8 Iap_Sernum [16] ;

//--------------------------------------------------------------------------//

typedef struct s_Iap_Sector_Info
{
  UInt32 addr_base ;
  UInt32 addr_last ;

} Iap_Sector_Info  ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

EXTERN_C_ENTER

Iap_Dev_ID iap_rd_device_id     (void) ;
UInt16     iap_rd_boot_code_ver (void) ;
void       iap_rd_device_ser    (Iap_Sernum sernum) ;

Iap_Status iap_sector_prepare   (const UInt8 sector_start , const UInt8 sector_end) ;
Iap_Status iap_sector_erase     (const UInt8 sector_start , const UInt8 sector_end) ;
Iap_Status iap_sector_write     (const void * const dest_flash  , void * const srce_ram , Iap_Sector_Wr_Size const wr_size) ;
SInt16     iap_sector_map_addr  (const void * const addr_flash) ;  // -1 = no such mapping , else sector number
Iap_Status iap_sector_blankcheck(UInt32 sector_start , UInt32 sector_end, UInt32 * first_nblk_loc, UInt32 * first_nblk_val);

EXTERN_C_LEAVE

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern const UInt8 IAP_NUM_SECTORS ;

extern const Iap_Sector_Info iap_sector_info_table [] ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

