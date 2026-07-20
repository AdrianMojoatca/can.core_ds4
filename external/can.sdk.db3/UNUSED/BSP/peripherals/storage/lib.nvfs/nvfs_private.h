/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs_private.h 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

#ifndef __NVFS_PRIVATE_H__
#define __NVFS_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include <setjmp.h>

#include "device.h"

//--------------------------------------------------------------------------//

#ifndef   NVFS_USE_OS
  #error !!! NVFS_USE_OS Must be explicitly defined by user !!!
#else

  #if NVFS_USE_OS == 1
    #include "os.h"
  #endif

  #if NVFS_US_OS != 0 && NVFS_USE_OS != 1
    #error !!! NVFS_USE_OS must be either 0 or 1 !!!
  #endif

#endif

//--------------------------------------------------------------------------//

#include "nvram.h"
#include "nvfs.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define NVFS_CRC_INIT 0xFF

//--------------------------------------------------------------------------//

#ifndef NVFS_FORMAT_FILL_VAL
#define NVFS_FORMAT_FILL_VAL 0xFF
#endif

//--------------------------------------------------------------------------//

enum // !!! ORDER IS CRITICAL : DO NOT CHANGE !!!
{
  NVFS_FORMAT_TYPE_1_00
} ;
  
//--------------------------------------------------------------------------//

typedef struct
{
  UInt16 addr ;
  UInt16 size ;

  UInt8  crc  ;

} NVFS_Entry_Link ;

//--------------------------------------------------------------------------//

typedef struct
{
  struct
    {
      NVFS_Entry_Link name  ;
      NVFS_Entry_Link data  ;

      UInt16          next  ;

      NVFS_Attributes attributes ;

    } link ;

  UInt8 crc ;

} NVFS_Entry ;

//--------------------------------------------------------------------------//

typedef struct
{
  UInt8 format_type   ; // !!! MUST ALWAYS BE FIRST FIELD : NOT PART OF CRC CALCULATION : MUST BE UINT8 !!!

  struct
    {
      UInt16 free ;     // Where new entries start 

      struct
        {
          UInt16 root ; // 0 = NULL
          UInt16 last ; // 0 = NULL
  
        } list  ;

    } mem ;

  UInt8 crc ;

} NVFS_Header ;

//--------------------------------------------------------------------------//

typedef struct
{
  #if NVFS_USE_OS == 1

    Res    res ;         // OS resource object

  #else

    int    irq_state  ;  // IRQ state prior to first nvfs_request () call

    UInt16 num_grants ;  // Number of concurrent request grants active

  #endif

} NVFS_Atomic ;

//--------------------------------------------------------------------------//

typedef struct
{
  NVFS_Atomic atomic ; // Atomic access control variables

  struct
    {
      UInt16 beg ;     // NVRAM begining for fs
      UInt16 end ;     // NVRAM end      for fs

    } nvram ;

  jmp_buf throw_vec  ; // Exception handling vector for errors

} NVFS_Vars ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void    nvfs_throw               (NVFS_Status nvfs_status) ;

UInt8   nvfs_rd_nvram            (UInt16 addr) ;
void    nvfs_wr_nvram            (UInt16 addr , UInt8 data) ;

void    nvfs_rd_header           (NVFS_Header * const nvfs_header) ;
void    nvfs_wr_header           (NVFS_Header * const nvfs_header) ;

void    nvfs_rd_entry            (NVFS_Entry  * const nvfs_entry , UInt16 entry_addr) ;
void    nvfs_wr_entry            (NVFS_Entry  * const nvfs_entry , UInt16 entry_addr) ;

UInt8   nvfs_crc_calc            (void * data , UInt16 size) ;

void    nvfs_crc_calc_header     (NVFS_Header * const nvfs_header) ;
void    nvfs_crc_test_header     (NVFS_Header * const nvfs_header) ;

void    nvfs_crc_test_entry      (NVFS_Entry  * const nvfs_entry) ;
void    nvfs_crc_calc_entry      (NVFS_Entry  * const nvfs_entry) ;

void    nvfs_crc_test_entry_link (NVFS_Entry_Link * const nvfs_entry_link) ;
void    nvfs_crc_calc_entry_link (NVFS_Entry_Link * const nvfs_entry_link) ;

void    nvfs_range_check         (UInt16 nvram_addr) ;

void    nvfs_init_format         (void) ;
void    nvfs_init_existing       (void) ;

Boolean nvfs_find_entry          (NVFS_Entry_Name const entry_name , UInt16 * const entry_addr , UInt16 * const entry_addr_prev) ;

void    nvfs_atomic_init         (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern NVFS_Vars nvfs_vars ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

