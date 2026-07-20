/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: display_flash.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "display_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/** lib.display will only be a wrapper functionality 
   this library will use lib.led in order to display the LED pattrens requested 

   Mapping of api: 
      t_offset -> maps to -> 0 (flash immediately, no wait time)
	  t_on -> maps to -> flash_time_on
	  t_off -> maps to -> flash_time_off
	  pulse -> maps_to -> flash_count
	  frames -> maps to -> 1
  
      void led_flash ( Led_Flash_Mode mode , Led_Type type ,  UInt32 t_offset ,  UInt32 t_on , UInt32 t_off  , UInt16 pulse , UInt16 frames );
*/

void display_flash (Display_Type type , UInt16 flash_count, UInt16 flash_time_on, UInt16 flash_time_off)
{	
  timeout_stop  (display_timeout) ;	
  /* Flash Both or Green or Red led depending on display type */
  if (type == BOTH_LED)
  {
    led_flash(LED_FLASH_NOW, LED_BOTH, 0, flash_time_on, flash_time_off, flash_count, 1);
  }  
  else if (type == GREEN_LED)
  {
    led_flash(LED_FLASH_NOW, LED_GREEN, 0, flash_time_on, flash_time_off, flash_count, 1);
  }
#ifdef PLATFORM_CM800
  else
  {
    led_flash(LED_FLASH_NOW, LED_RED, 0, flash_time_on, flash_time_off, flash_count, 1);
  }
#else
  else if (type == RED_LED)
  {
    led_flash(LED_FLASH_NOW, LED_RED, 0, flash_time_on, flash_time_off, flash_count, 1);
  }
  else if (type == BLUE_LED)
  {
    led_flash(LED_FLASH_NOW, LED_BLUE, 0, flash_time_on, flash_time_off, flash_count, 1);
  }
  else
  {
    led_flash(LED_FLASH_NOW, LED_ALL, 0, flash_time_on, flash_time_off, flash_count, 1);
  }

		#endif

}

