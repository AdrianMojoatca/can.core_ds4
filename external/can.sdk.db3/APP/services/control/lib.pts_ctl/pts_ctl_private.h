/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: pts_ctl_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __PTS_CTL_PRIVATE_H__
#define __PTS_CTL_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"
#include "dbg.h" 
#include "timeout.h"

#if DBG_PTS_CTL==1 
  #warning DBG_PTS_CTL
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//
typedef struct
{
  Boolean is_disabled    ;
  Boolean gwr            ;
  
  Timeout timeout_pts        ;
  Timeout timeout_pts_pulse  ;
  Timeout timeout_obd_status ;
  Timeout timeout_obd_req    ;
  
  Pts_Ctl_Func func_on  ;
  Pts_Ctl_Func func_off ;  
  
}Pts_Ctl_Vars ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void pts_ctl_dbg_init           (void) ;
void pts_ctl_on                 (void * arg) ;
void pts_ctl_off                (void * arg) ;
void pts_ctl_pts_timeout        (void * arg) ;  
void pts_ctl_obd_status_timeout (void * arg) ;
void pts_ctl_request            (void * arg) ;
void pts_ctl_request_stop       (void * arg) ;
void pts_ctl_gwr_on             (void * arg) ;
void pts_ctl_gwr_off            (void * arg) ;
void pts_ctl_speed_set 	        (void * arg) ;
void pts_ctl_speed_clr          (void * arg) ;

void pts_ctl_dbg_print 	(StrConst * text ,  const UInt8 data[] , UInt8 size) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Pts_Ctl_Vars pts_ctl_vars ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

#endif




