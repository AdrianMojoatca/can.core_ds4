/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: brand.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __BRAND_H__
#define __BRAND_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef enum 
{
  NO_BRAND                ,

  BRAND_XPRESS_START      , 

  BRAND_VP_1BTN           , 
  BRAND_VP_SC             ,
  
  BRAND_CLIFFORD_1BTN     ,   
  BRAND_CLIFFORD_SC       ,

  BRAND_AVITAL            ,
 
  BRAND_AUTOMATE          ,
 
  BRAND_UNGO              ,

  BRAND_AUTOSTART_LED     , 
  BRAND_AUTOSTART_LCD1    , // Small Lcd remote
  BRAND_AUTOSTART_LCD2    , // Big   Lcd remote

  BRAND_AUTOSTART_LED_HDR  ,
  BRAND_AUTOSTART_LCD1_HDR , // Small Lcd remote
  BRAND_AUTOSTART_LCD2_HDR , // Big   Lcd remote
 
  BRAND_ASTROSTART_LED1    ,  
  BRAND_ASTROSTART_LED2    ,
  BRAND_ASTROSTART_LCD1    ,
  BRAND_ASTROSTART_LCD2    ,
  
  
  BRAND_AUTOSTART_RFK_2800 , // Autostart casing with DEI RF module inside
  
  BRAND_AUTOSTART_NGRF    ,

  BRAND_HYUNDAI_CANADA    ,
 
  BRAND_VP_ASK            ,  // Low cost noisy RF 



  BRAND_COUNT

}Brand_Id ;

//--------------------------------------------------------------------------//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Boolean brand_init    (void);
Boolean brand_pre_init(void);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Brand_Id brand_id ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

