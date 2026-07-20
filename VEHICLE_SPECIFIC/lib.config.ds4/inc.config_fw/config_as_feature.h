/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: config_as_feature.h 33350 2015-08-25 11:17:08Z martin.bouchard $
/*==========================================================================*/

//             <<< Use Configuration Wizard in Context Menu >>>             //

#ifndef __CONFIG_AS_FEATURE_H__
#define __CONFIG_AS_FEATURE_H__

#include "device.h"

#define AS_FEATURE_VERSION 5

//--------------------------------------------------------------------------//
// <h> Convenience Features
//--------------------------------------------------------------------------//

//<e0>Ignition-Controlled Door Locks Enable
//<o1>Ignition Controlled Door Locks
//		<1=> *Ignition LOCK Disabled              
//		<2=> Ignition LOCK Enabled
//		<3=> Ignition UNLOCK ONLY
//		<4=> Ignition LOCK ONLY
//</e>
#define FEAT_AS00_ENABLE	0x01
#define FEAT_AS00  	        0x01


//<e0>Secure Lock Enabled
//<o1>Secure Lock
//      <1=> *Secure Lock DISABLED           
//      <2=> Standard Secure Lock ENABLED
//      <3=> Smart Secure Lock ENABLED          
//</e>
#define FEAT_AS01_ENABLE	0x01
#define FEAT_AS01 			0x01

//<e0>Door Locks Pulse Timing Enabled
//<o1>Door Locks Pulse Timing
//      <1=>*7/10 Second Lock/Unlock pulses
//      <2=>4 Seconds Lock/Unlock pulses
//      <3=>7/10 Seconds Lock pulse and two 1/4 second unlock pulses
//      <4=>1/10 Second Lock/Unlock pulses
//</e>
#define FEAT_AS02_ENABLE	0x01
#define FEAT_AS02 			0x01

//<e0>Priority Door Enable
//<o1>Priority Door 
//      <1=>*Disabled
//      <2=>Enabled
//</e>
#define FEAT_AS03_ENABLE	0x01
#define FEAT_AS03 			0x01  


//<e0>Trunk Programming Enable
//<o1>Trunk Programming 
//      <1=>*Constant Output
//      <2=>1-sec Output Pulse   
//</e>
#define FEAT_AS04_ENABLE	0x01
#define FEAT_AS04 			0x01

//</h>


//--------------------------------------------------------------------------//
// <h> Engine Control Features
//--------------------------------------------------------------------------//

//<e0>Engine Check Mode Enable
//<o1>Engine Check Mode
//      <1=>Tachmeter
//      <2=>*Virtual Tach
//      <3=> Hybrid
//</e>
#define FEAT_AS05_ENABLE	0x01
#define FEAT_AS05 			0x02

//<e0>Engine Cranking Time
//<o1>Engine Cranking Time
//      <1=>1.0 sec
//      <2=>*0.6 sec
//      <3=> 2.0 sec
//      <4=> 4.0 sec
//</e>
#define FEAT_AS06_ENABLE	0x01
#define FEAT_AS06 			0x02

//<e0>Diesel Engine Start Delay Enable
//<o1>Diesel Engine Start Delay <0-30:1>
//</e>
#define FEAT_AS07_ENABLE	0x01
#define FEAT_AS07 			0x0

//<e0>Engine Runtime Enabled
//<o1>Engine Runtime <1-30:1>
//</e>
#define FEAT_AS08_ENABLE	0x01
#define FEAT_AS08 			0x0F

//<e0>Turbo Mode Enable
//<o1>Turbo Mode
//      <1=> Disabled
//      <2=>*Enabled 1 minute, Automatic Transmission
//      <3=> Enabled 1 minute, Manual Transmission
//</e>
#define FEAT_AS09_ENABLE	0x01
#define FEAT_AS09 			0x02


//<e0>Cold Weather Runtime Enable
//<o1>Cold Weather Runtime <1-25:1>
//</e>
#define FEAT_AS10_ENABLE	0x01
#define FEAT_AS10 			0x03  

//<e0>Rear Defogger Output Type Enable                    
//<o1>Rear Defogger Output Type                     
//         <1=> *Latched( 10 minutes )
//         <2=> Pulse Only( 1 second )
//</e>
#define FEAT_AS11_ENABLE		0x01
#define FEAT_AS11				0x01


//<e0>Preprogrammed Tach Enable
//<o1>Preprogrammed Tach(x10)    <10-255:1>
//</e>
#define FEAT_AS12_ENABLE	     0x00
#define FEAT_AS12 			     0x50  

//<e0>Preprogrammed VTach Ref Enable
//<o1>Preprogrammed VTach Ref
//      <1=>  Level 0
//      <2=> *Level 1
//      <3=>  Level 2
//</e>
#define FEAT_AS13_ENABLE	     0x00
#define FEAT_AS13 			     0x02  

//<e0>VTach Ref Fine Adjust Enable
//<o1>VTach Ref Fine Adjust     <0-20:1>
//</e>
#define FEAT_AS14_ENABLE	     0x00
#define FEAT_AS14 			     0x0A

//</h>


//--------------------------------------------------------------------------//
// <h> Advanced Features
//--------------------------------------------------------------------------//
//<e0>Relock Enable
//<o1>Relock
//		<1=> Enabled       
//		<2=>*Disabled
//</e>
#define FEAT_AS15_ENABLE  0x01
#define FEAT_AS15  	  0x02

//<e0>External Trigger Enable
//<o1>External Trigger
//		<1=> Enable(single pulse)
//		<2=>*Disable
//</e>
#define FEAT_AS16_ENABLE	0x01
#define FEAT_AS16   	    0x02

//<e0>Ready Mode Completion Enable
//<o1>Ready Mode Completion
//		<1=> Open/Close door
//		<2=>*Enabled by remote
//		<3=> Open/Close door with Lock
//</e>
#define FEAT_AS17_ENABLE	0x01
#define FEAT_AS17   	    0x02

//<e0>Safe Start Enable
//<o1>Safe Start
//		<1=>Enabled
//		<2=>*Disabled
//</e>
#define FEAT_AS18_ENABLE	0x01
#define FEAT_AS18   	    0x02

//<e0>Horn Confirmation Enable
//<o1>Horn Confirmation
//      <1=>  Horn on 1st lock
//      <2=> *Disabled
//		<3=>  Horn on 2nd lock
//</e>
#define FEAT_AS19_ENABLE	0x01
#define FEAT_AS19 			0x02

//<e0>Panic Mode Enable
//<o1>Panic Mode
//      <1=>  Disabled         
//      <2=> *Enabled
//</e>
#define FEAT_AS20_ENABLE	0x01
#define FEAT_AS20 			0x02

//</h>

//--------------------------------------------------------------------------//
// <h> Security Features
//--------------------------------------------------------------------------//

//<e0>Confirmation Chirps Enable
//<o1>Confirmation Chirps
//		<1=>  Warning Chirps enabled
//		<2=> *Chirps enabled
//		<3=>  Enabled with open zone notification
//      <4=>  Disabled
//</e>
#define FEAT_AS21_ENABLE	0x01
#define FEAT_AS21  	    	0x02

//<e0>Siren Duration Enable
//<o1>Siren Duration
//      <1=>  Siren pulse 20 ms         
//      <2=> *Siren pulse 30 ms
//      <3=>  Siren pulse 40 ms
//      <4=>  Siren pulse 50 ms
//</e>
#define FEAT_AS22_ENABLE	0x01
#define FEAT_AS22 			0x02

//<e0>System Arming Mode Enable
//<o1>System Arming Mode
//      <1=>  Active arming with door notification
//      <2=> *Active arming
//		<3=>  Passive Arming-single stage disarm
//		<4=>  Passive Arming-two stages disarm
//</e>
#define FEAT_AS23_ENABLE	0x01
#define FEAT_AS23 			0x02

//<e0>IGN Monitoring Enable
//<o1>IGN Monitoring
//      <1=>  DISABLED
//      <2=> *ENABLED
//</e>
#define FEAT_AS24_ENABLE	0x01
#define FEAT_AS24 			0x02

//<e0>Shock Sense Programming Enable
//<o1>Shock Sense Programming
//      <1=>  Disabled
//      <2=> *Enabled
//      <3=>  Warn away disabled
//      <4=>  Shock Sense disabled
//</e>
#define FEAT_AS25_ENABLE	0x01
#define FEAT_AS25 			0x02  

//</h>

//--------------------------------------------------------------------------//
// <h>  Programmable Aux Options
//--------------------------------------------------------------------------//

//<e0>Aux1 Output Type Enable
//<o1>Aux1 Output Type
//		<1=>  Toggle ON/OFF
//		<2=> *Constant output
//    	<3=>  Toggle ON/OFF with IGN Reset
//</e>
#define FEAT_AS26_ENABLE	0x01
#define FEAT_AS26   	    0x02

//<e0>Aux1 Output Time Enable
//<o1>Aux1 Output Time (Seconds) <1-90:1>
//</e>
#define FEAT_AS27_ENABLE	0x01
#define FEAT_AS27   	    0x1E

//<e0>Aux2 Output Type Enable
//<o1>Aux2 Output Type
//		<1=>  Toggle ON/OFF
//		<2=> *Constant output
//    	<3=>  Toggle ON/OFF with IGN Reset
//</e>
#define FEAT_AS28_ENABLE	0x01
#define FEAT_AS28   	    0x02

//<e0>Aux2 Output Time Enable
//<o1>Aux2 Output Time (Seconds) <1-90:1>
//</e>
#define FEAT_AS29_ENABLE	0x01
#define FEAT_AS29   	    0x1E

//<e0>Aux3 Output Type Enable
//<o1>Aux3 Output Type
//		<1=>  Toggle ON/OFF
//		<2=> *Constant output
//    	<3=>  Toggle ON/OFF with IGN Reset
//</e> 
#define FEAT_AS30_ENABLE	0x01
#define FEAT_AS30   	    0x02

//<e0>Aux3 Output Time Enable
//<o1>Aux3 Output Time (Seconds) <1-90:1>
//</e>
#define FEAT_AS31_ENABLE	0x01
#define FEAT_AS31   	    0x1E

//<e0>Aux4 Output Type Enable
//<o1>Aux4 Output Type
//		<1=>  Toggle ON/OFF
//		<2=> *Constant output
//    	<3=>  Toggle ON/OFF with IGN Reset
//</e>
#define FEAT_AS32_ENABLE	0x01
#define FEAT_AS32   	    0x02

//<e0>Aux4 Output Time Enable
//<o1>Aux4 Output Time (Seconds) <1-90:1>
//</e>
#define FEAT_AS33_ENABLE	0x01
#define FEAT_AS33   	    0x1E

//</h>

//--------------------------------------------------------------------------//
// <h>  Programmable Inputs/Outputs
//--------------------------------------------------------------------------//

//<e0>Output 1 option is visible on web
//<o1>Output1 (-)Lt. Green/White H2[10]
//         <1=>  *Factory Alarm Arm
//         <2=>  Ground When Arm
//         <3=>  Factory Alarm Disarm
//         <4=>  Lock
//         <5=>  Ground when locked
//         <6=>  Unlock
//         <7=>  Pulse on 2nd unlock
//         <8=>  Ground when unlocked by User 2
//         <9=>  Trunk release
//         <10=> Aux 1 
//         <11=> Aux 2
//         <12=> Aux 3
//         <13=> Aux 4
//         <14=> Ground while running (Status)
//         <15=> Ground while running (5sec.) 
//         <16=> Pulse before starting
//         <17=> NOT Configured
//         <18=> Pulse after startup
//         <19=> Pulse after ignition Off (1 min duration)
//         <20=> Accessory
//         <21=> Ignition
//         <22=> Starter
//         <23=> Parking Lights
//         <24=> Rear Defroster
//         <25=> Dome light
//         <26=> Siren/Horn
//         <27=> Ground when alarm triggered
//         <29=> Not Configured
//</e>
#define FEAT_AS34_ENABLE   		0x00					
#define FEAT_AS34 				0x1D

//<e0>Output 2 option is visible on web
//<o1>Output2  (-)Blue/White H3[18]
//         <1=>  Factory Alarm Arm
//         <2=>  Ground When Arm
//         <3=>  Factory Alarm Disarm
//         <4=>  Lock
//         <5=>  Ground when locked
//         <6=>  Unlock
//         <7=>  Pulse on 2nd unlock
//         <8=>  Ground when unlocked by User 2
//         <9=>  Trunk release
//         <10=> Aux 1 
//         <11=> Aux 2
//         <12=> Aux 3
//         <13=> Aux 4
//         <14=> *Ground while running (Status)
//         <15=> Ground while running (5sec.)
//         <16=> Pulse before starting
//         <17=> NOT Configured
//         <18=> Pulse after startup
//         <19=> Pulse after ignition Off (1 min duration)
//         <20=> Accessory
//         <21=> Ignition
//         <22=> Starter
//         <23=> Parking Lights
//         <24=> Rear Defroster
//         <25=> Dome light
//         <26=> Siren/Horn
//         <27=> Ground when alarm triggered
//         <29=> Not Configured
//</e>
#define FEAT_AS35_ENABLE   		0x01					
#define FEAT_AS35				0x0E

