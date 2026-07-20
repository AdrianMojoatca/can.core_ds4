/*! 
 * @file   d2d_switch_handler.c
 * @brief  
 * @author  DIRECTED ELECTRONICS
 * @date   7/27/2015
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


// message metadata routines....

// get length of message
UInt8 smsg_getlen(Smsg id)
{
    return smsg_meta[id] & (SMSG_SIZE - 1);
}

// set length of message
void smsg_setlen(Smsg id, UInt8 len)
{
    smsg_meta[id] = (smsg_meta[id] & ~(SMSG_SIZE - 1)) | (len & (SMSG_SIZE - 1));
}

// get source port
UInt8 smsg_getport(Smsg id)
{
    return smsg_meta[id] >> SMSG_LG2SIZE;
}

// get pointer to message data
UInt8 *smsg_getptr(Smsg id)
{
    return smsg_ram[id - 1];
}

// put buffer back onto freelist
void smsg_free(Smsg id)
{
    ATOMIC(
        smsg_next[id] = smsg_freelist;
        smsg_freelist = id;
        smsg_freecnt++;
    )
}

// NON-BLOCKING try to get a new message buffer for srcport.  returns 0 on failure, else a message id
Smsg smsg_alloc(UInt8 srcportnumber)
{
    Smsg x = 0;
    UInt8 need_msg = 0;
    UInt8 i;
  
    for(i=0; i < PORTS; i++)
    {
      if(i != srcportnumber && smsg_port[i] && smsg_port[i]->sendfunc)
        need_msg++;
    }
  
    ATOMIC(
        if (smsg_freecnt - smsg_promise > need_msg)
        {// safe to allocate
            x = smsg_freelist;
            smsg_freelist = smsg_next[x];
            smsg_freecnt--;
            smsg_promise += need_msg - 1;
        }
    )

    if(x)
    {
        smsg_meta[x] = srcportnumber << SMSG_LG2SIZE; // store srcport and set length = 0,
    }

    return x;
}

// enq and deq...

// enque outbound and inbound messages
void smsg_enq(Smsg *q, Smsg id)
{
    Smsg p, x;

    if(id)
    {
        smsg_next[id] = 0; // nothing follows this one.

        p = 0;
        ATOMIC
        (
        for (x = *q; x; p = x, x = smsg_next[x])
            ;  // find end of list

        if (p)
            smsg_next[p] = id; // previous points to this
        else
            *q = id; // (no previous) this is first on list
        )
    }
}

// remove first msg on queue, else return 0
Smsg smsg_deq(Smsg *q)
{
    Smsg x;
    ATOMIC
    (
        x = *q;
        if(x)
            *q = smsg_next[x];
    )
        return x;
}

void smsg_submit(Smsg msg)
{
    Switch_Notify_Msg *notify_msg;

    UInt8 i, src;
    src = (smsg_meta[msg] >> SMSG_LG2SIZE) & ((1<<(8 - SMSG_LG2SIZE))-1);
    smsg_dlist[msg] = 0;
    for(i=0; i < PORTS; i++)
    {
      if( smsg_port[i]
          && smsg_port[i]->sendfunc
          && i != src      
        )
       {
          if( smsg_port[i]->filterfunc
            && ((*smsg_port[i]->filterfunc)(smsg_port[i], msg) == 0)
            )
             smsg_promise--;
          else
            smsg_dlist[msg] |= (1<<i);
       }
    }
  
    smsg_enq(&smsg_inputq, msg);

    notify_msg = (Switch_Notify_Msg *)q_remove(smsg_notify_free, D2D_SWITCH_Q_TIMEOUT);
    notify_msg->verify_msg = smsg_inputq;
    q_insert(smsg_notify_pending, (Q_Msg *)notify_msg);
}

UInt8 smsg_register(UInt8 port, void *pstruct)
{
    UInt8 i;
    for (i = port; i < PORTS; i++)
    {
        if (smsg_port[i] == NULL)
        {
            smsg_port[i] = (SmsgPort *)pstruct;
            return i;
        }
    }
    return 0xFF; // error
}

