/*! 
 * @file   d2d2_msg_handler.c
 * @brief  
 * @author  DIRECTED ELECTRONICS
 * @date   7/20/2015
 */

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2d2_private.h"

#include "crc.h"

#include "status_system.h"  //set_temp_sensor_present/value
#include "d2d.h"
#include "bootservice.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define IS_CHAR_SPECIAL(c)     ((UInt8)((c)+1)<4)
#define FRAMING_ERROR_MASK     ((UInt8)-1)
#define IS_FRAMING_ERROR(sts)  ((sts) & FRAMING_ERROR_MASK)

//#define SLOW_ENDMSG
#ifdef SLOW_ENDMSG
#define DELAY_ENDMSG  PORT_TIMER_MS(10)
#else //production
#define DELAY_ENDMSG  PORT_TIMER_MS(2)  // was 3
#endif
#define DELAY_GIVECHANCE PORT_TIMER_MS(4)  // was 10

#define DELAY_BTWEEN_BYTES PORT_TIMER_MS(2)
#define DELAY_WAKEUP       PORT_TIMER_MS(10)

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void crc_update(UInt16 *crcptr, UInt8 b);
static void feedback_frame_txmsg(D2d2_Port *port, UInt16 input_code);

UInt8 alloc_d2d2port_rxmsg(D2d2_Port *port);

void timeout_to_data(D2d2_Port *port);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
static void udelay(UInt32 microseconds)
{
UInt32 tbegin = T2TC;	
	 while(T2TC - tbegin < microseconds)
		 ;
}


void send_open(D2d2_Port *port)
{
    port->smsgbase.busstate = BUS_OPEN;
    d2d_switch_port_tx(port, CHAR_OPEN); // announce bus open
		// bus is open
		
		udelay(10); // be sure char shifted out before proceeding (max: 8.7 us delay)	
}

void to_master_delay(D2d2_Port *port)
{
    port->smsgbase.busstate = BUS_MASTER;
      d2d_switch_port_tx(port, CHAR_ERROR); // announce master state
      port_timer_rewind(port, PORT_TIMER_MS(10));  // wait before opening
}


static UInt8 random_byte(void)
{
static UInt16 seed;
    if(seed == 0)
        seed = 0xabcd; // anything, really...
    crc_update(&seed,0);
    return seed & 0xFF;
}


void to_open(D2d2_Port *port)
{
    // make sure we really want to open the bus
    if (hold_bus_off) // user holding button for possible hard reset?
    {
        //itimer(port, RT_MILLI(1));
        port_timer_rewind(port, PORT_TIMER_MS(2));  // wait longer before opening
        return;
    }

		send_open(port);
				
		if(!port->rxmsg)
		{// special case: bus is now open but no buffers are available for receive!

			if(port->txpending) // do I have something to send?
				to_data(port); // send now in order to dispose of some buffers
			else // we have nothing available to send!
				to_master_delay(port); // just wait, maybe some other port will clear.

			return;
		}
		// bus is open and buffers are available
		    
    to_data(port); // maybe seize the bus if we have something to send
}

void to_master(D2d2_Port *port)
{
    port->smsgbase.busstate = BUS_MASTER;
    d2d_switch_port_tx(port, CHAR_ERROR); // announce master state
    
        if( port->smsgbase.pre_wake == 0 )
        {
            port_timer_rewind(port, PORT_TIMER_MS(5));  // wait before opening
            return;
        }

    to_open(port);
}

// try to send data, otherwise start inactivity timer
void to_data(D2d2_Port *port)
{
    // ASSERT(port->smsgbase.busstate == BUS_OPEN);  // we know it is already open
    if (port->txtries >= D2D2_TXTRIES_LIMIT)
    {
        //itimer(port, RT_MILLI(1)); // timeout needs to clean up
        port_timer_rewind(port, 1); // asap!
        return;
    }

		

    if (port->txpending) // do I have a message to send?
    {
			if (port->givechance && port->rxmsg) // do I have to give chance
			{
				port->givechance = 0;
					//itimer(port,RT_MILLI(3));
					port_timer_rewind(port, PORT_TIMER_MS(DELAY_GIVECHANCE + (random_byte() & 3))); // 10 to 13 ms variable delay
					return;
			}
			
			
        port->txbuf = smsg_getptr(port->txpending);
        port->txlen = smsg_getlen(port->txpending);

        port->smsgbase.busstate = BUS_TXDATA;

        //enable tx interrupt
        port->txofs = 0;
//        d2d_switch_port_tx(port, port->txbuf[0]);  //Transmit interrupt does right way
//        port_timer_rewind(port, PORT_TIMER_MS(2));
        d2d_switch_port_tx_irq_ctrl(port, 1);
        d2d2_uart_tx_irq(port);
        port_timer_rewind(port, 0);
        return;
    }
    //itimer(port,20 * RT_SECOND);
    port_timer_rewind(port, PORT_TIMER_MS(50));
}

