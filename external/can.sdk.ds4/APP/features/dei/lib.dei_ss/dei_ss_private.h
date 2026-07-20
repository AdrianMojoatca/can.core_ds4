/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_private.h 33268 2015-08-19 15:40:18Z suchita.yadav $
/*==========================================================================*/

#ifndef __DEI_SS_PRIVATE_H__
#define __DEI_SS_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ss.h"
#include "dei_siren.h"
#include "dei_led.h"
#include "dei_prk.h"
#include "sensor.h"
#include "dei_feature.h"
#include "system_type.h"

#include "notify.h"
#include "cmd.h"

#include "nvfs.h"
#include "nvfs_usr.h"

#include "status_system.h"

#include "vrtc.h"
#include "vrtc_f.h"
#include "wake.h"
#include "trace.h"
#include "wdog_v.h"

#include "trace.h"
#include "dbg.h"

#ifdef DEV_LPC_236x
	#include "nvram_usr.h"
#endif
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//

#ifndef DEBUG_DEI_SS
#define DEBUG_DEI_SS			0
#endif

#if DEBUG_DEI_SS == 1
#define TRACE_DEI_SS			TRACE
#else
#define TRACE_DEI_SS(...)		;
#endif
//--------------------------------------------------------------------------//

#define DEI_SS_VER 1


//--------------------------------------------------------------------------//

typedef enum
{
	DEI_SS_SENSOR_NO_TRIGGER          ,
	DEI_SS_SENSOR_TRIGGER_TRUNK  , 
	DEI_SS_SENSOR_TRIGGER_SHOCK  , 
	DEI_SS_SENSOR_TRIGGER_DOOR   ,
	DEI_SS_SENSOR_TRIGGER_IGN    ,
	DEI_SS_SENSOR_TRIGGER_HOOD   ,
	DEI_SS_SENSOR_TRIGGER_TILT   ,
	DEI_SS_SENSOR_TRIGGER_UNKNOWN,
	DEI_SS_SENSOR_TRIGGER_FIELD ,
	DEI_SS_SENSOR_TRIGGER_GLASS    ,
	DEI_SS_SENSOR_TRIGGER_ULTRA

}Dei_Ss_Sensor_Trigger_Source ;

//--------------------------------------------------------------------------//

typedef enum
{
	DEI_SS_SENSOR_NO_WARN          ,
	DEI_SS_SENSOR_WARN_SHOCK  , 
	DEI_SS_SENSOR_WARN_TILT   ,
	DEI_SS_SENSOR_WARN_UNKNOWN,
	DEI_SS_SENSOR_WARN_FIELD ,
	DEI_SS_SENSOR_WARN_GLASS    ,
	DEI_SS_SENSOR_WARN_ULTRA

}Dei_Ss_Sensor_Warn_Source ;

//--------------------------------------------------------------------------//

#ifdef PLATFORM_CM800

typedef enum
{
	DEI_SS_NO_SILENT = 0, 
	DEI_SS_SILENT    = 1

}Dei_Ss_Silent ;

#else

//typedef enum
//{
//	DEI_SS_NO_SILENT = 0, 
//	DEI_SS_SILENT    = 1,
//	DEI_SS_SILENT_LOGIC_RESUME = 2,
//	DEI_SS_PASSIVE_NO_LOCK = 3

//}Dei_Ss_Silent ;

#endif

//--------------------------------------------------------------------------//

typedef enum 
{	
	DEI_ERR_SS_INVALID_OVERRIDE                 =0, //ERR_SS_1   
    DEI_ERR_SS_DOOR_TRIGGER_VALET                 , //ERR_SS_2   
    DEI_ERR_SS_HOOD_TRIGGER_VALET                 , //ERR_SS_3   
    DEI_ERR_SS_TRUNK_TRIGGER_VALET                , //ERR_SS_4   
    DEI_ERR_SS_IGNITION_TRIGGER_VALET             , //ERR_SS_5   
    DEI_ERR_SS_SENSOR_TRIGGER_VALET               , //ERR_SS_6   
    DEI_ERR_SS_SENSOR_WARNING_VALET               , //ERR_SS_7      
    DEI_ERR_SS_DOOR_TRIGGER                       , //ERR_SS_8   
    DEI_ERR_SS_HOOD_TRIGGER                       , //ERR_SS_9   
    DEI_ERR_SS_TRUNK_TRIGGER                      , //ERR_SS_10   
    DEI_ERR_SS_IGNITION_TRIGGER                   , //ERR_SS_11   
    DEI_ERR_SS_SENSOR_TRIGGER                     , //ERR_SS_12   
    DEI_ERR_SS_SENSOR_WARNING                     , //ERR_SS_13   
    DEI_ERR_SS_SENSOR_TRIGGER_NPC                 , //ERR_SS_14   
    DEI_ERR_SS_SENSOR_WARNING_NPC                 , //ERR_SS_15   
    DEI_ERR_SS_TRUNK_RELEASE_TIMEOUT                //ERR_SS_16   
      
}Dei_Err_Ss;
    
