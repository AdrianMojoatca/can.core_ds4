/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id:
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "led_private.h"
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

//--------------------------------------------------------------------------//

Boolean   led_flash ( Led_Flash_Mode mode , Led_Type type ,  UInt32 t_offset ,  UInt32 t_on , UInt32 t_off  , UInt16 pulse , UInt16 frames )
{
#ifdef PLATFORM_CM800
	static const Out_Func out_func[] = { (Out_Func)led_ctrl_green , (Out_Func)led_ctrl_red , (Out_Func)led_ctrl_yellow };
#else
	static const Out_Func out_func[] = { (Out_Func)led_ctrl_green , (Out_Func)led_ctrl_red , (Out_Func)led_ctrl_yellow, (Out_Func)led_ctrl_blue, (Out_Func)led_ctrl_all };
#endif
	
	Boolean ret = FALSE ;
    
    led_init(); 

	if( !led_lock && led_atomic_request() )
	{
		if( mode == LED_FLASH_NOW ) led_reset();
	
		if( pulse && frames )
		{
				ret = out_fifo_insert( led_fifo , out_func[type] , t_offset/LED_BASE_TIME , t_on/LED_BASE_TIME , t_off/LED_BASE_TIME , pulse , frames );
		}
		else
		{
				ret = TRUE ; 
		}			
        
		led_atomic_release();
	}

	return ret ;
}

//--------------------------------------------------------------------------//

void led_flash_lock( void )
{
    led_lock = TRUE ;
}

//--------------------------------------------------------------------------//

void led_flash_unlock( void )
{
    led_lock = FALSE ;
}

//--------------------------------------------------------------------------//
