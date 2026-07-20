/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: cmd_task.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "cmd_private.h"
#include "wdog_v.h"
#include "bus_status.h"
#include "intl_port.h"
#include "cmd.h"

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

/*--------------------------------------------------------------------------*/

#define _HI(x) ((UInt8)((x)>>8))
#define _LO(x) ((UInt8)(x))

#define CMDQ_PKT_FULL 32-4-4

UInt8 cmdq_pkt[CMDQ_PKT_FULL];
UInt8 cmdq_pkt_len = 0;


struct cssos_timer *cmdq_timer;
#define CMDQ_LATENCY_DELAY RT_MILLI(50)

unsigned cmdq_lostcount = 0;

unsigned cmdq_bytes_len = 0;
unsigned cmdq_tx_enable = 0;  // default is not enabled


#define CMD_MASK 511

/////////////// input
UInt8 cmdq_input_bytes[16];
UInt8 cmdq_input_len = 0;

void cmdq_process(UInt8 *pkt, UInt8 len)
{
	unsigned w;
	unsigned i,wi;
	
	if(pkt[0] >= 0xF8  || pkt[0] < 0x20)
		return;  // not a vaild source
	
	// TODO: make sure this was sent with Auth
	
	if(len < 4)  // no input tokens?
	{
		cmdq_tx_enable = pkt[2];  // 0=off, 1=on
		return;
	}
	
	cmdq_tx_enable = 1;  // make sure it is on for dialog
	
	for(i=2;i<len;i+=2)
	{
	
		w = pkt[i];
		w = (w<<8) + pkt[i+1];
		wi = w & CMD_MASK;  // just get the token
		if(wi  == CMDQ_BYTES(0))
		{// TODO...   inprove later
			
			cmdq_input_len = w>>12;  // 0 to 15 only
			memcpy(cmdq_input_bytes,pkt+i+2,cmdq_input_len); // copy data
			i+=cmdq_input_len;
			if(cmdq_input_len & 1)
				i++;  // skip padding
			continue;
		}
		
		if(wi == INPUT_LOSTCOUNT)
			continue;
		
		cmd_q_insert((Cmd)w,COMMAND_INSERT_TIMEOUT);  // insert input token into our cmd_q_table
	}
}





/////////////// output

static void cmdq_flush(void)
{
  proto_sendmsg(PROTO_CMDQ + PROTO_AUTH,(void *)cmdq_pkt,cmdq_pkt_len);  // we prechecked that won't get stuck
	cmdq_pkt_len = 0;
}

void cmdq_bcast(UInt16 cmd)
{	
	cmdq_pkt[cmdq_pkt_len++] = _HI(cmd);
	cmdq_pkt[cmdq_pkt_len++] = _LO(cmd);
	
	if(cmdq_pkt_len == 2)
	{
		if(cmdq_lostcount)
		{
			// show how many we lost
      cmdq_pkt[0] = _HI(CMDQ_BYTES(2));  // sizeof associated data
      cmdq_pkt[1] = _LO(CMDQ_BYTES(2));  // invalid command is how we include data for the next code
			cmdq_pkt[2] = _HI(cmdq_lostcount);   // show how many tokens were lost 
			cmdq_pkt[3] = _LO(cmdq_lostcount);
			
			// the included data was associated with this code
			cmdq_pkt[4] = _HI(INPUT_LOSTCOUNT);
			cmdq_pkt[5] = _LO(INPUT_LOSTCOUNT);
			
			// here' is the first input AFTER the lost data
			cmdq_pkt[6] = _HI(cmd);
			cmdq_pkt[7] = _LO(cmd);
			
			cmdq_pkt_len = 8;
			
			cmdq_lostcount = 0;  // no more lost data
		}
		
		cssos_timer_rewind(cmdq_timer,CMDQ_LATENCY_DELAY);
	}
	else if(cmdq_pkt_len >= CMDQ_PKT_FULL)
	{
		if(proto_sendmsg_wouldblock())
		{
			if(  cmdq_pkt[5] == _LO(INPUT_LOSTCOUNT) && cmdq_pkt[1] == _LO(CMDQ_BYTES(2))
				&& cmdq_pkt[4] == _HI(INPUT_LOSTCOUNT) && cmdq_pkt[0] == _HI(CMDQ_BYTES(2))
				)
			{// we are dropping a packet which contains the number of previously-dropped input tokens.
				cmdq_lostcount = cmdq_pkt[3] + (cmdq_pkt[2]<<8);  // re-read how many were dropped
				cmdq_lostcount += (cmdq_pkt_len-6)>>1;  // add the number input tokens we are dropping now
			}
			else
			{// we have to drop all these input tokens
				cmdq_lostcount += cmdq_pkt_len>>1;  // # of cmdq input tokens lost is # of bytes / 2
			}
			
			cmdq_pkt_len = 0;  // start filling next packet
		}
		else
		{// TODO: encrypt using broadcast stream key
			cmdq_flush();
		}
	}
	
}


void cmdq_timeout(void)
{
	if(cmdq_pkt_len)
	{
		if(proto_sendmsg_wouldblock())
		{// better to restart timer than lose data
			cssos_timer_rewind(cmdq_timer,CMDQ_LATENCY_DELAY);			
		}
		else
			cmdq_flush();
	}
}



void cmd_task (void * arg)
{
	unsigned cmd;
	
	cmdq_timer = cssos_timer_create(cmdq_timeout);  // create packetization timer for cmdq packets
	
	
	for (;;)
    {  
		Cmd_Q_Msg *cmd_q_msg     = (Cmd_Q_Msg *) q_remove (cmd_q_pending , 0) ;		
			
			  cmd = cmd_q_msg->cmd;
			
			  if((cmd & CMD_MASK) == CMDQ_BYTES(0))  // should never happen
				{
					cmdq_bytes_len = cmd>>12;  // how many bytes are here?
					// TODO: get this number of inserted data bytes
					
					continue; // pick up next command
				}
				if(cmd == INPUT_LOSTCOUNT)  // should never happen
				{
					continue; // nothing to do...
				}
				
				if(cmdq_tx_enable) // trace enabled?
				{
					cmdq_bcast(cmd); // trace out
				}

        VIRTUAL_WATCH
        (
            reg_q_exec_func( cmd_exec_table , (Reg_Id)cmd_q_msg->cmd , (Reg_Arg)cmd_q_msg->param );
        		
            cmd_trace(cmd_q_msg->cmd);
                		
    		q_return( (Q_Msg *)cmd_q_msg ) ; 		
        )        
    }
}

/*--------------------------------------------------------------------------*/

