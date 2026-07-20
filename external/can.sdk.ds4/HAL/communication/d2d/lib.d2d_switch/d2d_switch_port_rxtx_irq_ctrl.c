/*! 
 * @file   port_rxtx_irq_ctrl.c
 * @brief  
 * @author  DIRECTED ELECTRONICS
 * @date   8/20/2015
 */

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2d_switch_private.h"

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

void d2d_switch_port_tx_irq_ctrl(void *port, Boolean ctrl)
{
    UInt8 which_port;
    which_port = ((SmsgPort *)port)->number;

    if(which_port == UART_PORT0)
    {
          if(ctrl)
              U0IER |= 1 << 1 ;
          else
              U0IER &= ~(1 << 1) ;
    }
    else if(which_port == UART_PORT1)
    {
        if(ctrl)
            U1IER |= 1 << 1 ;
        else
            U1IER &= ~(1 << 1) ;
    }
    else if(which_port == UART_PORT3)
    {
        if(ctrl)
            U3IER |= 1 << 1 ;
        else
            U3IER &= ~(1 << 1) ;
    }
}

void d2d_switch_port_rx_irq_ctrl(void *port, Boolean ctrl)
{
    UInt8 which_port;
    which_port = ((SmsgPort *)port)->number;

    if(which_port == UART_PORT0)
    {
          if(ctrl)
              U0IER |= 1 << 0 ;
          else
              U0IER &= ~(1 << 0) ;
    }
    else if(which_port == UART_PORT1)
    {
        if(ctrl)
            U1IER |= 1 << 0 ;
        else
            U1IER &= ~(1 << 0) ;
    }
    else if(which_port == UART_PORT3)
    {
        if(ctrl)
            U3IER |= 1 << 0 ;
        else
            U3IER &= ~(1 << 0) ;
    }
}

