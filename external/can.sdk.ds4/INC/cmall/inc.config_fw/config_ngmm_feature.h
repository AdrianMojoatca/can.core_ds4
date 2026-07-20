/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: config_ngmm_feature.h  $
/*==========================================================================*/

//             <<< Use Configuration Wizard in Context Menu >>>             //

#ifndef __CONFIG_NGMM_FEATURE_H__
#define __CONFIG_NGMM_FEATURE_H__

#include "device.h"
#include "ngmm_feature.h"

#define NGMM_FEATURE_VERSION  23     //new feature list starts version 10


//--------------------------------------------------------------------------//

//<h>  3 Lock Start Features

//<e0>OEM Remote Start Activation Option Enable
//<o1>OEM Remote Start Activation Option
//         <1=> *Disable
//         <2=>  Enable with 3x Locks
//         <4=>  Enable with Lock, Unlock, then Lock
//</e>
#define FEAT_DEI_RS22_ENABLE    0x00
#define FEAT_DEI_RS22           0x01

// </h>

//<h>  Security Features

//<e0>System Arming Mode Enable
//<o1>System Arming Mode
//      <1=> *Active              
//      <2=> Passive Arm - No Lock
//      <3=> Passive Arm & Lock
//</e>
#define FEAT_DEI_SS01_ENABLE    0x01
#define FEAT_DEI_SS01           0x01


//<e0>Panic Mode Visible
//<o1>Panic Mode
//      <1=> *On           
//      <2=> Ignition Off only 
//      <3=> Off          
//</e>
#define FEAT_DEI_SS02_ENABLE    0x01
#define FEAT_DEI_SS02           0x01


//<e0>Confirmation Chirps Enable
//<o1>Confirmation Chirps
//      <1=>*On - Warn Chirps On
//      <2=>On  - Warn Chirps Off
//      <3=>Off - Warn Chirps On
//      <4=>Off - Warn Chirps Off
//</e>
#define FEAT_DEI_SS03_ENABLE    0x01
#define FEAT_DEI_SS03           0x01


//<e0>Horn Function Enable
//<o1>Horn function 
//      <1=>Trigger & Panic Only
//      <2=>OEM Functions   
//		<3=>*Normal
//</e>
#define FEAT_DEI_SS04_ENABLE    0x01
#define FEAT_DEI_SS04           0x03


//<e0>Override Pulse Count Enable
//<o1>Override Pulse Count
//      <1=>*1
//      <2=>2
//      <3=>3
//      <4=>4
//      <5=>5
//</e>
#define FEAT_DEI_SS05_ENABLE    0x01
#define FEAT_DEI_SS05           0x05

//<e0>Trigger/Panic Duration Enable
//<o1>Trigger/Panic Duration( seconds ) <1-180:1>
//</e>
#define FEAT_DEI_SS07_ENABLE    0x01
#define FEAT_DEI_SS07           0x1E


//<e0>NPC - Nuisance Prevention Circuitry Enable     
//<o1>NPC - Nuisance Prevention Circuitry     
//     <1=>*On  
//     <2=>Off 
//</e>
#define FEAT_DEI_SS09_ENABLE    0x01
#define FEAT_DEI_SS09           0x01


//<e0>Door Trigger Error Enable
//<o1>Door Trigger Error Chirp 
//         <1=>Off 
//         <2=>*On
//</e>
#define FEAT_DEI_SS10_ENABLE    0x01
#define FEAT_DEI_SS10           0x02


//<e0>Starter Kill Enable
//<o1>Starter Kill Option 
//         <1=>*Off 
//         <2=>Active
//         <3=>Passive
//</e>
#define FEAT_DEI_SS11_ENABLE    0x01
#define FEAT_DEI_SS11           0x01

//<e0>Horn Output Timing Enable
//<o1>Horn Output Timing Option
//         <1=>10 ms
//         <2=>20 ms
//         <3=>*30 ms
//         <4=>40 ms
//         <5=>50 ms
//         <6=>60 ms
//</e>
#define FEAT_DEI_SS12_ENABLE    0x01
#define FEAT_DEI_SS12           0x03

//<e0>Unlock & Disarm with Trunk Release Enable
//<o1>Unlock & Disarm with Trunk Release
//         <1=>Off
//         <2=>*Smart Unlock
//         <3=>On
//</e>
#define FEAT_DEI_SS13_ENABLE    0x01
#define FEAT_DEI_SS13           0x01

//<e0>Auto Re-Arm Mode Enable
//<o1>Auto Re-Arm Mode
//      <1=> *OFF              
//      <2=> Auto re-arm - No Lock
//      <3=> Auto re-arm & Lock
//</e>
#define FEAT_DEI_SS15_ENABLE    0x01
#define FEAT_DEI_SS15           0x01


//</h>


//--------------------------------------------------------------------------//
// <h> Remote Start Features
//--------------------------------------------------------------------------//

//<e0>Engine Check Mode Enable
//<o1> Engine Checking Mode                
//         <1=> Virtual Tach  
//         <2=> Voltage       
//         <3=> Off   
//         <4=> *Tachometer   
//</e>
#define FEAT_DEI_RS02_ENABLE    0x01
#define FEAT_DEI_RS02           0x04

//<e0>Cranking Time Enable
//<o1>Cranking Time( seconds )                       
//         <1=> *0.6 sec       
//         <2=> 0.8 sec       
//         <3=> 1.0 sec       
//         <4=> 1.2 sec       
//         <5=> 1.4 sec
//         <6=> 1.6 sec
//         <7=> 1.8 sec
//         <8=> 2.0 sec
//         <9=> 4.0 sec   
//</e>
#define FEAT_DEI_RS03_ENABLE    0x01
#define FEAT_DEI_RS03           0x01

//<e0>Activation Pulse Enable
//<o1>Activation Pulse Count              
//         <1=> *1     
//         <2=> 2     
//         <3=> 3 
//         <4=> 0   
//</e>
#define FEAT_DEI_RS04_ENABLE    0x01
#define FEAT_DEI_RS04           0x01

