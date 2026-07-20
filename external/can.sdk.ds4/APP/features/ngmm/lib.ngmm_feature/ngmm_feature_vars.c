/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_feature_vars.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "ngmm_feature_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

//--------------------------------------------------------------------------//

//<h>  Security Features

//<e0>System Arming Mode Enable
//<o1>System Arming Mode
//      <1=> *Active              
//      <2=> Passive Arm - No Lock
//      <3=> Passive Arm & Lock
//      <4=> Auto Re-arm - No Lock
//      <5=> Auto Re-arm & Lock
//</e>
#define FEAT_DEI_SS01_MIN           1
#define FEAT_DEI_SS01_MAX           5
#define FEAT_DEI_SS01_CHECK         TRUE

#if FEAT_DEI_SS01_CHECK==TRUE
    #if FEAT_DEI_SS01<FEAT_DEI_SS01_MIN
        #error FEAT_DEI_SS01 MIN ERROR
    #endif

    #if FEAT_DEI_SS01>FEAT_DEI_SS01_MAX
        #error FEAT_DEI_SS01 MAX ERROR
    #endif
#endif


//<e0>Panic Mode Visible
//<o1>Panic Mode
//      <1=> *On           
//      <2=> Ignition Off only 
//      <3=> Off          
//</e>
#define FEAT_DEI_SS02_MIN           1
#define FEAT_DEI_SS02_MAX           3
#define FEAT_DEI_SS02_CHECK         TRUE

#if FEAT_DEI_SS02_CHECK==TRUE
    #if FEAT_DEI_SS02<FEAT_DEI_SS02_MIN
        #error FEAT_DEI_SS02 MIN ERROR
    #endif

    #if FEAT_DEI_SS02>FEAT_DEI_SS02_MAX
        #error FEAT_DEI_SS02 MAX ERROR
    #endif
#endif


//<e0>Confirmation Chirps Enable
//<o1>Confirmation Chirps
//      <1=>*On - Warn Chirps On
//      <2=>On  - Warn Chirps Off
//      <3=>Off - Warn Chirps On
//      <4=>Off - Warn Chirps Off
//</e>
#define FEAT_DEI_SS03_MIN           1
#define FEAT_DEI_SS03_MAX           4
#define FEAT_DEI_SS03_CHECK         TRUE

#if FEAT_DEI_SS03_CHECK==TRUE
    #if FEAT_DEI_SS03<FEAT_DEI_SS03_MIN
        #error FEAT_DEI_SS03 MIN ERROR
    #endif

    #if FEAT_DEI_SS03>FEAT_DEI_SS03_MAX
        #error FEAT_DEI_SS03 MAX ERROR
    #endif
#endif


//<e0>Horn Function Enable
//<o1>Horn function 
//      <1=>*Trigger & Panic Only
//      <2=>Chirp 20 mS   
//      <3=>Chirp 30 mS   
//      <4=>Chirp 40 mS   
//      <5=>Chirp 50 mS    
//</e>
#define FEAT_DEI_SS04_MIN           1
#define FEAT_DEI_SS04_MAX           5
#define FEAT_DEI_SS04_CHECK         TRUE

#if FEAT_DEI_SS04_CHECK==TRUE
    #if FEAT_DEI_SS04<FEAT_DEI_SS04_MIN
        #error FEAT_DEI_SS04 MIN ERROR
    #endif

    #if FEAT_DEI_SS04>FEAT_DEI_SS04_MAX
        #error FEAT_DEI_SS04 MAX ERROR
    #endif
#endif

//<e0>Override Pulse Count Enable
//<o1>Override Pulse Count
//      <1=>*1
//      <2=>2
//      <3=>3
//      <4=>4
//      <5=>5
//</e>
#define FEAT_DEI_SS05_MIN           1
#define FEAT_DEI_SS05_MAX           5
#define FEAT_DEI_SS05_CHECK         TRUE

#if FEAT_DEI_SS05_CHECK==TRUE
    #if FEAT_DEI_SS05<FEAT_DEI_SS05_MIN
        #error FEAT_DEI_SS05 MIN ERROR
    #endif

    #if FEAT_DEI_SS05>FEAT_DEI_SS05_MAX
        #error FEAT_DEI_SS01 MAX ERROR
    #endif
#endif

//<e0>Sensor Icon Type Enable
//<o1>Sensor Icon Type
//      <1=> None                          
//      <2=> *Shock                  
//      <3=> Field disturbance              
//      <4=> Tilt sensor                    
//      <5=> Glass break
//      <6=> Ultrasonic  
//</e>
//#define FEAT_DEI_SS06_MIN           1
//#define FEAT_DEI_SS06_MAX           6
//#define FEAT_DEI_SS06_CHECK         TRUE

//#if FEAT_DEI_SS06_CHECK==TRUE
//    #if FEAT_DEI_SS06<FEAT_DEI_SS06_MIN
//        #error FEAT_DEI_SS06 MIN ERROR
//    #endif

//    #if FEAT_DEI_SS06>FEAT_DEI_SS06_MAX
//        #error FEAT_DEI_SS06 MAX ERROR
//    #endif
//#endif


//<e0>Trigger/Panic Duration Enable
//<o1>Trigger/Panic Duration( seconds ) <1-180:1>
//</e>
#define FEAT_DEI_SS07_MIN           1
#define FEAT_DEI_SS07_MAX           180
#define FEAT_DEI_SS07_CHECK         TRUE

#if FEAT_DEI_SS07_CHECK==TRUE
    #if FEAT_DEI_SS07<FEAT_DEI_SS07_MIN
        #error FEAT_DEI_SS07 MIN ERROR
    #endif

    #if FEAT_DEI_SS07>FEAT_DEI_SS07_MAX
        #error FEAT_DEI_SS07 MAX ERROR
    #endif
#endif


//<e0>NPC - Nuisance Prevention Circuitry Enable     
//<o1>NPC - Nuisance Prevention Circuitry     
//     <1=>*On  
//     <2=>Off 
//</e>
#define FEAT_DEI_SS09_MIN           1
#define FEAT_DEI_SS09_MAX           2
#define FEAT_DEI_SS09_CHECK         TRUE

#if FEAT_DEI_SS09_CHECK==TRUE
    #if FEAT_DEI_SS09<FEAT_DEI_SS09_MIN
        #error FEAT_DEI_SS09 MIN ERROR
    #endif

    #if FEAT_DEI_SS09>FEAT_DEI_SS09_MAX
        #error FEAT_DEI_SS09 MAX ERROR
    #endif
#endif


//<e0>Door Trigger Error Enable
//<o1>Door Trigger Error Chirp 
//         <1=>Off 
//         <2=>*On
//</e>
#define FEAT_DEI_SS10_MIN           1
#define FEAT_DEI_SS10_MAX           2
#define FEAT_DEI_SS10_CHECK         TRUE

#if FEAT_DEI_SS10_CHECK==TRUE
    #if FEAT_DEI_SS10<FEAT_DEI_SS10_MIN
        #error FEAT_DEI_SS10 MIN ERROR
    #endif

    #if FEAT_DEI_SS10>FEAT_DEI_SS10_MAX
        #error FEAT_DEI_SS10 MAX ERROR
    #endif
#endif


//<e0>Starter Kill Enable
//<o1>Starter Kill Option 
//         <1=>*Off 
//         <2=>Active
//         <3=>Passive
//</e>
#define FEAT_DEI_SS11_MIN           1
#define FEAT_DEI_SS11_MAX           4
#define FEAT_DEI_SS11_CHECK         TRUE

#if FEAT_DEI_SS11_CHECK==TRUE
    #if FEAT_DEI_SS11<FEAT_DEI_SS11_MIN
        #error FEAT_DEI_SS11 MIN ERROR
    #endif

    #if FEAT_DEI_SS11>FEAT_DEI_SS11_MAX
        #error FEAT_DEI_SS11 MAX ERROR
    #endif
#endif


//<e0>Horn Output Timing Enable
//<o1>Horn Output Timing Option
//         <1=>*30 ms
//         <2=>10 ms
//         <3=>20 ms
//         <4=>40 ms
//         <4=>50 ms
//         <6=>60 ms
//</e>
#define FEAT_DEI_SS12_MIN           1
#define FEAT_DEI_SS12_MAX           6
#define FEAT_DEI_SS12_CHECK         TRUE

#if FEAT_DEI_SS12_CHECK==TRUE
    #if FEAT_DEI_SS12<FEAT_DEI_SS12_MIN
        #error FEAT_DEI_SS12 MIN ERROR
    #endif

    #if FEAT_DEI_SS12>FEAT_DEI_SS12_MAX
        #error FEAT_DEI_SS12 MAX ERROR
    #endif
#endif

//<e0>Unlock & Disarm with Trunk Release Enable
//<o1>Unlock & Disarm with Trunk Release
//         <1=>*Off
//         <2=>Smart Unlock
//         <3=>On
//</e>
#define FEAT_DEI_SS13_MIN           1
#define FEAT_DEI_SS13_MAX           3
#define FEAT_DEI_SS13_CHECK         TRUE

#if FEAT_DEI_SS13_CHECK==TRUE
    #if FEAT_DEI_SS13<FEAT_DEI_SS13_MIN
        #error FEAT_DEI_SS13 MIN ERROR
    #endif

    #if FEAT_DEI_SS13>FEAT_DEI_SS13_MAX
        #error FEAT_DEI_SS13 MAX ERROR
    #endif
#endif


//<e0>Auto Re-Arm Mode Enable
//<o1>System Arming Mode
//      <1=> *OFF              
//      <2=> Auto re-arm - No Lock
//      <3=> Auto re-arm & Lock
//</e>
#define FEAT_DEI_SS15_MIN           1
#define FEAT_DEI_SS15_MAX           3
#define FEAT_DEI_SS15_CHECK         TRUE

#if FEAT_DEI_SS15_CHECK==TRUE
    #if FEAT_DEI_SS15<FEAT_DEI_SS15_MIN
        #error FEAT_DEI_SS15 MIN ERROR
    #endif

    #if FEAT_DEI_SS15>FEAT_DEI_SS15_MAX
        #error FEAT_DEI_SS15 MAX ERROR
    #endif
#endif

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
#define FEAT_DEI_RS02_MIN           1
#define FEAT_DEI_RS02_MAX           4
#define FEAT_DEI_RS02_CHECK         TRUE

#if FEAT_DEI_RS02_CHECK==TRUE
    #if FEAT_DEI_RS02<FEAT_DEI_RS02_MIN
        #error FEAT_DEI_RS02 MIN ERROR
    #endif

    #if FEAT_DEI_RS01>FEAT_DEI_RS01_MAX
        #error FEAT_DEI_RS02 MAX ERROR
    #endif
#endif


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
#define FEAT_DEI_RS03_MIN           1
#define FEAT_DEI_RS03_MAX           9
#define FEAT_DEI_RS03_CHECK         TRUE

#if FEAT_DEI_RS03_CHECK==TRUE
    #if FEAT_DEI_RS03<FEAT_DEI_RS03_MIN
        #error FEAT_DEI_RS03 MIN ERROR
    #endif

    #if FEAT_DEI_RS03>FEAT_DEI_RS03_MAX
        #error FEAT_DEI_RS03 MAX ERROR
    #endif
#endif

//<e0>Activation Pulse Enable
//<o1>Activation Pulse Count              
//         <1=> *1     
//         <2=>  2 
//         <3=>  3   
//				 <4=>  0
//</e>
#define FEAT_DEI_RS04_MIN           1
#define FEAT_DEI_RS04_MAX           4
#define FEAT_DEI_RS04_CHECK         TRUE

#if FEAT_DEI_RS04_CHECK==TRUE
    #if FEAT_DEI_RS04<FEAT_DEI_RS04_MIN
        #error FEAT_DEI_RS04 MIN ERROR
    #endif

    #if FEAT_DEI_RS04>FEAT_DEI_RS04_MAX
        #error FEAT_DEI_RS04 MAX ERROR
    #endif
#endif

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
#define FEAT_DEI_RS05_MIN           0
#define FEAT_DEI_RS05_MAX           90
#define FEAT_DEI_RS05_CHECK         TRUE

#if FEAT_DEI_RS05_CHECK==TRUE
    #if FEAT_DEI_RS05<FEAT_DEI_RS05_MIN
        #error FEAT_DEI_RS05 MIN ERROR
    #endif

    #if FEAT_DEI_RS05>FEAT_DEI_RS05_MAX
        #error FEAT_DEI_RS05 MAX ERROR
    #endif
#endif


//<e0>Parking Light Output Enable
//<o1>Parking Light Output                
//         <1=> *Constant      
//         <2=> Pulsed        
//         <3=> Off   
//</e>
#define FEAT_DEI_RS08_MIN           1
#define FEAT_DEI_RS08_MAX           3
#define FEAT_DEI_RS08_CHECK         TRUE

#if FEAT_DEI_RS08_CHECK==TRUE
    #if FEAT_DEI_RS08<FEAT_DEI_RS08_MIN
        #error FEAT_DEI_RS08 MIN ERROR
    #endif

    #if FEAT_DEI_RS08>FEAT_DEI_RS08_MAX
        #error FEAT_DEI_RS08 MAX ERROR
    #endif
#endif



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
#define FEAT_DEI_RS09_MIN           0
#define FEAT_DEI_RS09_MAX           10
#define FEAT_DEI_RS09_CHECK         TRUE

#if FEAT_DEI_RS09_CHECK==TRUE
    #if FEAT_DEI_RS09<FEAT_DEI_RS09_MIN
        #error FEAT_DEI_RS09 MIN ERROR
    #endif

    #if FEAT_DEI_RS09>FEAT_DEI_RS09_MAX
        #error FEAT_DEI_RS09 MAX ERROR
    #endif