//--------------------------------------------------------------------------//

#define SS_NPC_COUT_TRIGGER    3 
#define SS_NPC_COUNT_WARN      20 

//--------------------------------------------------------------------------//

#define DEI_SS_VRTC_TIMEOUT_NPC_TRIGGER 					VRTC_1SEC*60*60 	//1H 
#define DEI_SS_VRTC_TIMEOUT_NPC_WARN    					VRTC_1SEC*60*60 	//1H 
#define DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_ARM_COMFORT	    VRTC_1SEC*30 		//30  seconds
#define DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_ARM               VRTC_1SEC*15 		//15  seconds
#define DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_RS_DONE           VRTC_1SEC*10 		//10  seconds
#define DEI_SS_VRTC_TIMEOUT_ZONE_BYPASS_FIRST_CHECK         VRTC_1SEC*3 		//5   seconds
#define DEI_SS_VRTC_TIMEOUT_ZONE_BYPASS 			        VRTC_1SEC*5 		//5   seconds
#define DEI_SS_VRTC_TIMEOUT_TRUNK_RELEASE					VRTC_1SEC*600		//5	  minutes
#define DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_TRUNK_RELEASE		VRTC_1SEC*5			//5   seconds
#define DEI_SS_VRTC_TIMEOUT_TRIGGER_PAUSE					VRTC_1SEC*2			//2   seconds
#define DEI_SS_VRTC_TIMEOUT_SYS_ARMING_CHIRP			    VRTC_1SEC*20		//20  seconds
#define DEI_SS_VRTC_TIMEOUT_SYS_ARMING_LOCK					VRTC_1SEC*10		//10  seconds
#define DEI_SS_VRTC_TIMEOUT_ONE_TIME_BYPASS					VRTC_1SEC*3			//3   seconds
#define DEI_SS_VRTC_TIMEOUT_SIREN_LOW_BATT          		VRTC_1SEC*10 		//10   seconds
#define DEI_SS_VRTC_TIMEOUT_OVERRIDE_NEW_PASSWORD		 	VRTC_1SEC*15		//10  seconds
#define DEI_SS_VRTC_TIMEOUT_OVERRIDE_NEW_DIGIT			 	VRTC_1SEC*2			//1  seconds
#define DEI_SS_VRTC_TIMEOUT_TRIGGER_HISTORY_PUSH_RELEASE	VRTC_1SEC*5			//5   seconds
#define DEI_SS_VRTC_TIMEOUT_VALET_IGN_CHECK                 VRTC_1SEC*5         //5   seconds
#define DEI_SS_VRTC_TIMEOUT_TRIGGER_HISTORY_DISPLAY_RESET	VRTC_1SEC*60		//60  seconds
#define DEI_SS_VRTC_TIMEOUT_PUSH_VALET_TOGGLE				VRTC_1SEC*8			//8   seconds
#define DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_AUX               VRTC_1SEC*15 		//15  seconds
#define DEI_SS_DELAY_TRIGGER                                200                 //200 milis

//#define DEI_SS_IGNITION_CHECKING_TIMEOUT                    100*4               //4 seconds , the base time is 10 ms

//--------------------------------------------------------------------------//
#ifdef PLATFORM_CM800

#define DEI_SS_LED_ALARM_TRIG_HIST             (DEI_SS_NO_TRIGGER  |\
																							 (get_ss_door_trigger()  ? DEI_LED_DOOR_TRIGGERED  : 0 )|\
																							 (get_ss_hood_trigger()  ? DEI_LED_HOOD_TRIGGERED  : 0 )|\
																							 (get_ss_trunk_trigger() ? DEI_LED_TRUNK_TRIGGERED : 0 )|\
																							 (get_ss_ign_trigger()   ? DEI_LED_IGN_TRIGGERED   : 0 )|\
																							 (get_ss_sensor_trigger(1)? DEI_LED_SENSOR_TRIGGERED: 0))

