/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: status_system.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __STATUS_SYSTEM_H__
#define __STATUS_SYSTEM_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
 #include "device.h"
 #include "ecm_vtacho.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------// 

#define SS_TRIGGER_MEM_SIZE			4

//--------------------------------------------------------------------------//
#ifdef PLATFORM_CM800
typedef enum 
{
	SOURCE_AUTO    = 1,
	SOURCE_WIRE    = 2,
    SOURCE_DATA    = 3,

}SOURCE ;  
#else
typedef enum 
{
  SOURCE_DATA    = 1,
	SOURCE_WIRE    = 2,
	SOURCE_AUTO    = 3,
	SOURCE_NONE		 = 4,

}SOURCE ;  
#endif

//--------------------------------------------------------------------------//

typedef enum 
{
	RKE_DISABLE    = 1,
	RKE_ENABLE     = 2,

}RKE_USE ;  

//--------------------------------------------------------------------------//

typedef enum
{
	GEARBOX_MANUAL   , 
	GEARBOX_AUTOMATIC

}GEARBOX ; 

//--------------------------------------------------------------------------//

typedef enum 
{
	SAFETY_RS_DISABLE  ,
	SAFETY_RS_ENABLE
}SAFETY ;

//--------------------------------------------------------------------------//

typedef enum
{
	HOOD_NORMALLY_CLOSE , 
	HOOD_NORMALLY_OPEN 

}HOOD_TYPE ;

//--------------------------------------------------------------------------//

typedef enum
{
	DOOR_NORMALLY_CLOSE , 
	DOOR_NORMALLY_OPEN 

}DOOR_TYPE ;

//--------------------------------------------------------------------------//

typedef enum
{
	TRUNK_NORMALLY_CLOSE , 
	TRUNK_NORMALLY_OPEN 

}TRUNK_TYPE ;

//--------------------------------------------------------------------------//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

void 	status_system_init 	( void );

/*-------------------------------------------------------------------------*/
/* enable/disable sensing of RKE 										   */
/*-------------------------------------------------------------------------*/
void 	set_rke_lock_use  	   	    ( RKE_USE   status     ); 	
void 	set_rke_unlock_use	   	    ( RKE_USE   status     ); 	
void 	set_rke_trunk_use 	   	    ( RKE_USE   status     ); 	
/*-------------------------------------------------------------------------*/
/* select sensing source                                                   */
/*-------------------------------------------------------------------------*/
void   	set_door_source				( SOURCE    source     ); 	
void   	set_trunk_source			( SOURCE    source     ); 	
void   	set_hood_source				( SOURCE    source     ); 	
void   	set_ign_source				( SOURCE    source     ); 	
void   	set_brake_source			( SOURCE    source     ); 	
void   	set_handbrake_source		( SOURCE    source     ); 	
void   	set_rpm_source				( SOURCE    source 	   ); 	
void   	set_glowplug_source			( SOURCE    source 	   ); 	
void		set_speed_source			( SOURCE		source		);
void 		set_headlight_source	( SOURCE source );

