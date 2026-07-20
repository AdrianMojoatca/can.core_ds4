/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: tsk_rs_private.h 28789 2014-12-08 08:14:26Z daniel.erhan $
/*==========================================================================*/
#ifndef __TSK_RS_PRIVATE_H__
#define __TSK_RS_PRIVATE_H__
/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "status_system.h"
#include "cmd.h"
#include "timeout_q.h"
#include "trace.h"
#include "tsk_rs.h"
#include "wake.h"
#include "reg.h"
#include "sense_rs_can.h"
#include "led.h"
#include "config_dbg.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#include "config_dbg.h"
#ifndef DBG_TRS
#define DBG_TRS		0
#endif

#if DBG_TRS == 1
#define TRACE_TRS		TRACE
#else
#define TRACE_TRS(...)   ;
#endif

#if DBG_CAN_TSK_RS == 1
    #include "trace_can2.h"
	#define TRACE_CAN_TSK_RS    TRACE_CAN2
#else
	#if DBG_TRS == 1
		#define TRACE_CAN_TSK_RS    TRACE
	#else
		#define TRACE_CAN_TSK_RS(...)   ;
	#endif
#endif

/*--------------------------------------------------------------------------*/

#define TSK_RS_PIP_SIZE                  20
#define TSK_RS_PIP_INSERT_TIMEOUT        300
#define TSK_RS_Q_INSERT_TIMEOUT          300
#define TSK_RS_TKE_BRAKE_SET_TIMEOUT     50  //500ms
#define TSK_RS_TKE_BRAKE_RESET_TIMEOUT	 250 //500ms + 500ms

/*--------------------------------------------------------------------------*/

#define TSK_RS_PRI                      1000
#define TSK_RS_STK                      0x150

/*--------------------------------------------------------------------------*/
#ifndef TSK_RS_TAKEOVER_TIMEOUT
#define TSK_RS_TAKEOVER_TIMEOUT         45000
#else
#warning "The takeover timeout is defined externally"
#endif
/*--------------------------------------------------------------------------*/

#define TSK_RS_TAKEOVER_MANAGER_DISABLED()          (!!(tsk_rs_takeover_cfg  & TSK_RS_SKIP_DOOR_SHUTDOWN  ))
#define TSK_RS_TAKEOVER_MANAGER_ENABLED()           ( !(tsk_rs_takeover_cfg  & TSK_RS_SKIP_DOOR_SHUTDOWN  ))
#define TSK_RS_TAKEOVER_ALLOWED_BY_OEM()            ((!!(tsk_rs_takeover_cfg & TSK_RS_TAKEOVER_OEM        ))&&(!(tsk_rs_takeover_cfg & TSK_RS_SKIP_DOOR_SHUTDOWN)))
#define TSK_RS_TAKEOVER_ALLOWED_BY_AFTERMARKET()    ((!!(tsk_rs_takeover_cfg & TSK_RS_TAKEOVER_AFTERMARKET))&&(!(tsk_rs_takeover_cfg & TSK_RS_SKIP_DOOR_SHUTDOWN)))

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void                tsk_rs_cmd_init                 ( void );

void                tsk_rs_cmd_gwr_on               ( void );
void                tsk_rs_cmd_gwr_off              ( void );
void                tsk_rs_cmd_acc_on               ( void );
void                tsk_rs_cmd_acc_off              ( void );
void                tsk_rs_cmd_ign_on               ( void );
void                tsk_rs_cmd_ign_off              ( void );
void                tsk_rs_cmd_str_on               ( void );
void                tsk_rs_cmd_str_off              ( void );

void                tsk_rs_tsk                      ( void );

void                tsk_rs_tke_init                 ( void );
void                tsk_rs_tke_allowed              ( void );
void                tsk_rs_tke_not_allowed          ( void );
void                tsk_rs_tke_allowed_aftermarket  ( void );
void                tsk_rs_tke_allowed_oem          ( void );
Timeout_Q_Status    tsk_rs_tke_timeout              ( void );
void                tsk_rs_tke_door_shutdown        ( void );
Timeout_Q_Status    tsk_rs_tke_brake_reset          ( void );
Timeout_Q_Status    tsk_rs_tke_brake_set            ( void );
       
void                tsk_rs_brake_on                 ( void );
void                tsk_rs_brake_off                ( void );

void                tsk_rs_invalid_state_reset      ( void );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Pip              tsk_rs_pip                        ;
extern Boolean          tsk_rs_idle                       ; 
extern Boolean          tsk_rs_shutdown_req               ;
extern Boolean          tsk_rs_tke_shutdown_req           ;
extern Reg_Table        tsk_rs_reg                        ;
extern Tsk_Rs_Takeover  tsk_rs_takeover_cfg               ;
extern UInt32           tsk_rs_takeover_timeout           ;
extern Boolean          tsk_rs_gwr ; 
extern Boolean          tsk_rs_ign ;
extern Boolean          tsk_rs_str ;
extern Boolean          tsk_rs_do_oem_disarm;
extern Boolean          tsk_rs_lock_status;
extern Tsk_Rs_Pts       tsk_rs_pts_callback;
extern Boolean          tsk_rs_invalid_state_var;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

