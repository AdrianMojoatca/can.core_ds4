/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hobp_private.h 44458 2016-11-24 18:10:27Z martin.bouchard $
/*==========================================================================*/

#ifndef __HOBP_PRIVATE_H__
#define __HOBP_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os.h"
#include "hobp.h"
#include "nvfs_usr.h"
#include "timeout.h"
#include "q.h"
#include "dbg.h"
#include "config_hobp.h"


/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

// <h>  Task 'Honda_Rx' settings
// <s0> Name
// <o0> Priority           <0-65535>
// <i>  Lowest  priority = 0
// <i>  Highest priority = 65535
// <o1> Stack Size (bytes) <0x0000-0xFFFF:4> // <i>  Must be a multiple of 4.
// </h>
#define TASK_HOBP_RX_NAME         "hobp rx"
#define TASK_HOBP_RX_PRI          60000
#define TASK_HOBP_RX_STK          0x240

#if DBG_HOBP == 1
   #warning("DBG_HOBP_ENABLE") ;
#endif


//--------------------------------------------------------------------------//

#define HOBP_RX_TIMEOUT         10 // 
#define HOBP_RX_BYTE_TIMEOUT    10
                           
#define HOBP_TX_TIMEOUT         40 // 40 ms

#define HOBP_ERR_BUFF_SIZE      5 

#define HOBP_ISK_SIZE           25
#define HOBP_LOG_SIZE           58

//--------------------------------------------------------------------------//

#define     ALGO_HONDA_TYPE_0   0
#define     ALGO_HONDA_TYPE_1   1

#define     NOT_OK              0
#define     OK                  1  
//#define     ERROR                 -1
 

#define     PROCESS_INIT        0xFF
#define     PROCESS_ERROR          0
#define     PROCESS_OK             1
#define     PROCESS_TABLE1_DONE    2
#define     PROCESS_CHALLENGE_DONE 3
#define     PROCESS_TX             4



//--------------------------------------------------------------------------//
typedef enum
{
//  HOBP_PROG_FLAG_PTS       = (1U << 0) ,  // if is pts type
  HOBP_PROG_FLAG_SERIAL    = (1U << 1) ,  // imobiliser serial number
  HOBP_PROG_FLAG_SEQ_1     = (1U << 2) ,  // fisrt sequence
  HOBP_PROG_FLAG_SEQ_2     = (1U << 3) ,  // second sequence
  HOBP_PROG_FLAG_LOG_1     = (1U << 4) ,  // pair of challenge/response part1
  HOBP_PROG_FLAG_LOG_2     = (1U << 5) ,  // pair of challenge/response part2
  HOBP_PROG_FLAG_TABLE1    = (1U << 6) ,  // challenge/response done to build TABLE 1
  HOBP_PROG_FLAG_CHALLENGE = (1U << 7) ,  // challenge/response done to build a log, require only if it's not in the table
  HOBP_PROG_FLAG_POLY      = (1U << 8) ,  // poly found with SEQ1 & SEQ2 
 
  
  //HOBP_PROG_LOG_IS_DONE = (HOBP_PROG_FLAG_SERIAL | HOBP_PROG_FLAG_SEQ_1 | HOBP_PROG_FLAG_SEQ_2 | HOBP_PROG_FLAG_LOG_1 | HOBP_PROG_FLAG_LOG_2 | HOBP_PROG_FLAG_TABLE1 | HOBP_PROG_FLAG_CHALLENGE) ,

}Hobp_Prog_Flags;

//--------------------------------------------------------------------------//

//typedef enum
//{
////   HOBP_INIT_ECM         ,
//   HOBP_IMO_STATUS_PART1 ,
//   HOBP_IMO_STATUS_PART2 ,
//   HOBP_IMO_RESPONSE     ,
//
//}Hobp_State ;
//--------------------------------------------------------------------------//
enum
{
  HOBP_MSG_START_AUTH  ,
  HOBP_MSG_IMO_STATUS2 ,
  HOBP_MSG_ECM         ,
//  HOBP_MSG_TX          ,

  HOBP_MSG_COUNT ,
};
//--------------------------------------------------------------------------//

typedef struct
{
  UInt8 size   ;
  UInt8 data[16];

}Hobp_Msg ;

//--------------------------------------------------------------------------//
enum
{

  HOBP_T5_NO_VER ,
  HOBP_T5_VER_1  ,
  HOBP_T5_VER_2  ,
  HOBP_T5_VER_3  ,
  HOBP_T5_VER_4  ,
  HOBP_T5_VER_5  ,

} ;
//--------------------------------------------------------------------------//

#define HOBP_T5_VERSION HOBP_T5_VER_3
//--------------------------------------------------------------------------//

