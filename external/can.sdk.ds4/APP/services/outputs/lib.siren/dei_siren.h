/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_siren.h 17934 2012-10-29 19:23:36Z martin.bouchard $
/*==========================================================================*/

#ifndef __DEI_SIREN_H__
#define __DEI_SIREN_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "os.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//

typedef enum
{    
    DEI_SIREN_STOP               		= 0  ,
    DEI_SIREN_ARM             	 		= 1  ,
    DEI_SIREN_ARM_NOTIFICATION   		= 2  ,				// Any door, trunk, hood open while arming
    DEI_SIREN_ZONE_BYPASS        		= 3  ,				// 
    DEI_SIREN_DISARM_NO_EVENT  	 		= 4  ,				// Disarm (unlock), no event
    DEI_SIREN_DISARM_TRIGGER_ON	 		= 5  ,				// Disarm with alarm history
    DEI_SIREN_DISARM_NPC_ON    	 		= 6  ,				// 
    DEI_SIREN_CHIRP_THEN_TRIGGER 		= 7  ,				
    DEI_SIREN_TRIGGER            		= 8  ,
    DEI_SIREN_WARNING          	 		= 9  ,				// Shock, tilt warning
		DEI_SIREN_CAR_FINDER         		= 10 ,				// Car finder triggered
		DEI_SIREN_DISARM_LOW_BATT    		= 11 ,				// 
		DEI_SIREN_REMOTE_PROGRAMMING_ENTER 	= 12 , 
		DEI_SIREN_REMOTE_LEARNED      	= 13 , 
		DEI_SIREN_REMOTE_DELETE_ENTER      	= 14 , 
		DEI_SIREN_TACHO_DELETE_ENTER       	= 15 , 
		DEI_SIREN_PROGRAMMING_EXIT         	= 16 , 
		DEI_SIREN_FULL_SILENT_ARM        	= 17 ,
		DEI_SIREN_ZAP_ENTER                 = 18,  
		DEI_SIREN_SENSOR_MAX_LIMIT					= 19,
	  DEI_SIREN_SENSOR_MIN_LIMIT					= 20,
		DEI_SIREN_SENSOR_LEVEL_UP 					= 21,
	  DEI_SIREN_SENSOR_LEVEL_DOWN     		= 22,
	  DEI_SIREN_SENSOR_DEFAULT_SETTING 		= 23,
		DEI_SIREN_SENSOR_ADJUST_ENTER				= 24,
		DEI_SIREN_VALET_MODE_ON							= 25,
		DEI_SIREN_VALET_MODE_OFF      			= 26,
		DEI_SIREN_SENSOR_PROGRAMMING_EXIT  	= 27,
    DEI_SIREN_SECOND_ARM       	 				= 28,			// Arming while already armed
		
// new msg numbers for siren audio files
    DEI_SIREN_RS_VALET_ON = 29,
    DEI_SIREN_RS_VALET_OFF = 30,
    DEI_SIREN_COMBO_VALET_ON = 31,
    DEI_SIREN_COMBO_VALET_OFF = 32,
		
		DEI_SIREN_ZONE_DOOR = 33,
		DEI_SIREN_ZONE_TRUNK = 34,
		DEI_SIREN_ZONE_HOOD = 35,
		DEI_SIREN_ZONE_IGNITION = 36,
		DEI_SIREN_ZONE_SHOCK = 37,
		DEI_SIREN_ZONE_TILT = 38,
		DEI_SIREN_ZONE_OTHER = 39,
		
		DEI_SIREN_PASSIVE_ARMING = 40,
		
		DEI_SIREN_TIMER_MODE_ON = 44,
		DEI_SIREN_TIMER_MODE_OFF = 45,
		DEI_SIREN_LOW_TEMP_MODE_ON = 46,
		DEI_SIREN_LOW_TEMP_MODE_OFF = 47,
    DEI_SIREN_SENSOR_WARNING 		= 50,
    DEI_SIREN_SENSOR_TRIGGER 		= 51,
		DEI_SIREN_REMOTE_PROGRAMMING_DONE  	= 52 , 

}Dei_Siren_Chirp;



//--------------------------------------------------------------------------//

#define AS_SIREN_REMOTE_PROGRAMMED     DEI_SIREN_DISARM_NO_EVENT 
#define AS_SIREN_TACH_PROGRAMMING      DEI_SIREN_DISARM_NPC_ON 
#define AS_SIREN_TACH_PROGRAMMED       DEI_SIREN_DISARM_NO_EVENT
#define AS_SIREN_PRG_CHIRPS_CONFIRM    DEI_SIREN_DISARM_NO_EVENT

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
Boolean dei_siren_chirp ( Dei_Siren_Chirp chirp );
void dei_siren_send_zone(void);
void dei_siren_send_zone_trigger(void);
void dei_siren_send_zone_npc(void);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