#endif


//<e0>Remote Start Runtime Enable
//<o1> Remote Start Runtime( minutes ) <1-60:1>
//</e>
#define FEAT_DEI_RS10_MIN           1
#define FEAT_DEI_RS10_MAX           60
#define FEAT_DEI_RS10_CHECK         TRUE

#if FEAT_DEI_RS10_CHECK==TRUE
    #if FEAT_DEI_RS10<FEAT_DEI_RS10_MIN
        #error FEAT_DEI_RS10 MIN ERROR
    #endif

    #if FEAT_DEI_RS10>FEAT_DEI_RS10_MAX
        #error FEAT_DEI_RS10 MAX ERROR
    #endif
#endif


//<e0>Smart Start/Timer Mode Runtime Enable
//<o1> Smart Start/Timer Mode Runtime( minutes ) <1-16:1>
//</e>
#define FEAT_DEI_RS11_MIN           1
#define FEAT_DEI_RS11_MAX           16
#define FEAT_DEI_RS11_CHECK         TRUE

#if FEAT_DEI_RS11_CHECK==TRUE
    #if FEAT_DEI_RS11<FEAT_DEI_RS11_MIN
        #error FEAT_DEI_RS11 MIN ERROR
    #endif

    #if FEAT_DEI_RS11>FEAT_DEI_RS11_MAX
        #error FEAT_DEI_RS11 MAX ERROR
    #endif
#endif


// <e0>Smart Start/Timer Mode Start Count Enable
//  <o1> Smart Start/Timer Mode Start Count
//         <1=> *1
//         <2=> 2
//         <3=> 3
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
#define FEAT_DEI_RS12_MIN           1
#define FEAT_DEI_RS12_MAX           14
#define FEAT_DEI_RS12_CHECK         TRUE

#if FEAT_DEI_RS12_CHECK==TRUE
    #if FEAT_DEI_RS12<FEAT_DEI_RS12_MIN
        #error FEAT_DEI_RS12 MIN ERROR
    #endif

    #if FEAT_DEI_RS12>FEAT_DEI_RS12_MAX
        #error FEAT_DEI_RS12 MAX ERROR
    #endif
#endif


//<e0>Timer Mode Start Interval Enable
//<o1> Timer Mode Start Interval( Hours )
//         <1=> 1
//         <2=> 2
//         <3=> 3
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
//         <14=> *24
//</e>
#define FEAT_DEI_RS13_MIN           1
#define FEAT_DEI_RS13_MAX           14
#define FEAT_DEI_RS13_CHECK         TRUE

#if FEAT_DEI_RS13_CHECK==TRUE
    #if FEAT_DEI_RS13<FEAT_DEI_RS13_MIN
        #error FEAT_DEI_RS13 MIN ERROR
    #endif

    #if FEAT_DEI_RS13>FEAT_DEI_RS13_MAX
        #error FEAT_DEI_RS13 MAX ERROR
    #endif
#endif

//<e0>Smart Start High Temp Enable
//<o1> SmartStart High Temperature Threshold
//        <1=>  *OFF
//        <2=>  +40  F
//        <3=>  +50  F
//        <4=>  +60  F
//        <5=>  +70  F
//        <6=>  +80  F
//        <7=>  +90  F
//        <8=>  +100 F
//        <9=>  +110 F
//        <10=> +120 F
//        <11=> +130 F
//</e>
#define FEAT_DEI_RS14_MIN           1
#define FEAT_DEI_RS14_MAX           11
#define FEAT_DEI_RS14_CHECK         TRUE

#if FEAT_DEI_RS14_CHECK==TRUE
    #if FEAT_DEI_RS14<FEAT_DEI_RS14_MIN
        #error FEAT_DEI_RS14 MIN ERROR
    #endif

    #if FEAT_DEI_RS14>FEAT_DEI_RS14_MAX
        #error FEAT_DEI_RS14 MAX ERROR
    #endif
#endif


//<e0>Smart Start Low Temp Enable
//<o1> SmartStart Low Temperature Threshold
//        <1=>  *OFF
//        <2=>  -20  F
//        <3=>  -10  F
//        <4=>  - 0  F
//        <5=>  +10  F
//        <6=>  +20  F
//        <7=>  +30  F
//        <8=>  +40  F
//        <9=>  +50  F
//        <10=> +60  F
//        <11=> +70  F
//</e>
#define FEAT_DEI_RS15_MIN           1
#define FEAT_DEI_RS15_MAX           11
#define FEAT_DEI_RS15_CHECK         TRUE

#if FEAT_DEI_RS15_CHECK==TRUE
    #if FEAT_DEI_RS15<FEAT_DEI_RS15_MIN
        #error FEAT_DEI_RS15 MIN ERROR
    #endif

    #if FEAT_DEI_RS15>FEAT_DEI_RS15_MAX
        #error FEAT_DEI_RS15 MAX ERROR
    #endif
#endif

//<e0>Smart Start Low Battery Enable
//<o1>SmartStart Low Battery Threshold (Volts)
//         <1=>   9.0              
//         <2=>   9.5
//         <3=>  10.0
//         <4=>  10.5
//         <5=> *11.0
//         <6=>  11.5
//         <7=>  12.0
//         <8=>  12.5  
//</e>
#define FEAT_DEI_RS16_MIN           1
#define FEAT_DEI_RS16_MAX           8
#define FEAT_DEI_RS16_CHECK         TRUE

#if FEAT_DEI_RS16_CHECK==TRUE
    #if FEAT_DEI_RS16<FEAT_DEI_RS16_MIN
        #error FEAT_DEI_RS16 MIN ERROR
    #endif

    #if FEAT_DEI_RS16>FEAT_DEI_RS16_MAX
        #error FEAT_DEI_RS16 MAX ERROR
    #endif
#endif


//<e0>Virtual tach Fine Tune Enable
//<e1.7> Virtual Tach Fine Tune( Miliseconds )
//    <o1.7> *Not Initialized           
//    <o1.0..6> Virtual Tach Fine Tune( Miliseconds ) <0-1000:50><#/50>
//  </e>
//</e>
#define FEAT_DEI_RS17_MIN           1
#define FEAT_DEI_RS17_MAX           1
#define FEAT_DEI_RS17_CHECK         FALSE

#if FEAT_DEI_RS17_CHECK==TRUE
    #if FEAT_DEI_RS17<FEAT_DEI_RS17_MIN
        #error FEAT_DEI_RS17 MIN ERROR
    #endif

    #if FEAT_DEI_RS17>FEAT_DEI_RS17_MAX
        #error FEAT_DEI_RS17 MAX ERROR
    #endif
#endif

//<e0>Remote Start SafeLock Enable
//<o1>Remote Start SafeLock
//         <1=> *Off
//         <2=>  Safe Lock On
//         <3=>  Smart Safe Lock On
//</e>
#define FEAT_DEI_RS18_MIN           1
#define FEAT_DEI_RS18_MAX           3
#define FEAT_DEI_RS18_CHECK         TRUE

#if FEAT_DEI_RS18_CHECK==TRUE
    #if FEAT_DEI_RS18<FEAT_DEI_RS18_MIN
        #error FEAT_DEI_RS18 MIN ERROR
    #endif

    #if FEAT_DEI_RS18>FEAT_DEI_RS18_MAX
        #error FEAT_DEI_RS18 MAX ERROR
    #endif
#endif


//<e0>Tachometer Input Enable
//<o1>Tachometer Input option
//         <1=> *Tachometer
//         <2=>  Speed Sensor(VSS)
//</e>
#define FEAT_DEI_RS19_MIN           1
#define FEAT_DEI_RS19_MAX           2
#define FEAT_DEI_RS19_CHECK         TRUE

#if FEAT_DEI_RS19_CHECK==TRUE
    #if FEAT_DEI_RS19<FEAT_DEI_RS19_MIN
        #error FEAT_DEI_RS19 MIN ERROR
    #endif

    #if FEAT_DEI_RS19>FEAT_DEI_RS19_MAX
        #error FEAT_DEI_RS19 MAX ERROR
    #endif
#endif


//<e0>Idle/Pitstop Mode Enable
//<o1>Idle/Pitstop Mode
//         <1=> *On
//         <2=>  Off
//</e>
#define FEAT_DEI_RS20_MIN           1
#define FEAT_DEI_RS20_MAX           2
#define FEAT_DEI_RS20_CHECK         TRUE

#if FEAT_DEI_RS20_CHECK==TRUE
    #if FEAT_DEI_RS20<FEAT_DEI_RS20_MIN
        #error FEAT_DEI_RS20 MIN ERROR
    #endif

    #if FEAT_DEI_RS20>FEAT_DEI_RS20_MAX
        #error FEAT_DEI_RS20 MAX ERROR
    #endif
#endif


//<e0>TakeOver Option Enable
//<o1>TakeOver Option
//         <1=> *Enable
//         <2=>  Disable with Door Open
//         <3=>  Disable with Unlock command
//</e>
#define FEAT_DEI_RS21_MIN           1
#define FEAT_DEI_RS21_MAX           3
#define FEAT_DEI_RS21_CHECK         TRUE

#if FEAT_DEI_RS21_CHECK==TRUE
    #if FEAT_DEI_RS21<FEAT_DEI_RS21_MIN
        #error FEAT_DEI_RS21 MIN ERROR
    #endif

    #if FEAT_DEI_RS21>FEAT_DEI_RS21_MAX
        #error FEAT_DEI_RS21 MAX ERROR
    #endif
#endif


//<e0>OEM Remote Start Activation Option Enable
//<o1>OEM Remote Start Activation Option
//         <1=> *Disable
//         <2=>  Enable with 3x Locks
//         <3=>  Enable with Lock then Trunk
//         <4=>  Enable with Lock, Unlock, then Lock
//</e>
#define FEAT_DEI_RS22_MIN           1
#define FEAT_DEI_RS22_MAX           4
#define FEAT_DEI_RS22_CHECK         TRUE

#if FEAT_DEI_RS22_CHECK==TRUE
    #if FEAT_DEI_RS22<FEAT_DEI_RS22_MIN
        #error FEAT_DEI_RS22 MIN ERROR
    #endif

    #if FEAT_DEI_RS22>FEAT_DEI_RS22_MAX
        #error FEAT_DEI_RS22 MAX ERROR
    #endif
#endif

//<e0>Manual Transmission Initialization Enable
//<o1>Manual Transmission Initialization 
//         <1=> *Remote
//         <2=>  2x Parking Brake
//         <3=>  1x Parking Brake
//</e>
#define FEAT_DEI_RS23_MIN           1
#define FEAT_DEI_RS23_MAX           3
#define FEAT_DEI_RS23_CHECK         TRUE

#if FEAT_DEI_RS23_CHECK==TRUE
    #if FEAT_DEI_RS23<FEAT_DEI_RS23_MIN
        #error FEAT_DEI_RS23 MIN ERROR
    #endif

    #if FEAT_DEI_RS23>FEAT_DEI_RS23_MAX
        #error FEAT_DEI_RS23 MAX ERROR
    #endif
#endif


//<e0>Manual Transmission Completion Enable
//<o1>Manual Transmission Initialization 
//         <1=> *Door closed & remote
//         <2=>  Door closed with passive locking
//         <3=>  Door closed
//</e>
#define FEAT_DEI_RS24_MIN           1
#define FEAT_DEI_RS24_MAX           3
#define FEAT_DEI_RS24_CHECK         TRUE

#if FEAT_DEI_RS24_CHECK==TRUE
    #if FEAT_DEI_RS24<FEAT_DEI_RS24_MIN
        #error FEAT_DEI_RS24 MIN ERROR
    #endif

    #if FEAT_DEI_RS24>FEAT_DEI_RS24_MAX
        #error FEAT_DEI_RS24 MAX ERROR
    #endif
#endif


//<e0>Duration of Pulse After RS Enable
//<o1>Duration of Pulse After RS
//    <1=>  250 milliseconds
//    <2=> *500 milliseconds
//    <3=>  1000 milliseconds

//</e>
#define FEAT_DEI_RS25_MIN    1
#define FEAT_DEI_RS25_MAX    3
#define FEAT_DEI_RS25_CHECK  TRUE

#if FEAT_DEI_RS25_CHECK==TRUE
    #if FEAT_DEI_RS25 < FEAT_DEI_RS25_MIN
        #error FEAT_DEI_RS25 MIN ERROR
    #endif

    #if FEAT_DEI_RS25 > FEAT_DEI_RS25_MAX
        #error FEAT_DEI_RS25 MAX ERROR
    #endif
#endif



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
#define FEAT_DEI_RS26_MIN    1
#define FEAT_DEI_RS26_MAX    10
#define FEAT_DEI_RS26_CHECK  TRUE

#if FEAT_DEI_RS26_CHECK==TRUE
    #if FEAT_DEI_RS26 < FEAT_DEI_RS26_MIN
        #error FEAT_DEI_RS26 MIN ERROR
    #endif

    #if FEAT_DEI_RS26 > FEAT_DEI_RS26_MAX
        #error FEAT_DEI_RS26 MAX ERROR
    #endif
#endif


// </h>

//--------------------------------------------------------------------------//
// <h>  System Features
//--------------------------------------------------------------------------//

//<e0>Door lock pulses Enable
//<o1> DoorLock Pulses 
//         <1=>*Single                
//         <2=> Double Unlock Only   
//         <3=>Double Lock Only     
//         <4=>Double Lock & Unlock 
//</e>
#define FEAT_DEI_SYS02_MIN              1
#define FEAT_DEI_SYS02_MAX              4
#define FEAT_DEI_SYS02_CHECK            TRUE