//<e0>Output 3 option is visible on web
//<o1>Output3 (-)Dk. Green H3[15]
//         <1=>  Factory Alarm Arm
//         <2=>  Ground When Arm
//         <3=>  Factory Alarm Disarm
//         <4=>  *Lock
//         <5=>  Ground when locked
//         <6=>  Unlock
//         <7=>  Pulse on 2nd unlock
//         <8=>  Ground when unlocked by User 2
//         <9=>  Trunk release
//         <10=> Aux 1 
//         <11=> Aux 2
//         <12=> Aux 3
//         <13=> Aux 4
//         <14=> Ground while running (Status)
//         <15=> Ground while running (5sec.)
//         <16=> Pulse before starting
//         <17=> NOT Configured
//         <18=> Pulse after startup
//         <19=> Pulse after ignition Off (1 min duration)
//         <20=> Accessory
//         <21=> Ignition
//         <22=> Starter
//         <23=> Parking Lights
//         <24=> Rear Defroster
//         <25=> Dome light
//         <26=> Siren/Horn
//         <27=> Ground when alarm triggered
//         <29=> Not Configured
//</e>
#define FEAT_AS36_ENABLE   		0x01					
#define FEAT_AS36   			0x04

//<e0>Output 4 option is visible on web
//<o1>Output4 (-)Red/White H3[9]
//         <1=>  Factory Alarm Arm
//         <2=>  Ground When Arm
//         <3=>  Factory Alarm Disarm
//         <4=>  Lock
//         <5=>  Ground when locked
//         <6=>  Unlock
//         <7=>  Pulse on 2nd unlock
//         <8=>  Ground when unlocked by User 2
//         <9=>  *Trunk release
//         <10=> Aux 1 
//         <11=> Aux 2
//         <12=> Aux 3
//         <13=> Aux 4
//         <14=> Ground while running (Status)
//         <15=> Ground while running (5sec.)
//         <16=> Pulse before starting
//         <17=> NOT Configured
//         <18=> Pulse after startup
//         <19=> Pulse after ignition Off (1 min duration)
//         <20=> Accessory
//         <21=> Ignition
//         <22=> Starter
//         <23=> Parking Lights
//         <24=> Rear Defroster
//         <25=> Dome light
//         <26=> Siren/Horn
//         <27=> Ground when alarm triggered
//         <29=> Not Configured
//</e>
#define FEAT_AS37_ENABLE   		0x01					
#define FEAT_AS37 				0x09

//<e0>Output 5 option is visible on web
//<o1>Output5 (-)Dk.Blue H3[7]
//         <1=>  Factory Alarm Arm
//         <2=>  Ground When Arm
//         <3=>  Factory Alarm Disarm
//         <4=>  Lock
//         <5=>  Ground when locked
//         <6=>  *Unlock
//         <7=>  Pulse on 2nd unlock
//         <8=>  Ground when unlocked by User 2
//         <9=>  Trunk release
//         <10=> Aux 1 
//         <11=> Aux 2
//         <12=> Aux 3
//         <13=> Aux 4
//         <14=> Ground while running (Status)
//         <15=> Ground while running (5sec.)
//         <16=> Pulse before starting
//         <17=> NOT Configured
//         <18=> Pulse after startup
//         <19=> Pulse after ignition Off (1 min duration)
//         <20=> Accessory
//         <21=> Ignition
//         <22=> Starter
//         <23=> Parking Lights
//         <24=> Rear Defroster
//         <25=> Dome light
//         <26=> Siren/Horn
//         <27=> Ground when alarm triggered
//         <29=> Not Configured
//</e>
#define FEAT_AS38_ENABLE   		0x01					
#define FEAT_AS38 				0x06

//<e0>Output 6 option is visible on web
//<o1>Output6  (+)Green/Red H2[14]
//         <1=>  Factory Alarm Arm
//         <2=>  Ground When Arm
//         <3=>  Factory Alarm Disarm
//         <4=>  Lock
//         <5=>  Ground when locked
//         <6=>  Unlock
//         <7=>  Pulse on 2nd unlock
//         <8=>  Ground when unlocked by User 2
//         <9=>  Trunk release
//         <10=> Aux 1 
//         <11=> Aux 2
//         <12=> Aux 3
//         <13=> Aux 4
//         <14=> Ground while running (Status)
//         <15=> Ground while running (5sec.)
//         <16=> Pulse before starting
//         <17=> NOT Configured
//         <18=> Pulse after startup
//         <19=> Pulse after ignition Off (1 min duration)
//         <20=> Accessory
//         <21=> Ignition
//         <22=> Starter
//         <23=> Parking Lights
//         <24=> Rear Defroster
//         <25=> Dome light
//         <26=> Siren/Horn
//         <27=> Ground when alarm triggered
//         <29=> *Not Configured
//</e>
#define FEAT_AS39_ENABLE   		0x01					
#define FEAT_AS39 				0x0D

