/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_feature_vars.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_feature_private.h"
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

//<e0>System Arming Mode Enable    ok
//<o1>System Arming Mode
//		<1=> *Active              
//		<2=> Passive Arm - No Lock
//		<3=> Passive Arm & Lock
//		<4=> Auto Re-arm - No Lock
//		<5=> Auto Re-arm & Lock
//</e>

#define FEAT_DEI_SS01_MIN  			1
#define FEAT_DEI_SS01_MAX  			5
#define FEAT_DEI_SS01_CHECK			TRUE

#if FEAT_DEI_SS01_CHECK==TRUE
    #if FEAT_DEI_SS01<FEAT_DEI_SS01_MIN
        #error FEAT_DEI_SS01 MIN ERROR
    #endif

    #if FEAT_DEI_SS01>FEAT_DEI_SS01_MAX
        #error FEAT_DEI_SS01 MAX ERROR
    #endif
#endif

//<e0>Panic Mode Visible           ok
//<o1>Panic Mode
//      <1=> *On           
//      <2=> Ignition Off only 
//      <3=> Off          
//</e>
#define FEAT_DEI_SS02_MIN  			1
#define FEAT_DEI_SS02_MAX  			3
#define FEAT_DEI_SS02_CHECK			TRUE

#if FEAT_DEI_SS02_CHECK==TRUE
    #if FEAT_DEI_SS02<FEAT_DEI_SS02_MIN
        #error FEAT_DEI_SS02 MIN ERROR
    #endif

    #if FEAT_DEI_SS02>FEAT_DEI_SS02_MAX
        #error FEAT_DEI_SS02 MAX ERROR
    #endif
#endif

//<e0>Confirmation Chirps Enable   ok
//<o1>Confirmation Chirps
//      <1=>*On - Warn Chirps On
//      <2=>On  - Warn Chirps Off
//      <3=>Off - Warn Chirps On
//      <4=>Off - Warn Chirps Off
//</e>
#define FEAT_DEI_SS03_MIN  			1
#define FEAT_DEI_SS03_MAX  			4
#define FEAT_DEI_SS03_CHECK			TRUE

#if FEAT_DEI_SS03_CHECK==TRUE
    #if FEAT_DEI_SS03<FEAT_DEI_SS03_MIN
        #error FEAT_DEI_SS03 MIN ERROR
    #endif

    #if FEAT_DEI_SS03>FEAT_DEI_SS03_MAX
        #error FEAT_DEI_SS03 MAX ERROR
    #endif
#endif


//<e0>Horn Function Enable          ok
//<o1>Horn function 
//      <1=>*Trigger & Panic Only
//      <2=>Chirp 20 mS   
//      <3=>Chirp 30 mS   
//      <4=>Chirp 40 mS   
//      <5=>Chirp 50 mS    
//</e>
#define FEAT_DEI_SS04_MIN  			1
#define FEAT_DEI_SS04_MAX  			5
#define FEAT_DEI_SS04_CHECK			TRUE

#if FEAT_DEI_SS04_CHECK==TRUE
    #if FEAT_DEI_SS04<FEAT_DEI_SS04_MIN
        #error FEAT_DEI_SS04 MIN ERROR
    #endif

    #if FEAT_DEI_SS04>FEAT_DEI_SS04_MAX
        #error FEAT_DEI_SS04 MAX ERROR
    #endif
#endif


//<e0>Override Pulse Count Enable   ok
//<o1>Override Pulse Count
//      <1=>*1
//      <2=>2
//      <3=>3
//      <4=>4
//      <5=>5
//</e>
#define FEAT_DEI_SS05_MIN  			1
#define FEAT_DEI_SS05_MAX  			5
#define FEAT_DEI_SS05_CHECK			TRUE

#if FEAT_DEI_SS05_CHECK==TRUE
    #if FEAT_DEI_SS05<FEAT_DEI_SS05_MIN
        #error FEAT_DEI_SS05 MIN ERROR
    #endif

    #if FEAT_DEI_SS05>FEAT_DEI_SS05_MAX
        #error FEAT_DEI_SS01 MAX ERROR
    #endif
#endif



//<e0>Sensor Icon Type Enable      ok
//<o1>Sensor Icon Type
//      <1=> None                          
//      <2=> *Shock                  
//      <3=> Field disturbance              
//      <4=> Tilt sensor                    
//      <5=> Glass break
//      <6=> Ultrasonic  
//</e>
#define FEAT_DEI_SS06_MIN  			1
#define FEAT_DEI_SS06_MAX  			6
#define FEAT_DEI_SS06_CHECK			TRUE

#if FEAT_DEI_SS06_CHECK==TRUE
    #if FEAT_DEI_SS06<FEAT_DEI_SS06_MIN
        #error FEAT_DEI_SS06 MIN ERROR
    #endif

    #if FEAT_DEI_SS06>FEAT_DEI_SS06_MAX
        #error FEAT_DEI_SS06 MAX ERROR
    #endif
#endif


//<e0>Trigger/Panic Duration Enable   ok
//<o1>Trigger/Panic Duration( seconds ) <1-180:1>
//</e>
#define FEAT_DEI_SS07_MIN  			1
#define FEAT_DEI_SS07_MAX  			180
#define FEAT_DEI_SS07_CHECK			TRUE

#if FEAT_DEI_SS07_CHECK==TRUE
    #if FEAT_DEI_SS07<FEAT_DEI_SS07_MIN
        #error FEAT_DEI_SS07 MIN ERROR
    #endif

    #if FEAT_DEI_SS07>FEAT_DEI_SS07_MAX
        #error FEAT_DEI_SS07 MAX ERROR
    #endif
#endif

//<e0>One-Time bypass Enable         ok
//<o1>One-time Bypass          
//      <1=>On  
//      <2=>*Off
//</e>
#define FEAT_DEI_SS08_MIN  			1
#define FEAT_DEI_SS08_MAX  			2
#define FEAT_DEI_SS08_CHECK			TRUE

#if FEAT_DEI_SS08_CHECK==TRUE
    #if FEAT_DEI_SS08<FEAT_DEI_SS08_MIN
        #error FEAT_DEI_SS08 MIN ERROR
    #endif

    #if FEAT_DEI_SS08>FEAT_DEI_SS08_MAX
        #error FEAT_DEI_SS08 MAX ERROR
    #endif
#endif

//<e0>NPC - Nuisance Prevention Circuitry Enable    ok 
//<o1>NPC - Nuisance Prevention Circuitry     
//     <1=>*On  
//     <2=>Off 
//</e>
#define FEAT_DEI_SS09_MIN  			1
#define FEAT_DEI_SS09_MAX  			2
#define FEAT_DEI_SS09_CHECK			TRUE

#if FEAT_DEI_SS09_CHECK==TRUE
    #if FEAT_DEI_SS09<FEAT_DEI_SS09_MIN
        #error FEAT_DEI_SS09 MIN ERROR
    #endif

    #if FEAT_DEI_SS09>FEAT_DEI_SS09_MAX
        #error FEAT_DEI_SS09 MAX ERROR
    #endif
#endif

//<e0>Door Trigger Error Enable     ok
//<o1>Door Trigger Error Chirp 
//         <1=>Off 
//         <2=>*On
//</e>
#define FEAT_DEI_SS10_MIN  			1
#define FEAT_DEI_SS10_MAX  			2
#define FEAT_DEI_SS10_CHECK			TRUE

#if FEAT_DEI_SS10_CHECK==TRUE
    #if FEAT_DEI_SS10<FEAT_DEI_SS10_MIN
        #error FEAT_DEI_SS10 MIN ERROR
    #endif

    #if FEAT_DEI_SS10>FEAT_DEI_SS10_MAX
        #error FEAT_DEI_SS10 MAX ERROR
    #endif
#endif


//</h>


//--------------------------------------------------------------------------//
// <h> Remote Start Features
//--------------------------------------------------------------------------//

//<e0>Supported Transmission Mode Enable    ok
//<o1> Supported Transmission Mode  
//    <i> is used form Vip and firmware          
//         <1=>Automatic
//         <2=>Manual                                   
//         <3=>Both 
//</e>
#define FEAT_DEI_RS01_MIN  			1
#define FEAT_DEI_RS01_MAX  			3
#define FEAT_DEI_RS01_CHECK			TRUE

#if FEAT_DEI_RS01_CHECK==TRUE
    #if FEAT_DEI_RS01<FEAT_DEI_RS01_MIN
        #error FEAT_DEI_RS01 MIN ERROR
    #endif

    #if FEAT_DEI_RS01>FEAT_DEI_RS01_MAX
        #error FEAT_DEI_RS01 MAX ERROR
    #endif
#endif


//<e0>Engine Check Mode Enable       ok
//<o1> Engine Checking Mode                
//         <1=> Virtual Tach  
//         <2=> Voltage       
//         <3=> Off   
//         <4=> *Tachometer   
//</e>
#define FEAT_DEI_RS02_MIN  			1
#define FEAT_DEI_RS02_MAX  			4
#define FEAT_DEI_RS02_CHECK			TRUE

#if FEAT_DEI_RS02_CHECK==TRUE
    #if FEAT_DEI_RS02<FEAT_DEI_RS02_MIN
        #error FEAT_DEI_RS02 MIN ERROR
    #endif

    #if FEAT_DEI_RS01>FEAT_DEI_RS01_MAX
        #error FEAT_DEI_RS02 MAX ERROR
    #endif
#endif


//<e0>Cranking Time Enable          ok
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
#define FEAT_DEI_RS03_MIN  			1
#define FEAT_DEI_RS03_MAX  			9
#define FEAT_DEI_RS03_CHECK			TRUE

#if FEAT_DEI_RS03_CHECK==TRUE
    #if FEAT_DEI_RS03<FEAT_DEI_RS03_MIN
        #error FEAT_DEI_RS03 MIN ERROR
    #endif

    #if FEAT_DEI_RS03>FEAT_DEI_RS03_MAX
        #error FEAT_DEI_RS03 MAX ERROR
    #endif
#endif


//<e0>Activation Pulse Enable       ok
//<o1>Activation Pulse Count              
//         <1=> *1     
//         <2=>  2   
//</e>
#define FEAT_DEI_RS04_MIN  			1
#define FEAT_DEI_RS04_MAX  			3
#define FEAT_DEI_RS04_CHECK			TRUE

