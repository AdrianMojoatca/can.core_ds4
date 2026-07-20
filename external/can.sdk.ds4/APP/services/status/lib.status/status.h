/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: status.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __STATUS_H__
#define __STATUS_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"
#include "config_status.h"

#if STATUS_VIN_ENABLE == 1
#include "vin.h"
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef enum e_Status_Src
{   
	STATUS_NO_SOURCE ,     //  disable status update 
	STATUS_SOURCE_1 ,	
	STATUS_SOURCE_2 ,
	STATUS_SOURCE_3 ,
	STATUS_SOURCE_4 ,
	STATUS_SOURCE_COUNT ,

} Status_Src ;

typedef enum e_Status
{   
	STATUS_RPM , 
	STATUS_SPEED ,
	STATUS_DOOR ,
	STATUS_TRANS ,
	STATUS_BRAKE ,
	STATUS_HANDBRAKE ,
	STATUS_KEY ,
	STATUS_ALARM ,
	STATUS_DOORLOCK ,
	STATUS_RKE ,
	STATUS_ENG_FUEL_LEVEL ,
	STATUS_ENG_BAT_VOLT ,
	STATUS_ENG_RUN_TIME ,
	STATUS_ENG_RUNNING ,
    STATUS_ENG_OEM_RS_EQUIPPED ,
	STATUS_MILEAGE ,
	STATUS_COMFORT ,
	STATUS_TEMP_IN ,
	STATUS_TEMP_OUT ,
	STATUS_TEMP_ENG ,
	STATUS_TEMP_SENS ,
	STATUS_DTC ,
	STATUS_RS ,
	STATUS_DEFROST ,
	STATUS_MIRROR_DRIVER ,
	STATUS_MIRROR_PASS ,
	STATUS_SEAT_DRIVER ,
	STATUS_SEAT_PASS,
	STATUS_TPMS ,
    STATUS_LIGHT ,

	ALL_STATUS

} Status ;

//--------------------------------------------------------------------------// 

typedef enum e_Status_Rpm		// order is critical
{   
	STATUS_RPM_WORD,
	STATUS_RPM_HIGH,
	STATUS_RPM_LOW,

	STATUS_RPM_COUNT

} Status_Rpm ;

typedef union s_Rpm
{   
  UInt16 word    ;  
  UInt8  byte[2] ;

} Rpm ;

//--------------------------------------------------------------------------// 

typedef enum e_Status_Speed		// order is critical
{   
	STATUS_SPEED_IS_DETECTED,
	STATUS_SPEED_BYTE,

	STATUS_SPEED_COUNT,

} Status_Speed ;
 
typedef struct s_Speed
{
  struct
    {
      unsigned is_detected : 1 ;
      unsigned reserved    : 7 ;
    } bit ;
  
  UInt8  byte ;
 
} Speed ;

//--------------------------------------------------------------------------// 

typedef enum e_Status_Door		// order is critical
{   
	STATUS_DOOR_FRONT_DRIVER , 
	STATUS_DOOR_FRONT_PASSENGER ,
	STATUS_DOOR_REAR_DRIVER ,
	STATUS_DOOR_REAR_PASSENGER ,
	STATUS_DOOR_TRUNK ,
	STATUS_DOOR_HOOD ,
	STATUS_DOOR_TRUNK_GLASS ,
	STATUS_DOOR_GAS_CAP ,
	STATUS_DOOR_BYTE,

	STATUS_DOOR_COUNT,

} Status_Door ;
 
typedef union s_Door
{   
  struct
    {
      unsigned front_driver : 1 ;
      unsigned front_pass   : 1 ;
      unsigned rear_driver  : 1 ;
      unsigned rear_pass    : 1 ;
      unsigned trunk        : 1 ;
      unsigned hood         : 1 ;
      unsigned trunk_glass  : 1 ;
      unsigned gas_cap      : 1 ;
    } bit ;

  UInt8 byte ;

} Door ;

