/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_feature.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __NGMM_FEATURE_H__
#define __NGMM_FEATURE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "device.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/


// WARNING! we need to put these here, and not in config_ngmm_feature.h or config_933_gpio.h
#define FEAT_DEI_OUT_OPT_MAX 39  // last value for output
#define FEAT_DEI_INP_OPT_MAX 17   // last value for input

/*--------------------------------------------------------------------------*/

typedef enum 
{
	// 3 Lock Start Features
    DEI_FEAT_OEM_RS_ACTIVATION                      ,
    /*SS FEATURE*/
    DEI_FEAT_SYSTEM_ARMING_MODE                     ,
    DEI_FEAT_PANIC_MODE                             ,
    DEI_FEAT_CONFIRMATION_CHIRPS                    ,
    DEI_FEAT_HORN_FUNCTION                          ,
    DEI_FEAT_VALET_SWITCH_PULSE_COUNT               ,
    //DEI_FEAT_SENSOR_TYPE                            ,
    DEI_FEAT_SIREN_DURATION                         ,
    //DEI_FEAT_ONE_TIME_BYPASS                        ,
    DEI_FEAT_NUISANCE_PREVENTION                    ,
    DEI_FEAT_DOOR_TRIGGER_ERROR_CHIRP               ,

    DEI_FEAT_STARTER_KILL                           ,
    DEI_FEAT_HORN_OUTPUT_TIMING                     ,
    DEI_FEAT_UNLOCK_DISARM_WITH_TRUNK_RELEASE       ,
    //DEI_FEAT_OEM_DISARM_WITH_AUX                    ,		// need to be removed
	DEI_FEAT_AUTO_REARM															,

    /*RS FEATURE*/
    //DEI_FEAT_TRANSMISSION_MODE                      ,
    DEI_FEAT_ENGINE_CHECKING_MODE                   ,
    DEI_FEAT_CRANKING_TIME                          ,
    DEI_FEAT_ACTIVATION_PULSE_COUNT                 ,
    DEI_FEAT_DIESEL_START_DELAY                     ,
    //DEI_FEAT_ACCESSORY_DURING_DIESEL_START_DELAY    ,
    //DEI_FEAT_REAR_DEFOGGER_OUTPUT_TYPE              ,
    DEI_FEAT_PARKING_LIGHT_OUTPUT                   ,
    DEI_FEAT_TURBO_MODE_RUNTIME                     ,
    DEI_FEAT_REMOTE_START_RUNTIME                   ,
    DEI_FEAT_TIMER_MODE_RUNTIME                     ,
    DEI_FEAT_TIMER_MODE_START                       ,
    DEI_FEAT_TIMER_MODE_INTERVAL                    ,
    DEI_FEAT_SMART_START_HIGH_TEMP                  ,
    DEI_FEAT_SMART_START_LOW_TEMP                   ,
    DEI_FEAT_SMART_START_LOW_BATTERY                ,
    DEI_FEAT_VIRTUAL_TACH_FINE_TUNE                 ,
    DEI_FEAT_SECURE_LOCK                            ,

    DEI_FEAT_TACHOMETER_INPUT                       ,
    DEI_FEAT_PITSTOP_MODE_ENABLE                    ,
    DEI_FEAT_TAKEOVER                               ,
	  DEI_FEAT_MANUAL_TRANSMISSION_INIT				        ,
	  DEI_FEAT_MANUAL_TRANSMISSION_COMPLETION			    ,
		DEI_FEAT_DURATION_PULSE_AFTER_RS_SHUTDOWN      ,
		DEI_FEAT_ACT_DELAY_PULSE_AFTER_RS_SHUTDOWN     ,
    /*SYSTEM FEATURE*/
    //DEI_FEAT_IGN_CONTROLLED_LOCKS                   ,
    DEI_FEAT_DOORLOCK_PULSES                        ,
    DEI_FEAT_DOORLOCK_OUTPUT_DURATION               ,
    //DEI_FEAT_2ND_UNLOCK                             ,
    DEI_FEAT_COMFORT_CLOSURE                        ,
    DEI_FEAT_HOOD_TRIGGER_TYPE                      ,
    DEI_FEAT_IGN_CONTROLLED_DOMELIGHT               ,
    //DEI_FEAT_OEM_ALARM_DISARM_W_CH2                 ,
    DEI_FEAT_OEM_ALARM_DISARM_OUTPUT                ,
    //DEI_FEAT_OEM_ALARM_DISARM_PULSES                ,
    DEI_FEAT_AUX_1_ICON                             ,
    DEI_FEAT_AUX_1_OUTPUT_TYPE                      ,
    DEI_FEAT_AUX_1_LINKING                          ,
    DEI_FEAT_AUX_1_TIMED_OUTPUT_SECONDS             ,
    DEI_FEAT_AUX_2_ICON                             ,
    DEI_FEAT_AUX_2_OUTPUT_TYPE                      ,
    DEI_FEAT_AUX_2_LINKING                          ,
    DEI_FEAT_AUX_2_TIMED_OUTPUT_SECONDS             ,
    DEI_FEAT_AUX_3_ICON                             ,
    DEI_FEAT_AUX_3_OUTPUT_TYPE                      ,
    DEI_FEAT_AUX_3_LINKING                          ,
    DEI_FEAT_AUX_3_TIMED_OUTPUT_SECONDS             ,
    DEI_FEAT_AUX_4_OUTPUT_TYPE                      ,
    DEI_FEAT_AUX_4_LINKING                          ,
    DEI_FEAT_AUX_4_TIMED_OUTPUT_SECONDS             ,
    DEI_FEAT_CH2_OUTPUT_TYPE                        ,
    DEI_FEAT_CH2_ICON                               ,
    DEI_FEAT_RF_KEYBOARD_UNLOCK                     ,
//    DEI_FEAT_TRANSMITTER_PROGRAMMING                ,
//    DEI_FEAT_OEM_DISARM                             ,
  	

    DEI_FEAT_DOOR_SWITCH_TYPE                       ,
    DEI_FEAT_TRUNK_SWITCH_TYPE                      ,
    DEI_FEAT_CONTROL_DOOR_LOCK_OPTION               ,
    DEI_FEAT_CONTROL_DOOR_LOCK_FUNC                 ,
    DEI_FEAT_CONTROL_UNLOCK_ALL_DOOR                ,
    DEI_FEAT_DRIVER_DOOR_UNLOCK_PRIORITY            ,
    DEI_FEAT_PLIGHT_CONFIRMATION                    ,
    DEI_FEAT_ANTENNA_LED_FLASH_OPTION               ,
    DEI_FEAT_IGNITION_DISARM                        ,
    DEI_FEAT_FOB_VOLTAGE                            ,

    /*PROGRAMMABLE OUTPUTS*/
    DEI_FEAT_OUTPUT1                                ,
    DEI_FEAT_OUTPUT2                                ,
    DEI_FEAT_OUTPUT3                                ,
    DEI_FEAT_OUTPUT4                                ,
    DEI_FEAT_OUTPUT5                                ,
    DEI_FEAT_OUTPUT6                                ,
    DEI_FEAT_OUTPUT7                                ,
    DEI_FEAT_OUTPUT8                                ,
    DEI_FEAT_OUTPUT9                                ,
    DEI_FEAT_OUTPUT10                               ,
    DEI_FEAT_OUTPUT11                               ,
    DEI_FEAT_OUTPUT12                               ,
    DEI_FEAT_OUTPUT13                               ,
    DEI_FEAT_OUTPUT14                               ,
    DEI_FEAT_RELAY1                                 ,
    DEI_FEAT_RELAY2                                 ,
    DEI_FEAT_RELAY3                                 ,
    
    /*PROGRAMMABLE INPUTS*/   
    DEI_FEAT_IN_NEG1                                ,
    //DEI_FEAT_IN_NEG2                                ,
    DEI_FEAT_IN_NEG3                                ,
    DEI_FEAT_IN_NEG4                                ,
    DEI_FEAT_IN_NEG5                                ,
    //DEI_FEAT_IN_NEG6                                ,
    DEI_FEAT_IN_NEG7                                ,
    DEI_FEAT_IN_POS1                                ,
    DEI_FEAT_IN_POS2                                ,
    DEI_FEAT_IN_POS3                                ,
    DEI_FEAT_IN_NEG_AND_POS1                        ,
    
    
    /*SENSE CONFIG*/
    DEI_FEAT_SENSE_DOORS                            ,
    DEI_FEAT_SENSE_IGN                              ,
    DEI_FEAT_SENSE_HOOD                             ,
    DEI_FEAT_SENSE_TRUNK                            ,
    DEI_FEAT_SENSE_HANDBRAKE                        ,
    DEI_FEAT_SENSE_TACHO                            ,
    DEI_FEAT_SENSE_BRAKE                            ,
    //DEI_FEAT_SENSE_GLOWPLUG                         ,
    DEI_FEAT_SENSE_OEM_LOCK                         ,
    DEI_FEAT_SENSE_OEM_UNLOCK                       ,
    DEI_FEAT_SENSE_OEM_TRUNK                        ,

    DEI_FEAT_ENGINE_SENSING                         ,
    DEI_FEAT_ARM_OUTPUT                             ,
    DEI_FEAT_DISARM_OUTPUT                          ,
    DEI_FEAT_LOCK_OUTPUT                            ,
    DEI_FEAT_PLIGHT_OUTPUT                          ,
    DEI_FEAT_RAP_OUTPUT                             ,
    DEI_FEAT_TRUNK_OUTPUT                           ,
    DEI_FEAT_UNLOCK_OUTPUT                          ,

    /*FIRMWARE SPECIFIC CONFIGURATION*/             
    DEI_FEAT_CUSTOM_FEATURE_00                      ,
    DEI_FEAT_CUSTOM_FEATURE_01                      ,
    DEI_FEAT_CUSTOM_FEATURE_02                      ,
    DEI_FEAT_CUSTOM_FEATURE_03                      ,
    DEI_FEAT_CUSTOM_FEATURE_04                      ,
    DEI_FEAT_CUSTOM_FEATURE_05                      ,
    DEI_FEAT_CUSTOM_FEATURE_06                      ,
    DEI_FEAT_CUSTOM_FEATURE_07                      ,
    DEI_FEAT_TACH_VAL_LEARNED                       ,

    /*REMOTE STARTER DEFAULT SETTINGS              */
    DEI_FEAT_RPM_REF_DIV10                          ,  // Default Tach reference divided by 10 (make it one-byte nvfs variable)
    DEI_FEAT_VTACHO_REF                             ,
    
    DEI_FEAT_ENUM_SIZE

}Dei_Feat ;

