/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: cmd.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/
#ifndef __CMD_H__
#define __CMD_H__
/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "q.h"
#include "exec.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef enum
{
// !!! ORDER IS CRITICAL              !!! 
// !!! cmd_trace.ptp FOLLOW THIS LIST !!!
// !!! ADD NEW AT THE END             !!! 
// !!! AND UPDATE DOCKLIGHT PROJECT   !!! 


//  CMD                                     DEC     HEXA  
    INPUT_INVALID                         =   0, // 0x000
    INPUT_GWR                             =   1, // 0x001
    INPUT_ACCESSORY                       =   2, // 0x002
    INPUT_IGNITION                        =   3, // 0x003
    INPUT_STARTER                         =   4, // 0x004
    INPUT_DOME                            =   5, // 0x005
    INPUT_PARKING                         =   6, // 0x006
    INPUT_PUSH                            =   7, // 0x007
    INPUT_LONG_PUSH                       =   8, // 0x008
    INPUT_EXT_PUSH                        =   9, // 0x009
    INPUT_LONG_EXT_PUSH                   =  10, // 0x00A
    INPUT_POS1                            =  11, // 0x00B
    INPUT_RS_ACCESSORY                    =  12, // 0x00C
    INPUT_RS_IGNITION                     =  13, // 0x00D
    INPUT_RS_STARTER                      =  14, // 0x00E
    INPUT_ARM                             =  15, // 0x00F
    INPUT_DISARM                          =  16, // 0x010
    INPUT_LOCK1                           =  17, // 0x011
    INPUT_LOCK2                           =  18, // 0x012
    INPUT_LOCK3                           =  19, // 0x013
    INPUT_UNLOCK_DRIVER1                  =  20, // 0x014
    INPUT_UNLOCK_DRIVER2                  =  21, // 0x015
    INPUT_UNLOCK_DRIVER3                  =  22, // 0x016
    INPUT_UNLOCK_ALL1                     =  23, // 0x017
    INPUT_UNLOCK_ALL2                     =  24, // 0x018
    INPUT_UNLOCK_ALL3                     =  25, // 0x019
    INPUT_TRUNK                           =  26, // 0x01A
    INPUT_AUX1                            =  27, // 0x01B
    INPUT_AUX2                            =  28, // 0x01C
    INPUT_AUX3                            =  29, // 0x01D
    INPUT_AUX4                            =  30, // 0x01E
    INPUT_PANIC                           =  31, // 0x01F
    INPUT_START                           =  32, // 0x020
    INPUT_STOP                            =  33, // 0x021
    INPUT_LONG_START                      =  34, // 0x022
    INPUT_SHORT_TRUNK                     =  35, // 0x023
    INPUT_CHIRP                           =  36, // 0x024
    INPUT_LED                             =  37, // 0x025
    INPUT_READ_CONFIG                     =  38, // 0x026
    INPUT_WRITE_CONFIG                    =  39, // 0x027
    INPUT_DEFOGGER_ON                     =  40, // 0x028
    INPUT_DEFOGGER_OFF                    =  41, // 0x029
    INPUT_TRIGGER_ON                      =  42, // 0x02A
    INPUT_TRIGGER_OFF                     =  43, // 0x02B
    INPUT_WARNING_ON                      =  44, // 0x02C
    INPUT_PUSH_VALET_PRESS                =  45, // 0x02D
    INPUT_PUSH_VALET_RELEASE              =  46, // 0x02E                    
    INPUT_EXT_PUSH_VALET_PRESS            =  47, // 0x02F
    INPUT_EXT_PUSH_VALET_RELEASE          =  48, // 0x030
    INPUT_OEM_LOCK                        =  49, // 0x031
    INPUT_OEM_UNLOCK                      =  50, // 0x032
    INPUT_OEM_DISARM                      =  51, // 0x033
    INPUT_OEM_ARM                         =  52, // 0x034
    INPUT_OEM_TRUNK                       =  53, // 0x035
    INPUT_OEM_PANIC                       =  54, // 0x036
    INPUT_OEM_AUX1                        =  55, // 0x037
    INPUT_OEM_AUX2                        =  56, // 0x038
    INPUT_OEM_AUX3                        =  57, // 0x039
    INPUT_OEM_AUX4                        =  58, // 0x03A
    INPUT_OEM_RS_ON                       =  59, // 0x03B
    INPUT_OEM_RS_OFF                      =  60, // 0x03C
    INPUT_BRAKE_STATUS_PRESS              =  61, // 0x03D
    INPUT_BRAKE_STATUS_RELEASE            =  62, // 0x03E                            
    INPUT_PRK_NEUTRAL_STATUS_ON           =  63, // 0x03F
    INPUT_PRK_NEUTRAL_STATUS_OFF          =  64, // 0x040                               
    INPUT_HANDBRAKE_STATUS_ON             =  65, // 0x041
    INPUT_HANDBRAKE_STATUS_OFF            =  66, // 0x042
    INPUT_HOOD_STATUS_OPEN                =  67, // 0x043
    INPUT_HOOD_STATUS_CLOSE               =  68, // 0x044                              
    INPUT_TRUNK_STATUS_OPEN               =  69, // 0x045
    INPUT_TRUNK_STATUS_CLOSE              =  70, // 0x046                                  
    INPUT_ALL_DOOR_STATUS_OPEN            =  71, // 0x047
    INPUT_ALL_DOOR_STATUS_CLOSE           =  72, // 0x048                                                  
    INPUT_GLOWPLUG_STATUS_ON              =  77, // 0x04D
    INPUT_GLOWPLUG_STATUS_OFF             =  78, // 0x04E                           
    INPUT_SENSOR_TRIGGER                  =  79, // 0x04F
    INPUT_SENSOR_WARNING                  =  80, // 0x050
    INPUT_KEY_IN_STATUS_ON                =  81, // 0x051
    INPUT_KEY_IN_STATUS_OFF               =  82, // 0x052
    INPUT_REQ_INPUT_STATUS                =  83, // 0x053
    INPUT_REQ_RPM                         =  84, // 0x054
    INPUT_REQ_SPEED                       =  85, // 0x055
    INPUT_REQ_ENG_TEMP                    =  86, // 0x056
    INPUT_REQ_EXTERIOR_TEMP               =  87, // 0x057
    INPUT_REQ_INTERIOR_TEMP               =  88, // 0x058
    INPUT_REQ_DTC                         =  89, // 0x059
    INPUT_REQ_CLR_DTC                     =  90, // 0x05A
    INPUT_REQ_FUEL_LEVEL                  =  91, // 0x05B
    INPUT_REQ_ODO                         =  92, // 0x05C
    INPUT_REQ_BAT_VOLT                    =  93, // 0x05D
    INPUT_REQ_TPMS                        =  94, // 0x05E
    INPUT_REQ_VIN_1                       =  95, // 0x05F
    INPUT_REQ_VIN_2                       =  96, // 0x060
    INPUT_REQ_VIN_3                       =  97, // 0x061
    INPUT_REQ_VIN_4                       =  98, // 0x062
    INPUT_REQ_VIN_5                       =  99, // 0x063
    INPUT_REQ_VIN_6                       = 100, // 0x064
    INPUT_SKD_SET                         = 101, // 0x065
    INPUT_SKD_CLR                         = 102, // 0x066
    INPUT_SPEED_SET                       = 103, // 0x067
    INPUT_SPEED_CLR                       = 104, // 0x068
    INPUT_RPM_SET                         = 105, // 0x069
    INPUT_RPM_CLR                         = 106, // 0x06A
    INPUT_CUSTOM_NEG1                     = 107, // 0x06B
    INPUT_CUSTOM_NEG2                     = 108, // 0x06C
    INPUT_CUSTOM_NEG3                     = 109, // 0x06D
    INPUT_CUSTOM_NEG4                     = 110, // 0x06E
    INPUT_CUSTOM_NEG5                     = 111, // 0x06F
    INPUT_CUSTOM_NEG6                     = 112, // 0x070
    INPUT_CUSTOM_NEG7                     = 113, // 0x071
    INPUT_CUSTOM_POS1                     = 114, // 0x072
    INPUT_CUSTOM_POS2                     = 115, // 0x073
    INPUT_CUSTOM_POS3                     = 116, // 0x074                     
    INPUT_RF_ENTRY                        = 117, // 0x075  
    INPUT_RF_LOCK                         = 118, // 0x076
    INPUT_RF_UNLOCK                       = 119, // 0x077
    INPUT_RF_START                        = 120, // 0x078
    INPUT_RF_STOP                         = 121, // 0x079
    INPUT_RF_START_STOP_TOOGLE            = 122, // 0x07A
    INPUT_RF_TRUNK                        = 123, // 0x07B
    INPUT_RF_PANIC                        = 124, // 0x07C
    INPUT_RF_STATUS                       = 125, // 0x07D
    INPUT_RF_STATUS_ALARM                 = 126, // 0x07E
    INPUT_RF_STATUS_RS                    = 127, // 0x07F
    INPUT_RF_AUX1                         = 128, // 0x080
    INPUT_RF_AUX2                         = 129, // 0x081
    INPUT_RF_AUX3                         = 130, // 0x082
    INPUT_RF_AUX4                         = 131, // 0x083
    INPUT_RF_AUX5                         = 132, // 0x084
    INPUT_RF_AUX6                         = 133, // 0x085
    INPUT_RF_AUX7                         = 134, // 0x086
    INPUT_RF_AUX8                         = 135, // 0x087
    INPUT_RF_REAR_DEFOGGER                = 136, // 0x088
    INPUT_RF_SENSOR1_ADJUST               = 137, // 0x089
    INPUT_RF_SENSOR2_ADJUST               = 138, // 0x08A
    INPUT_RF_SENSOR3_ADJUST               = 139, // 0x08B
    INPUT_RF_SENSOR4_ADJUST               = 140, // 0x08C
    INPUT_RF_SENSOR_BYPASS_LEVEL          = 141, // 0x08D
    INPUT_RF_COLD_START_TOOGLE            = 142, // 0x08E
    INPUT_RF_TEMP_ADJUST                  = 143, // 0x08F
    INPUT_RF_TIME_START_ADJUST            = 144, // 0x090
    INPUT_RF_ALARM_CODE_REQ               = 145, // 0x091
    INPUT_RF_NOT_USED_BC                  = 146, // 0x092
    INPUT_RF_ERROR_START_CODE_REQ         = 147, // 0x093
    INPUT_RF_VALET_TOOGLE                 = 148, // 0x094
    INPUT_RF_CAR_FINDER                   = 149, // 0x095
    INPUT_RF_TEMP_REPORT_REQ              = 150, // 0x096
    INPUT_RF_RUNTIME_CHECK                = 151, // 0x097
    INPUT_RF_NOT_USED_C2                  = 152, // 0x098
    INPUT_RF_TIMER_MODE_TOOGLE            = 153, // 0x099
    INPUT_RF_SMART_START_TOOGLE           = 154, // 0x09A
    INPUT_RF_RUNTIME_RESET                = 155, // 0x09B
    INPUT_RF_SILENT_LOCK                  = 156, // 0x09C
    INPUT_RF_SILENT_UNLOCK                = 157, // 0x09D
    INPUT_RF_WINDOWS_ROLL_UP              = 158, // 0x09E
    INPUT_RF_WINDOWS_ROLL_DOWN            = 159, // 0x09F
    INPUT_RF_GARAGE_DOOR_TOOGLE           = 160, // 0x0A0
    INPUT_RF_GARAGE_DOOR_OPEN             = 161, // 0x0A1
    INPUT_RF_GARAGE_DOOR_CLOSE            = 162, // 0x0A2
    INPUT_RF_GARAGE_DOOR_STATUS           = 163, // 0x0A3
    INPUT_RF_AUX_ARM                      = 164, // 0x0A4
    INPUT_RF_AUX_DISARM                   = 165, // 0x0A5
    INPUT_RF_NOT_USED_D0                  = 166, // 0x0A6
    INPUT_RF_AUX_LOCK                     = 167, // 0x0A7
    INPUT_RF_AUX_UNLOCK                   = 168, // 0x0A8
    INPUT_RF_AUX_ARM_LOCK                 = 169, // 0x0A9
    INPUT_RF_AUX_DISARM_UNLOCK            = 170, // 0x0AA
    INPUT_RF_AUX_STATUS                   = 171, // 0x0AB
    INPUT_RF_LEARNING_BEGIN               = 172, // 0x0AC
    INPUT_RF_EXIT_PROGRAMMING_MODE        = 173, // 0x0AD
    INPUT_RF_ENTER_PROGRAMMING_MODE       = 174, // 0x0AE
    INPUT_RF_ADJUST_PLUS                  = 175, // 0x0AF
    INPUT_RF_ADJUST_MINUS                 = 176, // 0x0B0
    INPUT_RF_SENSOR_SILENT_ARM            = 177, // 0x0B1
    INPUT_RF_NOT_USED_DC                  = 178, // 0x0B2
    INPUT_RF_NOT_USED_DD                  = 179, // 0x0B3
    INPUT_RF_FULL_SILENT_ARM              = 180, // 0x0B4
    INPUT_RF_TRIGGER_HISTORY_REQUEST      = 181, // 0x0B5
    INPUT_RF_PUT_NEW_SENSOR1_VALUE        = 182, // 0x0B6
    INPUT_RF_TEMP_START_ADJUST            = 183, // 0x0B7
    INPUT_RF_PUT_NEW_TEMP                 = 184, // 0x0B8
    INPUT_RF_START_TIMES_ADJUST           = 185, // 0x0B9
    INPUT_RF_PUT_NEW_START_TIMES          = 186, // 0x0BA
    INPUT_RF_ONE_BUTTON_FUNCTION          = 187, // 0x0BB
    INPUT_RF_HOMEVALET                    = 188, // 0x0BC
    INPUT_RF_COMBO_VALET                  = 189, // 0x0BD
    INPUT_RF_TOGGEL_LED	                  = 190, // 0x0BE
    INPUT_RF_START_VALET                  = 191, // 0x0BF
    INPUT_RF_PROGRAMMING_DONE             = 192, // 0x0C0
    INPUT_RF_2NDCAR_LOCK                  = 193, // 0x0C1
    INPUT_RF_2NDCAR_UNLOCK                = 194, // 0x0C2
    INPUT_RF_2NDCAR_TRUNK                 = 195, // 0x0C3
    INPUT_REQ_DTC_CODE                    = 196, // 0x0C4
    INPUT_REQ_VIN                         = 197, // 0x0C5
    INPUT_REQ_PROTOCOL_VER                = 198, // 0x0C6
    INPUT_AL_QUERY                        = 199, // 0x0C7
    INPUT_AL_ALERT_CLEAR                  = 200, // 0x0C8
    INPUT_REQ_MODULE_INFO                 = 201, // 0x0C9
    INPUT_REQ_EXTENDED_STATUS_SUPPORT     = 202, // 0x0CA
    INPUT_REQ_EXTENDED_STATUS_SUPPORT_SYNC= 203, // 0x0CB
    INPUT_REQ_RUNTIME                     = 204, // 0x0CC
    INPUT_REQ_2WAY_STATUS                 = 205, // 0x0CD
    INPUT_REQ_EXTENDED_STATUS             = 206, // 0x0CE
    INPUT_VALET_TOGGLE                    = 207, // 0x0CF
    INPUT_CAR_FINDER                      = 208, // 0x0D0
    INPUT_EXTEND_RUNTIME                  = 209, // 0x0D1
    INPUT_DTC_ALERT                       = 210, // 0x0D2
    INPUT_OBD_CAN_ENABLE                  = 211, // 0x0D3
    INPUT_OBD_CAN_DISABLE                 = 212, // 0x0D4
    INPUT_START_ONLY                      = 213, // 0x0D5
    INPUT_EIPS_ALERT                      = 214, // 0x0D6
    INPUT_EIPS_SHUTDOWN                   = 215, // 0x0D7
    INPUT_EIPS_SHUTDOWN_FAIL              = 216, // 0x0D8
    INPUT_SECURE_DISARM                   = 217, // 0x0D9
    INPUT_SECURE_ARM                      = 218, // 0x0DA
    INPUT_OEM_LONG_LOCK                   = 219, // 0x0DB
    INPUT_OEM_ARMING                      = 220, // 0x0DC
    INPUT_PTS                             = 221, // 0x0DD
    
	//following commands introudced by NGMM, must be in this order
	INPUT_PUSH_VALET_RELEASE_             = 222,//0xDE
	INPUT_PUSH_VALET_PRESS_               = 223,//0xDF
	INPUT_BUTTON_TAP1                     = 224,//0xE0
	INPUT_BUTTON_TAP1_HOLD                = 225,//0xE1
	INPUT_BUTTON_TAP2                     = 226,//0xE2
	INPUT_BUTTON_TAP2_HOLD                = 227,//0xE3
	INPUT_BUTTON_TAP3                     = 228,//0xE4
    INPUT_BUTTON_TAP3_HOLD                = 229,//0xE5
	INPUT_BUTTON_TAP4                     = 230,//0xE6
	INPUT_BUTTON_TAP4_HOLD                = 231,//0xE7
	INPUT_BUTTON_TAP5                     = 232,//0xE8
	INPUT_BUTTON_TAP5_HOLD                = 233,//0xE9
	//end of must
    
	// Internal state machine command insert
	INPUT_EMERGENCY_OVERRIDE              = 234,//0xEA
	INPUT_TACH_LEARN                      = 235,//0xEB
	INPUT_RS_SHUTDOWN_HISTORY             = 236,//0xEC
	INPUT_ALARM_TRIG_HISTORY              = 237,//0xED
	INPUT_ALARM_VALET                     = 238,//0xEE
	INPUT_RS_VALET                        = 239,//0xEF
	INPUT_REMOTE_PAIRING                  = 240,//0xF0
    INPUT_REMOTE_DELETE                   = 241,//0xF1
	INPUT_ZAP                             = 242,//0xF2
	INPUT_TACH_DELETE                     = 243,//0xF3
	INPUT_TACH_LEARN_DONE                 = 244,//0xF4
	INPUT_PRG_MODE_EXIT                   = 245,//0xF5
	INPUT_TAP_HOLD                        = 246,//0xF6
	INPUT_TWO_TAP_HOLD                    = 247,//0xF7
	INPUT_THREE_TAP_HOLD                  = 248,//0xF8
	INPUT_CONVENIENCE_ALERT               = 249,//0xF9
	// end
	
	INPUT_HEADLIGHT_STATUS_ON             = 250,//0xFA
	INPUT_HEADLIGHT_STATUS_OFF            = 251,//0xFB
	INPUT_INST_TRIG_STATUS_ON             = 252,//0xFC
	INPUT_RPM_CONTROL_LOCK                = 253,//0xFD
    INPUT_PARKING_STATUS_ON               = 254,//0xFE
    INPUT_PARKING_STATUS_OFF              = 255,//0xFF
    INPUT_HAZARD_STATUS_ON                = 256,//0x100
    INPUT_HAZARD_STATUS_OFF               = 257,//0x101
    INPUT_LO_BEAM_STATUS_ON               = 258,//0x102
    INPUT_LO_BEAM_STATUS_OFF              = 259,//0x103
    INPUT_HI_BEAM_STATUS_ON               = 260,//0x104
    INPUT_HI_BEAM_STATUS_OFF              = 261,//0x105

	//old RF commands (doesn't exist in SCPP)
    INPUT_RF_2NDCAR_START                 = 262,//0x106
    INPUT_RF_2NDCAR_STOP                  = 263,//0x107
	INPUT_GND_WHEN_READY_MODE_ON          = 264,//0x108
	INPUT_GND_WHEN_READY_MODE_OFF         = 265,//0x109
    
    // new skip commands
    INPUT_DOOR_SKIP_SET                   = 266,//0x10A
    INPUT_DOOR_SKIP_CLR                   = 267,//0x10B
    INPUT_BRAKE_SKIP_SET                  = 268,//0x10C
    INPUT_BRAKE_SKIP_CLR                  = 269,//0x10D
    INPUT_IGNITION_SKIP_SET               = 270,//0x10E
    INPUT_IGNITION_SKIP_CLR               = 271,//0x10F
    
    //Key2Go
    INPUT_KEY2GO_LOG_DONE                 = 272,//0x110

    //Vin   
    INPUT_VIN_IS_DONE                     = 273,//0x111  
    
    //program from ble
    INPUT_BLE_PRG_ENTER                   = 274,//0x112
    INPUT_BLE_PRG_EXIT                    = 275,//0x113
    
    // external sensor
    INPUT_REQ_SENS_TEMP                   = 276,//0x114

    INPUT_STARTERKILL_SUSPEND             = 277,//0x115
		
		INPUT_INST_TRIG_STATUS_OFF            = 278,//0x116

    INPUT_APP_FOB                         = 279,//0x117
		INPUT_TAMPER                          = 280,//0x118
	INPUT_IID_ENGINE_RUNNING                  = 281,//0x119	


    //                         initial M2M commands
		INPUT_M2M_DEFERRED = 282, // 0x11A
		INPUT_OIL_ALARM = 283,   //0x11B
	  INPUT_TIRE_ALARM = 284,  //0x11C
		INPUT_SEAT_BELTS = 285,      //0x11D
		INPUT_SEAT_WEIGHTS = 286,    //0x11E
		INPUT_EV_BATTERY_LVL = 287, // 0x11F
		INPUT_EV_RANGE = 288, //0x120
		INPUT_OIL_LIFE = 289, //0x121
		INPUT_FUEL_LVL = 290, //0x122
		INPUT_FUEL_CAP = 291,  //0x123
		INPUT_HORN = 292, // 0x124
		INPUT_HEADLAMP = 293, // 0x125
		INPUT_CABIN_TEMP = 294, // 0x126
		INPUT_TIRE_PRESSURE = 295, // 0x127
		INPUT_RIB          = 296,  // 0x128
		INPUT_ODOMETER     = 297,  // 0x129
		INPUT_BATTERY_VOLTAGE = 298, // 0x12A
		// no INPUT_SPEED because we already have INPUT_SPEED_SET and INPUT_SPEED_CLR
		
    INPUT_ON                              = 0x0200 ,
    INPUT_RF_EXT                          = 0x4000 ,
    INPUT_RF_RELEASE                      = 0x8000 ,

} Cmd ;

