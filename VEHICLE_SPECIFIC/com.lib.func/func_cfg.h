/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: config_firmware.h 82 2008-03-21 19:35:35Z martin.bouchard $
/*==========================================================================*/

//             <<< Use Configuration Wizard in Context Menu >>>             //

#ifndef __BMW_FUNC_CFG_H__
#define __BMW_FUNC_CFG_H__

/*==========================================================================*/
/*                                N O T E S                                 */
/*==========================================================================*/
/* This file is included by 'device.h' and represents global compile-time   */
/* configuration parameters.  These parameters can be edited through the    */
/* configuration tool in the RealView IDE.                                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/
//<h>[0 ].Arm Configuration
/*--------------------------------------------------------------------------*/
//<h>Arm blocked by
//    <o0.0 > Arm
//    <o0.1 > Disarm
//    <o0.2 > Lock
//    <o0.3 > Unlock
//    <o0.4 > Trunk Release
//    <o0.5 > Relock Trunk Release
//    <o0.6 > Oem Lock 
//    <o0.7 > Oem Unlock
//    <o0.8 > Oem Trunk
//    <o0.9 > Windows Roll Up
//    <o0.10> Windows Roll Down Front
//    <o0.11> Windows Roll Down Rear
//    <o0.12> Gas Cap
//    <o0.13> Light
//    <o0.14> Light Disable
//    <o0.15> Domelight
//</h>

//<h>Arm suspended by
//    <o1.0 > Arm
//    <o1.1 > Disarm
//    <o1.2 > Lock
//    <o1.3 > Unlock
//    <o1.4 > Trunk Release
//    <o1.5 > Relock Trunk Release
//    <o1.6 > Oem Lock 
//    <o1.7 > Oem Unlock
//    <o1.8 > Oem Trunk
//    <o1.9 > Windows Roll Up
//    <o1.10> Windows Roll Down Front
//    <o1.11> Windows Roll Down Rear
//    <o1.12> Gas Cap
//    <o1.13> Light
//    <o1.14> Light Disable
//    <o1.15> Domelight
//</h>

//<o2>Arm Process Timeout[ms]                  <0-60000>

//</h>

#define FUNC_ARM_BLOCKED_BY         0x000301D2
#define FUNC_ARM_SUSPENDED_BY       0x00000000
#define FUNC_ARM_TIMEOUT            500

/*--------------------------------------------------------------------------*/
//<h>[1 ].Disarm Configuration
/*--------------------------------------------------------------------------*/
//<h> Disarm blocked by
//    <o0.0 > Arm
//    <o0.1 > Disarm
//    <o0.2 > Lock
//    <o0.3 > Unlock
//    <o0.4 > Trunk Release
//    <o0.5 > Relock Trunk Release
//    <o0.6 > Oem Lock 
//    <o0.7 > Oem Unlock
//    <o0.8 > Oem Trunk
//    <o0.9 > Windows Roll Up
//    <o0.10> Windows Roll Down Front
//    <o0.11> Windows Roll Down Rear
//    <o0.12> Gas Cap
//    <o0.13> Light
//    <o0.14> Light Disable
//    <o0.15> Domelight
//</h>

//<h> Disarm suspended by
//    <o1.0 > Arm
//    <o1.1 > Disarm
//    <o1.2 > Lock
//    <o1.3 > Unlock
//    <o1.4 > Trunk Release
//    <o1.5 > Relock Trunk Release
//    <o1.6 > Oem Lock 
//    <o1.7 > Oem Unlock
//    <o1.8 > Oem Trunk
//    <o1.9 > Windows Roll Up
//    <o1.10> Windows Roll Down Front
//    <o1.11> Windows Roll Down Rear
//    <o1.12> Gas Cap
//    <o1.13> Light
//    <o1.14> Light Disable
//    <o1.15> Domelight
//</h>

//<o2> Disarm Process Timeout[ms]                  <0-60000>

//</h>

#define FUNC_DISARM_BLOCKED_BY         0x000301C0
#define FUNC_DISARM_SUSPENDED_BY       0x000001C0
#define FUNC_DISARM_TIMEOUT            500

/*--------------------------------------------------------------------------*/
//<h>[2 ].Lock Configuration
/*--------------------------------------------------------------------------*/

//<h> Lock blocked by
//    <o0.0 > Arm
//    <o0.1 > Disarm
//    <o0.2 > Lock
//    <o0.3 > Unlock
//    <o0.4 > Trunk Release
//    <o0.5 > Relock Trunk Release
//    <o0.6 > Oem Lock 
//    <o0.7 > Oem Unlock
//    <o0.8 > Oem Trunk
//    <o0.9 > Windows Roll Up
//    <o0.10> Windows Roll Down Front
//    <o0.11> Windows Roll Down Rear
//    <o0.12> Gas Cap
//    <o0.13> Light
//    <o0.14> Light Disable
//    <o0.15> Domelight
//</h>

//<h> Lock suspended by
//    <o1.0 > Arm
//    <o1.1 > Disarm
//    <o1.2 > Lock
//    <o1.3 > Unlock
//    <o1.4 > Trunk Release
//    <o1.5 > Relock Trunk Release
//    <o1.6 > Oem Lock 
//    <o1.7 > Oem Unlock
//    <o1.8 > Oem Trunk
//    <o1.9 > Windows Roll Up
//    <o1.10> Windows Roll Down Front
//    <o1.11> Windows Roll Down Rear
//    <o1.12> Gas Cap
//    <o1.13> Light
//    <o1.14> Light Disable
//    <o1.15> Domelight
//</h>

//<o2> Lock Process Timeout[ms]                  <0-60000>

//</h>

#define FUNC_LOCK_BLOCKED_BY         0x000301D2
#define FUNC_LOCK_SUSPENDED_BY       0x00000000
#define FUNC_LOCK_TIMEOUT            500

/*--------------------------------------------------------------------------*/
//<h>[3 ].Unlock Configuration
/*--------------------------------------------------------------------------*/

//<h> Unlock blocked by
//    <o0.0 > Arm
//    <o0.1 > Disarm
//    <o0.2 > Lock
//    <o0.3 > Unlock
//    <o0.4 > Trunk Release
//    <o0.5 > Relock Trunk Release
//    <o0.6 > Oem Lock 
//    <o0.7 > Oem Unlock
//    <o0.8 > Oem Trunk
//    <o0.9 > Windows Roll Up
//    <o0.10> Windows Roll Down Front
//    <o0.11> Windows Roll Down Rear
//    <o0.12> Gas Cap
//    <o0.13> Light
//    <o0.14> Light Disable
//    <o0.15> Domelight
//</h>

//<h> Unlock suspended by
//    <o1.0 > Arm
//    <o1.1 > Disarm
//    <o1.2 > Lock
//    <o1.3 > Unlock
//    <o1.4 > Trunk Release
//    <o1.5 > Relock Trunk Release
//    <o1.6 > Oem Lock 
//    <o1.7 > Oem Unlock
//    <o1.8 > Oem Trunk
//    <o1.9 > Windows Roll Up
//    <o1.10> Windows Roll Down Front
//    <o1.11> Windows Roll Down Rear
//    <o1.12> Gas Cap
//    <o1.13> Light
//    <o1.14> Light Disable
//    <o1.15> Domelight
//</h>

//<o2> Unlock Process Timeout[ms]                  <0-60000>

//</h>


#define FUNC_UNLOCK_BLOCKED_BY         0x000301D0
#define FUNC_UNLOCK_SUSPENDED_BY       0x00000002
#define FUNC_UNLOCK_TIMEOUT            500


/*--------------------------------------------------------------------------*/
//<h>[4 ].Trunk Release Configuration
/*--------------------------------------------------------------------------*/

//<h> Trunk Release blocked by
//    <o0.0 > Arm
//    <o0.1 > Disarm
//    <o0.2 > Lock
//    <o0.3 > Unlock
//    <o0.4 > Trunk Release
//    <o0.5 > Relock Trunk Release
//    <o0.6 > Oem Lock 
//    <o0.7 > Oem Unlock
//    <o0.8 > Oem Trunk
//    <o0.9 > Windows Roll Up
//    <o0.10> Windows Roll Down Front
//    <o0.11> Windows Roll Down Rear
//    <o0.12> Gas Cap
//    <o0.13> Light
//    <o0.14> Light Disable
//    <o0.15> Domelight
//</h>