#define DEI_FEAT_OUTPUT_BASE(i) ((Dei_Feat)(DEI_FEAT_OUTPUT1 + i))
#define DEI_FEAT_INPUT_BASE(i) ((Dei_Feat)(DEI_FEAT_IN_NEG1 + i))

/*--------------------------------------------------------------------------*/
/*SS FEATURE*/
/*--------------------------------------------------------------------------*/

/*SYSTEM ARMING MODE*/
#define DEI_SYS_ARMING_ACTIVE                   0x01
#define DEI_SYS_ARMING_PASSIVE_WITHOUT_LOCK     0x02
#define DEI_SYS_ARMING_PASSIVE_WITH_LOCK        0x03
//#define DEI_SYS_ARMING_REARMING_WITHOUT_LOCK    0x04
//#define DEI_SYS_ARMING_REARMING_WITH_LOCK       0x05

/*--------------------------------------------------------------------------*/

/*PANIC MODE*/
#define DEI_PANIC_MODE_ON                       0x01
#define DEI_PANIC_MODE_IGN_OFF_ONLY             0x02
#define DEI_PANIC_MODE_OFF                      0x03

/*--------------------------------------------------------------------------*/

/*CONFIRMATION CHIRPS*/
#define DEI_CONFIRMATION_CHIRPS_ON_WARNING_ON   0x01
#define DEI_CONFIRMATION_CHIRPS_ON_WARNING_OFF  0x02
#define DEI_CONFIRMATION_CHIRPS_OFF_WARNING_ON  0x03
#define DEI_CONFIRMATION_CHIRPS_OFF_WARNING_OFF 0x04