//<e0>Output 7 option is visible on web
//<o1>Output7  (+/-)White/Red H2[12](+),White/Violet H2[11](-)
//         <1=>  Factory Alarm Arm
//         <2=>  Ground When Arm
//         <3=>  Factory Alarm Disarm
//         <4=>  Lock
//         <5=>  Ground when locked
//         <6=>  Unlock
//         <7=>  Pulse on 2nd unlock
//         <8=>  Ground when unlocked by User 2
//         <9=>  Trunk release
//         <10=> *Aux 1
//         <11=> Aux 2
//         <12=> Aux 3
//         <13=> Aux 4
//         <14=> Ground while running (Status)
//         <15=> Ground while running (5sec.)
//         <16=> Pulse before starting
//         <17=> NOT Configured
//         <18=> Pulse after startup
//         <19=> Pulse after ignition Off (1 min duration)
//         <20=> Accessory
//         <21=> Ignition
//         <22=> Starter
//         <23=> Parking Lights
//         <24=> Rear Defroster
//         <25=> Dome light
//         <26=> Siren/Horn
//         <27=> Ground when alarm triggered
//         <29=> Not Configured
//</e>
#define FEAT_AS40_ENABLE   		0x00
#define FEAT_AS40 				0x16

//<e0>Output 8 option is visible on web
//<o1>Output8  (+)Pink/White H4[3] (also input to yellow in ribbon cable)
//         <20=> Accessory
//         <21=> *Ignition
//         <22=> Starter
//         <29=> Not Configured
//</e>
#define FEAT_AS41_ENABLE   		0x01					
#define FEAT_AS41 				0x15

//<e0>Output 9 option is visible on web
//<o1>Output9  (+)Orange H4[5]
//         <20=> *Accessory
//         <29=> Not Configured
//</e>
#define FEAT_AS42_ENABLE   		0x00					
#define FEAT_AS42 				0x14

//<e0>Output 10 option is visible on web
//<o1>Output10  (+)Pink H4[1] 
//         <21=> *Ignition
//         <29=> Not Configured
//</e>
#define FEAT_AS43_ENABLE   		0x00					
#define FEAT_AS43 				0x15

//<e0>Output 11 option is visible on web
//<o1>Output11  (+)Violet H4[8]
//         <22=> *Starter
//         <29=> Not Configured
//</e>
#define FEAT_AS44_ENABLE   		0x00					
#define FEAT_AS44				0x16

//<e0>Output 12 option is visible on web
//<o1>Output12  (+)Green H4[7]
//         <1=>  Factory Alarm Arm
//         <2=>  Ground When Arm
//         <3=>  Factory Alarm Disarm
//         <4=>  Lock
//         <5=>  Ground when locked
//         <6=>  Unlock
//         <7=>  Pulse on 2nd unlock
//         <8=>  Ground when unlocked by User 2
//         <9=>  Trunk release
//         <10=> Aux 1
//         <11=> Aux 2
//         <12=> Aux 3
//         <13=> Aux 4
//         <14=> Ground while running (Status)
//         <15=> Ground while running (5sec.)
//         <16=> Pulse before starting
//         <17=> NOT Configured
//         <18=> Pulse after startup
//         <19=> Pulse after ignition Off (1 min duration)
//         <20=> Accessory
//         <21=> Ignition
//         <22=> Starter
//         <23=> Parking Lights
//         <24=> Rear Defroster
//         <25=> Dome light
//         <26=> Siren/Horn
//         <27=> Ground when alarm triggered
//         <28=> *Anti-Grind 
//         <29=> Not Configured
//</e>
#define FEAT_AS45_ENABLE   		0x01					
#define FEAT_AS45 					0x1C

//<e0>Output 13 option is visible on web
//<o1>Output13  (-)Lt.Green/Black H2[13]
//         <1=>  Factory Alarm Arm
//         <2=>  Ground When Arm
//         <3=>  *Factory Alarm Disarm
//         <4=>  Lock
//         <5=>  Ground when locked
//         <6=>  Unlock
//         <7=>  Pulse on 2nd unlock
//         <8=>  Ground when unlocked by User 2
//         <9=>  Trunk release
//         <10=> Aux 1
//         <11=> Aux 2
//         <12=> Aux 3
//         <13=> Aux 4
//         <14=> Ground while running (Status)
//         <15=> Ground while running (5sec.)
//         <16=> Pulse before starting
//         <17=> NOT Configured
//         <18=> Pulse after startup
//         <19=> Pulse after ignition Off (1 min duration)
//         <20=> Accessory
//         <21=> Ignition
//         <22=> Starter
//         <23=> Parking Lights
//         <24=> Rear Defroster
//         <25=> Dome light
//         <26=> Siren/Horn
//         <27=> Ground when alarm triggered
//         <29=> Not Configured
//</e>
#define FEAT_AS46_ENABLE   		0x00					
#define FEAT_AS46 					0x1D

