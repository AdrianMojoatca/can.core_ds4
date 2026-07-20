/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d3p.h 32240 2015-06-12 12:31:31Z simon.demers $
/*==========================================================================*/

#ifndef __D2D3P_H__
#define __D2D3P_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os.h"
#include "d2d_code.h"
#include "d2d_switch.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define D2D3P_TXFIFO_SIZE (1<<6) // must be power of 2 size!
#define D2D3P_RXFIFO_SIZE (1<<6) // must be power of 2 size!
struct s_D2d3p_Port
{
  //smsgbase
  SmsgPort smsgbase;

  UInt8 txlen;
  UInt8 txnr;
  UInt8 txnw;
  UInt8 txq[D2D3P_TXFIFO_SIZE];

  Smsg rxmsg;
  UInt8 rxbuf[D2D3P_RXFIFO_SIZE];
  UInt8 rxlen;
  UInt8 rxofs;
  UInt8 lastrx;  // temporary holding buffer for 1st (unechoed) character
};

typedef struct s_D2d3p_Port D2d3p_Port;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void  d2d3p_init   (UInt8 port);

//void  d2d3p_txmsg(D2d3p_Port *port,Smsg msg);
void d2d3p_txmsg(UInt8 * msg, UInt8 len);                       
void d2d3p_msg_timeout_handler(D2d3p_Port *port);
void d2d3p_setmsg_handler_function(void (*function)(void *));

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern UInt8 d2d3p_port_no;
extern D2d3p_Port port3p;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

