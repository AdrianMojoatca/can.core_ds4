/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_prk.h 1106 2010-04-27 13:30:39Z florin.olariu $
/*==========================================================================*/

#ifndef __SENSE_RS_CAN_PRIVATE_H__
#define __SENSE_RS_CAN_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs_can.h"

#include "cmd.h"
#include "trace.h"
#include "ied.h"
#include "idb.h"
#include "timeout_f.h"
#include "status_system.h"
#include "config_dbg.h"
#include "status.h"
#include "config_gpio.h"
#include "prog.h"
#include "rf.h"
#include "sense_notify.h"

#ifndef PLATFORM_CM800
    
#include "tach.h"

#endif   

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//

#if !defined(PLATFORM_CM800) && !defined(PLATFORM_933)
#define cmd_q_rs_insert				cmd_q_insert
#endif

//--------------------------------------------------------------------------//



#define SENSE_RS_CAN_TIMEOUT_CMD_Q          100

//--------------------------------------------------------------------------//

#ifndef DEBUG_SENSE_RS_CAN
#define DEBUG_SENSE_RS_CAN			1
#endif

#if DEBUG_SENSE_RS_CAN == 1
#define TRACE_SENSE_RS_CAN			TRACE
#else
#define TRACE_SENSE_RS_CAN(...)		;
#endif

//--------------------------------------------------------------------------//

#define IS_NOT_RSR_SETUP()  (prog_retrieve_option (FEAT_RF_OUTPUT) == NO_RF)

//--------------------------------------------------------------------------//



/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void          sense_rs_can_other_doors_init  ( void );
void          sense_rs_can_driver_door_init  ( void );
void          sense_rs_can_all_doors_init    ( void );
void          sense_rs_can_ign_init          ( void );
void          sense_rs_can_key_in_init       ( void );
void          sense_rs_can_hood_init         ( void );
void          sense_rs_can_trunk_init        ( void );
void          sense_rs_can_handbrake_init    ( void );
void          sense_rs_can_rpm_init          ( void );
void          sense_rs_can_speed_init        ( void );
void 		  sense_rs_can_skd_init			 ( void );
void          sense_rs_can_brake_init        ( void );
void          sense_rs_can_glowplug_init     ( void );
void          sense_rs_can_oem_disable_init  ( void );
void          sense_rs_can_oem_lock_init     ( void );
void          sense_rs_can_oem_unlock_init   ( void );
void          sense_rs_can_oem_trunk_init    ( void );
void          sense_rs_can_oem_autolock_init ( void );
void          sense_rs_can_neutral_init      ( void );

void          sense_rs_brake_reset           ( void );
void          sense_rs_handbrake_reset       ( void );

void 		  sense_rs_can_door_open_data    ( void );
void 		  sense_rs_can_door_close_data   ( void );

void          sense_rs_can_oem_alarm_disarm  ( void );
void          sense_rs_can_oem_alarm_arm     ( void );
void          sense_rs_can_oem_alarm_arming  ( void );
void          sense_rs_can_oem_alarm_trigger ( void );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern       Boolean           sense_rs_can_oem_active  ;
extern       Boolean           sense_rs_all_door_status ;
extern       Boolean           sense_rs_can_noise_window;
extern       Boolean           sense_rs_can_brake_skip;

extern Sense_Rs_Can_Cfg* sense_rs_can_cfg         ;
extern Sense_Rs_Can_Cfg  sense_rs_can_default_cfg ; 

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