//--------------------------------------------------------------------------//
#if STATUS_REQ_INPUT_ENABLE == 1
typedef struct s_Req_Input_Status
{  
  union
    { 
      struct
        {
          unsigned driver       : 1 ;
          unsigned other        : 1 ;
          unsigned trunk        : 1 ;
          unsigned hood         : 1 ;
          unsigned brake        : 1 ;
          unsigned key_in       : 1 ;
          unsigned park_neutral : 1 ;
          unsigned handbrake    : 1 ;
        } bit ;
    
      UInt8 byte ;
    } status ;

  union
    { 
      struct
        {
          unsigned driver       : 1 ;
          unsigned other        : 1 ;
          unsigned trunk        : 1 ;
          unsigned hood         : 1 ;
          unsigned brake        : 1 ;
          unsigned key_in       : 1 ;
          unsigned park_neutral : 1 ;
          unsigned handbrake    : 1 ;
        } bit ;
    
      UInt8 byte ;
    } support ;

} Req_Input_Status ;
#endif
//--------------------------------------------------------------------------// 

typedef enum e_Status_Trans		// order is critical
{   
	STATUS_TRANS_PARK , 
	STATUS_TRANS_NEUTRAL ,
	STATUS_TRANS_REVERSE ,
	STATUS_TRANS_FORWARD ,
	STATUS_TRANS_BYTE ,
	
	STATUS_TRANS_COUNT 

} Status_Trans ;

typedef union s_Trans
{   
  struct
    {
      unsigned park     : 1 ;
      unsigned neutral  : 1 ;
      unsigned reverse  : 1 ;
      unsigned forward  : 1 ;
      unsigned reserved : 4 ;
    } bit ;

  UInt8 byte ;

} Trans ;

//--------------------------------------------------------------------------//

typedef enum e_Status_Brake		// order is critical
{   
	STATUS_BRAKE_FOOT , 
	STATUS_BRAKE_EMERGENCY ,
	STATUS_BRAKE_BYTE ,

	STATUS_BRAKE_COUNT

} Status_Brake ;
  
typedef union s_Brake
{   
  struct
    {
      unsigned brake     : 1 ;
      unsigned handbrake : 1 ;
      unsigned reserved  : 6 ;
    } bit ;

  UInt8 byte ;

} Brake ;

//--------------------------------------------------------------------------// 

typedef enum e_Status_Key		// order is critical
{   
	STATUS_KEY_OFF , 
	STATUS_KEY_IN ,
	STATUS_KEY_ACC ,
	STATUS_KEY_IGN ,
	STATUS_KEY_START ,
	STATUS_KEY_BYTE ,

	STATUS_KEY_COUNT

} Status_Key ;
 
typedef union s_Key
{   
  struct
    {
      unsigned off      : 1 ;
      unsigned in       : 1 ;
      unsigned acc      : 1 ;
      unsigned ign      : 1 ;
      unsigned start    : 1 ;
      unsigned reserved : 3 ;
    } bit ;

  UInt8 byte ;

} Key ;

//--------------------------------------------------------------------------// 

typedef enum e_Status_Alarm		// order is critical
{   
	STATUS_ALARM_LIGHT_ON , 
	STATUS_ALARM_LIGHT_OFF ,
	STATUS_ALARM_EQUIPPED ,
	STATUS_ALARM_OFF ,
	STATUS_ALARM_ARMING ,
	STATUS_ALARM_ARMED ,
	STATUS_ALARM_TRIGGERED ,
	STATUS_ALARM_DISARMED ,
	STATUS_ALARM_BYTE ,

	STATUS_ALARM_COUNT

} Status_Alarm ;
 
typedef union s_Alarm
{   
  struct
    {
      unsigned light_on  : 1 ;
      unsigned light_off : 1 ;
      unsigned equipped  : 1 ;
      unsigned off       : 1 ;
      unsigned arming    : 1 ;
      unsigned armed     : 1 ;  
      unsigned triggered : 1 ;  
      unsigned disarmed  : 1 ;
    } bit ;

  UInt8 byte ;

} Alarm ;

