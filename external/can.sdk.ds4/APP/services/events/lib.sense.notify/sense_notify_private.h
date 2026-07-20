/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_notify_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __SENSE_NOTIFY_PRIVATE_H__
#define __SENSE_NOTIFY_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "sense_notify.h"
#include "status.h"
#include "dbg.h"
#include "d2d.h"
#include "cmd.h"
#include "rf.h"


#if DBG_SENSE == 1
  #warning "DBG_SENSE Is Enable"
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define RF_IS_DIS !!(prog_retrieve_option (FEAT_RF_OUTPUT) == NO_RF)

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void sense_notify_gwr_on        (void * arg);
void sense_notify_gwr_off       (void * arg);
void sense_notify_fake_rpm_on   (void * arg);
void sense_notify_fake_rpm_off  (void * arg);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Boolean sense_notify_lock_set            ;
extern Boolean sense_notify_lock_active         ;
extern Boolean sense_notify_door_skip_flag      ;
extern Boolean sense_notify_brake_skip_flag     ;
extern Boolean sense_notify_ignition_skip_flag  ;
extern Boolean sense_notify_gwr_flag            ;

#endif
