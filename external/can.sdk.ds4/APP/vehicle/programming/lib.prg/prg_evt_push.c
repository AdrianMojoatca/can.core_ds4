/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: prg_evt_push.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "prg_private.h"
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

void prg_evt_push_release( void )
{
	ATOMIC( prg_menu ++ ); 

	os_evt_signal( prg_evt , PRG_EVT_PUSH_RELEASE ) ;
}

//--------------------------------------------------------------------------//

void prg_evt_push_ext( void )
{
	os_evt_signal( prg_evt , PRG_EVT_PUSH_EXT_PRESS ) ;
}

//--------------------------------------------------------------------------//

void prg_evt_push_ext_release( void )
{
	os_evt_signal( prg_evt , PRG_EVT_PUSH_EXT_RELEASE );
}

//--------------------------------------------------------------------------//

void prg_evt_remote_pairing( void )
{
	prg_menu += 1;
	os_evt_signal( prg_evt , PRG_EVT_PUSH_EXT_PRESS );
}

//--------------------------------------------------------------------------//

void prg_evt_remote_delete( void )
{
	prg_menu += 2;
	os_evt_signal( prg_evt , PRG_EVT_PUSH_EXT_PRESS );
}

//--------------------------------------------------------------------------//
void prg_evt_zap( void )
{
	prg_menu += 3;
	os_evt_signal( prg_evt , PRG_EVT_PUSH_EXT_PRESS );
}

//--------------------------------------------------------------------------//
void prg_evt_tach_delete( void )
{
	prg_menu += 4;
	os_evt_signal( prg_evt , PRG_EVT_PUSH_EXT_PRESS );
}

//--------------------------------------------------------------------------//
void prg_menu_select_tap_hold ( void )
{
#ifdef PLATFORM_933
  if(dei_system_prg_mode_get() == FALSE)
    return;
#endif  
	prg_menu += 1;
	os_evt_signal( prg_evt , PRG_EVT_PUSH_EXT_PRESS );
}

//--------------------------------------------------------------------------//
void prg_menu_select_two_tap_hold ( void )
{
#ifdef PLATFORM_933
  if(dei_system_prg_mode_get() == FALSE)
    return;
#endif  
	prg_menu += 2;
	os_evt_signal( prg_evt , PRG_EVT_PUSH_EXT_PRESS );
}

//--------------------------------------------------------------------------//
void prg_menu_select_three_tap_hold ( void )
{
#ifdef PLATFORM_933
  if(dei_system_prg_mode_get() == FALSE)
    return;
#endif  
	prg_menu += 3;
	os_evt_signal( prg_evt , PRG_EVT_PUSH_EXT_PRESS );
}