//<h> Trunk release suspended by
//    <o1.0 > Arm
//    <o1.1 > Disarm
//    <o1.2 > Lock
//    <o1.3 > Unlock
//    <o1.4 > Trunk Release
//    <o1.5 > Relock Trunk Release
//    <o1.6 > Oem Lock 
//    <o1.7 > Oem Unlock
//    <o1.8 > Oem Trunk
//    <o1.9 > Windows Roll Up
//    <o1.10> Windows Roll Down Front
//    <o1.11> Windows Roll Down Rear
//    <o1.12> Gas Cap
//    <o1.13> Light
//    <o1.14> Light Disable
//    <o1.15> Domelight
//</h>

//<o2> Trunk Release Process Timeout[ms]                  <0-60000>

//</h>


#define FUNC_TRUNK_RELEASE_BLOCKED_BY         0x000301C0
#define FUNC_TRUNK_RELEASE_SUSPENDED_BY       0x00000002
#define FUNC_TRUNK_RELEASE_TIMEOUT            5000

/*--------------------------------------------------------------------------*/
//<h>[5 ].Trunk Release Relock Configuration
/*--------------------------------------------------------------------------*/

//<h>Trunk Release Relock blocked by
//    <o0.0 > Arm
//    <o0.1 > Disarm
//    <o0.2 > Lock
//    <o0.3 > Unlock
//    <o0.4 > Trunk Release
//    <o0.5 > Relock Trunk Release
//    <o0.6 > Oem Lock 
//    <o0.7 > Oem Unlock
//    <o0.8 > Oem Trunk
//    <o0.9 > Windows Roll Up
//    <o0.10> Windows Roll Down Front
//    <o0.11> Windows Roll Down Rear
//    <o0.12> Gas Cap
//    <o0.13> Light
//    <o0.14> Light Disable
//    <o0.15> Domelight
//</h>

//<h>Trunk Release Relock suspended by
//    <o1.0 > Arm
//    <o1.1 > Disarm
//    <o1.2 > Lock
//    <o1.3 > Unlock
//    <o1.4 > Trunk Release
//    <o1.5 > Relock Trunk Release
//    <o1.6 > Oem Lock 
//    <o1.7 > Oem Unlock
//    <o1.8 > Oem Trunk
//    <o1.9 > Windows Roll Up
//    <o1.10> Windows Roll Down Front
//    <o1.11> Windows Roll Down Rear
//    <o1.12> Gas Cap
//    <o1.13> Light
//    <o1.14> Light Disable
//    <o1.15> Domelight
//</h>

//<o2> Trunk Release Relock Timeout[ms]                  <0-60000>

//</h>

#define FUNC_RELOCK_TR_BLOCKED_BY         0x000001CD
#define FUNC_RELOCK_TR_SUSPENDED_BY       0x00001012
#define FUNC_RELOCK_TR_TIMEOUT            1000

/*--------------------------------------------------------------------------*/
//<h>[6 ].Oem Lock Configuration
/*--------------------------------------------------------------------------*/

//<h>Oem Lock blocked by
//    <o0.0 > Arm
//    <o0.1 > Disarm
//    <o0.2 > Lock
//    <o0.3 > Unlock
//    <o0.4 > Trunk Release
//    <o0.5 > Relock Trunk Release
//    <o0.6 > Oem Lock 
//    <o0.7 > Oem Unlock
//    <o0.8 > Oem Trunk
//    <o0.9 > Windows Roll Up
//    <o0.10> Windows Roll Down Front
//    <o0.11> Windows Roll Down Rear
//    <o0.12> Gas Cap
//    <o0.13> Light
//    <o0.14> Light Disable
//    <o0.15> Domelight
//</h>

//<h>Oem Lock suspended by
//    <o1.0 > Arm
//    <o1.1 > Disarm
//    <o1.2 > Lock
//    <o1.3 > Unlock
//    <o1.4 > Trunk Release
//    <o1.5 > Relock Trunk Release
//    <o1.6 > Oem Lock 
//    <o1.7 > Oem Unlock
//    <o1.8 > Oem Trunk
//    <o1.9 > Windows Roll Up
//    <o1.10> Windows Roll Down Front
//    <o1.11> Windows Roll Down Rear
//    <o1.12> Gas Cap
//    <o1.13> Light
//    <o1.14> Light Disable
//    <o1.15> Domelight
//</h>

//<o2> Oem Lock Timeout[ms]                  <0-60000>

//</h>



#define FUNC_OEM_LOCK_BLOCKED_BY         0x00020180
#define FUNC_OEM_LOCK_SUSPENDED_BY       0x00010000
#define FUNC_OEM_LOCK_TIMEOUT            2000

/*--------------------------------------------------------------------------*/
//<h>[7 ].Oem Unlock Configuration
/*--------------------------------------------------------------------------*/

//<h> Oem Unlock blocked by
//    <o0.0 > Arm
//    <o0.1 > Disarm
//    <o0.2 > Lock
//    <o0.3 > Unlock
//    <o0.4 > Trunk Release
//    <o0.5 > Relock Trunk Release
//    <o0.6 > Oem Lock 
//    <o0.7 > Oem Unlock
//    <o0.8 > Oem Trunk
//    <o0.9 > Windows Roll Up
//    <o0.10> Windows Roll Down Front
//    <o0.11> Windows Roll Down Rear
//    <o0.12> Gas Cap
//    <o0.13> Light
//    <o0.14> Light Disable
//    <o0.15> Domelight
//</h>

//<h> Oem Unlock suspended by
//    <o1.0 > Arm
//    <o1.1 > Disarm
//    <o1.2 > Lock
//    <o1.3 > Unlock
//    <o1.4 > Trunk Release
//    <o1.5 > Relock Trunk Release
//    <o1.6 > Oem Lock 
//    <o1.7 > Oem Unlock
//    <o1.8 > Oem Trunk
//    <o1.9 > Windows Roll Up
//    <o1.10> Windows Roll Down Front
//    <o1.11> Windows Roll Down Rear
//    <o1.12> Gas Cap
//    <o1.13> Light
//    <o1.14> Light Disable
//    <o1.15> Domelight
//</h>

//<o2> OEM Unlock Process Timeout[ms]                  <0-60000>

//</h>

#define FUNC_OEM_UNLOCK_BLOCKED_BY         0x00030000
#define FUNC_OEM_UNLOCK_SUSPENDED_BY       0x00000000
#define FUNC_OEM_UNLOCK_TIMEOUT            2000


/*--------------------------------------------------------------------------*/
//<h>[8 ].Oem Trunk Configuration
/*--------------------------------------------------------------------------*/

//<h> OEM Trunk blocked by
//    <o0.0 > Arm
//    <o0.1 > Disarm
//    <o0.2 > Lock
//    <o0.3 > Unlock
//    <o0.4 > Trunk Release
//    <o0.5 > Relock Trunk Release
//    <o0.6 > Oem Lock 
//    <o0.7 > Oem Unlock
//    <o0.8 > Oem Trunk
//    <o0.9 > Windows Roll Up
//    <o0.10> Windows Roll Down Front
//    <o0.11> Windows Roll Down Rear
//    <o0.12> Gas Cap
//    <o0.13> Light
//    <o0.14> Light Disable
//    <o0.15> Domelight
//</h>

//<h> OEM Trunk suspended by
//    <o1.0 > Arm
//    <o1.1 > Disarm
//    <o1.2 > Lock
//    <o1.3 > Unlock
//    <o1.4 > Trunk Release
//    <o1.5 > Relock Trunk Release
//    <o1.6 > Oem Lock 
//    <o1.7 > Oem Unlock
//    <o1.8 > Oem Trunk
//    <o1.9 > Windows Roll Up
//    <o1.10> Windows Roll Down Front
//    <o1.11> Windows Roll Down Rear
//    <o1.12> Gas Cap
//    <o1.13> Light
//    <o1.14> Light Disable
//    <o1.15> Domelight
//</h>

//<o2> OEM Trunk Process Timeout[ms]                  <0-60000>

//</h>

#define FUNC_OEM_TRUNK_BLOCKED_BY         0x00030080
#define FUNC_OEM_TRUNK_SUSPENDED_BY       0x00000000
#define FUNC_OEM_TRUNK_TIMEOUT            2000

/*--------------------------------------------------------------------------*/
//<h>[9 ].Windows Roll Up Configuration
/*--------------------------------------------------------------------------*/

//<h> Windows Roll Up blocked by
//    <o0.0 > Arm
//    <o0.1 > Disarm
//    <o0.2 > Lock
//    <o0.3 > Unlock
//    <o0.4 > Trunk Release
//    <o0.5 > Relock Trunk Release
//    <o0.6 > Oem Lock 
//    <o0.7 > Oem Unlock
//    <o0.8 > Oem Trunk
//    <o0.9 > Windows Roll Up
//    <o0.10> Windows Roll Down Front
//    <o0.11> Windows Roll Down Rear
//    <o0.12> Gas Cap
//    <o0.13> Light
//    <o0.14> Light Disable
//    <o0.15> Domelight
//</h>

//<h> Windows Roll Up suspended by
//    <o1.0 > Arm
//    <o1.1 > Disarm
//    <o1.2 > Lock
//    <o1.3 > Unlock
//    <o1.4 > Trunk Release
//    <o1.5 > Relock Trunk Release
//    <o1.6 > Oem Lock 
//    <o1.7 > Oem Unlock
//    <o1.8 > Oem Trunk
//    <o1.9 > Windows Roll Up
//    <o1.10> Windows Roll Down Front
//    <o1.11> Windows Roll Down Rear
//    <o1.12> Gas Cap
//    <o1.13> Light
//    <o1.14> Light Disable
//    <o1.15> Domelight
//</h>

//<o2> Windows Roll Up Process Timeout[ms]                  <0-60000>

//</h>

#define FUNC_WRU_BLOCKED_BY         0x00031D9A
#define FUNC_WRU_SUSPENDED_BY       0x00000045
#define FUNC_WRU_TIMEOUT            15000


/*--------------------------------------------------------------------------*/
//<h>[10].Windows Rolld Down Front Configuration
/*--------------------------------------------------------------------------*/

//<h> Windows Roll Down Front blocked by
//    <o0.0 > Arm
//    <o0.1 > Disarm
//    <o0.2 > Lock
//    <o0.3 > Unlock
//    <o0.4 > Trunk Release
//    <o0.5 > Relock Trunk Release
//    <o0.6 > Oem Lock 
//    <o0.7 > Oem Unlock
//    <o0.8 > Oem Trunk
//    <o0.9 > Windows Roll Up
//    <o0.10> Windows Roll Down Front
//    <o0.11> Windows Roll Down Rear
//    <o0.12> Gas Cap
//    <o0.13> Light
//    <o0.14> Light Disable
//    <o0.15> Domelight
//</h>

//<h> Windows Roll Down Front suspended by
//    <o1.0 > Arm
//    <o1.1 > Disarm
//    <o1.2 > Lock
//    <o1.3 > Unlock
//    <o1.4 > Trunk Release
//    <o1.5 > Relock Trunk Release
//    <o1.6 > Oem Lock 
//    <o1.7 > Oem Unlock
//    <o1.8 > Oem Trunk
//    <o1.9 > Windows Roll Up
//    <o1.10> Windows Roll Down Front
//    <o1.11> Windows Roll Down Rear
//    <o1.12> Gas Cap
//    <o1.13> Light
//    <o1.14> Light Disable
//    <o1.15> Domelight
//</h>

//<o2> Windows Roll Down Front Process Timeout[ms]                  <0-60000>

//</h>


#define FUNC_WRD_FRONT_BLOCKED_BY         0x000309DF
#define FUNC_WRD_FRONT_SUSPENDED_BY       0x00000000
#define FUNC_WRD_FRONT_TIMEOUT            2000

/*--------------------------------------------------------------------------*/
//<h>[11].Windows Roll Down Rear Configuration
/*--------------------------------------------------------------------------*/

//<h> Windows Roll Down Rear blocked by
//    <o0.0 > Arm
//    <o0.1 > Disarm
//    <o0.2 > Lock
//    <o0.3 > Unlock
//    <o0.4 > Trunk Release
//    <o0.5 > Relock Trunk Release
//    <o0.6 > Oem Lock 
//    <o0.7 > Oem Unlock
//    <o0.8 > Oem Trunk
//    <o0.9 > Windows Roll Up
//    <o0.10> Windows Roll Down Front
//    <o0.11> Windows Roll Down Rear
//    <o0.12> Gas Cap
//    <o0.13> Light
//    <o0.14> Light Disable
//    <o0.15> Domelight
//</h>

//<h> Windows Roll Down Rear suspended by
//    <o1.0 > Arm
//    <o1.1 > Disarm
//    <o1.2 > Lock
//    <o1.3 > Unlock
//    <o1.4 > Trunk Release
//    <o1.5 > Relock Trunk Release
//    <o1.6 > Oem Lock 
//    <o1.7 > Oem Unlock
//    <o1.8 > Oem Trunk
//    <o1.9 > Windows Roll Up
//    <o1.10> Windows Roll Down Front
//    <o1.11> Windows Roll Down Rear
//    <o1.12> Gas Cap
//    <o1.13> Light
//    <o1.14> Light Disable
//    <o1.15> Domelight
//</h>

//<o2> Windows Roll Down Rear Process Timeout[ms]                  <0-60000>

//</h>


#define FUNC_WRD_REAR_BLOCKED_BY         0x000315DF
#define FUNC_WRD_REAR_SUSPENDED_BY       0x00000000
#define FUNC_WRD_REAR_TIMEOUT            2000

/*--------------------------------------------------------------------------*/
//<h>[12].Gas Cap Configuration
/*--------------------------------------------------------------------------*/

//<h> Gas Cap blocked by
//    <o0.0 > Arm
//    <o0.1 > Disarm
//    <o0.2 > Lock
//    <o0.3 > Unlock
//    <o0.4 > Trunk Release
//    <o0.5 > Relock Trunk Release
//    <o0.6 > Oem Lock 
//    <o0.7 > Oem Unlock
//    <o0.8 > Oem Trunk
//    <o0.9 > Windows Roll Up
//    <o0.10> Windows Roll Down Front
//    <o0.11> Windows Roll Down Rear
//    <o0.12> Gas Cap
//    <o0.13> Light
//    <o0.14> Light Disable
//    <o0.15> Domelight
//</h>

//<h> Gas Cap suspended by
//    <o1.0 > Arm
//    <o1.1 > Disarm
//    <o1.2 > Lock
//    <o1.3 > Unlock
//    <o1.4 > Trunk Release
//    <o1.5 > Relock Trunk Release
//    <o1.6 > Oem Lock 
//    <o1.7 > Oem Unlock
//    <o1.8 > Oem Trunk
//    <o1.9 > Windows Roll Up
//    <o1.10> Windows Roll Down Front
//    <o1.11> Windows Roll Down Rear
//    <o1.12> Gas Cap
//    <o1.13> Light
//    <o1.14> Light Disable
//    <o1.15> Domelight
//</h>

//<o2> Gas Cap Process Timeout[ms]                  <0-60000>

//</h>


#define FUNC_GAS_CAP_BLOCKED_BY         0x000301DD
#define FUNC_GAS_CAP_SUSPENDED_BY       0x00000002
#define FUNC_GAS_CAP_TIMEOUT            1000

/*--------------------------------------------------------------------------*/
//<h>[13].Light Configuration
/*--------------------------------------------------------------------------*/

//<h> Light blocked by
//    <o0.0 > Arm
//    <o0.1 > Disarm
//    <o0.2 > Lock
//    <o0.3 > Unlock
//    <o0.4 > Trunk Release
//    <o0.5 > Relock Trunk Release
//    <o0.6 > Oem Lock 
//    <o0.7 > Oem Unlock
//    <o0.8 > Oem Trunk
//    <o0.9 > Windows Roll Up
//    <o0.10> Windows Roll Down Front
//    <o0.11> Windows Roll Down Rear
//    <o0.12> Gas Cap
//    <o0.13> Light
//    <o0.14> Light Disable
//    <o0.15> Domelight
//</h>

//<h> Light suspended by
//    <o1.0 > Arm
//    <o1.1 > Disarm
//    <o1.2 > Lock
//    <o1.3 > Unlock
//    <o1.4 > Trunk Release
//    <o1.5 > Relock Trunk Release
//    <o1.6 > Oem Lock 
//    <o1.7 > Oem Unlock
//    <o1.8 > Oem Trunk
//    <o1.9 > Windows Roll Up
//    <o1.10> Windows Roll Down Front
//    <o1.11> Windows Roll Down Rear
//    <o1.12> Gas Cap
//    <o1.13> Light
//    <o1.14> Light Disable
//    <o1.15> Domelight
//</h>

//<o2> Light Process Timeout[ms]                  <0-60000>

//</h>


#define FUNC_LIGHT_BLOCKED_BY         0x00030000
#define FUNC_LIGHT_SUSPENDED_BY       0x00000000
#define FUNC_LIGHT_TIMEOUT            3000


/*--------------------------------------------------------------------------*/
//<h>[14].Light Disable Configuration
/*--------------------------------------------------------------------------*/

//<h> Light Disable blocked by
//    <o0.0 > Arm
//    <o0.1 > Disarm
//    <o0.2 > Lock
//    <o0.3 > Unlock
//    <o0.4 > Trunk Release
//    <o0.5 > Relock Trunk Release
//    <o0.6 > Oem Lock 
//    <o0.7 > Oem Unlock
//    <o0.8 > Oem Trunk
//    <o0.9 > Windows Roll Up
//    <o0.10> Windows Roll Down Front
//    <o0.11> Windows Roll Down Rear
//    <o0.12> Gas Cap
//    <o0.13> Light
//    <o0.14> Light Disable
//    <o0.15> Domelight
//</h>

//<h> Light Disable suspended by
//    <o1.0 > Arm
//    <o1.1 > Disarm
//    <o1.2 > Lock
//    <o1.3 > Unlock
//    <o1.4 > Trunk Release
//    <o1.5 > Relock Trunk Release
//    <o1.6 > Oem Lock 
//    <o1.7 > Oem Unlock
//    <o1.8 > Oem Trunk
//    <o1.9 > Windows Roll Up
//    <o1.10> Windows Roll Down Front
//    <o1.11> Windows Roll Down Rear
//    <o1.12> Gas Cap
//    <o1.13> Light
//    <o1.14> Light Disable
//    <o1.15> Domelight
//</h>

//<o2> Light Disable Process Timeout[ms]                  <0-60000>

//</h>


#define FUNC_LIGHT_DISABLE_BLOCKED_BY         0x00030000
#define FUNC_LIGHT_DISABLE_SUSPENDED_BY       0x00000000
#define FUNC_LIGHT_DISABLE_TIMEOUT            6000


/*--------------------------------------------------------------------------*/
//<h>[15].Domelight Configuration
/*--------------------------------------------------------------------------*/

//<h> Domelight blocked by
//    <o0.0 > Arm
//    <o0.1 > Disarm
//    <o0.2 > Lock
//    <o0.3 > Unlock
//    <o0.4 > Trunk Release
//    <o0.5 > Relock Trunk Release
//    <o0.6 > Oem Lock 
//    <o0.7 > Oem Unlock
//    <o0.8 > Oem Trunk
//    <o0.9 > Windows Roll Up
//    <o0.10> Windows Roll Down Front
//    <o0.11> Windows Roll Down Rear
//    <o0.12> Gas Cap
//    <o0.13> Light
//    <o0.14> Light Disable
//    <o0.15> Domelight
//</h>

//<h> Domelight suspended by
//    <o1.0 > Arm
//    <o1.1 > Disarm
//    <o1.2 > Lock
//    <o1.3 > Unlock
//    <o1.4 > Trunk Release
//    <o1.5 > Relock Trunk Release
//    <o1.6 > Oem Lock 
//    <o1.7 > Oem Unlock
//    <o1.8 > Oem Trunk
//    <o1.9 > Windows Roll Up
//    <o1.10> Windows Roll Down Front
//    <o1.11> Windows Roll Down Rear
//    <o1.12> Gas Cap
//    <o1.13> Light
//    <o1.14> Light Disable
//    <o1.15> Domelight
//</h>

//<o2> Domelight Process Timeout[ms]                  <0-60000>

//</h>

#define FUNC_DOMELIGHT_BLOCKED_BY         0x00030FC0
#define FUNC_DOMELIGHT_SUSPENDED_BY       0x00000000
#define FUNC_DOMELIGHT_TIMEOUT            30000


#endif

