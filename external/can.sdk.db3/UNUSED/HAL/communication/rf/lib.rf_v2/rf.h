/*==========================================================================*/
// $Id: rf.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef _RF_H_
#define _RF_H_

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "device.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
typedef enum              // order is critical
{ 
  NO_RF  = 0x01 ,
  RF_TD         ,       // rftd
  RF_TD2        ,       // SmartStart rf
  RF_DTS_       ,       // datastart rf
  RF_DEI_       ,       // dei xx rf
  RF_AST_       ,       // astro xx rf
  RF_ATST_      ,       // autostart xx rf
  

  RF_ID_COUNT    ,      // number of type of RF

}Rf_Id;

//--------------------------------------------------------------------------//
typedef enum
{
  RF_ICON_MIN = 0,
  ICON_LOCK         = RF_ICON_MIN,
  ICON_ARMED        ,
  ICON_IGNITION     ,
  ICON_PARKING      ,
  ICON_HORN         ,
  ICON_VALET        ,
  ICON_STARTED      ,
  ICON_SUCCESS      ,
  ICON_PANIC        ,
  ICON_DOOR         ,
  ICON_TRUNK        ,
  ICON_HOOD         ,
  ICON_WARN         ,
  ICON_SHOCK        ,
  ICON_TIME         ,
  ICON_TEMP         ,
  ICON_TEMP_TYPE    ,
  ICON_OTP          ,
  ICON_NOT_READY    ,
  ICON_DTC          ,
  ICON_ALERT        ,
  ICON_RS_ARMED     ,
  ICON_RS_HORN      ,
  ICON_RS_PANIC     ,   


  RF_ICON_COUNT , //always the last
 
}Rf_Icon_Def ;
//--------------------------------------------------------------------------//
typedef enum
{
  NO_SOUND           ,
  SOUND_LOCK         ,
  SOUND_UNLOCK       ,
  SOUND_ARMED        ,
  SOUND_DISARMED     ,
  SOUND_IGNITION     ,
  SOUND_PARKING      ,
  SOUND_HORN         ,
  SOUND_VALET        ,
  SOUND_STARTED      ,
  SOUND_STOPPED      ,
  SOUND_SUCCESS      ,
  SOUND_FAIL         ,
  SOUND_PANIC        ,
  SOUND_DOOR         ,
  SOUND_TRUNK        ,
  SOUND_HOOD         ,
  SOUND_WARN         ,
  SOUND_SHOCK        ,
  SOUND_TIME         ,
  SOUND_TEMP         ,
  SOUND_OTP          ,
  SOUND_WEAHTER      ,
  SOUND_EXT_TIME     ,

  RF_SOUND_COUNT , //always the last
 
}Rf_Sound_Def ;

//--------------------------------------------------------------------------//
typedef struct s_Icon
{
  // private
  UInt8  bit[(RF_ICON_COUNT / 8) + 1] ;

}Icon;
//--------------------------------------------------------------------------//
typedef struct s_Rf_Time
{
  UInt8   value        ;

}Rf_Time ;

//--------------------------------------------------------------------------//
typedef struct s_Rf_Temp
{

  SInt16   value        ;

}Rf_Temp ;

//--------------------------------------------------------------------------//
typedef struct s_Rf_Data
{
  Icon     icon   ;
  Rf_Sound_Def sound ;
//  Rf_Temp  temp  ;
//  Rf_Time  time    ;

}Rf_Data ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Rf_Data rf_data ;
extern Rf_Data rf_support ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
void rf_icon_set     (Rf_Icon_Def icon, Boolean update) ;
void rf_icon_clr     (Rf_Icon_Def icon, Boolean update) ;
void rf_icon_clr_all (Boolean update) ;

void rf_sound_set    (Rf_Sound_Def sound, Boolean update) ;

void rf_update       (void) ;

void rf_init         (void) ;
 
//--------------------------------------------------------------------------//

#endif