//--------------------------------------------------------------------------//

#define INPUT_GWR_ON              (Cmd)(INPUT_GWR          | INPUT_ON )
#define INPUT_GWR_OFF                   INPUT_GWR                     
#define INPUT_RS_ACCESSORY_ON     (Cmd)(INPUT_RS_ACCESSORY | INPUT_ON )
#define INPUT_RS_ACCESSORY_OFF          INPUT_RS_ACCESSORY                      
#define INPUT_RS_IGNITION_ON      (Cmd)(INPUT_RS_IGNITION  | INPUT_ON )
#define INPUT_RS_IGNITION_OFF           INPUT_RS_IGNITION             
#define INPUT_RS_STARTER_ON       (Cmd)(INPUT_RS_STARTER   | INPUT_ON )
#define INPUT_RS_STARTER_OFF            INPUT_RS_STARTER              
#define INPUT_IGN_STATUS_ON       (Cmd)(INPUT_IGNITION     | INPUT_ON )
#define INPUT_IGN_STATUS_OFF            INPUT_IGNITION      
#define INPUT_IGNITION_ON               INPUT_IGN_STATUS_ON
#define INPUT_IGNITION_OFF              INPUT_IGN_STATUS_OFF
#define INPUT_STARTER_ON          (Cmd)(INPUT_STARTER      | INPUT_ON )
#define INPUT_STARTER_OFF               INPUT_STARTER                                                                                              
#define INPUT_DOME_ON             (Cmd)(INPUT_DOME         | INPUT_ON )
#define INPUT_DOME_OFF                  INPUT_DOME                    
#define INPUT_PARKING_ON          (Cmd)(INPUT_PARKING      | INPUT_ON )
#define INPUT_PARKING_OFF               INPUT_PARKING                 
#define INPUT_POS1_ON             (Cmd)(INPUT_POS1         | INPUT_ON )
#define INPUT_POS1_OFF                  INPUT_POS1 
#define INPUT_ACCESSORY_ON              INPUT_POS1_ON
#define INPUT_ACCESSORY_OFF             INPUT_POS1_OFF                   
#define INPUT_PUSH_PRESS          (Cmd)(INPUT_PUSH         | INPUT_ON )
#define INPUT_PUSH_RELEASE              INPUT_PUSH                                                
#define INPUT_EXT_PUSH_PRESS      (Cmd)(INPUT_EXT_PUSH     | INPUT_ON )
#define INPUT_EXT_PUSH_RELEASE          INPUT_PUSH      
#define INPUT_EIPS_SHUTDOWN_FAIL_ON     (Cmd)(INPUT_EIPS_SHUTDOWN_FAIL     | INPUT_ON )
#define INPUT_EIPS_SHUTDOWN_FAIL_OFF    INPUT_EIPS_SHUTDOWN_FAIL  

