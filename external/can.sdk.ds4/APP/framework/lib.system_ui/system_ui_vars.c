/*==========================================================================*/
// $Id: dei_sc_vars.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "system_ui_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
UInt8    system_ui_sel_func=0;
UInt8 	system_ui_state = 0;
UInt8   system_ui_first_action = NOT_DETECTED;
UInt8 	system_ui_ign_cnt = 0;
UInt8 	system_ui_cmd = 0;
UInt8		system_ui_btn_cnt = 0;
Vrtc		system_ui_vrtc		;
Vrtc		system_ui_cmd_vrtc	;	
UInt32 	system_ui_tstamp_ign=0;
UInt32 	system_ui_tstamp_curr;
Boolean system_ui_program_menu;
UInt16 timestamp_payload;
Boolean system_ui_hold_timeout;
Boolean system_ui_hold_status;

Boolean system_ui_flg_ign=FALSE;

Vrtc system_ui_ble_evt_vrtc;
UInt8  system_in_used_status = 0;
UInt8  tap_while_ign_on = 0;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/