//<e0>Output 14 option is visible on web
//<o1>Output14  (-)Brown/Black H3[8]
//         <1=>  Factory Alarm Arm
//         <2=>  Ground When Arm
//         <3=>  Factory Alarm Disarm
//         <4=>  Lock
//         <5=>  Ground when locked
//         <6=>  Unlock
//         <7=>  Pulse on 2nd unlock
//         <8=>  Ground when unlocked by User 2
//         <9=>  Trunk release
//         <10=> Aux 1
//         <11=> Aux 2
//         <12=> Aux 3
//         <13=> Aux 4
//         <14=> Ground while running (Status)
//         <15=> Ground while running (5sec.)
//         <16=> Pulse before starting
//         <17=> NOT Configured
//         <18=> Pulse after startup
//         <19=> Pulse after ignition Off (1 min duration)
//         <20=> Accessory
//         <21=> Ignition
//         <22=> Starter
//         <23=> Parking Lights
//         <24=> Rear Defroster
//         <25=> Dome light
//         <26=> *Siren/Horn
//         <27=> Ground when alarm triggered
//         <29=> Not Configured
//</e>
#define FEAT_AS47_ENABLE   	0x01					
#define FEAT_AS47			0x1A


//<e0>Relay 1 option is visible on web
//<o1>Relay1  COM-Yellow:H1[6] , NC-Orange/Yellow:H1[11] , NO-Brown:H1[12]
//         <1=>  Factory Alarm Arm
//         <2=>  Ground When Arm
//         <3=>  Factory Alarm Disarm
//         <4=>  Lock
//         <5=>  Ground when locked
//         <6=>  Unlock
//         <7=>  Pulse on 2nd unlock
//         <8=>  Ground when unlocked by User 2
//         <9=>  Trunk release
//         <10=> Aux 1
//         <11=> Aux 2
//         <12=> Aux 3
//         <13=> Aux 4
//         <14=> Ground while running (Status)
//         <15=> Ground while running (5sec.)
//         <16=> Pulse before starting
//         <17=> NOT Configured
//         <18=> Pulse after startup
//         <19=> Pulse after ignition Off (1 min duration)
//         <20=> Accessory
//         <21=> Ignition
//         <22=> Starter
//         <23=> Parking Lights
//         <24=> Rear Defroster
//         <25=> Dome light
//         <26=> Siren/Horn
//         <27=> Ground when alarm triggered
//         <29=> *Not Configured
//</e>
#define FEAT_AS48_ENABLE   		0x01					
#define FEAT_AS48				0x17


//<e0>Relay 2 option is visible on web
//<o1>Relay2  COM-Yellow/Red:H1[4] , NC-Orange/Red:H1[5] , NO-Brown/Red:H1[3]
//         <1=>  Factory Alarm Arm
//         <2=>  Ground When Arm
//         <3=>  Factory Alarm Disarm
//         <4=>  Lock
//         <5=>  Ground when locked
//         <6=>  Unlock
//         <7=>  Pulse on 2nd unlock
//         <8=>  Ground when unlocked by User 2
//         <9=>  Trunk release
//         <10=> Aux 1
//         <11=> Aux 2
//         <12=> Aux 3
//         <13=> Aux 4
//         <14=> Ground while running (Status)
//         <15=> Ground while running (5sec.)
//         <16=> Pulse before starting
//         <17=> NOT Configured
//         <18=> Pulse after startup
//         <19=> Pulse after ignition Off (1 min duration)
//         <20=> Accessory
//         <21=> Ignition
//         <22=> Starter
//         <23=> Parking Lights
//         <24=> Rear Defroster
//         <25=> Dome light
//         <26=> Siren/Horn
//         <27=> Ground when alarm triggered
//         <29=> *Not Configured
//</e>
#define FEAT_AS49_ENABLE   		0x00					
#define FEAT_AS49				0x1D