#define INPUT_RF_START_SMARTSTART       INPUT_START | INPUT_ON
#define INPUT_RF_STOP_SMARTSTART        INPUT_STOP  | INPUT_ON

#define INPUT_PTS_ON              (Cmd)(INPUT_PTS          | INPUT_ON )
#define INPUT_PTS_OFF                   INPUT_PTS                     

#define INPUT_LOCK1_ON            (Cmd)(INPUT_LOCK1 | INPUT_ON)
#define INPUT_UNLOCK_DRIVER1_ON   (Cmd)(INPUT_UNLOCK_DRIVER1 | INPUT_ON)
#define INPUT_UNLOCK_ALL1_ON   (Cmd)(INPUT_UNLOCK_ALL1 | INPUT_ON)

#define INPUT_RIB_ON           (Cmd)(INPUT_RIB | INPUT_ON)
#define INPUT_RIB_OFF          (Cmd)(INPUT_RIB)

#define COMMAND_INSERT_TIMEOUT    500

//--------------------------------------------------------------------------//

typedef Cmd         Cmd_List ;

//--------------------------------------------------------------------------//

#define EXEC_MODE_NORMAL  (1u << 0)
#define EXEC_MODE_PROG    (1u << 1)
typedef UInt32 Exec_Mode;

//--------------------------------------------------------------------------//

