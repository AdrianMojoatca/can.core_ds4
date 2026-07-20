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

//Wake source definition, maxium 32
enum Wake_Flag_Enum
{
	WAKE_MAIN_TSK        = (1<<0),
	WAKE_PER_EXEC        = (1<<1),
	WAKE_CAN_V_USR       = (1<<2),
	WAKE_PRG_MENU        = (1<<3),
	WAKE_DEI_SS_TRG_HIST = (1<<4),
	WAKE_DEI_SS_SENS_ADJ = (1<<5),
	WAKE_SENSE_RS        = (1<<6),
	WAKE_DEI_RS          = (1<<7),
	WAKE_DEI_RS_SMART    = (1<<8),
	WAKE_SC_SET_MODE     = (1<<9),
	WAKE_SC_TX_TSK       = (1<<10),
	WAKE_SCPP_CMD        = (1<<11),
	WAKE_PRK_TSK         = (1<<12),
	WAKE_LED_USR         = (1<<13),
	WAKE_D2D_VALET       = (1<<14),
	WAKE_D2D2_MSG        = (1<<15),
	WAKE_SENS_PROTO_TX   = (1<<16),
	WAKE_OBD_CAN_TX      = (1<<17),
	WAKE_PRG_TSK         = (1<<18),
	WAKE_D2D_TSK         = (1<<19),
	WAKE_D2D3P_SENSE_TSK = (1<<20),

	WAKE_NOFLAG          = ((int)((unsigned)1<<31))  // last
	
};
typedef enum Wake_Flag_Enum Wake_Flag;

typedef void    (* Wake_Func )(Wake_Status);
typedef Boolean (* Wake_Ready)(void);

/*--------------------------------------------------------------------------*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void    wake_init          (void);
void    wake_enter         (void);
void    wake_leave         (void);
void    wake_enter_flagged (Wake_Flag flags);
void    wake_leave_flagged (Wake_Flag flags);
void    wake_ck_enter      (void);
void    wake_ck_leave      (void);
void    wake_notify_wake   (void);
void    wake_register      (Wake_Func  wake_func);
void    wake_register_ready(Wake_Ready wake_func);
UInt32  wake_tc            (void);
void    wake_set_timeout1  (UInt16 timeout_in_ms);
void    wake_set_timeout2  (UInt16 timeout_in_ms);
#if USE_TIMESTAMP == 1
void    wake_set_timeout( UInt16 timeout_in_ms );
void    wake_warning_register(Wake_Func func) ;
#endif
void 	wake_clock_init (void );
void  wake_sleep_soon (void);

	void	crt0_clock_setup	(void);
	void	PLL_Lock_Wait			(void);
	void	crt0_pll_connect	(void);
	void	finish_clock_setup (void );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Boolean wake_going_sleep;
extern UInt8				 wake_uart_no;

extern UInt8 wake_reason;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/


#endif