//--------------------------------------------------------------------------//  

typedef enum e_Status_Doorlock		// order is critical
{   
	STATUS_DOORLOCK_IS_SUPPORT ,
	STATUS_DOORLOCK_LOCK ,
	STATUS_DOORLOCK_BYTE ,

	STATUS_DOORLOCK_COUNT

} Status_Doorlock ;

typedef union s_Doorlock_Status
{   
  struct
    {
      unsigned is_support : 1 ;
      unsigned lock       : 1 ; 
      unsigned reserved   : 6 ;
    } bit ;

  UInt8 byte ;

} Doorlock_Status ;

//--------------------------------------------------------------------------// 

typedef enum e_Status_Rke		// order is critical
{   
	STATUS_RKE_EQUIPPED ,
	STATUS_RKE_LOCK ,
	STATUS_RKE_UNLOCK ,
	STATUS_RKE_TRUNK ,
	STATUS_RKE_AUX1 ,
	STATUS_RKE_AUX2 ,
	STATUS_RKE_AUX3 ,
	STATUS_RKE_AUX4 ,
	STATUS_RKE_START ,
	STATUS_RKE_STOP ,
	STATUS_RKE_PANIC ,
	STATUS_RKE_SMART_KEY ,
	STATUS_RKE_POP_GLASS ,
	STATUS_RKE_WORD ,

	STATUS_RKE_COUNT

} Status_Rke ;
 
typedef union s_Rke
{   
  struct
    {
      unsigned equipped  : 1 ;
      unsigned lock      : 1 ;
      unsigned unlock    : 1 ;
      unsigned trunk     : 1 ;
      unsigned aux1      : 1 ;
      unsigned aux2      : 1 ;   
      unsigned aux3      : 1 ;   
      unsigned aux4      : 1 ;   
      unsigned start     : 1 ;   
      unsigned stop      : 1 ;
      unsigned panic     : 1 ;
      unsigned smart_key : 1 ;
      unsigned pop_glass : 1 ;
      unsigned idle      : 1 ;
      unsigned reserved  : 2 ;
    } bit ;

  UInt8  byte[(STATUS_RKE_COUNT / 8) + 1] ;

  UInt16 word ;

} Rke ;

//--------------------------------------------------------------------------//

typedef struct
{
  Boolean       oem_rs_equipped ;
  Boolean 		running    ;  
  UInt8   		fuel_level ;
  UInt8   		bat_volt   ;
  UInt16  		run_time   ;

} Engine ;

//--------------------------------------------------------------------------//

typedef enum e_Status_Mileage
{   
	STATUS_MILEAGE_DWORD ,
  STATUS_MILEAGE_WORD_HI,
  STATUS_MILEAGE_WORD_LO,  
	STATUS_MILEAGE_BYTE_HI ,
  STATUS_MILEAGE_BYTE_MED,
  STATUS_MILEAGE_BYTE_LO,  

} Status_Mileage ;

typedef union s_Mileage
{
  UInt32 		dword ;
  UInt16 		word[2] ;
  UInt8			byte[3] ;

} Mileage ;

//--------------------------------------------------------------------------//

typedef enum e_Status_TPMS
{   
	STATUS_TPMS_LIGHT ,
	STATUS_TPMS_FRONT_DRIVER ,
	STATUS_TPMS_FRONT_PASSENGER ,
	STATUS_TPMS_REAR_DRIVER ,
	STATUS_TPMS_REAR_PASSENGER ,

} Status_TPMS ;

typedef struct
{
  Boolean 		light_status	;
  UInt8 		front_driver    ;
  UInt8 		front_passenger ;
  UInt8 		rear_driver     ;
  UInt8 		rear_passenger  ;

} Tpms ;

