/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvram_vars.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "nvram_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#ifndef NVRAM_FLASH_SIZE
#error !!! Must define FLASH size used by NVRAM library !!!
#endif

//-[ Validate NVRAM size ]--------------------------------------------------//

#if NVRAM_FLASH_SIZE < 0x100 || NVRAM_FLASH_SIZE > 0x1000 || NVRAM_FLASH_SIZE % 0x100 != 0
#error !!! Invalid FLASH size specified - must be from 0x0100 to 0x1000 on 256-byte boundry !!!
#endif

//--------------------------------------------------------------------------//

#ifndef NVRAM_FLASH_BASE_ADDR
#error !!! Must define base address of FLASH block used by NVRAM library !!!
#endif

#ifdef  NVRAM_FLASH_BASE_ADDR_OK
#error !!! Macro name collision - unexpected definition of 'NVRAM_FLASH_BASE_ADDR_OK' !!!
#endif

//-[ Validate LPC2364/66/68 Addresses ]-------------------------------------//

#if defined (DEV_LPC2364) || defined (DEV_LPC2366) || defined (DEV_LPC2368)

#if     NVRAM_FLASH_BASE_ADDR == 0x00000000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00001000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00002000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00003000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00004000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00005000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00006000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00007000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00008000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00010000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00018000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#endif

//-[ Validate LPC2366/68 Addresses ]----------------------------------------//

#if defined (DEV_LPC2366) || defined (DEV_LPC2368)

#if     NVRAM_FLASH_BASE_ADDR == 0x00020000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00028000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00030000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00038000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#endif

//-[ Validate LPC2368 Addresses ]-------------------------------------------//

#if defined (DEV_LPC2368)

#if     NVRAM_FLASH_BASE_ADDR == 0x00040000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00048000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00050000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00058000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00060000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00068000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00070000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00078000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x00079000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x0007A000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x0007B000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x0007C000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#if     NVRAM_FLASH_BASE_ADDR == 0x0007D000
#define NVRAM_FLASH_BASE_ADDR_OK
#endif

#endif

//--------------------------------------------------------------------------//

#ifndef NVRAM_FLASH_BASE_ADDR_OK
#error !!! Invalid FLASH base address specified !!!
#endif

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

Boolean nvram_do_cache_load = TRUE  ;
Boolean nvram_do_cache_save = FALSE ;

//--------------------------------------------------------------------------//

const UInt16        NVRAM_SIZE       =                NVRAM_FLASH_SIZE      ;

      UInt8 * const NVRAM_RAM_256    = (     UInt8 *) 0x40001E60            ;

      void  * const NVRAM_FLASH_BUFF = (      void *) 0x7FD00000            ;
const void  * const NVRAM_FLASH_BASE = (const void *) NVRAM_FLASH_BASE_ADDR ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