/*--------------------------------------------------------------------------*/

/*HORN FUNCTION*/
#define DEI_HORN_FULL_ALARM                     0x01
#define DEI_HORN_NORMAL													0x02
//#define DEI_HORN_SIREN_20MS                     0x02
//#define DEI_HORN_SIREN_30MS                     0x03
//#define DEI_HORN_SIREN_40MS                     0x04
//#define DEI_HORN_SIREN_50MS                     0x05

/*--------------------------------------------------------------------------*/

/*SENSOR TYPE */
#define DEI_SENSOR_TYPE_NONE                    0x01
#define DEI_SENSOR_TYPE_SHOCK                   0x02
#define DEI_SENSOR_TYPE_FIELD                   0x03
#define DEI_SENSOR_TYPE_TILT                    0x04
#define DEI_SENSOR_TYPE_GLASS                   0x05
#define DEI_SENSOR_TYPE_ULTRASONIC              0x06

/*--------------------------------------------------------------------------*/

/*ONE TIME BYPASS*/
//#define DEI_ONE_TIME_BYPASS_ON                  0x01
//#define DEI_ONE_TIME_BYPASS_OFF                 0x02

/*--------------------------------------------------------------------------*/

/*NPC */
#define DEI_NPC_ON                              0x01
#define DEI_NPC_OFF                             0x02

