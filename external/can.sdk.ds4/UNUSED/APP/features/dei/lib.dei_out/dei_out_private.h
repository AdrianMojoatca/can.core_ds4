/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_out_private.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __DEI_OUT_PRIVATE_H
#define __DEI_OUT_PRIVATE_H

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "cmd.h"
#include "prg_out.h"
#include "status_system.h"
#include "per_exec.h"
#include "out_q.h"
#include "dei_feature.h"
#include "wdog_v.h"
#include "config_gpio.h"

#include "trace.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//

#define DEI_OUT_LOCK_RESET()										PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)prg_out_lock , TRUE ) )
#define DEI_OUT_LOCK(offset,on,off,pulses)							PER_EXEC_ATOMIC( out_q_insert( out_q_main , (Out_Func)prg_out_lock , offset,on,off,pulses))

#define DEI_OUT_UNLOCK_RESET()										PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)prg_out_unlock , TRUE ) )
#define DEI_OUT_UNLOCK(offset,on,off,pulses)						PER_EXEC_ATOMIC( out_q_insert( out_q_main , (Out_Func)prg_out_unlock , offset,on,off,pulses))

#define DEI_OUT_SECOND_UNLOCK_RESET()								PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)prg_out_second_unlock , TRUE ) )
#define DEI_OUT_SECOND_UNLOCK(offset,on,off,pulses)					PER_EXEC_ATOMIC( out_q_insert( out_q_main , (Out_Func)prg_out_second_unlock , offset,on,off,pulses))

#define DEI_OUT_FACTORY_ALARM_ARM_RESET()   						PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)prg_out_factory_alarm_arm , TRUE ) )  
#define DEI_OUT_FACTORY_ALARM_ARM(offset,on,off,pulses)				PER_EXEC_ATOMIC( out_q_insert( out_q_main , (Out_Func)prg_out_factory_alarm_arm , offset,on,off,pulses))

#define DEI_OUT_FACTORY_ALARM_DISARM_RESET()   						PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)prg_out_factory_alarm_disarm , TRUE ) )  
#define DEI_OUT_FACTORY_ALARM_DISARM(offset,on,off,pulses)			PER_EXEC_ATOMIC( out_q_insert( out_q_main , (Out_Func)prg_out_factory_alarm_disarm , offset,on,off,pulses))

#define DEI_OUT_TRUNK_RELEASE_RESET()                  				PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)prg_out_trunk_release , TRUE ) )
#define DEI_OUT_TRUNK_RELEASE(offset,on,off,pulses)					PER_EXEC_ATOMIC( out_q_insert( out_q_main , (Out_Func)prg_out_trunk_release , offset,on,off,pulses))

#define DEI_OUT_AUX1_RESET(x)                  						PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)dei_out_aux1_control , x ) )
#define DEI_OUT_AUX1(offset,on,off,pulses)							PER_EXEC_ATOMIC( out_q_insert( out_q_main , (Out_Func)dei_out_aux1_control , offset,on,off,pulses))

#define DEI_OUT_AUX2_RESET(x)                  						PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)dei_out_aux2_control , x ) )
#define DEI_OUT_AUX2(offset,on,off,pulses)							PER_EXEC_ATOMIC( out_q_insert( out_q_main , (Out_Func)dei_out_aux2_control , offset,on,off,pulses))

#define DEI_OUT_AUX3_RESET(x)                  						PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)dei_out_aux3_control , x ) )
#define DEI_OUT_AUX3(offset,on,off,pulses)							PER_EXEC_ATOMIC( out_q_insert( out_q_main , (Out_Func)dei_out_aux3_control , offset,on,off,pulses))

#define DEI_OUT_AUX4_RESET(x)                  						PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)dei_out_aux4_control , x ) )
#define DEI_OUT_AUX4(offset,on,off,pulses)							PER_EXEC_ATOMIC( out_q_insert( out_q_main , (Out_Func)dei_out_aux4_control , offset,on,off,pulses))

#define DEI_OUT_GROUND_WHEN_RUNNING_5SEC_RESET()					PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)prg_out_ground_when_running_5sec , TRUE ) )
#define DEI_OUT_GROUND_WHEN_RUNNING_5SEC(offset,on,off,pulses)		PER_EXEC_ATOMIC( out_q_insert( out_q_main , (Out_Func)prg_out_ground_when_running_5sec , offset,on,off,pulses))

#define DEI_OUT_HORN_RESET()                  						PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)prg_out_horn , TRUE ) )
#define DEI_OUT_HORN(offset,on,off,pulses)							PER_EXEC_ATOMIC( out_q_insert( out_q_main , (Out_Func)prg_out_horn , offset,on,off,pulses))

#define DEI_OUT_PULSE_AFTER_IGN_OFF_RESET()    						PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)prg_out_pulse_after_ign_off , TRUE ) )
#define DEI_OUT_PULSE_AFTER_IGN_OFF(offset,on,off,pulses)			PER_EXEC_ATOMIC( out_q_insert( out_q_main , (Out_Func)prg_out_pulse_after_ign_off , offset,on,off,pulses))

