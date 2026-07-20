/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: prg_evt_ign.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "prg_private.h"
#include "intl_port.h"

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

void prg_evt_ign_on( void )
{        	       
	static Boolean menu_active = FALSE;
				
	if( (!get_ss_lock() || get_ss_valet()) && !get_rs_gwr() && !prg_use_door )  //prg_use_door uses INPUT_BLE_PRG_ENTER!
	{
		if( !menu_active )
		{
			ATOMIC( menu_active = TRUE );

			prg_menu_execute();

			ATOMIC( menu_active = FALSE );
		}
	}
}

//--------------------------------------------------------------------------//

void prg_evt_ign_off( void )
{
	os_tsk_wait(100);   //in case of dhp processing, make IVU to be available for receiving D4 message
	os_evt_signal( prg_evt , PRG_EVT_IGN_OFF ) ; 
	
	if (ble_prg_menu)
	{
		/* exit the pairing mode if ignition from on to off */
		ble_prg_menu = 0;
		body_prk_set_level(ble_prg_menu);
	}
}

#ifdef PLATFORM_933
//--------------------------------------------------------------------------//
void prg_menu_set( UInt8 level )
{
  ble_prg_menu = level;

  body_prk_set_level(level);
}

//--------------------------------------------------------------------------//
UInt8 prg_menu_get(void)
{
  return ble_prg_menu;
}

Boolean prg_exit_get(void)
{
  return prg_exit;
}

void prg_exit_set(Boolean val)
{
  prg_exit = val;
}

//--------------------------------------------------------------------------//
void ble_prg_evt_enter(void)
{
  os_evt_signal(prg_evt, PRG_EVT_BLE_PRG_ENTER);
}

//--------------------------------------------------------------------------//
void ble_prg_evt_exit(void)
{
  os_evt_signal(prg_evt, PRG_EVT_BLE_PRG_EXIT);  
}

UInt8 get_ble_prg_menu(void)
{
	return ble_prg_menu;
}

#endif
//--------------------------------------------------------------------------//