//<e0>Diesel Start Delay Enable
//<o1>Diesel Delay Type/Duration
//    <0=> *Wait Off
//    <1=>  1  sec
//    <2=>  2  sec
//    <3=>  3  sec
//    <4=>  4  sec
//    <5=>  5  sec
//    <6=>  6  sec
//    <7=>  7  sec
//    <8=>  8  sec
//    <9=>  9  sec
//    <10=> 10 sec
//    <11=> 11 sec
//    <12=> 12 sec
//    <13=> 13 sec
//    <14=> 14 sec
//    <15=> 15 sec
//    <16=> 16 sec
//    <17=> 17 sec
//    <18=> 18 sec
//    <19=> 19 sec
//    <20=> 20 sec
//    <21=> 21 sec
//    <22=> 22 sec
//    <23=> 23 sec
//    <24=> 24 sec
//    <25=> 25 sec
//    <26=> 26 sec
//    <27=> 27 sec
//    <28=> 28 sec
//    <29=> 29 sec
//    <30=> 30 sec
//    <31=> 31 sec
//    <32=> 32 sec
//    <33=> 33 sec
//    <34=> 34 sec
//    <35=> 35 sec
//    <36=> 36 sec
//    <37=> 37 sec
//    <38=> 38 sec
//    <39=> 39 sec
//    <40=> 40 sec
//    <41=> 41 sec
//    <42=> 42 sec
//    <43=> 43 sec
//    <44=> 44 sec
//    <45=> 45 sec
//    <46=> 46 sec
//    <47=> 47 sec
//    <48=> 48 sec
//    <49=> 49 sec
//    <50=> 50 sec
//    <51=> 51 sec
//    <52=> 52 sec
//    <53=> 53 sec
//    <54=> 54 sec
//    <55=> 55 sec
//    <56=> 56 sec
//    <57=> 57 sec
//    <58=> 58 sec
//    <59=> 59 sec
//    <60=> 60 sec
//    <61=> 61 sec
//    <62=> 62 sec
//    <63=> 63 sec
//    <64=> 64 sec
//    <65=> 65 sec
//    <66=> 66 sec
//    <67=> 67 sec
//    <68=> 68 sec
//    <69=> 69 sec
//    <70=> 70 sec
//    <71=> 71 sec
//    <72=> 72 sec
//    <73=> 73 sec
//    <74=> 74 sec
//    <75=> 75 sec
//    <76=> 76 sec
//    <77=> 77 sec
//    <78=> 78 sec
//    <79=> 79 sec
//    <80=> 80 sec
//    <81=> 81 sec
//    <82=> 82 sec
//    <83=> 83 sec
//    <84=> 84 sec
//    <85=> 85 sec
//    <86=> 86 sec
//    <87=> 87 sec
//    <88=> 88 sec
//    <89=> 89 sec
//    <90=> 90 sec
//</e>
#define FEAT_DEI_RS05_ENABLE    0x01
#define FEAT_DEI_RS05           0x05

//<e0>Parking Light Output Enable
//<o1>Parking Light Output                
//         <1=> *Constant      
//         <2=> Pulsed        
//         <3=> Off   
//</e>
#define FEAT_DEI_RS08_ENABLE    0x01
#define FEAT_DEI_RS08           0x01



//<e0>Turbo Mode Runtime Enable
//<o1> Turbo Mode Runtime
//    <0=> *Turbo Mode disable
//    <1=>  1  min
//    <2=>  2  min
//    <3=>  3  min
//    <4=>  4  min
//    <5=>  5  min
//    <6=>  6  min
//    <7=>  7  min
//    <8=>  8  min
//    <9=>  9  min
//    <10=> 10 min
//</e>
#define FEAT_DEI_RS09_ENABLE    0x01
#define FEAT_DEI_RS09           0x00


//<e0>Remote Start Runtime Enable
//<o1> Remote Start Runtime( minutes ) <1-60:1>
//</e>
#define FEAT_DEI_RS10_ENABLE    0x01
#define FEAT_DEI_RS10           0x14


//<e0>Smart Start/Timer Mode Runtime Enable
//<o1> Smart Start/Timer Mode Runtime( minutes ) <1-16:1>
//</e>
#define FEAT_DEI_RS11_ENABLE    0x01
#define FEAT_DEI_RS11           0x03


// <e0>Smart Start/Timer Mode Start Count Enable
//  <o1> Smart Start/Timer Mode Start Count
//         <0=> 0
//         <1=> 1
//         <2=> 2
//         <3=> 3
//         <4=> 4
//         <5=> *6
//         <6=> 8
//         <7=> 10
//         <8=> 12
//         <9=> 14
//         <10=> 16
//         <11=> 18
//         <12=> 20
//         <13=> 22
//         <14=> 24
//</e>
#define FEAT_DEI_RS12_ENABLE    0x01
#define FEAT_DEI_RS12           0x05


//<e0>Timer Mode Start Interval Enable
//<o1> Timer Mode Start Interval( Hours )
//         <1=> 1
//         <2=> 2
//         <3=> *3
//         <4=> 4
//         <5=> 6
//         <6=> 8
//         <7=> 10
//         <8=> 12
//         <9=> 14
//         <10=> 16
//         <11=> 18
//         <12=> 20
//         <13=> 22
//         <14=> 24
//</e>
#define FEAT_DEI_RS13_ENABLE    0x01
#define FEAT_DEI_RS13           0x03


//<e0>Smart Start High Temp Enable
//<o1> SmartStart High Temperature Threshold
//        <1=>  OFF
//        <2=>  +40  F
//        <3=>  +50  F
//        <4=>  +60  F
//        <5=>  +70  F
//        <6=>  +80  F
//        <7=>  +90  F
//        <8=>  *+100 F
//        <9=>  +110 F
//        <10=> +120 F
//        <11=> +130 F
//</e>
#define FEAT_DEI_RS14_ENABLE    0x01
#define FEAT_DEI_RS14           0x08

//<e0>Smart Start Low Temp Enable
//<o1> SmartStart Low Temperature Threshold
//        <1=>  OFF
//        <2=>  -20  F
//        <3=>  *-10  F
//        <4=>  - 0  F
//        <5=>  +10  F
//        <6=>  +20  F
//        <7=>  +30  F
//        <8=>  +40  F
//        <9=>  +50  F
//        <10=> +60  F
//        <11=> +70  F
//</e>
#define FEAT_DEI_RS15_ENABLE    0x01
#define FEAT_DEI_RS15           0x03


//<e0>Smart Start Low Battery Enable
//<o1>SmartStart Low Battery Threshold (Volts)
//         <1=>   9.0              
//         <2=>   9.5
//         <3=>  10.0
//         <4=> *10.5
//         <5=>  11.0
//         <6=>  11.5
//         <7=>  12.0
//         <8=>  12.5  
//</e>
#define FEAT_DEI_RS16_ENABLE    0x01
#define FEAT_DEI_RS16           0x04


//<e0>Virtual tach Fine Tune Enable
//<e1.7> Virtual Tach Fine Tune( Miliseconds )
//    <o1.7> *Not Initialized           
//    <o1.0..6> Virtual Tach Fine Tune( Miliseconds ) <0-1000:50><#/50>
//  </e>
//</e>
#define FEAT_DEI_RS17_ENABLE    0x01
#define FEAT_DEI_RS17           0x94


//<e0>Remote Start SafeLock Enable
//<o1>Remote Start SafeLock
//         <1=> *Off
//         <2=>  Safe Lock On
//         <3=>  Smart Safe Lock On
//</e>
#define FEAT_DEI_RS18_ENABLE    0x01
#define FEAT_DEI_RS18           0x01


//<e0>Tachometer Input Enable
//<o1>Tachometer Input option
//         <1=> *Tachometer
//         <2=>  Speed Sensor(VSS)
//</e>
#define FEAT_DEI_RS19_ENABLE    0x01
#define FEAT_DEI_RS19           0x01


//<e0>Idle/Pitstop Mode Enable
//<o1>Idle/Pitstop Mode
//         <1=> *On
//         <2=>  Off
//</e>
#define FEAT_DEI_RS20_ENABLE    0x01
#define FEAT_DEI_RS20           0x01


//<e0>TakeOver Option Enable
//<o1>TakeOver Option
//         <1=> *Enable
//         <2=>  Disable with Door Open
//         <3=>  Disable with Unlock command
//</e>
#define FEAT_DEI_RS21_ENABLE    0x01
#define FEAT_DEI_RS21           0x01

//<e0>Manual Transmission Initialization Enable
//<o1>Manual Transmission Initialization 
//         <1=> *Remote
//         <2=>  2x Parking Brake
//         <3=>  1x Parking Brake
//</e>
#define FEAT_DEI_RS23_ENABLE    0x01
#define FEAT_DEI_RS23           0x01

