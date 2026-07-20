/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_private.h 4134 2012-09-17 08:48:12Z florin.olariu $
/*==========================================================================*/
#ifndef __SENSE_PRIVATE_H__
#define __SENSE_PRIVATE_H__
/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//

#define DATA 			can_msg->data._8_
#define DATA1(z)          can_msg->data._8_[z] 
#define CAST_BOOL(x)   	!!(x)

//--------------------------------------------------------------------------//

#define SENSE_WINDOW_VENT                  0x20

//--------------------------------------------------------------------------//

#if DBG_SENSE == 1
#define     TRACE_SENSE      TRACE
#else
#define     TRACE_SENSE(...)   ;
#endif

//--------------------------------------------------------------------------//


#define IDLE_STATE_GEARBOX  0x00
#define MANUAL_GEARBOX		0x01
#define AUTOMATIC_GEARBOX	0x02

//--------------------------------------------------------------------------//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void sense_doors_3B3               ( Can_Msg *can_msg );
void sense_hood_3B3                ( Can_Msg *can_msg );
void sense_trunk_3B3               ( Can_Msg *can_msg );
void sense_ignition_3B3            ( Can_Msg *can_msg );
void sense_rke_331                 ( Can_Msg *can_msg );
void sense_rke_3B3                 ( Can_Msg *can_msg );
void sense_left_front_window_xxx   ( Can_Msg *can_msg );
void sense_right_front_window_xxx  ( Can_Msg *can_msg );
void sense_left_rear_window_xxx    ( Can_Msg *can_msg );
void sense_right_rear_window_xxx   ( Can_Msg *can_msg );
void sense_brake_3C3               ( Can_Msg *can_msg );

void sense_handbrake_213           ( Can_Msg *can_msg ); // ADI - 12.01.2017

void sense_handbrake_3BC           ( Can_Msg *can_msg );
void sense_rpm_204                 ( Can_Msg *can_msg );
void sense_skd_xxx                 ( Can_Msg *can_msg );
void sense_speed_xxx               ( Can_Msg *can_msg );
void sense_hazard_3B3              ( Can_Msg *can_msg );

void sense_manual_box_171          ( Can_Msg *can_msg ); // ADI - 22_02_2019

void detect_frd_sense_ign           (Can_Msg *can_msg); 

void sense_handbrake_3B3           ( Can_Msg *can_msg ) ; // Fiesta
void trunk_status_handler( void ); 	   // Fiesta

void sense_rke_3C3( Can_Msg *can_msg ); // Fiesta 

void sense_rpm_fake_rpm            ( Can_Msg *can_msg ); // vers. 1_20 - 21_05_2020

void sense_index_446			   ( Can_Msg *can_msg ); // vers. 1_21 - 23_09_2020
void sense_index_332               ( Can_Msg *can_msg ); // vers 1_22 - 25_09_2020

void sense_index_331               ( Can_Msg *can_msg ); // 1_01 - 14_09_2021

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Boolean CAR_Gearbox;

extern UInt8 GLOBAL_LOCK_446[8];
extern UInt8 GLOBAL_UNLOCK_446[8];
extern UInt8 GLOBAL_331[8];
extern UInt8 GLOBAL_332[8];

extern Boolean sense_331_present;
extern Boolean sense_332_present;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

