/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: prg.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __PRG_MENU_H__
#define __PRG_MENU_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "reg.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//

typedef enum
{
	PRG_FUNC_REMOTE_PROGRAMMING_ENTER , 
	
	PRG_FUNC_REMOTE_PROGRAMMING_DONE  ,
	
	PRG_FUNC_REMOTE_DELETE_ENTER      ,

	PRG_FUNC_REMOTE_DELETE_DONE       ,
	
	PRG_FUNC_TACHO_DELETE_ENTER       ,

	PRG_FUNC_TACHO_DELETE_DONE        ,

	PRG_FUNC_ZAP_ENTER                ,

	PRG_FUNC_ZAP_DONE                 ,
	
	PRG_FUNC_EXIT

}Prg_Func ; 
	
//--------------------------------------------------------------------------//


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Boolean prg_init  		 		 ( void  ); 

void    prg_evt_ign_on	 		 ( void  );
void    prg_evt_ign_off  		 ( void  );

void 	prg_evt_push_release	 ( void  );
void    prg_evt_push_ext		 ( void  );
void    prg_evt_push_ext_release ( void  );
void 		prg_evt_remote_pairing( void );
void 		prg_evt_remote_delete( void );
void 		prg_evt_zap( void );
void 		prg_evt_tach_delete( void );
void		prg_menu_select_tap_hold( void );
void		prg_menu_select_two_tap_hold( void );
void		prg_menu_select_three_tap_hold( void );

void    prg_evt_door_close		 ( void  );
void    prg_evt_lock     		 ( void  );
void    prg_evt_remote_programmed( void  );

void    prg_func_register		 ( Reg_Func func , Prg_Func prg_func );

void 	prg_menu_execute			( void );
#ifdef PLATFORM_933
void prg_menu_set( UInt8 level );
UInt8 prg_menu_get(void);
Boolean prg_exit_get(void);
void prg_exit_set(Boolean val);
void ble_prg_evt_enter(void);
void ble_prg_evt_exit(void);

UInt8 get_ble_prg_menu(void);
void  set_prg_use_door(Boolean ctrl);
#endif

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


