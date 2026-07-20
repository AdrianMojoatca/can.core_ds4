/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/


//             <<< Use Configuration Wizard in Context Menu >>>             //

/*==========================================================================*/
// $Id: led_usr.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __LED_USR_H__
#define __LED_USR_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
typedef enum
{  
  LED_USR_DETECT_INIT               , // Solid Red
  LED_USR_DETECT_BP_SKIP            , // Solid orange  
  LED_USR_DETECT_CAN1               , // Flash Orange (Fast)
  LED_USR_DETECT_CAN2               , // Flash Green  (Fast)
  LED_USR_DETECT_KEY2GO             , // Flash Orange (Slow)
  LED_USR_DETECT_DONE               , // Green  3 Sec On
  LED_USR_DETECT_NO_BP_DONE         , // Orange 3 Sec On   
  LED_USR_DETECT_NO_VIN_DONE        , // Red    3 Sec on
  LED_USR_DETECT_GREEN_ON           , // Solid Green       
  LED_USR_DETECT_FLASH_RED_FAST     , // Flash Red         (Fast) 
  LED_USR_DETECT_FLASH_RED_SLOW     , // Flash Red         (Slow) 
  LED_USR_DETECT_FLASH_GREEN_SLOW   , // Flash Green       (Slow)  
   
}Led_Usr_Detect ;

//--------------------------------------------------------------------------//
typedef enum
{
  LED_USR_ERR_CAN_DEV0      =1  ,
  LED_USR_ERR_CAN_DEV1      =2  ,
  LED_USR_ERR_BP_LOG        =3  ,
  LED_USR_ERR_BP_TIMEOUT    =4  ,
  LED_USR_ERR_ISO1          =5  , 
  LED_USR_ERR_ISO2          =6  ,
  LED_USR_ERR_MUX           =7  ,
  LED_USR_ERR_ALL_STATE_SKIP=8  ,
  LED_USR_ERR_IGNITION      =9  ,
  LED_USR_ERR_INVALID_VIN   =10 ,
  LED_USR_ERR_INVALID_BCM   =11 ,   
  LED_USR_ERR_SPEED_VALUE   =12 ,     

} Led_Usr_Detect_Err_Code;


//--------------------------------------------------------------------------//
typedef enum
{
  NO_SHUTDOWN                  = 0 ,
  SHUTDOWN_RUNSAFE             = 1 ,
  LED_USR_CUSTOM_ERR1          = 1 ,
  SHUTDOWN_BRAKE               = 2 ,
  LED_USR_CUSTOM_ERR2          = 2 ,
  SHUTDOWN_NO_KEY              = 3 ,
  LED_USR_CUSTOM_ERR3          = 3 ,
  SHUTDOWN_SPEED_DETECT        = 4 , 
  LED_USR_CUSTOM_ERR4          = 4 ,
  RXT_DISPLAY_ERR_START_FAIL   = 5 ,
  LED_USR_CUSTOM_ERR5          = 5 ,
  RXT_DISPLAY_ERR_STOP_FAIL    = 6 ,
  RXT_DISPLAY_ERR_RFTD_NOT_ENA = 7 ,
  RSR_DISPLAY_ERR_RFTD_NOT_ENA = 7 ,
  RXT_DISPLAY_ERR_OEM_DIS      = 8 ,

  RSR_FEATURE_ERROR            = 9 ,  
  
  LED_USR_PTS_CTL_DISABLE      = 10 ,

  RFTD2_OBD_NOT_ACTIVE         = 21,

  LED_USR_ERR_COUNT                , // it's can be use to start a specific value

} Led_Usr_Op_Err_Code;



/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

EXTERN_C_ENTER

void led_usr_init             (void)       ;

void led_usr_detect           (Led_Usr_Detect          led_usr               ) ;
void led_usr_detect_err       (Led_Usr_Detect_Err_Code err_code , UInt8 count) ;
void led_usr_op_err           (Led_Usr_Op_Err_Code     err_code , UInt8 count) ;
 
EXTERN_C_LEAVE

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

#endif