//<e0>Manual Transmission Completion Enable
//<o1>Manual Transmission Completion 
//         <1=> *Door closed & remote
//         <2=>  Door closed
//         <3=>  Door closed with passive locking
//</e>
#define FEAT_DEI_RS24_ENABLE    0x01
#define FEAT_DEI_RS24           0x01

//<e0>Duration of Pulse After RS Enable
//<o1>Duration of Pulse After RS
//    <1=>  250 milliseconds
//    <2=> *500 milliseconds
//    <3=>  1000 milliseconds
//</e>
#define FEAT_DEI_RS25_ENABLE    0x01
#define FEAT_DEI_RS25           0x02


//<e0>Delay of Activation of Pulse After RS Enable
//<o1>Delay of Activation of Pulse After RS
//    <1=>  1  sec  
//    <2=>  2  sec
//    <3=>  3  sec
//    <4=>  4  sec
//    <5=> *5  sec
//    <6=>  6  sec
//    <7=>  7  sec
//    <8=>  8  sec
//    <9=>  9  sec
//    <10=> 10 sec
//</e>
#define FEAT_DEI_RS26_ENABLE    0x01
#define FEAT_DEI_RS26           0x05



// </h>

//--------------------------------------------------------------------------//
// <h>  System Features
//--------------------------------------------------------------------------//

//<e0>Door lock pulses Enable
//<o1> DoorLock Pulses 
//         <1=>*Single                
//         <2=> Double Unlock Only   
//         <3=>Double Lock & Unlock 
//</e>
#define FEAT_DEI_SYS02_ENABLE   0x01                    
#define FEAT_DEI_SYS02          0x01

//<e0>Door Lock output Duration Enable
//<o1> DoorLock Output Duration( seconds ) 
//      <1=>*0.8sec     
//      <2=>3.5 sec  
//      <3=>0.4 sec  
//</e>
#define FEAT_DEI_SYS03_ENABLE   0x01                    
#define FEAT_DEI_SYS03          0x01

//<e0>Comfort Closure Enable
//<o1>Comfort Closure 
//      <1=>*No Comfort Closure        
//      <2=>Activate with lock       
//      <3=>Activate after lock       
//</e>
#define FEAT_DEI_SYS05_ENABLE   0x01                    
#define FEAT_DEI_SYS05          0x01


//<e0>Hood Trigger Type Enable
//<o1>Hood Trigger Type  
//     <1=>*Normally Open       
//     <2=>Normally closed   
//</e>
#define FEAT_DEI_SYS06_ENABLE   0x01                    
#define FEAT_DEI_SYS06          0x01


//<e0>Ignition Controlled Domelight Enable
//<o1>Ignition Controlled Domelight 
//         <1=>Off 
//         <2=>*On
//</e>
#define FEAT_DEI_SYS07_ENABLE   0x01                    
#define FEAT_DEI_SYS07          0x02
 
 
//<e0>OEM Alarm Disarm Output  Enable
//<o1>OEM Alarm Disarm Output  
//     <1=> With Unlock       
//     <2=> *Before Unlock     
//     <3=> Remote Start Only 
//</e>
#define FEAT_DEI_SYS09_ENABLE   0x01                    
#define FEAT_DEI_SYS09          0x02

 
//<e0>AUX 1 Icon Enable
//<o1>AUX 1 Icon 
//    <1 => *Trunk
//    <2 => Window
//    <3 => Sunroof
//    <4 => Audio
//    <5 => Lights
//    <6 => Left Door
//    <7 => Right Door
//    <8 => Rear hatch
//    <9 => Timed
//    <10=> Pulsed
//    <11=> Garage door
//</e>
#define FEAT_DEI_SYS11_ENABLE   0x01                    
#define FEAT_DEI_SYS11          0x01

//<e0>Aux 1 Output Type Enable
//<o1>Aux 1 Output type        
//    <1=>*Validity         
//    <2=> Latch            
//    <3=> Latch/ reset/ign 
//    <4=> Timed   
//    <5=> Off
//</e>
#define FEAT_DEI_SYS12_ENABLE   0x01                    
#define FEAT_DEI_SYS12          0x01
 
//<e0>Aux 1 Linking Enable
//<o1>Aux 1 Linking            
//    <1=>*No Linking                
//    <2=> Link to Arm               
//    <3=> Link to Disarm            
//    <4=> Link to Arm / Disarm       
//    <5=> Link to Remote Start Only
//</e>
#define FEAT_DEI_SYS13_ENABLE   0x01                    
#define FEAT_DEI_SYS13          0x01
 
//<e0>Aux 1 Timed Output Enable
//<o1>AUX1 Timed Output (Seconds)  <1-90:1> 
//</e>
#define FEAT_DEI_SYS14_ENABLE   0x01                    
#define FEAT_DEI_SYS14          0x1E

//<e0>Aux 2 Icon Enable
//<o1> AUX 2 Icon 
//    <1 => *Trunk
//    <2 => Window
//    <3 => Sunroof
//    <4 => Audio
//    <5 => Lights
//    <6 => Left Door
//    <7 => Right Door
//    <8 => Rear hatch
//    <9 => Timed
//    <10=> Pulsed
//    <11=> Garage door
//</e>
#define FEAT_DEI_SYS15_ENABLE   0x01                    
#define FEAT_DEI_SYS15          0x01

//<e0>Aux 2 Output Type Enable
//<o1> Aux 2 Output type        
//    <1=>*Validity       
//    <2=> Latch           
//    <3=> Latch/ reset/ign
//    <4=> Timed           
//    <5=> Off
//</e>
#define FEAT_DEI_SYS16_ENABLE   0x01                    
#define FEAT_DEI_SYS16          0x01
 

//<e0>Aux 2 Linking Enable
//<o1> Aux 2 Linking            
//    <1=>*No Linking                
//    <2=> Link to Arm               
//    <3=> Link to Disarm            
//    <4=> Link to Arm / Disarm       
//    <5=> Link to Remote Start Only
//</e>
#define FEAT_DEI_SYS17_ENABLE   0x01                    
#define FEAT_DEI_SYS17          0x01


//<e0>Aux 2 Timed Output Enable
//<o1> AUX2 Timed Output (Seconds)  <1-90:1> 
//</e>
#define FEAT_DEI_SYS18_ENABLE   0x01                    
#define FEAT_DEI_SYS18          0x1E
 

//<e0>Aux 3 Icon Enable
//<o1> AUX 3  Icon
//    <1 => *Trunk
//    <2 => Window
//    <3 => Sunroof
//    <4 => Audio
//    <5 => Lights
//    <6 => Left Door
//    <7 => Right Door
//    <8 => Rear hatch
//    <9 => Timed
//    <10=> Pulsed
//    <11=> Garage door
//</e>
#define FEAT_DEI_SYS19_ENABLE   0x01                    
#define FEAT_DEI_SYS19          0x01


//<e0>Aux 3 Output Type Enable
//<o1> Aux 3 Output type        
//    <1=>*Validity       
//    <2=> Latch           
//    <3=> Latch/ reset/ign
//    <4=> Timed           
//    <5=> Off
//</e>
#define FEAT_DEI_SYS20_ENABLE   0x01                    
#define FEAT_DEI_SYS20          0x01
 