/*--------------------------------------------------------------------------*/

/*DOOR TRIGGER ERROR CHIRP*/
#define DEI_DOOR_TRIGGER_ERROR_CHIRP_OFF        0x01
#define DEI_DOOR_TRIGGER_ERROR_CHIRP_ON         0x02            

/*--------------------------------------------------------------------------*/
/*STARTER KILL OPTION*/
#define DEI_STARTER_KILL_OFF                    0x01
#define DEI_STARTER_KILL_ACTIVE                 0x02
#define DEI_STARTER_KILL_PASSIVE                0x03
#define DEI_STARTER_KILL_BAC					0x04

/*--------------------------------------------------------------------------*/
/*    DEI_FEAT_HORN_OUTPUT_TIMING   */
#define DEI_HORN_OUTPUT_10MS                    0x01
#define DEI_HORN_OUTPUT_20MS                    0x02
#define DEI_HORN_OUTPUT_30MS                    0x03
#define DEI_HORN_OUTPUT_40MS                    0x04
#define DEI_HORN_OUTPUT_50MS                    0x05
#define DEI_HORN_OUTPUT_60MS                    0x06

/*--------------------------------------------------------------------------*/
/*    DEI_FEAT_UNLOCK_DISARM_WITH_TRUNK_RELEASE  */
#define DEI_UNLOCK_DISARM_WITH_TRUNK_OFF               0x01
#define DEI_UNLOCK_DISARM_WITH_TRUNK_SMART             0x02
#define DEI_UNLOCK_DISARM_WITH_TRUNK_ON                0x03

/*--------------------------------------------------------------------------*/
/*    DEI_FEAT_OEM_DISARM_WITH_AUX   */
#define DEI_OEM_DISARM_WITH_AUX_ON              0x01
#define DEI_OEM_DISARM_WITH_AUX_OFF             0x02

/*--------------------------------------------------------------------------*/
/*    DEI_FEAT_AUTO_REARM   */
#define DEI_FEAT_AUTO_REARM_NO_LOCK             0x01
#define DEI_FEAT_AUTO_REARM_LOCK          		  0x02

/*--------------------------------------------------------------------------*/
/*RS FEATURE*/
/*--------------------------------------------------------------------------*/

/*SUPPORTED TRANSMISION MODE*/
//#define DEI_SUPPORTED_TM_AUTOMATIC                  0x01
//#define DEI_SUPPORTED_TM_MANUAL                     0x02
//#define DEI_SUPPORTED_TM_BOTH                       0x03

/*--------------------------------------------------------------------------*/

/*ENGINE CHECK MODE*/
#define DEI_ECM_VIRTUAL_TACHO                       0x01
#define DEI_ECM_VOLTAGE                             0x02
#define DEI_ECM_OFF                                 0x03
#define DEI_ECM_TACHOMETER                          0x04

/*--------------------------------------------------------------------------*/

/*CRANKING TIME*/
#define DEI_CRANK_TIME_600MS                        0x01
#define DEI_CRANK_TIME_800MS                        0x02
#define DEI_CRANK_TIME_1000MS                       0x03
#define DEI_CRANK_TIME_1200MS                       0x04
#define DEI_CRANK_TIME_1400MS                       0x05
#define DEI_CRANK_TIME_1600MS                       0x06
#define DEI_CRANK_TIME_1800MS                       0x07
#define DEI_CRANK_TIME_2000MS                       0x08
#define DEI_CRANK_TIME_4000MS                       0x09

/*--------------------------------------------------------------------------*/

/*REAR DEFOGGER OUTPUT TYPE*/
//#define DEI_DEFOGGER_LATCH                          0x01
//#define DEI_DEFOGGER_PULSED                         0x02

/*--------------------------------------------------------------------------*/

/*PARKING LIGHT OUTPUT*/
#define DEI_PARKING_LIGHT_CONSTANT                  0x01
#define DEI_PARKING_LIGHT_PULSED                    0x02
#define DEI_PARKING_LIGHT_OFF                       0x03

/*--------------------------------------------------------------------------*/