//--------------------------------------------------------------------------//

typedef union
{
  struct
    {
      unsigned defrost_id             : 1 ; // available yes/no
      unsigned driver_mirror_id       : 1 ; // available yes/no
      unsigned passenger_mirror_id    : 1 ; // available yes/no
      unsigned driver_seat_id         : 1 ; // available yes/no  
      unsigned passenger_seat_id      : 1 ;
      
      unsigned defrost_state          : 1 ;  // on/off
      unsigned driver_mirror_state    : 1 ;  // on/off
      unsigned passenger_mirror_state : 1 ;  // on/off
      unsigned climate_state_driver   : 1 ;  // on/off
      unsigned climate_state_passenger: 1 ;  // on/off

      unsigned driver_seat_position   : 3 ;  // position desire
      unsigned driver_seat_current    : 3 ;  // car position
      unsigned passenger_seat_position: 3 ;  // position desire
      unsigned passenger_seat_current : 3 ;  // car position

      unsigned reserved               : 10;
    } bit ;

  UInt32 data ; 

} Comfort ;

//--------------------------------------------------------------------------//
// new structure used for get and set
//--------------------------------------------------------------------------//

typedef struct
{
  struct
    {
	  unsigned state                 : 1 ;  // on/off
	} defrost ;

  struct
    {
      unsigned driver_state          : 1 ;  // on/off
      unsigned passenger_state       : 1 ;  // on/off     
    } mirror ;

  struct
    {
      unsigned driver_state          : 8 ;  // on/off
      unsigned passenger_state	     : 8 ;  // on/off     
    } seat ;

} Comfort2 ;

//--------------------------------------------------------------------------//
typedef struct
{
  UInt8 in          ;		// INTAKE temperature
  UInt8 out         ;		// OUTSIDE temperature - outside the car (not available via OBD)
  UInt8 eng_coolant ;		// ENGINE COOLANT temperature
  UInt8 cabin       ;		// CABIN temperature - inside the car
  UInt8 sensor      ;   // SENSOR temperature - shock sensor or temperature probe
  unsigned in_ena      : 1 ;
  unsigned out_ena     : 1 ;
  unsigned coolant_ena : 1 ;
  unsigned cabin_ena   : 1 ;
	unsigned sensor_ena  : 1 ;

} Temperature ;

//--------------------------------------------------------------------------//

typedef enum e_Status_Dtc		// order is critical
{   
	STATUS_DTC_CODE_1  ,
	STATUS_DTC_CODE_2  ,
	STATUS_DTC_CODE_3  ,
	STATUS_DTC_CODE_4  ,
	STATUS_DTC_CODE_5  ,
	STATUS_DTC_CODE_6  ,
	STATUS_DTC_CODE_7  ,
	STATUS_DTC_CODE_8  ,
	STATUS_DTC_CODE_9  ,
	STATUS_DTC_CODE_10 ,
	STATUS_DTC_CODE_11 ,
	STATUS_DTC_CODE_12 ,
	STATUS_DTC_CODE_13 ,
	STATUS_DTC_CODE_14 ,
	STATUS_DTC_CODE_15 ,

	STATUS_DTC_SIZE    ,

	STATUS_DTC_COUNT   ,

} Status_Dtc ;

typedef struct s_Dtc
{
  UInt8 count ;

  union
    {
      UInt8  _8_  [30] ; 
      UInt16 _16_ [15] ; 
    } code ;

} Dtc ;

