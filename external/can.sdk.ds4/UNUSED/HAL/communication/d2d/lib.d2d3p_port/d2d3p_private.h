/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d_private.h 32240 2015-06-12 12:31:31Z simon.demers $
/*==========================================================================*/

#ifndef __D2D3_PRIVATE_H__
#define __D2D3_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2d3p.h"
#include "d2d_switch.h"

#include "wake.h"
#include "tmr.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#if 0 // we don't need this
//D2D1 timer delta, 1ms base
#define D2D1_TIMER_DELTA_1MS    1
#define D2D1_TIMER_DELTA_2MS    2
#define D2D1_TIMER_DELTA_4MS    4
#define D2D1_TIMER_DELTA_10MS   10
#define D2D1_TIMER_DELTA_20MS   20
#define D2D1_TIMER_DELTA_20SEC  20000
#endif

typedef struct s_D2D_First_Byte
{
  UInt8   byte        ;
  Boolean validity    ;
  Boolean d2d_rx_swi  ;

} D2D_First_Byte ; 

typedef struct s_Switch_Notify_Msg
{
    Q_Msg base;
    Smsg  verify_msg;
} Switch_Notify_Msg;

// <h>  Task 'd2d3p' settings
// <s0> Name
// <o0> Priority           <0-65535>
// <i>  Lowest  priority = 0
// <i>  Highest priority = 65535
// <o1> Stack Size (bytes) <0x0000-0xFFFF:4>
// <i>  Must be a multiple of 4.
// </h>
#define TASK_D2D3P_NAME  "d2d3p"
#define TASK_D2D3P_PRI   10002
#define TASK_D2D3P_STK   0x0200

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/


void d2d3p_irq_service (UInt8 which_port, const volatile UInt32 *piir_value);

void    d2d3p_reset(void) ;
void    d2d3p_init_reset (void);

void d2d3p_uart_rx_irq(D2d3p_Port *port, UInt8 c);
void d2d3p_uart_tx_irq(D2d3p_Port *port);
void d2d3p_msg_timeout_handler(D2d3p_Port *port);
void d2d3p_wake (Wake_Status wake_status);
Boolean d2d3p_wake_ready(void);
void  d2d3p_irq0_service(void);
void  d2d3p_irq1_service(void);
void  d2d3p_irq3_service(void);
Boolean d2d3p_send_filter(D2d3p_Port *port,Smsg msg);
                  
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern volatile SInt16 d2d3p_rda_rx;

extern volatile D2D_First_Byte d2d3p_first_byte;

extern Wake_Status d2d3p_wake_status;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

