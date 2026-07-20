/** 
 * @file   d2d2_private.h
 * @brief  
 * author  DIRECTED ELECTRONICS
 * @date   7/20/2015
 */

#ifndef __d2d2_private_h__
#define __d2d2_private_h__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2d2.h"

#include "wake.h"
#include "tmr.h"
#include "dbg.h"
#include <string.h>

#include "d2d_switch.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/


//--------------------------------------------------------------------------//
#define CRC_CCITT_PRELOAD 0xFFFF
#define D2D2_TXLEN_MAX    16


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
Boolean d2d2_init_nvfs(UInt8 port);

void d2d2_irq_service (UInt8 which_port, const volatile UInt32 *iir_value);

void d2d2_wake (Wake_Status wake_status);
void d2d2_reset(void) ;

void to_open(D2d2_Port *port);
void to_master(D2d2_Port *port);
void to_data(D2d2_Port *port);
void rxbyte(D2d2_Port *port, UInt8 c);
void d2d2_msg_timeout_handler(D2d2_Port *port);
void d2d2_uart_rx_irq(D2d2_Port *port, UInt8 c);
void d2d2_uart_tx_irq(D2d2_Port *port);
void  d2d2_irq0_service(void);
void  d2d2_irq1_service(void);
void  d2d2_irq3_service(void);

void d2d2_wake (Wake_Status wake_status);
void check_dbg(void);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern volatile unsigned char d2d2_rda_rx;

extern UInt8 dbg_never;
extern UInt8 *pdbg_never;
extern UInt16 dbg16_never;

#endif
