/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: prg_out.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __PRG_OUT_H__
#define __PRG_OUT_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "os.h"
#include "reg.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
//--------------------------------------------------------------------------//
#ifdef PLATFORM_CM800

typedef enum
{
    PRG_OUT_FACTORY_ALARM_ARM          ,
	PRG_OUT_GROUND_WHEN_ARM            ,
	PRG_OUT_FACTORY_ALARM_DISARM       ,
	PRG_OUT_LOCK                       ,
    PRG_OUT_GROUND_WHEN_LOCKED         ,    
	PRG_OUT_UNLOCK                     ,
	PRG_OUT_SECOND_UNLOCK              ,
	PRG_OUT_UNLOCK_USER2               , 
	PRG_OUT_TRUNK_RELEASE              ,
	PRG_OUT_AUX1                       ,
    PRG_OUT_AUX2                       ,
    PRG_OUT_AUX3                       ,
    PRG_OUT_AUX4                       ,	    
    PRG_OUT_GROUND_WHEN_RUNNING_STATUS , 
    PRG_OUT_GROUND_WHEN_RUNNING_5SEC   ,
    PRG_OUT_PULSE_BEFORE_START         ,
	PRG_OUT_SMARTKEY_CONTROL           ,
	PRG_OUT_PULSE_AFTER_START          ,	
	PRG_OUT_PULSE_AFTER_IGN_OFF        ,
	PRG_OUT_ACC                        ,
	PRG_OUT_IGN                        ,
    PRG_OUT_START                      ,    
	PRG_OUT_PARKING_LIGHT              ,    	
    PRG_OUT_REAR_DEFOGGER              ,
    PRG_OUT_DOMELIGHT                  ,
    PRG_OUT_HORN                       ,
    PRG_OUT_GROUND_WHEN_TRIGGERED      ,
    PRG_OUT_ANTI_GRIND                 ,
    PRG_OUT_COUNT              

}Prg_Out_Type ;         

#else
typedef enum
{
    PRG_OUT_FACTORY_ALARM_ARM          ,
	PRG_OUT_GROUND_WHEN_ARM            ,
	PRG_OUT_FACTORY_ALARM_DISARM       ,
	PRG_OUT_LOCK                       ,
    PRG_OUT_GROUND_WHEN_LOCKED         ,    
	PRG_OUT_UNLOCK                     ,
	PRG_OUT_SECOND_UNLOCK              ,
	PRG_OUT_UNLOCK_USER2               , 
	PRG_OUT_TRUNK_RELEASE              ,
	PRG_OUT_AUX1                       ,
    PRG_OUT_AUX2                       ,
    PRG_OUT_AUX3                       ,
    PRG_OUT_AUX4                       ,	    
//    PRG_OUT_GROUND_WHEN_RUNNING_STATUS , 
//    PRG_OUT_GROUND_WHEN_RUNNING_5SEC   ,
//    PRG_OUT_PULSE_BEFORE_START         ,
	PRG_OUT_SMARTKEY_CONTROL           ,
	PRG_OUT_PULSE_AFTER_START          ,	
//	PRG_OUT_PULSE_AFTER_IGN_OFF        ,
	PRG_OUT_ACC                        ,
	PRG_OUT_IGN                        ,
    PRG_OUT_START                      ,    
	PRG_OUT_PARKING_LIGHT              ,    	
//    PRG_OUT_REAR_DEFOGGER              ,
    PRG_OUT_DOMELIGHT                  ,
    PRG_OUT_HORN                       ,
//    PRG_OUT_GROUND_WHEN_TRIGGERED      ,
    PRG_OUT_ANTI_GRIND                 ,
    PRG_OUT_DEFROSTER_LATCHED          ,
    PRG_OUT_DEFROSTER_PULSED           ,
    PRG_OUT_LED_FLASH                  ,
    PRG_OUT_COMFORT_CLOSURE            ,
    PRG_OUT_GROUND_WHEN_IN_READY_MODE   ,
    PRG_OUT_STATUS                     ,
    PRG_OUT_SENSOR_TRIGGER             ,
	  PRG_OUT_PULSE_AFTER_RS_SHUTDOWN    ,
    PRG_OUT_AG_STARTER_KILL            ,
    PRG_OUT_FOB                       ,		
	PRG_OUT_IID_ENGINE_RUNNING		  ,
	PRG_OUT_SLOPEN,
	PRG_OUT_SLCLOSE,
	
	PRG_OUT_PTIM_RELAY,
	PRG_OUT_STS_TRUNK,
	PRG_OUT_STS_BRAKE,
	PRG_OUT_STS_PROX,
	
	PRG_OUT_SIREN,
	
    PRG_OUT_COUNT              

}Prg_Out_Type ;         
#endif


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
EXTERN_C_ENTER