//<e0>Relay 3 option is visible on web
//<o1>Relay3  COM-White:H1[1], H1[7] , NO-White/Brown:H1[2],H1[8]
//         <1=>  Factory Alarm Arm
//         <2=>  Ground When Arm
//         <3=>  Factory Alarm Disarm
//         <4=>  Lock
//         <5=>  Ground when locked
//         <6=>  Unlock
//         <7=>  Pulse on 2nd unlock
//         <8=>  Ground when unlocked by User 2
//         <9=>  Trunk release
//         <10=> Aux 1
//         <11=> Aux 2
//         <12=> Aux 3
//         <13=> Aux 4
//         <14=> Ground while running (Status)
//         <15=> Ground while running (5sec.)
//         <16=> Pulse before starting
//         <17=> NOT Configured
//         <18=> Pulse after startup
//         <19=> Pulse after ignition Off (1 min duration)
//         <20=> Accessory
//         <21=> Ignition
//         <22=> Starter
//         <23=> *Parking Lights
//         <24=> Rear Defroster
//         <25=> Dome light
//         <26=> Siren/Horn
//         <27=> Ground when alarm triggered
//         <29=> Not Configured
//</e>
#define FEAT_AS50_ENABLE   		0x01					
#define FEAT_AS50 					0x17



//<e0>Input Negative 1 option is visible on web
//<o1>Input Negative 1 Start Trigger (Blue/Black H3[14]) 
//         <9=>  *Start trigger 
//</e>
#define FEAT_AS51_ENABLE   		0x01					
#define FEAT_AS51				0x09

//<e0>Input Negative 2 option is visible on web
//<o1>Input Negative 2(Lt.Blue/Red  H3[1]) 
//         <1=>  Handbrake
//         <2=>  Safety Switch
//         <3=>  Hood
//         <4=>  Trunk
//         <5=>  Door
//         <9=>  Start trigger 
//         <10=> Custom Neg1
//         <11=> *Custom Neg2 
//         <12=> Custom Neg3 
//</e>
#define FEAT_AS52_ENABLE   		0x01					
#define FEAT_AS52				0x0B


//<e0>Input Negative 3 option is visible on web
//<o1>Input Negative 3(Blue/Red H3[12]) 
//         <1=>  Handbrake
//         <2=>  Safety Switch
//         <3=>  Hood
//         <4=>  Trunk
//         <5=>  Door
//         <9=>  Start trigger 
//         <10=> Custom Neg1
//         <11=> Custom Neg2 
//         <12=> *Custom Neg3 
//</e>
#define FEAT_AS53_ENABLE   		0x01					
#define FEAT_AS53				0x0C


//<e0>Input Negative 4 option is visible on web
//<o1>Input Negative 4(White/Black H3[2]) 
//         <1=>  *Handbrake
//         <2=>  Safety Switch
//         <3=>  Hood
//         <4=>  Trunk
//         <5=>  Door
//         <9=>  Start trigger 
//         <10=> Custom Neg1
//         <11=> Custom Neg2 
//         <12=> Custom Neg3 
//</e>
#define FEAT_AS54_ENABLE   		0x01					
#define FEAT_AS54				0x01


//<e0>Input Negative 5 option is visible on web
//<o1>Input Negative 5(Gray  H3[3]) 
//         <1=>  Handbrake
//         <2=>  Safety Switch
//         <3=>  *Hood
//         <4=>  Trunk
//         <5=>  Door
//         <9=>  Start trigger 
//         <10=> Custom Neg1
//         <11=> Custom Neg2 
//         <12=> Custom Neg3 
//</e>
#define FEAT_AS55_ENABLE   		0x01					
#define FEAT_AS55 					0x03

//<e0>Input Negative 6 option is visible on web
//<o1>Input Negative 6(Black/White H3[4]) 
//         <1=>  Handbrake
//         <2=>  *Safety Switch
//         <3=>  Hood
//         <4=>  Trunk
//         <5=>  Door
//         <9=>  Start trigger 
//         <10=> Custom Neg1
//         <11=> Custom Neg2 
//         <12=> Custom Neg3 
//</e>
#define FEAT_AS56_ENABLE   		0x01					
#define FEAT_AS56 				0x02

//<e0>Input Negative 7 option is visible on web
//<o1>Input Negative 7(Lt.Blue H3[13]) 
//         <1=>  Handbrake
//         <2=>  Safety Switch
//         <3=>  Hood
//         <4=>  *Trunk
//         <5=>  Door
//         <9=>  Start trigger 
//         <10=> Custom Neg1
//         <11=> Custom Neg2 
//         <12=> Custom Neg3 
//</e>
#define FEAT_AS57_ENABLE   		0x01					
#define FEAT_AS57 				0x04


//<e0>Input Positive 1 option is visible on web
//<o1>Input Positive 1(Gray/Black H3[5]) 
//         <5=>  Door
//         <6=>  Brake
//         <7=>  *Glowplug
//         <8=>  Ignition
//         <13=> Custom Pos1
//         <14=> Custom Pos2
//</e>
#define FEAT_AS58_ENABLE   		0x01					
#define FEAT_AS58				0x07

