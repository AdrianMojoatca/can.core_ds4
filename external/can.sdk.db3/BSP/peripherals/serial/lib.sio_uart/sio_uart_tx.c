/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sio_uart_tx.c 32991 2015-07-28 15:42:19Z martin.bouchard $
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

SInt16 sio_uart_tx_t (Sio sio , SInt16 data , Sio_Timeout timeout)
{
  Sio_Uart sio_uart = cast_sio_to_sio_uart (sio) ;

  /// Check if FIFO is empty, if yes, we can insert immediately without blocking \\\

  if (sio_uart->uart_reg[ULSR] & BIT_THRE)
    {                                 
      goto write_first  ;
    }

  /// If FIFO not empty, check if room for one more \\\

  if (sio_uart->tx.fifo_count < 16)
    {
      sio_uart->tx.fifo_count ++ ;

      goto write_append ;
    }

  /// FIFO may be full, so we must wait for it to be empty before proceeding \\\

  if (! sio_uart_wait_tx_ready (sio_uart , timeout))
    {
      return SIO_TIMEOUT ;
    }

  /// If here, it was a TX ready IRQ that occurred, so FIFO is empty.  Insert the byte \\\

  write_first   : sio_uart->tx.fifo_count   = 1            ;

  write_append  : sio_uart->uart_reg [UTHR] = (UInt8) data ;

  return SIO_OK ;
}