#if FEAT_DEI_SYS02_CHECK==TRUE
    #if FEAT_DEI_SYS02<FEAT_DEI_SYS02_MIN
        #error FEAT_DEI_SYS02 MIN ERROR
    #endif

    #if FEAT_DEI_SYS02>FEAT_DEI_SYS02_MAX
        #error FEAT_DEI_SYS02 MAX ERROR
    #endif
#endif

//<e0>Door Lock output Duration Enable
//<o1> DoorLock Output Duration( seconds ) 
//      <1=>*0.8sec     
//      <2=>3.5 sec  
//      <3=>0.4 sec  
//</e>
#define FEAT_DEI_SYS03_MIN              1
#define FEAT_DEI_SYS03_MAX              3
#define FEAT_DEI_SYS03_CHECK            TRUE

#if FEAT_DEI_SYS03_CHECK==TRUE
    #if FEAT_DEI_SYS03<FEAT_DEI_SYS03_MIN
        #error FEAT_DEI_SYS03 MIN ERROR
    #endif

    #if FEAT_DEI_SYS03>FEAT_DEI_SYS03_MAX
        #error FEAT_DEI_SYS03 MAX ERROR
    #endif
#endif


//<e0>Comfort Closure Enable
//<o1>Comfort Closure 
//      <1=>*No Comfort Closure        
//      <2=>Comfort Closure 1       
//      <3=>Comfort Closure 2       
//</e>
#define FEAT_DEI_SYS05_MIN              1
#define FEAT_DEI_SYS05_MAX              3
#define FEAT_DEI_SYS05_CHECK            TRUE


#if FEAT_DEI_SYS05_CHECK==TRUE
    #if FEAT_DEI_SYS05<FEAT_DEI_SYS05_MIN
        #error FEAT_DEI_SYS05 MIN ERROR
    #endif

    #if FEAT_DEI_SYS05>FEAT_DEI_SYS05_MAX
        #error FEAT_DEI_SYS05 MAX ERROR
    #endif
#endif


//<e0>Hood Trigger Type Enable
//<o1>Hood Trigger Type  
//     <1=>*Normally Open       
//     <2=>Normally closed   
//</e>
#define FEAT_DEI_SYS06_MIN              1
#define FEAT_DEI_SYS06_MAX              2
#define FEAT_DEI_SYS06_CHECK            TRUE


#if FEAT_DEI_SYS06_CHECK==TRUE
    #if FEAT_DEI_SYS06<FEAT_DEI_SYS06_MIN
        #error FEAT_DEI_SYS06 MIN ERROR
    #endif

    #if FEAT_DEI_SYS06>FEAT_DEI_SYS06_MAX
        #error FEAT_DEI_SYS06 MAX ERROR
    #endif
#endif


//<e0>Ignition Controlled Domelight Enable
//<o1>Ignition Controlled Domelight 
//         <1=>Off 
//         <2=>*On
//</e>
#define FEAT_DEI_SYS07_MIN              1
#define FEAT_DEI_SYS07_MAX              2
#define FEAT_DEI_SYS07_CHECK            TRUE

#if FEAT_DEI_SYS07_CHECK==TRUE
    #if FEAT_DEI_SYS07<FEAT_DEI_SYS07_MIN
        #error FEAT_DEI_SYS07 MIN ERROR
    #endif

    #if FEAT_DEI_SYS07>FEAT_DEI_SYS07_MAX
        #error FEAT_DEI_SYS07 MAX ERROR
    #endif
#endif

//<e0>OEM Alarm Disarm Output  Enable
//<o1>OEM Alarm Disarm Output  
//     <1=>*With Unlock       
//     <2=> Before Unlock     
//     <3=> Remote Start Only 
//</e>
#define FEAT_DEI_SYS09_MIN              1
#define FEAT_DEI_SYS09_MAX              3
#define FEAT_DEI_SYS09_CHECK            TRUE

#if FEAT_DEI_SYS09_CHECK==TRUE
    #if FEAT_DEI_SYS09<FEAT_DEI_SYS09_MIN
        #error FEAT_DEI_SYS09 MIN ERROR
    #endif

    #if FEAT_DEI_SYS09>FEAT_DEI_SYS09_MAX
        #error FEAT_DEI_SYS09 MAX ERROR
    #endif
#endif
 
 
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
#define FEAT_DEI_SYS11_MIN              1
#define FEAT_DEI_SYS11_MAX              11
#define FEAT_DEI_SYS11_CHECK            TRUE

#if FEAT_DEI_SYS11_CHECK==TRUE
    #if FEAT_DEI_SYS11<FEAT_DEI_SYS11_MIN
        #error FEAT_DEI_SYS11 MIN ERROR
    #endif

    #if FEAT_DEI_SYS11>FEAT_DEI_SYS11_MAX
        #error FEAT_DEI_SYS11 MAX ERROR
    #endif
#endif


//<e0>Aux 1 Output Type Enable
//<o1>Aux 1 Output type        
//    <1=>*Validity         
//    <2=> Latch            
//    <3=> Latch/ reset/ign 
//    <4=> Timed   
//    <5=> Off
//</e>
#define FEAT_DEI_SYS12_MIN              1
#define FEAT_DEI_SYS12_MAX              5
#define FEAT_DEI_SYS12_CHECK            TRUE

#if FEAT_DEI_SYS12_CHECK==TRUE
    #if FEAT_DEI_SYS12<FEAT_DEI_SYS12_MIN
        #error FEAT_DEI_SYS12 MIN ERROR
    #endif

    #if FEAT_DEI_SYS12>FEAT_DEI_SYS12_MAX
        #error FEAT_DEI_SYS12 MAX ERROR
    #endif
#endif
 
//<e0>Aux 1 Linking Enable
//<o1>Aux 1 Linking            
//    <1=>*No Linking                
//    <2=> Link to Arm               
//    <3=> Link to Disarm            
//    <4=> Link to Arm / Disarm       
//    <5=> Link to Remote Start Only
//</e>
#define FEAT_DEI_SYS13_MIN              1
#define FEAT_DEI_SYS13_MAX              5
#define FEAT_DEI_SYS13_CHECK            TRUE

#if FEAT_DEI_SYS13_CHECK==TRUE
    #if FEAT_DEI_SYS13<FEAT_DEI_SYS13_MIN
        #error FEAT_DEI_SYS13 MIN ERROR
    #endif

    #if FEAT_DEI_SYS13>FEAT_DEI_SYS13_MAX
        #error FEAT_DEI_SYS13 MAX ERROR
    #endif
#endif
 
//<e0>Aux 1 Timed Output Enable
//<o1>AUX1 Timed Output (Seconds)  <1-90:1> 
//</e>
#define FEAT_DEI_SYS14_MIN              1
#define FEAT_DEI_SYS14_MAX              90
#define FEAT_DEI_SYS14_CHECK            TRUE

#if FEAT_DEI_SYS14_CHECK==TRUE
    #if FEAT_DEI_SYS14<FEAT_DEI_SYS14_MIN
        #error FEAT_DEI_SYS14 MIN ERROR
    #endif

    #if FEAT_DEI_SYS14>FEAT_DEI_SYS14_MAX
        #error FEAT_DEI_SYS14 MAX ERROR
    #endif
#endif

//<e0>Aux 2 Icon Enable
//<o1> AUX 2 Icon 
//    <1 => Trunk
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
#define FEAT_DEI_SYS15_MIN              1
#define FEAT_DEI_SYS15_MAX              11
#define FEAT_DEI_SYS15_CHECK            TRUE

#if FEAT_DEI_SYS15_CHECK==TRUE
    #if FEAT_DEI_SYS15<FEAT_DEI_SYS15_MIN
        #error FEAT_DEI_SYS15 MIN ERROR
    #endif

    #if FEAT_DEI_SYS15>FEAT_DEI_SYS15_MAX
        #error FEAT_DEI_SYS15 MAX ERROR
    #endif
#endif

//<e0>Aux 2 Output Type Enable
//<o1> Aux 2 Output type        
//    <1=>*Validity       
//    <2=> Latch           
//    <3=> Latch/ reset/ign
//    <4=> Timed           
//    <5=> Off
//</e>
#define FEAT_DEI_SYS16_MIN              1
#define FEAT_DEI_SYS16_MAX              5
#define FEAT_DEI_SYS16_CHECK            TRUE

#if FEAT_DEI_SYS16_CHECK==TRUE
    #if FEAT_DEI_SYS16<FEAT_DEI_SYS16_MIN
        #error FEAT_DEI_SYS16 MIN ERROR
    #endif

    #if FEAT_DEI_SYS16>FEAT_DEI_SYS16_MAX
        #error FEAT_DEI_SYS16 MAX ERROR
    #endif
#endif
 

//<e0>Aux 2 Linking Enable
//<o1> Aux 2 Linking            
//    <1=>*No Linking                
//    <2=> Link to Arm               
//    <3=> Link to Disarm            
//    <4=> Link to Arm / Disarm       
//    <5=> Link to Remote Start Only
//</e>
#define FEAT_DEI_SYS17_MIN              1
#define FEAT_DEI_SYS17_MAX              5
#define FEAT_DEI_SYS17_CHECK            TRUE

#if FEAT_DEI_SYS17_CHECK==TRUE
    #if FEAT_DEI_SYS17<FEAT_DEI_SYS17_MIN
        #error FEAT_DEI_SYS17 MIN ERROR
    #endif

    #if FEAT_DEI_SYS17>FEAT_DEI_SYS17_MAX
        #error FEAT_DEI_SYS17 MAX ERROR
    #endif
#endif


//<e0>Aux 2 Timed Output Enable
//<o1> AUX2 Timed Output (Seconds)  <1-90:1> 
//</e>
#define FEAT_DEI_SYS18_MIN              1
#define FEAT_DEI_SYS18_MAX              90
#define FEAT_DEI_SYS18_CHECK            TRUE

#if FEAT_DEI_SYS18_CHECK==TRUE
    #if FEAT_DEI_SYS18<FEAT_DEI_SYS18_MIN
        #error FEAT_DEI_SYS18 MIN ERROR
    #endif

    #if FEAT_DEI_SYS18>FEAT_DEI_SYS18_MAX
        #error FEAT_DEI_SYS18 MAX ERROR
    #endif
#endif
 

//<e0>Aux 3 Icon Enable
//<o1> AUX 3  Icon
//    <1 => Trunk
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
#define FEAT_DEI_SYS19_MIN              1
#define FEAT_DEI_SYS19_MAX              11
#define FEAT_DEI_SYS19_CHECK            TRUE

#if FEAT_DEI_SYS19_CHECK==TRUE
    #if FEAT_DEI_SYS19<FEAT_DEI_SYS19_MIN
        #error FEAT_DEI_SYS19 MIN ERROR
    #endif

    #if FEAT_DEI_SYS19>FEAT_DEI_SYS19_MAX
        #error FEAT_DEI_SYS19 MAX ERROR
    #endif
#endif

//<e0>Aux 3 Output Type Enable
//<o1> Aux 3 Output type        
//    <1=>*Validity       
//    <2=> Latch           
//    <3=> Latch/ reset/ign
//    <4=> Timed           
//    <5=> Off
//</e>
#define FEAT_DEI_SYS20_MIN              1
#define FEAT_DEI_SYS20_MAX              5
#define FEAT_DEI_SYS20_CHECK            TRUE
 
#if FEAT_DEI_SYS20_CHECK==TRUE
    #if FEAT_DEI_SYS20<FEAT_DEI_SYS20_MIN
        #error FEAT_DEI_SYS20 MIN ERROR
    #endif

    #if FEAT_DEI_SYS20>FEAT_DEI_SYS20_MAX
        #error FEAT_DEI_SYS20 MAX ERROR
    #endif
#endif

//<e0>Aux 3 Linking Enable
//<o1> Aux 3 Linking            
//    <1=>*No Linking                
//    <2=> Link to Arm               
//    <3=> Link to Disarm            
//    <4=> Link to Arm / Disarm       
//    <5=> Link to Remote Start Only
//</e>
#define FEAT_DEI_SYS21_MIN              1
#define FEAT_DEI_SYS21_MAX              5
#define FEAT_DEI_SYS21_CHECK            TRUE

#if FEAT_DEI_SYS21_CHECK==TRUE
    #if FEAT_DEI_SYS21<FEAT_DEI_SYS21_MIN
        #error FEAT_DEI_SYS21 MIN ERROR
    #endif

    #if FEAT_DEI_SYS21>FEAT_DEI_SYS21_MAX
        #error FEAT_DEI_SYS21 MAX ERROR
    #endif
#endif

//<e0>Aux 3 Timed Output Enable
//<o1> AUX3 Timed Output (Seconds)  <1-90:1> 
//</e>
#define FEAT_DEI_SYS22_MIN              1
#define FEAT_DEI_SYS22_MAX              90
#define FEAT_DEI_SYS22_CHECK            TRUE

#if FEAT_DEI_SYS22_CHECK==TRUE
    #if FEAT_DEI_SYS22<FEAT_DEI_SYS22_MIN
        #error FEAT_DEI_SYS22 MIN ERROR
    #endif

    #if FEAT_DEI_SYS22>FEAT_DEI_SYS22_MAX
        #error FEAT_DEI_SYS22 MAX ERROR
    #endif
#endif


//<e0>Aux 4 Output Type Enable
//<o1> Aux 4 Output type        
//    <1=>*Validity       
//    <2=> Latch           
//    <3=> Latch/ reset/ign
//    <4=> Timed           
//    <5=> Off
//</e>
#define FEAT_DEI_SYS23_MIN              1
#define FEAT_DEI_SYS23_MAX              5
#define FEAT_DEI_SYS23_CHECK            TRUE