//<e0>Input Positive 2 option is visible on web
//<o1>Input Positive 2 (Lt.Brown H3[16]) 
//         <5=>  Door
//         <6=>  *Brake
//         <7=>  Glowplug
//         <8=>  Ignition
//         <13=> Custom Pos1
//         <14=> Custom Pos2
//</e>
#define FEAT_AS59_ENABLE   		0x01					
#define FEAT_AS59				0x06

//<e0>Input Positive 3 option is visible on web
//<o1>Input Positive 3 Yellow (ribbon cable)
//         <5=>  Door
//         <6=>  Brake
//         <7=>  Glowplug
//         <8=>  *Ignition
//         <13=> Custom Pos1
//         <14=> Custom Pos2
//</e>
#define FEAT_AS60_ENABLE   		0x01					
#define FEAT_AS60				0x08


//<e0>Input Negative/Positive 1 option is visible on web
//<o1>Input Negative/Positive 1 (Green/Gray H3[10](-) , Purple/Grey  H3[11](+)) 
//         <1=>  Handbrake
//         <2=>  Safety Switch
//         <3=>  Hood
//         <4=>  Trunk
//         <5=>  *Door
//         <6=>  Brake
//         <7=>  Glowplug
//         <8=>  Ignition
//         <9=>  Start trigger 
//         <10=> Custom Neg1
//         <11=> Custom Neg2 
//         <12=> Custom Neg3 
//         <13=> Custom Pos1
//         <14=> Custom Pos2
//</e>
#define FEAT_AS61_ENABLE   		0x01					
#define FEAT_AS61				0x05

//</h>

//--------------------------------------------------------------------------//
// <h>  Sense Config
//--------------------------------------------------------------------------//
//<e0>Sense doors status Enable
//<o1>Sense doors status
//         <1=> *Auto
//         <2=> Wire
//         <3=> Data
//</e>
#define FEAT_AS62_ENABLE   		0x01					
#define FEAT_AS62				0x01

//<e0>Sense ignition status Enable
//<o1>Sense ignition status
//         <1=> *Auto
//         <2=> Wire
//         <3=> Data
//</e>
#define FEAT_AS63_ENABLE   		0x01					
#define FEAT_AS63				0x01

//<e0>Sense hood status Enable
//<o1>Sense hood status
//         <1=> *Auto
//         <2=> Wire
//         <3=> Data
//</e>
#define FEAT_AS64_ENABLE   		0x01					
#define FEAT_AS64 				0x01

//<e0> Sense trunk status Enable
// <o1>Sense trunk status
//         <1=> *Auto
//         <2=> Wire
//         <3=> Data
//</e>
#define FEAT_AS65_ENABLE   		0x01					
#define FEAT_AS65				0x01

//<e0> Sense Handbrake Enable
//<o1>Sense handbrake status
//         <1=> *Auto
//         <2=> Wire
//         <3=> Data
//</e>
#define FEAT_AS66_ENABLE   		0x01					
#define FEAT_AS66				0x01


//<e0>Sense Tacho Enable
//<o1>Sense tacho status
//         <1=> *Auto
//         <2=> Wire
//         <3=> Data
//</e>
#define FEAT_AS67_ENABLE   		0x01					
#define FEAT_AS67				0x01


//<e0>Sense brake Enable
//<o1>Sense brake status
//         <1=> *Auto
//         <2=> Wire
//         <3=> Data
//</e>
#define FEAT_AS68_ENABLE   		0x01					
#define FEAT_AS68				0x01

//<e0> Sense glowplug Enable
//<o1>Sense glowplug status
//         <1=> *Auto
//         <2=> Wire
//         <3=> Data
//</e>
#define FEAT_AS69_ENABLE   		0x01					
#define FEAT_AS69 				0x01


//<e0>Sense OEM LOCK Enable
//<o1>Sense oem lock
//         <1=> Disable             
//         <2=> *Enable
//</e>
#define FEAT_AS70_ENABLE   		0x01					
#define FEAT_AS70				0x02


//<e0>Sense OEM UNLOCK Enable
//<o1>Sense oem unlock
//         <1=> Disable
//         <2=> *Enable
//</e>
#define FEAT_AS71_ENABLE   		0x01					
#define FEAT_AS71				0x02


//<e0>Sense OEM TRUNK Enable
//<o1>Sense oem trunk
//         <1=> Disable            
//         <2=> *Enable
//</e>
#define FEAT_AS72_ENABLE   		0x01					
#define FEAT_AS72				0x02

//</h>


#endif