/*-------------------------------------------------------------------------*/
/* set / clear status 											   */
/*-------------------------------------------------------------------------*/
void 	set_driver_door_data_status ( Boolean   status     );	
void 	set_other_door_data_status 	( Boolean   status     );	
void 	set_door_wire_status	   	( Boolean   status     );	
void 	set_hood_data_status	   	( Boolean   status     );	
void 	set_hood_wire_status	   	( Boolean   status     );	
void 	set_trunk_data_status	   	( Boolean   status     );	
void	set_trunk_wire_status		( Boolean   status     );	
void	set_brake_data_status	   	( Boolean   status     );	
void	set_brake_wire_status	   	( Boolean   status     );	
void	set_handbrake_wire_status  	( Boolean   status     );	
void	set_handbrake_data_status  	( Boolean   status     );	
void	set_ign_wire_status	    	( Boolean   status     );	
void	set_ign_data_status	   	    ( Boolean   status     );
void    set_key_in_data_status      ( Boolean   status     );	
void	set_glowplug_off_wire_status( Boolean   status     );	
void	set_glowplug_off_data_status( Boolean   status     );	
void	set_rpm_wire			   	( UInt16    rpm        );	
void	set_rpm_data			   	( UInt16    rpm        );
void    set_rpm_timeout             ( UInt32    new_timeout);
void    set_speed                   ( UInt32    speed      );
void    set_speed_wire              ( UInt32    speed      );
void    set_skd_present             ( Boolean   is_present );
void    set_voltage                 ( double 	voltage    );	
void	set_safety_sw_status	   	( SAFETY 	status     );	
void	set_hood_close_type		   	( HOOD_TYPE type       );	
void	set_sensor_status		   	( Boolean   status     );	
void	set_push_board_status   	( Boolean   status     );	
void	set_push_antenna_status	   	( Boolean   status     );	
void    set_gearbox_type           	( GEARBOX   type       );	
void	set_temp_sensor_value	   	( UInt8     temp_value ); 	
void  	set_shock_sensor_present	( Boolean is_present	);
void  	set_shock_sensor_level		( UInt8 level 			);
void  set_shock_sensor_detect_bits( UInt8 bits )		;
void  set_shock_sensor_alert_bits( UInt8 bits )			;
void  	set_shock_sensor_db_num		( UInt8 num 			);
void  	set_shock_chanl_num			( UInt8 chanl 			);
void	set_temp_sensor_present	   	( Boolean   is_present ); 	
void  	set_temp_sensor_db_num		( UInt8 num 			);
void  	set_temp_chanl_num			( UInt8 chanl 			);
void    set_door_skip               ( void );
void    clr_door_skip               ( void );  	

void set_door_close_type ( DOOR_TYPE type )	;
void set_trunk_close_type ( TRUNK_TYPE type )	;
void set_headlight_wire_status( Boolean status );
void set_inst_trig_wire_status( Boolean status );

/*-------------------------------------------------------------------------*/
/* set / clear RS status and indications								   */
/*-------------------------------------------------------------------------*/
void 	set_rs_gwr				   	( Boolean   status     );   		
void 	set_rs_acc				   	( Boolean   status     );   		
void 	set_rs_ign				   	( Boolean   status     );   		
void 	set_rs_start			   	( Boolean   status     );   		
void 	set_rs_smart			   	( Boolean   status     );  
void 	set_rs_timer			   	( Boolean   status     );  
void 	set_rs_defogger			   	( Boolean   status     );  
void 	set_rs_turbo			   	( Boolean   status     );  
void 	set_rs_idle				   	( Boolean   status     );  
void    set_rs_idle_turbo_done		( Boolean   status     );
void 	set_rs_mts				   	( Boolean   status     );  
void 	set_rs_mts_progress		   	( Boolean   status     );  
void 	set_rs_mts_lock			   	( Boolean   status     );	
void 	set_rs_mts_prepared		   	( Boolean   status     );	
void 	set_rs_mts_exit_by_hood   	( Boolean   status     );
void 	set_rs_rpm_ref			   	( UInt16    rpm_ref    );	
void	set_rs_vtacho_ref		   	( UInt8     vtacho_ref );	
void 	set_rs_retry_count		   	( UInt8     retry_count);	
void 	set_rs_error_shutdown	   	( UInt8     err        );	
void 	set_rs_runtime_programmed  	( UInt8     runtime    );	
void 	set_rs_runtime_remaining   	( UInt8     runtime    );	
void 	set_rs_start_successful		( Boolean 	status 	   );
void 	set_rs_valet        		( Boolean 	status 	   );
void set_wire_ign_skip_status( Boolean status );
void set_rs_ign_off_status( Boolean status );
void set_rs_ign_disarm_oem( Boolean status );
void set_rs_takeover(Boolean status);
void set_rs_iid_engine_running(Boolean status);

