/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: ctl_dl_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef _CTL_DL_PRIVATE_H_
#define _CTL_DL_PRIVATE_H_

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "ctl_dl.h"
#include "timeout_f.h"
#include "cmd.h"
#include "prog.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define CTL_DL_GWR_DELAY		4000
#define LOCK_DELAY				3000
#define UNLOCK_DELAY			500
#define BRAKE_PRESS_DELAY		100
#define SPEED_SET_DELAY			100

#define GWR_ON					CTL_DL_COND_ON
#define GWR_OFF					CTL_DL_COND_OFF
#define GWR_DONT_CARE			CTL_DL_COND_DONT_CARE

#define IGN_ON					CTL_DL_COND_ON
#define IGN_OFF					CTL_DL_COND_OFF
#define IGN_DONT_CARE			CTL_DL_COND_DONT_CARE

typedef enum
{
	CTL_DL_COND_DONT_CARE,
	CTL_DL_COND_ON,
	CTL_DL_COND_OFF,

} Ctl_Dl_Condition ;

typedef enum
{
	CTL_DL_ACTION_LOCK     ,
	CTL_DL_ACTION_UNLOCK   ,

} Ctl_Dl_Action ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Ctl_Dl_Condition ctl_dl_flag_gwr      ;
extern Ctl_Dl_Condition ctl_dl_cond_gwr      ;
extern Ctl_Dl_Condition ctl_dl_flag_ign      ;
extern Ctl_Dl_Condition ctl_dl_cond_ign      ;

extern Boolean          ctl_dl_lock_done     ;

extern Ctl_Dl_Action    ctl_dl_action        ;

extern Timeout          ctl_dl_defer_timeout ;
extern Timeout 		    ctl_dl_gwr           ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void ctl_dl_gwr_on            (void * arg);
void ctl_dl_gwr_off           (void * arg);
void ctl_dl_ign_on            (void * arg);
void ctl_dl_ign_off           (void * arg);
void ctl_dl_brake_press       (void * arg);
void ctl_dl_speed_set         (void * arg);
void ctl_dl_defer_timeout_fnc (void * arg);
void ctl_dl_defer_action      (Ctl_Dl_Action action, Ctl_Dl_Condition cond_ign, Ctl_Dl_Condition  cond_gwr, UInt16 delay);
void ctl_dl_gwr_timeout		  (void * arg);

//--------------------------------------------------------------------------//

#endif


