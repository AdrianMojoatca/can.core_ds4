/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_vars.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ss_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
#ifdef PLATFORM_CM800
	Sensor  				ss_sensor         	 ;
	int  pop_trunk_state;
#else
	Sensor					ss_sensor[3]	;
	Sensor  				ss_sensor_ext   ;
#endif
System_Type          ss_system_type;
Boolean 				dei_ss_2nd_disarm 	 ;
Dei_Ss_Sensor_Bypass	dei_ss_sensor_bp     ; 
Boolean                 dei_ss_trigger_enable;
UInt8 					dei_ss_sensor_adjust_src;
UInt8					dei_ss_sensor_num;	
UInt8 					dei_ss_curr_sensor_aid;
UInt8					dei_ss_curr_sensor_chanl;
UInt8					dei_ss_curr_sensor_idx=0;
UInt8 					dei_ss_arm_cmd;
Boolean 				dei_ss_alarm_hist_report_on;
Vrtc 					dei_ss_trg_history_timeout;
Boolean				dei_ss_2nd_arm		;
Vrtc          dei_ss_door_skip_vrtc_timer;
Boolean       dei_ss_door_is_skip;

Dei_Ss_Trigger_Source current_trg_zone;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

