/*==========================================================================*/
// $Id: wake.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef _WAKE_H_
#define _WAKE_H_

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "device.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef enum
{
	s_SLEEP,
	s_WAKE ,    
}Wake_Status ;

typedef void    (* Wake_Func )(Wake_Status);
typedef Boolean (* Wake_Ready)(void);

/*--------------------------------------------------------------------------*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void    wake_init          (void);
void    wake_enter         (void);
void    wake_leave         (void);

#define wake_enter_flagged(dummy) wake_enter()
#define wake_leave_flagged(dummy) wake_leave()

void    wake_ck_enter      (void);
void    wake_ck_leave      (void);
void    wake_notify_wake   (void);
void    wake_register      (Wake_Func  wake_func);
void    wake_register_ready(Wake_Ready wake_func);
UInt32  wake_tc            (void);
void    wake_set_timeout1  (UInt16 timeout_in_ms);
void    wake_set_timeout2  (UInt16 timeout_in_ms);
void 	wake_clock_init (void );

	void	crt0_clock_setup	(void);
	void	PLL_Lock_Wait			(void);
	void	crt0_pll_connect	(void);
	void	finish_clock_setup (void );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Boolean wake_going_sleep;
extern Boolean wake_by_gpio;
extern UInt8				 wake_uart_no;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/


#endif


