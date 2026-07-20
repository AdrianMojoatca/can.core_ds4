#define NGMM_FEATURE_VERSION  23     //new feature list starts version 10
//             <<< Use Configuration Wizard in Context Menu >>>             //

//--------------------------------------------------------------------------//
// <h>  Programmable Inputs/Outputs
//--------------------------------------------------------------------------//

//<e0>Output 1 option is visible on web
//<o1>Output1 (-)	22 pin [1] Light Green/Black
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
//         <36=> Power Tailgate Interrupt Relay
//         <37=> Trunk Status Output
//         <38=> Brake Status Output
//         <39=> Proximity Status Output
//</e>
#define FEAT_DEI_SYS40_ENABLE           0x01
#define FEAT_DEI_SYS40                  0x00

//<e0>Output 2 option is visible on web
//<o1>Output2 (-)	22 pin [2] Blue/White
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
//         <36=> Power Tailgate Interrupt Relay
//         <37=> Trunk Status Output
//         <38=> Brake Status Output
//         <39=> Proximity Status Output
//</e>
#define FEAT_DEI_SYS41_ENABLE           0x00
#define FEAT_DEI_SYS41                  0x00

//<e0>Output 3 option is visible on web
//<o1>Output3 (-)	22 pin [3] Green/Black
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
//         <36=> Power Tailgate Interrupt Relay
//         <37=> Trunk Status Output
//         <38=> Brake Status Output
//         <39=> Proximity Status Output
//</e>
#define FEAT_DEI_SYS42_ENABLE           0x00
#define FEAT_DEI_SYS42                  0x04

//<e0>Output 4 option is visible on web
//<o1>Output4 (-)	22 pin [4] Red/White
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
//         <36=> Power Tailgate Interrupt Relay
//         <37=> Trunk Status Output
//         <38=> Brake Status Output
//         <39=> Proximity Status Output
//</e>
#define FEAT_DEI_SYS43_ENABLE           0x00
#define FEAT_DEI_SYS43                  0x00

//<e0>Output 5 option is visible on web
//<o1>Output5 (-)	22 pin [9] Blue/Black
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
//         <36=> Power Tailgate Interrupt Relay
//         <37=> Trunk Status Output
//         <38=> Brake Status Output
//         <39=> Proximity Status Output
//</e>
#define FEAT_DEI_SYS44_ENABLE           0x00
#define FEAT_DEI_SYS44                  0x06

//<e0>Output 6 option is visible on web
//<o1>Output6 (+) 22 pin [8] Purple/Black
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
//         <36=> Power Tailgate Interrupt Relay
//         <37=> Trunk Status Output
//         <38=> Brake Status Output
//         <39=> Proximity Status Output
//</e>
#define FEAT_DEI_SYS45_ENABLE           0x00
#define FEAT_DEI_SYS45                  0x1B

//<e0>Output 7 option is visible on web
//<o1>Output7 (+/-)	22 pin [7] White and (-) Pin [6] Brown/White
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
//         <36=> Power Tailgate Interrupt Relay
//         <37=> Trunk Status Output
//         <38=> Brake Status Output
//         <39=> Proximity Status Output
//</e>
#define FEAT_DEI_SYS46_ENABLE           0x01
#define FEAT_DEI_SYS46                  0x00

//<e0>Output 8 option is visible on web
//<o1>Output8 (+)	10 Pin Main Ign Harness [4] Pink/White
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
//         <36=> Power Tailgate Interrupt Relay
//         <37=> Trunk Status Output
//         <38=> Brake Status Output
//         <39=> Proximity Status Output
//</e>
#define FEAT_DEI_SYS47_ENABLE           0x01
#define FEAT_DEI_SYS47                  0x00

//<e0>Output 9 option is visible on web
//<o1>Output9 (+)	10 Pin Main Ign Harness [8] Orange
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
//         <36=> Power Tailgate Interrupt Relay
//         <37=> Trunk Status Output
//         <38=> Brake Status Output
//         <39=> Proximity Status Output
//</e>
#define FEAT_DEI_SYS48_ENABLE           0x00
#define FEAT_DEI_SYS48                  0x10

//<e0>Output 10 option is visible on web
//<o1>Output10  (+)	10 Pin Main Ign Harness [10] Pink
//         <17=> *Ignition
//</e>
#define FEAT_DEI_SYS49_ENABLE           0x00
#define FEAT_DEI_SYS49                  0x11

//<e0>Output 11 option is visible on web
//<o1>Output11  (+)	10 Pin Main Ign Harness [7] Violet
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
//         <36=> Power Tailgate Interrupt Relay
//         <37=> Trunk Status Output
//         <38=> Brake Status Output
//         <39=> Proximity Status Output
//</e>
#define FEAT_DEI_SYS50_ENABLE           0x00
#define FEAT_DEI_SYS50                  0x12

