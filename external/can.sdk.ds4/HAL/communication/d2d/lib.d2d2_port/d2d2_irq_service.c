/*! 
 * @file   d2d2_irq_service.c
 * @brief  
 * @author  DIRECTED ELECTRONICS
 * @date   7/20/2015
 */

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
#include "d2d2_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void d2d2_rls_service        (UInt8 port_no) ;
static void d2d2_rda_service        (UInt8 port_no) ;
static void d2d2_cti_service        (UInt8 port_no) ;
static void d2d2_thre_service       (UInt8 port_no) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/// Executes in SUPERVISOR mode ///

UInt8 itrace[32];
UInt8 itnw = 0;

void d2d2_irq_service (UInt8 which_port, const volatile UInt32 *piir_value)
{ 
UInt8 iir_value;
    
    iir_value = *piir_value; // read register
    for(;;)
    {
        switch ((iir_value>>1) & 7)
        {
          case IIR_RLS  : d2d2_rls_service  (which_port) ; break ;  /* Receive Line Status                     */
          case IIR_RDA  : d2d2_rda_service  (which_port) ; break ;     /* Receive Data Available                  */
          case IIR_CTI  : d2d2_cti_service  (which_port) ; break ;  /* Character timeout indicator             */
          case IIR_THRE : d2d2_thre_service (which_port) ; break ;  /* THRE, transmit holding register empty   */
        }
        iir_value = *piir_value;
        if((iir_value & 1)==1)
            break;
    }
}

void d2d2_irq0_service(void)
{
    d2d2_irq_service(UART_PORT0,&U0IIR);
}
void d2d2_irq1_service(void)
{
    d2d2_irq_service(UART_PORT1,&U1IIR);
}

void d2d2_irq3_service(void)
{
    d2d2_irq_service(UART_PORT3,&U3IIR);
}

static UInt8 lsr_value_rd(UInt8 port_no)
{
    UInt8 ret_rd=0;
    switch(port_no)
    {
    case UART_PORT0:
        ret_rd = U0LSR;
        break;
    case UART_PORT1:
        ret_rd = U1LSR;
        break;
    case UART_PORT3:
        ret_rd = U3LSR;
        break;
    }

    return ret_rd;
}

static SInt16 rda_rx_rd(UInt8 port_no)
{
    SInt16 ret_rd=0;

    switch(port_no)
    {
    case UART_PORT0:
        ret_rd = U0RBR;
        break;
    case UART_PORT1:
        ret_rd = U1RBR;
        break;
    case UART_PORT3:
        ret_rd = U3RBR;
        break;
    }

    return ret_rd;
}

//--------------------------------------------------------------------------//
static void d2d2_rls_service  (UInt8 port_no) 
{
  UInt8 lsr_value = 0;
  lsr_value = lsr_value_rd(port_no);

  if ( lsr_value & LSR_RDR )    /* Receive Data Ready */            
  {
    /* If no error on RLS, normal ready, save into the data buffer. */
    /* Note: read RBR will clear the interrupt */
    d2d2_rda_rx = rda_rx_rd(port_no);

    //os_irq_set_i (d2d2_irq_rx) ;                /* Signal task that data has arrived*/
  }
  else if ( lsr_value & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
  {
    /* There are errors or break interrupt */
    /* Read LSR will clear the interrupt */

    UInt8  dummy  = rda_rx_rd(port_no)      ;        /* Dummy read on RX to clear */
                                      /*interrupt, then bail out */
  }
}

//--------------------------------------------------------------------------//
static void d2d2_rda_service  (UInt8 port_no)                 /* Receive Data Available */
{
  struct s_Smsg_Port *port;
//  UInt8  lsr_value=0;

  port = smsg_port[port_no];
  d2d2_rda_rx = rda_rx_rd(port_no);
  /* lsr_value = */ (void)lsr_value_rd(port_no); // read but discard.

  port->rx_hist[(port->wofs_hist++) & (DETECT_SIZE -1)] = d2d2_rda_rx;
  if(d2d2_rda_rx == 0xFC)
  {
      if(  port->rx_hist[(port->wofs_hist-3) & (DETECT_SIZE -1)] == 0xE0
          && port->rx_hist[(port->wofs_hist-2) & (DETECT_SIZE -1)] == 0
          )
      {
          port->busstate = BUS_MAINT;
          port_timer_rewind(port, 1);
          return;
      }
  }

// THIS SHOULD NOT BE CONDITIONAL!  //  if (!( lsr_value & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI)))
  {
      if(port && port->rxirq)
          (* port->rxirq)(port, d2d2_rda_rx); // send the error anyway, crc or protocol will figure it out 
  }
//    else
//        dbg_never = lsr_value & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI);
}
//--------------------------------------------------------------------------//
static void d2d2_cti_service  (UInt8 port_no)    /* Character Time-out indicator */
{
  //UInt8  lsr_value = 0;
  //lsr_value = lsr_value_rd(port_no);

  /*UART0Status |= 0x100; */        /* Bit 9 as the CTI error */
}
//--------------------------------------------------------------------------//
static void d2d2_thre_service (UInt8 port_no)    /* THRE interrupt */
{
    struct s_Smsg_Port *port;
    UInt8  lsr_value=0;

    lsr_value = lsr_value_rd(port_no);     /* Check status in the LSR to see if */
                                   /* valid data in U0THR or not */ 

    if ( lsr_value & LSR_THRE )
    {
      port = smsg_port[port_no];
        
      if(port && port->txirq)
         (* port->txirq)(port); 
    }
    else
    {
      ;                            /* tx fifo not empty  */
    }
}

//--------------------------------------------------------------------------//