//<e0>Aux 3 Linking Enable
//<o1> Aux 3 Linking            
//    <1=>*No Linking                
//    <2=> Link to Arm               
//    <3=> Link to Disarm            
//    <4=> Link to Arm / Disarm       
//    <5=> Link to Remote Start Only
//</e>
#define FEAT_DEI_SYS21_ENABLE   0x01                    
#define FEAT_DEI_SYS21          0x01

//<e0>Aux 3 Timed Output Enable
//<o1> AUX3 Timed Output (Seconds)  <1-90:1> 
//</e>
#define FEAT_DEI_SYS22_ENABLE   0x01                    
#define FEAT_DEI_SYS22          0x1E


//<e0>Aux 4 Output Type Enable
//<o1> Aux 4 Output type        
//    <1=>*Validity       
//    <2=> Latch           
//    <3=> Latch/ reset/ign
//    <4=> Timed           
//    <5=> Off
//</e>
#define FEAT_DEI_SYS23_ENABLE   0x01                    
#define FEAT_DEI_SYS23          0x01
 
//<e0>Aux 4 Linking Enable
//<o1> Aux 4 Linking            
//    <1=>*No Linking                
//    <2=> Link to Arm               
//    <3=> Link to Disarm            
//    <4=> Link to Arm / Disarm       
//    <5=> Link to Remote Start Only
//</e>
#define FEAT_DEI_SYS24_ENABLE   0x01                    
#define FEAT_DEI_SYS24          0x01


//<e0>Aux 4 Timed Output Enable
//<o1> AUX4 Timed Output (Seconds)  <1-90:1> 
//</e>
#define FEAT_DEI_SYS25_ENABLE   0x01                    
#define FEAT_DEI_SYS25          0x1E


//<e0>Aux/Trunk Output Type Enable
//<o1>Aux/Trunk Output type
//    <1=> Validity       
//    <2=> *1-sec Pulse Output
//</e>
#define FEAT_DEI_SYS26_ENABLE   0x01                    
#define FEAT_DEI_SYS26          0x02

//<e0>Aux/Trunk Icon Enable
//<o1>Aux/Trunk Icon 
//         <1 => *Trunk
//         <2 => Window
//         <3 => Sunroof
//         <4 => Audio
//         <5 => Lights
//         <6 => Left Door
//         <7 => Right Door
//         <8 => Rear hatch
//         <9 => Timed
//         <10=> Pulsed
//         <11=> Garage door
//</e>
#define FEAT_DEI_SYS27_ENABLE   0x01                    
#define FEAT_DEI_SYS27          0x01


//<e0>Remote keypad unlocking Enable
//<o1>Remote keypad unlocking
//         <1 => Off
//         <2 =>*On
//</e>
#define FEAT_DEI_SYS28_ENABLE   0x01                    
#define FEAT_DEI_SYS28          0x02

//<e0>Door Switch Type Option ENABLE
//<o1>Door Switch Type Option
//         <1 =>*Normally Open
//         <2 => Normally Close
//</e>
#define FEAT_DEI_SYS30_ENABLE   0x01
#define FEAT_DEI_SYS30          0x01

//<e0>Trunk Switch Type Option ENABLE
//<o1>Trunk Switch Type Option
//         <1 =>*Normally Open
//         <2 => Normally Close
//</e>
#define FEAT_DEI_SYS31_ENABLE   0x01
#define FEAT_DEI_SYS31          0x01

//<e0>Controlled Door Lock Option ENABLE
//<o1>Controlled Door Lock Option
//         <1 =>*Off
//         <2 => Ignition
//         <3 => RPM
//</e>
#define FEAT_DEI_SYS32_ENABLE   0x01
#define FEAT_DEI_SYS32          0x01

//<e0>Controlled Door Lock Function ENABLE
//<o1>Controlled Door Lock Function
//         <1 =>*Lock & Unlock
//         <2 => Lock only
//         <3 => Unlock only
//</e>
#define FEAT_DEI_SYS33_ENABLE   0x01
#define FEAT_DEI_SYS33          0x01

//<e0>Controlled Unlock All Doors Option ENABLE
//<o1>Controlled Unlock All Doors Option
//         <1 =>*Off
//         <2 => On
//</e>
#define FEAT_DEI_SYS34_ENABLE   0x01
#define FEAT_DEI_SYS34          0x01

//<e0>Driver Priority Unlocking Option ENABLE
//<o1>Driver Priority Unlocking Option
//         <1 => On
//         <2 => *Off
//</e>
#define FEAT_DEI_SYS35_ENABLE   0x01
#define FEAT_DEI_SYS35          0x02

//<e0>Parking Light Confirmation Option ENABLE
//<o1>Parking Light Confirmation Option
//         <1 =>*On
//         <2 => Off
//</e>
#define FEAT_DEI_SYS36_ENABLE   0x01
#define FEAT_DEI_SYS36          0x01

//<e0>Antenna LED Flashing Option ENABLE
//<o1>Antenna LED Flashing Option
//         <1 =>*On
//         <2 => Off
//</e>
#define FEAT_DEI_SYS37_ENABLE   0x01
#define FEAT_DEI_SYS37          0x01

//<e0>Ignition Disarm Option ENABLE
//<o1>Ignition Disarm Option
//         <1 =>*Off
//         <2 => On
//</e>
#define FEAT_DEI_SYS38_ENABLE   0x01
#define FEAT_DEI_SYS38          0x01

//<e0>FOB Voltage Option ENABLE
//<o1>FOB Voltage Option
//         <1 =>*0 V
//         <2 => 1.5 V
//         <3 => 3.0 V
//         <4 => 4.5 V
//         <5 => 6.0 V
//</e>
#define FEAT_DEI_SYS39_ENABLE   		0x00
#define FEAT_DEI_SYS39 					0x01

// </h>

//--------------------------------------------------------------------------//
// <h>  Programmable Inputs/Outputs
//--------------------------------------------------------------------------//

//<e0>Output 1 option is visible on web
//<o1>Output1 (-)
//         <0=>  Not Configured
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
//         <15=> Pulse after startup
//         <16=> Accessory
//         <17=> Ignition
//         <18=> Starter
//         <19=> Parking Lights
//         <20=> Dome light
//         <21=> Siren/Horn
//         <22=> Starter-Kill
//         <23=> Defroster(Latched)
//         <24=> Defroster(Pulsed 800ms)
//         <25=> Led Flash Status
//         <26=> Comfort Closure
//         <27=> Ground when in ready mode
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> ALT RF Loop for FOB
//         <33=> IID Engine Running Detected
//         <34=> Slide Open
//         <35=> Slide Close
//         <36=> PTIM Relay
//         <37=> StsTrunk
//         <38=> StsBrake
//         <39=> StsProx
//</e>
#define FEAT_DEI_SYS40_ENABLE   		0x01
#define FEAT_DEI_SYS40 					0x00

//<e0>Output 2 option is visible on web
//<o1>Output2 (-)
//         <0=>  Not Configured
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
//         <15=> Pulse after startup
//         <16=> Accessory
//         <17=> Ignition
//         <18=> Starter
//         <19=> Parking Lights
//         <20=> Dome light
//         <21=> Siren/Horn
//         <22=> Starter-Kill
//         <23=> Defroster(Latched)
//         <24=> Defroster(Pulsed 800ms)
//         <25=> Led Flash Status
//         <26=> Comfort Closure
//         <27=> Ground when in ready mode
//         <28=> *Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> ALT RF Loop for FOB
//         <33=> IID Engine Running Detected
//         <34=> Slide Open
//         <35=> Slide Close
//         <36=> PTIM Relay
//         <37=> StsTrunk
//         <38=> StsBrake
//         <39=> StsProx
//</e>
#define FEAT_DEI_SYS41_ENABLE   		0x00
#define FEAT_DEI_SYS41 					0x00