/*TIMER MODE START COUNT*/
#define DEI_TM_START_COUNT_01                       0x01
#define DEI_TM_START_COUNT_02                       0x02
#define DEI_TM_START_COUNT_03                       0x03
#define DEI_TM_START_COUNT_04                       0x04
#define DEI_TM_START_COUNT_06                       0x05
#define DEI_TM_START_COUNT_08                       0x06
#define DEI_TM_START_COUNT_10                       0x07
#define DEI_TM_START_COUNT_12                       0x08
#define DEI_TM_START_COUNT_14                       0x09
#define DEI_TM_START_COUNT_16                       0x0A
#define DEI_TM_START_COUNT_18                       0x0B
#define DEI_TM_START_COUNT_20                       0x0C
#define DEI_TM_START_COUNT_22                       0x0D
#define DEI_TM_START_COUNT_24                       0x0E


/*--------------------------------------------------------------------------*/

/*TIMER MODE START INTERVAL*/
#define DEI_TM_START_INTERVAL_01                    0x01
#define DEI_TM_START_INTERVAL_02                    0x02
#define DEI_TM_START_INTERVAL_03                    0x03
#define DEI_TM_START_INTERVAL_04                    0x04
#define DEI_TM_START_INTERVAL_06                    0x05
#define DEI_TM_START_INTERVAL_08                    0x06
#define DEI_TM_START_INTERVAL_10                    0x07
#define DEI_TM_START_INTERVAL_12                    0x08
#define DEI_TM_START_INTERVAL_14                    0x09
#define DEI_TM_START_INTERVAL_16                    0x0A
#define DEI_TM_START_INTERVAL_18                    0x0B
#define DEI_TM_START_INTERVAL_20                    0x0C
#define DEI_TM_START_INTERVAL_22                    0x0D
#define DEI_TM_START_INTERVAL_24                    0x0E

/*--------------------------------------------------------------------------*/

#define DEI_FEATURE_SMART_OFF                       0x01

/*--------------------------------------------------------------------------*/

/*SMART START LOW BATT*/
#define DEI_SM_LOW_BATT_9                           0x01
#define DEI_SM_LOW_BATT_9_5                         0x02
#define DEI_SM_LOW_BATT_10                          0x03
#define DEI_SM_LOW_BATT_10_5                        0x04
#define DEI_SM_LOW_BATT_11                          0x05
#define DEI_SM_LOW_BATT_11_5                        0x06
#define DEI_SM_LOW_BATT_12                          0x07
#define DEI_SM_LOW_BATT_12_5                        0x08

/*--------------------------------------------------------------------------*/

/*ACC STATE DURING WAIT TO START*/
//#define DEI_ACC_ON                                  0x01
//#define DEI_ACC_OFF                                 0x02

/*--------------------------------------------------------------------------*/

/*SECURE LOCK */
#define DEI_SECURE_LOCK_OFF                         0x01
#define DEI_SECURE_LOCK_ON                          0x02
#define DEI_SMART_SECURE_LOCK_ON                    0x03

/*--------------------------------------------------------------------------*/
/*   DEI_FEAT_TACHOMETER_INPUT  */
#define DEI_TACHOMETER                              0x01
#define DEI_SPEED_SENSOR                            0x02

/*--------------------------------------------------------------------------*/
/*   DEI_FEAT_PITSTOP_MODE_ENABLE  */
#define DEI_PITSTOP_MODE_ON                         0x01
#define DEI_PITSTOP_MODE_OFF                        0x02

/*--------------------------------------------------------------------------*/
/*   DEI_FEAT_TAKEOVER   */
#define DEI_TAKEOVER_ENABLE                         0x01
#define DEI_TAKEOVER_DISABLE_W_DOOR_OPEN            0x02
#define DEI_TAKEOVER_DISABLE_W_UNLOCK_CMD           0x03

/*--------------------------------------------------------------------------*/
/*   DEI_FEAT_OEM_RS_ACTIVATION  */
#define DEI_OEM_RS_ACTIVATION_DISABLE               0x01
#define DEI_OEM_RS_ACTIVATION_ENA_W_3XLOCK          0x02
#define DEI_OEM_RS_ACTIVATION_ENA_W_LOCK_TRUNK      0x03
#define DEI_OEM_RS_ACTIVATION_ENA_W_LK_ULK_LK       0x04

/*--------------------------------------------------------------------------*/
/*   DEI_FEAT_MANUAL_TRANSMISSION_INIT  */
#define DEI_RS_MANUAL_TRAN_INIT_REMOTE          		0x01
#define DEI_RS_MANUAL_TRAN_INIT_2_PARK_BRAKE    		0x02
#define DEI_RS_MANUAL_TRAN_INIT_PARK_BRAKE      		0x03