typedef UInt32      Cmd_Pld ;

//--------------------------------------------------------------------------//

typedef void    (* Exec_Func) (void *) ;

//--------------------------------------------------------------------------//

typedef union s_Cmd_Rf_Pld
{    
    struct 
    {
        unsigned pld        :8  ;
        unsigned id         :2  ;    
        unsigned batt_low   :1  ;
        unsigned reserved   :21 ;
    }bits ;

    UInt32 pld ; 

}Cmd_Rf_Pld; 

//--------------------------------------------------------------------------//

typedef enum
{
    CMD_RF_SENSOR_BYPASS_OFF   = 0x00 ,
    CMD_RF_SENSOR_BYPASS_WARN  = 0x01 , 
    CMD_RF_SENSOR_BYPASS_SHOCK = 0x03 , 
    CMD_RF_SENSOR_BYPASS_OPT   = 0x08 ,
    CMD_RF_SENSOR_BYPASS_FULL  = 0x02 ,
    CMD_RF_SENSOR_BYPASS_TOGGLE= 0xFF 

}Cmd_Rf_Sensor_Bypass;

//--------------------------------------------------------------------------//

typedef enum
{
    CMD_LOCK_WITH_ARM        				= 1u << 0 ,    
    CMD_LOCK_WITH_COMFORT    				= 1u << 1 ,
    CMD_LOCK_OEM             				= 1u << 2 ,
    CMD_UNLOCK_WITH_DISARM   				= 1u << 3 ,
    CMD_UNLOCK_USER2         				= 1u << 4 ,
    CMD_UNLOCK_OEM           				= 1u << 6 ,
    CMD_UNLOCK_IGNITION      				= 1u << 7 ,
#ifdef PLATFORM_933   //leave bit 8:10 untouched
    CMD_UNLOCK_WITH_TRUNK_RELEASE   = 1u << 11,
		CMD_UNLOCK_WITH_TRUNK_CLOSE     = 1u << 12,  
    CMD_TRUNK_WITH_DISARM    				= 1u << 13 ,
    CMD_TRUNK_OEM            				= 1u << 14 ,
    CMD_LOCK_IGNITION        				= 1u << 15,
#else  
    CMD_TRUNK_WITH_DISARM    				= 1u << 8 ,
    CMD_TRUNK_OEM            				= 1u << 9 ,
    CMD_LOCK_IGNITION        				= 1u << 10,
    CMD_UNLOCK_WITH_TRUNK_RELEASE   = 1u << 11,
		CMD_UNLOCK_WITH_TRUNK_CLOSE     = 1u << 12,
#endif  

}Cmd_Lock_Pld ;

