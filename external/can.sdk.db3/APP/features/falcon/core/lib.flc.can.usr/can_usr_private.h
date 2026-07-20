/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_usr_private.h 22459 2013-09-06 10:42:02Z florin.olariu $
/*==========================================================================*/

#ifndef __CAN_USR_PRIVATE_H__
#define __CAN_USR_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "can_usr.h"
#include "pipe.h"
#include "wake.h"
#include "trace.h"
#include "per_exec.h"
#include "timeout.h"
#include "fiq.h"
#include "nvram.h"
#include "nvfs_log.h"
#include "core_contract_db3_can_config.h"
#include "gpio.h"
#include "db3_gpio.h"
#include "trace.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/


//--------------------------------------------------------------------------//

#define CAN_USR_TX_ONE_SHOT_TIMEOUT         (100)

#ifndef DEBUG_CAN_USR
#define DEBUG_CAN_USR			1
#endif

#if DEBUG_CAN_USR == 1
#define TRACE_CAN_USR			TRACE
#else
#define TRACE_CAN_USR(...)		;
#endif

//--------------------------------------------------------------------------//

//----------------------------------------------------------------------------//

//CAN1 BUS SPEED SETUP

#if CAN_DEV_1_SPEED == 0         //125K
    #define CAN_DEV_1_BTR	    31 +1
    #define CAN_DEV_1_SJW		1  +1
    #define CAN_DEV_1_TSEG1		10 +1
    #define CAN_DEV_1_TSEG2		5  +1
    #define CAN_DEV_1_SAM		1
#endif

#if CAN_DEV_1_SPEED == 1         //250K
    #define CAN_DEV_1_BTR        15 +1
    #define CAN_DEV_1_SJW        1  +1
    #define CAN_DEV_1_TSEG1      10 +1
    #define CAN_DEV_1_TSEG2      5  +1
    #define CAN_DEV_1_SAM        1
#endif

#if CAN_DEV_1_SPEED == 2         //500K
    #define CAN_DEV_1_BTR		7  +1
    #define CAN_DEV_1_SJW		2  +1
    #define CAN_DEV_1_TSEG1		10 +1
    #define CAN_DEV_1_TSEG2		5  +1
    #define CAN_DEV_1_SAM		0
#endif

//----------------------------------------------------------------------------//
//<h> TASK CONFIGURATION
/*------------------------------------------------------------------------*/
// <h> [01]. Task 'can_dev0_rx_tsk' settings
/*------------------------------------------------------------------------*/
// <s0> Name
// <o0> Priority           <0-65535>
// <i>  Lowest  priority = 0
// <i>  Highest priority = 65535
// <o1> Stack Size (bytes) <0x0000-0xFFFF:4>
// <i>  Must be a multiple of 4.
// <i>  size of parking patterns pipe
// </h>
#define CAN_DEV_0_RX_TSK_NAME  		  "can0_rx_tsk"
#define CAN_DEV_0_RX_TSK_PRI           5002
#define CAN_DEV_0_RX_TSK_STK           0x0170
//------------------------------------------------------------------------//
// <e0> [02]. Task 'can_dev0_rx_buffer_tsk' settings
/*------------------------------------------------------------------------*/
// <o1> Priority           <0-65535>
// <i>  Lowest  priority = 0
// <i>  Highest priority = 65535
// <o2> Stack Size (bytes) <0x0000-0xFFFF:4>
// <i>  Must be a multiple of 4.
// <i>  size of parking patterns pipe
// </e>
#ifndef CAN_DEV_0_RX_USE_BUFFER
#define CAN_DEV_0_RX_USE_BUFFER          1
#endif
#define CAN_DEV_0_RX_BUFFER_TSK_PRI      5001
#define CAN_DEV_0_RX_BUFFER_TSK_STK      0x0170
//------------------------------------------------------------------------//
// <h>  [03]. Task 'can_dev0_tx_tsk' settings
/*------------------------------------------------------------------------*/
// <s0> Name
// <o0> Priority           <0-65535>
// <i>  Lowest  priority = 0
// <i>  Highest priority = 65535
// <o1> Stack Size (bytes) <0x0000-0xFFFF:4>
// <i>  Must be a multiple of 4.
// <i>  size of parking patterns pipe
// </h>
#define CAN_DEV_0_TX_TSK_NAME	  "can0_tx_tsk"
#define CAN_DEV_0_TX_TSK_PRI      5000
#define CAN_DEV_0_TX_TSK_STK      0x0150
//------------------------------------------------------------------------//
// <h>  [04]. Task 'can_dev1_rx_tsk' settings
/*------------------------------------------------------------------------*/
// <s0> Name
// <o0> Priority           <0-65535>
// <i>  Lowest  priority = 0
// <i>  Highest priority = 65535
// <o1> Stack Size (bytes) <0x0000-0xFFFF:4>
// <i>  Must be a multiple of 4.
// <i>  size of parking patterns pipe
// </h>
#define CAN_DEV_1_RX_TSK_NAME  		  "can1_rx_tsk"
#define CAN_DEV_1_RX_TSK_PRI           5004
#define CAN_DEV_1_RX_TSK_STK           0x01C0
//------------------------------------------------------------------------//
// <e0> [05]. Task 'can_dev1_rx_buffer_tsk' settings
/*------------------------------------------------------------------------*/
// <o1> Priority           <0-65535>
// <i>  Lowest  priority = 0
// <i>  Highest priority = 65535
// <o2> Stack Size (bytes) <0x0000-0xFFFF:4>
// <i>  Must be a multiple of 4.
// <i>  size of parking patterns pipe
// </e>
#ifndef CAN_DEV_1_RX_USE_BUFFER
#define CAN_DEV_1_RX_USE_BUFFER          1
#endif