void timeout_to_data(D2d2_Port *port)
{
    CRITICAL;
    
    C_ENTER;
    if(port->smsgbase.busstate == BUS_OPEN && port->txpending)
    {
        to_data(port);
    }
    C_LEAVE;
}

//---------------------------------------------------//
#define CRC_PRELOAD ((U16)-1)
#define CRC_POLY 0x1021
void crc_update(UInt16 *pcrc, UInt8 b)
{
  UInt8 i;
  UInt16 crc;
    
    crc = *pcrc;
    for (i = 0; i < 8; i++)
    {
        if (crc & 0x8000)
            crc = (crc << 1) ^ CRC_POLY;
        else
            crc <<= 1;
    }
    *pcrc = crc ^ b;
}


void rxbyte(D2d2_Port *port, UInt8 c)
{
    if(port->rxlen < SMSG_SIZE)
    {
      port->rxbuf[port->rxlen] = c;
    }
    if(++port->rxlen == 0)
    {
      port->rxlen = 0xFF; // flag that buffer missed at least one byte
    }
    crc_update(&port->crc, c);        
    d2d_switch_port_tx(port, ((port->smsgbase.busstate==BUS_SHIFT || port->rxlen == 1 ) ? c : c^CHAR_SHIFT_AMOUNT));
}


UInt8 dbgbuf[32-2-2];

/**
* @brief  void check_dbg_msg(void)
* @param
* @return
*/
void check_dbg(void)
{
    UInt8 i;

    if(dbg_dest)
    {
        if(dbgclitx_avail() == 0 )  //sent all trace data already
        {
          return;
        }
      
        dbgbuf[0] = dbg_dest;

        for(i=3-2; dbgclitx_avail() && i<32-2-2-2;i++)   //fetching trace data in 26 bytes packet
        {
          dbgbuf[i] = get_dbgtx_fifo();
        }
        if(i > 3-2)
           proto_sendmsg(PROTO_DBG,dbgbuf,i);
    }
}

//FIXME: get correct length
unsigned have_txmsg(D2d2_Port *port)
{
    if(port->txofs == port->txlen)  // same?
        return 0;        // means empty txqueue

    //rutx.buf = txbufmem + rutx.nr * U0TX_BUFSIZE; // get next buffer to send

    return (port->txlen);
}

Boolean bus_avail_traceout(D2d2_Port *port)
{
  Boolean ret=FALSE;
    if(!have_txmsg(port))
    {
      check_dbg();
      if(have_txmsg(port))
        ret = TRUE;
      else
        ret = FALSE;
    }
    
    return ret;
}

