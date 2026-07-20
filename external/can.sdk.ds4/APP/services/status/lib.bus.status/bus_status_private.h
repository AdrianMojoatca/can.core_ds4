/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: bus_status_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/
//
/*==========================================================================*/

#ifndef __BUS_STATUS_PRIVATE_H__
#define __BUS_STATUS_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "bus_status.h"
#include "cmd.h"
#include "d2d_code.h"
#include "d2d.h"
#include "timeout.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define D2D_TIMEOUT		100

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void bus_status_notify_brake_on							(void *arg);
void bus_status_notify_brake_off 						(void *arg);
void bus_status_notify_prk_neutral_active				(void *arg);
void bus_status_notify_prk_neutral_not_active			(void *arg);
void bus_status_notify_handbrake_on						(void *arg);
void bus_status_notify_handbrake_off					(void *arg);
void bus_status_notify_hood_open						(void *arg);
void bus_status_notify_hood_close						(void *arg);
void bus_status_notify_trunk_open						(void *arg);
void bus_status_notify_trunk_close						(void *arg);
void bus_status_notify_other_door_open					(void *arg);
void bus_status_notify_other_door_close					(void *arg);
void bus_status_notify_driver_door_open					(void *arg);
void bus_status_notify_driver_door_close				(void *arg);
void bus_status_notify_glowplug_on						(void *arg);
void bus_status_notify_glowplug_off						(void *arg);
void bus_status_notify_key_in							(void *arg);
void bus_status_notify_key_out							(void *arg);

void bus_status_notify_input_keyless_lock				(void *arg);
void bus_status_notify_input_keyless_unlock				(void *arg);
void bus_status_notify_input_keyless_arm				(void *arg);
void bus_status_notify_input_keyless_disarm				(void *arg);
void bus_status_notify_input_keyless_trunk				(void *arg);
void bus_status_notify_input_keyless_panic				(void *arg);
void bus_status_notify_input_keyless_aux1				(void *arg);
void bus_status_notify_input_keyless_aux2				(void *arg);
void bus_status_notify_input_keyless_aux3				(void *arg);
void bus_status_notify_input_keyless_aux4				(void *arg);
void bus_status_notify_input_keyless_start				(void *arg);
void bus_status_notify_input_keyless_stop				(void *arg);
void bus_status_notify_eips_alert						(void *arg);
void bus_status_notify_eips_shutdown 					(void *arg);
void bus_status_notify_eips_shutdown_fail				(void *arg);

void bus_status_notify_input_lock 						(void *arg);
void bus_status_notify_input_unlock 					(void *arg);
void bus_status_notify_input_gwr_off					(void *arg);
void bus_status_notify_input_gwr_on						(void *arg);
void bus_status_notify_input_ignition_off				(void *arg);
void bus_status_notify_input_ignition_on				(void *arg);

void bus_status_notify_brake_d2d_delay				    (void *arg);

void bus_status_notify_headlight_on						(void *arg);
void bus_status_notify_headlight_off					(void *arg);

void bus_status_tamper(SInt8 do_tamper); // set this state (tampered)
SInt8 bus_status_tampered(void); // read current state
void bus_status_arm(SInt8 do_arm); // set this state (armed OR locked), can reset bus_status_tampered()
SInt8 bus_status_armed(void); // read current state
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Boolean bus_status_gwr ;
extern volatile Boolean bus_status_tx_delay ;

extern Timeout d2d_delay_timeout     ;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif
