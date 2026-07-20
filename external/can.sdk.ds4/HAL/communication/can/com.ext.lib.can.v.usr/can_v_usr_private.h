/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_v_usr_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __CAN_V_USR_PRIVATE_H__
#define __CAN_V_USR_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os.h"
#include "can_v_usr.h"
#include "wake.h"
#include "irq_user.h"
#include "q.h"
#include "nvfs_usr.h"
#include "dbg.h"

#if DBG_CAN == 1
  #warning "DBG_CAN Is Enable"
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

// <h>  Task 'can user tx 1' settings
// <s0> Name
// <o0> Priority           <0-65535>
// <i>  Lowest  priority = 0
// <i>  Highest priority = 65535
// <o1> Stack Size (bytes) <0x0000-0xFFFF:4>
// <i>  Must be a multiple of 4.
// </h>
#define TASK_CAN_USR_TX1_NAME  "can v usr tx 1"
#define TASK_CAN_USR_TX1_PRI   10003
#define TASK_CAN_USR_TX1_STK   0x100

//--------------------------------------------------------------------------//

#define CANV_TX_TIMEOUT        200

//--------------------------------------------------------------------------//

typedef struct s_Can_Tx_Q_Msg
{
    Q_Msg   q_base  ;   // MUST BE FIRST ELEMENT OF THE STRUCT
    Can_Msg can_msg ;
    UInt32  delay   ;

} Can_Tx_Q_Msg ;

typedef struct s_Can_Tx_Ctrl
{
    Can_Tx can_tx           ;
    Q *    can_tx_q_pending ;

} Can_Tx_Ctrl ;

//--------------------------------------------------------------------------//

#define CAN_V_USR_ERR_BUFF_SIZE 10

typedef enum
{
    CAN_V_USR_NO_ERR              ,
    CAN_V_USR_ERR_RX_Q_NO_FREE    ,
    CAN_V_USR_ERR_TX_Q_NO_FREE    ,
    CAN_V_USR_ERR_TX_TIMEOUT      ,
    CAN_V_USR_ERR_RX_OVERRUN      ,
    CAN_V_USR_ERR_RX_ERRATA       ,
    CAN_V_USR_ERR_BUS_OFF         ,
        
    CAN_V_USR_ERR_COUNT           ,

    CAN_V_USR_DEV1 = 0x80         ,   // when bit 7 is set, error occured on device 1

} Can_V_Usr_Error_Code ;

#define CAN_V_USR_DEV1_BIT        7
//--------------------------------------------------------------------------//

typedef struct s_Can_V_Usr_Err
{
    UInt8 count[CAN_V_USR_ERR_COUNT-1]   ;
    
    struct
    {
      UInt8                ptr ;
      Can_V_Usr_Error_Code code [CAN_V_USR_ERR_BUFF_SIZE] ;

    } log ;
          
} Can_V_Usr_Error ;

//--------------------------------------------------------------------------//

typedef enum 
{
    SWCAN_NORMAL_MODE,
    SWCAN_WAKE_UP_MODE,

} SWCAN_Mode ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void       can_v_usr_wake         (Wake_Status   wake_status) ;
void       can_v_usr_tx_task      (Can_Tx_Ctrl * can_tx_ctrl) ;
Boolean    can_v_usr_tx_one_shot  (Can_Tx can_dev_tx, Can_Msg * can_msg) ; 
void       can_v_usr_fiq_service  (void) ;
void       can_v_usr_user_service (void) ; 
Boolean    can_v_usr_init_nvfs    (void) ;
Boolean    can_v_usr_err_save     (void) ;
void       can_v_usr_err_add      (Can_V_Usr_Error_Code code, Can_Dev_Id dev_id) ;
void       can_v_usr_sw_mode      (SWCAN_Mode mode) ;
SWCAN_Mode can_v_usr_check_sw_wake_bit (UInt32 value) ;
void       can_v_usr_irq_service  (Can_Dev_Id dev_id, Can_Irq can_irq) ;
void       can_v_usr_reset        (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Tsk          tsk_can_tx          []  ;

extern Can_Dev      can_dev             []  ;
extern Can_Tx       can_dev_tx          []  ;

extern Can_Pin_Map  pin_map             []  ;

extern Q            can_rx_q_free           ;

extern Q            can_tx_q_free       []  ;
extern Q            can_tx_q_pending    []  ;

extern Boolean      can_wake_swi            ;
extern Boolean      can_do_err_save         ;

extern Wake_Status can_v_usr_wake_status ;

extern Res can_v_usr_tx_res[CAN_DEV_COUNT] ;

extern Can_V_Usr_Error can_v_usr_err ;

extern const NVFS_Entry_Name nvfs_can_v_usr_err ;


/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