void d2d2_msg_timeout_handler(D2d2_Port *port)
{
    CRITICAL;

      UInt8 i;
      UInt8 port_lsr;
    
    C_ENTER;

    switch (port->smsgbase.busstate)
    {
    case BUS_ENDMSG:
        // no one complained about slave message which was >= 4 bytes long
        port->smsgbase.busstate = BUS_MASTER; // silently change to master state
        C_LEAVE;

        if (port->rxlen <= SMSG_SIZE)
        {// small enough for me
            port->rxlen -= sizeof(UInt16); // cut crc
            smsg_setlen(port->rxmsg, port->rxlen);
            smsg_submit(port->rxmsg);
            alloc_d2d2port_rxmsg(port);
        }
        // else good crc message was too long for me, so just silently drop it otherwise they will retransmit!!!!!
        to_open(port);
        break;

    case BUS_TXENDMSG:
        // no one complained about my message
        port->smsgbase.busstate = BUS_MASTER; // silently change to master state
        d2d_switch_port_tx_irq_ctrl(port, 0);
        C_LEAVE;
        smsg_free(smsg_deq(&port->txpending)); // return this buffer back to switch
        port->txtries = 0;
		
		    if(port->txpending) // do I have something else to send?
				{
					send_open(port); // tell everyone bus is open
					to_master_delay(port); // then immediately re-close it and delay some more
				}
				else
					to_open(port); // announce bus is open for real
        break;

    case BUS_OPEN:
        if(port->txtries >= D2D2_TXTRIES_LIMIT)
        {// txpending message is undelivable
          C_LEAVE;
          port->txtries = 0;
          smsg_free(smsg_deq(&port->txpending));
          port_timer_rewind(port, 1*PORT_TIMER_SECOND);
          if(port->txpending)
              timeout_to_data(port);
          
          break;
        }
                
        if(port->txpending)
        {
          C_LEAVE;
          timeout_to_data(port);
          break;
        }
        
        // otherwise timeout in BUS_OPEN without givechance and no txpending means time to sleep
        if(bus_avail_traceout(port))
        {
					C_LEAVE;
          port_timer_rewind(port, PORT_TIMER_MS(20));
          break;
        }
        
        if(port->smsgbase.pre_wake)
        {
            port->smsgbase.pre_wake = 0;
            wake_leave_flagged(WAKE_D2D2_MSG);
        }
        C_LEAVE;
        break;

    case BUS_MASTER:
        // clean up any stale txmsgs
        C_LEAVE;

        if (port->txtries >= D2D2_TXTRIES_LIMIT)
        {
            port->txtries = 0;
            smsg_free(smsg_deq(&port->txpending)); // return this buffer back to switch
        }

        // try to get an rxmsg if needed
        if (port->rxmsg == 0)
            alloc_d2d2port_rxmsg(port);

        to_open(port); // try to open now
        break;

//    case BUS_MASTER_DELAY:
//        C_LEAVE;
//        port->givechance = 1;
//        to_open(port); // try to open now
//    
//        break;
    
    case BUS_SLEEP:
        C_LEAVE;
            
        break;
            
    case BUS_MAINT:
        C_LEAVE;
        if((get_ss_arm() || get_ss_lock()))  //not allowed if system is armed
        {
            to_master(port);
            break;
        }

        if (port->rxmsg == 0)
        {
           if(alloc_d2d2port_rxmsg(port) == 0)
           {
               port_timer_rewind(port, 1);
               break;
           }
        }
        port->rxlen = 0;   //create spec message to announce maintenance mode
        port->rxbuf[port->rxlen++] = 0xFE - port->smsgbase.number;
        port->rxbuf[port->rxlen++] = PROTO_INTL;
        port->rxbuf[port->rxlen++] = 0xEB; //new SC command
        smsg_setlen(port->rxmsg, port->rxlen);
        smsg_submit(port->rxmsg);
        break;

    case BUS_TXDATA:
        if(port->txofs == 0)
        {// transmit isr did not start?!?
            //dbg_never++;
            d2d_switch_port_tx(port,port->txbuf[port->txofs++]);  // explicitly send 1st char in case irq never started
            port_timer_rewind(port,PORT_TIMER_MS(2));
            break;
        }
        // fall through
    default:
        port->smsgbase.busstate = BUS_MASTER;
        C_LEAVE;
        to_master(port);
        break;
    }
}

// d2d master logic
// master RX
void d2d2_uart_rx_irq(D2d2_Port *port, UInt8 c)
{
    UInt8 i;

    switch(port->smsgbase.busstate)
    {
    case BUS_OPEN:
       if(c >= 0x03 && c < 0xFB )  //0xFB and above are reserved for UART0/1/3
       {
           //rewind timer
           port_timer_rewind(port, DELAY_BTWEEN_BYTES);  //2ms enough for slave start to talk
           port->smsgbase.busstate = BUS_DATA;
           port->givechance = 1;
           port->rxlen = 0;
           port->crc = CRC_CCITT_PRELOAD;

           if(!port->smsgbase.pre_wake)
           {
               port->smsgbase.pre_wake =1;
               wake_enter_flagged(WAKE_D2D2_MSG);
           }
           rxbyte(port, c);
       }
       else
           to_master_delay(port);
               
       break;
    
    case BUS_DATA:
       if(c==CHAR_ENDMSG)
       {
           if(port->crc) // || port->rxlen >= D2D2_RXLEN_MAX-1)
           {// reject this message
             to_master(port);
           }
           else    //receive message
           {
             port->smsgbase.busstate = BUS_ENDMSG;
                             
             d2d_switch_port_tx(port, c);
             port->smsgbase.busstate = BUS_ENDMSG;
             //rewind 2ms timer
             port_timer_rewind(port, DELAY_ENDMSG);  //May IVU ask longer than 2ms? Seems IVU could not receive all shortly.
           }
           break;
       }
       if(c==CHAR_SHIFT)
       {
           port_timer_rewind(port,DELAY_BTWEEN_BYTES);
           d2d_switch_port_tx(port, c);
           port->smsgbase.busstate = BUS_SHIFT;
           break;
       }
       
       if(IS_CHAR_SPECIAL(c))
       {
           to_master_delay(port);
           break;
       }
       			 
       port_timer_rewind(port,DELAY_BTWEEN_BYTES);
             
       if(port->rxlen == 0xFF)
       {
         to_master_delay(port);
       }
       else
         rxbyte(port,c ^ CHAR_SHIFT_AMOUNT);

       break;

    case BUS_SHIFT:
        rxbyte(port,c); // store data byte value (but echo received char!)
        port->smsgbase.busstate = BUS_DATA;
        port_timer_rewind(port, DELAY_BTWEEN_BYTES);
        break;
    
    case BUS_MAINT:
//        case BUS_MASTER_DELAY:

        break;

    case BUS_SLEEP:
        port->givechance = 1;
		    to_master_delay(port);
        break;
		
    default:
        if(port->smsgbase.busstate >= BUS_TXDATA)
        {
            port->txtries++;
            port->givechance = 0;
        }
        if(port->smsgbase.busstate != BUS_MASTER)
            to_master_delay(port);

        break;
    }
}

