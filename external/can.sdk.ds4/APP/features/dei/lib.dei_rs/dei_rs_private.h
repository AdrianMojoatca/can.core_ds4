/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_private.h 33268 2015-08-19 15:40:18Z suchita.yadav $
/*==========================================================================*/
#ifndef __DEI_RS_PRIVATE_H__
#define __DEI_RS_PRIVATE_H__
/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_led.h"
#include "dei_prk.h"
#include "dei_feature.h"
#include "dei_system_private.h"

#include "notify.h"
#include "cmd.h"
#include "out_q.h"
#include "per_exec.h"


#include "ecm_vtacho.h"

#include "nvfs.h"
#include "nvfs_usr.h"
#include "nvram.h"

#include "status_system.h"

#include "vrtc.h"
#include "vrtc_f.h"
#include "wake.h"
#include "wdog_v.h"
#include "config_gpio.h"

#ifdef DEV_LPC_236x
	#include "nvram_usr.h"
#endif

#ifdef DEV_LPC_236x
	#include "dei_temp.h"
#endif

#include "trace.h"
#include "dbg.h"

#include "timeout_f.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//

//#define DEI_RS_TRACE        TRACE_09

//--------------------------------------------------------------------------//

#define DEI_RS_VER 2

#ifndef DEBUG_DEI_RS
#define DEBUG_DEI_RS			0
#endif

#if DEBUG_DEI_RS == 1
#define TRACE_DEI_RS			TRACE
#else
#define TRACE_DEI_RS(...)		;
#endif

//#define TRACE_SENSE_DEI							TRACE_12

//--------------------------------------------------------------------------//

#define TASK_DEI_RS_ECM_NAME  "dei_rs_ecm_tsk"
#define TASK_DEI_RS_ECM_PRI   9
#ifdef PLATFORM_933
#define TASK_DEI_RS_ECM_STK   0x0160
#else
#define TASK_DEI_RS_ECM_STK   0x0100
#endif
//--------------------------------------------------------------------------//

typedef void (*Dei_Rs_State)( void );

//--------------------------------------------------------------------------//

typedef enum
{    
    DEI_ERR_RS_SHUTDOWN_TRANSMITTER      =0       , 
    DEI_ERR_RS_SHUTDOWN_RUNTIME_EXPIRED           , 
    DEI_ERR_RS_SHUTDOWN_BRAKE                     , 
    DEI_ERR_RS_SHUTDOWN_PARKING                   , 
    DEI_ERR_RS_SHUTDOWN_HANDBRAKE                 , 
    DEI_ERR_RS_SHUTDOWN_VALET_RS                  , 
    DEI_ERR_RS_SHUTDOWN_LOW_VOLTAGE               , 
    DEI_ERR_RS_SHUTDOWN_LOW_RPM                   , 
    DEI_ERR_RS_SHUTDOWN_OVER_RPM                  , 
    DEI_ERR_RS_SHUTDOWN_TRIGGER                   , 
    DEI_ERR_RS_SHUTDOWN_GLOWPLUG                  , 
    DEI_ERR_RS_SHUTDOWN_DOOR_RPM                  , 
    DEI_ERR_RS_SHUTDOWN_HOOD                      , 
    DEI_ERR_RS_START_GEARBOX_NOT_SUPPORTED        , 
    DEI_ERR_RS_START_MTS_TACHO_NOT_PROGRAMMED     , 
    DEI_ERR_RS_START_MTS_NOT_INIT                 , 
    DEI_ERR_RS_START_MTS_ECM_NOT_TACHO            , 
    DEI_ERR_RS_START_RS_VALET_SWITCH              , 
    DEI_ERR_RS_START_BRAKE                        , 
    DEI_ERR_RS_START_HOOD                         , 
    DEI_ERR_RS_START_PARKING                      , 
    DEI_ERR_RS_START_HANDBRAKE                    , 
    DEI_ERR_RS_START_ALARM_IN_TRIGGER             , 
    DEI_ERR_RS_TIMER_INVALID_REQUEST              , 
    DEI_ERR_RS_SMART_INVALID_REQUEST              , 
    DEI_ERR_RS_TIMER_ERROR_START                  , 
    DEI_ERR_RS_SMART_ERROR_START                  , 
    DEI_ERR_RS_SMART_INVALID_VOLTAGE              , 
    DEI_ERR_RS_INVALID_CRANK                      , 
    DEI_ERR_RS_VTACHO_INIT_ERROR                  , 

}Dei_Rs_Err_Code;
          

//---------------------------------------------------------------------------//