//--------------------------------------------------------------------------//
typedef struct
{
  unsigned parking       : 1 ;
  unsigned lo_beam       : 1 ;
  unsigned hi_beam       : 1 ;  
  unsigned hazard        : 1 ;
  unsigned left_turn     : 1 ;
  unsigned right_turn    : 1 ;
  unsigned fog           : 1 ;
  unsigned reserve       : 1 ;

} Status_Light ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
#if STATUS_RPM_ENABLE == 1
Boolean 	status_set_rpm 				(UInt16 value, Status_Rpm status, Status_Src source) ;
SInt32 		status_get_rpm 				(Status_Rpm status) ;
#endif
#if STATUS_SPEED_ENABLE > 0   // support in d2d AND in m2m, so we keep weird d2d arguments...
Boolean 	status_set_speed 			(UInt8 value, Status_Speed status, Status_Src source) ;
SInt16 		status_get_speed 			(Status_Speed status) ;
UInt16    atatus_get_m2m_speed(void);
#endif
#if STATUS_DOOR_ENABLE == 1
Boolean 	status_set_door 			(UInt8 value , Status_Door status, Status_Src source)	;
SInt16 		status_get_door 			(Status_Door status) ;
#endif
#if STATUS_TRANS_ENABLE == 1
Boolean 	status_set_trans 			(UInt8 value , Status_Trans status, Status_Src source)	;
SInt16 		status_get_trans 			(Status_Trans status) ;
#endif
#if STATUS_BRAKE_ENABLE == 1
Boolean 	status_set_brake 			(UInt8 value , Status_Brake status, Status_Src source)	;
SInt16 		status_get_brake 			(Status_Brake status) ;
#endif
#if STATUS_KEY_ENABLE == 1
Boolean 	status_set_key	 			(UInt8 value , Status_Key status, Status_Src source)	;
SInt16 		status_get_key	 			(Status_Key status) ;
#endif
#if STATUS_ALARM_ENABLE == 1
Boolean 	status_set_alarm 			(UInt8 value , Status_Alarm status, Status_Src source)	;
SInt16 		status_get_alarm 			(Status_Alarm status) ;
#endif
#if STATUS_DOORLOCK_ENABLE == 1
Boolean 	status_set_doorlock			(UInt8 value , Status_Doorlock status, Status_Src source)	;
SInt16 		status_get_doorlock			(Status_Doorlock status) ;
#endif
#if STATUS_RKE_ENABLE == 1
Boolean 	status_set_rke				(UInt16 value , Status_Rke status, Status_Src source)	;
SInt32 		status_get_rke				(Status_Rke status) ;
#endif
#if STATUS_ENGINE_ENABLE == 1
Boolean 	status_set_engine			(UInt16 value , Status status, Status_Src source)	;
SInt32 		status_get_engine			(Status status) ;
#endif
#if STATUS_COMFORT_ENABLE == 1
Boolean 	status_set_comfort			(UInt8 value , Status status, Status_Src source)	;
SInt16 		status_get_comfort			(Status status) ;
#endif
#if STATUS_TEMPERATURE_ENABLE == 1
Boolean 	status_set_temp 			(UInt8 value, Status status, Status_Src source) ;
SInt16 		status_get_temp 			(Status status) ;
#endif
#if STATUS_DTC_ENABLE == 1
Boolean 	status_set_dtc 				(UInt16 value , Status_Dtc status,  Status_Src source) ;
SInt32 		status_get_dtc 				(Status_Dtc status) ;
#endif
#if STATUS_MILEAGE_ENABLE == 1
Boolean   status_set_mileage    (UInt32 value, Status_Mileage status, Status_Src source) ;
SInt32 	  status_get_mileage 		(Status_Mileage status) ;
#endif
#if STATUS_TPMS_ENABLE == 1
Boolean 	status_set_tpms				(UInt8 value , Status_TPMS status,  Status_Src source) ;
SInt16 		status_get_tpms 			(Status_TPMS status) ;
#endif

#if STATUS_OIL_ALARM_ENABLE > 0
void 	status_set_oil_alarm			(Boolean value)	;
Boolean status_get_oil_alarm     (void);
#endif

#if STATUS_TIRE_ALARM_ENABLE > 0
void 	status_set_tire_alarm			(Boolean value)	;
Boolean status_get_tire_alarm     (void);
#endif