/*--------------------------------------------------------------------------*/
/*   DEI_FEAT_MANUAL_TRANSMISSION_COMPLETION  */
#define DEI_MANUAL_TRANS_COMPL_1              			0x01
#define DEI_MANUAL_TRANS_COMPL_2          					0x02
#define DEI_MANUAL_TRANS_COMPL_3							      0x03

/*--------------------------------------------------------------------------*/
/*SYSTEM FEATURE*/
/*--------------------------------------------------------------------------*/

/*IGN CONTROOLED LOCKS*/
#define DEI_NO_IGN_LOCKING                          0x01
#define DEI_IGN_LOCK_UNLOCK                         0x02
#define DEI_IGN_LOCK_ONLY                           0x03
#define DEI_IGN_UNLOCK_ONLY                         0x04

/*--------------------------------------------------------------------------*/

/*DOOR LOCK PULSES*/
#define DEI_DL_PULSES_SINGLE                        0x01
#define DEI_DL_PULSES_DOUBLE_UNLOCK_ONLY            0x02
//#define DEI_DL_PULSES_DOUBLE_LOCK_ONLY              0x03
#define DEI_DL_PULSES_DOUBLE_LOCK_UNLOCK            0x03

/*--------------------------------------------------------------------------*/

/*DOOR LOCK OUTPUT DURATION*/
#define DEI_DL_DURATION_800MS                       0x01
#define DEI_DL_DURATION_3600MS                      0x02
#define DEI_DL_DURATION_400MS                       0x03

/*--------------------------------------------------------------------------*/

/*SECOND UNLOCK*/
//#define DEI_2ND_UNLOCK_OFF                          0x01
//#define DEI_2ND_UNLOCK_AFTER_FIRST                  0x02
//#define DEI_2ND_UNLOCK_WITH_FIRST                   0x03

/*--------------------------------------------------------------------------*/

/*COMFORT CLOSURE*/
#define DEI_COMFORT_OFF                             0x01
#define DEI_COMFORT_ACTI_WITH_LOCK      		        0x02
#define DEI_COMFORT_ACTI_AFTER_LOCK                 0x03

/*--------------------------------------------------------------------------*/

/*HOOD TRIGGER*/
#define DEI_HOOD_NORMALLY_OPEN                      0x01
#define DEI_HOOD_NORMALLY_CLOSE                     0x02

/*--------------------------------------------------------------------------*/

/*IGN CONTROLLED DOMELIGHT*/
#define DEI_IGN_DOME_OFF                            0x01
#define DEI_IGN_DOME_ON                             0x02

/*--------------------------------------------------------------------------*/

/*OEM ALARM DISARM CH2*/
#define DEI_OEM_DISARM_CH2_OFF                      0x01
#define DEI_OEM_DISARM_CH2_ON                       0x02

/*--------------------------------------------------------------------------*/

/*OEM DISARM OUTPUT*/
#define DEI_OEM_DISARM_WITH_UNLOCK                  0x01
#define DEI_OEM_DISARM_BEFORE_UNLOCK                0x02
#define DEI_OEM_DISARM_RS_ONLY                      0x03

/*--------------------------------------------------------------------------*/

/*AUX ICON*/
#define DEI_AUX_ICON_TRUNK                          0x01
#define DEI_AUX_ICON_WINDOW                         0x02
#define DEI_AUX_ICON_SUNROOF                        0x03
#define DEI_AUX_ICON_AUDIO                          0x04
#define DEI_AUX_ICON_LIGHTS                         0x05
#define DEI_AUX_ICON_LEFT_DOOR                      0x06
#define DEI_AUX_ICON_RIGHT_DOOR                     0x07
#define DEI_AUX_ICON_REAR_HATCH                     0x08
#define DEI_AUX_ICON_TIMED                          0x09
#define DEI_AUX_ICON_PULSED                         0x0A
#define DEI_AUX_ICON_GARAGE_DOOR                    0x0B

/*--------------------------------------------------------------------------*/

/*AUX OUTPUT*/
#define DEI_AUX_OUTPUT_VALIDITY                     0x01
#define DEI_AUX_OUTPUT_LATCH                        0x02
#define DEI_AUX_OUTPUT_LATCH_RESET_IGN              0x03
#define DEI_AUX_OUTPUT_TIMED                        0x04
#define DEI_AUX_OUTPUT_OFF                          0x05

/*--------------------------------------------------------------------------*/

/*AUX LINKING*/
#define DEI_AUX_NO_LINKING                          0x01
#define DEI_AUX_LINK_TO_ARM                         0x02
#define DEI_AUX_LINK_TO_DISARM                      0x03
#define DEI_AUX_LINK_TO_ARM_DISARM                  0x04
#define DEI_AUX_LINK_TO_REMOTE_START                0x05

