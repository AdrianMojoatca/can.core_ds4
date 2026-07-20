/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: display_error.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "display_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define ERROR_ON_TIME 	 200  /* for 200ms */
#define ERROR_OFF_TIME	 400  /* for 400ms */
#define ERROR_PAUSE_TIME 800  /* for 800ms */

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
   
      t_offset -> maps to -> ERROR_PAUSE_TIME
	  t_on -> maps to -> ERROR_ON_TIME
	  t_off -> maps to -> ERROR_OFF_TIME
	  pulse -> maps_to -> error_code
	  frames -> maps to -> display_count
  
      void led_flash ( Led_Flash_Mode mode , Led_Type type ,  UInt32 t_offset ,  UInt32 t_on , UInt32 t_off  , UInt16 pulse , UInt16 frames );
   
   
   */
void display_error (UInt16 err_code, UInt8 display_count)
{	
  timeout_stop  (display_timeout) ;	

  led_flash(LED_FLASH_NOW, LED_RED, ERROR_PAUSE_TIME, ERROR_ON_TIME , ERROR_OFF_TIME, err_code , display_count);

}