//master TX

/**
 *Function Name:d2d2_uart_tx_irq(D2d2_Port *port)
 *Parameters:
 *Description:
 *Returns:
-------------------------------------------------------------*/
void d2d2_uart_tx_irq(D2d2_Port *port)
{
    UInt8 c;

    switch(port->smsgbase.busstate)
    {
    case BUS_TXSHIFT:
        port->smsgbase.busstate = BUS_TXDATA;
        d2d_switch_port_tx(port, port->txbuf[port->txofs]);
        port->txofs++;
        break;

    case BUS_TXDATA:
        if(port->txofs >= port->txlen)
        {
            //rewind 2ms timer
            port_timer_rewind(port, DELAY_ENDMSG);
            port->smsgbase.busstate = BUS_TXENDMSG;
            c = CHAR_ENDMSG;
        }
        else
        {
            c = port->txbuf[port->txofs];
            if(port->txofs)
                  c ^= CHAR_SHIFT_AMOUNT;    //All bytes shifted except the first (ID)
              
            if(IS_CHAR_SPECIAL(c))
            {
                port->smsgbase.busstate = BUS_TXSHIFT;
                c = CHAR_SHIFT;
            }
            else
                port->txofs++;
        }
        d2d_switch_port_tx(port, c);

        break;

    default:
        d2d_switch_port_tx_irq_ctrl(port, 0);
        break;
    }
}


void append_crc(Smsg msg)
{
    UInt8 *ptr;
    UInt8 len, i;
    UInt16 crc;

    ptr = smsg_getptr(msg);
    len = smsg_getlen(msg);

    ptr[len] = 0;
    ptr[len+1] = 0;

    len += sizeof(UInt16);

    crc = CRC_CCITT_PRELOAD;
    for (i = 0; i<len; i++)
        crc_update(&crc, ptr[i]);


    ptr[len - 2] = crc >> 8;
    ptr[len - 1] = crc & 0xFF;
    
    smsg_setlen(msg, len);
}

/**
 *Function Name: void d2d2_txmsg(D2d2_Port *port, Smsg txmsg)
 *Parameters:
 *Description:
 *Returns:

  This is a callback function running on the switch thread.
  You are not allowed to block, or request buffers.
  You must either smsg_free() the buffer immediately,
  or else send it to something else that will.
*/
void d2d2_txmsg(D2d2_Port *port, Smsg txmsg)
{
      UInt8 i;
      UInt8 *ptr;
      CRITICAL;
    
      ptr = smsg_getptr(txmsg);
    
      if(ptr[1] == PROTO_INTL)
        {
            if(  ptr[2] == 0xD8 ) //maintenance here
            {
                port->smsgbase.busstate = BUS_MAINT;
                port_timer_rewind(port, 0);
                d2d_switch_port_tx(port, CHAR_ERROR);
                for(i=0; i<10; i++)
                     d2d_switch_port_tx(port, CHAR_MAINTENANCE);
            }
            
            smsg_free(txmsg);
            return;
        }

        C_ENTER;
        if(!port->smsgbase.pre_wake)
        {
            port->smsgbase.pre_wake = 1;
            wake_enter_flagged(WAKE_D2D2_MSG);
        }
        C_LEAVE;
        append_crc(txmsg);
    smsg_enq(&port->txpending, txmsg);      // send it to task

		switch(port->smsgbase.busstate)
		{
		case BUS_SLEEP:
			  // to_master() but open after a 10 ms delay
		    to_master_delay(port);
		    break;
		
    case BUS_OPEN:
				port_timer_rewind(port, PORT_TIMER_MS(1));
			  break;
		
		case BUS_MASTER:
			  i = ((SmsgPort *)port)->itimer; // cheat: peek at remaining time
				if(i==0 || i > PORT_TIMER_MS(10))  // off or too long?
					port_timer_rewind(port, PORT_TIMER_MS(10)); // cut waiting time to this.
				break;
			  
		// not sure about these, but fall through
		// case BUS_MAINT:
		// case BUS_MAINT_END:
			  // fall through
		default: // receiving or transmitting a frame, don't touch port timer. let state machine solve things
			  break;
		}
}

UInt8 alloc_d2d2port_rxmsg(D2d2_Port *port)
{
    if ((port->rxmsg = smsg_alloc(port->smsgbase.number)) != 0)
        port->rxbuf = smsg_getptr(port->rxmsg);
    else
        port->rxbuf = NULL; // no msg means no buffer pointer
        
        return (port->rxmsg);
}