#if STATUS_SEAT_BELTS_ENABLE > 0
void 	status_set_seat_belts			(UInt8 bitmap)	;
UInt8 status_get_seat_belts    (void);
#endif

#if STATUS_SEAT_WEIGHTS_ENABLE > 0
void 	status_set_seat_weights			(UInt8 bitmap)	;
UInt8 status_get_seat_weights    (void);
#endif

#if STATUS_EV_BATTERY_LVL_ENABLE > 0
void status_set_ev_battery_lvl(UInt8 lvl);
UInt8 status_get_ev_battery_lvl(void);
#endif

#if STATUS_EV_RANGE_ENABLE > 0
void status_set_ev_range(UInt16 rangekm);
UInt8 status_get_ev_range(void);
#endif

#if STATUS_OIL_LIFE_ENABLE > 0
void status_set_oil_life(UInt8 remaining);
UInt8 status_get_oil_life(void);
#endif

#if STATUS_FUEL_LVL_ENABLE > 0
void status_set_fuel_lvl(UInt8 lvl);
UInt8 status_get_fuel_lvl(void);
#endif

#if STATUS_FUEL_CAP_ENABLE > 0
void status_set_fuel_cap(UInt8 litres);
UInt8 status_get_fuel_cap(void);
#endif

#if STATUS_CABIN_TEMP_ENABLE > 0
void status_set_cabin_temp(SInt8 degC);
SInt8 status_get_cabin_temp(void);
#endif

#if STATUS_TIRE_PRESSURE_ENABLE > 0
void status_set_tire_pressure(int tire, UInt8 psi);
int status_get_tire_pressure(int tire);
#endif

Boolean status_get_kib_state(void);

#if STATUS_ODOMETER_ENABLE > 0
void status_set_odometer(UInt32 km);
UInt32 status_get_odometer(void);
#endif

#if STATUS_BATTERY_VOLTAGE_ENABLE > 0
void status_set_battery_voltage(UInt16 millivolts);
UInt16 status_get_battery_voltage(void);
#endif

#if STATUS_SPEED_ENABLE > 0
// there is no status_set_m2m_speed because we pick up the existing status_set_speed(arg) from d2d
UInt16 status_get_m2m_speed(void);
#endif


Boolean 	status_set_source			(Status_Src source , Status status)	;
Status_Src	status_get_source			(Status status) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

#if STATUS_VIN_ENABLE == 1
extern Vin vin ;
#endif
#if STATUS_RKE_ENABLE == 1
extern Rke rke ;
#endif
#if STATUS_KEY_ENABLE == 1
extern Key key ;
#endif
#if STATUS_RPM_ENABLE == 1
extern Rpm rpm ;
#endif
#if STATUS_DOOR_ENABLE == 1
extern Door door ;
#endif
#if STATUS_SPEED_ENABLE > 0
extern Speed speed ;
#endif
#if STATUS_BRAKE_ENABLE == 1
extern Brake brake ;
#endif
#if STATUS_TRANS_ENABLE == 1
extern Trans trans ;
#endif
#if STATUS_ALARM_ENABLE == 1
extern Alarm alarm ;
#endif
#if STATUS_DOORLOCK_ENABLE == 1
extern Doorlock_Status doorlock_status ;
#endif
#if STATUS_ENGINE_ENABLE == 1
extern Engine engine ;
#endif
#if STATUS_COMFORT_ENABLE == 1
extern Comfort  comfort ;
extern Comfort2 comfort2 ;
#endif
#if STATUS_TEMPERATURE_ENABLE == 1
extern Temperature temperature ;
#endif
#if STATUS_REQ_INPUT_ENABLE == 1
extern Req_Input_Status req_input ;
#endif
#if STATUS_DTC_ENABLE == 1
extern Dtc dtc ;
#endif

#if STATUS_MILEAGE_ENABLE == 1
extern Mileage mileage ;
#endif

extern Status_Light status_light ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif
