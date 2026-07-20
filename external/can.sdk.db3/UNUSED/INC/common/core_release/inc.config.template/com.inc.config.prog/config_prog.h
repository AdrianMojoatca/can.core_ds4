/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: config_prog.h 33351 2015-08-25 11:20:35Z martin.bouchard $
/*==========================================================================*/

//             <<< Use Configuration Wizard in Context Menu >>>             //

#ifndef __CONFIG_PROG_H__
#define __CONFIG_PROG_H__
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
//          FEATURE CONFIGURATION WIZARD                                    //
//--------------------------------------------------------------------------//


// <h>  Option Available for TRUNK / AUX Features
// <q0> Trunk
#define OPTION_TRUNK_ENABLE             0  // 

// <q0> Right Sliding Door
#define OPTION_RIGHT_SLIDE_ENABLE       0  // 

// <q0> Left Sliding Door
#define OPTION_LEFT_SLIDE_ENABLE        0  // 

// <q0> Tail Glass Opening
#define OPTION_TAIL_GLASS_ENABLE        0  //  

// <q0> Gas Cap Opening
#define OPTION_GAS_CAP_ENABLE           0  // 

// <q0> Head Lights
#define OPTION_HEAD_LIGHTS_ENABLE       0  // 

// <q0> Sunroof Closure
#define OPTION_SUNROOF_CLOSURE_ENABLE   0  // 

// <q0> Windows Closure
#define OPTION_WINDOWS_CLOSURE_ENABLE   0  // 

// <q0> Windows Vent
#define OPTION_WINDOWS_VENT_ENABLE      0  // 

// </h>

// <h>  Heated Seats Level Configuration 
//    <q0> High
//    <q1> Med
//    <q2> Low
#define HEATED_SEATS_LEVEL_HIGH_ENABLE     0  //
#define HEATED_SEATS_LEVEL_MED_ENABLE      0  // 
#define HEATED_SEATS_LEVEL_LOW_ENABLE      0  //  
// </h>

//--------------------------------------------------------------------------//
// <h>  Feature Configuration

// <e0> Unlock Driver Priority
//    <o1> Option
//         <1=> *Driver priority
//         <2=>  All
// </e>   
#define FEAT_UNLOCK_DRIVER_ENABLE       1
#define FEAT_UNLOCK_DRIVER_VALUE        1

// <e0> Relock After Remote Starter Stops 
//    <o1> Option
//         <1=> *Disabled             
//         <2=>  Relock
// </e>     
#define FEAT_RELOCK_ENABLE              0  
#define FEAT_RELOCK_VALUE               1  

// <e0> Auto-Lock
//    <o1> Option
//         <1=> *Disabled
//         <2=>  With speed
// </e> 
#define FEAT_AUTO_LOCK_ENABLE           0   
#define FEAT_AUTO_LOCK_VALUE            1 

// <e0> Auto-Unlock
//    <o1> Option
//         <1=> *Disabled
//         <2=>  With key out
// </e> 
#define FEAT_AUTO_UNLOCK_ENABLE         0   
#define FEAT_AUTO_UNLOCK_VALUE          1  
 
// <e0> Trunk
//    <o1> Default   
//         <0=> *Trunk
//         <1=> Right Sliding Door 
//         <2=> Left Sliding Door 
//         <3=> Tail Glass Opening 
//         <4=> Gas Cap Opening
//         <5=> Head Lights 
//         <6=> Sunroof Closure 
//         <7=> Windows Closure
//         <8=> Windows Vent 
// </e>
#define FEAT_TRUNK_ENABLE               0   
#define FEAT_TRUNK_DEFAULT              0   

// <e0> Aux1
//    <o1> Default
//         <0=> Trunk 
//         <1=> *Right Sliding Door 
//         <2=> Left Sliding Door 
//         <3=> Tail Glass Opening 
//         <4=> Gas Cap Opening
//         <5=> Head Lights 
//         <6=> Sunroof Closure 
//         <7=> Windows Closure
//         <8=> Windows Vent 
// </e>
#define FEAT_AUX1_ENABLE                0   
#define FEAT_AUX1_DEFAULT               3   

// <e0> Aux2
//    <o1> Default
//         <0=>  Trunk 
//         <1=>  Right Sliding Door 
//         <2=> *Left Sliding Door 
//         <3=>  Tail Glass Opening 
//         <4=>  Gas Cap Opening
//         <5=>  Head Lights 
//         <6=>  Sunroof Closure 
//         <7=>  Windows Closure
//         <8=>  Windows Vent 
// </e>
#define FEAT_AUX2_ENABLE                0   
#define FEAT_AUX2_DEFAULT               2   