#if FEAT_DEI_SYS23_CHECK==TRUE
    #if FEAT_DEI_SYS23<FEAT_DEI_SYS23_MIN
        #error FEAT_DEI_SYS23 MIN ERROR
    #endif

    #if FEAT_DEI_SYS23>FEAT_DEI_SYS23_MAX
        #error FEAT_DEI_SYS23 MAX ERROR
    #endif
#endif

 
//<e0>Aux 4 Linking Enable
//<o1> Aux 4 Linking            
//    <1=>*No Linking                
//    <2=> Link to Arm               
//    <3=> Link to Disarm            
//    <4=> Link to Arm / Disarm       
//    <5=> Link to Remote Start Only
//</e>
#define FEAT_DEI_SYS24_MIN              1
#define FEAT_DEI_SYS24_MAX              5
#define FEAT_DEI_SYS24_CHECK            TRUE

#if FEAT_DEI_SYS24_CHECK==TRUE
    #if FEAT_DEI_SYS24<FEAT_DEI_SYS24_MIN
        #error FEAT_DEI_SYS24 MIN ERROR
    #endif

    #if FEAT_DEI_SYS24>FEAT_DEI_SYS24_MAX
        #error FEAT_DEI_SYS24 MAX ERROR
    #endif
#endif


//<e0>Aux 4 Timed Output Enable
//<o1> AUX4 Timed Output (Seconds)  <1-90:1> 
//</e>
#define FEAT_DEI_SYS25_MIN              1
#define FEAT_DEI_SYS25_MAX              90
#define FEAT_DEI_SYS25_CHECK            TRUE

#if FEAT_DEI_SYS25_CHECK==TRUE
    #if FEAT_DEI_SYS25<FEAT_DEI_SYS25_MIN
        #error FEAT_DEI_SYS25 MIN ERROR
    #endif

    #if FEAT_DEI_SYS25>FEAT_DEI_SYS25_MAX
        #error FEAT_DEI_SYS25 MAX ERROR
    #endif
#endif


//<e0>CH2 Output Type Enable
//<o1>CH2 Output type
//    <1=>*Validity       
//    <2=> Off
//</e>
#define FEAT_DEI_SYS26_MIN              1
#define FEAT_DEI_SYS26_MAX              2
#define FEAT_DEI_SYS26_CHECK            TRUE

#if FEAT_DEI_SYS26_CHECK==TRUE
    #if FEAT_DEI_SYS26<FEAT_DEI_SYS26_MIN
        #error FEAT_DEI_SYS26 MIN ERROR
    #endif

    #if FEAT_DEI_SYS26>FEAT_DEI_SYS26_MAX
        #error FEAT_DEI_SYS26 MAX ERROR
    #endif
#endif


//<e0>CH2 Icon Enable
//<o1>CH2 Icon 
//         <1 => Trunk
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
#define FEAT_DEI_SYS27_MIN              1
#define FEAT_DEI_SYS27_MAX              11
#define FEAT_DEI_SYS27_CHECK            TRUE

#if FEAT_DEI_SYS27_CHECK==TRUE
    #if FEAT_DEI_SYS27<FEAT_DEI_SYS27_MIN
        #error FEAT_DEI_SYS27 MIN ERROR
    #endif

    #if FEAT_DEI_SYS27>FEAT_DEI_SYS27_MAX
        #error FEAT_DEI_SYS27 MAX ERROR
    #endif
#endif

//<e0>Remote keypad unlocking Enable
//<o1>Remote keypad unlocking
//         <1 =>*Off
//         <2 => On
//</e>
#define FEAT_DEI_SYS28_MIN              1
#define FEAT_DEI_SYS28_MAX              2
#define FEAT_DEI_SYS28_CHECK            TRUE

#if FEAT_DEI_SYS28_CHECK==TRUE
    #if FEAT_DEI_SYS28<FEAT_DEI_SYS28_MIN
        #error FEAT_DEI_SYS28 MIN ERROR
    #endif

    #if FEAT_DEI_SYS28>FEAT_DEI_SYS28_MAX
        #error FEAT_DEI_SYS28 MAX ERROR
    #endif
#endif

//<e0>Transmitter Programming Enable
//<o1>Transmitter Programming
//         <1 =>*Unlocked
//         <2 => Locked
//</e>
//#define FEAT_DEI_SYS29_MIN              1
//#define FEAT_DEI_SYS29_MAX              2
//#define FEAT_DEI_SYS29_CHECK            TRUE

//#if FEAT_DEI_SYS29_CHECK==TRUE
//    #if FEAT_DEI_SYS29<FEAT_DEI_SYS29_MIN
//        #error FEAT_DEI_SYS29 MIN ERROR
//    #endif

//    #if FEAT_DEI_SYS29>FEAT_DEI_SYS29_MAX
//        #error FEAT_DEI_SYS29 MAX ERROR
//    #endif
//#endif

//<e0>Door Switch Type Option ENABLE
//<o1>Door Switch Type Option
//         <1 =>*Normally Open
//         <2 => Normally Close
//</e>
#define FEAT_DEI_SYS30_MIN              1
#define FEAT_DEI_SYS30_MAX              2
#define FEAT_DEI_SYS30_CHECK            TRUE

#if FEAT_DEI_SYS30_CHECK==TRUE
    #if FEAT_DEI_SYS30<FEAT_DEI_SYS30_MIN
        #error FEAT_DEI_SYS30 MIN ERROR
    #endif

    #if FEAT_DEI_SYS30>FEAT_DEI_SYS30_MAX
        #error FEAT_DEI_SYS30 MAX ERROR
    #endif
#endif

//<e0>Trunk Switch Type Option ENABLE
//<o1>Trunk Switch Type Option
//         <1 =>*Normally Open
//         <2 => Normally Close
//</e>
#define FEAT_DEI_SYS31_MIN              1
#define FEAT_DEI_SYS31_MAX              2
#define FEAT_DEI_SYS31_CHECK            TRUE

#if FEAT_DEI_SYS31_CHECK==TRUE
    #if FEAT_DEI_SYS31<FEAT_DEI_SYS31_MIN
        #error FEAT_DEI_SYS31 MIN ERROR
    #endif

    #if FEAT_DEI_SYS31>FEAT_DEI_SYS31_MAX
        #error FEAT_DEI_SYS31 MAX ERROR
    #endif
#endif

//<e0>Controlled Door Lock Option ENABLE
//<o1>Controlled Door Lock Option
//         <1 =>*Off
//         <2 => Ignition
//         <3 => RPM
//</e>
#define FEAT_DEI_SYS32_MIN              1
#define FEAT_DEI_SYS32_MAX              3
#define FEAT_DEI_SYS32_CHECK            TRUE

#if FEAT_DEI_SYS32_CHECK==TRUE
    #if FEAT_DEI_SYS32<FEAT_DEI_SYS32_MIN
        #error FEAT_DEI_SYS32 MIN ERROR
    #endif

    #if FEAT_DEI_SYS32>FEAT_DEI_SYS32_MAX
        #error FEAT_DEI_SYS32 MAX ERROR
    #endif
#endif

//<e0>Controlled Door Lock Function ENABLE
//<o1>Controlled Door Lock Function
//         <1 =>*Lock & Unlock
//         <2 => Lock only
//         <3 => Unlock only
//</e>
#define FEAT_DEI_SYS33_MIN              1
#define FEAT_DEI_SYS33_MAX              3
#define FEAT_DEI_SYS33_CHECK            TRUE

#if FEAT_DEI_SYS33_CHECK==TRUE
    #if FEAT_DEI_SYS33<FEAT_DEI_SYS33_MIN
        #error FEAT_DEI_SYS33 MIN ERROR
    #endif

    #if FEAT_DEI_SYS33>FEAT_DEI_SYS33_MAX
        #error FEAT_DEI_SYS33 MAX ERROR
    #endif
#endif

//<e0>Controlled Unlock All Doors Option ENABLE
//<o1>Controlled Unlock All Doors Option
//         <1 =>*Off
//         <2 => On
//</e>
#define FEAT_DEI_SYS34_MIN              1
#define FEAT_DEI_SYS34_MAX              2
#define FEAT_DEI_SYS34_CHECK            TRUE

#if FEAT_DEI_SYS34_CHECK==TRUE
    #if FEAT_DEI_SYS34<FEAT_DEI_SYS34_MIN
        #error FEAT_DEI_SYS34 MIN ERROR
    #endif

    #if FEAT_DEI_SYS34>FEAT_DEI_SYS34_MAX
        #error FEAT_DEI_SYS34 MAX ERROR
    #endif
#endif

//<e0>Driver Priority Unlocking Option ENABLE
//<o1>Driver Priority Unlocking Option
//         <1 =>*On
//         <2 => Off
//</e>
#define FEAT_DEI_SYS35_MIN              1
#define FEAT_DEI_SYS35_MAX              2
#define FEAT_DEI_SYS35_CHECK            TRUE

#if FEAT_DEI_SYS35_CHECK==TRUE
    #if FEAT_DEI_SYS35<FEAT_DEI_SYS35_MIN
        #error FEAT_DEI_SYS35 MIN ERROR
    #endif

    #if FEAT_DEI_SYS35>FEAT_DEI_SYS35_MAX
        #error FEAT_DEI_SYS35 MAX ERROR
    #endif
#endif

//<e0>Parking Light Confirmation Option ENABLE
//<o1>Parking Light Confirmation Option
//         <1 =>*On
//         <2 => Off
//</e>
#define FEAT_DEI_SYS36_MIN              1
#define FEAT_DEI_SYS36_MAX              2
#define FEAT_DEI_SYS36_CHECK            TRUE

#if FEAT_DEI_SYS36_CHECK==TRUE
    #if FEAT_DEI_SYS36<FEAT_DEI_SYS36_MIN
        #error FEAT_DEI_SYS36 MIN ERROR
    #endif

    #if FEAT_DEI_SYS36>FEAT_DEI_SYS36_MAX
        #error FEAT_DEI_SYS36 MAX ERROR
    #endif
#endif

//<e0>Antenna LED Flashing Option ENABLE
//<o1>Antenna LED Flashing Option
//         <1 =>*On
//         <2 => Off
//</e>
#define FEAT_DEI_SYS37_MIN              1
#define FEAT_DEI_SYS37_MAX              2
#define FEAT_DEI_SYS37_CHECK            TRUE

#if FEAT_DEI_SYS37_CHECK==TRUE
    #if FEAT_DEI_SYS37<FEAT_DEI_SYS37_MIN
        #error FEAT_DEI_SYS37 MIN ERROR
    #endif

    #if FEAT_DEI_SYS37>FEAT_DEI_SYS37_MAX
        #error FEAT_DEI_SYS37 MAX ERROR
    #endif
#endif


//<e0>Ignition Disarm Option ENABLE
//<o1>Ignition Disarm Option
//         <1 =>*Off
//         <2 => On
//</e>
#define FEAT_DEI_SYS38_MIN              1
#define FEAT_DEI_SYS38_MAX              2
#define FEAT_DEI_SYS38_CHECK            TRUE

#if FEAT_DEI_SYS38_CHECK==TRUE
    #if FEAT_DEI_SYS38<FEAT_DEI_SYS38_MIN
        #error FEAT_DEI_SYS38 MIN ERROR
    #endif

    #if FEAT_DEI_SYS38>FEAT_DEI_SYS38_MAX
        #error FEAT_DEI_SYS38 MAX ERROR
    #endif
#endif

//<e0>FOB Voltage Option ENABLE
//<o1>FOB Voltage Option
//         <1 =>*0 V
//         <2 => 1.5 V
//         <3 => 3.0 V
//         <4 => 4.5 V
//         <5 => 6.0 V
//</e>
#define FEAT_DEI_SYS39_MIN              1
#define FEAT_DEI_SYS39_MAX              5
#define FEAT_DEI_SYS39_CHECK            TRUE

#if FEAT_DEI_SYS39_CHECK==TRUE
    #if FEAT_DEI_SYS39<FEAT_DEI_SYS39_MIN
        #error FEAT_DEI_SYS39 MIN ERROR
    #endif

    #if FEAT_DEI_SYS39>FEAT_DEI_SYS38_MAX
        #error FEAT_DEI_SYS39 MAX ERROR
    #endif
#endif

// </h>

//--------------------------------------------------------------------------//
// <h>  Programmable Inputs/Outputs
//--------------------------------------------------------------------------//

//<e0>Output 1 option is visible on web
//<o1>Output1 (-)
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
//         <32=> Not Configured
//</e>
#define FEAT_DEI_SYS40_CHECK            TRUE

#if FEAT_DEI_SYS40_CHECK==TRUE
    #if FEAT_DEI_SYS40>FEAT_DEI_OUT_OPT_MAX
        #error FEAT_DEI_SYS40 MAX ERROR
    #endif
#endif

//<e0>Output 2 option is visible on web
//<o1>Output2 (-) 
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
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> Not Configured
//</e>
#define FEAT_DEI_SYS41_CHECK            TRUE

#if FEAT_DEI_SYS41_CHECK==TRUE
    #if FEAT_DEI_SYS41>FEAT_DEI_OUT_OPT_MAX
        #error FEAT_DEI_SYS41 MAX ERROR
    #endif
#endif

//<e0>Output 3 option is visible on web
//<o1>Output3 (-)
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
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> Not Configured
//</e>
#define FEAT_DEI_SYS42_CHECK            TRUE

#if FEAT_DEI_SYS42_CHECK==TRUE
    #if FEAT_DEI_SYS42>FEAT_DEI_OUT_OPT_MAX
        #error FEAT_DEI_SYS42 MAX ERROR
    #endif
#endif

//<e0>Output 4 option is visible on web
//<o1>Output4 (-)
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
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> Not Configured
//</e>
#define FEAT_DEI_SYS43_CHECK            TRUE

