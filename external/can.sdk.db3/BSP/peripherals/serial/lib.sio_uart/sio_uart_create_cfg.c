/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sio_uart_create_cfg.c 33206 2015-08-17 19:00:16Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "isr.h"
#include "sio_uart_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
typedef struct s_Sio_Uart_Ctrl_Bits /// !!! Order is Critical !!!
{
  volatile unsigned long * reg         ;  // Register address 
  unsigned                 bit_rhs : 5 ;  // LSB starting bit
  unsigned                 bit_val : 2 ;  // Value to set field to

} Sio_Uart_Ctrl_Bits ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void sio_uart_init_sio             (Sio sio) ;
static void sio_uart_init_sio_uart        (Sio_Uart sio_uart , const Sio_Uart_Cfg * const cfg) ;
static void sio_uart_init_sio_uart_tx     (Sio_Uart_Tx * tx) ;
static void sio_uart_init_sio_uart_rx     (Sio_Uart_Rx * rx  , const struct s_Sio_Uart_Rx_Param * const rx_param) ;

static void sio_uart_init_pconp           (const Sio_Uart_Id uart_id) ;
static void sio_uart_init_cclk_divider    (const Sio_Uart_Id uart_id , const Sio_Uart_CCLK_Div cclk_div) ;

static void sio_uart_init_baud_divider    (volatile UInt32 * const uart_reg , const UInt16 divider) ;
static void sio_uart_init_uart_parameters (volatile UInt32 * const uart_reg , const struct s_Sio_Uart_Framing * const framing , Sio_Uart_Rx_Fifo_Level rx_fifo_level) ;

static void sio_uart_init_vic_vector      (const Sio_Uart_Id uart_id , const UInt8 vic_priority) ;

static void sio_uart_init_io_pins         (const struct s_Sio_Uart_Pin_Map * const pin_map) ;

static void sio_uart_set_ctrl_bits        (const Sio_Uart_Ctrl_Bits * const ctrl_bits_map) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Sio sio_uart_create_cfg (const Sio_Uart_Cfg * const cfg)
{
  Sio_Uart sio_uart = (Sio_Uart) os_mem_request (sizeof (struct s_Sio_Uart)) ;

  ATOMIC
    (
      sio_uart_init_sio_uart        (sio_uart , cfg) ;
    
      sio_uart_init_pconp           (cfg->uart_id)   ;
    
      sio_uart_init_io_pins         (& cfg->pin_map) ;
    
      sio_uart_init_cclk_divider    (cfg->uart_id , cfg->clk.cclk_div) ;
    
      sio_uart_init_baud_divider    (sio_uart->uart_reg , cfg->clk.divider) ;
    
      sio_uart_init_uart_parameters (sio_uart->uart_reg , & cfg->framing , cfg->rx_param.fifo_level) ;
    
      sio_uart_init_vic_vector      (cfg->uart_id , cfg->vic_priority) ;

      sio_uart_irq_rx_ena  (sio_uart) ;

      if (cfg->rls_enabled)
        {
          sio_uart_irq_rls_ena (sio_uart) ;
        }

      sio_uart_table [cfg->uart_id] = sio_uart ;
    )

  return cast_sio_uart_to_sio (sio_uart)   ;
}

//--------------------------------------------------------------------------//

static void sio_uart_init_sio_uart (Sio_Uart sio_uart , const Sio_Uart_Cfg * const cfg)
{
  sio_uart_init_sio (cast_sio_uart_to_sio (sio_uart)) ;

  /*sio_uart_init_uart_base_reg :*/
    {
      static const UInt32 uart_base_addr [SIO_UART_COUNT] = // !!! Order is critical !!!
        {
          UART0_BASE_ADDR , UART1_BASE_ADDR , UART2_BASE_ADDR , UART3_BASE_ADDR
        } ;
    
      sio_uart->uart_reg = (volatile UInt32 *) uart_base_addr [cfg->uart_id] ;
    }

  sio_uart_init_sio_uart_tx (& sio_uart->tx                  ) ;
  sio_uart_init_sio_uart_rx (& sio_uart->rx , & cfg->rx_param) ;
}

//--------------------------------------------------------------------------//

static void sio_uart_init_sio (Sio sio)
{
  sio->tx_t = sio_uart_tx_t ;
  sio->rx_t = sio_uart_rx_t ;
  sio->ctrl = sio_uart_ctrl ;
}

