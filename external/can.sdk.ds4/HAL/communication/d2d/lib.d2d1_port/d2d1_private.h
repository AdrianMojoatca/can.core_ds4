/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d_private.h 32240 2015-06-12 12:31:31Z simon.demers $
/*==========================================================================*/

#ifndef __D2D1_PRIVATE_H__
#define __D2D1_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2d1.h"
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

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/


void d2d1_irq_service (UInt8 which_port, const volatile UInt32 *piir_value);

void    d2d1_reset(void) ;
void    d2d1_init_reset (void);

void d2d1_uart_rx_irq(D2d1_Port *port, UInt8 c);
void d2d1_uart_tx_irq(D2d1_Port *port);
void d2d1_msg_timeout_handler(D2d1_Port *port);
void d2d1_wake (Wake_Status wake_status);
Boolean d2d1_wake_ready(void);
void  d2d1_irq0_service(void);
void  d2d1_irq1_service(void);
void  d2d1_irq3_service(void);
Boolean d2d1_send_filter(D2d1_Port *port,Smsg msg);
                  
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern volatile SInt16 d2d_rda_rx;

extern volatile D2D_First_Byte d2d_first_byte;

extern Wake_Status d2d_wake_status;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

