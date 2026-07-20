/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: prg_menu_remote_programming.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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

Boolean prg_menu_remote_programming( void )
{
#ifdef PLATFORM_933
  #define EVT		PRG_EVT_DOOR_CLOSED | PRG_EVT_IGN_OFF | PRG_EVT_REMOTE_PROGRAMMED | PRG_EVT_PUSH_EXT_PRESS | PRG_EVT_BLE_PRG_EXIT
#else
	#define EVT		PRG_EVT_DOOR_CLOSED | PRG_EVT_IGN_OFF | PRG_EVT_REMOTE_PROGRAMMED | PRG_EVT_PUSH_RELEASE
#endif
	
	UInt16 evt ;
	
	prg_exit = TRUE ; 

	reg_execute( prg_reg , PRG_FUNC_REMOTE_PROGRAMMING_ENTER , NULL );
		
	remote_prg_wait_event:
	{
		evt = os_evt_wait( prg_evt , EVT , EVT , 60000 ) ; 
		
		if( evt & PRG_EVT_REMOTE_PROGRAMMED )   //TODO: BLE card notify pairing success
		{
			reg_execute( prg_reg , PRG_FUNC_REMOTE_PROGRAMMING_DONE , NULL ) ; 

			goto remote_prg_wait_event;
		}

#ifdef DEV_LPC_236x
		if( evt & PRG_EVT_PUSH_RELEASE ) 
#else
		if( evt & PRG_EVT_PUSH_EXT_PRESS ) 
#endif
		{
			TRACE_DEI_PRG("\n\r[EXIT REMOTE PROGARMMING]\n\r");

			return TRUE ; 				
		}
	}

	#undef EVT

	return FALSE  ;
}
		

//--------------------------------------------------------------------------//