//<e0>Output 3 option is visible on web
//<o1>Output3 (-)
//         <0=>  Not Configured
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
//         <15=> Pulse after startup
//         <16=> Accessory
//         <17=> Ignition
//         <18=> Starter
//         <19=> Parking Lights
//         <20=> Dome light
//         <21=> Siren/Horn
//         <22=> Starter-Kill
//         <23=> Defroster(Latched)
//         <24=> Defroster(Pulsed 800ms)
//         <25=> Led Flash Status
//         <26=> Comfort Closure
//         <27=> Ground when in ready mode
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> ALT RF Loop for FOB
//         <33=> IID Engine Running Detected
//         <34=> Slide Open
//         <35=> Slide Close
//         <36=> PTIM Relay
//         <37=> StsTrunk
//         <38=> StsBrake
//         <39=> StsProx
//</e>
#define FEAT_DEI_SYS42_ENABLE   		0x00
#define FEAT_DEI_SYS42 					0x00

//<e0>Output 4 option is visible on web
//<o1>Output4 (-)
//         <0=>  Not Configured
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
//         <15=> Pulse after startup
//         <16=> Accessory
//         <17=> Ignition
//         <18=> Starter
//         <19=> Parking Lights
//         <20=> Dome light
//         <21=> Siren/Horn
//         <22=> Starter-Kill
//         <23=> Defroster(Latched)
//         <24=> Defroster(Pulsed 800ms)
//         <25=> Led Flash Status
//         <26=> Comfort Closure
//         <27=> Ground when in ready mode
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> ALT RF Loop for FOB
//         <33=> IID Engine Running Detected
//         <34=> Slide Open
//         <35=> Slide Close
//         <36=> PTIM Relay
//         <37=> StsTrunk
//         <38=> StsBrake
//         <39=> StsProx
//</e>
#define FEAT_DEI_SYS43_ENABLE   		0x01
#define FEAT_DEI_SYS43 					0x00

//<e0>Output 5 option is visible on web
//<o1>Output5 (-)
//         <0=>  Not Configured
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
//         <15=> Pulse after startup
//         <16=> Accessory
//         <17=> Ignition
//         <18=> Starter
//         <19=> Parking Lights
//         <20=> Dome light
//         <21=> Siren/Horn
//         <22=> Starter-Kill
//         <23=> Defroster(Latched)
//         <24=> Defroster(Pulsed 800ms)
//         <25=> Led Flash Status
//         <26=> Comfort Closure
//         <27=> Ground when in ready mode
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> ALT RF Loop for FOB
//         <33=> IID Engine Running Detected
//         <34=> Slide Open
//         <35=> Slide Close
//         <36=> PTIM Relay
//         <37=> StsTrunk
//         <38=> StsBrake
//         <39=> StsProx
//</e>
#define FEAT_DEI_SYS44_ENABLE           0x00
#define FEAT_DEI_SYS44                  0x02

//<e0>Output 6 option is visible on web
//<o1>Output6 (+)
//         <0=>  Not Configured
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
//         <11=> *Aux 2
//         <12=> Aux 3
//         <13=> Aux 4
//         <15=> Pulse after startup
//         <16=> Accessory
//         <17=> Ignition
//         <18=> Starter
//         <19=> Parking Lights
//         <20=> Dome light
//         <21=> Siren/Horn
//         <22=> Starter-Kill
//         <23=> Defroster(Latched)
//         <24=> Defroster(Pulsed 800ms)
//         <25=> Led Flash Status
//         <26=> Comfort Closure
//         <27=> Ground when in ready mode
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> ALT RF Loop for FOB
//         <33=> IID Engine Running Detected
//         <34=> Slide Open
//         <35=> Slide Close
//         <36=> PTIM Relay
//         <37=> StsTrunk
//         <38=> StsBrake
//         <39=> StsProx
//</e>
#define FEAT_DEI_SYS45_ENABLE   		0x00
#define FEAT_DEI_SYS45 					0x1B

//<e0>Output 7 option is visible on web
//<o1>Output7 (+/-)
//         <0=>  Not Configured
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
//         <15=> Pulse after startup
//         <16=> Accessory
//         <17=> Ignition
//         <18=> Starter
//         <19=> Parking Lights
//         <20=> Dome light
//         <21=> Siren/Horn
//         <22=> Starter-Kill
//         <23=> Defroster(Latched)
//         <24=> Defroster(Pulsed 800ms)
//         <25=> Led Flash Status
//         <26=> Comfort Closure
//         <27=> Ground when in ready mode
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> ALT RF Loop for FOB
//         <33=> IID Engine Running Detected
//         <34=> Slide Open
//         <35=> Slide Close
//         <36=> PTIM Relay
//         <37=> StsTrunk
//         <38=> StsBrake
//         <39=> StsProx
//</e>
#define FEAT_DEI_SYS46_ENABLE   		0x01
#define FEAT_DEI_SYS46 					0x00

//<e0>Output 8 option is visible on web
//<o1>Output8 (+)
//         <0=>  Not Configured
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
//         <15=> Pulse after startup
//         <16=> Accessory
//         <17=> *Ignition
//         <18=> Starter
//         <19=> Parking Lights
//         <20=> Dome light
//         <21=> Siren/Horn
//         <22=> Starter-Kill
//         <23=> Defroster(Latched)
//         <24=> Defroster(Pulsed 800ms)
//         <25=> Led Flash Status
//         <26=> Comfort Closure
//         <27=> Ground when in ready mode
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> ALT RF Loop for FOB
//         <33=> IID Engine Running Detected
//         <34=> Slide Open
//         <35=> Slide Close
//         <36=> PTIM Relay
//         <37=> StsTrunk
//         <38=> StsBrake
//         <39=> StsProx
//</e>
#define FEAT_DEI_SYS47_ENABLE   		0x01
#define FEAT_DEI_SYS47 					0x00

//<e0>Output 9 option is visible on web
//<o1>Output9 (+)
//         <0=>  Not Configured
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
//         <15=> Pulse after startup
//         <16=> *Accessory
//         <17=> Ignition
//         <18=> Starter
//         <19=> Parking Lights
//         <20=> Dome light
//         <21=> Siren/Horn
//         <22=> Starter-Kill
//         <23=> Defroster(Latched)
//         <24=> Defroster(Pulsed 800ms)
//         <25=> Led Flash Status
//         <26=> Comfort Closure
//         <27=> Ground when in ready mode
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> ALT RF Loop for FOB
//         <33=> IID Engine Running Detected
//         <34=> Slide Open
//         <35=> Slide Close
//         <36=> PTIM Relay
//         <37=> StsTrunk
//         <38=> StsBrake
//         <39=> StsProx
//</e>
#define FEAT_DEI_SYS48_ENABLE   		0x01
#define FEAT_DEI_SYS48 					0x00

//<e0>Output 10 option is visible on web
//<o1>Output10  (+)
//         <17=> *Ignition
//</e>
#define FEAT_DEI_SYS49_ENABLE   		0x00
#define FEAT_DEI_SYS49 					0x11