//-------------------------------------------------------------------------//

#ifdef PLATFORM_933

typedef enum
{
	DEI_SS_NO_SILENT = 0, 
	DEI_SS_SILENT    = 1,
	DEI_SS_SILENT_LOGIC_RESUME = 2,
	DEI_SS_PASSIVE_NO_LOCK = 3

}Dei_Ss_Silent ;

typedef enum
{
	CMD_EXT_WARNAWAY = 1,
	CMD_EXT_TRIGGER     ,
	
} Cmd_Ext_Trig_Pld;

#endif

typedef enum
{
    CMD_AUX_FROM_RF            = 1u << 0,   
    CMD_AUX_WITH_ARM           = 1u << 1,   
    CMD_AUX_WITH_DISARM        = 1u << 2,   
    CMD_AUX_WITH_RS            = 1u << 3,   
    CMD_AUX_RESET              = 1u << 4    

}Cmd_Aux_Pld ;

//-------------------------------------------------------------------------//

typedef enum
{
    CMD_RS_START_ERROR          = 1u << 0 , 
    CMD_RS_START_SUCCESSFUL     = 1u << 1 , 
    CMD_RS_INVALID_TAKEOVER     = 1u << 2 ,
    CMD_RS_VALID_TAKEOVER       = 1u << 3

}Cmd_Rs_Pld ;