#define CAN_DEV_1_RX_BUFFER_TSK_PRI      5003
#define CAN_DEV_1_RX_BUFFER_TSK_STK      0x01C0
//------------------------------------------------------------------------//
// <h>  [06]. Task 'can_dev1_tx_tsk' settings
/*------------------------------------------------------------------------*/
// <s0> Name
// <o0> Priority           <0-65535>
// <i>  Lowest  priority = 0
// <i>  Highest priority = 65535
// <o1> Stack Size (bytes) <0x0000-0xFFFF:4>
// <i>  Must be a multiple of 4.
// <i>  size of parking patterns pipe
// </h>
#define CAN_DEV_1_TX_TSK_NAME	  "can1_tx_tsk"
#define CAN_DEV_1_TX_TSK_PRI      5001
#define CAN_DEV_1_TX_TSK_STK      0x0200
//------------------------------------------------------------------------//

#ifndef CAN_USR_FILTER_TABLE_SIZE
#define CAN_USR_FILTER_TABLE_SIZE     30
#endif

typedef struct s_Can_Usr_Filter
{
    UInt32 table[CAN_USR_FILTER_TABLE_SIZE];

    UInt8 size;

    Boolean enable;
}Can_Usr_Filter;


typedef struct s_Can_Usr
{
    Can_Dev        can_dev        ;
    
    Can_Usr_Filter filter         ;

	
	Res       tx_res              ;
    Can_Tx    tx_can_slow         ;
    Can_Tx    tx_can_fast         ;    
    Pipe      tx_pipe             ;
    Timeout   tx_timeout          ;


    Can_Rx    rx_can              ;    
    Pipe      rx_pipe             ;
    Reg_Table rx_reg_fast         ;
    Reg_Table rx_reg_slow         ;
    Boolean   rx_use_buffer       ; //<-- foward the messages received by RX TASK to RX HANDLER TASK
	Boolean   tx_use_buffer       ;

    Boolean   bus_off_ind         ;
	UInt16    err_cnt_slow        ;// <-- used to count the number of transmisson erros
								   //     if the number is larger then 10 reset the CAN controller
    
    UInt16    err_cnt_fast        ;// <-- used to count the number of transmisson erros
								   //     if the number is larger then 10 reset the CAN controller        

}Can_Usr ;  

//--------------------------------------------------------------------------//

#define CAN_USR_ERR_BUFF_SIZE 10