#if FEAT_DEI_RS04_CHECK==TRUE
    #if FEAT_DEI_RS04<FEAT_DEI_RS04_MIN
        #error FEAT_DEI_RS04 MIN ERROR
    #endif

    #if FEAT_DEI_RS04>FEAT_DEI_RS04_MAX
        #error FEAT_DEI_RS04 MAX ERROR
    #endif
#endif


//<e0>Diesel Start Delay Enable      ok
//<o1>Diesel Delay Type/Duration<0-90:1>                 
//    <i> 0 is mean : Wait for Glowplug Input
//</e>
#define FEAT_DEI_RS05_MIN  			0
#define FEAT_DEI_RS05_MAX  			90
#define FEAT_DEI_RS05_CHECK			TRUE

#if FEAT_DEI_RS05_CHECK==TRUE
    #if FEAT_DEI_RS05<FEAT_DEI_RS05_MIN
        #error FEAT_DEI_RS05 MIN ERROR
    #endif

    #if FEAT_DEI_RS05>FEAT_DEI_RS05_MAX
        #error FEAT_DEI_RS05 MAX ERROR
    #endif
#endif


// <e0>Accessory Output during Diesel Start Delay    ok
//  <o1> Accessory Output During Diesel Start Delay 
//         <1=> *On    
//         <2=> Off  
//</e>
#define FEAT_DEI_RS06_MIN  			1
#define FEAT_DEI_RS06_MAX  			2
#define FEAT_DEI_RS06_CHECK			TRUE

#if FEAT_DEI_RS06_CHECK==TRUE
    #if FEAT_DEI_RS06<FEAT_DEI_RS06_MIN
        #error FEAT_DEI_RS06 MIN ERROR
    #endif

    #if FEAT_DEI_RS06>FEAT_DEI_RS06_MAX
        #error FEAT_DEI_RS06 MAX ERROR
    #endif
#endif


//<e0>Rear Defogger Output Type Enable          ok          
//<o1>Rear Defogger Output Type                     
//         <1=> *Latched( 10 minutes )
//         <2=> Pulse Only( 1 second )
//</e>
#define FEAT_DEI_RS07_MIN  			1
#define FEAT_DEI_RS07_MAX  			2
#define FEAT_DEI_RS07_CHECK			TRUE

#if FEAT_DEI_RS07_CHECK==TRUE
    #if FEAT_DEI_RS07<FEAT_DEI_RS07_MIN
        #error FEAT_DEI_RS07 MIN ERROR
    #endif

    #if FEAT_DEI_RS07>FEAT_DEI_RS07_MAX
        #error FEAT_DEI_RS07 MAX ERROR
    #endif
#endif


//<e0>Parking Light Output Enable    ok
//<o1>Parking Light Output                
//         <1=> *Constant      
//         <2=> Pulsed        
//         <3=> Off   
//</e>
#define FEAT_DEI_RS08_MIN  			1
#define FEAT_DEI_RS08_MAX  			3
#define FEAT_DEI_RS08_CHECK			TRUE

#if FEAT_DEI_RS08_CHECK==TRUE
    #if FEAT_DEI_RS08<FEAT_DEI_RS08_MIN
        #error FEAT_DEI_RS08 MIN ERROR
    #endif

    #if FEAT_DEI_RS08>FEAT_DEI_RS08_MAX
        #error FEAT_DEI_RS08 MAX ERROR
    #endif
#endif


//<e0>Turbo Mode Runtime Enable     ok
//<o1> Turbo Mode Runtime( minutes ) <0-10:1>
//    <i>  0 : turbo mode is not enable
//</e>
#define FEAT_DEI_RS09_MIN  			0
#define FEAT_DEI_RS09_MAX  			10
#define FEAT_DEI_RS09_CHECK			TRUE

#if FEAT_DEI_RS09_CHECK==TRUE
    #if FEAT_DEI_RS09<FEAT_DEI_RS09_MIN
        #error FEAT_DEI_RS09 MIN ERROR
    #endif

    #if FEAT_DEI_RS09>FEAT_DEI_RS09_MAX
        #error FEAT_DEI_RS09 MAX ERROR
    #endif
#endif



//<e0>Remote Start Runtime Enable   ok
//<o1> Remote Start Runtime( minutes ) <1-60:1>
//</e>
#define FEAT_DEI_RS10_MIN  			1
#define FEAT_DEI_RS10_MAX  			60
#define FEAT_DEI_RS10_CHECK			TRUE

#if FEAT_DEI_RS10_CHECK==TRUE
    #if FEAT_DEI_RS10<FEAT_DEI_RS10_MIN
        #error FEAT_DEI_RS10 MIN ERROR
    #endif

    #if FEAT_DEI_RS10>FEAT_DEI_RS10_MAX
        #error FEAT_DEI_RS10 MAX ERROR
    #endif
#endif

//<e0>Smart Start/Timer Mode Runtime Enable  ok
//<o1> Smart Start/Timer Mode Runtime( minutes ) <1-16:1>
//</e>
#define FEAT_DEI_RS11_MIN  			1
#define FEAT_DEI_RS11_MAX  			16
#define FEAT_DEI_RS11_CHECK			TRUE

#if FEAT_DEI_RS11_CHECK==TRUE
    #if FEAT_DEI_RS11<FEAT_DEI_RS11_MIN
        #error FEAT_DEI_RS11 MIN ERROR
    #endif

    #if FEAT_DEI_RS11>FEAT_DEI_RS11_MAX
        #error FEAT_DEI_RS11 MAX ERROR
    #endif
#endif



// <e0>Smart Start/Timer Mode Start Count Enable   ok
//  <o1> Smart Start/Timer Mode Start Count
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
//         <14=> 24
//</e>
#define FEAT_DEI_RS12_MIN  			1
#define FEAT_DEI_RS12_MAX  			14
#define FEAT_DEI_RS12_CHECK			TRUE

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
//         <14=> 24
//</e>
#define FEAT_DEI_RS13_MIN  			1
#define FEAT_DEI_RS13_MAX  			14
#define FEAT_DEI_RS13_CHECK			TRUE

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
#define FEAT_DEI_RS14_MIN  			1
#define FEAT_DEI_RS14_MAX  			11
#define FEAT_DEI_RS14_CHECK			TRUE

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
#define FEAT_DEI_RS15_MIN  			1
#define FEAT_DEI_RS15_MAX  			11
#define FEAT_DEI_RS15_CHECK			TRUE

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
#define FEAT_DEI_RS16_MIN  			1
#define FEAT_DEI_RS16_MAX  			8
#define FEAT_DEI_RS16_CHECK			TRUE

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
//    <o1.0..6> Millisecond <0-1000:50><#/50>
//  </e>
//</e>
#define FEAT_DEI_RS17_MIN  			1
#define FEAT_DEI_RS17_MAX  			1
#define FEAT_DEI_RS17_CHECK			FALSE

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
#define FEAT_DEI_RS18_MIN  			1
#define FEAT_DEI_RS18_MAX  			3
#define FEAT_DEI_RS18_CHECK			TRUE

#if FEAT_DEI_RS18_CHECK==TRUE
    #if FEAT_DEI_RS18<FEAT_DEI_RS18_MIN
        #error FEAT_DEI_RS18 MIN ERROR
    #endif

    #if FEAT_DEI_RS18>FEAT_DEI_RS18_MAX
        #error FEAT_DEI_RS18 MAX ERROR
    #endif
#endif


// </h>

//--------------------------------------------------------------------------//
// <h>  System Features
//--------------------------------------------------------------------------//

//<e0>Ignition Controlled Locksk Enable
//<o1>Ignition Controlled Locks 
//       <1=>*Off
//       <2=>Lock & Unlock  
//       <3=>Lock Only      
//       <4=>Unlock Only    
//</e>
#define FEAT_DEI_SYS01_MIN  			1
#define FEAT_DEI_SYS01_MAX  			4
#define FEAT_DEI_SYS01_CHECK			TRUE

#if FEAT_DEI_SYS01_CHECK==TRUE
    #if FEAT_DEI_SYS01<FEAT_DEI_SYS01_MIN
        #error FEAT_DEI_SYS01 MIN ERROR
    #endif

    #if FEAT_DEI_SYS01>FEAT_DEI_SYS01_MAX
        #error FEAT_DEI_SYS01 MAX ERROR
    #endif
#endif


//<e0>Door lock pulses Enable
//<o1> DoorLock Pulses 
//         <1=>*Single                
//         <2=>*Double Unlock Only   
//         <3=>Double Lock Only     
//         <4=>Double Lock & Unlock 
//</e>
#define FEAT_DEI_SYS02_MIN  			1
#define FEAT_DEI_SYS02_MAX  			4
#define FEAT_DEI_SYS02_CHECK			TRUE

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
#define FEAT_DEI_SYS03_MIN  			1
#define FEAT_DEI_SYS03_MAX  			3
#define FEAT_DEI_SYS03_CHECK			TRUE

#if FEAT_DEI_SYS03_CHECK==TRUE
    #if FEAT_DEI_SYS03<FEAT_DEI_SYS03_MIN
        #error FEAT_DEI_SYS03 MIN ERROR
    #endif

    #if FEAT_DEI_SYS03>FEAT_DEI_SYS03_MAX
        #error FEAT_DEI_SYS03 MAX ERROR
    #endif
#endif



//<e0>2nd Unlock Enable
//<o1>2nd Unlock 
//     <1=>*Off                              
//     <2=>On w/delayed Ignition Control Output
//     <3=>On w/immediate Ignition Control Output
//</e>
#define FEAT_DEI_SYS04_MIN  			1
#define FEAT_DEI_SYS04_MAX  			3
#define FEAT_DEI_SYS04_CHECK			TRUE


#if FEAT_DEI_SYS04_CHECK==TRUE
    #if FEAT_DEI_SYS04<FEAT_DEI_SYS04_MIN
        #error FEAT_DEI_SYS04 MIN ERROR
    #endif

    #if FEAT_DEI_SYS04>FEAT_DEI_SYS04_MAX
        #error FEAT_DEI_SYS04 MAX ERROR
    #endif
#endif


