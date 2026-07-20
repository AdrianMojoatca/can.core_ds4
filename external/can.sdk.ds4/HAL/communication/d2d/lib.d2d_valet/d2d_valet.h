/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d1.h 32240 2015-06-12 12:31:31Z simon.demers $
/*==========================================================================*/

#ifndef __D2D_VALET_H__
#define __D2D_VALET_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os.h"
#include "d2d_code.h"
#include "d2d_switch.h"
#include "intl_port.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define D2D_VALET_TXFIFO_SIZE (1<<6) // must be power of 2 size!



typedef struct s_D2d_Valet_Port D2d_Valet_Port;


struct s_D2d_Valet_Port
{
  //smsgbase
  SmsgPort smsgbase;

	Gpio_Pl_T butn_pin_label;
	Gpio_Pl_T led_pin_label;
	
  UInt8 txlen;
  UInt8 txnr;
  UInt8 txnw;
  UInt8 txq[4];
	
  Smsg rxmsg;
  UInt8 *rxbuf;
  UInt8 rxlen;
  UInt8 rxofs;
  UInt8 lastrx;  // temporary holding buffer for 1st (unechoed) character

};

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void  d2d_valet_init    (void);
void  d2d_valet_pre_init(void);

Boolean d2d_valet_filter(D2d_Valet_Port *port,Smsg msg);
void  d2d_valet_txmsg(D2d_Valet_Port *port,Smsg msg);

void  d2d_led_msg_timeout_handler( Intl_Port *port);

void d2d_valet_reset_request(void);

;/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern UInt8 d2d_valet_port_no;
extern Boolean d2d_valet_debounce_state;
extern UInt8 d2d_valet_butn_state;
extern UInt8 d2d_valet_butn_tap_cnt;
extern UInt8 d2d_valet_led_pattern;
extern UInt8 d2d_valet_led_flash_cnt;
extern UInt8 d2d_valet_led_state;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