#define DEI_SS_LED_ALARM_LAST_TRIG	   (DEI_SS_NO_TRIGGER                                     |\
							   (( get_ss_last_trigger() == DEI_SS_TRIGGER_ZONE_DOOR ) 	? DEI_LED_DOOR_TRIGGERED  : 0 )|\
							   (( get_ss_last_trigger() == DEI_SS_TRIGGER_ZONE_HOOD )	? DEI_LED_HOOD_TRIGGERED  : 0 )|\
							   (( get_ss_last_trigger() == DEI_SS_TRIGGER_ZONE_TRUNK)	? DEI_LED_TRUNK_TRIGGERED : 0 )|\
							   (( get_ss_last_trigger() == DEI_SS_TRIGGER_ZONE_IGN  )	? DEI_LED_IGN_TRIGGERED   : 0 )|\
							   (( get_ss_last_trigger() == DEI_SS_TRIGGER_ZONE_SENSOR)	? DEI_LED_SENSOR_TRIGGERED: 0 ))
								 

#define DEI_SS_LED_DISARM         	   (DEI_LED_DISARM                                         |\
							   (( get_ss_last_trigger() == DEI_SS_TRIGGER_ZONE_DOOR ) 	? DEI_LED_DOOR_TRIGGERED  : 0 )|\
							   (( get_ss_last_trigger() == DEI_SS_TRIGGER_ZONE_HOOD )	? DEI_LED_HOOD_TRIGGERED  : 0 )|\
							   (( get_ss_last_trigger() == DEI_SS_TRIGGER_ZONE_TRUNK)	? DEI_LED_TRUNK_TRIGGERED : 0 )|\
							   (( get_ss_last_trigger() == DEI_SS_TRIGGER_ZONE_IGN  )	? DEI_LED_IGN_TRIGGERED   : 0 )|\
							   (( get_ss_last_trigger() == DEI_SS_TRIGGER_ZONE_SENSOR)	? DEI_LED_SENSOR_TRIGGERED: 0 ))

#define	DEI_SS_TRIGGER_PLD	   (get_ss_door_trigger()  ? NOTIFY_PLD_TRIG_DOOR  : 0)|\
							   (get_ss_hood_trigger()  ? NOTIFY_PLD_TRIG_HOOD  : 0)|\
							   (get_ss_trunk_trigger() ? NOTIFY_PLD_TRIG_TRUNK : 0)|\
							   (get_ss_ign_trigger()   ? NOTIFY_PLD_TRIG_IGN   : 0)|\
								 (get_ss_sensor_trigger(1)? DEI_LED_SENSOR_TRIGGERED: 0 )|\
							   (get_ss_sensor_npc(1)    ? NOTIFY_PLD_TRIG_S1_NPC: 0)

#define	DEI_SS_ZONE_OPEN_PLD   (get_door_status()   ? NOTIFY_PLD_ZONE_DOOR  : 0)|\
							   (get_hood_status()   ? NOTIFY_PLD_ZONE_HOOD  : 0)|\
							   (get_trunk_status()  ? NOTIFY_PLD_ZONE_TRUNK : 0)|\
							   (get_ss_sensor_npc(1) ? NOTIFY_PLD_ZONE_S1_NPC: 0)

#else

#define DEI_SS_LED_ALARM_TRIG_HIST             (DEI_SS_NO_TRIGGER  |\
																							 (get_ss_door_trigger()  ? DEI_LED_DOOR_TRIGGERED  : 0 )|\
																							 (get_ss_hood_trigger()  ? DEI_LED_HOOD_TRIGGERED  : 0 )|\
																							 (get_ss_trunk_trigger() ? DEI_LED_TRUNK_TRIGGERED : 0 )|\
																							 (get_ss_ign_trigger()   ? DEI_LED_IGN_TRIGGERED   : 0 )|\
																							 (get_ss_sensor_trigger(1)? DEI_LED_SHOCK_SENSOR_TRIGGERED: 0 )|\
																							 (get_ss_sensor_trigger(2)? DEI_LED_TILT_SENSOR_TRIGGERED: 0 )|\
																							 (get_ss_sensor_trigger(0)? DEI_LED_UNKNOWN_SENSOR_TRIGGERED: 0))

