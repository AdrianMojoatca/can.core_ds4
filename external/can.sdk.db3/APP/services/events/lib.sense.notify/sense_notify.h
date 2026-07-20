/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_notify.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __SENSE_NOTIFY_H__
#define __SENSE_NOTIFY_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"
#include "timeout.h"
#include "timeout_f.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define sense_dbg_print(x,y,z) sense_notify_dbg_print(x ,  y , z)

#define FAKE_RPM_300 300

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
void     sense_notify_init              (void)       ;
                                             
void     sense_notify_brake_on          (void)       ;
void     sense_notify_brake_off         (void)       ;
                                        
void     sense_notify_door_update       (void)       ;
Boolean  sense_notify_set_front_driver  ( Boolean state );
Boolean  sense_notify_set_front_pass	( Boolean state );
Boolean  sense_notify_set_rear_driver	( Boolean state );
Boolean  sense_notify_set_rear_pass		( Boolean state );
void  	 sense_notify_set_trunk		    ( Boolean state );
void  	 sense_notify_set_hood			( Boolean state );
void     sense_notify_door_trunk_update (void)       ;
                                           
void     sense_notify_doorlock          (void)       ;
void     sense_notify_doorunlock        (void)       ;
                                        
void     sense_notify_hbrake_on         (void)       ;
void     sense_notify_hbrake_off        (void)       ;
                                        
void     sense_notify_hood_open         (void)       ;
void     sense_notify_hood_close        (void)       ;
                                        
void     sense_notify_ign_on            (void)       ;
void     sense_notify_ign_off           (void)       ; 

void     sense_notify_keyin             (void)       ;
void     sense_notify_keyout            (void)       ; 

void     sense_notify_rke_idle          (void)       ;                                        
void     sense_notify_rke_lock          (void)       ;
void     sense_notify_rke_unlock        (void)       ;
void     sense_notify_rke_trunk         (void)       ;
void     sense_notify_rke_aux1          (void)       ;
void     sense_notify_rke_aux2          (void)       ;
void     sense_notify_rke_aux3          (void)       ;
void     sense_notify_rke_aux4          (void)       ;
void     sense_notify_rke_panic         (void)       ;
void     sense_notify_rke_start         (void)       ;
void     sense_notify_rke_stop          (void)       ;
                                              
void     sense_notify_trunk_open        (void)       ;
void     sense_notify_trunk_close       (void)       ;
                                              
void     sense_notify_skd_not_detect    (void)       ;
void     sense_notify_skd_detect        (void)       ;

void     sense_notify_disarm            (void)       ;
void     sense_notify_arm               (void)       ;
void     sense_notify_arming            (void)       ;
void     sense_notify_trigger           (void)       ;
                                        
void     sense_notify_rpm               (UInt16 value) ;
                                        
void     sense_notify_speed             (UInt8 value)  ;

void     sense_notify_parking_on         (void)       ;
void     sense_notify_parking_off        (void)       ;

void     sense_notify_lo_beam_on         (void)       ;
void     sense_notify_lo_beam_off        (void)       ;

void     sense_notify_hi_beam_on         (void)       ;
void     sense_notify_hi_beam_off        (void)       ;

void     sense_notify_hazard_on          (void)       ;
void     sense_notify_hazard_off         (void)       ;

void 	sense_notify_door_skip			 (Boolean do_skip);
void 	sense_notify_brake_skip			 (Boolean do_skip);
void 	sense_notify_ignition_skip		 (Boolean do_skip);

void sense_notify_dbg_init               (void)       ;
void sense_notify_dbg_print(StrConst * text ,  const UInt8 data[] , UInt8 size) ;

void sense_notify_rke_handle_lock		(void) 		 ;
void sense_notify_rke_timeout  			(void * arg) ;

// m2m extensions
void sense_notify_oil_alarm(Boolean on);
void sense_notify_tire_alarm(Boolean on);
void sense_notify_seat_belts(UInt8 bitmap);
void sense_notify_seat_weights(UInt8 bitmap);
void sense_notify_ev_battery_lvl(UInt8 lvl);
void sense_notify_ev_range(UInt16 rangekm);
void sense_notify_oil_life(UInt8 remaining);
void sense_notify_fuel_lvl(UInt8 lvl);
void sense_notify_fuel_cap(UInt8 litres);
void sense_notify_cabin_temp(SInt8 degC);
void sense_notify_tire_pressure(int tirenumber,UInt8 psi);
void sense_notify_odometer(UInt32 km);
void sense_notify_battery_voltage(UInt16 millivolts);


/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Timeout timeout_unlock ;

#endif