#if FEAT_DEI_SYS43_CHECK==TRUE
    #if FEAT_DEI_SYS43>FEAT_DEI_OUT_OPT_MAX
        #error FEAT_DEI_SYS43 MAX ERROR
    #endif
#endif

//<e0>Output 5 option is visible on web
//<o1>Output5 (-)
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
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> Not Configured
//</e>
#define FEAT_DEI_SYS44_CHECK            TRUE

#if FEAT_DEI_SYS44_CHECK==TRUE
    #if FEAT_DEI_SYS44>FEAT_DEI_OUT_OPT_MAX
        #error FEAT_DEI_SYS44 MAX ERROR
    #endif
#endif

//<e0>Output 6 option is visible on web
//<o1>Output6 (+) 
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
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> Not Configured
//</e>
#define FEAT_DEI_SYS45_CHECK            TRUE

#if FEAT_DEI_SYS45_CHECK==TRUE
    #if FEAT_DEI_SYS45>FEAT_DEI_OUT_OPT_MAX
        #error FEAT_DEI_SYS45 MAX ERROR
    #endif
#endif

//<e0>Output 7 option is visible on web
//<o1>Output7 (+/-)
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
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> Not Configured
//</e>
#define FEAT_DEI_SYS46_CHECK            TRUE

#if FEAT_DEI_SYS46_CHECK==TRUE
    #if FEAT_DEI_SYS46>FEAT_DEI_OUT_OPT_MAX
        #error FEAT_DEI_SYS46 MAX ERROR
    #endif
#endif

//<e0>Output 8 option is visible on web
//<o1>Output8 (+)
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
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> Not Configured
//</e>
#define FEAT_DEI_SYS47_CHECK            TRUE

#if FEAT_DEI_SYS47_CHECK==TRUE
    #if FEAT_DEI_SYS47>FEAT_DEI_OUT_OPT_MAX
        #error FEAT_DEI_SYS47 MAX ERROR
    #endif
#endif

//<e0>Output 9 option is visible on web
//<o1>Output9 (+) 
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
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> Not Configured
//</e>
#define FEAT_DEI_SYS48_CHECK            TRUE

#if FEAT_DEI_SYS48_CHECK==TRUE
    #if FEAT_DEI_SYS48>FEAT_DEI_OUT_OPT_MAX
        #error FEAT_DEI_SYS48 MAX ERROR
    #endif
#endif

//<e0>Output 10 option is visible on web
//<o1>Output10  (+) 
//         <17=> *Ignition
//</e>
#define FEAT_DEI_SYS49_CHECK            TRUE

#if FEAT_DEI_SYS49_CHECK==TRUE
    #if FEAT_DEI_SYS49>FEAT_DEI_OUT_OPT_MAX
        #error FEAT_DEI_SYS49 MAX ERROR
    #endif
#endif

//<e0>Output 11 option is visible on web
//<o1>Output11  (+)
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
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> Not Configured
//</e>
#define FEAT_DEI_SYS50_CHECK            TRUE

#if FEAT_DEI_SYS50_CHECK==TRUE
    #if FEAT_DEI_SYS50>FEAT_DEI_OUT_OPT_MAX
        #error FEAT_DEI_SYS50 MAX ERROR
    #endif
#endif

//<e0>Output 12 option is visible on web
//<o1>Output12  (+)
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
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> Not Configured
//</e>
#define FEAT_DEI_SYS51_CHECK            TRUE

#if FEAT_DEI_SYS51_CHECK==TRUE
    #if FEAT_DEI_SYS51>FEAT_DEI_OUT_OPT_MAX
        #error FEAT_DEI_SYS51 MAX ERROR
    #endif
#endif

//<e0>Output 13 option is visible on web
//<o1>Output13  (-)
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
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> Not Configured
//</e>
#define FEAT_DEI_SYS52_CHECK            TRUE

#if FEAT_DEI_SYS52_CHECK==TRUE
    #if FEAT_DEI_SYS52>FEAT_DEI_OUT_OPT_MAX
        #error FEAT_DEI_SYS52 MAX ERROR
    #endif
#endif

//<e0>Output 14 option is visible on web
//<o1>Output14  (-)
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
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> Not Configured
//</e>
#define FEAT_DEI_SYS53_CHECK            TRUE

#if FEAT_DEI_SYS53_CHECK==TRUE
    #if FEAT_DEI_SYS53>FEAT_DEI_OUT_OPT_MAX
        #error FEAT_DEI_SYS53 MAX ERROR
    #endif
#endif


//<e0>Relay 1 option is visible on web
//<o1>Relay1  
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
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> Not Configured
//</e>
#define FEAT_DEI_SYS54_CHECK            TRUE

#if FEAT_DEI_SYS54_CHECK==TRUE

    #if FEAT_DEI_SYS54>FEAT_DEI_OUT_OPT_MAX
        #error FEAT_DEI_SYS54 MAX ERROR
    #endif
#endif


//<e0>Relay 2 option is visible on web
//<o1>Relay2  
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
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> Not Configured
//</e>
#define FEAT_DEI_SYS55_CHECK            TRUE

#if FEAT_DEI_SYS55_CHECK==TRUE
    #if FEAT_DEI_SYS55>FEAT_DEI_OUT_OPT_MAX
        #error FEAT_DEI_SYS55 MAX ERROR
    #endif
#endif

//<e0>Relay 3 option is visible on web
//<o1>Relay3  
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
//         <28=> Status
//         <29=> Sensor Trigger Output
//         <30=> Pulse after RS shutdown
//         <31=> AG Starter-kill
//         <32=> Not Configured
//</e>
#define FEAT_DEI_SYS56_CHECK            TRUE

#if FEAT_DEI_SYS56_CHECK==TRUE

    #if FEAT_DEI_SYS56>FEAT_DEI_OUT_OPT_MAX
        #error FEAT_DEI_SYS56 MAX ERROR
    #endif
#endif


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
//         <12=> Sensor Trigger Iutput
//         <13=> Tamper
//</e>
#define FEAT_DEI_SYS57_CHECK            TRUE

#if FEAT_DEI_SYS57_CHECK==TRUE
    #if FEAT_DEI_SYS57>FEAT_DEI_INP_OPT_MAX
        #error FEAT_DEI_SYS57 MAX ERROR
    #endif
#endif


//<e0>Input Negative 3 option is visible on web
//<o1>Input Negative 3
//         <0=>  Not Configured
//         <1=>  Activation
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
//         <12=> Sensor Trigger Iutput
//         <13=> Tamper
//</e>
#define FEAT_DEI_SYS58_CHECK            TRUE

#if FEAT_DEI_SYS58_CHECK==TRUE
    #if FEAT_DEI_SYS58>FEAT_DEI_INP_OPT_MAX
        #error FEAT_DEI_SYS58 MAX ERROR
    #endif
#endif


//<e0>Input Negative 4 option is visible on web
//<o1>Input Negative 4
//         <0=>  Not Configured
//         <1=>  Activation
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
//         <12=> Sensor Trigger Iutput
//         <13=> Tamper
//</e>
#define FEAT_DEI_SYS59_CHECK            TRUE

#if FEAT_DEI_SYS59_CHECK==TRUE
    #if FEAT_DEI_SYS59>FEAT_DEI_INP_OPT_MAX
        #error FEAT_DEI_SYS59 MAX ERROR
    #endif
#endif


//<e0>Input Negative 5 option is visible on web
//<o1>Input Negative 5
//         <0=>  Not Configured
//         <1=>  Activation
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
//         <12=> Sensor Trigger Iutput
//         <13=> Tamper
//</e>
#define FEAT_DEI_SYS60_CHECK            TRUE

#if FEAT_DEI_SYS60_CHECK==TRUE
    #if FEAT_DEI_SYS60>FEAT_DEI_INP_OPT_MAX
        #error FEAT_DEI_SYS60 MAX ERROR
    #endif
#endif


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
//         <8=>  Trunk Trigger
//         <9=>  Valet Switch 
//         <10=> Ignition 
//         <11=> Custom Pos1 
//         <12=> Sensor Trigger Iutput
//         <13=> Tamper
//</e>
#define FEAT_DEI_SYS61_CHECK            TRUE

#if FEAT_DEI_SYS61_CHECK==TRUE
    #if FEAT_DEI_SYS61>FEAT_DEI_INP_OPT_MAX
        #error FEAT_DEI_SYS61 MAX ERROR
    #endif
#endif


//<e0>Input Positive 1 option is visible on web
//<o1>Input Positive 1
//         <0=>  Not Configured
//         <1=>  Activation
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
//         <12=> Sensor Trigger Iutput
//         <13=> Tamper
//</e>
#define FEAT_DEI_SYS62_CHECK            TRUE

#if FEAT_DEI_SYS62_CHECK==TRUE

    #if FEAT_DEI_SYS62>FEAT_DEI_INP_OPT_MAX
        #error FEAT_DEI_SYS62 MAX ERROR
    #endif
#endif


//<e0>Input Positive 2 option is visible on web
//<o1>Input Positive 2 
//         <0=>  Not Configured
//         <1=>  Activation
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
//         <12=> Sensor Trigger Iutput
//         <13=> Tamper
//</e>
#define FEAT_DEI_SYS63_CHECK            TRUE

#if FEAT_DEI_SYS63_CHECK==TRUE
    #if FEAT_DEI_SYS63>FEAT_DEI_INP_OPT_MAX
        #error FEAT_DEI_SYS63 MAX ERROR
    #endif
#endif


//<e0>Input Positive 3 option is visible on web
//<o1>Input Positive 3 

//         <1=>  *Ignition
//</e>
#define FEAT_DEI_SYS64_CHECK            TRUE

#if FEAT_DEI_SYS64_CHECK==TRUE
    #if FEAT_DEI_SYS64>FEAT_DEI_INP_OPT_MAX
        #error FEAT_DEI_SYS64 MAX ERROR
    #endif
#endif


//<e0>Input Positive Aux1 option is visible on web
//<o1>Input Positive Aux1 
//         <0=>  Not Configured
//         <1=>  Activation
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
//         <12=> Sensor Trigger Iutput
//         <13=> Tamper
//</e>
#define FEAT_DEI_SYS65_CHECK            TRUE

#if FEAT_DEI_SYS65_CHECK==TRUE
    #if FEAT_DEI_SYS65>FEAT_DEI_INP_OPT_MAX
        #error FEAT_DEI_SYS65 MAX ERROR
    #endif
#endif

// </h>

//--------------------------------------------------------------------------//
// <h>  Sense config
//--------------------------------------------------------------------------//
//<e0>Sense doors status Enable
//<o1>Sense doors status
//         <1=> *Enabled
//         <2=> Disabled
//</e>
#define FEAT_DEI_SYS69_MIN              1
#define FEAT_DEI_SYS69_MAX              2
#define FEAT_DEI_SYS69_CHECK            TRUE

#if FEAT_DEI_SYS69_CHECK==TRUE
    #if FEAT_DEI_SYS69<FEAT_DEI_SYS69_MIN
        #error FEAT_DEI_SYS69 MIN ERROR
    #endif

    #if FEAT_DEI_SYS69>FEAT_DEI_SYS69_MAX
        #error FEAT_DEI_SYS69 MAX ERROR
    #endif
#endif


//<e0>Sense ignition status Enable
//<o1>Sense ignition status
//         <1=> *Enabled
//         <2=> Disabled
//</e>

#define FEAT_DEI_SYS70_MIN              1
#define FEAT_DEI_SYS70_MAX              2
#define FEAT_DEI_SYS70_CHECK            TRUE

#if FEAT_DEI_SYS70_CHECK==TRUE
    #if FEAT_DEI_SYS70<FEAT_DEI_SYS70_MIN
        #error FEAT_DEI_SYS70 MIN ERROR
    #endif

    #if FEAT_DEI_SYS70>FEAT_DEI_SYS70_MAX
        #error FEAT_DEI_SYS70 MAX ERROR
    #endif
#endif


//<e0>Sense hood status Enable
//<o1>Sense hood status
//         <1=> *Enabled
//         <2=> Disabled
//</e>

#define FEAT_DEI_SYS71_MIN              1
#define FEAT_DEI_SYS71_MAX              2
#define FEAT_DEI_SYS71_CHECK            TRUE

#if FEAT_DEI_SYS71_CHECK==TRUE
    #if FEAT_DEI_SYS71<FEAT_DEI_SYS71_MIN
        #error FEAT_DEI_SYS71 MIN ERROR
    #endif

    #if FEAT_DEI_SYS71>FEAT_DEI_SYS71_MAX
        #error FEAT_DEI_SYS71 MAX ERROR
    #endif
#endif


//<e0> Sense trunk status Enable
// <o1>Sense trunk status
//         <1=> *Enabled
//         <2=> Disabled
//</e>

#define FEAT_DEI_SYS72_MIN              1
#define FEAT_DEI_SYS72_MAX              2
#define FEAT_DEI_SYS72_CHECK            TRUE

#if FEAT_DEI_SYS72_CHECK==TRUE
    #if FEAT_DEI_SYS72<FEAT_DEI_SYS72_MIN
        #error FEAT_DEI_SYS72 MIN ERROR
    #endif

    #if FEAT_DEI_SYS72>FEAT_DEI_SYS72_MAX
        #error FEAT_DEI_SYS72 MAX ERROR
    #endif
#endif


//<e0> Sense Handbrake Enable
//<o1>Sense handbrake status
//         <1=> *Enabled
//         <2=> Disabled
//</e>

#define FEAT_DEI_SYS73_MIN              1
#define FEAT_DEI_SYS73_MAX              2
#define FEAT_DEI_SYS73_CHECK            TRUE

