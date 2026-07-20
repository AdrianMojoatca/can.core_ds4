/*==========================================================================*/
// $ d2d_switch_private.h  $
/*==========================================================================*/

#ifndef __D2D_SWITCH_PRIVATE_H__
#define __D2D_SWITCH_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2d_switch.h"
#include "q.h"
#include "wake.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

// <h>  Task 'd2d_switch' settings
// <s0> Name
// <o0> Priority           <0-65535>
// <i>  Lowest  priority = 0
// <i>  Highest priority = 65535
// <o1> Stack Size (bytes) <0x0000-0xFFFF:4>
// <i>  Must be a multiple of 4.
// </h>
#define TASK_D2D_SWITCH_NAME  "d2d_switch"
#define TASK_D2D_SWITCH_PRI   10001
#define TASK_D2D_SWITCH_STK   0x0200

//--------------------------------------------------------------------------//

//for switch task
typedef struct s_Switch_Notify_Msg
{
    Q_Msg base;
    Smsg  verify_msg;
} Switch_Notify_Msg;

#define SMSG_NOTIFY_Q_SIZE  10


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void d2d_switch_task                (void * tsk_arg);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Tsk  tsk_d2d_switch;

//extern Wake_Status d2d_switch_wake_status ;
//storage for message data
extern UInt8 smsg_ram[SMSG_MAX][SMSG_SIZE];

// metadata arrays
extern UInt8 smsg_dlist[SMSG_MAX + 1];
extern UInt8 smsg_meta[SMSG_MAX + 1]; // hold src port and msg length
extern UInt8 smsg_next[SMSG_MAX + 1]; // hold ??? and msg successor

extern Smsg smsg_freelist; // first free buffer
extern Smsg smsg_freecnt;  // how many buffers free?
extern Smsg smsg_promise;  // how many buffers promised?

extern Smsg smsg_inputq;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

