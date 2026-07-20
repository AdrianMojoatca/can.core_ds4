/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sio_uart_ctrl_rx_append_t.c 32991 2015-07-28 15:42:19Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "sio_uart_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static SInt16 sio_uart_process_irq_call (Sio_Uart sio_uart , va_list args) ;
static SInt16 sio_uart_process_tsk_call (Sio_Uart sio_uart , va_list args) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

SInt16 sio_uart_ctrl_rx_append_t (Sio_Uart sio_uart , va_list args)
{
  return (os_irq_executing ? sio_uart_process_irq_call : sio_uart_process_tsk_call) (sio_uart , args) ;
}

//--------------------------------------------------------------------------//

static SInt16 sio_uart_process_irq_call (Sio_Uart sio_uart , va_list args)
{
  return sio_uart_fifo_rx_append_i (sio_uart , (SInt16) va_arg (args , int)) ? SIO_OK : SIO_RX_APPEND_FAIL ;
}

//--------------------------------------------------------------------------//

static SInt16 sio_uart_process_tsk_call (Sio_Uart sio_uart , va_list args)
{
  SInt16      data    = (SInt16     ) va_arg (args , int        ) ;
  Sio_Timeout timeout = (Sio_Timeout) va_arg (args , Sio_Timeout) ;

  MbxMsg * rx_data = os_mbx_rcev (sio_uart->rx.q_free , timeout) ;

  if (rx_data)
    {
      cast_mbx_msg_to_sio_rx_data (rx_data)->data = data ;

      os_mbx_send (sio_uart->rx.q_rcev , rx_data) ;

      return SIO_OK ;
    }

  return SIO_TIMEOUT ;
}

