/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: config_rsr.h 24723 2014-03-06 22:12:23Z louis-philippe.rispoli $
/*==========================================================================*/

//             <<< Use Configuration Wizard in Context Menu >>>             //

#ifndef __CONFIG_RSR_H__
#define __CONFIG_RSR_H__

//--------------------------------------------------------------------------//
#define CONFIG_RSR_VERSION 5  //will generate an error if lib.rsr does not match with config file
//--------------------------------------------------------------------------//
// <h> RSR OUTPUT CONFIG
// <e0> ACC 	Enable
// </e>
#define RSR_ACC_ENA				   	   0 
// <e0> IGN 	Enable
// </e>
#define RSR_IGN_ENA				   	   0 
// <e0> STARTER Enable
// </e>
#define RSR_STARTER_ENA				   0 
// <e0> LOCK 	Enable
//    <o1> Pulse Count
//        <1=> *1
//        <2=>  2
//    <o2> Duration
//        <400=>   *400 ms
//        <800=>    800 ms
//        <3500=>  3500 ms
// </e>
#define RSR_LOCK_ENA				   0 
#define RSR_LOCK_PULSE				   1 	   // pulse count
#define RSR_LOCK_DURATION			   400 	   // timebase millisecond
// <e0> UNLOCK 	Enable
//    <o1> Pulse Count
//        <1=> *1
//        <2=>  2
//    <o2> Duration
//        <400=>   *400 ms
//        <800=>    800 ms
//        <3500=>  3500 ms
// </e>
#define RSR_UNLOCK_ENA				   0 
#define RSR_UNLOCK_PULSE			   1 	   // pulse count
#define RSR_UNLOCK_DURATION			   400 	   // timebase millisecond
// <e0> ARM 	Enable
//    <o1> OEM Alarm Arm Output
//       <1=>*With lock 
//       <2=> Remote Start Only	(Smart OEM Alarm Control)
// </e>
#define RSR_ARM_ENA				   	   0 
#define RSR_ARM_OUTPUT				   1 
// <e0> DISARM 	Enable
//    <o1> OEM Alarm Disarm Output
//       <1=>*With Unlock       
//       <2=> Before Unlock     
//       <3=> Remote Start Only	(Smart OEM Alarm Control)
// </e>
#define RSR_DISARM_ENA				   0 
#define RSR_DISARM_OUTPUT			   1

// <e0> TRUNK 	Enable
// </e>
#define RSR_TRUNK_ENA				   0 
// <e0> PARKING LIGHTS Enable
//    <o1> Parking Flash enabled with DL functions
//        <0=> Disabled
//        <1=> Enabled 
// </e>
#define RSR_PARKING_ENA                0
#define RSR_DL_PARKING_FLASH           0

//--------------------------------------------------------------------------//
// </h>
// <h> RSR INPUT CONFIG
// <h> Hood
//    <o0> Input Selection
//        <0=> GWR_IN
//        <1=> IGN_IN
//        <2=> POS_IN1
//        <3=> AUX3_IN
//        <4=> AUX2_IN
//        <5=> AUX1_IN
//        <6=> TRUNK_IN
//        <7=> UNLOCK_IN
//        <8=> LOCK_IN
//    <o1> Polatity
//        <0=>   Normal
//        <1=>   Reversed
// </h>
#define RSR_HOOD_SEL     	 3   
#define RSR_HOOD_POLARITY 	 0      
// <h> Valet
//    <o0> Input Selection
//        <0=> GWR_IN
//        <1=> IGN_IN
//        <2=> POS_IN1
//        <3=> AUX3_IN
//        <4=> AUX2_IN
//        <5=> AUX1_IN
//        <6=> TRUNK_IN
//        <7=> UNLOCK_IN
//        <8=> LOCK_IN
// </h>
#define RSR_VALET_SEL	    3   



//--------------------------------------------------------------------------//
// </h>
// <h> RSR OPTIONS CONFIG
//    <o0> DTC CHECK
//        <0=> Disabled
//        <1=> Don't RS if code is found
//    <o1> Transmission Check
//        <0=> Disabled
//        <1=> Don't RS if transmission is in gear
//    <o2> Default Crank time(millisecond) <100-8000:100>
//    <o3> Default RPM RUN * 100 (underev limit)<1-15:1>
//    <o4> Default RPM CRANK * 100 (starter OFF limit)<1-15:1>
//    <o5> Default RPM HIGH * 100 (overev limit)<20-50:1>
//    <o6> Idle Mode Enable
//        <0=> Disabled
//        <1=> *Enable
#define RSR_DTC_CHECK       		    0
#define RSR_TRANS_CHECK     		    0
#define DEFAULT_RSR_CRANK_TIME       	5000// timebase millisecond
#define DEFAULT_RSR_RPM_RUN_VALUE    	3   // * 100
#define DEFAULT_RSR_RPM_CRANK_VALUE     5   // * 100
#define DEFAULT_RSR_RPM_HIGH_VALUE    	30  // * 100
#define RSR_IDLE_MODE_ENABLE       	    1
// </h>

//--------------------------------------------------------------------------//

#endif

