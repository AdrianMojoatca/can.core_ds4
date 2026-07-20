/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

//             <<< Use Configuration Wizard in Context Menu >>>             //

#ifndef __CONFIG_D2D_H__
#define __CONFIG_D2D_H__

//-------------------------------------------------------------------------//
// **********Copy CORE TEMPLATE HERE********** //
//-------------------------------------------------------------------------//

//-------------------------------------------------------------------------//
// <h> CONFIG D2D V.1.61
//-------------------------------------------------------------------------//
// <h> FW SPECIFIC               ->CHECK according to FW
//      <h> Function
//-------------------------------------------------------------------------//
// <e32.06> FN_ARM
// <o33.06> Enabled
// </e>
// <e32.05> FN_DISARM
// <o33.05> Enabled
// </e>
// <e34.07> FN_GWR_ON
// <o35.07> Enabled
// </e>
// <e2.07> FN_GWR_OFF
// <o3.07> Enabled
// </e>
// <e48.05> FN_IGN_ON
// <o49.05> Enabled
// </e>
// <e16.05> FN_IGN_OFF
// <o17.05> Enabled
// </e>
// <e48.03> FN_START_ON
// <o49.03> Enabled
// </e>
// <e16.03> FN_START_OFF
// <o17.03> Enabled
// </e>
// <e50.07> FN_PARKING_ON
// <o51.07> Enabled
// </e>
// <e18.07> FN_PARKING_OFF
// <o19.07> Enabled
// </e>
// <e36.06> FN_LOCK_REM1_ON
// <o37.06> Enabled
// </e>
// <e40.06> FN_LOCK_REM2_ON
// <o41.06> Enabled
// </e>
// <e44.06> FN_LOCK_REM3_ON
// <o45.06> Enabled
// </e>
// <e36.05> FN_UNLOCK_DR_REM1_ON
// <o37.05> Enabled
// </e>
// <e40.05> FN_UNLOCK_DR_REM2_ON
// <o41.05> Enabled
// </e>
// <e44.05> FN_UNLOCK_DR_REM3_ON
// <o45.05> Enabled
// </e>
// <e38.05> FN_UNLOCK_ALL_REM1_ON
// <o39.05> Enabled
// </e>
// <e42.05> FN_UNLOCK_ALL_REM2_ON
// <o43.05> Enabled
// </e>
// <e46.05> FN_UNLOCK_ALL_REM3_ON
// <o47.05> Enabled
// </e>
// <e36.03> FN_TRUNK_REM1_ON
// <o37.03> Enabled
// </e>
// <e40.03> FN_TRUNK_REM2_ON
// <o41.03> Enabled
// </e>
// <e44.03> FN_TRUNK_REM3_ON
// <o45.03> Enabled
// </e>
// <e36.04> FN_AUX1_REM1_ON
// <o37.04> Enabled
// </e>
// <e40.04> FN_AUX1_REM2_ON
// <o41.04> Enabled
// </e>
// <e44.04> FN_AUX1_REM3_ON
// <o45.04> Enabled
// </e>
// <e36.02> FN_AUX2_REM1_ON
// <o37.02> Enabled
// </e>
// <e40.02> FN_AUX2_REM2_ON
// <o41.02> Enabled
// </e>
// <e44.02> FN_AUX2_REM3_ON
// <o45.02> Enabled
// </e>
// <e36.01> FN_AUX3_REM1_ON
// <o37.01> Enabled
// </e>
// <e40.01> FN_AUX3_REM2_ON
// <o41.01> Enabled
// </e>
// <e44.01> FN_AUX3_REM3_ON
// <o45.01> Enabled
// </e>
// <e36.00> FN_AUX4_REM1_ON
// <o37.00> Enabled
// </e>
// <e40.00> FN_AUX4_REM2_ON
// <o41.00> Enabled
// </e>
// <e44.00> FN_AUX4_REM3_ON
// <o45.00> Enabled
// </e>
// <e36.07> FN_PANIC_REM1_ON
// <o37.07> Enabled
// </e>
// <e4.07> FN_PANIC_REM1_OFF
// <o5.07> Enabled
// </e>
// <e40.07> FN_PANIC_REM2_ON
// <o41.07> Enabled
// </e>
// <e8.07> FN_PANIC_REM2_OFF
// <o9.07> Enabled
// </e>
// <e44.07> FN_PANIC_REM3_ON
// <o45.07> Enabled
// </e>
// <e12.07> FN_PANIC_REM3_OFF
// <o13.07> Enabled
// </e>
// </h>
//-------------------------------------------------------------------------//
//      <h> Sensing
//-------------------------------------------------------------------------//
// <e60.03> SET_DRIVER_OPEN
// <o61.03> Enabled
// </e>
// <e28.03> SET_DRIVER_CLOSE
// <o29.03> Enabled
// </e>
// <e62.07> SET_OTHER_OPEN
// <o63.07> Enabled
// </e>
// <e30.07> SET_OTHER_CLOSE
// <o31.07> Enabled
// </e>
// <e60.05> SET_TRUNK_OPEN
// <o61.05> Enabled
// </e>
// <e28.05> SET_TRUNK_CLOSE
// <o29.05> Enabled
// </e>
// <e60.06> SET_HOOD_OPEN
// <o61.06> Enabled
// </e>
// <e28.06> SET_HOOD_CLOSE
// <o29.06> Enabled
// </e>
// <e62.06> SET_BRAKE_ON
// <o63.06> Enabled
// </e>
// <e30.06> SET_BRAKE_OFF
// <o31.06> Enabled
// </e>
// <e50.00> SET_HANDBRAKE_ON
// <o51.00> Enabled
// </e>
// <e18.00> SET_HANDBRAKE_OFF
// <o19.00> Enabled
// </e>
// <e62.02> SET_IGN_ON
// <o63.02> Enabled
// </e>
// <e30.02> SET_IGN_OFF
// <o31.02> Enabled
// </e>
// <e62.03> SET_KEY_IN
// <o63.03> Enabled
// </e>
// <e30.03> SET_KEY_OUT
// <o31.03> Enabled
// </e>
// <e60.04> SET_RPM
// <o61.04> Enabled
// </e>
// <e2.03> REQ_RPM
// <o3.03> Enabled
// </e>
// <e54.02> SET_KEYLESS_DISARM
// <o55.02> Enabled
// </e>
// <e54.01> SET_KEYLESS_ARM
// <o55.01> Enabled
// </e>
// </h>
// </h>
//-------------------------------------------------------------------------//
// <h> CORE
//-------------------------------------------------------------------------//
// <h> ON Codes             ->Must be CHECKED
//-------------------------------------------------------------------------//
// <e0.04> REQ_DTC_CODE
// <o1.04> Enabled
// </e>
// <e12.02> SET_DTC_CODE
// <o13.02> Enabled
// </e>
// <e0.03> REQ_DTC
// <o1.03> Enabled
// </e>
// <e12.03> SET_DTC
// <o13.03> Enabled
// </e>
// <e0.01> REQ_CLR_DTC
// <o1.01> Enabled
// </e>
// <e12.01> SET_DTC_CLR
// <o13.01> Enabled
// </e>
// <e6.07> AL_NEW_DTC_CODE
// <o7.07> Enabled
// </e>
// <e0.00> REQ_FUEL_LEVEL
// <o1.00> Enabled
// </e>
// <e12.00> SET_FUEL_LEVEL
// <o13.00> Enabled
// </e>
// <e2.05> REQ_ODO
// <o3.05> Enabled
// </e>
// <e14.05> SET_ODO
// <o15.05> Enabled
// </e>
// <e2.04> REQ_BAT_VOLT
// <o3.04> Enabled
// </e>
// <e14.04> SET_BAT_VOLT
// <o15.04> Enabled
// </e>
// <e2.02> REQ_SPEED
// <o3.02> Enabled
// </e>
// <e62.05> SET_SPEED
// <o63.05> Enabled
// </e>
// <e2.01> REQ_TPMS
// <o3.01> Enabled
// </e>
// <e14.01> SET_TPMS
// <o15.01> Enabled
// </e>
// <e0.02> REQ_ENG_TEMP
// <o1.02> Enabled
// </e>
// <e60.02> SET_ENG_TEMP
// <o61.02> Enabled
// </e>
// <e2.00> REQ_INT_TEMP
// <o3.00> Enabled
// </e>
// <e60.07> SET_INT_TEMP
// <o61.07> Enabled
// </e>
// <e4.00> REQ_VIN
// <o5.00> Enabled
// </e>
// <e4.06> REQ_VIN_1
// <o5.06> Enabled
// </e>
// <e8.06> SET_VIN_1
// <o9.06> Enabled
// </e>
// <e4.05> REQ_VIN_2
// <o5.05> Enabled
// </e>
// <e8.05> SET_VIN_2
// <o9.05> Enabled
// </e>
// <e4.04> REQ_VIN_3
// <o5.04> Enabled
// </e>
// <e8.04> SET_VIN_3
// <o9.04> Enabled
// </e>
// <e4.03> REQ_VIN_4
// <o5.03> Enabled
// </e>
// <e8.03> SET_VIN_4
// <o9.03> Enabled
// </e>
// <e4.02> REQ_VIN_5
// <o5.02> Enabled
// </e>
// <e8.02> SET_VIN_5
// <o9.02> Enabled
// </e>
// <e4.01> REQ_VIN_6
// <o5.01> Enabled
// </e>
// <e8.01> SET_VIN_6
// <o9.01> Enabled
// </e>
// <e18.06> REQ_PROTOCOL_VER
// <o19.06> Enabled
// </e>
// <e18.05> SET_PROTOCOL_VER
// <o19.05> Enabled
// </e>
// <e20.07> REQ_MODULE_INFO
// <o21.07> Enabled
// </e>
// <e20.06> SET_PLATFORM_ID
// <o21.06> Enabled
// </e>
// <e20.05> SET_HW_VERSION
// <o21.05> Enabled
// </e>
// <e20.04> SET_FW_ID
// <o21.04> Enabled
// </e>
// <e20.02> SET_FW_VERSION
// <o21.02> Enabled
// </e>
// <e0.06> REQ_INPUT_STATUS
// <o1.06> Enabled
// </e>
// <e62.01> SET_STATUS
// <o63.01> Enabled
// </e>
// <e30.01> REQ_2WAY_STATUS
// <o31.01> Enabled
// </e>
// <e52.07> SET_2WAY_STATUS
// <o53.07> Enabled
// </e>
// <e20.01> REQ_EXTENDED_STATUS_SUPPORT
// <o21.01> Enabled
// </e>
// <e20.00> SET_EXTENDED_STATUS_SUPPORT
// <o21.00> Enabled
// </e>
// <e30.00> REQ_EXTENDED_STATUS
// <o31.00> Enabled
// </e>
// <e20.02> SET_EXTENDED_STATUS
// <o21.02> Enabled
// </e>
// <e32.07> REQ_EXTENDED_STATUS_SUPPORT_SYNC
// <o33.07> Enabled
// </e>
// <e22.06> REQ_RUNTIME
// <o23.06> Enabled
// </e>
// <e22.05> SET_RUNTIME
// <o23.05> Enabled
// </e>
// <e18.04> AL_QUERY
// <o19.04> Enabled
// </e>
// <e22.04> AL_ALERT_STATUS
// <o23.04> Enabled
// </e>
// <e22.03> AL_FAILED_TO_UNLOCK
// <o23.03> Enabled
// </e>
// <e22.02> AL_FAILED_TO_START
// <o23.02> Enabled
// </e>
// <e22.01> AL_FAILED_TO_STOP
// <o23.01> Enabled
// </e>
// <e22.00> AL_FAILED_TO_LOCK
// <o23.00> Enabled
// </e>
// <e28.07> AL_ALERT_CLEAR
// <o29.07> Enabled
// </e>
// <e46.06> AL_ALARM_ALERT
// <o47.06> Enabled
// </e>
// <e52.05> SET_EIPS_ALERT
// <o53.05> Enabled
// </e>
// <e52.04> SET_EIPS_SHUTDOWN
// <o53.04> Enabled
// </e>
// <e28.04> SET_EIPS_SHUTDOWN_FAIL
// <o29.04> Enabled
// </e>
// <e38.06> FN_START_REM
// <o39.06> Enabled
// </e>
// <e38.04> FN_START_ONLY
// <o39.04> Enabled
// </e>
// <e8.00> FN_EXTEND_RUNTIME
// <o9.00> Enabled
// </e>
// <e6.06> FN_STOP_REM
// <o7.06> Enabled
// </e>
// <e46.04> FN_OBD_CAN_ENABLE
// <o47.04> Enabled
// </e>
// <e46.03> FN_OBD_CAN_DISABLE
// <o47.03> Enabled
// </e>
// <e24.07> REQ_AUTH_CHALLENGE
// <o25.07> Enabled
// </e>
// <e56.07> SET_AUTH_RESPONSE
// <o57.07> Enabled
// </e>
// </h>
//-------------------------------------------------------------------------//
// <h> OFF Codes            ->Must be UNCHECKED
//-------------------------------------------------------------------------//
// <e2.06> FN_READ_EEPROM
// <o3.06> Enabled
// </e>
// <e60.01> FN_WRITE_EEPROM
// <o61.01> Enabled
// </e>
// <e48.07> FN_DOME_ON
// <o49.07> Enabled
// </e>
// <e16.07> FN_DOME_OFF
// <o17.07> Enabled
// </e>
// <e48.06> FN_ACC_ON
// <o49.06> Enabled
// </e>
// <e16.06> FN_ACC_OFF
// <o17.06> Enabled
// </e>
// <e48.04> FN_CAR_IGN_ON
// <o49.04> Enabled
// </e>
// <e16.04> FN_CAR_IGN_OFF
// <o17.04> Enabled
// </e>
// <e20.03> CHAR_T
// <o21.03> Enabled
// </e>
// <e22.07> CHAR_X
// <o23.07> Enabled
// </e>
// <e60.00> SET_PRK_NTRL_ACTIVE
// <o61.00> Enabled
// </e>
// <e28.00> SET_PRK_NTRL_NOT_ACTIVE
// <o29.00> Enabled
// </e>
// <e62.04> SET_GLOWPLUG_ON
// <o63.04> Enabled
// </e>
// <e30.04> SET_GLOWPLUG_OFF
// <o31.04> Enabled
// </e>
// <e38.07> FN_CHIRP
// <o39.07> Enabled
// </e>
// <e42.07> FN_SHORT_TRUNK
// <o43.07> Enabled
// </e>
// <e42.06> FN_LONG_START
// <o43.06> Enabled
// </e>
// <e46.07> FN_CAR_FINDER
// <o47.07> Enabled
// </e>
// <e52.06> RESERVED_D1
// <o53.06> Enabled
// </e>
// <e52.03> SET_KEYLESS_UNLOCK
// <o53.03> Enabled
// </e>
// <e52.02> SET_KEYLESS_LOCK
// <o53.02> Enabled
// </e>
// <e52.01> SET_KEYLESS_AUX3
// <o53.01> Enabled
// </e>
// <e52.00> SET_KEYLESS_PANIC
// <o53.00> Enabled
// </e>
// <e54.07> SET_KEYLESS_AUX1
// <o55.07> Enabled
// </e>
// <e54.06> SET_KEYLESS_AUX2
// <o55.06> Enabled
// </e>
// <e54.05> SET_KEYLESS_START
// <o55.05> Enabled
// </e>
// <e54.04> SET_KEYLESS_STOP
// <o55.04> Enabled
// </e>
// <e54.03> SET_KEYLESS_AUX4
// <o55.03> Enabled
// </e>
// <e54.00> SET_KEYLESS_TRUNK
// <o55.00> Enabled
// </e>
// </h>
// </h>
//-------------------------------------------------------------------------//
// <h> UNDEFINED CODES   ->Must be UNCHECKED
//-------------------------------------------------------------------------//
// <e0.07> FN_00
// <o1.07> Enabled
// </e>
// <e0.05> FN_02
// <o1.05> Enabled
// </e>
// <e6.05> FN_1A
// <o7.05> Enabled
// </e>
// <e6.04> FN_1B
// <o7.04> Enabled
// </e>
// <e6.03> FN_1C
// <o7.03> Enabled
// </e>
// <e6.02> FN_1D
// <o7.02> Enabled
// </e>
// <e6.01> FN_1E
// <o7.01> Enabled
// </e>
// <e6.00> FN_1F
// <o7.00> Enabled
// </e>
// <e10.07> FN_28
// <o11.07> Enabled
// </e>
// <e10.06> FN_29
// <o11.06> Enabled
// </e>
// <e10.05> FN_2A
// <o11.05> Enabled
// </e>
// <e10.04> FN_2B
// <o11.04> Enabled
// </e>
// <e10.03> FN_2C
// <o11.03> Enabled
// </e>
// <e10.02> FN_2D
// <o11.02> Enabled
// </e>
// <e10.01> FN_2E
// <o11.01> Enabled
// </e>
// <e10.00> FN_2F
// <o11.00> Enabled
// </e>
// <e12.06> FN_31
// <o13.06> Enabled
// </e>
// <e12.05> FN_32
// <o13.05> Enabled
// </e>
// <e12.04> FN_33
// <o13.04> Enabled
// </e>
// <e14.07> FN_38
// <o15.07> Enabled
// </e>
// <e14.06> FN_39
// <o15.06> Enabled
// </e>
// <e14.03> FN_3C
// <o15.03> Enabled
// </e>
// <e14.02> FN_3D
// <o15.02> Enabled
// </e>
// <e14.00> FN_3F
// <o15.00> Enabled
// </e>
// <e16.02> FN_45
// <o17.02> Enabled
// </e>
// <e16.01> FN_46
// <o17.01> Enabled
// </e>
// <e16.00> FN_47
// <o17.00> Enabled
// </e>
// <e18.03> FN_4C
// <o19.03> Enabled
// </e>
// <e18.02> FN_4D
// <o19.02> Enabled
// </e>
// <e18.01> FN_4E
// <o19.01> Enabled
// </e>
// <e24.06> FN_61
// <o25.06> Enabled
// </e>
// <e24.05> FN_62
// <o25.05> Enabled
// </e>
// <e24.04> FN_63
// <o25.04> Enabled
// </e>
// <e24.03> FN_64
// <o25.03> Enabled
// </e>
// <e24.02> FN_65
// <o25.02> Enabled
// </e>
// <e24.01> FN_66
// <o25.01> Enabled
// </e>
// <e24.00> FN_67
// <o25.00> Enabled
// </e>
// <e26.07> FN_68
// <o27.07> Enabled
// </e>
// <e26.06> FN_69
// <o27.06> Enabled
// </e>
// <e26.05> FN_6A
// <o27.05> Enabled
// </e>
// <e26.04> FN_6B
// <o27.04> Enabled
// </e>
// <e26.03> FN_6C
// <o27.03> Enabled
// </e>
// <e26.02> FN_6D
// <o27.02> Enabled
// </e>
// <e26.01> FN_6E
// <o27.01> Enabled
// </e>
// <e26.00> FN_6F
// <o27.00> Enabled
// </e>
// <e28.01> FN_76
// <o29.01> Enabled
// </e>
// <e30.05> FN_7A
// <o31.05> Enabled
// </e>
// <e32.04> FN_83
// <o33.04> Enabled
// </e>
// <e32.03> FN_84
// <o33.03> Enabled
// </e>
// <e32.02> FN_85
// <o33.02> Enabled
// </e>
// <e32.01> FN_86
// <o33.01> Enabled
// </e>
// <e32.00> FN_87
// <o33.00> Enabled
// </e>
// <e34.06> FN_89
// <o35.06> Enabled
// </e>
// <e34.05> FN_8A
// <o35.05> Enabled
// </e>
// <e34.04> FN_8B
// <o35.04> Enabled
// </e>
// <e34.03> FN_8C
// <o35.03> Enabled
// </e>
// <e34.02> FN_8D
// <o35.02> Enabled
// </e>
// <e34.01> FN_8E
// <o35.01> Enabled
// </e>
// <e34.00> FN_8F
// <o35.00> Enabled
// </e>
// <e38.03> FN_9C
// <o39.03> Enabled
// </e>
// <e38.02> FN_9D
// <o39.02> Enabled
// </e>
// <e38.01> FN_9E
// <o39.01> Enabled
// </e>
// <e38.00> FN_9F
// <o39.00> Enabled
// </e>
// <e42.04> FN_AB
// <o43.04> Enabled
// </e>
// <e42.03> FN_AC
// <o43.03> Enabled
// </e>
// <e42.02> FN_AD
// <o43.02> Enabled
// </e>
// <e42.01> FN_AE
// <o43.01> Enabled
// </e>
// <e42.00> FN_AF
// <o43.00> Enabled
// </e>
// <e46.02> FN_BD
// <o47.02> Enabled
// </e>
// <e46.01> FN_BE
// <o47.01> Enabled
// </e>
// <e46.00> FN_BF
// <o47.00> Enabled
// </e>
// <e48.02> FN_C5
// <o49.02> Enabled
// </e>
// <e48.01> FN_C6
// <o49.01> Enabled
// </e>
// <e48.00> FN_C7
// <o49.00> Enabled
// </e>
// <e50.06> FN_C9
// <o51.06> Enabled
// </e>
// <e50.05> FN_CA
// <o51.05> Enabled
// </e>
// <e50.04> FN_CB
// <o51.04> Enabled
// </e>
// <e50.03> FN_CC
// <o51.03> Enabled
// </e>
// <e50.02> FN_CD
// <o51.02> Enabled
// </e>
// <e50.01> FN_CE
// <o51.01> Enabled
// </e>
// <e56.06> FN_E1
// <o57.06> Enabled
// </e>
// <e56.05> FN_E2
// <o57.05> Enabled
// </e>
// <e56.04> FN_E3
// <o57.04> Enabled
// </e>
// <e56.03> FN_E4
// <o57.03> Enabled
// </e>
// <e56.02> FN_E5
// <o57.02> Enabled
// </e>
// <e56.01> FN_E6
// <o57.01> Enabled
// </e>
// <e56.00> FN_E7
// <o57.00> Enabled
// </e>
// <e58.07> FN_E8
// <o59.07> Enabled
// </e>
// <e58.06> FN_E9
// <o59.06> Enabled
// </e>
// <e58.05> FN_EA
// <o59.05> Enabled
// </e>
// <e58.04> FN_EB
// <o59.04> Enabled
// </e>
// <e58.03> FN_EC
// <o59.03> Enabled
// </e>
// <e58.02> FN_ED
// <o59.02> Enabled
// </e>
// <e58.01> FN_EE
// <o59.01> Enabled
// </e>
// <e58.00> FN_EF
// <o59.00> Enabled
// </e>
// <e62.00> FN_FF
// <o63.00> Enabled
// </e>
// </h>
// </h>
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//