//<e0>Comfort Closure Enable
//<o1>Comfort Closure 
//      <1=>*No Comfort Closure        
//      <2=>Comfort Closure 1       
//      <3=>Comfort Closure 2       
//</e>
#define FEAT_DEI_SYS05_MIN  			1
#define FEAT_DEI_SYS05_MAX  			3
#define FEAT_DEI_SYS05_CHECK			TRUE


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
#define FEAT_DEI_SYS06_MIN  			1
#define FEAT_DEI_SYS06_MAX  			2
#define FEAT_DEI_SYS06_CHECK			TRUE


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
#define FEAT_DEI_SYS07_MIN  			1
#define FEAT_DEI_SYS07_MAX  			2
#define FEAT_DEI_SYS07_CHECK			TRUE

#if FEAT_DEI_SYS07_CHECK==TRUE
    #if FEAT_DEI_SYS07<FEAT_DEI_SYS07_MIN
        #error FEAT_DEI_SYS07 MIN ERROR
    #endif

    #if FEAT_DEI_SYS07>FEAT_DEI_SYS07_MAX
        #error FEAT_DEI_SYS07 MAX ERROR
    #endif
#endif

 
//<e0>OEM Alarm Disarm w/Aux-Trunk Enable
//<o1>OEM Alarm Disarm w/Aux-Trunk   
//      <1=>Off 
//      <2=>*On
//</e>
#define FEAT_DEI_SYS08_MIN  			1
#define FEAT_DEI_SYS08_MAX  			2
#define FEAT_DEI_SYS08_CHECK			TRUE


#if FEAT_DEI_SYS08_CHECK==TRUE
    #if FEAT_DEI_SYS08<FEAT_DEI_SYS08_MIN
        #error FEAT_DEI_SYS08 MIN ERROR
    #endif

    #if FEAT_DEI_SYS08>FEAT_DEI_SYS08_MAX
        #error FEAT_DEI_SYS08 MAX ERROR
    #endif
#endif

 
//<e0>OEM Alarm Disarm Output  Enable
//<o1>OEM Alarm Disarm Output  
//     <1=>*With Unlock       
//     <2=> Before Unlock     
//     <3=> Remote Start Only 
//</e>
#define FEAT_DEI_SYS09_MIN  			1
#define FEAT_DEI_SYS09_MAX  			3
#define FEAT_DEI_SYS09_CHECK			TRUE

#if FEAT_DEI_SYS09_CHECK==TRUE
    #if FEAT_DEI_SYS09<FEAT_DEI_SYS09_MIN
        #error FEAT_DEI_SYS09 MIN ERROR
    #endif

    #if FEAT_DEI_SYS09>FEAT_DEI_SYS09_MAX
        #error FEAT_DEI_SYS09 MAX ERROR
    #endif
#endif
 
//<e0>OEM Alarm Disarm Pulses Enable
//<o1>OEM Alarm Disarm Pulses  
//    <1=>*1
//    <2=> 2
//</e>
#define FEAT_DEI_SYS10_MIN  			1
#define FEAT_DEI_SYS10_MAX  			2
#define FEAT_DEI_SYS10_CHECK			TRUE

#if FEAT_DEI_SYS10_CHECK==TRUE
    #if FEAT_DEI_SYS10<FEAT_DEI_SYS10_MIN
        #error FEAT_DEI_SYS10 MIN ERROR
    #endif

    #if FEAT_DEI_SYS10>FEAT_DEI_SYS10_MAX
        #error FEAT_DEI_SYS10 MAX ERROR
    #endif
#endif


//<e0>AUX 1 Icon Enable
//<o1>AUX 1 Icon 
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
#define FEAT_DEI_SYS11_MIN  			1
#define FEAT_DEI_SYS11_MAX  			11
#define FEAT_DEI_SYS11_CHECK			TRUE

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
#define FEAT_DEI_SYS12_MIN  			1
#define FEAT_DEI_SYS12_MAX  			5
#define FEAT_DEI_SYS12_CHECK			TRUE

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
#define FEAT_DEI_SYS13_MIN  			1
#define FEAT_DEI_SYS13_MAX  			5
#define FEAT_DEI_SYS13_CHECK			TRUE

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
#define FEAT_DEI_SYS14_MIN  			1
#define FEAT_DEI_SYS14_MAX  			90
#define FEAT_DEI_SYS14_CHECK			TRUE

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
#define FEAT_DEI_SYS15_MIN  			1
#define FEAT_DEI_SYS15_MAX  			11
#define FEAT_DEI_SYS15_CHECK			TRUE

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
#define FEAT_DEI_SYS16_MIN  			1
#define FEAT_DEI_SYS16_MAX  			5
#define FEAT_DEI_SYS16_CHECK			TRUE

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
//    <4=> Link to Arm/ disarm       
//    <5=> Link to Remote Start only 
//</e>
#define FEAT_DEI_SYS17_MIN  			1
#define FEAT_DEI_SYS17_MAX  			5
#define FEAT_DEI_SYS17_CHECK			TRUE

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
#define FEAT_DEI_SYS18_MIN  			1
#define FEAT_DEI_SYS18_MAX  			90
#define FEAT_DEI_SYS18_CHECK			TRUE

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
#define FEAT_DEI_SYS19_MIN  			1
#define FEAT_DEI_SYS19_MAX  			11
#define FEAT_DEI_SYS19_CHECK			TRUE

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
#define FEAT_DEI_SYS20_MIN  			1
#define FEAT_DEI_SYS20_MAX  			5
#define FEAT_DEI_SYS20_CHECK			TRUE
 
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
//    <4=> Link to Arm/ disarm       
//    <5=> Link to Remote Start only 
//</e>
#define FEAT_DEI_SYS21_MIN  			1
#define FEAT_DEI_SYS21_MAX  			5
#define FEAT_DEI_SYS21_CHECK			TRUE

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
#define FEAT_DEI_SYS22_MIN  			1
#define FEAT_DEI_SYS22_MAX  			90
#define FEAT_DEI_SYS22_CHECK			TRUE

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
#define FEAT_DEI_SYS23_MIN  			1
#define FEAT_DEI_SYS23_MAX  			5
#define FEAT_DEI_SYS23_CHECK			TRUE

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
//    <4=> Link to Arm/ disarm       
//    <5=> Link to Remote Start only 
//</e>
#define FEAT_DEI_SYS24_MIN  			1
#define FEAT_DEI_SYS24_MAX  			5
#define FEAT_DEI_SYS24_CHECK			TRUE

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
#define FEAT_DEI_SYS25_MIN  			1
#define FEAT_DEI_SYS25_MAX  			90
#define FEAT_DEI_SYS25_CHECK			TRUE

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
#define FEAT_DEI_SYS26_MIN  			1
#define FEAT_DEI_SYS26_MAX  			2
#define FEAT_DEI_SYS26_CHECK			TRUE

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
#define FEAT_DEI_SYS27_MIN  			1
#define FEAT_DEI_SYS27_MAX  			11
#define FEAT_DEI_SYS27_CHECK			TRUE

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
#define FEAT_DEI_SYS28_MIN  			1
#define FEAT_DEI_SYS28_MAX  			2
#define FEAT_DEI_SYS28_CHECK			TRUE

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
#define FEAT_DEI_SYS29_MIN  			1
#define FEAT_DEI_SYS29_MAX  			2
#define FEAT_DEI_SYS29_CHECK			TRUE

#if FEAT_DEI_SYS29_CHECK==TRUE
    #if FEAT_DEI_SYS29<FEAT_DEI_SYS29_MIN
        #error FEAT_DEI_SYS29 MIN ERROR
    #endif

    #if FEAT_DEI_SYS29>FEAT_DEI_SYS29_MAX
        #error FEAT_DEI_SYS29 MAX ERROR
    #endif
#endif

//<e0>OEM Alarm Disarm With Unlock Enable
//<o1>OEM Alarm Disarm With Unlock
//         <1 =>*Disable
//         <2 => Enable
//</e>
#define FEAT_DEI_SYS30_MIN  			1
#define FEAT_DEI_SYS30_MAX  			2
#define FEAT_DEI_SYS30_CHECK			TRUE

#if FEAT_DEI_SYS30_CHECK==TRUE
    #if FEAT_DEI_SYS30<FEAT_DEI_SYS30_MIN
        #error FEAT_DEI_SYS30 MIN ERROR
    #endif

    #if FEAT_DEI_SYS30>FEAT_DEI_SYS30_MAX
        #error FEAT_DEI_SYS30 MAX ERROR
    #endif
#endif



// </h>

//--------------------------------------------------------------------------//
// <h>  Programmable Outputs
//--------------------------------------------------------------------------//

//<e0>Output 1 option is visible on web
//<o1>Output1 (-)Lt. Green/White CN2[10]
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
//         <17=> Smartkey control (for PTS)
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
//         <29=> Reserved
//</e>
#define FEAT_DEI_SYS31_MIN  			1
#define FEAT_DEI_SYS31_MAX  			29
#define FEAT_DEI_SYS31_CHECK			TRUE

#if FEAT_DEI_SYS31_CHECK==TRUE
    #if FEAT_DEI_SYS31<FEAT_DEI_SYS31_MIN
        #error FEAT_DEI_SYS31 MIN ERROR
    #endif

    #if FEAT_DEI_SYS31>FEAT_DEI_SYS31_MAX
        #error FEAT_DEI_SYS31 MAX ERROR
    #endif
#endif


//<e0>Output 2 option is visible on web
//<o1>Output2  (-)Blue/White CN3[18]
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
//         <17=> Smartkey control (for PTS)
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
//         <29=> Reserved
//</e>
#define FEAT_DEI_SYS32_MIN  			1
#define FEAT_DEI_SYS32_MAX  			29
#define FEAT_DEI_SYS32_CHECK			TRUE

#if FEAT_DEI_SYS32_CHECK==TRUE
    #if FEAT_DEI_SYS32<FEAT_DEI_SYS32_MIN
        #error FEAT_DEI_SYS32 MIN ERROR
    #endif

    #if FEAT_DEI_SYS32>FEAT_DEI_SYS32_MAX
        #error FEAT_DEI_SYS32 MAX ERROR
    #endif
#endif


//<e0>Output 3 option is visible on web
//<o1>Output3 (-)Dk. Green CN3[15]
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
//         <17=> Smartkey control (for PTS)
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
//         <29=> Reserved
//</e>
#define FEAT_DEI_SYS33_MIN  			1
#define FEAT_DEI_SYS33_MAX  			29
#define FEAT_DEI_SYS33_CHECK			TRUE