//<e0>Output 11 option is visible on web
//<o1>Output11  (+)
//         <0=>  Not Configured
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
//         <15=> Pulse after startup
//         <16=> Accessory
//         <17=> Ignition
//         <18=> *Starter
//         <19=> Parking Lights
//         <20=> Dome light
//         <21=> Siren/Horn
//         <22=> Starter-Kill
//         <23=> Defroster(Latched)
//         <24=> Defroster(Pulsed 800ms)
//         <25=> Led Flash Status
//         <26=> Comfort Closure
//         <27=> Ground when in ready mode
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> ALT RF Loop for FOB
//         <33=> IID Engine Running Detected
//         <34=> Slide Open
//         <35=> Slide Close
//         <36=> PTIM Relay
//         <37=> StsTrunk
//         <38=> StsBrake
//         <39=> StsProx
//</e>
#define FEAT_DEI_SYS50_ENABLE   		0x01
#define FEAT_DEI_SYS50 					0x00

//<e0>Output 12 option is visible on web
//<o1>Output12  (+)
//         <31=> *AG Starter Kill
//</e>
#define FEAT_DEI_SYS51_ENABLE   		0x01
#define FEAT_DEI_SYS51 					0x1E

//<e0>Output 13 option is visible on web
//<o1>Output13  (-)
//         <0=>  Not Configured
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
//         <15=> Pulse after startup
//         <16=> Accessory
//         <17=> Ignition
//         <18=> Starter
//         <19=> Parking Lights
//         <20=> Dome light
//         <21=> Siren/Horn
//         <22=> Starter-Kill
//         <23=> Defroster(Latched)
//         <24=> Defroster(Pulsed 800ms)
//         <25=> Led Flash Status
//         <26=> Comfort Closure
//         <27=> Ground when in ready mode
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> ALT RF Loop for FOB
//         <33=> IID Engine Running Detected
//         <34=> Slide Open
//         <35=> Slide Close
//         <36=> PTIM Relay
//         <37=> StsTrunk
//         <38=> StsBrake
//         <39=> StsProx
//</e>
#define FEAT_DEI_SYS52_ENABLE   	0x00
#define FEAT_DEI_SYS52				0x00

//<e0>Output 14 option is visible on web
//<o1>Output14  (-)
//         <0=>  Not Configured
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
//         <15=> Pulse after startup
//         <16=> Accessory
//         <17=> Ignition
//         <18=> Starter
//         <19=> Parking Lights
//         <20=> Dome light
//         <21=> *Siren/Horn
//         <22=> Starter-Kill
//         <23=> Defroster(Latched)
//         <24=> Defroster(Pulsed 800ms)
//         <25=> Led Flash Status
//         <26=> Comfort Closure
//         <27=> Ground when in ready mode
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> ALT RF Loop for FOB
//         <33=> IID Engine Running Detected
//         <34=> Slide Open
//         <35=> Slide Close
//         <36=> PTIM Relay
//         <37=> StsTrunk
//         <38=> StsBrake
//         <39=> StsProx
//</e>
#define FEAT_DEI_SYS53_ENABLE           0x01
#define FEAT_DEI_SYS53                  0x00


//<e0>Relay 1 option is visible on web
//<o1>Relay1
//         <0=>  Not Configured
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
//         <15=> Pulse after startup
//         <16=> Accessory
//         <17=> Ignition
//         <18=> Starter
//         <19=> Parking Lights
//         <20=> Dome light
//         <21=> Siren/Horn
//         <22=> Starter-Kill
//         <23=> Defroster(Latched)
//         <24=> Defroster(Pulsed 800ms)
//         <25=> Led Flash Status
//         <26=> Comfort Closure
//         <27=> Ground when in ready mode
//         <28=> *Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> ALT RF Loop for FOB
//         <33=> IID Engine Running Detected
//         <34=> Slide Open
//         <35=> Slide Close
//         <36=> PTIM Relay
//         <37=> StsTrunk
//         <38=> StsBrake
//         <39=> StsProx
//</e>
#define FEAT_DEI_SYS54_ENABLE   		0x00
#define FEAT_DEI_SYS54 					0x00


//<e0>Relay 2 option is visible on web
//<o1>Relay2
//         <0=>  Not Configured
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
//         <15=> Pulse after startup
//         <16=> Accessory
//         <17=> Ignition
//         <18=> Starter
//         <19=> Parking Lights
//         <20=> Dome light
//         <21=> Siren/Horn
//         <22=> Starter-Kill
//         <23=> Defroster(Latched)
//         <24=> Defroster(Pulsed 800ms)
//         <25=> Led Flash Status
//         <26=> Comfort Closure
//         <27=> Ground when in ready mode
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> ALT RF Loop for FOB
//         <33=> IID Engine Running Detected
//         <34=> Slide Open
//         <35=> Slide Close
//         <36=> PTIM Relay
//         <37=> StsTrunk
//         <38=> StsBrake
//         <39=> StsProx
//</e>
#define FEAT_DEI_SYS55_ENABLE   		0x00
#define FEAT_DEI_SYS55 					0x00

//<e0>Relay 3 option is visible on web
//<o1>Relay3
//         <0=>  Not Configured
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
//         <15=> Pulse after startup
//         <16=> Accessory
//         <17=> Ignition
//         <18=> Starter
//         <19=> *Parking Lights
//         <20=> Dome light
//         <21=> Siren/Horn
//         <22=> Starter-Kill
//         <23=> Defroster(Latched)
//         <24=> Defroster(Pulsed 800ms)
//         <25=> Led Flash Status
//         <26=> Comfort Closure
//         <27=> Ground when in ready mode
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> ALT RF Loop for FOB
//         <33=> IID Engine Running Detected
//         <34=> Slide Open
//         <35=> Slide Close
//         <36=> PTIM Relay
//         <37=> StsTrunk
//         <38=> StsBrake
//         <39=> StsProx
//</e>
#define FEAT_DEI_SYS56_ENABLE   		0x01
#define FEAT_DEI_SYS56 					0x00


//<e0>Input Negative 1 option is visible on web
//<o1>Input Negative 1 Start Trigger
//         <0=>  Not Configured
//         <1=>  *Activation
//         <2=>  Brake
//         <3=>  Door Trigger
//         <4=>  Hood Trigger
//         <5=>  Instant Trigger
//         <6=>  E-Brake
//         <7=>  Headlight Detection
//         <8=>  Trunk Trigger
//         <9=>  Valet Switch
//         <10=> Ignition
//         <11=> Custom Pos1
//         <12=> Sensor Trigger Input
//         <13=> Tamper
//         <14=> PTIM_Stop
//         <15=> Ext Open Trigger
//         <16=> Ext Close Trigger
//         <17=> Ext Trunk Trigger
//</e>
#define FEAT_DEI_SYS57_ENABLE   		0x01
#define FEAT_DEI_SYS57 					0x0D

//Input Negative 2 is fixed

//<e0>Input Negative 3 option is visible on web
//<o1>Input Negative 3
//         <0=>  Not Configured
//         <1=>  Activation
//         <2=>  Brake
//         <3=>  Door Trigger
//         <4=>  Hood Trigger
//         <5=>  Instant Trigger
//         <6=>  E-Brake
//         <7=>  *Headlight Detection
//         <8=>  Trunk Trigger
//         <9=>  Valet Switch
//         <10=> Ignition
//         <11=> Custom Pos1
//         <12=> Sensor Trigger Input
//         <13=> Tamper
//         <14=> PTIM_Stop
//         <15=> Ext Open Trigger
//         <16=> Ext Close Trigger
//         <17=> Ext Trunk Trigger
//</e>
#define FEAT_DEI_SYS58_ENABLE   		0x01
#define FEAT_DEI_SYS58 					0x0D