typedef struct s_HoBp_Entry
{  
  
  UInt8  serial   [2]    ;
  UInt8  status   [2][4] ;
  UInt8  challenge[2][3] ;
  UInt8  response [2][3] ;
 
  UInt8 u8Tab1Col1   [16]  ; 
  UInt8 u8Tab1    [4][16]  ;

  Boolean         is_pts     ;
  Hobp_Prog_Flags prog_flags ;
  
}Hobp_Nvfs_Entry ;

//--------------------------------------------------------------------------//

typedef enum
{
   NO_HOBP_ERR         ,

   HOBP_ERR_SEQ2          ,
   HOBP_ERR_VALIDATION_1  ,
   HOBP_ERR_VALIDATION_2  ,
   HOBP_ERR_PROCESS_TABLE ,
   HOBP_ERR_GET_TABLE1    ,
   HOBP_ERR_GET_CHALLENGE ,

   ERR_HOBP_COUNT 

}Hobp_Error_Code ;

//--------------------------------------------------------------------------//

typedef struct s_Hobp_Err
{
  UInt16         count                  ;

  Hobp_Error_Code code [HOBP_ERR_BUFF_SIZE] ;
  
}Hobp_Error ;

//--------------------------------------------------------------------------//
typedef union
{
  struct 
  {
    UInt8 u8TabResponse[29][2];

  }item;

  UInt8 data[HOBP_LOG_SIZE] ;
}Hobp_Log ;

//--------------------------------------------------------------------------//

typedef struct s_HoBp_Vars
{  
  Boolean log_is_done ;
  Boolean is_prog     ;
  Boolean gwr         ;
  Boolean ign         ;
  Boolean do_err_save ;
  UInt8   imo_skip_counter ;
  UInt8   tx_resp_ptr ;

  Hobp_Log log                 ;
  UInt8    isk[HOBP_ISK_SIZE]  ;

//  Hobp_State state ;
 
  Tsk        tsk_rx          ;

  Q          rx_q_pending    ;

  Timeout    tx_timeout      ;

  Hobp_Nvfs_Entry nvfs_entry ;
  Hobp_Error      err        ;

}Hobp_Vars ;
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void    hobp_dbg_init           (void)       ;

Boolean hobp_init_nvfs          (void)                 ;
Boolean hobp_save               (void)                 ;
                                                      
void    hobp_rx_task            (void   * tsk_arg)     ;

void    hobp_err_add            (Hobp_Error_Code code) ;

void    hobp_ign_on             (void * arg)           ;
void    hobp_ign_off            (void * arg)           ;
void    hobp_gwr_on             (void * arg)           ;
void    hobp_gwr_off            (void * arg)           ;
void    hobp_tx_timeout         (void * arg )          ;


void    hobp_tx                 (Hobp_Msg * msg)       ;

void    hobp_tx_broadcast        (Hobp_Msg * msg , UInt32 timeout) ;

UInt8   hobp_csum               (UInt8 * data  , UInt8 size , UInt8 seed)  ;

int     hobp_rx                 (UInt32 timeout)                ;
Boolean hobp_rx_data            (UInt8 * data , UInt8 size)     ;

void hobp_dbg_print             (StrConst * text ,  const UInt8 data[] , UInt8 size)  ;

void    hobp_set_flag           (const Hobp_Prog_Flags flag) ;
Boolean hobp_get_flag           (const Hobp_Prog_Flags flag) ;


//--------------------------------------------------------------------------//
void setTableSeq   (void);

void  initBypassLib(void);

void  convert_isk_to_table(const UInt8 * status , const UInt8 * isk) ;

UInt8  getChallenge ( UInt8  * u8Challenge    , UInt8 * u8Response, UInt8 (*u8TabResponse)[2]);
UInt8  getTable1    ( UInt8  * u8Challenge    , UInt8 * u8Response, UInt8 * u8Tab1Col1, UInt8 (*u8Tab1)[16]  );

void generateTabs   (UInt8 * u8ISK);
void algo_Honda_V2  (const UInt8 * cChallenge, UInt8 * cResponse) ;

int getPolynome( const UInt8 * u8Challenge, const UInt8 * u8Response, const UInt8 * u8Challenge2, const UInt8 * u8Response2, const UInt8 * iMMOStatus) ;

void serial_process (UInt8 data) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern const NVFS_Entry_Name nvfs_hobp_ver       ;
extern const NVFS_Entry_Name nvfs_hobp           ;
extern const NVFS_Entry_Name nvfs_hobp_log       ;
extern const NVFS_Entry_Name nvfs_hobp_isk       ;
extern const NVFS_Entry_Name nvfs_hobp_error     ;

extern Hobp_Vars hobp_vars ;

extern Hobp_Msg hobp_msg[HOBP_MSG_COUNT] ;
///*==========================================================================*/
///*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
///*==========================================================================*/
//
#endif

