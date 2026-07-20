/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dstcrypt_private.h 34923 2015-11-02 21:11:27Z martin.bouchard $
/*==========================================================================*/

#ifndef __DSTCRYPT_PRIVATE_H__
#define __DSTCRYPT_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dstcrypt.h"
#include "dbg.h"

#if DBG_DSTCRYPT == 1
  #warning "DBG_DSTCRYPT Is Enable"
#endif


/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct
{
  Dst_Device_Type device_type ;
  
  UInt8 isk[10]               ;

}DstCrypt_Nvfs_Entry ;

typedef struct
{
  Boolean nvfs_entry_done;
}DstCrypt_Vars ;


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void dstcrypt_dbg_print(StrConst * text ,  const UInt8 data[] , UInt8 size) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern DstCrypt_Nvfs_Entry dstcrypt_nvfs_entry    ;

extern DstCrypt_Vars dstcrypt_vars;

extern const NVFS_Entry_Name nvfs_dst_isk ;

extern const UInt8 dst_table4  [32]   ;
extern const UInt8 dst_table32 [32]   ;                        
extern const UInt8 dst_table60 [32]   ;
extern const UInt8 dst_table88 [32]   ;
extern const UInt8 dst_table116[16]   ;
extern const UInt8 dst_table144[16]   ;
extern const UInt8 dst_table172[16]   ;
                                      
#endif