typedef enum
{
	DEI_RS_EVT_TIMEOUT             ,		 //0
	DEI_RS_EVT_RUNTIME_EXPIRED     , 		 //1
    DEI_RS_EVT_OVERREV             ,		 //2
    DEI_RS_EVT_LOW_TACH            ,		 //3
    DEI_RS_EVT_TRANSMITTER_SHUTDOWN,		 //4
    DEI_RS_EVT_BRAKE               ,		 //5
    DEI_RS_EVT_HOOD                ,		 //6
    DEI_RS_EVT_MTS                 ,		 //7
    DEI_RS_EVT_NEUTRAL             ,		 //8
    DEI_RS_EVT_LOW_BATT            ,		 //9
    DEI_RS_EVT_TRIGGER             ,		 //10
    DEI_RS_EVT_GLOWPLUG_TIMEOUT    ,		 //11
    DEI_RS_EVT_ENGINE_RUNNING      ,		 //12	
	  DEI_RS_EVT_GLOWPLUG_OFF        ,     //13
	  DEI_RS_EVT_MTS_LOCK_SHUTDOWN   ,     //14
    DEI_RS_EVT_HANDBRAKE           ,     //15
#ifdef PLATFORM_933  
    DEI_RS_EVT_PITSTOP_NOT_AVAIL
#endif  
	
}Dei_Rs_Evt ;   

//--------------------------------------------------------------------------//

typedef enum
{
	DEI_RS_TURBO_START , 
	DEI_RS_NORMAL_START , 
	DEI_RS_TIMER_START  ,
	DEI_RS_SMART_START

}Dei_Rs_Start_Source ;

//--------------------------------------------------------------------------//

#define	DEI_RS_VOLTAGE_CHECK_TIMEOUT   						100*15     		//10ms base time
	
#define DEI_RS_GWR_TO_ACC_TIMEOUT      						1800       		//1ms base time
#define DEI_RS_ACC_TO_IGN_TIMEOUT      						100        		//1ms base time
#define DEI_RS_WAIT_TIMEOUT            						2500       		//1ms base time
#define DEI_RS_WAIT_GLOWPLUG_TIMEOUT   						180*1000   		//1ms base time 
#define DEI_RS_START_TIMEOUT           						5*1000     		//1ms base time

#define DEI_RS_VRTC_TIMEOUT_FIRST_PRESS			   			VRTC_1SEC*5     
#define DEI_RS_VRTC_TIMEOUT_ERROR_NOTIFY					VRTC_1SEC*6     
#define DEI_RS_VRTC_TIMEOUT_TACHO_PRG  						VRTC_1SEC*20
#define DEI_RS_VRTC_TIMEOUT_SHUTDOWN_HISTORY_PUSH_RELEASE   VRTC_1SEC*2
#define DEI_RS_VRTC_TIMEOUT_SHUTDOWN_HISTORY_PUSH_PRESS     VRTC_1SEC*5
#define DEI_RS_VRTC_TIMEOUT_SHUTDOWN_HISTORY_DISPLAY_RESET	VRTC_1SEC*60
#define DEI_RS_VRTC_TIMEOUT_SMART_1H		                VRTC_1SEC*60*60
#define DEI_RS_VRTC_TIMEOUT_SECURE_LOCK_OFF					VRTC_1SEC*4     
#define DEI_RS_VRTC_TIMEOUT_SECURE_LOCK_ON					VRTC_1SEC*4     
#define DEI_RS_VRTC_TIMEOUT_SMART_SECURE_LOCK_ON			VRTC_1SEC*4     
#define DEI_RS_VRTC_TIMEOUT_DEFOGGER						VRTC_1SEC*10*60
#define DEI_RS_VRTC_TIMEOUT_DEFOGGER_ON					VRTC_1SEC*10
#define DEI_RS_VRTC_TIMEOUT_MTS								VRTC_1SEC*20
#define DEI_RS_VRTC_TIMEOUT_RUNTIME_TIME_BASE				VRTC_1SEC*60
#define DEI_RS_VRTC_TIMEOUT_2X_HANDBRAKE								VRTC_1SEC*10

#define DEI_RS_MTS_DOOR_CHECKING_TIMEOUT                    250        //10ms base time ,2500 miliseconds

#ifdef PLATFORM_CM800
	#define DEI_MTS_DOOR_OPEN_CLOSE_TIMEOUT                     250         //
#else
	#define DEI_MTS_DOOR_OPEN_CLOSE_TIMEOUT                     VRTC_1SEC*15         //
#endif

#define DEI_RS_TEMP_DEFOGGER_TRIGGER  (40+12)	       	   //+ 12 Celsius degree

#ifdef PLATFORM_CM800
	#define DEI_RS_MTS_COMPLETION		dei_feature_get(DEI_FEAT_CUSTOM_FEATURE_01)
#else
	#define DEI_RS_MTS_COMPLETION		dei_feature_get(DEI_FEAT_MANUAL_TRANSMISSION_COMPLETION)
#endif

#define AS_RS_NOTIFY_TIMEOUT			VRTC_1SEC*3

#define DEI_RS_MTS_BROKEN_CHK_TIMEOUT     3500    //3.5 sec   
//--------------------------------------------------------------------------//

#define DEI_RS_RPM_MIN		( get_rs_rpm_ref() / 6 )

typedef struct s_Dei_Rs_Oem
{
    Boolean  rt_override;
    UInt8    runtime;
}Dei_Rs_Oem;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Dei_Rs_State  dei_rs_state   ;
extern Boolean       dei_rs_trigger_enable; 
extern Dei_Rs_Oem    dei_rs_oem;
extern Boolean			 dei_rs_takeover_without_key;
extern Boolean 			 rs_progress;

