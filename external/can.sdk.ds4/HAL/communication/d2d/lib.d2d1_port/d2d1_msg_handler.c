/*! 
 * @file   d2d1_msg_handler.c
 * @brief  
 * @author  DIRECTED ELECTRONICS
 * @date   8/13/2015
 */

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2d1_private.h"
#include "intl_port.h"  //d2d_cmd_enabled
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static UInt8 d2d1_cdeq(D2d1_Port *port);
static void d2d1_cenq(D2d1_Port *port,UInt8 c);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/


/**
* @brief  called by task only
* @param
* @return
*/
void d2d1_enqmsg(D2d1_Port *port,UInt8 *ptr,UInt8 len)
{
    CRITICAL;

    C_ENTER;

    d2d1_cenq(port,len);  // enque number of bytes in message
    while(len--)
      d2d1_cenq(port,*ptr++); // enque each byte
    
    d2d_switch_port_tx_irq_ctrl(port,1); // enable if not enabled
    d2d1_uart_tx_irq(port);

    C_LEAVE;
}


static UInt8 d2d1_cdeq(D2d1_Port *port)
{
    UInt8 c;

    c = port->txq[port->txnr & (D2D1_TXFIFO_SIZE-1)];
    port->txnr++;
    
   return c;
}

static void d2d1_cenq(D2d1_Port *port,UInt8 c)
{
    port->txq[port->txnw & (D2D1_TXFIFO_SIZE-1)] = c;
    port->txnw++;
}


/**
* @brief  d2d1 uart rx irq
* @param
* @return
*/
void d2d1_uart_rx_irq(D2d1_Port *port,UInt8 c)
{
    if(port->smsgbase.busstate == BUS_SLEEP)
    {
      port->smsgbase.busstate = BUS_MASTER;
      port->lastrx = c;  // let timer task decide if this command is recognized
      port_timer_rewind(port, PORT_TIMER_MS(1));
      return;
    }
    
    if(port->smsgbase.busstate == BUS_DATA)
    {
      port->rxbuf[port->rxofs] = c; // save byte
    
      if(++port->rxofs < port->rxlen)  // more to come?
      {
        port_timer_rewind(port, PORT_TIMER_MS(20));
        return;  // stay in data state
      }
    
      // we now have the full d2d command
      port->smsgbase.busstate = BUS_ENDMSG;
      port_timer_rewind(port, PORT_TIMER_MS(1)); // wake task
    }
}

/**
* @brief  d2d1 tx irq
* @param
* @return
*/
void d2d1_uart_tx_irq(D2d1_Port *port)
{
    UInt8 c;

    while(port->txnr != port->txnw)
    {
        if(port->txlen)
        {
          port->txlen--;
          c = d2d1_cdeq(port);
          d2d_switch_port_tx(port, c); // send the next byte
          return;
        }
        
        port->txlen = d2d1_cdeq(port);
    }
    d2d_switch_port_tx_irq_ctrl(port,0); // disable interrupt  
}

/**
* @brief  called by task
* @param
* @return
*/
void d2d1_msg_timeout_handler(D2d1_Port *port)
{
    switch(port->smsgbase.busstate)
    {
    case BUS_MASTER: // is this a command?
      if(d2d_cmd_enabled(port->lastrx) ==0)
      {// we ignore this byte
        port->smsgbase.busstate = BUS_SLEEP;
        break;
      }
    
      // else fall through
    case BUS_SLEEP: // we need to echo this byte
      if(port->rxmsg == 0)
      {
        port->rxmsg = smsg_alloc(port->smsgbase.number);
    
        if(port->rxmsg == 0)
        {
          port->smsgbase.busstate = BUS_SLEEP;
          port_timer_rewind(port, PORT_TIMER_MS(2)); // try again soon
          break; // still no buffer
        }
        port->rxbuf = smsg_getptr(port->rxmsg);
        port->rxbuf[0] = 0xFE - port->smsgbase.number;  //AID of D2D1 port: fake originator id for this port
        port->rxbuf[1] = PROTO_D2D; // raw d2d1 port is not an Auth source
      }
      port->rxbuf[2] = port->lastrx;  // write 1st byte
    
      d2d1_enqmsg(port, &port->lastrx,1);  // echo the byte as a 1 byte message
    
      port->rxofs = 3; // current offset for next char (if there is one)
    
      port->rxlen = 3; // TODO: increase this for multibyte command
      if(port->lastrx == REQ_DTC_CODE)
          port->rxlen++;
    
      if(port->rxofs < port->rxlen) // more to come?
      {
        port->smsgbase.busstate = BUS_DATA; // ready for next command byte from isr
    
        port_timer_rewind(port, PORT_TIMER_MS(200));
        break;
      }
      // fall through
    
    case BUS_ENDMSG: // last byte of message was received
      smsg_setlen(port->rxmsg,port->rxlen);
      smsg_submit(port->rxmsg);
    
      port->rxmsg = 0;  // buffer is gone
      port->smsgbase.busstate = BUS_SLEEP;
      break;
    
    
    case BUS_DATA: // timeout while receiving data?
      port->smsgbase.busstate = BUS_SLEEP;  // abandon this message but keep msg buffer
      break;
    }
}

/**
* @brief  send msg to hardware
* @param
* @return
*/
void d2d1_txmsg(D2d1_Port *port,Smsg msg)
{
    UInt8 *ptr;
    ptr = smsg_getptr(msg);
    
    if(d2d1_send_filter(port, msg))  //double check to be safe
    {
        d2d1_enqmsg(port, ptr+2, smsg_getlen(msg)-2);  //no need send aid of sender and proto_id
    }
    
    smsg_free(msg); // don't need this anymore
}

Boolean d2d1_send_filter(D2d1_Port *port,Smsg msg)
{
    UInt8 *ptr;
    ptr = smsg_getptr(msg);
  
    return(ptr[0] == 0xFE && (ptr[1] & ~PROTO_AUTH) == PROTO_D2D);
}