#define DEI_OUT_PULSE_AFTER_START_RESET()    						PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)prg_out_pulse_after_start , TRUE ) )
#define DEI_OUT_PULSE_AFTER_START(offset,on,off,pulses)				PER_EXEC_ATOMIC( out_q_insert( out_q_main , (Out_Func)prg_out_pulse_after_start , offset,on,off,pulses))

#define DEI_OUT_PULSE_BEFORE_START_RESET()    						PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)prg_out_pulse_before_start , TRUE ) )
#define DEI_OUT_PULSE_BEFORE_START(offset,on,off,pulses)			PER_EXEC_ATOMIC( out_q_insert( out_q_main , (Out_Func)prg_out_pulse_before_start , offset,on,off,pulses))

#define DEI_OUT_PULSE_WHILE_STARTING_RESET()    					PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)prg_out_pulse_while_starting , TRUE ) )
#define DEI_OUT_PULSE_WHILE_STARTING_START(offset,on,off,pulses)	PER_EXEC_ATOMIC( out_q_insert( out_q_main , (Out_Func)prg_out_pulse_while_starting , offset,on,off,pulses))

#define DEI_OUT_SMARTKEY_CONTROL_RESET()    						PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)prg_out_smartkey_control , TRUE ) )
#define DEI_OUT_SMARTKEY_CONTROL(offset,on,off,pulses)				PER_EXEC_ATOMIC( out_q_insert( out_q_main , (Out_Func)prg_out_smartkey_control , offset,on,off,pulses))

#define DEI_OUT_REAR_DEFOGGER_RESET()    							PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)prg_out_rear_defogger , TRUE ) )
#define DEI_OUT_REAR_DEFOGGER(offset,on,off,pulses)					PER_EXEC_ATOMIC( out_q_insert( out_q_main , (Out_Func)prg_out_rear_defogger , offset,on,off,pulses))

//--------------------------------------------------------------------------//

#define DEI_OUT_PAYLOAD(x)		(UInt32)( *cmd_get_pld(x) )

//--------------------------------------------------------------------------//

#define FEATURE(x)				dei_feature_get(x)

//--------------------------------------------------------------------------//					

#ifndef DEBUG_DEI_OUT
#define DEBUG_DEI_OUT			0
#endif

#if DEBUG_DEI_OUT == 1
#define TRACE_DEI_OUT			TRACE
#else
#define TRACE_DEI_OUT(...)		;
#endif

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void dei_out_acc_on						 	( void *payload );
void dei_out_acc_off					 	( void *payload );

void dei_out_aux1_init						( void );
void dei_out_aux1_control				 	( Boolean state );
void dei_out_aux2_init						( void );
void dei_out_aux2_control				 	( Boolean state );
void dei_out_aux3_init						( void );
void dei_out_aux3_control				 	( Boolean state );
void dei_out_aux4_init						( void );
void dei_out_aux4_control				 	( Boolean state );

void dei_out_domelight_on 				 	( void *payload );
void dei_out_domelight_off				 	( void *payload );
void dei_out_factory_alarm_arm	 		 	( void *payload );
void dei_out_factory_alarm_disarm		 	( void *payload );
void dei_out_ground_when_arm	     	 	( void *payload );
void dei_out_ground_when_locked     	 	( void *payload );
void dei_out_ground_when_running_5sec	 	( void *payload );
void dei_out_ground_when_running_status_on	( void *payload );
void dei_out_ground_when_running_status_off ( void *payload );
void dei_out_ground_when_triggered_on       ( void *payload );
void dei_out_ground_when_triggered_off      ( void *payload );
void dei_out_horn                           ( void *payload );
void dei_out_ign_on                         ( void *payload );
void dei_out_ign_off                        ( void *payload );
void dei_out_lock                           ( void *payload );
void dei_out_parking_light_on               ( void *payload );
void dei_out_parking_light_off              ( void *payload );
void dei_out_pulse_after_ign_off            ( void *payload );
void dei_out_pulse_after_ign_off_reset      ( void *payload );
void dei_out_pulse_after_start              ( void *payload );
void dei_out_pulse_before_start             ( void *payload );
void dei_out_smartkey_control               ( void *payload );
void dei_out_rear_defogger_on               ( void *payload );
void dei_out_rear_defogger_off              ( void *payload );
void dei_out_second_unlock                  ( void *payload );
void dei_out_start_on                       ( void *payload );
void dei_out_start_off                      ( void *payload );

void dei_out_trunk_release_on               ( void *payload );
void dei_out_trunk_release_off              ( void *payload );

void dei_out_unlock                         ( void *payload );
void dei_out_unlock_user2                   ( void *payload );
void dei_out_unlock_driver                  ( void *payload );
void dei_out_unlock_all                     ( void *payload );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern const UInt16 dei_out_lock_pulse_length_table[] ;
extern const UInt8  dei_out_horn_pulse_length_table[] ;
extern       vWatch dei_out_start_on_wdog ; 
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