//--------------------------------------------------------------------------//

static void sio_uart_init_sio_uart_tx (Sio_Uart_Tx * tx)
{
  tx->irq        = os_irq_create ( ) ;

  tx->pte_break  = os_pte_create (0) ;

  tx->fifo_count = 0 ;
}

//--------------------------------------------------------------------------//

static void sio_uart_init_sio_uart_rx (Sio_Uart_Rx * rx , const struct s_Sio_Uart_Rx_Param * const rx_param)
{
  UInt16 fifo_size = rx_param->fifo_size ;

  rx->q_rcev = os_mbx_create () ;
  rx->q_free = os_mbx_create () ;

  while (fifo_size--)
    {
      os_mbx_send (rx->q_free , cast_sio_rx_data_to_mbx_msg ((Sio_Rx_Data *) os_mem_request (sizeof (Sio_Rx_Data)))) ;
    }
}

//--------------------------------------------------------------------------//

static void sio_uart_init_vic_vector (const Sio_Uart_Id uart_id , const UInt8 vic_priority)
{
  typedef struct s_Vic_Info
    {
      Isr_Number irq_num ;

      void (* irq_vector)(void) ;

    } Irq_Info ;

  static Irq_Info const irq_info [SIO_UART_COUNT] = // !!! Order is critical !!! 
    {
      { ISR_ID_UART_0 , sio_uart_irq_service_vector_0 } ,
      { ISR_ID_UART_1 , sio_uart_irq_service_vector_1 } ,
      { ISR_ID_UART_2 , sio_uart_irq_service_vector_2 } ,
      { ISR_ID_UART_3 , sio_uart_irq_service_vector_3 }
    } ;

  #ifdef DEV_LPC_236x
    #define Isr_Pri Vic_Isr_Pri
  #endif  
  isr_set_vector ((Isr_Number)irq_info[uart_id].irq_num , (Isr_Pri)vic_priority , ISR_TYPE_IRQ , (PVF)irq_info[uart_id].irq_vector) ;
}

//--------------------------------------------------------------------------//

static void sio_uart_init_pconp (Sio_Uart_Id uart_id)
{
  static const UInt8 pcon_bits [SIO_UART_COUNT] = // !!! Order is critical !!!
    {
       3 , // SIO_UART_0
       4 , // SIO_UART_1
      24 , // SIO_UART_2
      25 , // SIO_UART_3
    } ;

  PCONP |= (UInt32) 1 <<  pcon_bits [uart_id] ;   // Enable UART peripheral
}

//--------------------------------------------------------------------------//

static void sio_uart_init_cclk_divider (const Sio_Uart_Id uart_id , const Sio_Uart_CCLK_Div cclk_div)
{
  static const Sio_Uart_Ctrl_Bits sio_uart_set_ctrl_bits_clock [SIO_UART_TX_MAP_COUNT] = // !!! Order is critical !!!
    {
      { VUL_TYPECAST & PCLKSEL0 ,  6U } , // SIO_UART_0
      { VUL_TYPECAST & PCLKSEL0 ,  8U } , // SIO_UART_1
      { VUL_TYPECAST & PCLKSEL1 , 16U } , // SIO_UART_2
      { VUL_TYPECAST & PCLKSEL1 , 18U } , // SIO_UART_3
    } ;
  
  Sio_Uart_Ctrl_Bits ctrl_bits = sio_uart_set_ctrl_bits_clock[uart_id] ;
  
  ctrl_bits.bit_val = cclk_div ;
  
  sio_uart_set_ctrl_bits (& ctrl_bits)  ;
}

//--------------------------------------------------------------------------//

static void sio_uart_init_baud_divider (volatile UInt32 * const uart_reg , const UInt16 divider)
{
  uart_reg [ULCR] |= 0x80 ; // Enable  access to divisor latches

  uart_reg [UDLM] = (UInt8) (divider >> 8) ; // MSB of divider
  uart_reg [UDLL] = (UInt8) (divider >> 0) ; // LSB of divider

  uart_reg [ULCR] &= 0x7F ; // Disable access to divisor latches
}

//--------------------------------------------------------------------------//

