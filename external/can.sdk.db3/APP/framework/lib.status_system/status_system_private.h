/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: status_system_private.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __STATUS_SYSTEM_PRIVATE_H__
#define __STATUS_SYSTEM_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "status_system.h"
#include "timeout_f.h"
#include "cmd.h"
#include <string.h>
#include "trace.h"
#include "dbg.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//

#ifndef PLATFORM_CM800
#define cmd_q_rs_insert				cmd_q_insert
#endif

//--------------------------------------------------------------------------//

#define   STATUS_Q_TIMEOUT                  500

#ifndef DEBUG_SYS_STATUS
#define DEBUG_SYS_STATUS			0
#endif

#if DEBUG_SYS_STATUS == 1
#define TRACE_DEI_SYS_STATUS			TRACE
#else
#define TRACE_DEI_SYS_STATUS(...)		;
#endif

//--------------------------------------------------------------------------//

typedef struct s_Sensor_Temp
{
	Boolean is_present ; 
	UInt8 	db_entry_num;
	UInt8	tempsens_chanl;
	UInt8   value      ; 

}Sensor_Temp;

//--------------------------------------------------------------------------//

typedef struct s_Sensor_Shock
{
	Boolean is_present ; 
	UInt8 	db_entry_num;
	UInt8 	shock_chanl;
	UInt8   sens_level  ; 
	UInt8		sens_ena_detect_bits;
	UInt8 	sens_ena_alert_bits;

}Sensor_Shock;

//--------------------------------------------------------------------------//

typedef struct s_Rs
{
  	Boolean  gwr                   ;
  	Boolean  acc                   ;
  	Boolean  ign                   ;
  	Boolean  start                 ;
  
  	Boolean  smart_on              ;
  	Boolean  timer_on              ;
  	Boolean  defogger_status       ;
  	Boolean  turbo_on              ;
  	Boolean  idle_on               ;
    Boolean  idle_turbo_done	   ;	
    Boolean  valet                 ;
  
  	Boolean  mts_on                ;
  	Boolean  mts_in_progress       ;
  	Boolean  mts_lock              ;
  	Boolean  mts_prepared          ;
	Boolean  mts_exit_by_hood	   ;

  	UInt16   rpm_ref               ;
  	UInt8    vtacho_ref            ;
  	UInt8    retry_count           ;
  	UInt8    error_shutdown        ;
  	UInt8    runtime_programmed    ;
	UInt8    runtime_remaining     ;
	Boolean  wire_ign_skip;
	Boolean rs_ign_off;
	
	Boolean  start_successful	   ;
	Boolean  ign_disarm_oem  	   ;  
	Boolean  iid_engine_running    ;

}Rs ;

//--------------------------------------------------------------------------//

typedef struct s_Ss
{   
    Boolean  armed                ;
    Boolean  locked               ;
    Boolean  trigger_active       ;
    Boolean  panic_active         ;
    Boolean  valet                ;
    Boolean  shock_sensor_npc           ;
    Boolean  tilt_sensor_npc           ;
    Boolean  unknown_sensor_npc           ;

		Boolean  shock_sensor_triggered     ;
    Boolean  tilt_sensor_triggered     ;
    Boolean  unknown_sensor_triggered     ;
    Boolean  door_triggered       ;
    Boolean  hood_triggered       ;
    Boolean  trunk_triggered      ;
    Boolean  ign_triggered        ;
    Boolean  door_bypassed        ;
    Boolean  trunk_bypassed       ;
    Boolean  hood_bypassed        ;
	  Boolean  inst_trig_bypassed   ;
	
	  Boolean custom_triggered			;

    Boolean  zone_bypass_in_progress;
    
    Boolean  trunk_release_active ;

    Boolean  sensor_silent_arm    ;
    Boolean  full_silent_arm      ;

    Boolean  one_time_bypass      ;

    Boolean  ch3_status           ;
    Boolean  ch4_status           ;
    Boolean  ch5_status           ;
    Boolean  ch6_status           ;
	Boolean  programming_on		  ;
	
    UInt8    last_trigger         ;
		UInt8 	trig_src						;
		UInt8 	warn_src						;
		UInt8   sensor_trig_src			;

		Boolean custom_trig_zone   ;
		
		Boolean rpm_control_lock_status ;
	UInt8    event_memory[SS_TRIGGER_MEM_SIZE]      ;
    
	    
}Ss ;

//--------------------------------------------------------------------------//

typedef struct s_Sense
{
	/*WIRE SENSING*/
	Boolean 	door_wire          ;
	Boolean 	trunk_wire         ; 
	Boolean 	hood_wire          ;
	Boolean 	ign_wire           ;
	Boolean     brake_wire         ;
	Boolean     handbrake_wire     ;	
	Boolean 	glowplug_off_wire  ;
	UInt16     	rpm_wire           ;
	Boolean		headlight_wire		;
	Boolean   inst_trig_wire		;
		
	Boolean     sensor             ;
	Boolean     push_board         ;
	Boolean     push_antenna       ;

	Sensor_Temp	temp               ;	
	Sensor_Shock	shock			;
	
	double      voltage            ;

	HOOD_TYPE	hood_close_type    ;
	GEARBOX		gearbox_type       ;
	SAFETY   	safety_sw	       ;
	
	/*DATA SENSING*/
	Boolean     driver_door_data   ;	
	Boolean     other_door_data    ;
	Boolean     hood_data          ;	
	Boolean 	trunk_data         ;	
	Boolean 	ign_data           ;
	Boolean 	brake_data         ;
	Boolean 	handbrake_data     ;
	UInt16      rpm_data           ;
    UInt32      speed              ;
		UInt32 			speed_wire				;
		
	Boolean     glowplug_off_data  ;
    Boolean     key_in_data        ;
    Boolean     skd_is_present     ;
	
	/*SKIP SENSE*/
	Boolean     door_skip          ;
	Boolean     door_status_before_skip;
	
	DOOR_TYPE		door_close_type;
	TRUNK_TYPE	trunk_close_type;

}Sense ; 

//--------------------------------------------------------------------------//

typedef struct s_Sense_Source
{
	SOURCE door      ; 
	SOURCE trunk     ;
	SOURCE hood      ;
	SOURCE ign       ;
	SOURCE brake     ;
	SOURCE handbrake ;
	SOURCE glowplug  ;
	SOURCE rpm       ;	
	SOURCE speed		 ;
	SOURCE headlight ;

  Boolean rpm_updated_by_data ;
  Boolean rpm_updated_by_wire ;

	Boolean speed_updated_by_data ;
  Boolean speed_updated_by_wire ;

}Sense_Source ; 

//--------------------------------------------------------------------------//

typedef struct s_Rke_Use
{
	RKE_USE lock   ; 
	RKE_USE unlock ;
	RKE_USE trunk  ;

}Rke_Use ;

//--------------------------------------------------------------------------//


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void  reset_rpm_data( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Ss			status_ss              ; 
extern Rs       	status_rs              ;
extern Sense		status_sense	       ; 
extern Sense_Source status_sense_source    ;
extern Rke_Use      status_rke_use         ;
extern Boolean      cmd_source_d2d         ;
extern Boolean      status_rs_takeover	   ;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif
