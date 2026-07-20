/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __DEI_SS_H__
#define __DEI_SS_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "device.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
enum{
	DEI_SS_LED_REMOTE_INCREASE = 1,
	DEI_SS_LED_REMOTE_DECREASE,
	DEI_SS_LCD_REMOTE,
	DEI_SS_LED_REMOTE_DEFAULT
};

enum{
	DEI_SS_ARM_NONE = 0,
	DEI_SS_FULL_SILENT_ARM = 1,
	DEI_SS_SENSOR_SILENT_ARM = 2
};

//--------------------------------------------------------------------------//
#ifdef PLATFORM_933

	typedef enum
	{
		DEI_SS_NO_TRIGGER          ,
		DEI_SS_TRIGGER_ZONE_TRUNK  , 
		DEI_SS_TRIGGER_ZONE_SENSOR ,
		DEI_SS_TRIGGER_ZONE_DOOR   ,
		DEI_SS_TRIGGER_ZONE_IGN    ,
		DEI_SS_TRIGGER_ZONE_HOOD   ,
		DEI_SS_TRIGGER_ZONE_SENSOR2,
		DEI_SS_TRIGGER_ZONE_SENSOR3,
	}Dei_Ss_Trigger_Source ;

#else

	typedef enum
	{
		DEI_SS_NO_TRIGGER          ,
		DEI_SS_TRIGGER_ZONE_TRUNK  , 
		DEI_SS_TRIGGER_ZONE_SENSOR ,
		DEI_SS_TRIGGER_ZONE_DOOR   ,
		DEI_SS_TRIGGER_ZONE_IGN    ,
		DEI_SS_TRIGGER_ZONE_HOOD 

	}Dei_Ss_Trigger_Source ;
	
#endif

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void dei_ss_init  ( void );
UInt8 dei_ss_sensor_curr_aid( void );
UInt8 dei_ss_sensor_curr_chanl( void );
void dei_ss_sensor_curr_aid_reset( void );
void dei_ss_sensor_curr_chanl_reset( void );
UInt8 dei_ss_arm_cmd_status( void );
void dei_ss_silent_arm_updating(Boolean armed);

void  dei_ss_ignore_pts_ign				 ( void );
UInt8 get_ss_trigger_source(void);

void dei_ss_zone_bypass_start  ( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern UInt8 					dei_ss_sensor_adjust_src;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