#if FEAT_DEI_SYS33_CHECK==TRUE
    #if FEAT_DEI_SYS33<FEAT_DEI_SYS33_MIN
        #error FEAT_DEI_SYS33 MIN ERROR
    #endif

    #if FEAT_DEI_SYS33>FEAT_DEI_SYS33_MAX
        #error FEAT_DEI_SYS33 MAX ERROR
    #endif
#endif


//<e0>Output 4 option is visible on web
//<o1>Output4 (-)Red/White CN3[9]
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
//         <17=> Smartkey control (for PTS)
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
//         <29=> Reserved
//</e>
#define FEAT_DEI_SYS34_MIN  			1
#define FEAT_DEI_SYS34_MAX  			29
#define FEAT_DEI_SYS34_CHECK			TRUE

#if FEAT_DEI_SYS34_CHECK==TRUE
    #if FEAT_DEI_SYS34<FEAT_DEI_SYS34_MIN
        #error FEAT_DEI_SYS34 MIN ERROR
    #endif

    #if FEAT_DEI_SYS34>FEAT_DEI_SYS34_MAX
        #error FEAT_DEI_SYS34 MAX ERROR
    #endif
#endif


//<e0>Output 5 option is visible on web
//<o1>Output5 (-)Dk.Blue CN3[7]
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
//         <17=> Smartkey control (for PTS)
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
//         <29=> Reserved
//</e>
#define FEAT_DEI_SYS35_MIN  			1
#define FEAT_DEI_SYS35_MAX  			29
#define FEAT_DEI_SYS35_CHECK			TRUE

#if FEAT_DEI_SYS35_CHECK==TRUE
    #if FEAT_DEI_SYS35<FEAT_DEI_SYS35_MIN
        #error FEAT_DEI_SYS35 MIN ERROR
    #endif

    #if FEAT_DEI_SYS35>FEAT_DEI_SYS35_MAX
        #error FEAT_DEI_SYS35 MAX ERROR
    #endif
#endif


//<e0>Output 6 option is visible on web
//<o1>Output6  (+)Green/Red CN2[14]
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
//         <17=> Smartkey control (for PTS)
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
//         <29=> Reserved
//</e>
#define FEAT_DEI_SYS36_MIN  			1
#define FEAT_DEI_SYS36_MAX  			29
#define FEAT_DEI_SYS36_CHECK			TRUE

#if FEAT_DEI_SYS36_CHECK==TRUE
    #if FEAT_DEI_SYS36<FEAT_DEI_SYS36_MIN
        #error FEAT_DEI_SYS36 MIN ERROR
    #endif

    #if FEAT_DEI_SYS36>FEAT_DEI_SYS36_MAX
        #error FEAT_DEI_SYS36 MAX ERROR
    #endif
#endif


//<e0>Output 7 option is visible on web
//<o1>Output7  (+/-)White/Red CN2[12](+),White/Violet CN2[11](-)
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
//         <17=> Smartkey control (for PTS)
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
//         <29=> Reserved
//</e>
#define FEAT_DEI_SYS37_MIN  			1
#define FEAT_DEI_SYS37_MAX  			29
#define FEAT_DEI_SYS37_CHECK			TRUE

#if FEAT_DEI_SYS37_CHECK==TRUE
    #if FEAT_DEI_SYS37<FEAT_DEI_SYS37_MIN
        #error FEAT_DEI_SYS37 MIN ERROR
    #endif

    #if FEAT_DEI_SYS37>FEAT_DEI_SYS37_MAX
        #error FEAT_DEI_SYS37 MAX ERROR
    #endif
#endif


//<e0>Output 8 option is visible on web
//<o1>Output8  (-)Pink/White CN9[3]
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
//         <17=> Smartkey control (for PTS)
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
//         <29=> Reserved
//</e>
#define FEAT_DEI_SYS38_MIN  			1
#define FEAT_DEI_SYS38_MAX  			29
#define FEAT_DEI_SYS38_CHECK			TRUE

#if FEAT_DEI_SYS38_CHECK==TRUE
    #if FEAT_DEI_SYS38<FEAT_DEI_SYS38_MIN
        #error FEAT_DEI_SYS38 MIN ERROR
    #endif

    #if FEAT_DEI_SYS38>FEAT_DEI_SYS38_MAX
        #error FEAT_DEI_SYS38 MAX ERROR
    #endif
#endif

//<e0>Output 9 option is visible on web
//<o1>Output9  (-)Orange CN9[4]
//         <20=> Accessory
//         <29=> Reserved
//</e>
#define FEAT_DEI_SYS39_MIN  			20
#define FEAT_DEI_SYS39_MAX  			20
#define FEAT_DEI_SYS39_CHECK			TRUE

#if FEAT_DEI_SYS39_CHECK==TRUE
    #if FEAT_DEI_SYS39<FEAT_DEI_SYS39_MIN
        #error FEAT_DEI_SYS39 MIN ERROR
    #endif

    #if FEAT_DEI_SYS39>FEAT_DEI_SYS39_MAX
        #error FEAT_DEI_SYS39 MAX ERROR
    #endif
#endif


//<e0>Output 10 option is visible on web
//<o1>Output10  (-)Pink/Black CN9[5]
//         <21=> Ignition
//         <29=> Reserved
//</e>
#define FEAT_DEI_SYS40_MIN  			21
#define FEAT_DEI_SYS40_MAX  			21
#define FEAT_DEI_SYS40_CHECK			TRUE

#if FEAT_DEI_SYS40_CHECK==TRUE
    #if FEAT_DEI_SYS40<FEAT_DEI_SYS40_MIN
        #error FEAT_DEI_SYS40 MIN ERROR
    #endif

    #if FEAT_DEI_SYS40>FEAT_DEI_SYS40_MAX
        #error FEAT_DEI_SYS40 MAX ERROR
    #endif
#endif


//<e0>Output 11 option is visible on web
//<o1>Output11  (-)Purple CN9[6]
//         <22=> Starter
//         <29=> Reserved
//</e>
#define FEAT_DEI_SYS41_MIN  			22
#define FEAT_DEI_SYS41_MAX  			22
#define FEAT_DEI_SYS41_CHECK			TRUE

#if FEAT_DEI_SYS41_CHECK==TRUE
    #if FEAT_DEI_SYS41<FEAT_DEI_SYS41_MIN
        #error FEAT_DEI_SYS41 MIN ERROR
    #endif

    #if FEAT_DEI_SYS41>FEAT_DEI_SYS41_MAX
        #error FEAT_DEI_SYS41 MAX ERROR
    #endif
#endif


//<e0>Output 12 option is visible on web
//<o1>Output12  (-)Orange/White CN9[1]
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
//         <17=> Smartkey control (for PTS)
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
//         <28=> Anti-Grind 
//         <29=> Reserved
//</e>
#define FEAT_DEI_SYS42_MIN  			1
#define FEAT_DEI_SYS42_MAX  			29
#define FEAT_DEI_SYS42_CHECK			TRUE

#if FEAT_DEI_SYS42_CHECK==TRUE
    #if FEAT_DEI_SYS42<FEAT_DEI_SYS42_MIN
        #error FEAT_DEI_SYS42 MIN ERROR
    #endif

    #if FEAT_DEI_SYS42>FEAT_DEI_SYS42_MAX
        #error FEAT_DEI_SYS42 MAX ERROR
    #endif
#endif


//<e0>Output 13 option is visible on web
//<o1>Output13  (-)Lt.Green/Black CN2[13]
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
//         <17=> Smartkey control (for PTS)
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
//         <29=> Reserved
//</e>
#define FEAT_DEI_SYS43_MIN  			1
#define FEAT_DEI_SYS43_MAX  			29
#define FEAT_DEI_SYS43_CHECK			TRUE

#if FEAT_DEI_SYS43_CHECK==TRUE
    #if FEAT_DEI_SYS43<FEAT_DEI_SYS43_MIN
        #error FEAT_DEI_SYS43 MIN ERROR
    #endif

    #if FEAT_DEI_SYS43>FEAT_DEI_SYS43_MAX
        #error FEAT_DEI_SYS43 MAX ERROR
    #endif
#endif


//<e0>Output 14 option is visible on web
//<o1>Output14  (-)Brown/Black CN3[8]
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
//         <17=> Smartkey control (for PTS)
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
//         <29=> Reserved
//</e>
#define FEAT_DEI_SYS44_MIN  			1
#define FEAT_DEI_SYS44_MAX  			29
#define FEAT_DEI_SYS44_CHECK			TRUE

#if FEAT_DEI_SYS44_CHECK==TRUE
    #if FEAT_DEI_SYS44<FEAT_DEI_SYS44_MIN
        #error FEAT_DEI_SYS44 MIN ERROR
    #endif

    #if FEAT_DEI_SYS44>FEAT_DEI_SYS44_MAX
        #error FEAT_DEI_SYS44 MAX ERROR
    #endif
#endif


//<e0>Relay 1 option is visible on web
//<o1>Relay1  COM-Yellow[7] , NC-Orange/Yellow[2] , NO-Brown[1]
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
//         <17=> Smartkey control (for PTS)
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
//         <29=> Reserved
//</e>
#define FEAT_DEI_SYS45_MIN  			1
#define FEAT_DEI_SYS45_MAX  			29
#define FEAT_DEI_SYS45_CHECK			TRUE

#if FEAT_DEI_SYS45_CHECK==TRUE
    #if FEAT_DEI_SYS45<FEAT_DEI_SYS45_MIN
        #error FEAT_DEI_SYS45 MIN ERROR
    #endif

    #if FEAT_DEI_SYS45>FEAT_DEI_SYS45_MAX
        #error FEAT_DEI_SYS45 MAX ERROR
    #endif
#endif



//<e0>Relay 2 option is visible on web
//<o1>Relay2  COM-Yellow/Red[9] , NC-Orange/Red[8] , NO-Brown/Red[10]
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
//         <17=> Smartkey control (for PTS)
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
//         <29=> Reserved
//</e>
#define FEAT_DEI_SYS46_MIN  			1
#define FEAT_DEI_SYS46_MAX  			29
#define FEAT_DEI_SYS46_CHECK			TRUE

#if FEAT_DEI_SYS46_CHECK==TRUE
    #if FEAT_DEI_SYS46<FEAT_DEI_SYS46_MIN
        #error FEAT_DEI_SYS46 MIN ERROR
    #endif

    #if FEAT_DEI_SYS46>FEAT_DEI_SYS46_MAX
        #error FEAT_DEI_SYS46 MAX ERROR
    #endif
#endif