#if FEAT_DEI_SYS73_CHECK==TRUE
    #if FEAT_DEI_SYS73<FEAT_DEI_SYS73_MIN
        #error FEAT_DEI_SYS73 MIN ERROR
    #endif

    #if FEAT_DEI_SYS73>FEAT_DEI_SYS73_MAX
        #error FEAT_DEI_SYS73 MAX ERROR
    #endif
#endif



//<e0>Sense Tacho Enable
//<o1>Sense tacho status
//         <1=> *Enabled
//         <2=> Disabled
//</e>

#define FEAT_DEI_SYS74_MIN              1
#define FEAT_DEI_SYS74_MAX              2
#define FEAT_DEI_SYS74_CHECK            TRUE

#if FEAT_DEI_SYS74_CHECK==TRUE
    #if FEAT_DEI_SYS74<FEAT_DEI_SYS74_MIN
        #error FEAT_DEI_SYS74 MIN ERROR
    #endif

    #if FEAT_DEI_SYS74>FEAT_DEI_SYS74_MAX
        #error FEAT_DEI_SYS74 MAX ERROR
    #endif
#endif

//<e0>Sense brake Enable
//<o1>Sense brake status
//         <1=> *Enabled
//         <2=> Disabled
//</e>

#define FEAT_DEI_SYS75_MIN              1
#define FEAT_DEI_SYS75_MAX              2
#define FEAT_DEI_SYS75_CHECK            TRUE

#if FEAT_DEI_SYS75_CHECK==TRUE
    #if FEAT_DEI_SYS75<FEAT_DEI_SYS75_MIN
        #error FEAT_DEI_SYS75 MIN ERROR
    #endif

    #if FEAT_DEI_SYS75>FEAT_DEI_SYS75_MAX
        #error FEAT_DEI_SYS75 MAX ERROR
    #endif
#endif

//<e0>Sense OEM LOCK Enable
//<o1>Sense oem lock
//         <1=> Disable             
//         <2=> *Enable
//</e>

#define FEAT_DEI_SYS77_MIN              1
#define FEAT_DEI_SYS77_MAX              2
#define FEAT_DEI_SYS77_CHECK            TRUE

#if FEAT_DEI_SYS77_CHECK==TRUE
    #if FEAT_DEI_SYS77<FEAT_DEI_SYS77_MIN
        #error FEAT_DEI_SYS77 MIN ERROR
    #endif

    #if FEAT_DEI_SYS77>FEAT_DEI_SYS77_MAX
        #error FEAT_DEI_SYS77 MAX ERROR
    #endif
#endif



//<e0>Sense OEM UNLOCK Enable
//<o1>Sense oem unlock
//         <1=> Disable
//         <2=> *Enable
//</e>

#define FEAT_DEI_SYS78_MIN              1
#define FEAT_DEI_SYS78_MAX              2
#define FEAT_DEI_SYS78_CHECK            TRUE

#if FEAT_DEI_SYS78_CHECK==TRUE
    #if FEAT_DEI_SYS78<FEAT_DEI_SYS78_MIN
        #error FEAT_DEI_SYS78 MIN ERROR
    #endif

    #if FEAT_DEI_SYS78>FEAT_DEI_SYS78_MAX
        #error FEAT_DEI_SYS78 MAX ERROR
    #endif
#endif


//<e0>Sense OEM TRUNK Enable
//<o1>Sense oem trunk
//         <1=> Disable            
//         <2=> *Enable
//</e>

#define FEAT_DEI_SYS79_MIN              1
#define FEAT_DEI_SYS79_MAX              2
#define FEAT_DEI_SYS79_CHECK            TRUE

#if FEAT_DEI_SYS79_CHECK==TRUE
    #if FEAT_DEI_SYS79<FEAT_DEI_SYS79_MIN
        #error FEAT_DEI_SYS79 MIN ERROR
    #endif

    #if FEAT_DEI_SYS79>FEAT_DEI_SYS79_MAX
        #error FEAT_DEI_SYS79 MAX ERROR
    #endif
#endif

//<e0>Engine Sensing Enable
//<o1>Engine Sensing
//         <1=> Disable            
//         <2=> *Enable
//</e>

#define FEAT_DEI_SYS80_MIN              1
#define FEAT_DEI_SYS80_MAX              2
#define FEAT_DEI_SYS80_CHECK            TRUE

#if FEAT_DEI_SYS80_CHECK==TRUE
    #if FEAT_DEI_SYS80<FEAT_DEI_SYS80_MIN
        #error FEAT_DEI_SYS80 MIN ERROR
    #endif

    #if FEAT_DEI_SYS80>FEAT_DEI_SYS80_MAX
        #error FEAT_DEI_SYS80 MAX ERROR
    #endif
#endif

//<e0>Arm Output Enable
//<o1>Arm Output
//         <1=> Disable            
//         <2=> *Enable
//</e>

#define FEAT_DEI_SYS81_MIN              1
#define FEAT_DEI_SYS81_MAX              2
#define FEAT_DEI_SYS81_CHECK            TRUE

#if FEAT_DEI_SYS81_CHECK==TRUE
    #if FEAT_DEI_SYS81<FEAT_DEI_SYS81_MIN
        #error FEAT_DEI_SYS81 MIN ERROR
    #endif

    #if FEAT_DEI_SYS81>FEAT_DEI_SYS81_MAX
        #error FEAT_DEI_SYS81 MAX ERROR
    #endif
#endif


//<e0>Disarm Output Enable
//<o1>Disarm Output
//         <1=> Disable            
//         <2=> *Enable
//</e>

#define FEAT_DEI_SYS82_MIN              1
#define FEAT_DEI_SYS82_MAX              2
#define FEAT_DEI_SYS82_CHECK            TRUE

#if FEAT_DEI_SYS82_CHECK==TRUE
    #if FEAT_DEI_SYS82<FEAT_DEI_SYS82_MIN
        #error FEAT_DEI_SYS82 MIN ERROR
    #endif

    #if FEAT_DEI_SYS82>FEAT_DEI_SYS82_MAX
        #error FEAT_DEI_SYS82 MAX ERROR
    #endif
#endif


//<e0>Lock Output Enable
//<o1>Lock Output
//         <1=> Disable            
//         <2=> *Enable
//</e>

#define FEAT_DEI_SYS83_MIN              1
#define FEAT_DEI_SYS83_MAX              2
#define FEAT_DEI_SYS83_CHECK            TRUE

#if FEAT_DEI_SYS83_CHECK==TRUE
    #if FEAT_DEI_SYS83<FEAT_DEI_SYS83_MIN
        #error FEAT_DEI_SYS83 MIN ERROR
    #endif

    #if FEAT_DEI_SYS83>FEAT_DEI_SYS83_MAX
        #error FEAT_DEI_SYS83 MAX ERROR
    #endif
#endif


//<e0>Parking Lights Output Enable
//<o1>Parking Lights Output
//         <1=> Disable            
//         <2=> *Enable
//</e>

#define FEAT_DEI_SYS84_MIN              1
#define FEAT_DEI_SYS84_MAX              2
#define FEAT_DEI_SYS84_CHECK            TRUE

#if FEAT_DEI_SYS84_CHECK==TRUE
    #if FEAT_DEI_SYS84<FEAT_DEI_SYS84_MIN
        #error FEAT_DEI_SYS84 MIN ERROR
    #endif

    #if FEAT_DEI_SYS84>FEAT_DEI_SYS84_MAX
        #error FEAT_DEI_SYS84 MAX ERROR
    #endif
#endif


//<e0>RAP Output Enable
//<o1>RAP Output
//         <1=> Disable            
//         <2=> *Enable
//</e>

#define FEAT_DEI_SYS85_MIN              1
#define FEAT_DEI_SYS85_MAX              2
#define FEAT_DEI_SYS85_CHECK            TRUE

#if FEAT_DEI_SYS85_CHECK==TRUE
    #if FEAT_DEI_SYS85<FEAT_DEI_SYS85_MIN
        #error FEAT_DEI_SYS85 MIN ERROR
    #endif

    #if FEAT_DEI_SYS85>FEAT_DEI_SYS85_MAX
        #error FEAT_DEI_SYS85 MAX ERROR
    #endif
#endif


//<e0>Trunk Output Enable
//<o1>Trunk Output
//         <1=> Disable            
//         <2=> *Enable
//</e>

#define FEAT_DEI_SYS86_MIN              1
#define FEAT_DEI_SYS86_MAX              2
#define FEAT_DEI_SYS86_CHECK            TRUE

#if FEAT_DEI_SYS86_CHECK==TRUE
    #if FEAT_DEI_SYS86<FEAT_DEI_SYS86_MIN
        #error FEAT_DEI_SYS86 MIN ERROR
    #endif

    #if FEAT_DEI_SYS86>FEAT_DEI_SYS86_MAX
        #error FEAT_DEI_SYS86 MAX ERROR
    #endif
#endif


//<e0>Unlock Output Enable
//<o1>Unlock Output
//         <1=> Disable            
//         <2=> *Enable
//</e>

#define FEAT_DEI_SYS87_MIN              1
#define FEAT_DEI_SYS87_MAX              2
#define FEAT_DEI_SYS87_CHECK            TRUE

#if FEAT_DEI_SYS87_CHECK==TRUE
    #if FEAT_DEI_SYS87<FEAT_DEI_SYS87_MIN
        #error FEAT_DEI_SYS87 MIN ERROR
    #endif

    #if FEAT_DEI_SYS87>FEAT_DEI_SYS87_MAX
        #error FEAT_DEI_SYS87 MAX ERROR
    #endif
#endif


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

#define FEAT_DEI_SYS88_MIN              1
#define FEAT_DEI_SYS88_MAX              5
#define FEAT_DEI_SYS88_CHECK            FALSE

#if FEAT_DEI_SYS88_CHECK==TRUE
    #if FEAT_DEI_SYS88<FEAT_DEI_SYS88_MIN
        #error FEAT_DEI_SYS88 MIN ERROR
    #endif

    #if FEAT_DEI_SYS88>FEAT_DEI_SYS88_MAX
        #error FEAT_DEI_SYS88 MAX ERROR
    #endif
#endif


//<e0>Feature Custom 01 Enable
//<o1>Feature Custom 01
//         <1=> *Option1
//         <2=>  Option2
//         <3=>  Option3
//         <4=>  Option4
//         <5=>  Option5 
//</e>
#define FEAT_DEI_SYS89_MIN              1
#define FEAT_DEI_SYS89_MAX              5
#define FEAT_DEI_SYS89_CHECK            FALSE

#if FEAT_DEI_SYS89_CHECK==TRUE
    #if FEAT_DEI_SYS89<FEAT_DEI_SYS89_MIN
        #error FEAT_DEI_SYS89 MIN ERROR
    #endif

    #if FEAT_DEI_SYS89>FEAT_DEI_SYS89_MAX
        #error FEAT_DEI_SYS89 MAX ERROR
    #endif
#endif



//<e0>Feature Trunk Programming Output
//<o1>Trunk Output type
//         <1=> *Constant Output
//         <2=>  1-sec Pulse Output
//</e>

#define FEAT_DEI_SYS90_MIN              1
#define FEAT_DEI_SYS90_MAX              2
#define FEAT_DEI_SYS90_CHECK            FALSE

#if FEAT_DEI_SYS90_CHECK==TRUE
    #if FEAT_DEI_SYS90<FEAT_DEI_SYS90_MIN
        #error FEAT_DEI_SYS90 MIN ERROR
    #endif

    #if FEAT_DEI_SYS90>FEAT_DEI_SYS90_MAX
        #error FEAT_DEI_SYS90 MAX ERROR
    #endif
#endif


//<e0>Feature Custom 03 Enable
//<o1>Feature Custom 03
//         <1=> *Option1
//         <2=>  Option2
//         <3=>  Option3
//         <4=>  Option4
//         <5=>  Option5 
//</e>

#define FEAT_DEI_SYS91_MIN              1
#define FEAT_DEI_SYS91_MAX              5
#define FEAT_DEI_SYS91_CHECK            FALSE

#if FEAT_DEI_SYS91_CHECK==TRUE
    #if FEAT_DEI_SYS91<FEAT_DEI_SYS91_MIN
        #error FEAT_DEI_SYS91 MIN ERROR
    #endif

    #if FEAT_DEI_SYS91>FEAT_DEI_SYS91_MAX
        #error FEAT_DEI_SYS91 MAX ERROR
    #endif
#endif


//<e0>Feature Custom 04 Enable
//<o1>Feature Custom 04
//         <1=> *Option1
//         <2=>  Option2
//         <3=>  Option3
//         <4=>  Option4
//         <5=>  Option5 
//</e>

#define FEAT_DEI_SYS92_MIN              1
#define FEAT_DEI_SYS92_MAX              5
#define FEAT_DEI_SYS92_CHECK            FALSE

#if FEAT_DEI_SYS92_CHECK==TRUE
    #if FEAT_DEI_SYS92<FEAT_DEI_SYS92_MIN
        #error FEAT_DEI_SYS92 MIN ERROR
    #endif

    #if FEAT_DEI_SYS92>FEAT_DEI_SYS92_MAX
        #error FEAT_DEI_SYS92 MAX ERROR
    #endif
#endif


//<e0>Feature Custom 05 Enable
//<o1>Feature Custom 05
//         <1=> *Option1
//         <2=>  Option2
//         <3=>  Option3
//         <4=>  Option4
//         <5=>  Option5 
//</e>

#define FEAT_DEI_SYS93_MIN              1
#define FEAT_DEI_SYS93_MAX              5
#define FEAT_DEI_SYS93_CHECK            FALSE

