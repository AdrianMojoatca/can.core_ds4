/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: key_private.h 16273 2012-07-31 13:01:18Z martin.bouchard $
/*==========================================================================*/

#ifndef __KEY_PRIVATE_H__
#define __KEY_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "key.h"
#include "dst.h"
#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define KEY_ERR_BUFF_SIZE 5

#define KEY_NVFS_SIZE     sizeof(Dst_Memory)

//--------------------------------------------------------------------------//

typedef enum e_Key_Err_Code
{
  KEY_NO_ERR = 0 ,

  KEY_ERR_BASE_PWD  ,
  KEY_ERR_TRANS_PWD ,


  KEY_ERR_COUNT

}Key_Err_Code ;

//--------------------------------------------------------------------------//

typedef struct s_Key_Error
{
  UInt16             count ;

  Key_Err_Code code [KEY_ERR_BUFF_SIZE] ;

}Key_Error ;

typedef struct
{
  Boolean is_prog ;

}Key_Vars ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Boolean key_init_nvfs (void) ;

Boolean key_error_save (void)              ;
void    key_error_add  (Key_Err_Code code) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern const  NVFS_Entry_Name nvfs_key_error  ;

extern        Key_Vars     key_vars           ;
extern        Key_Error    key_error          ;

#endif

