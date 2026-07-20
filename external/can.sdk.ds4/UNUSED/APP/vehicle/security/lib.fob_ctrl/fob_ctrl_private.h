/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: fob_ctrl_private.h 33268 2015-08-19 15:40:18Z suchita.yadav $
/*==========================================================================*/

#ifndef __FOB_CTRL_PRIVATE_H__
#define __FOB_CTRL_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "cmd.h"
#include "status_system.h"
#include "timeout_f.h"
#include "reg.h"
#include "fob_ctrl.h"
#include "reset.h"
#include "nvfs_usr.h"

#ifdef DEV_LPC_236x
	#include "nvram_usr.h"
#endif

#include "trace.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/


#ifndef DBG_FOB_CTRL
#define DBG_FOB_CTRL			0
#endif

#if DBG_FOB_CTRL == 1
#define TRACE_DEI_FOB			TRACE
#else
#define TRACE_DEI_FOB(...)		;
#endif

/*--------------------------------------------------------------------------*/

#define FOB_CTRL_TIMEOUT        			5	 	//<--- 5 seconds default value
#define FOB_CTRL_TIMEOUT_BETWEEN_PRESSES	50      //<--- 500ms default value
#define FOB_CTRL_1SEC_BASE_TIME		        1000
#define FOB_CTRL_RESET_TIMEOUT              300     //<---300ms  

/*--------------------------------------------------------------------------*/

typedef struct s_Fob_Ctrl_Cfg
{
	Boolean is_enable    ; 
	UInt8   timeout_sec  ;
	UInt8   locks        ;
	UInt8   timeout_10ms ;
	UInt8   dummy[4]     ;

}Fob_Ctrl_Cfg ;

/*--------------------------------------------------------------------------*/

typedef struct s_Fob_Ctrl_Vars
{
	Fob_Ctrl_Cfg cfg        ;
	
	UInt8 		lock_count  ;
	Reg_Table 	reg         ;
	Boolean     tbp_expired ;
	Timeout     timeout     ;
	Timeout     timeout_tbp ; 
	
	Boolean     in_progress ;

}Fob_Ctrl_Vars;

/*--------------------------------------------------------------------------*/


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Fob_Ctrl_Vars	fob_ctrl_vars    ;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

void 	fob_ctrl_lock         ( void );
void 	fob_ctrl_reset        ( void );
void 	fob_ctrl_dei_init	  ( void );
Boolean fob_ctrl_nvfs_init    ( void );
Boolean fob_ctrl_nvfs_reset   ( void );
void    fob_ctrl_tbp_start    ( void );
void    fob_ctrl_tbp_stop     ( void );
void    fob_ctrl_tbp_expired  ( void );

#endif