//-------------------------------------------------------------------------//

typedef struct s_Cmd_Q_Msg
{
    Q_Msg       q_base ; // MUST BE FIRST ELEMENT OF THE STRUCT
        
    Cmd_List    cmd    ;

    Cmd_Pld     param  ;

} Cmd_Q_Msg ;

//--------------------------------------------------------------------------//

typedef struct s_Cmd_Array1
{
    UInt16     cmd  ; 
    Exec_Func  func ;

}Cmd_Array1 ;

#define CMD_LUT_INSERT1_RAM(x,mode)  do\
                                 { \
                                    UInt8 i ;\
                                    for( i = 0 ; i < sizeof_array(x) ; i++)\
                                        cmd_lut_insert((Cmd)x[i].cmd ,x[i].func,mode);\
                                }while(0)



#define CMD_LUT_INSERT1(x,mode)  cmd_lut_insert_table1(x,sizeof_array(x),mode)


typedef struct s_Cmd_Array2
{
    UInt16     cmd  ; 
    Exec_Func  func ;

    Exec_Mode  mode ;

}Cmd_Array2 ;

#define CMD_LUT_INSERT2_RAM(x)  do\
                            { \
                                UInt8 i ;\
                                for( i = 0 ; i < sizeof_array(x) ; i++)\
                                    cmd_lut_insert((Cmd)x[i].cmd ,x[i].func,x[i].mode);\
                            }while(0)


