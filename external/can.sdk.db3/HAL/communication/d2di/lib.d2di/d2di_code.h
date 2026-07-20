/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

#ifndef __D2DI_CODE_H__
#define __D2DI_CODE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
/* List of supported codes. */

// initialization codes
#define D2DI_CODE_RESET                     0x00
#define D2DI_CODE_REQ_PROTO_VERSION         0x06
#define D2DI_CODE_ANS_PROTO_VERSION         0x07
#define D2DI_CODE_REQ_MANUFACTURER_ID       0x08
#define D2DI_CODE_ANS_MANUFACTURER_ID       0x09
#define D2DI_CODE_FULL_STATUS_REQUEST       0x0F
        
// alarm control codes      
#define D2DI_CODE_LOCK                      0x10
#define D2DI_CODE_UNLOCK                    0x11
#define D2DI_CODE_ARM                       0x12
#define D2DI_CODE_DISARM                    0x13
#define D2DI_CODE_GWR_ON                    0x14
#define D2DI_CODE_GWR_OFF                   0x15
#define D2DI_CODE_TRUNK                     0x16
#define D2DI_CODE_PANIC_ON                  0x17
#define D2DI_CODE_PANIC_OFF                 0x18
#define D2DI_CODE_AUX1                      0x1B
#define D2DI_CODE_AUX2                      0x1C
#define D2DI_CODE_AUX3                      0x1D
#define D2DI_CODE_AUX4                      0x1E
#define D2DI_CODE_UNLOCK_DRIVER             0x1F
        
// power output codes       
#define D2DI_CODE_POWER_ACC_RELAY_ON        0x20  // FN_ACC_ON
#define D2DI_CODE_POWER_ACC_RELAY_OFF       0x21  // FN_ACC_OFF
#define D2DI_CODE_POWER_IGN_RELAY_ON        0x22  // FN_IGN_ON
#define D2DI_CODE_POWER_IGN_RELAY_OFF       0x23  // FN_IGN_OFF
#define D2DI_CODE_POWER_START_RELAY_ON      0x24  // FN_START_ON
#define D2DI_CODE_POWER_START_RELAY_OFF     0x25  // FN_START_OFF
#define D2DI_CODE_PARKING_ON                0x26
#define D2DI_CODE_PARKING_OFF               0x27
#define D2DI_CODE_HORN_ON                   0x28  // FN_CHIRP
#define D2DI_CODE_HORN_OFF                  0x29  // FN_CHIRP
#define D2DI_CODE_ACC_ON                    0x2A
#define D2DI_CODE_ACC_OFF                   0x2B
#define D2DI_CODE_IGN_ON                    0x2C  // SET_IGN_ON
#define D2DI_CODE_IGN_OFF                   0x2D  // SET_IGN_OFF
#define D2DI_CODE_START_ON                  0x2E
#define D2DI_CODE_START_OFF                 0x2F


// external remote control codes
#define D2DI_CODE_OEM_LOCK_ARM              0x30  // SET_KEYLESS_ARM
#define D2DI_CODE_OEM_DISARM_UNLOCK         0x31  // SET_KEYLESS_DISARM
#define D2DI_CODE_OEM_START                 0x32  // SET_KEYLESS_START
#define D2DI_CODE_OEM_STOP                  0x33  // SET_KEYLESS_STOP
#define D2DI_CODE_OEM_TRUNK                 0x34  // SET_KEYLESS_TRUNK
#define D2DI_CODE_OEM_PANIC_ON              0x35  // SET_KEYLESS_PANIC
#define D2DI_CODE_OEM_PANIC_OFF             0x36  // SET_KEYLESS_PANIC
#define D2DI_CODE_OEM_AUX1                  0x39  // SET_KEYLESS_AUX1
#define D2DI_CODE_OEM_AUX2                  0x3A  // SET_KEYLESS_AUX2
#define D2DI_CODE_OEM_AUX3                  0x3B  // SET_KEYLESS_AUX3
#define D2DI_CODE_OEM_AUX4                  0x3C  // SET_KEYLESS_AUX4
#define D2DI_CODE_DRIVER1                   0x40
#define D2DI_CODE_DRIVER2                   0x41
#define D2DI_CODE_DRIVER3                   0x42
#define D2DI_CODE_DRIVER4                   0x43

// engine and vehicle information codes
#define D2DI_CODE_TACH                      0x51
#define D2DI_CODE_NEUTRAL_SWITCH            0x56
#define D2DI_CODE_ALARM_TRIGGER             0x70
#define D2DI_CODE_BRAKE_INFO                0x71
#define D2DI_CODE_OEM_ALARM_STATUS          0x80

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

#endif
