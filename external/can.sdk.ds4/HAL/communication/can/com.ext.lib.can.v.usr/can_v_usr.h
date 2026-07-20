/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_v_usr.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __CAN_V_USR_H__
#define __CAN_V_USR_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can.h"
#include "can_v.h"
#include "can_lut_sw.h"
#include "exec.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

// <h>  Task 'can v usr' settings
// <h>  Task 'can virtual user rx' settings
// <s0> Name
// <o0> Priority           <0-65535>
// <i>  Lowest  priority = 0
// <i>  Highest priority = 65535
// <o1> Stack Size (bytes) <0x0000-0xFFFF:4>
// <i>  Must be a multiple of 4.
// </h>
#define TASK_CAN_V_USR_RX_NAME  "can v usr rx"
#define TASK_CAN_V_USR_RX_PRI   10002
#define TASK_CAN_V_USR_RX_STK   0x100

#define TASK_CAN_USR_TX0_NAME  "can v usr tx 0"
#define TASK_CAN_USR_TX0_PRI   10004
#define TASK_CAN_USR_TX0_STK   0x100

//--------------------------------------------------------------------------//

typedef struct s_Can_Rx_Q_Msg
{
    Exec_Q_Msg exec_base ;  // MUST BE FIRST ELEMENT OF THE STRUCT
    Can_Msg    can_msg ;

} Can_Rx_Q_Msg ;

//--------------------------------------------------------------------------// 

typedef struct s_Can_V_Usr
{
    Can_V_Rx           can_v_rx ;
    Can_Lut_Sw         can_lut_sw ;
    Can_Lut_Sw_Entry * can_lut_sw_table ;   

} Can_V_Usr ;

//--------------------------------------------------------------------------// 

typedef struct
{
  Can_Dev_Id can_dev_id           ;
  
  const Can_Bit_Def * can_bit_def ;
  
  UInt8       rx_size             ;
  UInt8       tx_size             ;

} Can_V_Init_Options ;

// bit 31 is the bit that determine if we are passing to the wake mode or not
// just use the macro replacing the x with the desired delay and the bit is set automatically.
#define SW_WAKE_BIT              (1u << 31)
#define DELAY_WITH_SW_WAKE(x)    (x | SW_WAKE_BIT)

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void    can_v_usr_init       (void) ;
Boolean can_v_usr_execute    (void) ;
void    can_v_usr_rx_task    (Can_V_Usr * arg) ;
Boolean can_v_usr_tx         (Can_Dev_Id can_dev_id, Can_Msg * can_msg, UInt32 delay) ;

void    can_v_usr_init_opt   (Can_V_Init_Options * options, Boolean single_wire) ;
void    can_v_usr_init_reset (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


