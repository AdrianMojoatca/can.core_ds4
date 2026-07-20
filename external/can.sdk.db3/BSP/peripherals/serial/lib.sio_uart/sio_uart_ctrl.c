/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sio_uart_ctrl.c 32991 2015-07-28 15:42:19Z martin.bouchard $
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

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

SInt16 sio_uart_ctrl (Sio sio , int cmd , va_list args)
{
  Sio_Uart sio_uart = cast_sio_to_sio_uart (sio) ;

  switch (cmd)
    {
      case SIO_CTRL_TX_DRAIN                : return sio_uart_ctrl_tx_drain_t  (sio_uart , va_arg (args , Sio_Timeout)) ;
      case SIO_CTRL_RX_FLUSH                : return sio_uart_ctrl_rx_flush_t  (sio_uart , va_arg (args , Sio_Timeout)) ;

      case SIO_CTRL_RX_APPEND               : return sio_uart_ctrl_rx_append_t (sio_uart , args) ;

      case SIO_UART_CTRL_RX_BYPASS_ENABLE   : sio_uart_ctrl_rx_bypass_enable   (sio_uart)                               ; break ;
      case SIO_UART_CTRL_RX_BYPASS_DISABLE  : sio_uart_ctrl_rx_bypass_disable  (sio_uart)                               ; break ;

      case SIO_UART_CTRL_TX_BREAK           : sio_uart_ctrl_tx_break           (sio_uart , va_arg (args , Sio_Timeout)) ; break ;  
    }

  /// Undefined Commands are Just Ignored \\\

  return SIO_OK ;
}
