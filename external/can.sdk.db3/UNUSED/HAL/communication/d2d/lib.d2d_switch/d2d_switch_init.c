/*! 
 * @file   d2d_switch_init.c
 * @brief  handle the d2d message between two or more UART port
 * author  DIRECTED ELECTRONICS
 * @date   7/15/2015
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

/**
* @brief initialize d2d switch
* @param
* @return
*/
void d2d_switch_init(void)
{
    Tsk tsk;
    UInt8 i;

    Switch_Notify_Msg *smsg_notify;

    smsg_notify_free = q_create();
    for(i = 0; i < SMSG_NOTIFY_Q_SIZE; i++)
    {
        smsg_notify = (Switch_Notify_Msg *)os_mem_request(sizeof(struct s_Switch_Notify_Msg));

        q_assign(smsg_notify_free, (Q_Msg*)smsg_notify);
        q_insert(smsg_notify_free, (Q_Msg*)smsg_notify);
    }
     smsg_notify_pending = q_create();

    smsg_freecnt = smsg_promise = 0;

    smsg_freelist = smsg_inputq = 0;

    for (i = 0; i < SMSG_MAX; i++)
        smsg_free(i + 1); // valid id is never zero

    for (i = 0; i < PORTS; i++)
        smsg_port[i] = NULL;

    tsk = os_tsk_create(d2d_switch_task, TASK_D2D_SWITCH_PRI, TASK_D2D_SWITCH_STK, 0, TASK_D2D_SWITCH_NAME);

    os_tsk_start(tsk, 0);
}
