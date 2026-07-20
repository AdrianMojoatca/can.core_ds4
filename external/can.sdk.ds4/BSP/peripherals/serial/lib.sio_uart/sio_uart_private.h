/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sio_uart_private.h 32991 2015-07-28 15:42:19Z martin.bouchard $
/*==========================================================================*/

#ifndef __SIO_UART_PRIVATE_H__
#define __SIO_UART_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "sio_uart.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

enum e_Uart_Reg_Offsets
{
  #define OFFSET(z) z >> 2

  URBR = OFFSET (0x00) , // RX buffer reg
  UTHR = OFFSET (0x00) , // TX holding reg
  UDLL = OFFSET (0x00) , // Divisor latch LSB
  UDLM = OFFSET (0x04) , // Divisor latch MSB
  UIER = OFFSET (0x04) , // IRQ Enable reg
  UIIR = OFFSET (0x08) , // IRQ Identification reg
  UFCR = OFFSET (0x08) , // FIFO Control reg
  ULCR = OFFSET (0x0C) , // Line Control reg
  ULSR = OFFSET (0x14) , // Line Status  reg
  USCR = OFFSET (0x1C) , // Scratch pad reg
  UACR = OFFSET (0x20) , // Auto-Baud control reg
  UICR = OFFSET (0x24) , // IrDA Control reg (UART-3 Only)
  UFDR = OFFSET (0x28) , // Fractional divider reg
  UTER = OFFSET (0x30) , // Transmit Enable reg

  #undef  OFFSET
} ;

//--------------------------------------------------------------------------//

#define cast_sio_rx_data_to_mbx_msg(sio_rx_data) ((MbxMsg      *)((UInt8 *)(sio_rx_data) + struct_offs(Sio_Rx_Data * , _base_)))
#define cast_mbx_msg_to_sio_rx_data(mbx_msg)     ((Sio_Rx_Data *)((UInt8 *)(mbx_msg    ) - struct_offs(Sio_Rx_Data * , _base_)))

//--------------------------------------------------------------------------//

typedef struct
{
  MbxMsg _base_ ;

  SInt16 data   ;

} Sio_Rx_Data   ;

//--------------------------------------------------------------------------//

typedef struct
{
  Irq   irq        ;

  Pte   pte_break  ;

  UInt8 fifo_count ;

} Sio_Uart_Tx ;

//--------------------------------------------------------------------------//

typedef struct
{
  Mbx q_rcev ;
  Mbx q_free ;

} Sio_Uart_Rx ;

//--------------------------------------------------------------------------//

typedef struct s_Sio_Uart
{
  Sio_Uart_Tx tx ;
  Sio_Uart_Rx rx ;

  struct s_Sio _sio_base_ ;

  volatile UInt32 * uart_reg ;

} * Sio_Uart ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

SInt16  sio_uart_rx_t                   (Sio sio ,               Sio_Timeout timeout) ;
SInt16  sio_uart_tx_t                   (Sio sio , SInt16 data , Sio_Timeout timeout) ;
SInt16  sio_uart_ctrl                   (Sio sio , int    cmd  , va_list args       ) ;

void    sio_uart_ctrl_tx_break          (Sio_Uart sio_uart , Sio_Timeout timeout) ;

SInt16  sio_uart_ctrl_tx_drain_t        (Sio_Uart sio_uart , Sio_Timeout timeout) ;
SInt16  sio_uart_ctrl_rx_flush_t        (Sio_Uart sio_uart , Sio_Timeout timeout) ;

SInt16  sio_uart_ctrl_rx_append_t       (Sio_Uart sio_uart , va_list args) ;

Boolean sio_uart_fifo_rx_append_i       (Sio_Uart sio_uart , SInt16  data) ;

void    sio_uart_ctrl_rx_bypass_enable  (Sio_Uart sio_uart)               ;
void    sio_uart_ctrl_rx_bypass_disable (Sio_Uart sio_uart)               ;

void    sio_uart_irq_tx_ena             (const Sio_Uart sio_uart) ;
void    sio_uart_irq_tx_dis             (const Sio_Uart sio_uart) ;

void    sio_uart_irq_rx_ena             (const Sio_Uart sio_uart) ;
void    sio_uart_irq_rx_dis             (const Sio_Uart sio_uart) ;

void    sio_uart_irq_rls_ena            (const Sio_Uart sio_uart) ;
void    sio_uart_irq_rls_dis            (const Sio_Uart sio_uart) ;

void    sio_uart_irq_service            (Sio_Uart sio_uart) ;
void    sio_uart_irq_service_rx         (Sio_Uart sio_uart) ;
void    sio_uart_irq_service_tx         (Sio_Uart sio_uart) ;
void    sio_uart_irq_service_rls        (Sio_Uart sio_uart) ;

Boolean sio_uart_wait_tx_ready          (Sio_Uart sio_uart , Sio_Timeout timeout) ;

void    sio_uart_irq_service_vector_0   (void) ;
void    sio_uart_irq_service_vector_1   (void) ;
void    sio_uart_irq_service_vector_2   (void) ;
void    sio_uart_irq_service_vector_3   (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern const UInt32 IRQ_LSR_ENABLE  ;

extern const UInt8  BIT_RDR         ;
extern const UInt32 IRQ_RDR_ENABLE  ;

extern const UInt8  BIT_THRE        ;
extern const UInt32 IRQ_THRE_ENABLE ;

//--------------------------------------------------------------------------//

extern Sio_Uart sio_uart_table [SIO_UART_COUNT] ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

