/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dstcrypt.h 17555 2012-10-10 17:10:30Z vijay.rudrappa $
/*==========================================================================*/

#ifndef __DSTCRYPT_H__
#define __DSTCRYPT_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"
#include "nvfs_usr.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define DSTCRYPT_KEY40_SIZE 5
#define DSTCRYPT_KEY80_SIZE 10

//--------------------------------------------------------------------------//

typedef enum
{
  DST_40         =  0 ,
  DST_PLUS       =  1 ,
  DST_80         =  2 ,
  DST_AES        =  3 ,

  DST_NOT_DEFINE = -1 ,

    
}Dst_Device_Type ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Boolean         dstcrypt_delete_isk (void)                                      ; 
Boolean         dstcrypt_load_isk   (void)                                      ; 

void            dstcrypt            (UInt8 * addr , Dst_Device_Type device_type);

Boolean         dstcrypt_write_isk  (const UInt8 * isk , UInt8 size)            ;
Boolean         dstcrypt_set_isk    (const UInt8 * isk , UInt8 size)            ;

Dst_Device_Type dstcrypt_get_device_type(void)                                  ;

Boolean         dstcrypt_init_nvfs  (void)                           ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/




#endif

