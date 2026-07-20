/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_private.h 21376 2013-07-01 06:22:17Z florin.olariu $
/*==========================================================================*/

#ifndef __DEI_SYSTEM_PRIVATE_H__
#define __DEI_SYSTEM_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "brand.h"
#include "dei_feature.h"
#include "dei_ss.h"
#include "dei_rs.h"
#include "dei_system.h"
#include "dei_prk.h"
#include "dei_led.h"
#include "dei_siren.h"
#include "dei_out.h"
#include "nvfs_usr.h"
#include "system_type.h"
#include "cmd.h"
#include "sense_rs.h"
#include "notify.h"
#include "prg.h"
#include "wake.h"
#include "vrtc.h"
#include "vrtc_f.h"
#include "timeout_f.h"
#include "timeout.h"
#include "status_system.h"
#include "wdog_v.h"
#include "fob_ctrl_dei.h"
#include "dei_sc.h"
#include "status_system.h"

#ifdef DEV_LPC_236x
	#include "nvram_usr.h"
#endif

#ifdef DEV_LPC_236x
	#include "dei_sc.h"
	#include "dei_rf1.h"
#endif

#include "dbg.h"
#include "trace.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*---------------------------------------------------------------------------*/

//#define DEI_SYS_TRACE           TRACE_07
#ifndef DEBUG_DEI_SYS
#define DEBUG_DEI_SYS			0
#endif

#if DEBUG_DEI_SYS == 1
#define TRACE_SYS			TRACE
#else
#define TRACE_SYS(...)		;
#endif

/*---------------------------------------------------------------------------*/

#define cast_to_remote(x)       ((Dei_Remote_Button*)((UInt8*)x + (UInt32)struct_offs(Exec_Q_Msg*,arg) )) 

//---------------------------------------------------------------------------//

#define	DEI_NOTIFY_LOCK_PLD	        (get_ss_sensor_npc(1) ? NOTIFY_PLD_ZONE_S1_NPC: 0)|\
																		(get_ss_sensor_npc(2) ? NOTIFY_PLD_ZONE_S2_NPC: 0)|\
																		(get_ss_sensor_npc(0) ? NOTIFY_PLD_ZONE_S3_NPC: 0)
									


#define	DEI_NOTIFY_UNLOCK_PLD		(get_ss_door_trigger()  ? NOTIFY_PLD_TRIG_DOOR  : 0)|\
									(get_ss_hood_trigger()  ? NOTIFY_PLD_TRIG_HOOD  : 0)|\
									(get_ss_trunk_trigger() ? NOTIFY_PLD_TRIG_TRUNK : 0)|\
									(get_ss_ign_trigger()   ? NOTIFY_PLD_TRIG_IGN   : 0)|\
									(get_ss_sensor_trigger(1)? NOTIFY_PLD_TRIG_S1: 0 )|\
									(get_ss_sensor_trigger(2)? NOTIFY_PLD_TRIG_S2: 0 )|\
									(get_ss_sensor_trigger(0)? NOTIFY_PLD_TRIG_S3: 0)|\
									(get_ss_sensor_npc(1)    ? NOTIFY_PLD_TRIG_S1_NPC: 0)|\
									(get_ss_sensor_npc(2)    ? NOTIFY_PLD_TRIG_S2_NPC: 0)|\
									(get_ss_sensor_npc(0)    ? NOTIFY_PLD_TRIG_S3_NPC: 0)

//---------------------------------------------------------------------------//

    
#define DEI_SYSTEM_VRTC_TIMEOUT_DOME		 	 VRTC_1SEC*30
#define DEI_SYSTEM_VRTC_TIMEOUT_IGN_LOCK         VRTC_1SEC*3
#define DEI_SYSTEM_VRTC_TIMEOUT_RPM_LOCK         VRTC_1SEC*6
#define DEI_SYSTEM_HEADLIGHT_ALERT_TIMOUT		 VRTC_1SEC*3

#define DEI_SYSTEM_TIMEOUT_1BTN_UNLOCK           1000  //<--500ms    
#define DEI_SYSTEM_TIMEOUT_1BTN_UNLOCK_AFTER_RS  500   //<--500ms       

#define SYSTEM_2ND_UNLOCK_WINDOW                 (VRTC_1SEC*5)
//---------------------------------------------------------------------------//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void dei_system_ss_init       		( void );
void dei_system_rs_init       		( void );
void dei_system_hybrid_init	  		( void );

