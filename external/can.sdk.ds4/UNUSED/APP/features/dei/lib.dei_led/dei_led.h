/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_led.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __DEI_LED_H__
#define __DEI_LED_H__

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
    DEI_LED_NONE                = 0,	
    DEI_LED_RESET               = 1u << 0   ,
    DEI_LED_ARM             	= 1u << 1   ,
    DEI_LED_ARM_NOTIFICATION    = 1u << 2   ,

    DEI_LED_DISARM              = 1u << 3   ,
        
    DEI_LED_VALET_ON            = 1u << 4   ,
    DEI_LED_VALET_OFF           = 1u << 5   ,
    
    DEI_LED_TRUNK_BYPASSED      = 1u << 6   ,
	DEI_LED_DOOR_BYPASSED       = 1u << 7   ,
    DEI_LED_HOOD_BYPASSED       = 1u << 8   ,
    
    
    DEI_LED_TRUNK_TRIGGERED     = 1u << 9   ,    
	DEI_LED_SENSOR_TRIGGERED    = 1u << 10  ,
	DEI_LED_DOOR_TRIGGERED      = 1u << 11  ,
    DEI_LED_IGN_TRIGGERED       = 1u << 12  ,
	DEI_LED_HOOD_TRIGGERED      = 1u << 13  ,
        
    
	DEI_LED_RS_ERROR_RUNTIME    = 1u << 14  ,
	DEI_LED_RS_ERROR_OVERREV    = 1u << 15  ,
	DEI_LED_RS_ERROR_LOW_RPM    = 1u << 16  ,
	DEI_LED_RS_ERROR_TRANSMITTER= 1u << 17  ,
	DEI_LED_RS_ERROR_BRAKE      = 1u << 18  ,
	DEI_LED_RS_ERROR_HOOD       = 1u << 19  , 
	DEI_LED_RS_ERROR_MTS        = 1u << 20  , 
	DEI_LED_RS_ERROR_NEUTRAL    = 1u << 21  , 
	DEI_LED_RS_ERROR_LOW_BATT   = 1u << 22  , 
	DEI_LED_RS_ERROR_TRIGGER    = 1u << 23  , 
	DEI_LED_RS_ERROR_GLOWPLUG   = 1u << 24  ,
		
	DEI_LED_REMOTE_PROGRAMMING  = 1u << 25  ,
	DEI_LED_REMOTE_DELETE       = 1u << 26  ,	
	DEI_LED_ZAP					= 1u << 27  ,
	DEI_LED_TACHO_DELETE        = 1u << 28  ,
	
	DEI_LED_RS_TACHO_PRG        = 1u << 29  ,
	 
    DEI_LED_RESERVED            = 1u << 30  
    
}Dei_Led_Pattern;


#define LED_ENABLE		TRUE
//--------------------------------------------------------------------------//


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void    dei_led_init  ( void  ); 

Boolean dei_led_flash ( Dei_Led_Pattern pattern, Boolean led_cnfg );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


