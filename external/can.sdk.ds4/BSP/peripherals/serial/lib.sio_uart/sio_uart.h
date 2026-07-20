/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sio_uart.h 32991 2015-07-28 15:42:19Z martin.bouchard $
/*==========================================================================*/

#ifndef __SIO_UART_H__
#define __SIO_UART_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include <stdarg.h>

#include "os.h"
#include "sio.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Sio_Uart * Sio_Uart ;

//--------------------------------------------------------------------------//

typedef enum // !!! Order Critical !!!
{
  SIO_UART_CTRL_TX_BREAK = SIO_CTRL_USER_BASE ,  // Generate break on TX   Thread : (IRQ only)

  SIO_UART_CTRL_RX_BYPASS_ENABLE              ,  // Enable  RX bypass mode Thread : (IRQ or TASK)
  SIO_UART_CTRL_RX_BYPASS_DISABLE             ,  // Disable RX bypass mode Thread : (IRQ or TASK)

  /// !!! This Must Be Last !!! ///

  SIO_UART_CTRL_USER_BASE                        // Starting point for derived-type commands

} Sio_Uart_Ctrl_Cmds ;

//--------------------------------------------------------------------------//

typedef enum
{
  SIO_UART_STATUS_BREAK   = SIO_STATUS_USER_BASE - 0 ,
  SIO_UART_STATUS_PARITY  = SIO_STATUS_USER_BASE - 1 ,
  SIO_UART_STATUS_FRAMING = SIO_STATUS_USER_BASE - 2 ,
  SIO_UART_STATUS_OVERRUN = SIO_STATUS_USER_BASE - 3 ,

  /// !!! This Must Be Last !!! ///

  SIO_UART_STATUS_USER_BASE = SIO_STATUS_USER_BASE - 4 ,

} Sio_Uart_Status ;

//--------------------------------------------------------------------------//

typedef enum // !!! Order is Critical !!!
{
  SIO_UART_0 ,
  SIO_UART_1 ,
  SIO_UART_2 ,
  SIO_UART_3 ,

  SIO_UART_COUNT

} Sio_Uart_Id ;

//--------------------------------------------------------------------------//

typedef enum e_Sio_Uart_CCLK_Div
{
  #define FIELD(b1,b0) (((unsigned) (b1) << 1) | ((unsigned) (b0) << 0))

  SIO_UART_CCLK_DIV_1 = FIELD (0,1) ,
  SIO_UART_CCLK_DIV_2 = FIELD (1,0) , 
  SIO_UART_CCLK_DIV_4 = FIELD (0,0) , 
  SIO_UART_CCLK_DIV_8 = FIELD (1,1) , 

  #undef  FIELD

} Sio_Uart_CCLK_Div ;

//--------------------------------------------------------------------------//

typedef enum // !!! Order is Critical !!!
{
  SIO_UART_TX_0_P0_2  ,

  SIO_UART_TX_1_P0_15 ,
  SIO_UART_TX_1_P2_0  ,
           
  SIO_UART_TX_2_P0_10 ,
  SIO_UART_TX_2_P2_8  ,
           
  SIO_UART_TX_3_P0_0  ,
  SIO_UART_TX_3_P0_25 ,
  SIO_UART_TX_3_P4_28 ,

  SIO_UART_TX_MAP_COUNT

} Sio_Uart_Tx_Map ;

//--------------------------------------------------------------------------//

typedef enum // !!! Order is Critical !!!
{
  SIO_UART_RX_0_P0_3  ,

  SIO_UART_RX_1_P0_16 ,
  SIO_UART_RX_1_P2_1  ,
           
  SIO_UART_RX_2_P0_11 ,
  SIO_UART_RX_2_P2_9  ,
           
  SIO_UART_RX_3_P0_1  ,
  SIO_UART_RX_3_P0_26 ,
  SIO_UART_RX_3_P4_29 ,

  SIO_UART_RX_MAP_COUNT

} Sio_Uart_Rx_Map ;

//--------------------------------------------------------------------------//

typedef enum // !!! Order is Critical !!!
{
  SIO_UART_DATA_BITS_5 ,
  SIO_UART_DATA_BITS_6 ,
  SIO_UART_DATA_BITS_7 ,
  SIO_UART_DATA_BITS_8 ,

} Sio_Uart_Data_Bits ;

//--------------------------------------------------------------------------//

typedef enum // !!! Order is Critical !!!
{
  SIO_UART_STOP_BITS_1 ,
  SIO_UART_STOP_BITS_2 ,

} Sio_Uart_Stop_Bits ;

//--------------------------------------------------------------------------//

typedef enum // !!! Order is Critical !!!
{
  SIO_UART_PARITY_NONE    ,
  SIO_UART_PARITY_ODD     ,
  SIO_UART_PARITY_EVEN    ,
  SIO_UART_PARITY_FORCE_1 ,
  SIO_UART_PARITY_FORCE_0 ,

} Sio_Uart_Parity ;

//--------------------------------------------------------------------------//

typedef enum // !!! Order is Critical !!!
{
  #define FIELD(z) ((UInt8)((z) << 6))

  SIO_UART_RX_FIFO_LEVEL_1  = FIELD (0) ,
  SIO_UART_RX_FIFO_LEVEL_4  = FIELD (1) ,
  SIO_UART_RX_FIFO_LEVEL_8  = FIELD (2) ,
  SIO_UART_RX_FIFO_LEVEL_14 = FIELD (3) ,

  #undef  FIELD

} Sio_Uart_Rx_Fifo_Level ;

//--------------------------------------------------------------------------//

typedef struct
{
  Sio_Uart_Id uart_id  ;

  UInt8   vic_priority ;

  Boolean rls_enabled  ;

  struct s_Sio_Uart_Pin_Map
    {
      Sio_Uart_Tx_Map tx ;
      Sio_Uart_Rx_Map rx ;

    } pin_map ;

  struct s_Sio_Uart_Clk
    {
      Sio_Uart_CCLK_Div cclk_div ;

      UInt16            divider  ;

    } clk ;

  struct s_Sio_Uart_Rx_Param
    {
      UInt16                 fifo_size  ;
      Sio_Uart_Rx_Fifo_Level fifo_level ;

    } rx_param ;

  struct s_Sio_Uart_Framing
    {
      Sio_Uart_Parity    parity    ;

      Sio_Uart_Data_Bits data_bits ;
      Sio_Uart_Stop_Bits stop_bits ;

    } framing ;

} Sio_Uart_Cfg ;

//--------------------------------------------------------------------------//

#define cast_sio_uart_to_sio(zz) ((Sio     )((UInt8 *)(zz) + struct_offs (Sio_Uart , _sio_base_)))
#define cast_sio_to_sio_uart(zz) ((Sio_Uart)((UInt8 *)(zz) - struct_offs (Sio_Uart , _sio_base_)))

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

EXTERN_C_ENTER

Sio sio_uart_create     (const Sio_Uart_Id uart_id , const Sio_Uart_Tx_Map tx_map , const Sio_Uart_Rx_Map rx_map , const UInt8 vic_priority , const Sio_Uart_CCLK_Div cclk_div , const UInt16 divider) ;

Sio sio_uart_create_cfg (const Sio_Uart_Cfg * const cfg) ;

EXTERN_C_LEAVE

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

