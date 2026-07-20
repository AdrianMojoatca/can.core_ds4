/** 
 * @file   intl_port_private.h
 * @brief  
 * author  DIRECTED ELECTRONICS
 * @date   7/20/2015
 */

#ifndef __intl_port_private_h__
#define __intl_port_private_h__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "intl_port.h"
#include "d2d_switch.h"
#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define TASK_INTL_NAME  "intl_port"
#define TASK_INTL_PRI   10001
#define TASK_INT_STK   0x0220


#define AID_MAX        16  //?TBD
#define AID_MSG_LEN    1

//legacy d2d
#define D2D_Q_TIMEOUT  1000


#define INTL_EVT_TIMEOUT  100
enum
{
	INTL_EVT_NULL    ,
	INTL_EVT_RX_MSG  ,
	INTL_EVT_RX_AUTHFAIL  ,
};

enum
{
    INSTANT_D2D1,
    UPDATE_D2D1,
    INSTANT_SC,
    UPDATE_SC,
};

#define INTL_PORT_TXBUFSIZE 8

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void    intl_port_rx(Intl_Port *port, Smsg msg);

void d2d_adaptor_scpp(UInt8 *p_cmd, UInt8 len);
void d2d2_adaptor_ngss(UInt8 len, UInt8 *p_cmd );


/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern UInt8  aidbyte[AID_MAX];
extern Tsk tsk_intl_port;
extern Evt  intl_port_evt;

//d2d configure
extern D2D_Lut_Entry * d2d_lut_list  ;
#endif