#if FEAT_DEI_SYS93_CHECK==TRUE
    #if FEAT_DEI_SYS93<FEAT_DEI_SYS93_MIN
        #error FEAT_DEI_SYS93 MIN ERROR
    #endif

    #if FEAT_DEI_SYS93>FEAT_DEI_SYS93_MAX
        #error FEAT_DEI_SYS93 MAX ERROR
    #endif
#endif


//<e0>Feature Custom 06 Enable
//<o1>Feature Custom 06
//         <1=> *Option1
//         <2=>  Option2
//         <3=>  Option3
//         <4=>  Option4
//         <5=>  Option5 
//</e>

#define FEAT_DEI_SYS94_MIN              1
#define FEAT_DEI_SYS94_MAX              5
#define FEAT_DEI_SYS94_CHECK            FALSE

#if FEAT_DEI_SYS94_CHECK==TRUE
    #if FEAT_DEI_SYS94<FEAT_DEI_SYS94_MIN
        #error FEAT_DEI_SYS94 MIN ERROR
    #endif

    #if FEAT_DEI_SYS94>FEAT_DEI_SYS94_MAX
        #error FEAT_DEI_SYS94 MAX ERROR
    #endif
#endif


//<e0>Feature Custom 07 Enable
//<o1>Feature Custom 07
//         <1=> *Option1
//         <2=>  Option2
//         <3=>  Option3
//         <4=>  Option4
//         <5=>  Option5 
//</e>

#define FEAT_DEI_SYS95_MIN              1
#define FEAT_DEI_SYS95_MAX              5
#define FEAT_DEI_SYS95_CHECK            FALSE

#if FEAT_DEI_SYS95_CHECK==TRUE
    #if FEAT_DEI_SYS95<FEAT_DEI_SYS95_MIN
        #error FEAT_DEI_SYS95 MIN ERROR
    #endif

    #if FEAT_DEI_SYS95>FEAT_DEI_SYS95_MAX
        #error FEAT_DEI_SYS95 MAX ERROR
    #endif
#endif


//<e0>Feature Custom 08 Enable
//<o1>Feature Custom 08
//         <1=> *Option1
//         <2=>  Option2
//         <3=>  Option3
//         <4=>  Option4
//         <5=>  Option5 
//</e>

#define FEAT_DEI_SYS96_MIN              1
#define FEAT_DEI_SYS96_MAX              5
#define FEAT_DEI_SYS96_CHECK            FALSE

#if FEAT_DEI_SYS96_CHECK==TRUE
    #if FEAT_DEI_SYS96<FEAT_DEI_SYS96_MIN
        #error FEAT_DEI_SYS96 MIN ERROR
    #endif

    #if FEAT_DEI_SYS96>FEAT_DEI_SYS96_MAX
        #error FEAT_DEI_SYS96 MAX ERROR
    #endif
#endif


// </h>

/*--------------------------------------------------------------------------*/

const UInt8 dei_feature_default_table[FEAT_DEI_SIZE] = 
{
		/* 3 Lock start feature */
		FEAT_DEI_RS22  ,
    /*SS FEATURE*/
    FEAT_DEI_SS01  , FEAT_DEI_SS02  , FEAT_DEI_SS03  , FEAT_DEI_SS04  , FEAT_DEI_SS05  , 
    FEAT_DEI_SS07  , FEAT_DEI_SS09  , FEAT_DEI_SS10  , 
    FEAT_DEI_SS11  , FEAT_DEI_SS12  , FEAT_DEI_SS13  , FEAT_DEI_SS15	 ,

    /*RS FEATURE*/
    FEAT_DEI_RS02  , FEAT_DEI_RS03  , FEAT_DEI_RS04  , FEAT_DEI_RS05 , 
    FEAT_DEI_RS08  , FEAT_DEI_RS09  , FEAT_DEI_RS10  , 
    FEAT_DEI_RS11  , FEAT_DEI_RS12  , FEAT_DEI_RS13  , FEAT_DEI_RS14  , FEAT_DEI_RS15  , 
    FEAT_DEI_RS16  , FEAT_DEI_RS17  , FEAT_DEI_RS18  , FEAT_DEI_RS19  , FEAT_DEI_RS20  , 
    FEAT_DEI_RS21  , FEAT_DEI_RS23  , FEAT_DEI_RS24  , FEAT_DEI_RS25  , FEAT_DEI_RS26  , 

    /*SYSTEM FEATURE*/
    FEAT_DEI_SYS02 , FEAT_DEI_SYS03 , FEAT_DEI_SYS05 ,
    FEAT_DEI_SYS06 , FEAT_DEI_SYS07 , FEAT_DEI_SYS09 , 
    FEAT_DEI_SYS11 , FEAT_DEI_SYS12 , FEAT_DEI_SYS13 , FEAT_DEI_SYS14 , FEAT_DEI_SYS15 ,
    FEAT_DEI_SYS16 , FEAT_DEI_SYS17 , FEAT_DEI_SYS18 , FEAT_DEI_SYS19 , FEAT_DEI_SYS20 ,
    FEAT_DEI_SYS21 , FEAT_DEI_SYS22 , FEAT_DEI_SYS23 , FEAT_DEI_SYS24 , FEAT_DEI_SYS25 ,
    FEAT_DEI_SYS26 , FEAT_DEI_SYS27 , FEAT_DEI_SYS28 , FEAT_DEI_SYS30 ,
    FEAT_DEI_SYS31 , FEAT_DEI_SYS32 , FEAT_DEI_SYS33 , FEAT_DEI_SYS34 , FEAT_DEI_SYS35 ,
    FEAT_DEI_SYS36 , FEAT_DEI_SYS37 , FEAT_DEI_SYS38 , FEAT_DEI_SYS39 ,

    /*PROGRAMMABLE OUTPUTS*/
    FEAT_DEI_SYS40 , FEAT_DEI_SYS41 , FEAT_DEI_SYS42 , FEAT_DEI_SYS43 , FEAT_DEI_SYS44 , 
    FEAT_DEI_SYS45 , FEAT_DEI_SYS46 , FEAT_DEI_SYS47 , FEAT_DEI_SYS48 , FEAT_DEI_SYS49 ,
    FEAT_DEI_SYS50 , FEAT_DEI_SYS51 , FEAT_DEI_SYS52 , FEAT_DEI_SYS53 , FEAT_DEI_SYS54 ,
    FEAT_DEI_SYS55 , FEAT_DEI_SYS56 ,

    /*PROGRAMMABLE INPUTS*/
    FEAT_DEI_SYS57 , FEAT_DEI_SYS58 , FEAT_DEI_SYS59 , FEAT_DEI_SYS60 , FEAT_DEI_SYS61 ,
    FEAT_DEI_SYS62 , FEAT_DEI_SYS63 , FEAT_DEI_SYS64 , FEAT_DEI_SYS65 ,

    /*SENSE CONFIG*/
    FEAT_DEI_SYS69 , FEAT_DEI_SYS70 , FEAT_DEI_SYS71 , FEAT_DEI_SYS72 , FEAT_DEI_SYS73 ,
    FEAT_DEI_SYS74 , FEAT_DEI_SYS75 , FEAT_DEI_SYS77 , FEAT_DEI_SYS78 ,
    FEAT_DEI_SYS79 , FEAT_DEI_SYS80 , FEAT_DEI_SYS81 , FEAT_DEI_SYS82 , FEAT_DEI_SYS83 ,
    FEAT_DEI_SYS84 , FEAT_DEI_SYS85 , FEAT_DEI_SYS86 , FEAT_DEI_SYS87 ,
    
    /*SPECIAL FEATURES*/
    FEAT_DEI_SYS88 , FEAT_DEI_SYS89 , FEAT_DEI_SYS90 , FEAT_DEI_SYS91 , FEAT_DEI_SYS92 ,
    FEAT_DEI_SYS93 , FEAT_DEI_SYS94 , FEAT_DEI_SYS95 , FEAT_DEI_SYS96 , 

    /*DEFAULT SETTINGS FOR TACHO*/     
    TACH_DEFAULT_REF_DIV10 ,        // Default Tach reference divided by 10 (make it one-byte nvfs variable)
    
    /*DEFAULT SETTINGS FOR VIRTUAL TACHO*/    
    VIRTUAL_TACH_DEF ,
};

/*-------------------------------------------------------------------------------------*/

