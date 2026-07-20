/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sio_uart_irq_service_rls.c 32991 2015-07-28 15:42:19Z martin.bouchard $
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

void sio_uart_irq_service_rls (Sio_Uart sio_uart)
{
  typedef struct
    { 
      UInt8  ulsr_bit      ;
      SInt16 ulsr_evt_name ;

    } Map ;

  static const Map map [] = // !!! Order Is Critical !!!
    {
      #define BIT(z) (1 << z)

      { BIT (4) , SIO_UART_STATUS_BREAK   } ,  // 
      { BIT (3) , SIO_UART_STATUS_FRAMING } ,  // These status bits *must* be serviced in the order that they
      { BIT (2) , SIO_UART_STATUS_PARITY  } ,  // appear in order to correctly determine the error event
      { BIT (1) , SIO_UART_STATUS_OVERRUN } ,  // 

      #undef  BIT
    } ;

  UInt8 ulsr = sio_uart->uart_reg [ULSR] ; // Clears IRQ flags

  UInt8 index = 0 ;

  /// Scan the URLS status register bits to determine event \\\

  do
    {
      if (ulsr & map [index].ulsr_bit)
        {
          sio_uart->uart_reg [URBR] ;          // Read out the received char that caused the error and discard

          sio_uart_fifo_rx_append_i (sio_uart , map [index].ulsr_evt_name) ;

          return ;
        }
    }
  while (++index < sizeof_array (map)) ;

  /// !!! Should never arrive here !!! ///
}

