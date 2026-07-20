/*! 
 * @file   d2d_switch_task.c
 * @brief  
 * @author  DIRECTED ELECTRONICS
 * @date   7/17/2015
 */

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2d_switch_private.h"
#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void smsg_switch(void);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
const UInt8 ham8[256] =   {
  0 ,1 ,1 ,2 ,1 ,2 ,2 ,3 ,1 ,2 ,2 ,3 ,2 ,3 ,3 ,4
 ,1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5
 ,1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5
 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6
 ,1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5
 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6
 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6
 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7
 ,1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5
 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6
 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6
 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7
 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6
 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7
 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7
 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,5 ,6 ,6 ,7 ,6 ,7 ,7 ,8
} ;

/**
* @brief  task of switch to handle message from uart ports 
*         task will dispatch the message to right destination 
* @param
* @return
*/
void d2d_switch_task(void * tsk_arg)
{
    Switch_Notify_Msg *n_msg;
      UInt8 i;

    for(; ;)
    {
        n_msg = (Switch_Notify_Msg *)q_remove(smsg_notify_pending, 1);

        if(n_msg)
        {
            if(smsg_inputq && smsg_inputq == n_msg->verify_msg)
                smsg_switch();

            q_return((Q_Msg *)n_msg);
        }
        else
        {
					  // process all port timers
            for(i = 0; i < PORTS; i++)
            {
                if(smsg_port[i] && smsg_port[i]->itimer) 
                {
									if(--(smsg_port[i]->itimer) == 0)
									{
                    if(smsg_port[i]->timeoutfunc)
                        (*smsg_port[i]->timeoutfunc)(smsg_port[i]);
									}
                }
            }
        }
   };
}

// do as much switch work as there is....
void smsg_switch(void)
{
    Smsg x, y, first;
    Smsg *duplist;
    UInt8 i;
    UInt8 len;
    UInt8 *sptr;
    UInt8 *dptr;
    UInt8  qty, dlist;

    duplist = &first;  // this is storage for list

    while ((x = smsg_deq(&smsg_inputq)) != 0)
    {
        len = smsg_getlen(x);
        sptr = smsg_getptr(x);
        first = x; // this msg is first on list

        dlist = smsg_dlist[x];
        qty =  ham8[dlist] - 1; //how many more copies of this msg do we need?
     
        // build the chain of duplicate messages
        for (i = 0; i < qty; i++)
        {
            y = smsg_freelist;
            smsg_freelist = smsg_next[y];
            smsg_freecnt--;
            smsg_promise--;

            smsg_meta[y] = smsg_meta[x]; // copy length informaiton
            dptr = smsg_getptr(y);

            memcpy(dptr, sptr, len); // copy data
            smsg_next[x] = y;
            x = y;
        }
        smsg_next[x] = 0;

        // now send to all ports except original src port
        for (i = 0; i<PORTS; i++)
        {
            if (dlist & 1 )   //send to this port?
            {
                y = smsg_deq(duplist);
                if (smsg_port[i] && smsg_port[i]->sendfunc)  //double check if we need
                    (*smsg_port[i]->sendfunc)(smsg_port[i], y);  // this can be optimized....
            }
            dlist >>= 1;
        }
    }
}


void port_timer_rewind(void *port, UInt16 amt)
{
	if(amt )
		amt++;
	((SmsgPort *)port)->itimer = amt;  // TODO: use different implementation for large values 
}