const Dei_Feature_Check dei_feature_check_list[FEAT_DEI_SIZE] = 
{
		/* 3 Lock Start feature */
    { FEAT_DEI_RS22_MIN  , FEAT_DEI_RS22_MAX  , FEAT_DEI_RS22_CHECK},

    /*SS FEATURE*/
    { FEAT_DEI_SS01_MIN  , FEAT_DEI_SS01_MAX  , FEAT_DEI_SS01_CHECK}, 
    { FEAT_DEI_SS02_MIN  , FEAT_DEI_SS02_MAX  , FEAT_DEI_SS02_CHECK}, 
    { FEAT_DEI_SS03_MIN  , FEAT_DEI_SS03_MAX  , FEAT_DEI_SS03_CHECK}, 
    { FEAT_DEI_SS04_MIN  , FEAT_DEI_SS04_MAX  , FEAT_DEI_SS04_CHECK}, 
    { FEAT_DEI_SS05_MIN  , FEAT_DEI_SS05_MAX  , FEAT_DEI_SS05_CHECK}, 
    //{ FEAT_DEI_SS06_MIN  , FEAT_DEI_SS06_MAX  , FEAT_DEI_SS06_CHECK}, 
    { FEAT_DEI_SS07_MIN  , FEAT_DEI_SS07_MAX  , FEAT_DEI_SS07_CHECK}, 
    //{ FEAT_DEI_SS08_MIN  , FEAT_DEI_SS08_MAX  , FEAT_DEI_SS08_CHECK}, 
    { FEAT_DEI_SS09_MIN  , FEAT_DEI_SS09_MAX  , FEAT_DEI_SS09_CHECK}, 
    { FEAT_DEI_SS10_MIN  , FEAT_DEI_SS10_MAX  , FEAT_DEI_SS10_CHECK}, 
    { FEAT_DEI_SS11_MIN  , FEAT_DEI_SS11_MAX  , FEAT_DEI_SS11_CHECK}, 
    { FEAT_DEI_SS12_MIN  , FEAT_DEI_SS12_MAX  , FEAT_DEI_SS12_CHECK}, 
    { FEAT_DEI_SS13_MIN  , FEAT_DEI_SS13_MAX  , FEAT_DEI_SS13_CHECK}, 
    //{ FEAT_DEI_SS14_MIN  , FEAT_DEI_SS14_MAX  , FEAT_DEI_SS14_CHECK},
    { FEAT_DEI_SS15_MIN  , FEAT_DEI_SS15_MAX  , FEAT_DEI_SS15_CHECK},

    /*RS FEATURE*/
//    { FEAT_DEI_RS01_MIN  , FEAT_DEI_RS01_MAX  , FEAT_DEI_RS01_CHECK}, 
    { FEAT_DEI_RS02_MIN  , FEAT_DEI_RS02_MAX  , FEAT_DEI_RS02_CHECK}, 
    { FEAT_DEI_RS03_MIN  , FEAT_DEI_RS03_MAX  , FEAT_DEI_RS03_CHECK}, 
    { FEAT_DEI_RS04_MIN  , FEAT_DEI_RS04_MAX  , FEAT_DEI_RS04_CHECK}, 
    { FEAT_DEI_RS05_MIN  , FEAT_DEI_RS05_MAX  , FEAT_DEI_RS05_CHECK}, 
//    { FEAT_DEI_RS06_MIN  , FEAT_DEI_RS06_MAX  , FEAT_DEI_RS06_CHECK}, 
    //{ FEAT_DEI_RS07_MIN  , FEAT_DEI_RS07_MAX  , FEAT_DEI_RS07_CHECK}, 
    { FEAT_DEI_RS08_MIN  , FEAT_DEI_RS08_MAX  , FEAT_DEI_RS08_CHECK}, 
    { FEAT_DEI_RS09_MIN  , FEAT_DEI_RS09_MAX  , FEAT_DEI_RS09_CHECK}, 
    { FEAT_DEI_RS10_MIN  , FEAT_DEI_RS10_MAX  , FEAT_DEI_RS10_CHECK}, 
    { FEAT_DEI_RS11_MIN  , FEAT_DEI_RS11_MAX  , FEAT_DEI_RS11_CHECK}, 
    { FEAT_DEI_RS12_MIN  , FEAT_DEI_RS12_MAX  , FEAT_DEI_RS12_CHECK}, 
    { FEAT_DEI_RS13_MIN  , FEAT_DEI_RS13_MAX  , FEAT_DEI_RS13_CHECK}, 
    { FEAT_DEI_RS14_MIN  , FEAT_DEI_RS14_MAX  , FEAT_DEI_RS14_CHECK}, 
    { FEAT_DEI_RS15_MIN  , FEAT_DEI_RS15_MAX  , FEAT_DEI_RS15_CHECK}, 
    { FEAT_DEI_RS16_MIN  , FEAT_DEI_RS16_MAX  , FEAT_DEI_RS16_CHECK}, 
    { FEAT_DEI_RS17_MIN  , FEAT_DEI_RS17_MAX  , FEAT_DEI_RS17_CHECK}, 
    { FEAT_DEI_RS18_MIN  , FEAT_DEI_RS18_MAX  , FEAT_DEI_RS18_CHECK}, 
    { FEAT_DEI_RS19_MIN  , FEAT_DEI_RS19_MAX  , FEAT_DEI_RS19_CHECK}, 
    { FEAT_DEI_RS20_MIN  , FEAT_DEI_RS20_MAX  , FEAT_DEI_RS20_CHECK}, 
    { FEAT_DEI_RS21_MIN  , FEAT_DEI_RS21_MAX  , FEAT_DEI_RS21_CHECK}, 
//    { FEAT_DEI_RS22_MIN  , FEAT_DEI_RS22_MAX  , FEAT_DEI_RS22_CHECK},
    { FEAT_DEI_RS23_MIN  , FEAT_DEI_RS23_MAX  , FEAT_DEI_RS23_CHECK},
    { FEAT_DEI_RS24_MIN  , FEAT_DEI_RS24_MAX  , FEAT_DEI_RS24_CHECK},
		{ FEAT_DEI_RS25_MIN  , FEAT_DEI_RS25_MAX  , FEAT_DEI_RS25_CHECK},
		{ FEAT_DEI_RS26_MIN  , FEAT_DEI_RS26_MAX  , FEAT_DEI_RS26_CHECK},

    /*SYSTEM FEATURE*/                                             
    { FEAT_DEI_SYS02_MIN , FEAT_DEI_SYS02_MAX , FEAT_DEI_SYS02_CHECK}, 
    { FEAT_DEI_SYS03_MIN , FEAT_DEI_SYS03_MAX , FEAT_DEI_SYS03_CHECK}, 
    //{ FEAT_DEI_SYS04_MIN , FEAT_DEI_SYS04_MAX , FEAT_DEI_SYS04_CHECK}, 
    { FEAT_DEI_SYS05_MIN , FEAT_DEI_SYS05_MAX , FEAT_DEI_SYS05_CHECK}, 
    { FEAT_DEI_SYS06_MIN , FEAT_DEI_SYS06_MAX , FEAT_DEI_SYS06_CHECK}, 
    { FEAT_DEI_SYS07_MIN , FEAT_DEI_SYS07_MAX , FEAT_DEI_SYS07_CHECK}, 
    //{ FEAT_DEI_SYS08_MIN , FEAT_DEI_SYS08_MAX , FEAT_DEI_SYS08_CHECK}, 
    { FEAT_DEI_SYS09_MIN , FEAT_DEI_SYS09_MAX , FEAT_DEI_SYS09_CHECK}, 
    //{ FEAT_DEI_SYS10_MIN , FEAT_DEI_SYS10_MAX , FEAT_DEI_SYS10_CHECK}, 
    { FEAT_DEI_SYS11_MIN , FEAT_DEI_SYS11_MAX , FEAT_DEI_SYS11_CHECK}, 
    { FEAT_DEI_SYS12_MIN , FEAT_DEI_SYS12_MAX , FEAT_DEI_SYS12_CHECK}, 
    { FEAT_DEI_SYS13_MIN , FEAT_DEI_SYS13_MAX , FEAT_DEI_SYS13_CHECK}, 
    { FEAT_DEI_SYS14_MIN , FEAT_DEI_SYS14_MAX , FEAT_DEI_SYS14_CHECK}, 
    { FEAT_DEI_SYS15_MIN , FEAT_DEI_SYS15_MAX , FEAT_DEI_SYS15_CHECK}, 
    { FEAT_DEI_SYS16_MIN , FEAT_DEI_SYS16_MAX , FEAT_DEI_SYS16_CHECK}, 
    { FEAT_DEI_SYS17_MIN , FEAT_DEI_SYS17_MAX , FEAT_DEI_SYS17_CHECK}, 
    { FEAT_DEI_SYS18_MIN , FEAT_DEI_SYS18_MAX , FEAT_DEI_SYS18_CHECK}, 
    { FEAT_DEI_SYS19_MIN , FEAT_DEI_SYS19_MAX , FEAT_DEI_SYS19_CHECK}, 
    { FEAT_DEI_SYS20_MIN , FEAT_DEI_SYS20_MAX , FEAT_DEI_SYS20_CHECK}, 
    { FEAT_DEI_SYS21_MIN , FEAT_DEI_SYS21_MAX , FEAT_DEI_SYS21_CHECK}, 
    { FEAT_DEI_SYS22_MIN , FEAT_DEI_SYS22_MAX , FEAT_DEI_SYS22_CHECK}, 
    { FEAT_DEI_SYS23_MIN , FEAT_DEI_SYS23_MAX , FEAT_DEI_SYS23_CHECK}, 
    { FEAT_DEI_SYS24_MIN , FEAT_DEI_SYS24_MAX , FEAT_DEI_SYS24_CHECK}, 
    { FEAT_DEI_SYS25_MIN , FEAT_DEI_SYS25_MAX , FEAT_DEI_SYS25_CHECK}, 
    { FEAT_DEI_SYS26_MIN , FEAT_DEI_SYS26_MAX , FEAT_DEI_SYS26_CHECK}, 
    { FEAT_DEI_SYS27_MIN , FEAT_DEI_SYS27_MAX , FEAT_DEI_SYS27_CHECK}, 
    { FEAT_DEI_SYS28_MIN , FEAT_DEI_SYS28_MAX , FEAT_DEI_SYS28_CHECK}, 
    //{ FEAT_DEI_SYS29_MIN , FEAT_DEI_SYS29_MAX , FEAT_DEI_SYS29_CHECK}, 
    { FEAT_DEI_SYS30_MIN , FEAT_DEI_SYS30_MAX , FEAT_DEI_SYS30_CHECK},
    { FEAT_DEI_SYS31_MIN , FEAT_DEI_SYS31_MAX , FEAT_DEI_SYS31_CHECK},
    { FEAT_DEI_SYS32_MIN , FEAT_DEI_SYS32_MAX , FEAT_DEI_SYS32_CHECK},
    { FEAT_DEI_SYS33_MIN , FEAT_DEI_SYS33_MAX , FEAT_DEI_SYS33_CHECK},
    { FEAT_DEI_SYS34_MIN , FEAT_DEI_SYS34_MAX , FEAT_DEI_SYS34_CHECK},
    { FEAT_DEI_SYS35_MIN , FEAT_DEI_SYS35_MAX , FEAT_DEI_SYS35_CHECK},
    { FEAT_DEI_SYS36_MIN , FEAT_DEI_SYS36_MAX , FEAT_DEI_SYS36_CHECK},
    { FEAT_DEI_SYS37_MIN , FEAT_DEI_SYS37_MAX , FEAT_DEI_SYS37_CHECK},
    { FEAT_DEI_SYS38_MIN , FEAT_DEI_SYS38_MAX , FEAT_DEI_SYS38_CHECK},
    { FEAT_DEI_SYS39_MIN , FEAT_DEI_SYS39_MAX , FEAT_DEI_SYS39_CHECK},
    /*PROGRAMMABLE OUTPUTS*/
    {  0, FEAT_DEI_OUT_OPT_MAX, FEAT_DEI_SYS40_CHECK },
    {  0, FEAT_DEI_OUT_OPT_MAX, FEAT_DEI_SYS41_CHECK },
    {  0, FEAT_DEI_OUT_OPT_MAX, FEAT_DEI_SYS42_CHECK },
    {  0, FEAT_DEI_OUT_OPT_MAX, FEAT_DEI_SYS43_CHECK },
    {  0, FEAT_DEI_OUT_OPT_MAX, FEAT_DEI_SYS44_CHECK },
    {  0, FEAT_DEI_OUT_OPT_MAX, FEAT_DEI_SYS45_CHECK },
    {  0, FEAT_DEI_OUT_OPT_MAX, FEAT_DEI_SYS46_CHECK },
    {  0, FEAT_DEI_OUT_OPT_MAX, FEAT_DEI_SYS47_CHECK },
    {  0, FEAT_DEI_OUT_OPT_MAX, FEAT_DEI_SYS48_CHECK },
    {  0, FEAT_DEI_OUT_OPT_MAX, FEAT_DEI_SYS49_CHECK },
    {  0, FEAT_DEI_OUT_OPT_MAX, FEAT_DEI_SYS50_CHECK },
    {  0, FEAT_DEI_OUT_OPT_MAX, FEAT_DEI_SYS51_CHECK },
    {  0, FEAT_DEI_OUT_OPT_MAX, FEAT_DEI_SYS52_CHECK },
    {  0, FEAT_DEI_OUT_OPT_MAX, FEAT_DEI_SYS53_CHECK },
    {  0, FEAT_DEI_OUT_OPT_MAX, FEAT_DEI_SYS54_CHECK },
    {  0, FEAT_DEI_OUT_OPT_MAX, FEAT_DEI_SYS55_CHECK },
    {  0, FEAT_DEI_OUT_OPT_MAX, FEAT_DEI_SYS56_CHECK },
		
    /*PROGRAMMABLE INPUTS*/ 
	  {  0, FEAT_DEI_INP_OPT_MAX, FEAT_DEI_SYS57_CHECK },
	  {  0, FEAT_DEI_INP_OPT_MAX, FEAT_DEI_SYS58_CHECK },
	  {  0, FEAT_DEI_INP_OPT_MAX, FEAT_DEI_SYS59_CHECK },
	  {  0, FEAT_DEI_INP_OPT_MAX, FEAT_DEI_SYS60_CHECK },
	  {  0, FEAT_DEI_INP_OPT_MAX, FEAT_DEI_SYS61_CHECK },
	  {  0, FEAT_DEI_INP_OPT_MAX, FEAT_DEI_SYS62_CHECK },
	  {  0, FEAT_DEI_INP_OPT_MAX, FEAT_DEI_SYS63_CHECK },
	  {  0, FEAT_DEI_INP_OPT_MAX, FEAT_DEI_SYS64_CHECK },
	  {  0, FEAT_DEI_INP_OPT_MAX, FEAT_DEI_SYS65_CHECK },
		
    /*SENSE CONFIG*/    
    { FEAT_DEI_SYS69_MIN , FEAT_DEI_SYS69_MAX , FEAT_DEI_SYS69_CHECK},  
    { FEAT_DEI_SYS70_MIN , FEAT_DEI_SYS70_MAX , FEAT_DEI_SYS70_CHECK},
    { FEAT_DEI_SYS71_MIN , FEAT_DEI_SYS71_MAX , FEAT_DEI_SYS71_CHECK},
    { FEAT_DEI_SYS72_MIN , FEAT_DEI_SYS72_MAX , FEAT_DEI_SYS72_CHECK},
    { FEAT_DEI_SYS73_MIN , FEAT_DEI_SYS73_MAX , FEAT_DEI_SYS73_CHECK},
    { FEAT_DEI_SYS74_MIN , FEAT_DEI_SYS74_MAX , FEAT_DEI_SYS74_CHECK},
    { FEAT_DEI_SYS75_MIN , FEAT_DEI_SYS75_MAX , FEAT_DEI_SYS75_CHECK},
    { FEAT_DEI_SYS77_MIN , FEAT_DEI_SYS77_MAX , FEAT_DEI_SYS77_CHECK},
    { FEAT_DEI_SYS78_MIN , FEAT_DEI_SYS78_MAX , FEAT_DEI_SYS78_CHECK},
    { FEAT_DEI_SYS79_MIN , FEAT_DEI_SYS79_MAX , FEAT_DEI_SYS79_CHECK},
    { FEAT_DEI_SYS80_MIN , FEAT_DEI_SYS80_MAX , FEAT_DEI_SYS80_CHECK},
    { FEAT_DEI_SYS81_MIN , FEAT_DEI_SYS81_MAX , FEAT_DEI_SYS81_CHECK},
    { FEAT_DEI_SYS82_MIN , FEAT_DEI_SYS82_MAX , FEAT_DEI_SYS82_CHECK},
    { FEAT_DEI_SYS83_MIN , FEAT_DEI_SYS83_MAX , FEAT_DEI_SYS83_CHECK},
    { FEAT_DEI_SYS84_MIN , FEAT_DEI_SYS84_MAX , FEAT_DEI_SYS84_CHECK},
    { FEAT_DEI_SYS85_MIN , FEAT_DEI_SYS85_MAX , FEAT_DEI_SYS85_CHECK},
    { FEAT_DEI_SYS86_MIN , FEAT_DEI_SYS86_MAX , FEAT_DEI_SYS86_CHECK},
    { FEAT_DEI_SYS87_MIN , FEAT_DEI_SYS87_MAX , FEAT_DEI_SYS87_CHECK},
    /*SPECIAL FEATURES*/    
    { FEAT_DEI_SYS88_MIN , FEAT_DEI_SYS88_MAX , FEAT_DEI_SYS88_CHECK},
    { FEAT_DEI_SYS89_MIN , FEAT_DEI_SYS89_MAX , FEAT_DEI_SYS89_CHECK},
    { FEAT_DEI_SYS90_MIN , FEAT_DEI_SYS90_MAX , FEAT_DEI_SYS90_CHECK},
    { FEAT_DEI_SYS91_MIN , FEAT_DEI_SYS91_MAX , FEAT_DEI_SYS91_CHECK},
    { FEAT_DEI_SYS92_MIN , FEAT_DEI_SYS92_MAX , FEAT_DEI_SYS92_CHECK},
    { FEAT_DEI_SYS93_MIN , FEAT_DEI_SYS93_MAX , FEAT_DEI_SYS93_CHECK},
    { FEAT_DEI_SYS94_MIN , FEAT_DEI_SYS94_MAX , FEAT_DEI_SYS94_CHECK},
    { FEAT_DEI_SYS95_MIN , FEAT_DEI_SYS95_MAX , FEAT_DEI_SYS95_CHECK},
    { FEAT_DEI_SYS96_MIN , FEAT_DEI_SYS96_MAX , FEAT_DEI_SYS96_CHECK},

    /*DEFAULT SETTINGS FOR TACHO and VIRTUAL TACHO*/
    {0 , 0 , FALSE }, 
    {3 , 0 , FALSE }
};

/*-------------------------------------------------------------------------------------*/


UInt8              dei_feature_list       [FEAT_DEI_SIZE];


/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

