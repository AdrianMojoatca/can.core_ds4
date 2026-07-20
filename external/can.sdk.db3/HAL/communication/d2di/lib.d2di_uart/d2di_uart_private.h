/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

#ifndef __d2di_uart_private_h__
#define __d2di_uart_private_h__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2di_uart.h"
#include "gpio.h"
#include "wake.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define IER_RBR     0x01
#define IER_THRE    0x02
#define IER_RLS     0x04

#define IIR_PEND    0x01
#define IIR_RLS     0x03
#define IIR_RDA     0x02
#define IIR_CTI     0x06
#define IIR_THRE    0x01

#define LSR_RDR     0x01
#define LSR_OE      0x02
#define LSR_PE      0x04
#define LSR_FE      0x08
#define LSR_BI      0x10
#define LSR_THRE    0x20
#define LSR_TEMT    0x40
#define LSR_RXFE    0x80

#define D2DI_UART_D2D_TYPE_IDATA  1
#define D2DI_UART_ERR_BUFF_SIZE   10

// needs to be power of 2 and less than 256 (because we are using UInt8)
#define D2DI_UART_Q_MAX_SIZE 64

/*
 * Interrupt safe queue implementation for UART rx and tx.
 * Uses AND operation to wrap the end of buffer. This is equivalent of using
 * modulo operator, but much faster.
 * Limitations: 
 *   1. D2DI_UART_Q_MAX_SIZE needs to be power of 2 for wrapping to work.
 *   2. Actual size is of queue is (D2DI_UART_Q_MAX_SIZE - 1). This is done to avoid
 * using a count variable which would force us to protect it from concurrent access.
 */
typedef struct s_D2Di_Uart_Q
{
    volatile UInt8 head;
    volatile UInt8 tail;
    UInt8 data[D2DI_UART_Q_MAX_SIZE];
    
} D2Di_Uart_Q;

//--------------------------------------------------------------------------//

typedef enum
{
    // bus errors
    D2DI_UART_ERR_OVERRUN = 1  , // UART overrun
    D2DI_UART_ERR_PARITY       , // UART parity
    D2DI_UART_ERR_FRAMING      , // UART framing
    D2DI_UART_ERR_BREAK        , // UART sync break
    D2DI_UART_ERR_RXFE         , // UART rx fifo error
    D2DI_UART_ERR_TIMEOUT      , // UART character timeout error
    D2DI_UART_ERR_RX_Q_FULL    , // UART rx queue full
    D2DI_UART_ERR_TX_Q_FULL    , // UART tx queue full

    D2DI_UART_ERR_COUNT 

} D2Di_Uart_Error_Code ;

//--------------------------------------------------------------------------//
typedef enum
{
    D2DI_UART_NO_VER,
    D2DI_UART_VER_1
    
} D2Di_Uart_Nvfs_Version ;

#define D2DI_UART_VERSION  D2DI_UART_VER_1

//--------------------------------------------------------------------------//
typedef enum
{
    D2DI_UART_72MHZ,
    D2DI_UART_4MHZ
    
} D2Di_Uart_Clk_Speed;
    
//--------------------------------------------------------------------------//
typedef struct s_D2Di_Uart_Err
{
    UInt16 count;
    D2Di_Uart_Error_Code code[D2DI_UART_ERR_BUFF_SIZE] ;
} D2Di_Uart_Error ;

//--------------------------------------------------------------------------//
typedef struct s_D2Di_Uart_Vars
{ 
    volatile Boolean d2di_wake_ready_got_byte;
    
    // nvfs
    Boolean d2di_uart_do_err_save ;
    D2Di_Uart_Error err;
    D2Di_Uart_Nvfs_Version version;
    
    // rx and tx queues
    D2Di_Uart_Q *d2di_uart_rx_q;
    D2Di_Uart_Q *d2di_uart_tx_q;
    
} D2Di_Uart_Vars;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void d2di_uart_irq_service (void);
void d2di_wake             (Wake_Status wake_status);
Boolean d2di_wake_ready    (void);
void d2di_fiq_service      (void);

D2Di_Uart_Q_Err d2di_uart_tx_q_remove (UInt8 *val);
D2Di_Uart_Q_Err d2di_uart_tx_q_insert (UInt8 val);
D2Di_Uart_Q_Err d2di_uart_rx_q_insert (UInt8 val);

void d2di_uart_err_add     (D2Di_Uart_Error_Code code);
void d2di_uart_cfg_baudrate(UInt32 chan_num, D2Di_Uart_Clk_Speed clk_speed);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern D2Di_Uart_Vars d2di_uart_vars;

#endif