// <e0> Aux3
//    <o1> Default
//         <0=>  Trunk 
//         <1=>  Right Sliding Door 
//         <2=>  Left Sliding Door 
//         <3=>  Tail Glass Opening 
//         <4=> *Gas Cap Opening
//         <5=>  Head Lights 
//         <6=>  Sunroof Closure 
//         <7=>  Windows Closure
//         <8=>  Windows Vent 
// </e>
#define FEAT_AUX3_ENABLE                0   
#define FEAT_AUX3_DEFAULT               0   

// <e0> Aux4
//    <o1> Default 
//         <0=>  Trunk 
//         <1=>  Right Sliding Door 
//         <2=>  Left Sliding Door 
//         <3=>  Tail Glass Opening 
//         <4=>  Gas Cap Opening
//         <5=>  Head Lights 
//         <6=>  Sunroof Closure 
//         <7=> *Windows Closure
//         <8=>  Windows Vent 
// </e>
#define FEAT_AUX4_ENABLE                    0  
#define FEAT_AUX4_DEFAULT                   7  

// <e0> Rf Output
//    <o1> Option
//         <1=> *Disabled
//         <2=>  Rftd
//         <3=>  SmartStart
// </e>     
#define FEAT_RF_OUTPUT_ENABLE               1  
#define FEAT_RF_OUTPUT_VALUE                1  
                                                                               
// <e0> Horn Confirmation               
//    <o1> Option                       
//         <1=> *Disabled               
//         <2=>  20 ms                  
//         <3=>  30 ms                  
//         <4=>  40 ms                  
//         <5=>  50 ms                  
// </e>                                 
#define FEAT_HORN_CONFIRMATION_ENABLE       0  
#define FEAT_HORN_CONFIRMATION_VALUE        1
                                        
// <e0> Comfort Closure with Lock       
//    <o1> Option                       
//         <1=> *Disabled                                
//         <2=>  Windows only       
//         <3=>  Windows and sunroof    
// </e>                                 
#define FEAT_COMFORT_CLOSURE_ENABLE         0  
#define FEAT_COMFORT_CLOSURE_VALUE          1  
                                        
                                        
// <e0> OEM Security Equipped           
//    <o1> Option                       
//         <1=> *Without OEM alarm                       
//         <2=>  With    OEM alarm                       
// </e>                                 
#define FEAT_OEM_SECURITY_ENABLE            0  
#define FEAT_OEM_SECURITY_VALUE             1  
                                        
// <e0> Key in ignition arm defeat      
//    <o1> Option                       
//         <1=>  Disabled                       
//         <2=> *Enabled                    
// </e>                                 
#define FEAT_KEY_IGN_ARM_DEFEAT_ENABLE      0  
#define FEAT_KEY_IGN_ARM_DEFEAT_VALUE       2  
                                        
// <e0> Light Control                   
//    <o1> Option                       
//         <1=> *Parking lights                      
//         <2=>  Turn signals                        
// </e>                                 
#define FEAT_LIGHT_CONTROL_ENABLE           0  
#define FEAT_LIGHT_CONTROL_VALUE            1  
                                        
// <e0> Exterior Light Supervision
//    <o1> Option
//         <1=> *Disabled
//         <2=>  Enabled
// </e>     
#define FEAT_EXT_LIGHT_SUPERVISION_ENABLE   0  
#define FEAT_EXT_LIGHT_SUPERVISION_VALUE    1  

// <e0> Heated Seats
//    <o1> Option
//         <1=> *Disabled
//         <2=>  Enabled
// </e>     
#define FEAT_HEATED_SEATS_ENABLE            0 
#define FEAT_HEATED_SEATS_VALUE             1  

// <e0> Defroster
//    <o1> Option
//         <1=> *Disabled
//         <2=>  Enabled 
// </e>     
#define FEAT_DEFROSTER_ENABLE               0  
#define FEAT_DEFROSTER_VALUE                1  

// <e0> Heated Seats/Defroster Temperature Control
//    <o1> Option
//         <1=> *32°F (0°C) 
//         <2=>  23°F (-5°C)
//         <3=>  41°F (+5°C)
//         <4=>  Don't care, always on
// </e>     
#define FEAT_SEATS_TEMP_CONTROL_ENABLE      0  
#define FEAT_SEATS_TEMP_CONTROL_VALUE       1 