//<e0>Relay 3 option is visible on web
//<o1>Relay3  COM-White[12,6] , NO-White/Brown[5,11] 
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
//         <17=> Smartkey control (for PTS)
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
//         <29=> Reserved
//</e>
#define FEAT_DEI_SYS47_MIN  			1
#define FEAT_DEI_SYS47_MAX  			29
#define FEAT_DEI_SYS47_CHECK			TRUE

#if FEAT_DEI_SYS47_CHECK==TRUE
    #if FEAT_DEI_SYS47<FEAT_DEI_SYS47_MIN
        #error FEAT_DEI_SYS47 MIN ERROR
    #endif

    #if FEAT_DEI_SYS47>FEAT_DEI_SYS47_MAX
        #error FEAT_DEI_SYS47 MAX ERROR
    #endif
#endif

// </h>

//--------------------------------------------------------------------------//
// <h>  Programmable Inputs
//--------------------------------------------------------------------------//
//<e0>Input Negative 1 option is visible on web
//<o1>Input Negative 1(Blue/Black CN3[14]) 
//         <9=>  Start trigger 
//         <10=> Reserved
//</e>
#define FEAT_DEI_SYS48_MIN  			1
#define FEAT_DEI_SYS48_MAX  			14
#define FEAT_DEI_SYS48_CHECK			TRUE

#if FEAT_DEI_SYS48_CHECK==TRUE
    #if FEAT_DEI_SYS48<FEAT_DEI_SYS48_MIN
        #error FEAT_DEI_SYS48 MIN ERROR
    #endif

    #if FEAT_DEI_SYS48>FEAT_DEI_SYS48_MAX
        #error FEAT_DEI_SYS48 MAX ERROR
    #endif
#endif


//<e0>Input Negative 2 option is visible on web
//<o1>Input Negative 2(Lt.Blue/Red  CN3[1]) 
//         <1=>  Handbrake
//         <2=>  Safety Switch
//         <3=>  Hood
//         <4=>  Trunk
//         <5=>  Door
//         <9=>  Start Trigger
//         <10=> Custom Neg1
//         <11=> Custom Neg2 *
//         <12=> Custom Neg3
//</e>
#define FEAT_DEI_SYS49_MIN  			1
#define FEAT_DEI_SYS49_MAX  			14
#define FEAT_DEI_SYS49_CHECK			TRUE

#if FEAT_DEI_SYS49_CHECK==TRUE
    #if FEAT_DEI_SYS49<FEAT_DEI_SYS49_MIN
        #error FEAT_DEI_SYS49 MIN ERROR
    #endif

    #if FEAT_DEI_SYS49>FEAT_DEI_SYS49_MAX
        #error FEAT_DEI_SYS49 MAX ERROR
    #endif
#endif



//<e0>Input Negative 3 option is visible on web
//<o1>Input Negative 3(Blue/Red CN3[12]) 
//         <1=>  Handbrake
//         <2=>  Safety Switch
//         <3=>  Hood
//         <4=>  Trunk
//         <5=>  Door
//         <9=>  Start Trigger
//         <10=> Custom Neg1
//         <11=> Custom Neg2 *
//         <12=> Custom Neg3
//</e>
#define FEAT_DEI_SYS50_MIN  			1
#define FEAT_DEI_SYS50_MAX  			14
#define FEAT_DEI_SYS50_CHECK			TRUE


#if FEAT_DEI_SYS50_CHECK==TRUE
    #if FEAT_DEI_SYS50<FEAT_DEI_SYS50_MIN
        #error FEAT_DEI_SYS50 MIN ERROR
    #endif

    #if FEAT_DEI_SYS50>FEAT_DEI_SYS50_MAX
        #error FEAT_DEI_SYS50 MAX ERROR
    #endif
#endif


//<e0>Input Negative 4 option is visible on web
//<o1>Input Negative 4(White/Black CN3[2]) 
//         <1=>  Handbrake *
//         <2=>  Safety Switch
//         <3=>  Hood
//         <4=>  Trunk
//         <5=>  Door
//         <9=>  Start trigger 
//         <10=> Custom Neg1
//         <11=> Custom Neg2 
//         <12=> Custom Neg3 
//</e>
#define FEAT_DEI_SYS51_MIN  			1
#define FEAT_DEI_SYS51_MAX  			14
#define FEAT_DEI_SYS51_CHECK			TRUE

#if FEAT_DEI_SYS51_CHECK==TRUE
    #if FEAT_DEI_SYS51<FEAT_DEI_SYS51_MIN
        #error FEAT_DEI_SYS51 MIN ERROR
    #endif

    #if FEAT_DEI_SYS51>FEAT_DEI_SYS51_MAX
        #error FEAT_DEI_SYS51 MAX ERROR
    #endif
#endif


//<e0>Input Negative 5 option is visible on web
//<o1>Input Negative 5(Gray  CN3[3]) 
//         <1=>  Handbrake
//         <2=>  Safety Switch
//         <3=>  Hood *
//         <4=>  Trunk
//         <5=>  Door
//         <9=>  Start trigger 
//         <10=> Custom Neg1
//         <11=> Custom Neg2 
//         <12=> Custom Neg3 
//</e>
#define FEAT_DEI_SYS52_MIN  			1
#define FEAT_DEI_SYS52_MAX  			14
#define FEAT_DEI_SYS52_CHECK			TRUE

#if FEAT_DEI_SYS52_CHECK==TRUE
    #if FEAT_DEI_SYS52<FEAT_DEI_SYS52_MIN
        #error FEAT_DEI_SYS52 MIN ERROR
    #endif

    #if FEAT_DEI_SYS52>FEAT_DEI_SYS52_MAX
        #error FEAT_DEI_SYS52 MAX ERROR
    #endif
#endif


//<e0>Input Negative 6 option is visible on web
//<o1>Input Negative 6(Black/White CN3[4]) 
//         <1=>  Handbrake
//         <2=>  Safety Switch *
//         <3=>  Hood
//         <4=>  Trunk
//         <5=>  Door
//         <9=>  Start trigger 
//         <10=> Custom Neg1
//         <11=> Custom Neg2 
//         <12=> Custom Neg3 
//</e>
#define FEAT_DEI_SYS53_MIN  			1
#define FEAT_DEI_SYS53_MAX  			14
#define FEAT_DEI_SYS53_CHECK			TRUE

#if FEAT_DEI_SYS53_CHECK==TRUE
    #if FEAT_DEI_SYS53<FEAT_DEI_SYS53_MIN
        #error FEAT_DEI_SYS53 MIN ERROR
    #endif

    #if FEAT_DEI_SYS53>FEAT_DEI_SYS53_MAX
        #error FEAT_DEI_SYS53 MAX ERROR
    #endif
#endif


//<e0>Input Negative 7 option is visible on web
//<o1>Input Negative 7(Lt.Blue CN3[13]) 
//         <1=>  Handbrake
//         <2=>  Safety Switch
//         <3=>  Hood
//         <4=>  Trunk *
//         <5=>  Door
//         <9=>  Start trigger 
//         <10=> Custom Neg1
//         <11=> Custom Neg2 
//         <12=> Custom Neg3 
//</e>
#define FEAT_DEI_SYS54_MIN  			1
#define FEAT_DEI_SYS54_MAX  			14
#define FEAT_DEI_SYS54_CHECK			TRUE

#if FEAT_DEI_SYS54_CHECK==TRUE
    #if FEAT_DEI_SYS54<FEAT_DEI_SYS54_MIN
        #error FEAT_DEI_SYS54 MIN ERROR
    #endif

    #if FEAT_DEI_SYS54>FEAT_DEI_SYS54_MAX
        #error FEAT_DEI_SYS54 MAX ERROR
    #endif
#endif


//<e0>Input Positive 1 option is visible on web
//<o1>Input Positive 1(Gray/Black CN3[5]) 
//         <5=>  Door
//         <6=>  Brake
//         <7=>  Glowplug *
//         <8=>  Ignition
//         <12=> Custom Pos1
//         <13=> Custom Pos2
//</e>
#define FEAT_DEI_SYS55_MIN  			1
#define FEAT_DEI_SYS55_MAX  			14
#define FEAT_DEI_SYS55_CHECK			TRUE

#if FEAT_DEI_SYS55_CHECK==TRUE
    #if FEAT_DEI_SYS55<FEAT_DEI_SYS55_MIN
        #error FEAT_DEI_SYS55 MIN ERROR
    #endif

    #if FEAT_DEI_SYS55>FEAT_DEI_SYS55_MAX
        #error FEAT_DEI_SYS55 MAX ERROR
    #endif
#endif


//<e0>Input Positive 2 option is visible on web
//<o1>Input Positive 2 (Lt.Brown CN3[16]) 
//         <5=>  Door
//         <6=>  Brake *
//         <7=>  Glowplug
//         <8=>  Ignition
//         <12=> Custom Pos1
//         <13=> Custom Pos2
//</e>
#define FEAT_DEI_SYS56_MIN  			1
#define FEAT_DEI_SYS56_MAX  			14
#define FEAT_DEI_SYS56_CHECK			TRUE

#if FEAT_DEI_SYS56_CHECK==TRUE
    #if FEAT_DEI_SYS56<FEAT_DEI_SYS56_MIN
        #error FEAT_DEI_SYS56 MIN ERROR
    #endif

    #if FEAT_DEI_SYS56>FEAT_DEI_SYS56_MAX
        #error FEAT_DEI_SYS56 MAX ERROR
    #endif
#endif


//<e0>Input Positive 3 option is visible on web
//<o1>Input Positive 3 (Pink CN9[7]) 
//         <5=>  Door
//         <6=>  Brake
//         <7=>  Glowplug
//         <8=>  Ignition *
//         <12=> Custom Pos1
//         <13=> Custom Pos2
//</e>
#define FEAT_DEI_SYS57_MIN  			1
#define FEAT_DEI_SYS57_MAX  			14
#define FEAT_DEI_SYS57_CHECK			TRUE

#if FEAT_DEI_SYS57_CHECK==TRUE
    #if FEAT_DEI_SYS57<FEAT_DEI_SYS57_MIN
        #error FEAT_DEI_SYS57 MIN ERROR
    #endif

    #if FEAT_DEI_SYS57>FEAT_DEI_SYS57_MAX
        #error FEAT_DEI_SYS57 MAX ERROR
    #endif
#endif