void prg_out_init( void );

void prg_out_factory_alarm_arm         ( Boolean ctrl );
void prg_out_factory_alarm_disarm      ( Boolean ctrl );
void prg_out_lock                      ( Boolean ctrl );
void prg_out_ground_when_locked        ( Boolean ctrl );
void prg_out_second_unlock             ( Boolean ctrl );
void prg_out_unlock_user2              ( Boolean ctrl );
void prg_out_trunk_release             ( Boolean ctrl );
void prg_out_aux1                      ( Boolean ctrl );
void prg_out_aux2                      ( Boolean ctrl );
void prg_out_aux3                      ( Boolean ctrl );
void prg_out_aux4                      ( Boolean ctrl );
void prg_out_smartkey_control          ( Boolean ctrl );
void prg_out_pulse_after_start         ( Boolean ctrl );
void prg_out_acc                       ( Boolean ctrl );
void prg_out_ign                       ( Boolean ctrl );
void prg_out_start                     ( Boolean ctrl );
void prg_out_parking_light             ( Boolean ctrl );
void prg_out_domelight                 ( Boolean ctrl );
void prg_out_horn                      ( Boolean ctrl );
void prg_out_unlock                    ( Boolean ctrl );
void prg_out_ground_when_arm           ( Boolean ctrl );
void prg_out_anti_grind                ( Boolean ctrl );
void prg_out_ground_when_running_status( Boolean ctrl ); 

#ifdef PLATFORM_CM800
	
	void prg_out_ground_when_running_5sec  ( Boolean ctrl );
	void prg_out_pulse_before_start        ( Boolean ctrl );
	void prg_out_pulse_after_ign_off       ( Boolean ctrl );
	void prg_out_rear_defogger             ( Boolean ctrl );
	void prg_out_ground_when_triggered     ( Boolean ctrl );

#else

	void prg_out_comfort_closure           ( Boolean ctrl );
	void prg_out_status                    ( Boolean ctrl );
	void prg_out_ground_when_ready         ( Boolean ctrl );
	void prg_out_led_flash                 ( Boolean ctrl );
	void prg_out_defroster_pulse           ( Boolean ctrl );
	void prg_out_defroster_latch           ( Boolean ctrl );
	void prg_out_sensor_trigger            ( Boolean ctrl );
	void prg_out_pulse_after_rs_shutdown   ( Boolean ctrl );
	void prg_out_fob                       ( Boolean ctrl );
	void prg_out_iid_engine_running        ( Boolean ctrl );
#endif

  void prg_out_siren( UInt8 msg );
  extern UInt8 prg_out_timeout;
	
  void prg_out_execute( UInt8 funcnum, UInt8 ctrl );
	UInt8 prg_out_shadow_read( UInt8 funcnum );
	UInt8 prg_out_funcname_lookup ( char *name );
	void prg_out_iid_relay1(UInt8 ctrl);
	
	void prg_out_slide(UInt8 funcnum);
	UInt8 prg_out_user_adj(UInt8 funcnum,int amt);
	
EXTERN_C_LEAVE

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