//<e0>Output 12 option is visible on web
//<o1>Output12  (+)	10 Pin Main Ign Harness [6] Green
//         <31=> *AG Starter-kill
//</e>
#define FEAT_DEI_SYS51_ENABLE           0x01
#define FEAT_DEI_SYS51                  0x1E

//<e0>Output 13 option is visible on web
//<o1>Output13  (-)	22 pin [10] Light Blue/Black
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
//         <36=> Power Tailgate Interrupt Relay
//         <37=> Trunk Status Output
//         <38=> Brake Status Output
//         <39=> Proximity Status Output
//</e>
#define FEAT_DEI_SYS52_ENABLE           0x00
#define FEAT_DEI_SYS52                  0x00

//<e0>Output 14 option is visible on web
//<o1>Output14  (-)	22 pin [5] Brown/Black
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
//         <36=> Power Tailgate Interrupt Relay
//         <37=> Trunk Status Output
//         <38=> Brake Status Output
//         <39=> Proximity Status Output
//</e>
#define FEAT_DEI_SYS53_ENABLE           0x01
#define FEAT_DEI_SYS53                  0x00


//<e0>Relay 1 option is visible on web
//<o1>Relay1	12 Pin Relay Harness pin NC[1], NO[2], COM[3]
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
//         <36=> Power Tailgate Interrupt Relay
//         <37=> Trunk Status Output
//         <38=> Brake Status Output
//         <39=> Proximity Status Output
//</e>
#define FEAT_DEI_SYS54_ENABLE           0x00
#define FEAT_DEI_SYS54                  0x00


//<e0>Relay 2 option is visible on web
//<o1>Relay2	12 Pin Relay Harness pin NO[7], COM[8], NC[9]
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
//         <36=> Power Tailgate Interrupt Relay
//         <37=> Trunk Status Output
//         <38=> Brake Status Output
//         <39=> Proximity Status Output
//</e>
#define FEAT_DEI_SYS55_ENABLE           0x00
#define FEAT_DEI_SYS55                  0x00

//<e0>Relay 3 option is visible on web
//<o1>Relay3	12 Pin Relay Harness pin NC[4], COM[5-11], NO = FUSE
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
//         <36=> Power Tailgate Interrupt Relay
//         <37=> Trunk Status Output
//         <38=> Brake Status Output
//         <39=> Proximity Status Output
//</e>
#define FEAT_DEI_SYS56_ENABLE           0x01
#define FEAT_DEI_SYS56                  0x00


#define FEAT_DEI_INP_OPT_MAX 13 // Tamper is last

//<e0>Input Negative 1 option is visible on web
//<o1>Input 1 (-) Start Trigger 22 pin [15] White/Blue
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
#define FEAT_DEI_SYS57_ENABLE           0x01
#define FEAT_DEI_SYS57                  0x0D

//Input Negative 2 is fixed

//<e0>Input Negative 3 option is visible on web
//<o1>Input 3 (-) 22 pin [16] Red/Blue
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
#define FEAT_DEI_SYS58_ENABLE           0x01
#define FEAT_DEI_SYS58                  0x0D


//<e0>Input Negative 4 option is visible on web
//<o1>Input 4 (-) 22 pin [17] Black/White
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
#define FEAT_DEI_SYS59_ENABLE           0x01
#define FEAT_DEI_SYS59                  0x0D


//<e0>Input Negative 5 option is visible on web
//<o1>Input 5 (-) 22 pin [21] Gray
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
#define FEAT_DEI_SYS60_ENABLE           0x01
#define FEAT_DEI_SYS60                  0x0D

//<e0>Input Negative 7 option is visible on web
//<o1>Input 7 (-) 22 pin [19] Blue
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
#define FEAT_DEI_SYS61_ENABLE           0x01
#define FEAT_DEI_SYS61                  0x0D


//<e0>Input Positive 1 option is visible on web
//<o1>Input 1 (+) 22 pin [18] Orange/Black
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
#define FEAT_DEI_SYS62_ENABLE           0x01
#define FEAT_DEI_SYS62                  0x0D

//<e0>Input Positive 2 option is visible on web
//<o1>Input 2 (+) 22 pin [14] Brown
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
#define FEAT_DEI_SYS63_ENABLE           0x01
#define FEAT_DEI_SYS63                  0x0A

//<e0>Input Positive 3 option is visible on web
//<o1>Input 3 (+) Pink small input wire
//         <10=>  ignition
//</e>
#define FEAT_DEI_SYS64_ENABLE           0x01
#define FEAT_DEI_SYS64                  0x0D


//<e0>Input Positive Aux1 option is visible on web
//<o1>Input (+) 22 pin [13] Violet
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
#define FEAT_DEI_SYS65_ENABLE           0x01
#define FEAT_DEI_SYS65                  0x03

// </h>
