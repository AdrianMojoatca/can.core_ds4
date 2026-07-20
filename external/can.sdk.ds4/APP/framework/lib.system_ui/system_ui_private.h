/*==========================================================================*/
// $Id: dei_sc_private.h 33405 2015-08-26 21:21:53Z suchita.yadav $
/*==========================================================================*/

#ifndef __MENU_PRIVATE_H_
#define __MENU_PRIVATE_H_

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "wake.h"
#include "timeout.h"
#include "q.h"
#include "nvfs_usr.h"
#include "nvfs.h"  
#include "trace.h"
#include "cmd.h"
#include "notify.h"
#include "stdarg.h"
#include "vrtc_f.h"
#include "wdog_v.h"
#include "d2d.h"
#include "intl_port.h"
#include "sensor_proto.h"
#include "system_ui.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
typedef enum{
	NOT_DETECTED,
	IGN_FIRST,
	BUTN_PRESS_FIRST
}Ign_Btn_State;

//--------------------------------------------------------------------------//
#ifndef DEBUG_SYS_UI
#define DEBUG_SYS_UI			0
#endif

#if DEBUG_SYS_UI == 1
#define TRACE_SYS_UI			TRACE
#else
#define TRACE_SYS_UI(...)		;
#endif

//--------------------------------------------------------------------------//

// state number defined on the basis of number of butn press and ign transition count
#define HOLD_5_SEC_STATE			0x01
#define TAP_HOLD_STATE				0x03
#define IGN_ON_STATE					0x10
#define TACH_LEARN_STATE			0x91
#define TACH_LEARN_DONE				0x92 //0x12
#define TACH_LEARN_DONE_SECOND	0xA2
#define OVERRIDE_STATE				( 0x90 | (dei_feature_get( DEI_FEAT_VALET_SWITCH_PULSE_COUNT ) * 2) )
#define REMOTE_PAIRING_STATE	0x93	//0x13
#define TRIG_HIST_STATE				0x14
#define REMOTE_DELETE_STATE		0x95	//0x15
#define ZAP_STATE							0x97	//0x17
#define TACH_DELETE_STATE			0x99	//0x19
#define RS_VALET_STATE				0xA1	//0x21
#define ALARM_VALET_STATE			0xA2	//0x22
#define RS_SHUT_DOWN_STATE		0x24

#define TAP_HOLD_STATE							0x03
#define QUICK_TAP_HOLD_STATE				0x04
#define TWO_TAP_HOLD_STATE					0x05
#define QUICK_TWO_TAP_HOLD_STATE		0x06
#define THREE_TAP_HOLD_STATE				0x07
#define QUICK_THREE_TAP_HOLD_STATE	0x08

#define BUTN_PRESS_TIMEOUT			VRTC_1SEC*5
#define PRG_MENU_EXIT						VRTC_1SEC
#define TACH_LEARN_TIMEOUT			VRTC_1SEC*20
#define PRG_MENU_TIMEOUT				VRTC_1SEC*60
#define PRG_MENU_NEXT_TIMEOUT		VRTC_1SEC*15
#define HHU_ALARM_TRIG_TIMEOUT	VRTC_1SEC*5
#define RS_VALET_TIMEOUT				VRTC_1SEC*5

#define BUTN_PRESS_TIMESTAMP(x) 	(UInt32)( *cmd_get_pld(x) )
//--------------------------------------------------------------------------//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void system_ui_menu_select( void );
void system_ui_reset( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern UInt8    system_ui_sel_func;
extern UInt8 		system_ui_state;
extern UInt8   	system_ui_first_action;
extern UInt8 		system_ui_ign_cnt;
extern UInt8 		system_ui_cmd;
extern UInt8		system_ui_btn_cnt;
extern Vrtc			system_ui_vrtc;
extern Vrtc			system_ui_cmd_vrtc	;	
extern UInt32 	system_ui_tstamp_ign;
extern UInt32 	system_ui_tstamp_curr;
extern Boolean system_ui_program_menu;
extern Boolean system_ui_hold_timeout;
extern Boolean system_ui_hold_status;

extern Boolean system_ui_flg_ign;

extern Vrtc system_ui_ble_evt_vrtc;
extern UInt8 system_in_used_status;
extern UInt8 tap_while_ign_on;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
void system_ui_execute(void);
void system_ui_cmd_execute(void);
void system_ui_butn_release(void);
void system_ui_butn_press(void* payload);
void system_ui_butn_1_tap(void* payload);
void system_ui_butn_1_tap_hold(void* payload);
void system_ui_butn_2_tap(void* payload);
void system_ui_butn_2_tap_hold(void* payload);
void system_ui_butn_3_tap(void* payload);
void system_ui_butn_3_tap_hold(void* payload);
void system_ui_butn_4_tap(void* payload);
void system_ui_butn_4_tap_hold(void* payload);
void system_ui_butn_5_tap(void* payload);
void system_ui_butn_5_tap_hold(void* payload);
void system_ui_ign_on(void);
void system_ui_ign_off(void);

void system_ui_ble_evt(void);
//--------------------------------------------------------------------------//

#endif