//<e0>Input Negative/Positive 1 option is visible on web
//<o1>Input Negative/Positive 1 (Green/Gray CN3[10](-) , Purple/Grey  CN3[11](+)) 
//         <1=>  Handbrake
//         <2=>  Safety Switch
//         <3=>  Hood
//         <4=>  Trunk
//         <5=>  Door *
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
#define FEAT_DEI_SYS58_MIN  			1
#define FEAT_DEI_SYS58_MAX  			14
#define FEAT_DEI_SYS58_CHECK			TRUE

#if FEAT_DEI_SYS58_CHECK==TRUE
    #if FEAT_DEI_SYS58<FEAT_DEI_SYS58_MIN
        #error FEAT_DEI_SYS58 MIN ERROR
    #endif

    #if FEAT_DEI_SYS58>FEAT_DEI_SYS58_MAX
        #error FEAT_DEI_SYS58 MAX ERROR
    #endif
#endif


// </h>

//--------------------------------------------------------------------------//
// <h>  Sense config
//--------------------------------------------------------------------------//
//<e0>Sense doors status Enable
//<o1>Sense doors status
//         <1=> *Auto
//         <2=> Wire
//         <3=> Data
//</e>
#define FEAT_DEI_SYS59_MIN  			1
#define FEAT_DEI_SYS59_MAX  			3
#define FEAT_DEI_SYS59_CHECK			TRUE

#if FEAT_DEI_SYS59_CHECK==TRUE
    #if FEAT_DEI_SYS59<FEAT_DEI_SYS59_MIN
        #error FEAT_DEI_SYS59 MIN ERROR
    #endif

    #if FEAT_DEI_SYS59>FEAT_DEI_SYS59_MAX
        #error FEAT_DEI_SYS59 MAX ERROR
    #endif
#endif


//<e0>Sense ignition status Enable
//<o1>Sense ignition status
//         <1=> *Auto
//         <2=> Wire
//         <3=> Data
//</e>
#define FEAT_DEI_SYS60_MIN  			1
#define FEAT_DEI_SYS60_MAX  			3
#define FEAT_DEI_SYS60_CHECK			TRUE

#if FEAT_DEI_SYS60_CHECK==TRUE
    #if FEAT_DEI_SYS60<FEAT_DEI_SYS60_MIN
        #error FEAT_DEI_SYS60 MIN ERROR
    #endif

    #if FEAT_DEI_SYS60>FEAT_DEI_SYS60_MAX
        #error FEAT_DEI_SYS60 MAX ERROR
    #endif
#endif


//<e0>Sense hood status Enable
//<o1>Sense hood status
//         <1=> *Auto
//         <2=> Wire
//         <3=> Data
//</e>
#define FEAT_DEI_SYS61_MIN  			1
#define FEAT_DEI_SYS61_MAX  			3
#define FEAT_DEI_SYS61_CHECK			TRUE

#if FEAT_DEI_SYS61_CHECK==TRUE
    #if FEAT_DEI_SYS61<FEAT_DEI_SYS61_MIN
        #error FEAT_DEI_SYS61 MIN ERROR
    #endif

    #if FEAT_DEI_SYS61>FEAT_DEI_SYS61_MAX
        #error FEAT_DEI_SYS61 MAX ERROR
    #endif
#endif


//<e0> Sense trunk status Enable
// <o1>Sense trunk status
//         <1=> *Auto
//         <2=> Wire
//         <3=> Data
//</e>
#define FEAT_DEI_SYS62_MIN  			1
#define FEAT_DEI_SYS62_MAX  			3
#define FEAT_DEI_SYS62_CHECK			TRUE

#if FEAT_DEI_SYS62_CHECK==TRUE
    #if FEAT_DEI_SYS62<FEAT_DEI_SYS62_MIN
        #error FEAT_DEI_SYS62 MIN ERROR
    #endif

    #if FEAT_DEI_SYS62>FEAT_DEI_SYS62_MAX
        #error FEAT_DEI_SYS62 MAX ERROR
    #endif
#endif


//<e0> Sense Handbrake Enable
//<o1>Sense handbrake status
//         <1=> *Auto
//         <2=> Wire
//         <3=> Data
//</e>
#define FEAT_DEI_SYS63_MIN  			1
#define FEAT_DEI_SYS63_MAX  			3
#define FEAT_DEI_SYS63_CHECK			TRUE

#if FEAT_DEI_SYS63_CHECK==TRUE
    #if FEAT_DEI_SYS63<FEAT_DEI_SYS63_MIN
        #error FEAT_DEI_SYS63 MIN ERROR
    #endif

    #if FEAT_DEI_SYS63>FEAT_DEI_SYS63_MAX
        #error FEAT_DEI_SYS63 MAX ERROR
    #endif
#endif



//<e0>Sense Tacho Enable
//<o1>Sense tacho status
//         <1=> *Auto
//         <2=> Wire
//         <3=> Data
//</e>
#define FEAT_DEI_SYS64_MIN  			1
#define FEAT_DEI_SYS64_MAX  			3
#define FEAT_DEI_SYS64_CHECK			TRUE

#if FEAT_DEI_SYS64_CHECK==TRUE
    #if FEAT_DEI_SYS64<FEAT_DEI_SYS64_MIN
        #error FEAT_DEI_SYS64 MIN ERROR
    #endif

    #if FEAT_DEI_SYS64>FEAT_DEI_SYS64_MAX
        #error FEAT_DEI_SYS64 MAX ERROR
    #endif
#endif



//<e0>Sense brake Enable
//<o1>Sense brake status
//         <1=> *Auto
//         <2=> Wire
//         <3=> Data
//</e>
#define FEAT_DEI_SYS65_MIN  			1
#define FEAT_DEI_SYS65_MAX  			3
#define FEAT_DEI_SYS65_CHECK			TRUE

#if FEAT_DEI_SYS65_CHECK==TRUE
    #if FEAT_DEI_SYS65<FEAT_DEI_SYS65_MIN
        #error FEAT_DEI_SYS65 MIN ERROR
    #endif

    #if FEAT_DEI_SYS65>FEAT_DEI_SYS65_MAX
        #error FEAT_DEI_SYS65 MAX ERROR
    #endif
#endif


//<e0> Sense glowplug Enable
//<o1>Sense glowplug status
//         <1=> *Auto
//         <2=> Wire
//         <3=> Data
//</e>
#define FEAT_DEI_SYS66_MIN  			1
#define FEAT_DEI_SYS66_MAX  			3
#define FEAT_DEI_SYS66_CHECK			TRUE

#if FEAT_DEI_SYS66_CHECK==TRUE
    #if FEAT_DEI_SYS66<FEAT_DEI_SYS66_MIN
        #error FEAT_DEI_SYS66 MIN ERROR
    #endif

    #if FEAT_DEI_SYS66>FEAT_DEI_SYS66_MAX
        #error FEAT_DEI_SYS66 MAX ERROR
    #endif
#endif



//<e0>Sense OEM LOCK Enable
//<o1>Sense oem lock
//         <1=> Disable             
//         <2=> *Enable
//</e>
#define FEAT_DEI_SYS67_MIN  			1
#define FEAT_DEI_SYS67_MAX  			2
#define FEAT_DEI_SYS67_CHECK			TRUE

#if FEAT_DEI_SYS67_CHECK==TRUE
    #if FEAT_DEI_SYS67<FEAT_DEI_SYS67_MIN
        #error FEAT_DEI_SYS67 MIN ERROR
    #endif

    #if FEAT_DEI_SYS67>FEAT_DEI_SYS67_MAX
        #error FEAT_DEI_SYS67 MAX ERROR
    #endif
#endif



//<e0>Sense OEM UNLOCK Enable
//<o1>Sense oem unlock
//         <1=> Disable
//         <2=> *Enable
//</e>
#define FEAT_DEI_SYS68_MIN  			1
#define FEAT_DEI_SYS68_MAX  			2
#define FEAT_DEI_SYS68_CHECK			TRUE


#if FEAT_DEI_SYS68_CHECK==TRUE
    #if FEAT_DEI_SYS68<FEAT_DEI_SYS68_MIN
        #error FEAT_DEI_SYS68 MIN ERROR
    #endif

    #if FEAT_DEI_SYS68>FEAT_DEI_SYS68_MAX
        #error FEAT_DEI_SYS68 MAX ERROR
    #endif
#endif


//<e0>Sense OEM TRUNK Enable
//<o1>Sense oem trunk
//         <1=> Disable            
//         <2=> *Enable
//</e>
#define FEAT_DEI_SYS69_MIN  			1
#define FEAT_DEI_SYS69_MAX  			2
#define FEAT_DEI_SYS69_CHECK			TRUE

#if FEAT_DEI_SYS69_CHECK==TRUE
    #if FEAT_DEI_SYS69<FEAT_DEI_SYS69_MIN
        #error FEAT_DEI_SYS69 MIN ERROR
    #endif

    #if FEAT_DEI_SYS69>FEAT_DEI_SYS69_MAX
        #error FEAT_DEI_SYS69 MAX ERROR
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
#define FEAT_DEI_SYS70_MIN  			1
#define FEAT_DEI_SYS70_MAX  			3
#define FEAT_DEI_SYS70_CHECK			FALSE

#if FEAT_DEI_SYS70_CHECK==TRUE
    #if FEAT_DEI_SYS70<FEAT_DEI_SYS70_MIN
        #error FEAT_DEI_SYS70 MIN ERROR
    #endif

    #if FEAT_DEI_SYS70>FEAT_DEI_SYS70_MAX
        #error FEAT_DEI_SYS70 MAX ERROR
    #endif
#endif


//<e0>Feature Custom 01 Enable
//<o1>MTS completion
//         <1=> *By Remote
//         <2=>  MTS completion by door open/close
//</e>
#define FEAT_DEI_SYS71_MIN  			1
#define FEAT_DEI_SYS71_MAX  			2
#define FEAT_DEI_SYS71_CHECK			FALSE

#if FEAT_DEI_SYS71_CHECK==TRUE
    #if FEAT_DEI_SYS71<FEAT_DEI_SYS71_MIN
        #error FEAT_DEI_SYS71 MIN ERROR
    #endif

    #if FEAT_DEI_SYS71>FEAT_DEI_SYS71_MAX
        #error FEAT_DEI_SYS71 MAX ERROR
    #endif
#endif


