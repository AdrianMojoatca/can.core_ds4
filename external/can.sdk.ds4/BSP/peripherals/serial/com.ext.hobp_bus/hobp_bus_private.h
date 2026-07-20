/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hobp_bus_private.h 34443 2015-10-19 14:58:23Z martin.bouchard $
/*==========================================================================*/

#ifndef __HOBP_BUS_PRIVATE_H__
#define __HOBP_BUS_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os.h"
#include "hobp_bus.h"
#include "tmr.h"
#include "nvfs.h"
#include "timeout_f.h"
#include "cmd.h"
#include "fiq.h"
#include "config_gpio.h"
#include "dbg.h"
#include "wake.h"
#include "config_hobp_bus.h"

#if DBG_HOBP_BUS == 1
   #warning("DBG_HOBP_BUS_ENABLE") ;
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define TASK_HOBP_BUS_NAME       "hobpbus"
#define TASK_HOBP_BUS_PRI        64000
#define TASK_HOBP_BUS_STK        0x0120


//--------------------------------------------------------------------------//

//#define HOBP_BUS_SPEED           10400 
#define HOBP_BUS_BAUDRATE        (CPU_CLOCK_HZ/ HOBP_BUS_SPEED)  
#define HOBP_BUS_TX_SPEED        (HOBP_BUS_BAUDRATE)  //+ (1 * CPU_CLOCK_MULT)

#define HOBP_BUS_BYTE_DELAY      ((35 /1000000)  * CPU_CLOCK_HZ) //35 us

#define HOBP_BUS_TX_TIMEOUT_2MS  2
//--------------------------------------------------------------------------//

#define HOBP_BUS_ERR_BUFF_SIZE   5


//--------------------------------------------------------------------------//
typedef enum
{
  HOBP_BUS_RX_DATA       = (1U << 1) ,
  HOBP_BUS_TX_BYTE_DONE  = (1U << 2) ,

} Hobp_Bus_Flags ;
//--------------------------------------------------------------------------//

typedef enum
{
  HOBP_BUS_NO_ERR        = 0 ,
  HOBP_BUS_ERR_FRAMING       ,
  HOBP_BUS_ERR_RX            ,
  HOBP_BUS_ERR_RX_TIMEOUT    ,
  HOBP_BUS_ERR_TX_TIMEOUT    ,
  HOBP_BUS_ERR_RX_PIP_FULL   , // 5
  HOBP_BUS_ERR_TX_PIP_FULL   ,
  HOBP_BUS_ERR_TX_NOT_EMPTY  ,
  HOBP_BUS_ERR_TX_BUS_DIS    ,

  HOBP_BUS_ERR_TX_RUNNING    ,
  HOBP_BUS_ERR_RX_RUNNING    ,
  HOBP_BUS_ERR_RX_NOT_EMPTY  ,


} Hobp_Bus_Error_Code ;

//--------------------------------------------------------------------------//

typedef struct s_Hobp_Bus_Err
{
  UInt16                 count                         ; 
  Hobp_Bus_Error_Code code [HOBP_BUS_ERR_BUFF_SIZE] ;
  
} Hobp_Bus_Error ;

//--------------------------------------------------------------------------//

typedef struct s_Hobp_Bus_Entry
{
    Hobp_Bus_Func func    ;  
               
    UInt8 q_size      ;
    Q     q_free      ;
    Q     q_pending   ;
    
    struct s_Hobp_Bus_Entry *next; 

}Hobp_Bus_Entry ;

//--------------------------------------------------------------------------//

typedef struct s_Hobp_Bus_Tx_Q_Msg
{ 
  Q_Msg      q_base    ;
    
  Hobp_Bus_Msg   msg   ;
  UInt32         delay ;

}Hobp_Bus_Tx_Q_Msg ;

//--------------------------------------------------------------------------//
typedef struct
{                                    
  volatile  Hobp_Bus_Flags flag      ;

  volatile  int     fiq_rx_data     ;
  volatile  int     fiq_tx_data     ;

  Boolean       do_save             ;
  Boolean       is_connected        ;

  Wake_Status   wake_status         ;

  UInt32        baudrate            ;
  
  Hobp_Bus_Entry * list             ;

  Hobp_Bus_Error err ; // error logging buffer

  Tmr tmr_rx        ;  
  Tmr tmr_tx        ;  

  Tsk tsk           ;  

  Q rx_q_free       ;
  Q rx_q_pending    ;

  Q tx_q_free       ;
  Q tx_q_pending    ;

  Hobp_Bus_Msg fiq_tx_msg ;


}Hobp_Bus_Vars ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

EXTERN_C_ENTER

void    hobp_bus_fiq_service     (void) ;
void    hobp_bus_user_service    (void) ;

void    hobp_bus_dbg_init        (void) ;

void    hobp_bus_rx_clr          (void) ;
void    hobp_bus_rx_edge_dis     (void) ;
void    hobp_bus_rx_edge_ena     (void) ;

void    hobp_bus_task            (void) ;

void    hobp_bus_err_add         (Hobp_Bus_Error_Code code)   ;
void    hobp_bus_set_flag        (Hobp_Bus_Flags flag)        ;
void    hobp_bus_wake            (Wake_Status status)        ;

void    hobp_bus_dbg_print       (StrConst * text ,  const UInt8 data[] , UInt8 size) ;

int     hobp_bus_rx              (UInt32 timeout)            ;

Boolean hobp_bus_init_nvfs       (void)                      ;



EXTERN_C_LEAVE  

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern const NVFS_Entry_Name nvfs_hobp_bus_error ;

extern       Hobp_Bus_Vars    hobp_bus_vars       ;

#endif