//<e0>Input Negative 4 option is visible on web
//<o1>Input Negative 4
//         <0=>  Not Configured
//         <1=>  Activation
//         <2=>  Brake
//         <3=>  Door Trigger
//         <4=>  Hood Trigger
//         <5=>  Instant Trigger
//         <6=>  *E-Brake
//         <7=>  Headlight Detection
//         <8=>  Trunk Trigger
//         <9=>  Valet Switch
//         <10=> Ignition
//         <11=> Custom Pos1
//         <12=> Sensor Trigger Input
//         <13=> Tamper
//         <14=> PTIM_Stop
//         <15=> Ext Open Trigger
//         <16=> Ext Close Trigger
//         <17=> Ext Trunk Trigger
//</e>
#define FEAT_DEI_SYS59_ENABLE   		0x01
#define FEAT_DEI_SYS59 					0x0D


//<e0>Input Negative 5 option is visible on web
//<o1>Input Negative 5
//         <0=>  Not Configured
//         <1=>  Activation
//         <2=>  Brake
//         <3=>  Door Trigger
//         <4=>  *Hood Trigger
//         <5=>  Instant Trigger
//         <6=>  E-Brake
//         <7=>  Headlight Detection
//         <8=>  Trunk Trigger
//         <9=>  Valet Switch
//         <10=> Ignition
//         <11=> Custom Pos1
//         <12=> Sensor Trigger Input
//         <13=> Tamper
//         <14=> PTIM_Stop
//         <15=> Ext Open Trigger
//         <16=> Ext Close Trigger
//         <17=> Ext Trunk Trigger
//</e>
#define FEAT_DEI_SYS60_ENABLE   		0x01
#define FEAT_DEI_SYS60 					0x0D

//<e0>Input Negative 7 option is visible on web
//<o1>Input Negative 7
//         <0=>  Not Configured
//         <1=>  Activation
//         <2=>  Brake
//         <3=>  Door Trigger
//         <4=>  Hood Trigger
//         <5=>  Instant Trigger
//         <6=>  E-Brake
//         <7=>  Headlight Detection
//         <8=>  *Trunk Trigger
//         <9=>  Valet Switch
//         <10=> Ignition
//         <11=> Custom Pos1
//         <12=> Sensor Trigger Input
//         <13=> Tamper
//         <14=> PTIM_Stop
//         <15=> Ext Open Trigger
//         <16=> Ext Close Trigger
//         <17=> Ext Trunk Trigger
//</e>
#define FEAT_DEI_SYS61_ENABLE   		0x01
#define FEAT_DEI_SYS61 					0x0D


//<e0>Input Positive 1 option is visible on web
//<o1>Input Positive 1
//         <0=>  Not Configured
//         <1=>  Activation
//         <2=>  Brake
//         <3=>  Door Trigger
//         <4=>  Hood Trigger
//         <5=>  *Instant Trigger
//         <6=>  E-Brake
//         <7=>  Headlight Detection
//         <8=>  Trunk Trigger
//         <9=>  Valet Switch
//         <10=> Ignition
//         <11=> Custom Pos1
//         <12=> Sensor Trigger Input
//         <13=> Tamper
//         <14=> PTIM_Stop
//         <15=> Ext Open Trigger
//         <16=> Ext Close Trigger
//         <17=> Ext Trunk Trigger
//</e>
#define FEAT_DEI_SYS62_ENABLE   		0x01
#define FEAT_DEI_SYS62 					0x0D

//<e0>Input Positive 2 option is visible on web
//<o1>Input Positive 2
//         <0=>  Not Configured
//         <1=>  Activation
//         <2=>  *Brake
//         <3=>  Door Trigger
//         <4=>  Hood Trigger
//         <5=>  Instant Trigger
//         <6=>  E-Brake
//         <7=>  Headlight Detection
//         <8=>  Trunk Trigger
//         <9=>  Valet Switch
//         <10=> Ignition
//         <11=> Custom Pos1
//         <12=> Sensor Trigger Input
//         <13=> Tamper
//         <14=> PTIM_Stop
//         <15=> Ext Open Trigger
//         <16=> Ext Close Trigger
//         <17=> Ext Trunk Trigger
//</e>
#define FEAT_DEI_SYS63_ENABLE   		0x01
#define FEAT_DEI_SYS63 					0x0A

//<e0>Input Positive 3 option is visible on web
//<o1>Input Positive 3
//         <10=>  ignition
//</e>
#define FEAT_DEI_SYS64_ENABLE   		0x01
#define FEAT_DEI_SYS64 					0x0D


//<e0>Input Positive Aux1 option is visible on web
//<o1>Input Positive Aux1
//         <0=>  Not Configured
//         <1=>  Activation
//         <2=>  Brake
//         <3=>  *Door Trigger
//         <4=>  Hood Trigger
//         <5=>  Instant Trigger
//         <6=>  E-Brake
//         <7=>  Headlight Detection
//         <8=>  Trunk Trigger
//         <9=>  Valet Switch
//         <10=> Ignition
//         <11=> Custom Pos1
//         <12=> Sensor Trigger Input
//         <13=> Tamper
//         <14=> PTIM_Stop
//         <15=> Ext Open Trigger
//         <16=> Ext Close Trigger
//         <17=> Ext Trunk Trigger
//</e>
#define FEAT_DEI_SYS65_ENABLE   		0x01
#define FEAT_DEI_SYS65 					0x03

// </h>


//--------------------------------------------------------------------------//
// <h>  Sense config
//--------------------------------------------------------------------------//
//<e0>Sense doors status Enable -
//<o1>Sense doors status	
//         <1=> *Enabled
//         <2=> Disabled
//</e>
#define FEAT_DEI_SYS69_ENABLE           0x01
#define FEAT_DEI_SYS69                  0x01

//<e0>Sense ignition status Enable
//<o1>Sense ignition status
//         <1=> *Enabled
//         <2=> Disabled
//</e>
#define FEAT_DEI_SYS70_ENABLE           0x01
#define FEAT_DEI_SYS70                  0x01

//<e0>Sense hood status Enable
//<o1>Sense hood status
//         <1=> *Enabled
//         <2=> Disabled
//</e>
#define FEAT_DEI_SYS71_ENABLE           0x01
#define FEAT_DEI_SYS71                  0x01

//<e0> Sense trunk status Enable
// <o1>Sense trunk status
//         <1=> *Enabled
//         <2=> Disabled
//</e>
#define FEAT_DEI_SYS72_ENABLE           0x01
#define FEAT_DEI_SYS72                  0x01

//<e0> Sense Handbrake Enable
//<o1>Sense handbrake status
//         <1=> *Enabled
//         <2=> Disabled
//</e>
#define FEAT_DEI_SYS73_ENABLE           0x01
#define FEAT_DEI_SYS73                  0x01


//<e0>Sense Tach Enable
//<o1>Sense tach status
//         <1=> *Enabled
//         <2=> Disabled
//</e>
#define FEAT_DEI_SYS74_ENABLE           0x01
#define FEAT_DEI_SYS74                  0x01


