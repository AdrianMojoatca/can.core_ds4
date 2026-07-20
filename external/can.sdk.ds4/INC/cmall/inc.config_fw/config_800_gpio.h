#define DEI_FEATURE_VERSION  24
#define AS_FEATURE_VERSION    5
//             <<< Use Configuration Wizard in Context Menu >>>             //

//--------------------------------------------------------------------------//
// <h>  Programmable Inputs/Outputs
//--------------------------------------------------------------------------//

//<e0>Output 1 option is visible on web
//<o1>Output1 (-) H2[10]
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
//         <29=> Not Configured
//</e>
#define FEAT_DEI_SYS31_ENABLE   		0x00					
#define FEAT_DEI_SYS31 					0x1D

//<e0>Output 2 option is visible on web
//<o1>Output2  (-) H3[18]
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
//         <29=> Not Configured
//</e>
#define FEAT_DEI_SYS32_ENABLE   		0x01					
#define FEAT_DEI_SYS32 					0x0E

//<e0>Output 3 option is visible on web
//<o1>Output3 (-)H3[15]
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
//         <29=> Not Configured
//</e>
#define FEAT_DEI_SYS33_ENABLE   		0x00					
#define FEAT_DEI_SYS33 					0x1D

//<e0>Output 4 option is visible on web
//<o1>Output4 (-) H3[9]
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
//         <29=> Not Configured
//</e>
#define FEAT_DEI_SYS34_ENABLE   		0x01					
#define FEAT_DEI_SYS34 					0x09

//<e0>Output 5 option is visible on web
//<o1>Output5 (-) H3[7]
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
//         <29=> Not Configured
//</e>
#define FEAT_DEI_SYS35_ENABLE   		0x01					
#define FEAT_DEI_SYS35 					0x06

//<e0>Output 6 option is visible on web
//<o1>Output6  (+) H2[14]
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
//         <29=> *Not Configured
//</e>
#define FEAT_DEI_SYS36_ENABLE   		0x01					
#define FEAT_DEI_SYS36 					0x0D

//<e0>Output 7 option is visible on web
//<o1>Output7  (+/-) H2[12](+), H2[11](-)
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
//         <29=> Not Configured
//</e>
#define FEAT_DEI_SYS37_ENABLE   		0x00
#define FEAT_DEI_SYS37 					0x16

//<e0>Output 8 option is visible on web
//<o1>Output8  (+) H4[3]
//         <20=> Accessory
//         <21=> *Ignition
//         <22=> Starter
//         <29=> Not Configured
//</e>
#define FEAT_DEI_SYS38_ENABLE   		0x01					
#define FEAT_DEI_SYS38 					0x15

//<e0>Output 9 option is visible on web
//<o1>Output9  (+) H4[5]
//         <20=> *Accessory
//</e>
#define FEAT_DEI_SYS39_ENABLE   		0x00					
#define FEAT_DEI_SYS39 					0x14

//<e0>Output 10 option is visible on web
//<o1>Output10  (+) H4[1] 
//         <21=> *Ignition
//</e>
#define FEAT_DEI_SYS40_ENABLE   		0x00					
#define FEAT_DEI_SYS40 					0x15

//<e0>Output 11 option is visible on web
//<o1>Output11  (+) H4[8]
//         <22=> *Starter
//</e>
#define FEAT_DEI_SYS41_ENABLE   		0x00					
#define FEAT_DEI_SYS41 					0x16

//<e0>Output 12 option is visible on web
//<o1>Output12  (+) H4[7]
//         <28=> *Anti-Grind 
//</e>
#define FEAT_DEI_SYS42_ENABLE   		0x00					
#define FEAT_DEI_SYS42 					0x1C

//<e0>Output 13 option is visible on web
//<o1>Output13  (-) H2[13]
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
//         <29=> Not Configured
//</e>
#define FEAT_DEI_SYS43_ENABLE   		0x00					
#define FEAT_DEI_SYS43 					0x1D

//<e0>Output 14 option is visible on web
//<o1>Output14  (-) H3[8]
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
//         <26=> *Siren/Horn
//         <27=> Ground when alarm triggered
//         <29=> Not Configured
//</e>
#define FEAT_DEI_SYS44_ENABLE   	0x01					
#define FEAT_DEI_SYS44				0x1A


//<e0>Relay 1 option is visible on web
//<o1>Relay1  COM:H1[6] , NC:H1[11] , NO:H1[12]
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
//         <26=> *Siren/Horn
//         <27=> Ground when alarm triggered
//         <29=> Not Configured
//</e>
#define FEAT_DEI_SYS45_ENABLE   		0x00					
#define FEAT_DEI_SYS45 					0x1D