typedef enum
{
	CAN_USR_ERR_DEV0_RX_PIPE_FULL = 1 ,
    CAN_USR_ERR_DEV0_TX_PIPE_FULL     ,
    CAN_USR_ERR_DEV0_TX_SLOW_TIMEOUT  ,
	CAN_USR_ERR_DEV0_TX_SLOW_ERROR    ,
    CAN_USR_ERR_DEV0_TX_SYNC_TIMEOUT  ,
    CAN_USR_ERR_DEV0_BUS_OFF          ,
    CAN_USR_ERR_DEV0_OVERRUN          ,
    CAN_USR_ERR_DEV0_ERRATA           ,
    CAN_USR_ERR_DEV0_TX_RETRY_TIMEOUT ,
	CAN_USR_ERR_DEV0_CAN_RESET        ,	

    CAN_USR_ERR_DEV1_RX_PIPE_FULL     ,
    CAN_USR_ERR_DEV1_TX_PIPE_FULL     ,
    CAN_USR_ERR_DEV1_TX_SLOW_TIMEOUT  ,
	CAN_USR_ERR_DEV1_TX_SLOW_ERROR    ,    
    CAN_USR_ERR_DEV1_TX_SYNC_TIMEOUT  ,
    CAN_USR_ERR_DEV1_BUS_OFF          ,
    CAN_USR_ERR_DEV1_OVERRUN          ,
    CAN_USR_ERR_DEV1_ERRATA           ,
    CAN_USR_ERR_DEV1_TX_RETRY_TIMEOUT ,
	CAN_USR_ERR_DEV1_CAN_RESET        ,	

	CAN_USR_ERR_COUNT 

} Can_Usr_Error_Code ;

//--------------------------------------------------------------------------//

#define 	CAN_USR_SLOW_ERROR_MAX		5
#define     CAN_USR_FAST_ERROR_MAX		5

//--------------------------------------------------------------------------//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/


void    	can_usr_rx_dev0_init    ( void              );
void    	can_usr_rx_dev1_init    ( void              );
void    	can_usr_rx_tsk          ( void *     dev_id );
void    	can_usr_rx_handler_tsk  ( void *     dev_id );

void    	can_usr_tx_dev0_init    ( void              );
void    	can_usr_tx_dev1_init    ( void              );
void    	can_usr_tx_tsk          ( void *     dev_id );

void    	can_usr_bus_off_init    ( Can_Dev_Id dev_id );

Boolean 	can_usr_err_init        ( void              );
void    	can_usr_err_add         ( Can_Usr_Error_Code code );

void    	can_usr_wake            ( Wake_Status status );

Boolean    	can_usr_tx_normal       ( Can_Dev_Id dev_id , Can_Msg *can_msg , TskTimeout timeout );
Boolean     can_usr_tx_one_shot     ( Can_Dev_Id dev_id , Can_Msg *can_msg , TskTimeout timeout );    

void 		can_usr_one_shot_timeout_init ( Can_Dev_Id dev_id );
void 		can_usr_one_shot_timeout_stop ( Can_Dev_Id dev_id );
Boolean 	can_usr_one_shot_timeout_test ( Can_Dev_Id dev_id );
void 		can_usr_one_shot_timeout_start( Can_Dev_Id dev_id  , TskTimeout timeout );

void 		can_usr_controller_reset	  ( Can_Dev_Id dev_id );

Boolean     can_usr_rx_filter_check_id    (Can_Dev_Id dev_id , UInt32 id);
Boolean     can_usr_dev0_speed_to_timing  (UInt32 speed, UInt32 *btr, UInt32 *tseg1, UInt32 *tseg2, UInt32 *sjw, UInt32 *sam);

#define CAN_USR_DEV0_RUNTIME_ENABLED() ((CAN_CFG_CAN_DEV_0_RX_BUFFER_SIZE > 0u) || (CAN_CFG_CAN_DEV_0_TX_BUFFER_SIZE > 0u))
#define CAN_USR_DEV0_IS_FT()           (CAN_CFG_CAN_DEV_0_FT_SW != 0u)


/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern       Can_Usr      can_usr[CAN_DEV_COUNT];
extern       Boolean      can_use[CAN_DEV_COUNT];

extern       Nvfs_Log     can_usr_err           ;
extern       const UInt32 can_dev_0_filter_table[] ; 
extern       const UInt32 can_dev_1_filter_table[] ; 
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