void dei_system_common_init   		( void );
void dei_system_common_events		( void );
void dei_system_prg_init            ( void );
void dei_system_sense_init          ( void );
void dei_system_rf_1btn_init		( void );
void dei_system_smart_start_init    ( void );

void dei_system_rf_car_finder       ( void *remote );
void dei_system_panic_init          ( void );
void dei_system_rf_panic            ( void *remote );
void as_system_rf_panic							( void *remote );
void dei_system_panic_start         ( void );
void dei_system_panic_stop          ( void );

void dei_system_rf_lock       		( void *remote ); 
void dei_system_rf_unlock     		( void *remote );

void dei_system_rf_trunk      		( void *remote );
void dei_system_rf_trunk_ext        ( void *remote );
void dei_system_rf_trunk_release    ( void *remote );

void dei_system_rf_poptrunk_zone_open	( void);

void dei_system_rf_range_check		( void ); 
void dei_system_rf_error      		( void );
void dei_system_rf_keyboard_unlock	( void );
void dei_system_rf_temp             ( void );

void dei_system_rf_ch3_on      		( void *remote );
void dei_system_rf_ch3_off		    ( void *remote );
void dei_system_ch3_reset		    ( void );
void dei_system_ch3_link_arm        ( void );
void dei_system_ch3_link_disarm     ( void );
void dei_system_ch3_link_rs_on      ( void );
void dei_system_ch3_link_rs_off     ( void );


void dei_system_rf_ch4_on      		( void *remote );
void dei_system_rf_ch4_off		    ( void *remote );
void dei_system_ch4_reset		    ( void );
void dei_system_ch4_link_arm        ( void );
void dei_system_ch4_link_disarm     ( void );
void dei_system_ch4_link_rs_on      ( void );
void dei_system_ch4_link_rs_off     ( void );

void dei_system_rf_ch5_on      		( void *remote );
void dei_system_rf_ch5_off		    ( void *remote );
void dei_system_ch5_reset		    ( void );
void dei_system_ch5_link_arm        ( void );
void dei_system_ch5_link_disarm     ( void );
void dei_system_ch5_link_rs_on      ( void );
void dei_system_ch5_link_rs_off     ( void );

void dei_system_rf_ch6_on      		( void *remote );
void dei_system_rf_ch6_off		    ( void *remote );
void dei_system_ch6_reset		    ( void );
void dei_system_ch6_link_arm        ( void );
void dei_system_ch6_link_disarm     ( void );
void dei_system_ch6_link_rs_on      ( void );
void dei_system_ch6_link_rs_off     ( void );

void dei_system_oem_lock	  		( void *pld );
void dei_system_oem_unlock    		( void );
void dei_system_oem_trunk           ( void *remote );

void dei_system_ign_dome_init 		( void );
void dei_system_ign_lock_init 		( void );
void dei_system_ign_stat_init		( void );
void dei_system_wait_ign_off		( UInt32 timeout_ms );

void dei_system_2nd_unlock_reset    (void);
void dei_system_convenience_alert			(void);
void dei_system_headlight_alert_on			(void);
void dei_system_headlight_alert_off			(void);
void dei_system_trunk_close						(void);
void dei_system_trunk_open						(void);
void dei_system_rf_trunk_disarm			( void *cmdpld );


void dei_system_headlight_begin (void);
void dei_system_headlight_process (void);

void dei_system_ptim_stop_on(void);
void dei_system_ptim_stop_off(void);

void dei_system_ext_open_trigger(void);
void dei_system_ext_close_trigger(void);
void dei_system_ext_trunk_trigger(void);


void dei_system_prox_oem_arm(void);
void dei_system_prox_oem_disarm(void);
void dei_system_prox_rf_lock(void);
void dei_system_prox_rf_unlock(void);
	
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*===============================void dei_system_ptim_stop_on(void);
===========================================*/
extern Boolean     	dei_system_2nd_unlock ;
extern Boolean			dei_ss_2nd_disarm;

extern System_Type 	dei_system_type       ;
extern Vrtc        	vrtc_system_2nd_unlock;
extern Vrtc					dei_system_headlight_alert_vrtc;
extern Vrtc					dei_system_pop_trunk_vrtc;

extern Boolean         dei_system_prg_mode;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