//<e0>Sense brake Enable
//<o1>Sense brake status
//         <1=> *Enabled
//         <2=> Disabled
//</e>
#define FEAT_DEI_SYS75_ENABLE           0x01
#define FEAT_DEI_SYS75                  0x01

//<e0>Sense OEM LOCK Enable
//<o1>Sense oem lock
//         <1=> Disabled
//         <2=> *Enabled
//</e>
#define FEAT_DEI_SYS77_ENABLE           0x01
#define FEAT_DEI_SYS77                  0x02


//<e0>Sense OEM UNLOCK Enable
//<o1>Sense oem unlock
//         <1=> Disabled
//         <2=> *Enabled
//</e>
#define FEAT_DEI_SYS78_ENABLE           0x01
#define FEAT_DEI_SYS78                  0x02


//<e0>Sense OEM TRUNK Enable
//<o1>Sense oem trunk
//         <1=> Disabled
//         <2=> *Enabled
//</e>
#define FEAT_DEI_SYS79_ENABLE           0x01
#define FEAT_DEI_SYS79                  0x02

//<e0>Engine Speed Sensing Enable
//<o1>Engine Speed Sensing
//         <1=> *Enabled
//         <2=> Disabled
//</e>
#define FEAT_DEI_SYS80_ENABLE           0x01
#define FEAT_DEI_SYS80                  0x01

//<e0>Arm Output Enable
//<o1>Arm Output
//         <1=> *Enabled
//         <2=> Disabled
//</e>
#define FEAT_DEI_SYS81_ENABLE           0x01
#define FEAT_DEI_SYS81                  0x01

//<e0>Disarm Output Enable
//<o1>Disarm Output
//         <1=> *Enabled
//         <2=> Disabled
//</e>
#define FEAT_DEI_SYS82_ENABLE           0x01
#define FEAT_DEI_SYS82                  0x01

//<e0>Lock Output Enable
//<o1>Lock Output
//         <1=> *Enabled
//         <2=> Disabled
//</e>
#define FEAT_DEI_SYS83_ENABLE           0x01
#define FEAT_DEI_SYS83                  0x01

//<e0>Parking Lights Output Enable
//<o1>Parking Lights Output
//         <1=> *Enabled
//         <2=> Disabled
//</e>
#define FEAT_DEI_SYS84_ENABLE           0x01
#define FEAT_DEI_SYS84                  0x01

//<e0>RAP Output Enable
//<o1>RAP Output
//         <1=> *Enabled
//         <2=> Disabled
//</e>
#define FEAT_DEI_SYS85_ENABLE           0x01
#define FEAT_DEI_SYS85                  0x01

//<e0>Trunk Output Enable
//<o1>Trunk Output
//         <1=> *Enabled
//         <2=> Disabled
//</e>
#define FEAT_DEI_SYS86_ENABLE           0x01
#define FEAT_DEI_SYS86                  0x01

//<e0>Unlock Output Enable
//<o1>Unlock Output
//         <1=> *Enabled
//         <2=> Disabled
//</e>
#define FEAT_DEI_SYS87_ENABLE           0x01
#define FEAT_DEI_SYS87                  0x01

// </h>



//--------------------------------------------------------------------------//
// <h>Special Features
//--------------------------------------------------------------------------//

//<e0>Feature Custom 00 Enable
//<o1>Light control on CAN bus
//         <1=> *Option1
//         <2=>  Option2
//         <3=>  Option3
//         <4=>  Option4
//         <5=>  Option5 
//</e>
#define FEAT_DEI_SYS88_ENABLE           0x00                    
#define FEAT_DEI_SYS88                  0x01


//<e0>Feature Custom 01 Enable
//<o1>Feature Custom 01
//         <1=> *Option1
//         <2=>  Option2
//         <3=>  Option3
//         <4=>  Option4
//         <5=>  Option5 
//</e>
#define FEAT_DEI_SYS89_ENABLE   		0x00
#define FEAT_DEI_SYS89 					0x01


//<e0>Feature Custom 02 Enable
//<o1>Feature Custom 02
//         <1=> *Option1
//         <2=>  Option2
//         <3=>  Option3
//         <4=>  Option4
//         <5=>  Option5 
//</e>
#define FEAT_DEI_SYS90_ENABLE   		0x00					
#define FEAT_DEI_SYS90 					0x01

//<e0>Feature Custom 03 Enable
//<o1>Feature Custom 03
//         <1=> *Option1
//         <2=>  Option2
//         <3=>  Option3
//         <4=>  Option4
//         <5=>  Option5 
//</e>
#define FEAT_DEI_SYS91_ENABLE           0x00                    
#define FEAT_DEI_SYS91                  0x01

//<e0>Feature Custom 04 Enable
//<o1>Feature Custom 04
//         <1=> *Option1
//         <2=>  Option2
//         <3=>  Option3
//         <4=>  Option4
//         <5=>  Option5 
//</e>
#define FEAT_DEI_SYS92_ENABLE           0x00                    
#define FEAT_DEI_SYS92                  0x01

//<e0>Feature Custom 05 Enable
//<o1>Feature Custom 05
//         <1=> *Option1
//         <2=>  Option2
//         <3=>  Option3
//         <4=>  Option4
//         <5=>  Option5 
//</e>
#define FEAT_DEI_SYS93_ENABLE           0x00                    
#define FEAT_DEI_SYS93                  0x01

//<e0>Feature Custom 06 Enable
//<o1>Feature Custom 06
//         <1=> *Option1
//         <2=>  Option2
//         <3=>  Option3
//         <4=>  Option4
//         <5=>  Option5 
//</e>
#define FEAT_DEI_SYS94_ENABLE           0x00                    
#define FEAT_DEI_SYS94                  0x01

//<e0>Feature Custom 07 Enable
//<o1>Feature Custom 07
//         <1=> *Option1
//         <2=>  Option2
//         <3=>  Option3
//         <4=>  Option4
//         <5=>  Option5 
//</e>
#define FEAT_DEI_SYS95_ENABLE           0x00                    
#define FEAT_DEI_SYS95                  0x01

//<e0>Tach Val Status
//<o1>Tach Val Status
//         <1=> *Not Learned
//         <2=>  Learned
//         <3=>  Option3
//         <4=>  Option4
//         <5=>  Option5 
//</e>
#define FEAT_DEI_SYS96_ENABLE           0x00                    
#define FEAT_DEI_SYS96                  0x01


// </h>

// <o0.0..8> Tach Default Reference (x 10)<1-255:1>  
#define TACH_DEFAULT_REF_DIV10         0x50         // Default Tach reference divided by 10 (make it one-byte nvfs variable)

// <o0.0..8> Default Setting for Virtual Tach <1-255:1>  
#define VIRTUAL_TACH_DEF     3

//--------------------------------------------------------------------------//

// <o0.0..8> Security Feature Size <1-255:1>  
#define FEAT_DEI_SS_SIZE     12

// <o0.0..8> Remote Start Feature Size <1-255:1>  
#define FEAT_DEI_RS_SIZE     23

// <o0.0..8> System Feature Size <1-255:1> 
#define FEAT_DEI_SYS_SIZE    87

//*** <<< end of configuration section >>>    ***
     
//--------------------------------------------------------------------------//

#define FEAT_DEI_SIZE       (FEAT_DEI_SYS_SIZE + FEAT_DEI_SS_SIZE + FEAT_DEI_RS_SIZE + 2)

// </h>


#endif