#define D2D_MASK_0 0x5F
#define D2D_ENA_0  0xFF
#define D2D_MASK_1 0x37
#define D2D_ENA_1  0xFF
#define D2D_MASK_2 0x7F
#define D2D_ENA_2  0xFF
#define D2D_MASK_3 0xC0
#define D2D_ENA_3  0xFF
#define D2D_MASK_4 0x7F
#define D2D_ENA_4  0xFF
#define D2D_MASK_5 0x00
#define D2D_ENA_5  0xFF
#define D2D_MASK_6 0x0F
#define D2D_ENA_6  0xFF
#define D2D_MASK_7 0x32
#define D2D_ENA_7  0xFF
#define D2D_MASK_8 0x00
#define D2D_ENA_8  0xFF
#define D2D_MASK_9 0x70
#define D2D_ENA_9  0xFF
#define D2D_MASK_10 0xF7
#define D2D_ENA_10  0xFF
#define D2D_MASK_11 0x7F
#define D2D_ENA_11  0xFF
#define D2D_MASK_12 0x80
#define D2D_ENA_12  0xFF
#define D2D_MASK_13 0x00
#define D2D_ENA_13  0xFF
#define D2D_MASK_14 0x94
#define D2D_ENA_14  0xFF
#define D2D_MASK_15 0x03
#define D2D_ENA_15  0xFF
#define D2D_MASK_16 0x80
#define D2D_ENA_16  0xFF
#define D2D_MASK_17 0x00
#define D2D_ENA_17  0xFF
#define D2D_MASK_18 0x00
#define D2D_ENA_18  0xFF
#define D2D_MASK_19 0x50
#define D2D_ENA_19  0xFF
#define D2D_MASK_20 0x00
#define D2D_ENA_20  0xFF
#define D2D_MASK_21 0x00
#define D2D_ENA_21  0xFF
#define D2D_MASK_22 0x00
#define D2D_ENA_22  0xFF
#define D2D_MASK_23 0x58
#define D2D_ENA_23  0xFF
#define D2D_MASK_24 0x00
#define D2D_ENA_24  0xFF
#define D2D_MASK_25 0x00
#define D2D_ENA_25  0xFF
#define D2D_MASK_26 0xB0
#define D2D_ENA_26  0xFF
#define D2D_MASK_27 0x00
#define D2D_ENA_27  0xFF
#define D2D_MASK_28 0x80
#define D2D_ENA_28  0xFF
#define D2D_MASK_29 0x00
#define D2D_ENA_29  0xFF
#define D2D_MASK_30 0x84
#define D2D_ENA_30  0xFF
#define D2D_MASK_31 0x22
#define D2D_ENA_31  0xFF
//-------------------------------------------------------------------------//


#endif

