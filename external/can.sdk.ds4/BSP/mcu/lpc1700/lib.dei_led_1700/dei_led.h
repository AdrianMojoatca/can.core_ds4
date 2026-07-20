/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_led.h 17934 2012-10-29 19:23:36Z martin.bouchard $
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

#define LED_ENABLE		(dei_feature_get( DEI_FEAT_ANTENNA_LED_FLASH_OPTION ) == 1)

#define LED_POS_NUM_FLASH		0
#define LED_POS_PULSE				4
#define LED_POS_OFFSET			5
#define LED_POS_INVER				7

#define LED_BITS_NUM_FLASH		4
#define LED_BITS_PULSE				1
#define LED_BITS_OFFSET				2
#define LED_BITS_INVER				1

#define LED_MASK_NUM_FLASH		((1<<LED_BITS_NUM_FLASH)-1)
#define LED_MASK_PULSE				((1<<LED_BITS_PULSE)-1)
#define LED_MASK_OFFSET				((1<<LED_BITS_OFFSET)-1)
#define LED_MASK_INVER				((1<<LED_BITS_OFFSET)-1)

#define LED_NUM_FLASH(led_patt)		((led_patt >> LED_POS_NUM_FLASH) & LED_MASK_NUM_FLASH)
#define LED_PULSE(led_patt)			  ((led_patt >> LED_POS_PULSE) & LED_MASK_PULSE)
#define LED_OFFSET(led_patt)	    ((led_patt >> LED_POS_OFFSET) & LED_MASK_OFFSET)
#define LED_INVER(led_patt)				((led_patt >> LED_POS_INVER) & LED_MASK_INVER)

#define LED_FLASH_LONG_OFFSET			0
#define LED_FLASH_MED_OFFSET      1
#define LED_FLASH_SHORT_OFFSET    2
#define LED_FLASH_SPCL_OFFSET     3

#define LED SHORT_FLASH           0
#define LED_LONG_FLASH            1

#define SHORT_PULSE				0		// 300ms
#define LONG_PULSE				1		// 700ms

#define LONG_OFFSET				0		// 1.5sec
#define MED_OFFSET				1		// 1 sec
#define SHORT_OFFSET			2   // 700ms
#define SPEC_OFFSET				3		// not really any offset just continuous pulse, special mode to get the corrcet pulse width

#define INVER_ALLOW				0
#define INVER_NOT_ALLOW		1


#define LED_PATTERN(inver, offset, t_on_off, flash_num) 		(((offset & LED_MASK_OFFSET) << LED_POS_OFFSET) |\
																											((t_on_off & LED_MASK_PULSE) << LED_POS_PULSE) |\
																											((flash_num & LED_MASK_NUM_FLASH) << LED_POS_NUM_FLASH) |\
																											((inver & LED_MASK_INVER) << LED_POS_INVER))

typedef enum
{
	LED_NO_FLASH,
	LED_1_FLASH,
	LED_2_FLASH,
	LED_3_FLASH,
	LED_4_FLASH,
	LED_5_FLASH,
	LED_6_FLASH,
	LED_7_FLASH,
	LED_8_FLASH,
	LED_9_FLASH,
	LED_10_FLASH,
	LED_11_FLASH,
	LED_FLASH_FOREVER
}Led_Flash_Num;


typedef enum
{
    DEI_LED_NONE                = 0,    
//    DEI_LED_RESET               = 1u << 0   ,
    DEI_LED_ARM             	= 1u << 1   ,
    DEI_LED_ARM_NOTIFICATION    = 1u << 2   ,

    DEI_LED_DISARM              = 1u << 3   ,
        
    DEI_LED_VALET_ON            = 1u << 4   ,
//    DEI_LED_VALET_OFF           = 1u << 5   ,
    
    DEI_LED_TRUNK_BYPASSED      = 1u << 5   ,
	DEI_LED_DOOR_BYPASSED       = 1u << 6   ,
    DEI_LED_HOOD_BYPASSED       = 1u << 7   ,
    
    
    DEI_LED_TRUNK_TRIGGERED     = 1u << 8   ,    
	DEI_LED_SHOCK_SENSOR_TRIGGERED    = 1u << 9  ,
	DEI_LED_DOOR_TRIGGERED      = 1u << 10  ,
    DEI_LED_IGN_TRIGGERED       = 1u << 11  ,
	DEI_LED_HOOD_TRIGGERED      = 1u << 12  ,
        
    
	DEI_LED_RS_ERROR_RUNTIME    = 1u << 13  ,
	DEI_LED_RS_ERROR_OVERREV    = 1u << 14  ,
	DEI_LED_RS_ERROR_LOW_RPM    = 1u << 15  ,
	DEI_LED_RS_ERROR_TRANSMITTER= 1u << 16  ,
	DEI_LED_RS_ERROR_BRAKE      = 1u << 17  ,
	DEI_LED_RS_ERROR_HOOD       = 1u << 18  , 
	DEI_LED_RS_ERROR_MTS        = 1u << 19  , 
	DEI_LED_RS_ERROR_NEUTRAL    = 1u << 20  , 
	DEI_LED_RS_ERROR_LOW_BATT   = 1u << 21  , 
	DEI_LED_RS_ERROR_TRIGGER    = 1u << 22  , 
	DEI_LED_RS_ERROR_GLOWPLUG   = 1u << 23  ,
		
	DEI_LED_REMOTE_PROGRAMMING  = 1u << 24  ,
	DEI_LED_REMOTE_DELETE       = 1u << 25  ,	
	DEI_LED_ZAP					= 1u << 26  ,
	DEI_LED_TACHO_DELETE        = 1u << 27  ,
	
	DEI_LED_RS_TACHO_PRG        = 1u << 28  ,
	
	DEI_LED_TILT_SENSOR_TRIGGERED = 1u << 29,
	DEI_LED_UNKNOWN_SENSOR_TRIGGERED	= 1u << 30 ,
	
  DEI_LED_HHU_CONV_ALERT					= 0xFF,

	// same patterns as ffrom DEI_LED_RS_ERROR_RUNTIME - DEI_LED_RS_ERROR_GLOWPLUG for sensor adjust mode

	DEI_LED_SENSOR1_ADJUST    = 1u << 13  ,
	DEI_LED_SENSOR2_ADJUST    = 1u << 14  ,
	DEI_LED_SENSOR3_ADJUST    = 1u << 15  ,
	DEI_LED_SENSOR4_ADJUST		= 1u << 16  ,
	DEI_LED_SENSOR5_ADJUST    = 1u << 17  ,
	DEI_LED_SENSOR6_ADJUST    = 1u << 18  , 
	DEI_LED_SENSOR7_ADJUST    = 1u << 19  , 
	DEI_LED_SENSOR8_ADJUST    = 1u << 20  , 
	DEI_LED_SENSOR9_ADJUST   	= 1u << 21  , 
	DEI_LED_SENSOR10_ADJUST   = 1u << 22  , 
	DEI_LED_SENSOR11_ADJUST   = 1u << 23  ,

    DEI_LED_RESERVED            = 1u << 30  
    
}Dei_Led_Pattern;

#define DEI_LED_RESET  DEI_LED_DISARM
#define AS_LED_REMOTE_PRG_SUCCESS  LED_RS_TACHO_PRG	//share the same pattern

//--------------------------------------------------------------------------//


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void    dei_led_init  ( void  ); 
void    dei_led_patt_reset ( void );

Boolean dei_led_flash ( Dei_Led_Pattern pattern , Boolean led_cnfg);

void dei_led_resume(void);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


