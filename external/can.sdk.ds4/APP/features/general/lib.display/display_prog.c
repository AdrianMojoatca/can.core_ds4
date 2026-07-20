/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: display_prog.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "display_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define GREEN_ON_TIME    400     /* for 400ms */
#define GREEN_OFF_TIME   300     /* for 300ms */
#define GREEN_BREAK_TIME 400    /* for 1500ms*/
#define RED_ON_TIME      100     /* for 100ms */
#define RED_OFF_TIME	 300     /* for 300ms */
#define RED_BREAK_TIME   400    /* for 1500ms*/

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

      t_offset -> maps to -> BREAK_TIME  
	  t_on -> maps to -> ON_TIME 
	  t_off -> maps to -> OFF_TIME 
	  pulse -> maps_to -> option count or feature count 
	  frames -> maps to -> number of loops
  
      void led_flash ( Led_Flash_Mode mode , Led_Type type ,  UInt32 t_offset ,  UInt32 t_on , UInt32 t_off  , UInt16 pulse , UInt16 frames );
	
	*/
void display_prog (UInt8 feature, UInt8 option, Boolean feature_change)
{	
  	timeout_stop  (display_timeout) ;	
    /* if not feature change, then it is option change, so flash RED LED */
  	if(!feature_change)
  	{
    	led_flash(LED_FLASH_NOW  , 
				  LED_RED        , 
				  RED_BREAK_TIME , 
				  RED_ON_TIME    , 
				  RED_OFF_TIME   , 
				  option , 
				  1);

		led_flash(LED_FLASH_LATER , 
				  LED_GREEN       , 
				  (RED_BREAK_TIME+((RED_ON_TIME+RED_OFF_TIME)*option))+GREEN_BREAK_TIME, 
				  GREEN_ON_TIME   , 
				  GREEN_OFF_TIME  , 
				  feature         , 
				  LED_LOOP );

		led_flash(LED_FLASH_LATER , 
		          LED_RED         ,
				  (GREEN_BREAK_TIME+((GREEN_ON_TIME+GREEN_OFF_TIME)*feature))+RED_BREAK_TIME, 
				  RED_ON_TIME     , 
				  RED_OFF_TIME    , 
				  option          , 
				  LED_LOOP );				
  	}
  	/* feature change: so flash Green led */
  	else
  	{
			led_flash(	LED_FLASH_NOW    , 
					  	LED_GREEN        ,  
						GREEN_BREAK_TIME , 
						GREEN_ON_TIME    , 
						GREEN_OFF_TIME   , 
						feature , 
						1 );

			led_flash(	LED_FLASH_LATER , 
						LED_RED         , 
						(GREEN_BREAK_TIME+((GREEN_ON_TIME+GREEN_OFF_TIME)*feature))+RED_BREAK_TIME , 
						RED_ON_TIME     , 
						RED_OFF_TIME    , 
						option          , 
						LED_LOOP );
			
			led_flash  (LED_FLASH_LATER , 
						LED_GREEN       , 
						(RED_BREAK_TIME+((RED_ON_TIME+RED_OFF_TIME)*option))+GREEN_BREAK_TIME , 
						GREEN_ON_TIME   , 
						GREEN_OFF_TIME  , 
						feature         , 
						LED_LOOP );
	    				
  	}	    
}