// <e0> Heated Seats Temperature Level
//    <o1> Option
//         <1=> *High                    
//         <2=>  Med
//         <3=>  Low                   
// </e>     
#define FEAT_SEATS_TEMP_LEVEL_ENABLE      	0  
#define FEAT_SEATS_TEMP_LEVEL_VALUE       	1 

// <e0> Automatic Controlled Door Lock
//    <o1> Option
//         <1=> *Disabled
//         <2=>  Ignition
//         <3=>  Brake
//         <4=>  Speed
// <q2> Ignition enable
// <q3> Brake    enable
// <q4> Speed    enable
// </e>   
#define FEAT_CTL_DOORLOCK_ENABLE   		1
#define FEAT_CTL_DOORLOCK_VALUE    		1
#define CTL_DOORLOCK_IGNITION_ENABLE    1  
#define CTL_DOORLOCK_BRAKE_ENABLE       1  
#define CTL_DOORLOCK_SPEED_ENABLE       1   

// <e0> Smart OEM Alarm Control
//    <o1> Option
//         <1=>  Disabled                   
//         <2=>  Safelock              
//         <3=> *Enabled      
// <q2> Smart enable
     
// </e>    
#define FEAT_SECURE_LOCK_ENABLE      1 
#define FEAT_SECURE_LOCK_VALUE       3 
#define SECURE_LOCK_SMART_ENABLE     1  //  

// <e0> Feature Custom1
//    <o1> Option
//         <1=> *Option1
//         <2=>  Option2
//         <3=>  Option3 
//         <4=>  Option4 
//         <5=>  Option5 
// <q2> Option2 enable
// <q3> Option3 enable
// <q4> Option4 enable
// <q5> Option5 enable
    
// </e>     
#define FEAT_CUSTOM1_ENABLE         0 
#define FEAT_CUSTOM1_DEFAULT_VALUE  1  //  
#define CUSTOM1_OPT2_ENABLE	 	  	1
#define CUSTOM1_OPT3_ENABLE 	  	1
#define CUSTOM1_OPT4_ENABLE 	  	1
#define CUSTOM1_OPT5_ENABLE	 		1

// <e0> Feature Custom2
//    <o1> Option
//         <1=> *Option1
//         <2=>  Option2
//         <3=>  Option3 
//         <4=>  Option4 
//         <5=>  Option5 
// <q2> Option2 enable
// <q3> Option3 enable
// <q4> Option4 enable
// <q5> Option5 enable
   
// </e>     
#define FEAT_CUSTOM2_ENABLE         0 
#define FEAT_CUSTOM2_DEFAULT_VALUE  1  //  
#define CUSTOM2_OPT2_ENABLE	 	  	1
#define CUSTOM2_OPT3_ENABLE 	  	1
#define CUSTOM2_OPT4_ENABLE 	  	1
#define CUSTOM2_OPT5_ENABLE	 		1

// <e0> Feature Custom3
//    <o1> Option
//         <1=> *Option1
//         <2=>  Option2
//         <3=>  Option3 
//         <4=>  Option4 
//         <5=>  Option5 
// <q2> Option2 enable
// <q3> Option3 enable
// <q4> Option4 enable
// <q5> Option5 enable
   
// </e>     
#define FEAT_CUSTOM3_ENABLE         0 
#define FEAT_CUSTOM3_DEFAULT_VALUE  1  //  
#define CUSTOM3_OPT2_ENABLE	 	  	1
#define CUSTOM3_OPT3_ENABLE 	  	1
#define CUSTOM3_OPT4_ENABLE 	  	1
#define CUSTOM3_OPT5_ENABLE	 		1

// <e0> Feature Custom4
//    <o1> Option
//         <1=> *Option1
//         <2=>  Option2
//         <3=>  Option3 
//         <4=>  Option4 
//         <5=>  Option5 
// <q2> Option2 enable
// <q3> Option3 enable
// <q4> Option4 enable
// <q5> Option5 enable
   
// </e>     
#define FEAT_CUSTOM4_ENABLE         0 
#define FEAT_CUSTOM4_DEFAULT_VALUE  1  //  
#define CUSTOM4_OPT2_ENABLE	 	  	1
#define CUSTOM4_OPT3_ENABLE 	  	1
#define CUSTOM4_OPT4_ENABLE 	  	1
#define CUSTOM4_OPT5_ENABLE	 		1

// </h>

//--------------------------------------------------------------------------//


//--------------------------------------------------------------------------//

#endif

