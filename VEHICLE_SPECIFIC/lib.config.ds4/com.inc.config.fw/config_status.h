/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: config_status.h 20892 2013-05-22 18:59:57Z martin.bouchard $
/*==========================================================================*/

//             <<< Use Configuration Wizard in Context Menu >>>             //

#ifndef __CONFIG_STATUS_H__
#define __CONFIG_STATUS_H__

#define CORE_CONFIG_STATUS_VERSION_2_00

// <h>  Status Variables

// <q0> Vin
// <q1> Rke
// <q2> Key
// <q3> Rpm
// <q4> Door
// <q5> Speed
// <q6> Brake
// <q7> Trans
// <q8> Alarm
// <q9> Engine
// <q10> Comfort
// <q11> Temperature
// <q12> Req Input Status
// <q13> Dtc
// <q14> Mileage
// <q15> Tpms
// <q16> Door Lock
// </h>
  
#define STATUS_VIN_ENABLE			1	// 
#define STATUS_RKE_ENABLE			1	//
#define STATUS_KEY_ENABLE			1	// 
#define STATUS_RPM_ENABLE			1	//
#define STATUS_DOOR_ENABLE			1	// 
#define STATUS_SPEED_ENABLE			1	//
#define STATUS_BRAKE_ENABLE			1	// 
#define STATUS_TRANS_ENABLE			1	//
#define STATUS_ALARM_ENABLE			1	// 
#define STATUS_ENGINE_ENABLE		1	//
#define STATUS_COMFORT_ENABLE		1	//
#define STATUS_TEMPERATURE_ENABLE	1	//
#define STATUS_REQ_INPUT_ENABLE		1	//
#define STATUS_DTC_ENABLE           1   //
#define STATUS_MILEAGE_ENABLE       1   //
#define STATUS_TPMS_ENABLE          1   //
#define STATUS_DOORLOCK_ENABLE		1	// 

//<h> M2M Status
// <o0> Oil Low Alarm
//     <0=> No support
//     <1=> Automatic
//     <2=> Must Request
// <o1> Low Tire Pressure Alarm
//     <0=> No support
//     <1=> Automatic
//     <2=> Must Request
// <o2> Seat Belt bitmap
//     <0=> No support
//     <1=> Automatic
//     <2=> Must Request
// <o3> Seat Weight bitmap
//     <0=> No support
//     <1=> Automatic
//     <2=> Must Request
// <o4> EV Battery level
//     <0=> No support
//     <1=> Automatic
//     <2=> Must Request
// <o5> EV Range
//     <0=> No support
//     <1=> Automatic
//     <2=> Must Request
// <o6> Oil Life Remaining
//     <0=> No support
//     <1=> Automatic
//     <2=> Must Request
// <o7> Fuel level
//     <0=> No support
//     <1=> Automatic
//     <2=> Must Request
// <o8> Fuel capacity	
//     <0=> No support
//     <1=> Automatic
//     <2=> Must Request
// <o9> Cabin temperature	
//     <0=> No support
//     <1=> Automatic
//     <2=> Must Request
// <o10> Tire Pressure bitmap	
//     <0=> No support
//     <1=> Automatic
//     <2=> Must Request
// <o11> Odometer
//     <0=> No support
//     <1=> Automatic
//     <2=> Must Request
// <o12> Battery Voltage
//     <0=> No support
//     <1=> Automatic
//     <2=> Must Request
//</h>

#define STATUS_OIL_ALARM_ENABLE         1 //
#define STATUS_TIRE_ALARM_ENABLE        1 //
#define STATUS_SEAT_BELTS_ENABLE        1 //
#define STATUS_SEAT_WEIGHTS_ENABLE      1 //
#define STATUS_EV_BATTERY_LVL_ENABLE  1 //
#define STATUS_EV_RANGE_ENABLE 1 //
#define STATUS_OIL_LIFE_ENABLE 1 //
#define STATUS_FUEL_LVL_ENABLE 1 //
#define STATUS_FUEL_CAP_ENABLE 1 //
#define STATUS_CABIN_TEMP_ENABLE 1 //
#define STATUS_TIRE_PRESSURE_ENABLE 1//
#define STATUS_ODOMETER_ENABLE 1 //
#define STATUS_BATTERY_VOLTAGE_ENABLE 1 //


// <h>  M2M Functions

// <q0> Honk pattern
// <q1> Headlight pattern
// <q2> RIB
// </h>
#define FUNCTION_HONKS_ENABLE 1 //
#define FUNCTION_HEADLIGHTS_ENABLE 1 //
#define FUNCTION_RIB_ENABLE 1 //  firmware supports special CAN msgs or not

#endif