//<e0>Relay 2 option is visible on web
//<o1>Relay2  COM:H1[4] , NC:H1[5] , NO:H1[3]
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
//         <29=> *Not Configured
//</e>
#define FEAT_DEI_SYS46_ENABLE   		0x00					
#define FEAT_DEI_SYS46 					0x1D

//<e0>Relay 3 option is visible on web
//<o1>Relay3  COM:H1[1], H1[7] , NO:H1[2],H1[8]
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
//         <29=> Not Configured
//</e>
#define FEAT_DEI_SYS47_ENABLE   		0x01					
#define FEAT_DEI_SYS47 					0x17



//<e0>Input Negative 1 option is visible on web
//<o1>Input Negative 1 Start Trigger (H3[14]) 
//         <9=>  *Start trigger 
//</e>
#define FEAT_DEI_SYS48_ENABLE   		0x01					
#define FEAT_DEI_SYS48 					0x09

//<e0>Input Negative 2 option is visible on web
//<o1>Input Negative 2(H3[1]) 
//         <1=>  Handbrake
//         <2=>  Safety Switch
//         <3=>  Hood
//         <4=>  Trunk
//         <5=>  Door
//         <9=>  Start trigger 
//         <10=> Custom Neg1
//         <11=> Custom Neg2 *
//         <12=> Custom Neg3
//</e>
#define FEAT_DEI_SYS49_ENABLE   		0x01					
#define FEAT_DEI_SYS49 					0x0B


//<e0>Input Negative 3 option is visible on web
//<o1>Input Negative 3(H3[12]) 
//         <1=>  Handbrake
//         <2=>  Safety Switch
//         <3=>  Hood
//         <4=>  Trunk
//         <5=>  Door
//         <9=>  Start trigger 
//         <10=> Custom Neg1
//         <11=> Custom Neg2 
//         <12=> Custom Neg3 *
//</e>
#define FEAT_DEI_SYS50_ENABLE   		0x01					
#define FEAT_DEI_SYS50 					0x0C


//<e0>Input Negative 4 option is visible on web
//<o1>Input Negative 4(H3[2]) 
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
#define FEAT_DEI_SYS51_ENABLE   		0x01					
#define FEAT_DEI_SYS51 					0x01


//<e0>Input Negative 5 option is visible on web
//<o1>Input Negative 5(H3[3]) 
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
#define FEAT_DEI_SYS52_ENABLE   		0x01					
#define FEAT_DEI_SYS52 					0x03

//<e0>Input Negative 6 option is visible on web
//<o1>Input Negative 6(H3[4]) 
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
#define FEAT_DEI_SYS53_ENABLE   		0x01					
#define FEAT_DEI_SYS53 					0x02

//<e0>Input Negative 7 option is visible on web
//<o1>Input Negative 7(H3[13]) 
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
#define FEAT_DEI_SYS54_ENABLE   		0x01					
#define FEAT_DEI_SYS54 					0x04


//<e0>Input Positive 1 option is visible on web
//<o1>Input Positive 1(H3[5]) 
//         <5=>  Door
//         <6=>  Brake
//         <7=>  Glowplug *
//         <8=>  Ignition
//         <13=> Custom Pos1
//         <14=> Custom Pos2
//</e>
#define FEAT_DEI_SYS55_ENABLE   		0x01					
#define FEAT_DEI_SYS55 					0x07

//<e0>Input Positive 2 option is visible on web
//<o1>Input Positive 2 (H3[16]) 
//         <5=>  Door
//         <6=>  Brake *
//         <7=>  Glowplug
//         <8=>  Ignition
//         <13=> Custom Pos1
//         <14=> Custom Pos2
//</e>
#define FEAT_DEI_SYS56_ENABLE   		0x01					
#define FEAT_DEI_SYS56 					0x06

//<e0>Input Positive 3 option is visible on web
//<o1>Input Positive 3 (ribbon cable)
//         <5=>  Door
//         <6=>  Brake
//         <7=>  Glowplug
//         <8=>  Ignition *
//         <13=> Custom Pos1
//         <14=> Custom Pos2
//</e>
#define FEAT_DEI_SYS57_ENABLE   		0x01					
#define FEAT_DEI_SYS57 					0x08


//<e0>Input Negative/Positive 1 option is visible on web
//<o1>Input Negative/Positive 1 (H3[10](-) , H3[11](+)) 
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
#define FEAT_DEI_SYS58_ENABLE   		0x01					
#define FEAT_DEI_SYS58 					0x05


// </h>
