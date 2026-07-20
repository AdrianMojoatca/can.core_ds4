/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: display_once.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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

    mapping of api: 
      t_offset -> maps to -> 0  (no wait time)
	  t_on -> maps to -> ON_TIME (timeout)
	  t_off -> maps to -> OFF_TIME = 0
	  pulse -> maps_to -> display once = 1
	  frames -> maps to -> display once = 1
  
      void led_flash ( Led_Flash_Mode mode , Led_Type type ,  UInt32 t_offset ,  UInt32 t_on , UInt32 t_off  , UInt16 pulse , UInt16 frames );
   
      Note: if timeout is equal to 0, then it implies a continuous LED display request. (which maps to t_on = 0xFFFF
   
   */
void display_once (Display_Type type, UInt32 timeout)
{	
  timeout_stop  (display_timeout) ;	
  /* Flash Both or Green or Red led depending on display type */
  if (type == BOTH_LED)
  {
    /* 0 offset and timout = t_on, 0 t_off, 1 pulse and 1 frame 
      Note: if timeout is equal to 0, then it implies a continuous LED display request. (which maps to t_on = 0xFFFF
	*/
    led_flash(LED_FLASH_NOW, LED_BOTH, 0, timeout ? timeout : 0xFFFF, 0 , timeout ? 1 : LED_LOOP , timeout ? 1 : LED_LOOP);
  }  
  else if (type == GREEN_LED)
  {
    /* 0 offset and timout = t_on, 0 t_off, 1 pulse and 1 frame 
      Note: if timeout is equal to 0, then it implies a continuous LED display request. (which maps to t_on = 0xFFFF
	*/
    led_flash(LED_FLASH_NOW, LED_GREEN, 0, timeout ? timeout : 0xFFFF, 0 , timeout ? 1 : LED_LOOP , timeout ? 1 : LED_LOOP);
  }
#ifdef PLATFORM_CM800
  else
  {
    /* 0 offset and timout = t_on, 0 t_off, 1 pulse and 1 frame 
      Note: if timeout is equal to 0, then it implies a continuous LED display request. (which maps to t_on = 0xFFFF
	*/
    led_flash(LED_FLASH_NOW, LED_RED, 0, timeout ? timeout : 0xFFFF, 0 , timeout ? 1 : LED_LOOP , timeout ? 1 : LED_LOOP);
  }
#else
	else if (type == RED_LED)
	{
		led_flash(LED_FLASH_NOW, LED_RED, 0, timeout ? timeout : 0xFFFF, 0 , timeout ? 1 : LED_LOOP , timeout ? 1 : LED_LOOP);
	}
	else if (type == BLUE_LED)
	{
		led_flash(LED_FLASH_NOW, LED_BLUE, 0, timeout ? timeout : 0xFFFF, 0 , timeout ? 1 : LED_LOOP , timeout ? 1 : LED_LOOP);
	}
	else
	{
		led_flash(LED_FLASH_NOW, LED_ALL, 0, timeout ? timeout : 0xFFFF, 0 , timeout ? 1 : LED_LOOP , timeout ? 1 : LED_LOOP);
	}
#endif

}