/*--------------------------------------------------------------------------*/

/*CH2 OUTPUT TYPE*/
#define DEI_CH2_OUTPUT_VALIDITY                     0x01
#define DEI_CH2_OUTPUT_OFF                          0x02

/*--------------------------------------------------------------------------*/

/*REMOTE KEYBOARD UNLOCK*/
#define DEI_REMOTE_KEYBOARD_UNLOCK_OFF              0x01
#define DEI_REMOTE_KEYBOARD_UNLOCK_ON               0x02

/*--------------------------------------------------------------------------*/

/*TRANSMITTER PROGRAMMING*/
#define DEI_TRANSMITTER_PROGRAMMING_OFF             0x01
#define DEI_TRANSMITTER_PROGRAMMING_ON              0x02


/*--------------------------------------------------------------------------*/

/*OEM DISARM*/
#define DEI_IGNITION_DISARM_OFF                          0x01
#define DEI_IGNITION_DISARM_ON                           0x02

/*--------------------------------------------------------------------------*/
/*    DEI_FEAT_DOOR_SWITCH_TYPE             */
#define DEI_DOOR_SWITCH_NORMALLY_OPEN               0x01
#define DEI_DOOR_SWITCH_NORMALLY_CLOSE              0x02

/*--------------------------------------------------------------------------*/
/*    DEI_FEAT_TRUNK_SWITCH_TYPE            */
#define DEI_TRUNK_SWITCH_NORMALLY_OPEN              0x01
#define DEI_TRUNK_SWITCH_NORMALLY_CLOSE             0x02

/*--------------------------------------------------------------------------*/
/*    DEI_FEAT_CONTROL_DOOR_LOCK_OPTION     */
#define DEI_CTRL_DOOR_LOCK_OFF                      0x01
#define DEI_CTRL_DOOR_LOCK_IGNITION                 0x02
#define DEI_CTRL_DOOR_LOCK_RPM                      0x03

/*--------------------------------------------------------------------------*/
/*    DEI_FEAT_CONTROL_DOOR_LOCK_FUNC       */
#define DEI_CTRL_DOOR_LOCK_UNLOCK                   0x01
#define DEI_CTRL_DOOR_LOCK_ONLY                     0x02
#define DEI_CTRL_DOOR_UNLOCK_ONLY                   0x03

/*--------------------------------------------------------------------------*/
/*    DEI_FEAT_CONTROL_UNLOCK_ALL_DOOR      */
#define DEI_CTRL_UNLOCK_ALL_DOOR_OFF                0x01
#define DEI_CTRL_UNLOCK_ALL_DOOR_ON                 0x02

/*--------------------------------------------------------------------------*/
/*    DEI_FEAT_DRIVER_DOOR_UNLOCK_PRIORITY  */
#define DEI_DRIVER_DOOR_UNLOCK_PRIORITY_ON          0x01
#define DEI_DRIVER_DOOR_UNLOCK_PRIORITY_OFF         0x02

/*--------------------------------------------------------------------------*/
/*    DEI_FEAT_PLIGHT_CONFIRMATION          */
#define DEI_PLIGHT_CONFIRMATION_ON                  0x01
#define DEI_PLIGHT_CONFIRMATION_OFF                 0x02

/*--------------------------------------------------------------------------*/
/*    DEI_FEAT_ANTENNA_LED_FLASH_OPTION     */
#define DEI_ANTENNA_LED_FLASH_ON                    0x01
#define DEI_ANTENNA_LED_FLASH_OFF                   0x02


/*--------------------------------------------------------------------------*/
/*PROGRAMMMABLE OUTPUTS*/
/*--------------------------------------------------------------------------*/

