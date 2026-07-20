/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: prg_private.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __PRG_PRIVATE_H__
#define __PRG_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "prg.h"
#include "status_system.h"
#include "wake.h"
#include "nvfs_usr.h"
#include "system_type.h"
#include "config_gpio.h"

#include "trace.h"
#include "dbg.h"

#ifdef PLATFORM_933
  #include "dei_system.h"    //dei_system_prg_mode_set/get()
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*-------------------------------------------------------------------------*/

typedef enum
{
	PRG_EVT_PUSH_EXT_PRESS		= 1u << 0 ,

	PRG_EVT_PUSH_EXT_RELEASE    = 1u << 1 ,

	PRG_EVT_PUSH_RELEASE        = 1u << 2 ,

	PRG_EVT_IGN_ON              = 1u << 3 ,

	PRG_EVT_IGN_OFF             = 1u << 4 ,

	PRG_EVT_DOOR_CLOSED         = 1u << 5 ,

	PRG_EVT_LOCK                = 1u << 6 ,

	PRG_EVT_REMOTE_PROGRAMMED   = 1u << 7 
#ifdef PLATFORM_933
                                        ,
  PRG_EVT_BLE_PRG_ENTER       = 1u << 8 , 
  PRG_EVT_BLE_PRG_EXIT        = 1u << 9 
#endif  

}Prg_Evt ; 

/*-------------------------------------------------------------------------*/


#ifndef DEBUG_PRG
#define DEBUG_PRG			0
#endif

#if DEBUG_PRG == 1
#define TRACE_DEI_PRG			TRACE
#else
#define TRACE_DEI_PRG(...)		;
#endif

/*-------------------------------------------------------------------------*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
Boolean prg_menu_remote_programming ( void );
Boolean prg_menu_remote_delete      ( void );
Boolean prg_menu_tacho_delete       ( void );
Boolean prg_menu_zap				( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Evt 			prg_evt  ; 
extern UInt8		prg_menu ;
extern Reg_Table	prg_reg  ;
extern Boolean      prg_exit ;
extern Boolean      prg_use_door ;

#ifdef PLATFORM_933
extern UInt8 ble_prg_menu;
#endif
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