/*-------------------------------------------------------------------------*/
/* set / clear SS status and indications								   */
/*-------------------------------------------------------------------------*/
void 	set_ss_arm				   	( Boolean   status 	   );		
void 	set_ss_lock				   	( Boolean   status 	   );		
void 	set_ss_trigger			   	( Boolean   status 	   );		
void 	set_ss_panic			   	( Boolean   status 	   );		
void 	set_ss_valet			   	( Boolean   status 	   );		
void 	set_ss_sensor_npc		   	( Boolean   status 	   );		
void 	set_ss_sensor_trigger	   	( Boolean   status 	   );	
void set_ss_sensor_trigger_all( Boolean status 				);
void  set_ss_sensor_trigger_src_history	( UInt8 src 						);
void 	set_ss_door_trigger		   	( Boolean   status 	   );		
void 	set_ss_hood_trigger		   	( Boolean   status 	   );		
void 	set_ss_trunk_trigger	   	( Boolean   status 	   );	    
void 	set_ss_ign_trigger		   	( Boolean   status     );		
void 	set_ss_door_bypass		   	( Boolean   status     );		
void 	set_ss_trunk_bypass		   	( Boolean   status     );		
void 	set_ss_hood_bypass		   	( Boolean   status     );		
void set_ss_inst_trig_bypass( Boolean status );
void 	set_ss_trunk_release	   	( Boolean   status     );		
void 	set_ss_sensor_silent_arm   	( Boolean   status     );		
void 	set_ss_full_silent_arm	   	( Boolean   status     );		
void 	set_ss_one_time_bypass	   	( Boolean   status     );		
void 	set_ss_ch3				   	( Boolean   status     );		
void 	set_ss_ch4				   	( Boolean   status     );
void 	set_ss_ch5				   	( Boolean   status     );
void 	set_ss_ch6				   	( Boolean   status     );
void 	set_ss_programming_on   	( Boolean   status     );
void 	set_ss_last_trigger		   	( UInt8     evt        );
void 	set_ss_trig_src						( UInt8 src 					);
void 	set_ss_warn_src						( UInt8 src 					);
void 	set_ss_memory_trigger_event	( UInt8     evt        );
void    set_zone_bypass_status      ( Boolean   status     );
void 	set_ss_custom_trigger( Boolean status );
void 	set_ss_custom_trigger_zone( Boolean status );
void	set_rpm_control_lock_status( Boolean status );

void set_cmd_source_d2d(Boolean status);

/*-------------------------------------------------------------------------*/
/* get usage status of RKE												   */
/*-------------------------------------------------------------------------*/
RKE_USE	get_rke_lock_use 		   	( void );
RKE_USE	get_rke_unlock_use		 	( void );
RKE_USE	get_rke_trunk_use 		 	( void );
/*-------------------------------------------------------------------------*/
/* get sensing source 													   */
/*-------------------------------------------------------------------------*/
SOURCE	get_door_source				( void );
SOURCE	get_trunk_source			( void );
SOURCE 	get_hood_source				( void );
SOURCE	get_ign_source				( void );
SOURCE	get_brake_source			( void );
SOURCE	get_handbrake_source		( void );
SOURCE	get_rpm_source				( void );
SOURCE	get_glowplug_source  	 	( void );
SOURCE	get_headlight_source  	 	( void );
/*-------------------------------------------------------------------------*/
/* get sensing status 													   */
/*-------------------------------------------------------------------------*/
Boolean get_door_status			 	( void );
Boolean get_driver_door_status      ( void );
Boolean get_other_door_status       ( void );
Boolean get_hood_status		 		( void );
Boolean get_trunk_status		 	( void );
Boolean get_ign_status			 	( void );
Boolean get_key_in_status           ( void );
Boolean get_brake_status		 	( void );
Boolean get_handbrake_status	 	( void );
Boolean get_glowplug_off_status		( void );
UInt16  get_rpm					 	( void );
UInt32  get_speed				 	( void );
Boolean get_skd_present             ( void );
double  get_voltage		 			( void );
Boolean get_temp_sensor_present	 	( void );
UInt8  	get_temp_sensor_value	 	( void );
UInt8  get_temp_sensor_db_num		( void );
UInt8  get_temp_chanl_num			( void );
Boolean  get_shock_sensor_present	( void );
UInt8  get_shock_sensor_level		( void );
UInt8  get_shock_sensor_detect_bits( void );
UInt8  get_shock_sensor_alert_bits( void );
UInt8  get_shock_sensor_db_num		( void );
UInt8  get_shock_chanl_num			( void );
Boolean get_sensor_status		 	( void );
Boolean get_push_board_status 	    ( void );
Boolean get_push_antenna_status	 	( void );
GEARBOX	get_gearbox_type         	( void );
DOOR_TYPE get_door_close_type( void );
HOOD_TYPE get_hood_close_type ( void );
TRUNK_TYPE get_trunk_close_type(void);
SAFETY	get_safety_sw_status        ( void );	
Boolean	get_headlight_wire_status ( void );
Boolean	get_inst_trig_wire_status ( void );