#define DEI_OUT_PRG_FACTORY_ALARM_ARM               0x01
#define DEI_OUT_PRG_GROUND_WHEN_ARM                 0x02
#define DEI_OUT_PRG_FACTORY_ALARM_DISARM            0x03
#define DEI_OUT_PRG_LOCK                            0x04
#define DEI_OUT_PRG_GROUND_WHEN_LOCKED              0x05
#define DEI_OUT_PRG_UNLOCK                          0x06
#define DEI_OUT_PRG_PULSE_ON_2ND_UNLOCK             0x07
#define DEI_OUT_PRG_GROUND_WHEN_UNLOCKED_BY_USER2   0x08
#define DEI_OUT_PRG_TRUNK_RELEASE                   0x09
#define DEI_OUT_PRG_AUX_1                           0x0A
#define DEI_OUT_PRG_AUX_2                           0x0B
#define DEI_OUT_PRG_AUX_3                           0x0C
#define DEI_OUT_PRG_AUX_4                           0x0D
//#define DEI_OUT_PRG_GROUND_WHILE_RUNNING_STATUS     0x0E
//#define DEI_OUT_PRG_GROUND_WHILE_RUNNING_5SEC       0x0F
//#define DEI_OUT_PRG_PULSE_BEFORE_STARTING           0x10
#define DEI_OUT_PRG_SMARTKEY_CONTROL_PTS            0x0E
#define DEI_OUT_PRG_PULSE_AFTER_STARTUP             0x0F
//#define DEI_OUT_PRG_PULSE_AFTER_IGNITION_OFF_1MIN   0x13
#define DEI_OUT_PRG_ACCESSORY                       0x10
#define DEI_OUT_PRG_IGNITION                        0x11
#define DEI_OUT_PRG_STARTER                         0x12
#define DEI_OUT_PRG_PARKING_LIGHTS                  0x13
//#define DEI_OUT_PRG_REAR_DEFOGGER                   0x18
#define DEI_OUT_PRG_DOME_LIGHT                      0x14
#define DEI_OUT_PRG_HORN                            0x15
#define DEI_OUT_PRG_ANTI_GRIND                      0x16
#define DEI_OUT_PRG_DEFROSTER_LATCH									0x17
#define DEI_OUT_PRG_DEFROSTER_PULSED								0x18
#define DEI_OUT_PRG_LED_FLASH												0x19
#define DEI_OUT_PRG_COMFORT_CLOSURE									0x1A
#define DEI_OUT_PRG_GROUND_WHEN_READY_MODE					0x1B
#define DEI_OUT_PRG_STATUS													0x1C
//#define DEI_OUT_PRG_GROUND_WHEN_ALARM_TRIGGERED     0x1B

//#define DEI_OUT_PRG_STATUS                          0x1D    //VER 10

#define DEI_PROG_OUT_COUNT                          17


/*--------------------------------------------------------------------------*/
/*PROGRAMMMABLE INPUTS*/
/*--------------------------------------------------------------------------*/


#define DEI_PROG_IN_COUNT                          10
#define DEI_IN_PRG_FUNCTION_START_TRIGGER						0x01
#define DEI_IN_PRG_FUNCTION_BRAKE        						0x02
#define DEI_IN_PRG_FUNCTION_DOOR         						0x03
#define DEI_IN_PRG_FUNCTION_IN_HOOD      						0x04
#define	DEI_IN_PRG_FUNCTION_INST_TRIGGER						0x05
#define DEI_IN_PRG_FUNCTION_HANDBRAKE    						0x06
#define DEI_IN_PRG_FUNCTION_HEADLIGHT_DETECT 				0x07
#define DEI_IN_PRG_FUNCTION_IN_TRUNK     						0x08
#define DEI_IN_PRG_VALET_SWITCH						 					0x09
#define DEI_IN_PRG_FUNCTION_IGNITION     						0x0A

/*--------------------------------------------------------------------------*/
/*SENSE CONFIGURATION*/
/*--------------------------------------------------------------------------*/

#define DEI_SENSE_WIRE_ENABLE                       0x01
#define DEI_SENSE_WIRE_DISABLE                      0x02

/*--------------------------------------------------------------------------*/
/*FEATURE CUSTOM 01*/
/*--------------------------------------------------------------------------*/
#define DEI_MTS_COMPLETION_BY_REMOTE                0x01
#define DEI_MTS_COMPLETION_DOOR_OPEN_CLOSE          0x02

/*--------------------------------------------------------------------------*/
/*FEATURE CUSTOM 02*/
/*--------------------------------------------------------------------------*/
#define DEI_TRUNK_CONSTANT_OUTPUT                   0x01
#define DEI_TRUNK_1SEC_PULSE                        0x02

/*--------------------------------------------------------------------------*/
/*FEATURE CUSTOM 02*/
/*--------------------------------------------------------------------------*/
#define DEI_TACH_NOT_LEARNED                        0x01
#define DEI_TACH_LEARNED		                        0x02

/*--------------------------------------------------------------------------*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
Boolean dei_feature_init ( void );
Boolean dei_feature_set  ( Dei_Feat index , UInt8 value );
Boolean dei_feature_get  ( Dei_Feat index   );
Boolean dei_feature_reset( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

