/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hazard_private.h 19197 2013-02-12 10:50:11Z florin.olariu $
/*==========================================================================*/

#ifndef __HAZARD_PRIVATE_H__
#define __HAZARD_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "hazard.h"
#include "cmd.h"
#include "trace.h"
#include "wake.h"
#include "timeout_q.h"
#include "lf.h"
#include "status_system.h"

#include "config_dbg.h"
#ifndef DBG_HAZARD
#define DBG_HAZARD		0
#endif

#if DBG_HAZARD == 1
#define TRACE_HAZARD		TRACE
#else
#define TRACE_HAZARD(...)   ;
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*------------------------------------------------------------------------*/
// <h>  [08]. Task 'hazard_tsk' settings
// <o0> 1. TSK PRIORITY    <0-65535>
// <i>  Lowest  priority = 0
// <i>  Highest priority = 65535
// <o1> 2. TSK STACK SIZE (bytes) <0x0000-0xFFFF:4>
// <i>  Must be a multiple of 4.
// </h>
#define HAZARD_TSK_PRI            100
#define HAZARD_TSK_STK            0x0120
#define HAZARD_TSK_NAME           "hazard_tsk"

enum
{
    HAZARD_EVT_STATUS_ON         = 1 << 0 , 
    HAZARD_EVT_STATUS_OFF        = 1 << 1 ,
    HAZARD_EVT_REQ_ON            = 1 << 2 ,
    HAZARD_EVT_REQ_OFF           = 1 << 3 ,    
};

#define HAZARD_LIGHT_DELAYED            50      //500ms

#define HAZARD_SKIP_LOCK_TIMEOUT        200     //2000ms
#define HAZARD_SKIP_RS_LOCK_TIMEOUT     200     //2000ms

#define HAZARD_SKIP_UNLOCK_TIMEOUT      600     //6000ms
#define HAZARD_SKIP_RS_UNLOCK_TIMEOUT   600     //6000ms

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void hazard_request_on       ( void );
void hazard_request_off      ( void );
void hazard_tsk              ( void );
void hazard_disable_lock     ( void );
void hazard_disable_unlock   ( void );
void hazard_temporary_disable( UInt32 timeout );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Evt                hazard_evt         ; 
extern Hazard_Pulse_Func  hazard_pulse_func ;
extern Boolean            hazard_enable      ; 
extern Hazard_Cfg         hazard_cfg         ;
extern Boolean            hazard_was_instantiated;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