/*-------------------------------------------------------------------------*/
/* get RS status														   */
/*-------------------------------------------------------------------------*/
Boolean get_rs_gwr				 	( void );
Boolean get_rs_acc				 	( void );
Boolean get_rs_ign				 	( void );
Boolean get_rs_start			 	( void );
Boolean get_rs_smart			 	( void );
Boolean get_rs_timer			 	( void );
Boolean get_rs_manual_gearbox	 	( void );
Boolean get_rs_defogger			 	( void );
Boolean get_rs_turbo			 	( void );
Boolean get_rs_idle				 	( void );
Boolean get_rs_idle_turbo_done		( void );
Boolean get_rs_mts				 	( void );
Boolean get_rs_mts_progress		 	( void );
Boolean get_rs_mts_lock			 	( void );
Boolean get_rs_mts_prepared		 	( void );
Boolean get_rs_mts_exit_by_hood	 	( void );
UInt16 	get_rs_rpm_ref			 	( void );
Ecm_Vtacho_Ref 	get_rs_vtacho_ref		 	( void );
UInt8 	get_rs_retry_count		 	( void );
UInt8 	get_rs_error_shutdown	 	( void );
UInt8 	get_rs_runtime_programmed	( void );
UInt8 	get_rs_runtime_remaining 	( void );
Boolean get_rs_start_successful		( void );
Boolean get_rs_valet                ( void ); 
Boolean get_wire_ign_skip_status( void );
Boolean get_rs_ign_off_status( void );
Boolean get_rs_ign_disarm_oem( void );
Boolean get_rs_takeover(void);
Boolean get_rs_iid_engine_running(void);

/*-------------------------------------------------------------------------*/
/* get SS status														   */
/*-------------------------------------------------------------------------*/
Boolean get_ss_arm					( void );
Boolean get_ss_lock					( void );
Boolean get_ss_trigger				( void );
Boolean get_ss_panic				( void );
Boolean get_ss_valet				( void );
Boolean get_ss_sensor_npc			( UInt8 index );
Boolean get_ss_sensor_trigger		( UInt8 index );
Boolean get_ss_last_sensor_trigger( void );
UInt8   get_ss_sensor_trigger_src_history( void );
Boolean get_ss_door_trigger			( void );
Boolean get_ss_hood_trigger			( void );
Boolean get_ss_trunk_trigger		( void );
Boolean get_ss_ign_trigger			( void );
Boolean get_ss_door_bypass			( void );
Boolean get_ss_trunk_bypass			( void );
Boolean get_ss_hood_bypass			( void );
Boolean get_ss_inst_trig_bypass( void );
Boolean get_ss_trunk_release		( void );
Boolean get_ss_sensor_silent_arm	( void );
Boolean get_ss_full_silent_arm		( void );
Boolean get_ss_one_time_bypass		( void );
Boolean get_ss_ch3					( void );
Boolean get_ss_ch4					( void );
Boolean get_ss_ch5					( void );
Boolean get_ss_ch6					( void );
Boolean	get_ss_programming_on   	( void );
UInt8 	get_ss_last_trigger			( void );
UInt8 	get_ss_trig_src					( void );
UInt8 	get_ss_warn_src					( void );
UInt8 	get_ss_memory_trigger_event	( UInt8 pos );
Boolean get_zone_bypass_status      ( void );
Boolean get_ss_custom_trigger_zone( void );
Boolean get_rpm_control_lock_status( void );

Boolean get_cmd_source_d2d(void);
	



#endif