#define DEI_SS_LED_ALARM_LAST_TRIG	   dei_ss_led_alarm_last_trig()
								 

#define DEI_SS_LED_DISARM  dei_ss_led_disarm()


#define	DEI_SS_TRIGGER_PLD	   (get_ss_door_trigger()  ? NOTIFY_PLD_TRIG_DOOR  : 0)|\
							   (get_ss_hood_trigger()  ? NOTIFY_PLD_TRIG_HOOD  : 0)|\
							   (get_ss_trunk_trigger() ? NOTIFY_PLD_TRIG_TRUNK : 0)|\
							   (get_ss_ign_trigger()   ? NOTIFY_PLD_TRIG_IGN   : 0)|\
								 (get_ss_sensor_trigger(1)? NOTIFY_PLD_TRIG_S1: 0 )|\
								 (get_ss_sensor_trigger(2)? NOTIFY_PLD_TRIG_S2: 0 )|\
								 (get_ss_sensor_trigger(0)? NOTIFY_PLD_TRIG_S3: 0)|\
							   (get_ss_sensor_npc(1)    ? NOTIFY_PLD_TRIG_S1_NPC: 0)|\
							   (get_ss_sensor_npc(2)    ? NOTIFY_PLD_TRIG_S2_NPC: 0)|\
							   (get_ss_sensor_npc(0)    ? NOTIFY_PLD_TRIG_S3_NPC: 0)

#define	DEI_SS_ZONE_OPEN_PLD   (get_door_status()   ? NOTIFY_PLD_ZONE_DOOR  : 0)|\
							   (get_hood_status()   ? NOTIFY_PLD_ZONE_HOOD  : 0)|\
							   (get_trunk_status()  ? NOTIFY_PLD_ZONE_TRUNK : 0)|\
							   (get_ss_sensor_npc(1) ? NOTIFY_PLD_ZONE_S1_NPC: 0)|\
							   (get_ss_sensor_npc(2) ? NOTIFY_PLD_ZONE_S2_NPC: 0)|\
							   (get_ss_sensor_npc(0) ? NOTIFY_PLD_ZONE_S3_NPC: 0)|\
								 (get_inst_trig_wire_status() ? NOTIFY_PLD_ZONE_DOOR : 0)

#endif

								 



#define DEI_SS_LED_ZONE_BYPASS dei_ss_led_zone_bypass()
								
#define DEI_SS_TRIG_HIST_REPORT_TIMEOUT  VRTC_1SEC*5

//--------------------------------------------------------------------------//



//--------------------------------------------------------------------------//

typedef struct s_Dei_Ss_Status // defines structure of nvfs file contents, so sizes must be explicit
{
	UInt8                 arm            ;  // cast as Boolean
	UInt8                 valet          ;  // cast as Boolean
	UInt8                 trigger_active ;  // cast as Boolean 
	UInt8                 trigger_source ;  // cast as Dei_Ss_Trigger_Source 
	UInt8                 event_memory[SS_TRIGGER_MEM_SIZE];  // 4 byte array
}Dei_Ss_Status;

//--------------------------------------------------------------------------//

typedef enum 
{
	DEI_SS_SENSOR_BYPASS_OFF , 	
	DEI_SS_SENSOR_BYPASS_WARN,	
	DEI_SS_SENSOR_BYPASS_FULL
		 
}Dei_Ss_Sensor_Bypass;						  

#define POP_TRUNK_STATE_WAIT10MIN			3		// Wait 10 minutes in OFF mode
#define POP_TRUNK_STATE_WAIT_CLOSE_2	4		// Wait for trunk to close in OFF mode

#define DEI_SS_DOOR_SKIP_TIMEOUT    (5*VRTC_1SEC)

//--------------------------------------------------------------------------//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
#ifdef PLATFORM_933
	extern Sensor  				ss_sensor[3]            ;
	extern Sensor  				ss_sensor_ext           ;
  extern int pop_trunk_state;
#else
	extern Sensor  				ss_sensor            ;