//<e0>Feature Custom 02 Enable
//<o1>Trunk Output
//         <1=> *Constant Output
//         <2=>  1-sec Pulse Output
//</e>
#define FEAT_DEI_SYS72_MIN  			1
#define FEAT_DEI_SYS72_MAX  			2
#define FEAT_DEI_SYS72_CHECK			FALSE

#if FEAT_DEI_SYS72_CHECK==TRUE
    #if FEAT_DEI_SYS72<FEAT_DEI_SYS72_MIN
        #error FEAT_DEI_SYS72 MIN ERROR
    #endif

    #if FEAT_DEI_SYS72>FEAT_DEI_SYS72_MAX
        #error FEAT_DEI_SYS72 MAX ERROR
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
#define FEAT_DEI_SYS73_MIN  			1
#define FEAT_DEI_SYS73_MAX  			3
#define FEAT_DEI_SYS73_CHECK			FALSE

#if FEAT_DEI_SYS73_CHECK==TRUE
    #if FEAT_DEI_SYS73<FEAT_DEI_SYS73_MIN
        #error FEAT_DEI_SYS73 MIN ERROR
    #endif

    #if FEAT_DEI_SYS73>FEAT_DEI_SYS73_MAX
        #error FEAT_DEI_SYS73 MAX ERROR
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
#define FEAT_DEI_SYS74_MIN  			1
#define FEAT_DEI_SYS74_MAX  			3
#define FEAT_DEI_SYS74_CHECK			FALSE

#if FEAT_DEI_SYS74_CHECK==TRUE
    #if FEAT_DEI_SYS74<FEAT_DEI_SYS74_MIN
        #error FEAT_DEI_SYS74 MIN ERROR
    #endif

    #if FEAT_DEI_SYS74>FEAT_DEI_SYS74_MAX
        #error FEAT_DEI_SYS74 MAX ERROR
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
#define FEAT_DEI_SYS75_MIN  			1
#define FEAT_DEI_SYS75_MAX  			3
#define FEAT_DEI_SYS75_CHECK			FALSE

#if FEAT_DEI_SYS75_CHECK==TRUE
    #if FEAT_DEI_SYS75<FEAT_DEI_SYS75_MIN
        #error FEAT_DEI_SYS75 MIN ERROR
    #endif

    #if FEAT_DEI_SYS75>FEAT_DEI_SYS75_MAX
        #error FEAT_DEI_SYS75 MAX ERROR
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
#define FEAT_DEI_SYS76_MIN  			1
#define FEAT_DEI_SYS76_MAX  			3
#define FEAT_DEI_SYS76_CHECK			FALSE

#if FEAT_DEI_SYS76_CHECK==TRUE
    #if FEAT_DEI_SYS76<FEAT_DEI_SYS76_MIN
        #error FEAT_DEI_SYS76 MIN ERROR
    #endif

    #if FEAT_DEI_SYS76>FEAT_DEI_SYS76_MAX
        #error FEAT_DEI_SYS76 MAX ERROR
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
#define FEAT_DEI_SYS77_MIN  			1
#define FEAT_DEI_SYS77_MAX  			3
#define FEAT_DEI_SYS77_CHECK			FALSE

#if FEAT_DEI_SYS77_CHECK==TRUE
    #if FEAT_DEI_SYS77<FEAT_DEI_SYS77_MIN
        #error FEAT_DEI_SYS77 MIN ERROR
    #endif

    #if FEAT_DEI_SYS77>FEAT_DEI_SYS77_MAX
        #error FEAT_DEI_SYS77 MAX ERROR
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
#define FEAT_DEI_SYS78_MIN  			1
#define FEAT_DEI_SYS78_MAX  			3
#define FEAT_DEI_SYS78_CHECK			FALSE

#if FEAT_DEI_SYS78_CHECK==TRUE
    #if FEAT_DEI_SYS78<FEAT_DEI_SYS78_MIN
        #error FEAT_DEI_SYS78 MIN ERROR
    #endif

    #if FEAT_DEI_SYS78>FEAT_DEI_SYS78_MAX
        #error FEAT_DEI_SYS78 MAX ERROR
    #endif
#endif


// </h>

/*--------------------------------------------------------------------------*/

const UInt8 dei_feature_default_table[FEAT_DEI_SIZE] = 
{
	/*SS FEATURE*/
	FEAT_DEI_SS01  , FEAT_DEI_SS02  , FEAT_DEI_SS03  , FEAT_DEI_SS04  , FEAT_DEI_SS05  , 
	FEAT_DEI_SS06  , FEAT_DEI_SS07  , FEAT_DEI_SS08  , FEAT_DEI_SS09  , FEAT_DEI_SS10  , 

	/*RS FEATURE*/
	FEAT_DEI_RS01  , FEAT_DEI_RS02  , FEAT_DEI_RS03  , FEAT_DEI_RS04  , FEAT_DEI_RS05  , 
	FEAT_DEI_RS06  , FEAT_DEI_RS07  , FEAT_DEI_RS08  , FEAT_DEI_RS09  , FEAT_DEI_RS10  , 
	FEAT_DEI_RS11  , FEAT_DEI_RS12  , FEAT_DEI_RS13  , FEAT_DEI_RS14  , FEAT_DEI_RS15  , 
	FEAT_DEI_RS16  , FEAT_DEI_RS17  , FEAT_DEI_RS18  , 

	/*SYSTEM FEATURE*/
	FEAT_DEI_SYS01 , FEAT_DEI_SYS02 , FEAT_DEI_SYS03 , FEAT_DEI_SYS04 , FEAT_DEI_SYS05 ,
	FEAT_DEI_SYS06 , FEAT_DEI_SYS07 , FEAT_DEI_SYS08 , FEAT_DEI_SYS09 , FEAT_DEI_SYS10 ,
	FEAT_DEI_SYS11 , FEAT_DEI_SYS12 , FEAT_DEI_SYS13 , FEAT_DEI_SYS14 , FEAT_DEI_SYS15 ,
	FEAT_DEI_SYS16 , FEAT_DEI_SYS17 , FEAT_DEI_SYS18 , FEAT_DEI_SYS19 , FEAT_DEI_SYS20 ,
	FEAT_DEI_SYS21 , FEAT_DEI_SYS22 , FEAT_DEI_SYS23 , FEAT_DEI_SYS24 , FEAT_DEI_SYS25 ,
	FEAT_DEI_SYS26 , FEAT_DEI_SYS27 , FEAT_DEI_SYS28 , FEAT_DEI_SYS29 , FEAT_DEI_SYS30 ,

	/*PROGRAMMABLE OUTPUTS*/
	FEAT_DEI_SYS31 , FEAT_DEI_SYS32 , FEAT_DEI_SYS33 , FEAT_DEI_SYS34 , FEAT_DEI_SYS35 ,
	FEAT_DEI_SYS36 , FEAT_DEI_SYS37 , FEAT_DEI_SYS38 , FEAT_DEI_SYS39 , FEAT_DEI_SYS40 ,
	FEAT_DEI_SYS41 , FEAT_DEI_SYS42 , FEAT_DEI_SYS43 , FEAT_DEI_SYS44 , FEAT_DEI_SYS45 ,
    FEAT_DEI_SYS46 , FEAT_DEI_SYS47 ,

    /*PROGRAMMABLE INPUTS*/
	FEAT_DEI_SYS48 , FEAT_DEI_SYS49 , FEAT_DEI_SYS50 , FEAT_DEI_SYS51 , FEAT_DEI_SYS52 ,
	FEAT_DEI_SYS53 , FEAT_DEI_SYS54 , FEAT_DEI_SYS55 , FEAT_DEI_SYS56 , FEAT_DEI_SYS57 ,
	FEAT_DEI_SYS58 ,

    /*SENSE CONFIG*/
    FEAT_DEI_SYS59 , FEAT_DEI_SYS60 , FEAT_DEI_SYS61 , FEAT_DEI_SYS62 , FEAT_DEI_SYS63 ,
    FEAT_DEI_SYS64 , FEAT_DEI_SYS65 , FEAT_DEI_SYS66 , FEAT_DEI_SYS67 , FEAT_DEI_SYS68 ,
    FEAT_DEI_SYS69 ,
	
	/*SPECIAL FEATURES*/
	FEAT_DEI_SYS70 , FEAT_DEI_SYS71 , FEAT_DEI_SYS72 , FEAT_DEI_SYS73 , FEAT_DEI_SYS74,
    FEAT_DEI_SYS75 , FEAT_DEI_SYS76 , FEAT_DEI_SYS77 , FEAT_DEI_SYS78 , 

	/*DEFAULT SETTINGS FOR TACHO*/     
    TACH_DEFAULT_REF_DIV10 ,		// Default Tach reference divided by 10 (make it one-byte nvfs variable)
    
    /*DEFAULT SETTINGS FOR VIRTUAL TACHO*/    
    VIRTUAL_TACH_DEF ,
};

/*-------------------------------------------------------------------------------------*/