extern Boolean        door_open_allow_once;
extern Boolean dei_rs_safe_to_takeover;
extern Boolean dei_rs_safe_to_takeover_check;

extern Timeout dei_rs_mts_broken_check_timer;

extern Boolean    dei_ss_lock_stored;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

void        dei_rs_secure_lock_init       ( void );
void        dei_rs_secure_lock_gwr_on     ( void );
void        dei_rs_secure_lock_gwr_off    ( void );
void        dei_rs_secure_lock_start_off  ( void );

void 		dei_rs_rf_start_stop_init	  ( void );
 
void 		dei_rs_cmd_init				  ( void );

void        dei_rs_smart_start_init  	  ( void );
void        dei_rs_smart_start       	  ( void );
void        dei_rs_smart_stop        	  ( void );
void        dei_rs_timer_start_init       ( void );
void        dei_rs_timer_start            ( void );
void        dei_rs_timer_stop             ( void );

void        dei_rs_rf_start_stop          ( void *remote );
void 		dei_rs_rf_timer_toogle        ( void *remote );
void        dei_rs_rf_smart_prg_set       ( void *remote );
void        dei_rs_rf_smart_prg_enter     ( void *remote );
void 		dei_rs_rf_smart_toogle        ( void *remote );
void        dei_rs_rf_timer_prg_adjust    ( void *remote );
void        dei_rs_rf_timer_prg_enter     ( void *remote );
void        dei_rs_rf_runtime_reset       ( void *remote );
void 		dei_rs_rf_runtime_check		  ( void *remote );
void 				as_rs_extend_runtime_cmd( void );


void        dei_rs_defogger_init          ( void );
void        dei_rs_rf_defogger_toogle     ( void *remote );
void        dei_rs_auto_defogger          ( void ); 

void        dei_rs_ecm_init               ( void );
void        dei_rs_ecm_start              ( void );
void    	dei_rs_ecm_vtacho             ( void );
void    	dei_rs_ecm_voltage            ( void );
void    	dei_rs_ecm_off                ( void );
void    	dei_rs_ecm_tacho              ( void );

void        dei_rs_mts_init               ( void );
void    	dei_rs_gwr                    ( void );
void    	dei_rs_acc                    ( void );
void    	dei_rs_ign                    ( void );
void    	dei_rs_wait_start             ( void );
void    	dei_rs_start                  ( void );
void    	dei_rs_running                ( void );
void    	dei_rs_retry                  ( void );
void    	dei_rs_shutdown               ( void );

Dei_Rs_Evt 	dei_rs_error_start            ( void );
void    	dei_rs_execute                ( void );

void        dei_rs_evt_init               ( void );
void        dei_rs_evt_signal             ( Dei_Rs_Evt rs_evt );
Dei_Rs_Evt  dei_rs_evt_wait               ( UInt16 evt_msk , TskTimeout timeout );
void 		dei_rs_runtime_init		      ( void );
void        dei_rs_runtime_set            ( Dei_Rs_Start_Source source );
void        dei_rs_runtime_start          ( void );
void        dei_rs_runtime_stop           ( void );
void        dei_rs_runtime_reset          ( void );
void        dei_rs_runtime_pause(void);

void		dei_rs_trigger_door           ( void );
void		dei_rs_trigger_trunk          ( void );
void		dei_rs_trigger_generic        ( void );

void        dei_rs_door                   ( void );
void        dei_rs_hood                   ( void );
void        dei_rs_brake                  ( void );
void        dei_rs_neutral                ( void );
void        dei_rs_handbrake              ( void );
void        dei_rs_key                    ( void );
void        dei_rs_glowplug               ( void );
void 		dei_rs_tacho_prg_init	      ( void );
void        dei_rs_shutdown_history_init  ( void );
void        dei_rs_defoger_init           ( void );
#ifdef PLATFORM_933
void dei_rs_door_open(void);
void dei_rs_door_closed(void);
void dei_rs_unlock_cmd(void);
#endif

void        dei_rs_mts_smart_timer_disable( void );
Boolean     dei_rs_ready_for_mts_enable   ( void );

Boolean     dei_rs_pts_out_is_programmed  ( void );

void        dei_rs_mts_door_checking_timeout_stop  ( void );
Boolean     dei_rs_mts_door_checking_timeout_status( void );
void        dei_rs_mts_door_checking_timeout_start ( void );
void        dei_rs_mts_door_checking_timeout       ( Boolean status );
void        dei_rs_mts_door_checking_disable       ( void );

void        dei_rs_start_only								 ( void );
void        dei_rs_stop_only 								 ( void );
void        dei_rs_start_stop_from_smartstart( void );

UInt8       dei_rs_get_oem_rt(void);
void		dei_rs_push_valet(void);
void		dei_rs_rf_valet(void);
void 		dei_rs_rf_combo_valet( void );

void dei_rs_mts_broken_check(void);
void dei_rs_mts_is_broken(void);
void start_restart_guard(void);

#endif

