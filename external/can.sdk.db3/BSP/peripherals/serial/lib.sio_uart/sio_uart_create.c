/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sio_uart_create.c 32991 2015-07-28 15:42:19Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "isr.h"
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

Sio sio_uart_create (const Sio_Uart_Id uart_id , const Sio_Uart_Tx_Map tx_map , const Sio_Uart_Rx_Map rx_map , const UInt8 vic_priority , const Sio_Uart_CCLK_Div cclk_div , const UInt16 divider)
{
  Sio_Uart_Cfg cfg ;

  cfg.uart_id                = uart_id ;

  cfg.rls_enabled            = FALSE ;

  cfg.vic_priority           = vic_priority ;

  cfg.pin_map.tx             = tx_map ; 
  cfg.pin_map.rx             = rx_map ; 

  cfg.clk.cclk_div           = cclk_div ;
  cfg.clk.divider            = divider  ;

  cfg.framing.parity         = SIO_UART_PARITY_NONE ;
  cfg.framing.data_bits      = SIO_UART_DATA_BITS_8 ;
  cfg.framing.stop_bits      = SIO_UART_STOP_BITS_1 ;

  cfg.rx_param.fifo_size     = 16                       ;
  cfg.rx_param.fifo_level    = SIO_UART_RX_FIFO_LEVEL_1 ;

  return sio_uart_create_cfg (& cfg) ;
}