#endif
extern System_Type          ss_system_type;
extern Boolean 				dei_ss_2nd_disarm    ; 
extern Dei_Ss_Sensor_Bypass	dei_ss_sensor_bp     ;
extern Boolean              dei_ss_trigger_enable;
extern UInt8				dei_ss_sensor_num;	
extern UInt8 				dei_ss_curr_sensor_aid;
extern UInt8				dei_ss_curr_sensor_chanl;
extern UInt8					dei_ss_curr_sensor_idx;
extern UInt8 					dei_ss_arm_cmd;
extern Boolean 				dei_ss_alarm_hist_report_on;
extern Vrtc 					dei_ss_trg_history_timeout;
extern Boolean				dei_ss_2nd_arm		;
extern Vrtc          dei_ss_door_skip_vrtc_timer;
extern Boolean       dei_ss_door_is_skip;

extern Dei_Ss_Trigger_Source current_trg_zone;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

void 	dei_ss_cmd_init			   ( void );
void 	dei_ss_sensor_type_init    ( void );

Boolean dei_ss_status_init		   ( Dei_Ss_Status *dei_status );
void    dei_ss_status_save         ( Wake_Status wake_status );

void    dei_ss_logic_resume        ( void );

void    dei_ss_arm                 ( void *remote );
void    dei_ss_arm_oem             ( void *pld    );
void    dei_ss_lock_oem            ( void *pld    );
void    dei_ss_arm_silent          ( void *remote );
void    dei_ss_arm_actions         ( Dei_Ss_Silent silent , void *remote );

void    dei_ss_disarm              ( void *remote );
void    dei_ss_disarm_oem          ( void *pld    );
void    dei_ss_disarm_silent       ( void *remote );
void    dei_ss_disarm_actions      ( Dei_Ss_Silent silent , void *remote );

void    dei_ss_siren_low_batt_init ( void );
void    dei_ss_siren_low_batt_check( void *remote );
void    dei_ss_siren_low_batt_stop ( void );

void    dei_ss_trunk_release_init  ( void );
void    dei_ss_zone_bypass_init    ( void );
Boolean dei_ss_zone_bypass_status  ( void );

void    dei_ss_sensor_bypass       ( void *remote );
void    dei_ss_sensor_silent_arm   ( void *remote );
void    dei_ss_full_silent_arm     ( void *remote );
void    dei_ss_sensor_bp_rs_done   ( void );

void    dei_ss_sensor_adjust_enter ( void *remote );
void    dei_ss_sensor_adjust_put   ( void *remote );
void		dei_ss_sensor_adjust_exit	 (	void );

void   dei_ss_rf_trg_history       ( void *remote );

void   dei_ss_rf_valet             ( void *remote );

void   dei_ss_panic                ( void *remote );

void   dei_ss_trigger_init         ( void );
void   dei_ss_trigger_start        ( Dei_Ss_Trigger_Source trg_zone );
void   dei_ss_trigger_stop         ( void );


void  dei_ss_door_open			   ( void );
void  dei_ss_door_close            ( void );
void  dei_ss_hood_open			   ( void );
void  dei_ss_hood_close            ( void );
void  dei_ss_trunk_open			   ( void );
void  dei_ss_trunk_close           ( void );

//void dei_ss_brake_press (void);

void  dei_ss_ignition_on           ( void );
void dei_ss_ignition_off					( void );
void  dei_ss_sensor_trigger        ( void *pld);
void  dei_ss_sensor_warning        ( void *pld);
void  dei_ss_sys_arming_init       ( void );
void ngmm_ss_sys_arming_disable		(void);
void  dei_ss_disarm_override_init  ( void );
void  dei_ss_push_valet_init       ( void );
void  dei_ss_push_trg_history_init ( void );
void  dei_ss_instant_alarm_trig		 ( void );

void dei_ss_ignition_checking_after_gwr_off(void);
void dei_ss_ignition_checking_timeout      (Boolean timeout_expired);

void dei_ss_sensor_tap1_count(void);
void dei_ss_sensor_tap2_count(void);
void dei_ss_sensor_tap3_count(void);
void dei_ss_sensor_tap4_count(void);
void dei_ss_sensor_tap5_count(void);
void dei_ss_sensor_mode_exit_ign_on(void);

void dei_ss_ign_on_for_trig_history(void);
void dei_ss_door_skip_set(void);
void dei_ss_door_skip_clr(void);
void dei_ss_door_skip_timeout_handler(void);

Dei_Led_Pattern dei_ss_led_zone_bypass(void);
Dei_Led_Pattern dei_ss_led_alarm_last_trig(void);
Dei_Led_Pattern get_led_last_pattern(void);
void dei_ss_sys_arming_save_led_pattern(Dei_Led_Pattern pattern);

/*==========================================================================*/

#endif

