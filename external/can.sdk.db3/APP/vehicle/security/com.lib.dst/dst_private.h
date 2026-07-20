/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dst_private.h 24713 2014-03-05 18:48:51Z martin.bouchard $
/*==========================================================================*/

#ifndef __DST_PRIVATE_H__
#define __DST_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dst.h"
#include "nvfs_usr.h"
#include "dstcrypt.h"
#include <string.h>
#include "dbg.h"

#include "dst_bus.h"

#if DBG_DST == 1
  #warning "DBG_DST is Enabled"
#endif


/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//

#define DST_ERR_BUFF_SIZE           5

//--------------------------------------------------------------------------//

typedef enum
{
   NO_DST_ERR                     ,

   DST_ERR_WRITE                  ,

   DST_ERR_ISK_SEARCH_LOG_INVALID ,
   DST_ERR_ISK_SEARCH_NOT_FOUND   ,

   DST_ERR_ISK_MISTMATCH          , //not a real error but we log it 

   DST_ERR_COUNT 

}Dst_Error_Code ;

//--------------------------------------------------------------------------//

typedef struct s_Dst_Bus_Error
{
    UInt16             count                ;
    
    Dst_Error_Code code [DST_ERR_BUFF_SIZE] ;
      
} Dst_Error ;

//--------------------------------------------------------------------------//
typedef struct s_Dst_Vars
{  
  Boolean log_done             ;
  UInt8   log_count            ;
  
  Dst_Log log      [DST_LOG_SIZE]       ;
    
  Dst_Memory    * key_mem               ;

  Dst_Error       err                   ;
   
} Dst_Vars ;


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Boolean         dst_init_nvfs           (void)                                      ;

Dst_Trans_Resp  dst_cmd_gen_read        (UInt8 page   , UInt8 * data , UInt8 size)  ; 
Dst_Trans_Resp  dst_cmd_sel_read        (UInt8 page   , UInt8 * data , UInt8 size)  ; 

Dst_Trans_Resp  dst_cmd_write           (UInt8 page   , UInt8 * data , UInt8 size)  ; 
void            dst_log_update_page     (UInt8 page   , UInt8 * data , UInt8 size)  ;

Dst_Trans_Resp  dst_cmd_lock            (UInt8 page)                                ;

Boolean         dst_sel_addr_is_valid   (UInt8 * data)                              ;
Boolean         dst_get_lock_status     (UInt8   page)                              ;

void            dst_log_update_sel_addr (UInt8 * data)                              ;
void            dst_log_update_user_data(UInt8 * data)                              ;
void            dst_log_update_ma       (UInt8 * data)                              ;
void            dst_log_update_challenge(UInt8 * data)                              ;
void            dst_log_update_uid      (UInt8 * data)                              ;
void            dst_log_update_response (UInt8 * data)                              ;
void            dst_log_update_isk      (UInt8 * data , UInt8 size)                 ;

void            dst_err_add             (Dst_Error_Code code)                       ;
Boolean         dst_err_save            (void)                                      ;

void            dst_dbg_print           (StrConst * text ,  const UInt8 data[] , UInt8 size) ;
                           
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern const NVFS_Entry_Name nvfs_dst_log   ;
extern const NVFS_Entry_Name nvfs_dst_error ;

extern Dst_Vars  dst_vars ;

#endif

