/*<#======================================================================#>*/
/*                          FILE  if(tmr_int_sta (dst_bus_vars.tmr_tag_tx))
    {
      tx_data_bit()     ;
      
      // Clear timer interrupt flag to prevent infinite FIQ loop
      {
        Tmr tmr = dst_bus_vars.tmr_tag_tx;
        switch (tmr->timer_base_addr)
        {
          case TMR0_BASE_ADDR: T0IR = (1u << tmr->tmr_index); break;
          case TMR1_BASE_ADDR: T1IR = (1u << tmr->tmr_index); break;
          case TMR2_BASE_ADDR: T2IR = (1u << tmr->tmr_index); break;
          case TMR3_BASE_ADDR: T3IR = (1u << tmr->tmr_index); break;
        }
      }
    }ES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dst_bus_fiq_service.c 34922 2015-11-02 21:11:00Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dst_bus_private.h"
#include "fiq.h"
#include "tmr_private.h"

#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/


#define DST_BUS_READER_DATA_MIN      (100 * CPU_CLOCK_MULT)
#define DST_BUS_READER_DATA_MAX      (600 * CPU_CLOCK_MULT)

#define DST_BUS_READER_DATA_LOGIC1   (300 * CPU_CLOCK_MULT) // is greater 300 bit = 1 else i't 0


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void  reader_rx_data  (void) ;

static void  reader_rx_reset (void)        ;
static void  reader_rx_done  (void)        ;


static void  rx_data_bit         (Boolean bit)   ;
static void  tx_data_bit         (void)          ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static Dst_Bus_Msg fiq_reader_msg = {NULL} ;

static volatile UInt8   reader_bit_counter ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
void dst_bus_fiq_service (void)
{  
  static volatile Boolean level;
  static volatile SInt8 last_level = -1; // assume line at connector is low,

  
  if(DST_BUS_TAG_RX_RISING || DST_BUS_TAG_RX_FALLING) 
    {
	  if(DST_BUS_TAG_RX_FALLING)
        {
          UInt32 cur_time = T1TC - (fiq_get_tc (FIQ_TC_TIMER_1)) ;

          tmr_start_fiq_rx (dst_bus_vars.tmr_tag_rx , (dst_bus_vars.tag_baudrate/2) - cur_time) ;

          tmr_dis   (dst_bus_vars.tmr_tag_rx_timeout) ;
        }
      
      DST_BUS_TAG_RX_INT_CLR ;
    }  
  
  if(tmr_int_sta (dst_bus_vars.tmr_tag_rx))
    {
      Boolean io_state = DST_BUS_TAG_IO_RX ;
      
      tmr_start (dst_bus_vars.tmr_tag_rx , (dst_bus_vars.tag_baudrate) , TRUE) ; 

      rx_data_bit (io_state)    ;
      
      // Clear timer interrupt flag to prevent infinite FIQ loop
      {
        Tmr tmr = dst_bus_vars.tmr_tag_rx;
        TxIR = (1u << tmr->tmr_index);
      }
    }    

  level = DST_BUS_READER_RX;
  
  if((level != last_level)) // real line change detect
    {
 
	  last_level = level;

      DST_BUS_READER_RX_INT_CLR                       ;
      
      reader_rx_data() ;

      tmr_update(dst_bus_vars.tmr_reader_rx_timeout , DST_BUS_READER_DATA_MAX , TRUE) ;
      tmr_ena   (dst_bus_vars.tmr_reader_rx_timeout)     ;      
      
    } 
  else if(DST_BUS_READER_RX_RISING || DST_BUS_READER_RX_FALLING)
    {      
      DST_BUS_READER_RX_INT_CLR                       ;
      
//      reader_rx_data() ;

//      tmr_update(dst_bus_vars.tmr_reader_rx_timeout , DST_BUS_READER_DATA_MAX , TRUE) ;
//      tmr_ena   (dst_bus_vars.tmr_reader_rx_timeout)     ;
    }
 
  if(tmr_int_sta (dst_bus_vars.tmr_tag_tx))
    {
      tx_data_bit () ;
      
      // Clear timer interrupt flag to prevent infinite FIQ loop
      {
        Tmr tmr = dst_bus_vars.tmr_tag_tx;
        TxIR = (1u << tmr->tmr_index);
      }
    } 

  
  if(tmr_int_sta (dst_bus_vars.tmr_reader_rx_timeout))
    {     
      reader_rx_done() ;
      
      // Clear timer interrupt flag to prevent infinite FIQ loop
      {
        Tmr tmr = dst_bus_vars.tmr_reader_rx_timeout;
        TxIR = (1u << tmr->tmr_index);
      }
    }

  if(tmr_int_sta (dst_bus_vars.tmr_tag_rx_timeout))
    {     
      dst_bus_tag_rx_done() ;
      
      // Clear timer interrupt flag to prevent infinite FIQ loop
      {
        Tmr tmr = dst_bus_vars.tmr_tag_rx_timeout;
        TxIR = (1u << tmr->tmr_index);
      }
    }

}

//--------------------------------------------------------------------------//
static void  reader_rx_data (void)
{
  static UInt32 last_time = NULL;
  UInt32 cur_time;
  volatile Boolean bit;
  
  cur_time = fiq_get_tc(FIQ_TC_TIMER_1); 
  
  if(dst_bus_vars.nvfs_entry.reader_data_level == DST_BUS_READER_RX)
    {          
      if(reader_bit_counter == 0 && fiq_reader_msg.size == NULL)
        {
          last_time = NULL ;
        }
    }
  else
    {
      // Protecție la buffer overflow
      if(fiq_reader_msg.size >= 30)  // Max array index is 29, size becomes 30 after last byte
        {
          reader_rx_reset();
          return;
        }

      bit = (cur_time - last_time) > DST_BUS_READER_DATA_LOGIC1;
      
      fiq_reader_msg.data[fiq_reader_msg.size] >>= 1;
      fiq_reader_msg.data[fiq_reader_msg.size] |= (bit << 7);
 
      if(++reader_bit_counter == 8)
        {          
          reader_bit_counter = 0;
          fiq_reader_msg.size++;
        }            
    }

  last_time  =  cur_time                   ;  
}
//--------------------------------------------------------------------------//
static void  reader_rx_reset (void)
{
  tmr_dis(dst_bus_vars.tmr_reader_rx_timeout)           ;

  memset (&fiq_reader_msg , 0 , sizeof(fiq_reader_msg)) ;  
    
  reader_bit_counter  = 0;
  
}
//--------------------------------------------------------------------------//
static void  reader_rx_done (void)
{    
  if(fiq_reader_msg.size > 0)  // Protecție la underflow
    {
      fiq_reader_msg.size--;
      dst_bus_vars.reader_msg = fiq_reader_msg;
      dst_bus_set_flag(DST_BUS_FLAG_READER);
    }

  reader_rx_reset() ;

  if((dst_bus_vars.nvfs_entry.reader_data_level == DST_BUS_READER_RX))
    {   
      if((dst_bus_vars.tx_msg.size != NULL) && dst_bus_vars.flg_gwr)
        {
          tmr_start (dst_bus_vars.tmr_tag_tx , (DST_BUS_TAG_TX_DATA_TIMEOUT) , TRUE) ;
        }
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
      case 1: // start bit
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
          if(bit)
            {              
              if(bit_counter == 10)
                {
                  
                  dst_bus_vars.fiq_rx_data = (UInt8)(shift_register >> 1)  ;
                }
              else
                {
                  dst_bus_vars.fiq_rx_data = -1 ; // error
                }

              dst_bus_set_flag(DST_BUS_FLAG_TAG_RX_BYTE) ;
            }
          else if(bit_counter < 50)
            {
              return ;
            }          
          
          tmr_dis (dst_bus_vars.tmr_tag_rx) ;
          
          bit_counter = 0 ; 
          
          break ;
        }
    }
}
//--------------------------------------------------------------------------//
static void tx_data_bit(void)
{
  static int   shift_register ;
  static UInt8 bit_counter    ;
  static UInt8 data_ptr       = NULL ;
 
  tmr_start (dst_bus_vars.tmr_tag_tx, (dst_bus_vars.tag_baudrate), TRUE) ;

  switch  (++bit_counter)
    {
      case 1: // start bit
        {          
          DST_BUS_TX_CLR;
          
          // Protecție la buffer overflow
          if(data_ptr >= 30)  // Max msg size
            {
              tmr_dis(dst_bus_vars.tmr_tag_tx);
              data_ptr = 0;
              bit_counter = 0;
              return;
            }

          shift_register = dst_bus_vars.tx_msg.data[data_ptr++];
          break;
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
              DST_BUS_TX_SET ;
            }
          else
            {
              DST_BUS_TX_CLR ;
            }
          break ;
        }
      case 10 : // stop bit
        {
          DST_BUS_TX_SET ;

          break ;
        }
      case 11 :
        {
          bit_counter = 0 ;
          
          if(--dst_bus_vars.tx_msg.size == NULL)
            {
              tmr_dis (dst_bus_vars.tmr_tag_tx) ;
              
              data_ptr = NULL ; 
            }
     
          break ;           
        }
    }
}
