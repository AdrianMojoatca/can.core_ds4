/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

#ifndef __D2DU_UART_H__
#define __D2DU_UART_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "os.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
// enum of possible error codes
typedef enum e_D2Di_Uart_Q_Err
{
    D2DI_UART_Q_OK = 0,
    D2DI_UART_Q_FULL,
    D2DI_UART_Q_EMPTY
    
} D2Di_Uart_Q_Err;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
Boolean d2di_uart_init                (void);
void d2di_uart_tx_q_insert_frame      (const UInt8 *d2di_frame, UInt8 frame_size);
D2Di_Uart_Q_Err d2di_uart_rx_q_remove (UInt8 *val);
Boolean d2di_uart_rx_q_is_empty       (void);
Boolean d2di_uart_tx_q_is_empty       (void);
Boolean d2di_uart_err_save            (void);
Boolean d2di_uart_is_err_save_set     (void);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
// os_irq used to signal task that data has arrived
extern Irq d2di_uart_rx_irq;

// used during wake from sleep
extern volatile UInt8 d2di_wake_ready_bytes;

#endif
