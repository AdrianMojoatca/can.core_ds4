/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dst.h 21825 2013-07-25 18:42:00Z louis-philippe.rispoli $
/*==========================================================================*/

#ifndef __DST_H__
#define __DST_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"
#include "dstcrypt.h"
#include "nvfs_usr.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define DST_PAGE_STATUS_UNLOCK 0
#define DST_PAGE_STATUS_LOCK   1

//--------------------------------------------------------------------------//

#define DST_PWD_SIZE                1
#define DST_USR_DATA_SIZE           1
#define DST_MA_SIZE                 1
#define DST_SIGNATURE_SIZE          3 
#define DST_UID_SIZE_NO_MA          3
#define DST_UID_SIZE_WITH_MA        (DST_UID_SIZE_NO_MA + DST_MA_SIZE)
#define DST_CHALLENGE_SIZE          5
#define DST_BCC_SIZE                2

#define DST_USER_DATA_2_TO_9_SIZE   5

#define DST_CONFIG_SIZE             2 
#define DST_TIC_SIZE                4
#define DST_RC_SIZE                 2

//--------------------------------------------------------------------------//

enum
{
 DST_PWD_PAGE        =1,
 DST_USER_DATA1_PAGE =2,
 DST_SERIAL_PAGE     =3,
 DST_ISK_PAGE        =4,
 DST_ISSUER_PAGE     =6,
 DST_MUTH_PAGE       =7,

 DST_USER_DATA2_PAGE =8,
 DST_USER_DATA3_PAGE =9,
 DST_USER_DATA4_PAGE =10,
 DST_USER_DATA5_PAGE =11,
 DST_USER_DATA6_PAGE =12,
 DST_USER_DATA7_PAGE =13,
 DST_USER_DATA8_PAGE =14,
 DST_USER_DATA9_PAGE =15,
 DST_TIC_PAGE        =16,
 DST_RC_PAGE         =17,
 DST_PUSH_BUTTON     =18,

 DST_AMB_PAGE        =28,
 DST_COUNTER_PAGE    =29,
 DST_CONFIG_PAGE     =30,

 DST_PAGE_SIZE          ,
};

//--------------------------------------------------------------------------//
typedef struct s_Dst_Log
{ 
  UInt8 sel_addr            ; /* Select Adress */
  UInt8 user_data           ; /* User Data 1 */
  
  UInt8 ma                  ;

  UInt8 uid       [DST_UID_SIZE_NO_MA]  ; /* Unique Identification (3 byte serial number +  1 byte manuf. code) */

  UInt8 challenge [DST_CHALLENGE_SIZE]  ;
  UInt8 response  [DST_SIGNATURE_SIZE]  ;

}Dst_Log ;
//--------------------------------------------------------------------------//
typedef union
{
  struct
  {
     UInt8 ma        ; 
     UInt8 serial[3] ;
  }item ;

  UInt8 byte[4]   ;

}Uid ;
//--------------------------------------------------------------------------//
typedef union
{
  struct
  {
    unsigned int mh           : 1 ;
    unsigned int muth_aut     : 1 ;
    unsigned int fast_auth    : 1 ;
    unsigned int enc_80       : 1 ;
    unsigned int vbat_checker : 4 ;

    unsigned int counter      : 4 ;
    unsigned int ppm_on       : 1 ;
    unsigned int _0_          : 1 ;
    unsigned int bcc_on       : 1 ;
    unsigned int rd_rc_off    : 1 ;
  
  }bit;  

  UInt8  byte[2] ;

}Dst_Func_Def ;
//--------------------------------------------------------------------------//

typedef struct
{

  UInt8 sel_addr                                ; /* Page 1  Select Adress */
  UInt8 usr_data1                               ; /* Page 2  User Data 1 */
  Uid   uid                                     ; /* Page 3  Unique Identification (3 byte serial number +  1 byte manuf. code) */  
  UInt8 isk        [DSTCRYPT_KEY80_SIZE]        ; /* Page 4  Encryption key 40 bits dst40 and 80bit dst 80 */
  UInt8 mak        [DSTCRYPT_KEY80_SIZE]        ; /* Page 7  Mutual authent. key */
  UInt8 usr_data2  [DST_USER_DATA_2_TO_9_SIZE]  ; /* Page 8  User Data 2 */
  UInt8 usr_data3  [DST_USER_DATA_2_TO_9_SIZE]  ; /* Page 9  User Data 3 */
  UInt8 usr_data4  [DST_USER_DATA_2_TO_9_SIZE]  ; /* Page 10 User Data 4 */
  UInt8 usr_data5  [DST_USER_DATA_2_TO_9_SIZE]  ; /* Page 11 User Data 5 */
  UInt8 usr_data6  [DST_USER_DATA_2_TO_9_SIZE]  ; /* Page 12 User Data 6 */
  UInt8 usr_data7  [DST_USER_DATA_2_TO_9_SIZE]  ; /* Page 13 User Data 7 */
  UInt8 usr_data8  [DST_USER_DATA_2_TO_9_SIZE]  ; /* Page 14 User Data 8 */
  UInt8 usr_data9  [DST_USER_DATA_2_TO_9_SIZE]  ; /* Page 15 User Data 9 */
  UInt8 tic        [DST_TIC_SIZE]               ; /* Page 16 Transmission Identification Code*/
  UInt8 rc         [DST_RC_SIZE]                ; /* Page 17 Roll Code*/
  UInt8 push_mask                               ; /* Page 18 Push Button Mask */  
  Dst_Func_Def config                           ; /* Page 30 configuration 1 byte for counter + 1 byte for function */

  UInt8 lock_status[DST_PAGE_SIZE / 8]          ;      
     
}Dst_Memory ;

//--------------------------------------------------------------------------//
typedef union s_Dst_Trans_Resp
{
  struct 
  {
    UInt8   data   [6] ;
    UInt8   rd_addr    ;
  }item ;

  UInt8 buff[7] ;

}Dst_Trans_Resp ;

//--------------------------------------------------------------------------//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

EXTERN_C_ENTER

void              dst_log_init          (void)                                      ;
void              dst_log_update_reset  (void)                                      ;
void              dst_dbg_print_log     (void)                                      ;
void              dst_log_delete        (void)                                      ;
UInt8             dst_log_get_count     (void)                                      ;
Boolean           dst_log_read          (Dst_Log * log)                             ;

void              dst_set_lock          (UInt8 page)                                ;

void              dst_trans             (UInt8 rd_addr, UInt8 * data , UInt8 size)  ;
void              dst_bcc               (UInt16 * crc , UInt8 * data , UInt8 size)  ;

Boolean           dst_log_is_done       (void)                                      ;

Boolean           dst_init              (Dst_Memory * key_mem)                      ;

Dst_Trans_Resp    dst_cmd               (UInt8 cmd  , UInt8 * data , UInt8 size)    ;

Boolean           dst_isk_search        (Dst_Log * log , UInt8 isk[DSTCRYPT_KEY80_SIZE])  ;
Boolean           dst_isk_search_v2     (Dst_Log * log , UInt8 isk[DSTCRYPT_KEY80_SIZE])  ;

EXTERN_C_LEAVE  
                                  
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/


/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/


#endif