#define CMD_LUT_INSERT2(x)  cmd_lut_insert_table2(x,sizeof_array(x))

//--------------------------------------------------------------------------//    

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void      cmd_init            ( const UInt8 cmd_pip_size , const UInt8 cmd_q_free_size );
void      cmd_lut_insert      ( UInt16 cmd , Exec_Func func , Exec_Mode mode );
Boolean   cmd_lut_remove      ( UInt16 cmd , Exec_Func func );
void      cmd_lut_set_mode    ( Exec_Mode mode );                                                                                    
void      cmd_lut_clr_mode    ( Exec_Mode mode );
#if	!defined PLATFORM_CM800 && !defined PLATFORM_933		//ndef PLATFORM_CM800
    Boolean   cmd_q_insert        ( UInt16 cmd , TskTimeout timeout , ...);
#else
    Boolean   cmd_q_insert        ( UInt16 cmd , TskTimeout timeout);
    Boolean   cmd_q_rs_insert(UInt16 cmd , TskTimeout timeout, ...);
	  void cmd_q_rs_insert_hhu_num( UInt8 hhu_num , Cmd cmd, TskTimeout timeout , UInt32 payload );
#endif
Cmd_Pld*  cmd_get_pld         ( void *exec_arg );
Cmd_List* cmd_get_command     ( void *exec_arg );
void      cmd_init_reset      (void);


void cmd_lut_insert_table1(const Cmd_Array1* cmd_array,UInt8 size,Exec_Mode mode);
void cmd_lut_insert_table2(const Cmd_Array2* cmd_array,UInt8 size );


/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/


/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

