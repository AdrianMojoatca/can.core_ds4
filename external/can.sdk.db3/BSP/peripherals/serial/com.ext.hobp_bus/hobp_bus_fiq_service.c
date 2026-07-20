/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hobp_bus_fiq_service.c 34443 2015-10-19 14:58:23Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "hobp_bus_private.h"
   
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define MIN(z) (z + (z * 0.05))//-5% 
#define MAX(z) (z - (z * 0.05))//+5%



/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void hobp_bus_fiq_process (void)          ;

static void rx_data_bit         (Boolean bit)   ;
static void tx_data_bit         (void)          ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

 
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void hobp_bus_fiq_service(void)
{
  hobp_bus_fiq_process() ;
}
//--------------------------------------------------------------------------//
static void hobp_bus_fiq_process(void)
{
  if(HOBP_BUS_RX_RISING || HOBP_BUS_RX_FALLING) // inverted data line
    {
      if(HOBP_BUS_RX_RISING)
        {
          UInt32 cur_time = T1TC - (fiq_get_tc (FIQ_TC_TIMER_1)) ;

          tmr_start_fiq_rx (hobp_bus_vars.tmr_rx , (hobp_bus_vars.baudrate/2) - cur_time) ;
        }
      
      hobp_bus_rx_clr () ;
    }
  
  if(tmr_int_sta (hobp_bus_vars.tmr_rx))
    {
      Boolean io_state = HOBP_BUS_IO_RX ;
      
      tmr_start (hobp_bus_vars.tmr_rx , (hobp_bus_vars.baudrate) , TRUE) ; 

      rx_data_bit (io_state)    ;
    }

  if(tmr_int_sta (hobp_bus_vars.tmr_tx))
    {
      tx_data_bit () ;
    } 
}

//--------------------------------------------------------------------------//
static void rx_data_bit(Boolean bit)
{
  static UInt16 shift_register ;
  static UInt8  bit_counter    ;
    
  shift_register  |= ((UInt16)bit << 10) ;
  
  shift_register >>= 1 ; 

  switch  (++bit_counter)
    {
      case 1 : shift_register = NULL ;// start bit
      case 2 :
      case 3 :
      case 4 :
      case 5 :
      case 6 :
      case 7 :
      case 8 :
      case 9 :   // data bit
        {   
          break ;
        }
      default:
        {
          if(bit || (bit_counter > 50))
            {              
              if(bit_counter == 10)
                {
                  hobp_bus_vars.fiq_rx_data = (UInt8)(shift_register >> 1)  ;
                }
              else if(bit_counter < 50)
                {
                  hobp_bus_vars.fiq_rx_data = HOBP_BUS_SYNCBREAK ;
                }
              else
                {
                  hobp_bus_vars.fiq_rx_data = NULL ;
                }
 
              hobp_bus_set_flag(HOBP_BUS_RX_DATA) ;
              
              tmr_dis (hobp_bus_vars.tmr_rx) ;
              
              bit_counter = 0 ; 
            }
          break ;
        }
    }
}

//--------------------------------------------------------------------------//
static void tx_data_bit(void)
{
  static int   shift_register ;
  static UInt8 bit_counter    ;
  static UInt8 index          ;
 
  tmr_start (hobp_bus_vars.tmr_tx, (HOBP_BUS_TX_SPEED), TRUE) ;

  switch  (++bit_counter)
    {
      case 1: // start bit
        {
          HOBP_BUS_TX_CLR      ;

          shift_register = hobp_bus_vars.fiq_tx_msg.data[index++] ;

          //shift_register = hobp_bus_vars.fiq_tx_data ;

          break ;
        }
      case 2 :
      case 3 :
      case 4 :
      case 5 :
      case 6 :
      case 7 :
      case 8 :
      case 9 :   // data bit
        { 
          Boolean bit = ( shift_register & 0x01) ;
          
          shift_register >>= 1 ;

          if(bit)
            {
              HOBP_BUS_TX_SET ;
            }
          else
            {
              HOBP_BUS_TX_CLR ;
            }
          break ;
        }
      case 10 : // stop bit
        {
          HOBP_BUS_TX_SET ;

          if(--hobp_bus_vars.fiq_tx_msg.size == NULL)
            {
              tmr_dis (hobp_bus_vars.tmr_tx) ;
              
              hobp_bus_set_flag(HOBP_BUS_TX_BYTE_DONE) ; 

              index = 0 ;
            }
          bit_counter = 0 ;

          break ;
        }
      case 11 :
        {
          if(--hobp_bus_vars.fiq_tx_msg.size == NULL)
            {
              tmr_dis (hobp_bus_vars.tmr_tx) ;
              
              hobp_bus_set_flag(HOBP_BUS_TX_BYTE_DONE) ; 

              index = 0 ;
            }
          bit_counter = 0 ;

          break ;           
        }
    }
}


