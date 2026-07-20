/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvram_vars.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

//==========================================================================//
// Note #1                                                                  //
//                                                                          //
// In order to ensure that global variables for FLASH, IAP write, and       //
// CACHE buffers are not initialized, they must have two attributes set     //
// at the same time:                                                        //
//                                                                          //
// 1) The 'C' '__attribute__' must include 'zero_init'                      //
//                                                                          //
// 2) The scatter file attribute for the section must have 'UNINIT'         //
//                                                                          //
// See: http://www.keil.com/support/docs/3480.htm                           //
//      http://www.keil.com/forum/11937/                                    //
//                                                                          //
// Note #2                                                                  //
//                                                                          //
// - FLASH blocks *must* be 4k in size and *must* be aligned on a 4k        //
//   boundry.                                                               //
//   Must not be initialized by startup code!                               //
//                                                                          //
// - IAP Write buffer *must* be 256 bytes and aligned on 256 byte boundry.  //
//   It must also be located in on-chip RAM.  Cannot be in USB or ETHERNET  //
//   RAM.                                                                   //
//   It may be initialized, but does not need to.                           //
//                                                                          //
// - CACHE block *must* be 4k in size and *must* be aligned on 4-byte       //
//   boundry.                                                               //
//   It may be located in any RAM (ON-CHIP or USB or ETHERNET).             //
//   It may be initialized, but does not need to.                           //
//                                                                          //
//==========================================================================//

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

Nvram_Vars   nvram_vars ;

const UInt16 NVRAM_SIZE = NVRAM_IMAGE_SIZE - sizeof (Nvram_Header) ;

UInt8        nvram_image_cache   [NVRAM_IMAGE_SIZE]       __attribute__ ((section ("NVRAM_IMAGE_CACHE"  ) , aligned (4)                       , zero_init)) ;  

UInt8        nvram_image_flash_0 [NVRAM_IMAGE_SIZE]       __attribute__ ((section ("NVRAM_IMAGE_FLASH_0") , aligned (NVRAM_IMAGE_ALIGN)       , zero_init)) ; 
UInt8        nvram_image_flash_1 [NVRAM_IMAGE_SIZE]       __attribute__ ((section ("NVRAM_IMAGE_FLASH_1") , aligned (NVRAM_IMAGE_ALIGN)       , zero_init)) ; 

UInt8        nvram_iap_wr_buff   [NVRAM_IAP_WR_BUFF_SIZE] __attribute__ ((section ("NVRAM_IAP_WR_BUFF"  ) , aligned (NVRAM_IAP_WR_BUFF_ALIGN) , zero_init)) ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