static void sio_uart_init_uart_parameters (volatile UInt32 * const uart_reg , const struct s_Sio_Uart_Framing * const framing , Sio_Uart_Rx_Fifo_Level rx_fifo_level)
{
  UInt8 ulcr ;

  if (framing->parity == SIO_UART_PARITY_NONE)
    {
      ulcr = 0x00 ;
    }
  else
    {
      ulcr = 0x08 | ((framing->parity - 1) << 4) ;
    }

  ulcr |= (framing->stop_bits << 2) ;
  ulcr |= (framing->data_bits << 0) ;

  uart_reg [ULCR] &= 0x80 ;                  // Set everything to zero and enable divisor latch select bit
  uart_reg [ULCR] |= ulcr ;                  // Configure framing
  uart_reg [UFCR]  = 0x01 | rx_fifo_level ;  // Enable HW FIFO logic and RX triger level
}

//--------------------------------------------------------------------------//

static void sio_uart_init_io_pins (const struct s_Sio_Uart_Pin_Map * const pin_map)
{
  static const Sio_Uart_Ctrl_Bits sio_uart_set_ctrl_bits_tx [SIO_UART_TX_MAP_COUNT] = // !!! Order is critical !!!
    {
      { VUL_TYPECAST & (PINSEL0) ,  4U , 0x01 } , // SIO_UART_TX_0_P0_2  
    
      { VUL_TYPECAST & (PINSEL0) , 30U , 0x01 } , // SIO_UART_TX_1_P0_15 
      { VUL_TYPECAST & (PINSEL4) ,  0U , 0x02 } , // SIO_UART_TX_1_P2_0  
                                  
      { VUL_TYPECAST & PINSEL0 , 20U , 0x01 } , // SIO_UART_TX_2_P0_10 
      { VUL_TYPECAST & PINSEL4 , 16U , 0x02 } , // SIO_UART_TX_2_P2_8  
                                  
      { VUL_TYPECAST & PINSEL0 ,  0U , 0x02 } , // SIO_UART_TX_3_P0_0  
      { VUL_TYPECAST & PINSEL1 , 18U , 0x03 } , // SIO_UART_TX_3_P0_25 
      { VUL_TYPECAST & PINSEL9 , 24U , 0x03 } , // SIO_UART_TX_3_P4_28
    } ;

  static const Sio_Uart_Ctrl_Bits sio_uart_set_ctrl_bits_rx [SIO_UART_RX_MAP_COUNT] = // !!! Order is critical !!!
    {
      { VUL_TYPECAST & PINSEL0 ,  6U , 0x01 } , // SIO_UART_RX_0_P0_3 
    
      { VUL_TYPECAST & PINSEL1 ,  0U , 0x01 } , // SIO_UART_RX_1_P0_16
      { VUL_TYPECAST & PINSEL4 ,  2U , 0x02 } , // SIO_UART_RX_1_P2_1 
                                                  
      { VUL_TYPECAST & PINSEL0 , 22U , 0x01 } , // SIO_UART_RX_2_P0_11
      { VUL_TYPECAST & PINSEL4 , 18U , 0x02 } , // SIO_UART_RX_2_P2_9 
                                                  
      { VUL_TYPECAST & PINSEL0 ,  2U , 0x02 } , // SIO_UART_RX_3_P0_1 
      { VUL_TYPECAST & PINSEL1 , 20U , 0x03 } , // SIO_UART_RX_3_P0_26
      { VUL_TYPECAST & PINSEL9 , 26U , 0x03 } , // SIO_UART_RX_3_P4_29
    } ;

  sio_uart_set_ctrl_bits (& sio_uart_set_ctrl_bits_tx [pin_map->tx]) ;
  sio_uart_set_ctrl_bits (& sio_uart_set_ctrl_bits_rx [pin_map->rx]) ;
}

//--------------------------------------------------------------------------//

static void sio_uart_set_ctrl_bits (const Sio_Uart_Ctrl_Bits * const ctrl_bits_map)
{
  const UInt32 bit_mask = ~ ((UInt32) 0x03                   << ctrl_bits_map->bit_rhs) ;
  const UInt32 bit_val  =   ((UInt32) ctrl_bits_map->bit_val << ctrl_bits_map->bit_rhs) ;

  *ctrl_bits_map->reg &= bit_mask ;
  *ctrl_bits_map->reg |= bit_val  ;
}