const Dei_Feature_Check dei_feature_check_list[FEAT_DEI_SIZE] = 
{
	/*SS FEATURE*/
	{ FEAT_DEI_SS01_MIN  , FEAT_DEI_SS01_MAX  , FEAT_DEI_SS01_CHECK}, 
	{ FEAT_DEI_SS02_MIN  , FEAT_DEI_SS02_MAX  , FEAT_DEI_SS02_CHECK}, 
    { FEAT_DEI_SS03_MIN  , FEAT_DEI_SS03_MAX  , FEAT_DEI_SS03_CHECK}, 
	{ FEAT_DEI_SS04_MIN  , FEAT_DEI_SS04_MAX  , FEAT_DEI_SS04_CHECK}, 
	{ FEAT_DEI_SS05_MIN  , FEAT_DEI_SS05_MAX  , FEAT_DEI_SS05_CHECK}, 
	{ FEAT_DEI_SS06_MIN  , FEAT_DEI_SS06_MAX  , FEAT_DEI_SS06_CHECK}, 
	{ FEAT_DEI_SS07_MIN  , FEAT_DEI_SS07_MAX  , FEAT_DEI_SS07_CHECK}, 
	{ FEAT_DEI_SS08_MIN  , FEAT_DEI_SS08_MAX  , FEAT_DEI_SS08_CHECK}, 
	{ FEAT_DEI_SS09_MIN  , FEAT_DEI_SS09_MAX  , FEAT_DEI_SS09_CHECK}, 
	{ FEAT_DEI_SS10_MIN  , FEAT_DEI_SS10_MAX  , FEAT_DEI_SS10_CHECK}, 

	/*RS FEATURE*/
	{ FEAT_DEI_RS01_MIN  , FEAT_DEI_RS01_MAX  , FEAT_DEI_RS01_CHECK},    
    { FEAT_DEI_RS02_MIN  , FEAT_DEI_RS02_MAX  , FEAT_DEI_RS02_CHECK},     
    { FEAT_DEI_RS03_MIN  , FEAT_DEI_RS03_MAX  , FEAT_DEI_RS03_CHECK},     
    { FEAT_DEI_RS04_MIN  , FEAT_DEI_RS04_MAX  , FEAT_DEI_RS04_CHECK},     
    { FEAT_DEI_RS05_MIN  , FEAT_DEI_RS05_MAX  , FEAT_DEI_RS05_CHECK},     
	{ FEAT_DEI_RS06_MIN  , FEAT_DEI_RS06_MAX  , FEAT_DEI_RS06_CHECK},     
    { FEAT_DEI_RS07_MIN  , FEAT_DEI_RS07_MAX  , FEAT_DEI_RS07_CHECK},     
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
                                                                       
	/*SYSTEM FEATURE*/                                             
	{ FEAT_DEI_SYS01_MIN , FEAT_DEI_SYS01_MAX , FEAT_DEI_SYS01_CHECK}, 
    { FEAT_DEI_SYS02_MIN , FEAT_DEI_SYS02_MAX , FEAT_DEI_SYS02_CHECK}, 
    { FEAT_DEI_SYS03_MIN , FEAT_DEI_SYS03_MAX , FEAT_DEI_SYS03_CHECK}, 
    { FEAT_DEI_SYS04_MIN , FEAT_DEI_SYS04_MAX , FEAT_DEI_SYS04_CHECK}, 
    { FEAT_DEI_SYS05_MIN , FEAT_DEI_SYS05_MAX , FEAT_DEI_SYS05_CHECK}, 
	{ FEAT_DEI_SYS06_MIN , FEAT_DEI_SYS06_MAX , FEAT_DEI_SYS06_CHECK}, 
    { FEAT_DEI_SYS07_MIN , FEAT_DEI_SYS07_MAX , FEAT_DEI_SYS07_CHECK}, 
    { FEAT_DEI_SYS08_MIN , FEAT_DEI_SYS08_MAX , FEAT_DEI_SYS08_CHECK}, 
    { FEAT_DEI_SYS09_MIN , FEAT_DEI_SYS09_MAX , FEAT_DEI_SYS09_CHECK}, 
    { FEAT_DEI_SYS10_MIN , FEAT_DEI_SYS10_MAX , FEAT_DEI_SYS10_CHECK}, 
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
    { FEAT_DEI_SYS29_MIN , FEAT_DEI_SYS29_MAX , FEAT_DEI_SYS29_CHECK}, 
    { FEAT_DEI_SYS30_MIN , FEAT_DEI_SYS30_MAX , FEAT_DEI_SYS30_CHECK},

	/*PROGRAMMABLE OUTPUTS*/	
    { FEAT_DEI_SYS31_MIN , FEAT_DEI_SYS31_MAX , FEAT_DEI_SYS31_CHECK},
    { FEAT_DEI_SYS32_MIN , FEAT_DEI_SYS32_MAX , FEAT_DEI_SYS32_CHECK},
    { FEAT_DEI_SYS33_MIN , FEAT_DEI_SYS33_MAX , FEAT_DEI_SYS33_CHECK},
    { FEAT_DEI_SYS34_MIN , FEAT_DEI_SYS34_MAX , FEAT_DEI_SYS34_CHECK},
	{ FEAT_DEI_SYS35_MIN , FEAT_DEI_SYS35_MAX , FEAT_DEI_SYS35_CHECK},
    { FEAT_DEI_SYS36_MIN , FEAT_DEI_SYS36_MAX , FEAT_DEI_SYS36_CHECK},
    { FEAT_DEI_SYS37_MIN , FEAT_DEI_SYS37_MAX , FEAT_DEI_SYS37_CHECK},
    { FEAT_DEI_SYS38_MIN , FEAT_DEI_SYS38_MAX , FEAT_DEI_SYS38_CHECK},
    { FEAT_DEI_SYS39_MIN , FEAT_DEI_SYS39_MAX , FEAT_DEI_SYS39_CHECK},
	{ FEAT_DEI_SYS40_MIN , FEAT_DEI_SYS40_MAX , FEAT_DEI_SYS40_CHECK},
    { FEAT_DEI_SYS41_MIN , FEAT_DEI_SYS41_MAX , FEAT_DEI_SYS41_CHECK},
    { FEAT_DEI_SYS42_MIN , FEAT_DEI_SYS42_MAX , FEAT_DEI_SYS42_CHECK},
    { FEAT_DEI_SYS43_MIN , FEAT_DEI_SYS43_MAX , FEAT_DEI_SYS43_CHECK},
	{ FEAT_DEI_SYS44_MIN , FEAT_DEI_SYS44_MAX , FEAT_DEI_SYS44_CHECK},
    { FEAT_DEI_SYS45_MIN , FEAT_DEI_SYS45_MAX , FEAT_DEI_SYS45_CHECK},
    { FEAT_DEI_SYS46_MIN , FEAT_DEI_SYS46_MAX , FEAT_DEI_SYS46_CHECK},
    { FEAT_DEI_SYS47_MIN , FEAT_DEI_SYS47_MAX , FEAT_DEI_SYS47_CHECK},
      
    /*PROGRAMMABLE INPUTS*/	
    { FEAT_DEI_SYS48_MIN , FEAT_DEI_SYS48_MAX , FEAT_DEI_SYS48_CHECK}, 
    { FEAT_DEI_SYS49_MIN , FEAT_DEI_SYS49_MAX , FEAT_DEI_SYS49_CHECK}, 
    { FEAT_DEI_SYS50_MIN , FEAT_DEI_SYS50_MAX , FEAT_DEI_SYS50_CHECK}, 
    { FEAT_DEI_SYS51_MIN , FEAT_DEI_SYS51_MAX , FEAT_DEI_SYS51_CHECK}, 
	{ FEAT_DEI_SYS52_MIN , FEAT_DEI_SYS52_MAX , FEAT_DEI_SYS52_CHECK},  
    { FEAT_DEI_SYS53_MIN , FEAT_DEI_SYS53_MAX , FEAT_DEI_SYS53_CHECK}, 
    { FEAT_DEI_SYS54_MIN , FEAT_DEI_SYS54_MAX , FEAT_DEI_SYS54_CHECK}, 
    { FEAT_DEI_SYS55_MIN , FEAT_DEI_SYS55_MAX , FEAT_DEI_SYS55_CHECK}, 
    { FEAT_DEI_SYS56_MIN , FEAT_DEI_SYS56_MAX , FEAT_DEI_SYS56_CHECK}, 
	{ FEAT_DEI_SYS57_MIN , FEAT_DEI_SYS57_MAX , FEAT_DEI_SYS57_CHECK}, 
    { FEAT_DEI_SYS58_MIN , FEAT_DEI_SYS58_MAX , FEAT_DEI_SYS58_CHECK},
    
    /*SENSE CONFIG*/    
    { FEAT_DEI_SYS59_MIN , FEAT_DEI_SYS59_MAX , FEAT_DEI_SYS59_CHECK},
    { FEAT_DEI_SYS60_MIN , FEAT_DEI_SYS60_MAX , FEAT_DEI_SYS60_CHECK},
    { FEAT_DEI_SYS61_MIN , FEAT_DEI_SYS61_MAX , FEAT_DEI_SYS61_CHECK},
    { FEAT_DEI_SYS62_MIN , FEAT_DEI_SYS62_MAX , FEAT_DEI_SYS62_CHECK},
    { FEAT_DEI_SYS63_MIN , FEAT_DEI_SYS63_MAX , FEAT_DEI_SYS63_CHECK},
    { FEAT_DEI_SYS64_MIN , FEAT_DEI_SYS64_MAX , FEAT_DEI_SYS64_CHECK},
    { FEAT_DEI_SYS65_MIN , FEAT_DEI_SYS65_MAX , FEAT_DEI_SYS65_CHECK},
    { FEAT_DEI_SYS66_MIN , FEAT_DEI_SYS66_MAX , FEAT_DEI_SYS66_CHECK},
    { FEAT_DEI_SYS67_MIN , FEAT_DEI_SYS67_MAX , FEAT_DEI_SYS67_CHECK},
    { FEAT_DEI_SYS68_MIN , FEAT_DEI_SYS68_MAX , FEAT_DEI_SYS68_CHECK},
    { FEAT_DEI_SYS69_MIN , FEAT_DEI_SYS69_MAX , FEAT_DEI_SYS69_CHECK},	

	/*SPECIAL FEATURES*/	
    { FEAT_DEI_SYS70_MIN , FEAT_DEI_SYS70_MAX , FEAT_DEI_SYS70_CHECK},
	{ FEAT_DEI_SYS71_MIN , FEAT_DEI_SYS71_MAX , FEAT_DEI_SYS71_CHECK},
    { FEAT_DEI_SYS72_MIN , FEAT_DEI_SYS72_MAX , FEAT_DEI_SYS72_CHECK},
    { FEAT_DEI_SYS73_MIN , FEAT_DEI_SYS73_MAX , FEAT_DEI_SYS73_CHECK},
    { FEAT_DEI_SYS74_MIN , FEAT_DEI_SYS74_MAX , FEAT_DEI_SYS74_CHECK},
    { FEAT_DEI_SYS75_MIN , FEAT_DEI_SYS75_MAX , FEAT_DEI_SYS75_CHECK},
    { FEAT_DEI_SYS76_MIN , FEAT_DEI_SYS76_MAX , FEAT_DEI_SYS76_CHECK},
    { FEAT_DEI_SYS77_MIN , FEAT_DEI_SYS77_MAX , FEAT_DEI_SYS77_CHECK},
    { FEAT_DEI_SYS78_MIN , FEAT_DEI_SYS78_MAX , FEAT_DEI_SYS78_CHECK},

	/*DEFAULT SETTINGS FOR TACHO and VIRTUAL TACHO*/
	{0 , 0 , FALSE }, 
    {3 , 0 , FALSE }
};

/*-------------------------------------------------------------------------------------*/


UInt8              dei_feature_list       [FEAT_DEI_SIZE];


/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

