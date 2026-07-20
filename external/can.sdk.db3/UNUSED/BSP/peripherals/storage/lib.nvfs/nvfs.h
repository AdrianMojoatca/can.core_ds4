/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs.h 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

#ifndef __NVFS_H__
#define __NVFS_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

enum // !! BIT-MAPPINGS CRITICAL : FLASH MEMORY DEPENDANT !!!
{
  NVFS_ATTRIB_WR_PROTECTED = (1U << 0) ,

//NVFS_ATTRIB_RESERVED_1   = (1U << 1) // DO-NOT-USE : Must be zero!
//NVFS_ATTRIB_RESERVED_2   = (1U << 2) // DO-NOT-USE : Must be zero! 
//NVFS_ATTRIB_RESERVED_3   = (1U << 3) // DO-NOT-USE : Must be zero! 
//NVFS_ATTRIB_RESERVED_4   = (1U << 4) // DO-NOT-USE : Must be zero! 
//NVFS_ATTRIB_RESERVED_5   = (1U << 5) // DO-NOT-USE : Must be zero! 

  NVFS_ATTRIB_USER_0       = (1U << 6) ,
  NVFS_ATTRIB_USER_1       = (1U << 7) ,
} ;

//--------------------------------------------------------------------------//

typedef UInt8 NVFS_Attributes ;

typedef char  NVFS_Char                   ;
typedef const NVFS_Char * NVFS_Entry_Name ;

//--------------------------------------------------------------------------//

typedef enum
{
  NVFS_OK = 0          ,  // !!! MUST BE ZERO !!!

  NVFS_ERROR           ,

  NVFS_WR_PROTECTED    ,

  NVFS_NAME_NOT_FOUND  ,

  NVFS_NAME_DUPLICATE  ,

  NVFS_FORMAT_UNKNOWN  ,

  NVFS_IRQ_CALL_FAILED ,  // Can only happen if function called from IRQ handler

} NVFS_Status ;

//--------------------------------------------------------------------------//

typedef struct
{
 // public  :

    Boolean is_valid     ;
    Boolean is_truncated ;

 // private :

    NVFS_Char * name_buff       ;
    UInt16      name_buff_size  ;

    UInt16      entry_addr_next ;

} NVFS_Iterator ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

EXTERN_C_ENTER

NVFS_Status nvfs_init               (UInt16 fs_base , UInt16 fs_size , Boolean do_format ) ;                                 // NVFS_OK , NVFS_ERROR , NVFS_IRQ_CALL_FAILED

NVFS_Status nvfs_create             (NVFS_Entry_Name const entry_name , UInt16 entry_size) ;                                 // NVFS_OK , NVFS_ERROR , NVFS_NAME_DUPLICATE , NVFS_IRQ_CALL_FAILED
NVFS_Status nvfs_size_is_valid      (NVFS_Entry_Name const entry_name , UInt16 entry_size) ;

NVFS_Status nvfs_delete             (NVFS_Entry_Name const entry_name) ;                                                     // NVFS_OK , NVFS_ERROR , NVFS_NAME_NOT_FOUND , NVFS_WR_PROTECTED , NVFS_IRQ_CALL_FAILED

// depreciated API, use nvfs_rd_s() instead
NVFS_Status nvfs_rd                 (NVFS_Entry_Name const entry_name , void   * const ram_dest  ) ;                         // NVFS_OK , NVFS_ERROR , NVFS_NAME_NOT_FOUND , NVFS_IRQ_CALL_FAILED
// new API to replace nvfs_rd
NVFS_Status nvfs_rd_s               (NVFS_Entry_Name const entry_name , void   * const ram_dest, UInt16 dest_size  ) ;       // NVFS_OK , NVFS_ERROR , NVFS_NAME_NOT_FOUND , NVFS_IRQ_CALL_FAILED

NVFS_Status nvfs_rd_size            (NVFS_Entry_Name const entry_name , UInt16 * const entry_size) ;                         // NVFS_OK , NVFS_ERROR , NVFS_NAME_NOT_FOUND , NVFS_IRQ_CALL_FAILED

NVFS_Status nvfs_wr                 (NVFS_Entry_Name const entry_name , const void * const ram_srce) ;                       // NVFS_OK , NVFS_ERROR , NVFS_NAME_NOT_FOUND , NVFS_WR_PROTECTED , NVFS_IRQ_CALL_FAILED
NVFS_Status nvfs_wr_create          (NVFS_Entry_Name const entry_name , const void * const ram_srce , UInt16 entry_size) ;   // NVFS_OK , NVFS_ERROR , NVFS_WR_PROTECTED   , NVFS_IRQ_CALL_FAILED
NVFS_Status nvfs_wr_delete          (NVFS_Entry_Name const entry_name , const void * const ram_srce , UInt16 entry_size) ;   // NVFS_OK , NVFS_ERROR , NVFS_WR_PROTECTED   , NVFS_IRQ_CALL_FAILED

NVFS_Status nvfs_iter_init          (NVFS_Iterator * const iter , NVFS_Char * const entry_name , UInt16 max_name_size) ;     // NVFS_OK , NVFS_ERROR , NVFS_IRQ_CALL_FAILED
NVFS_Status nvfs_iter_next          (NVFS_Iterator * const iter) ;                                                           // NVFS_OK , NVFS_ERROR , NVFS_IRQ_CALL_FAILED

NVFS_Status nvfs_set_attributes     (NVFS_Entry_Name const entry_name , NVFS_Attributes         attributes) ;                // NVFS_OK , NVFS_ERROR , NVFS_NAME_NOT_FOUND , NVFS_IRQ_CALL_FAILED 
NVFS_Status nvfs_get_attributes     (NVFS_Entry_Name const entry_name , NVFS_Attributes * const attributes) ;                // NVFS_OK , NVFS_ERROR , NVFS_NAME_NOT_FOUND , NVFS_IRQ_CALL_FAILED 

NVFS_Status nvfs_bit_set_attributes (NVFS_Entry_Name const entry_name , NVFS_Attributes         attributes_flags) ;          // NVFS_OK , NVFS_ERROR , NVFS_NAME_NOT_FOUND , NVFS_IRQ_CALL_FAILED 
NVFS_Status nvfs_bit_clr_attributes (NVFS_Entry_Name const entry_name , NVFS_Attributes         attributes_flags) ;          // NVFS_OK , NVFS_ERROR , NVFS_NAME_NOT_FOUND , NVFS_IRQ_CALL_FAILED 

/// Atomic Access Control \\\

Boolean     nvfs_atomic_request     (void) ;
void        nvfs_atomic_release     (void) ;

EXTERN_C_LEAVE

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

