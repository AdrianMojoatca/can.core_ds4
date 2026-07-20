/*<#======================================================================#>*/
/*           OPTION DEFINES PENTRU CONFIG_PROG.H (DS4 CONTRACT)           */
/*<#======================================================================#>*/

#ifndef __CORE_CONTRACT_DS4_PROG_OPTION_DEFS_H__
#define __CORE_CONTRACT_DS4_PROG_OPTION_DEFS_H__

#include <stdint.h>

#define PROG_TRUNK_AUX_OPTION_TRUNK                 ((uint32_t)0U)
#define PROG_TRUNK_AUX_OPTION_RIGHT_SLIDING_DOOR    ((uint32_t)1U)
#define PROG_TRUNK_AUX_OPTION_LEFT_SLIDING_DOOR     ((uint32_t)2U)
#define PROG_TRUNK_AUX_OPTION_TAIL_GLASS_OPENING    ((uint32_t)3U)
#define PROG_TRUNK_AUX_OPTION_GAS_CAP_OPENING       ((uint32_t)4U)
#define PROG_TRUNK_AUX_OPTION_HEAD_LIGHTS           ((uint32_t)5U)
#define PROG_TRUNK_AUX_OPTION_SUNROOF_CLOSURE       ((uint32_t)6U)
#define PROG_TRUNK_AUX_OPTION_WINDOWS_CLOSURE       ((uint32_t)7U)
#define PROG_TRUNK_AUX_OPTION_WINDOWS_VENT          ((uint32_t)8U)

#define PROG_OPTION_DISABLED                         ((uint32_t)1U)

#define PROG_UNLOCK_DRIVER_PRIORITY_DRIVER           ((uint32_t)1U)
#define PROG_UNLOCK_DRIVER_PRIORITY_ALL              ((uint32_t)2U)

#define PROG_RELOCK_DISABLED                         ((uint32_t)1U)
#define PROG_RELOCK_ENABLED                          ((uint32_t)2U)

#define PROG_AUTO_LOCK_DISABLED                      ((uint32_t)1U)
#define PROG_AUTO_LOCK_WITH_SPEED                    ((uint32_t)2U)

#define PROG_AUTO_UNLOCK_DISABLED                    ((uint32_t)1U)
#define PROG_AUTO_UNLOCK_WITH_KEY_OUT                ((uint32_t)2U)

#define PROG_RF_OUTPUT_DISABLED                      ((uint32_t)1U)
#define PROG_RF_OUTPUT_RFTD                          ((uint32_t)2U)
#define PROG_RF_OUTPUT_SMARTSTART                    ((uint32_t)3U)

#define PROG_HORN_CONFIRMATION_DISABLED              ((uint32_t)1U)
#define PROG_HORN_CONFIRMATION_20MS                  ((uint32_t)2U)
#define PROG_HORN_CONFIRMATION_30MS                  ((uint32_t)3U)
#define PROG_HORN_CONFIRMATION_40MS                  ((uint32_t)4U)
#define PROG_HORN_CONFIRMATION_50MS                  ((uint32_t)5U)

#define PROG_COMFORT_CLOSURE_DISABLED                ((uint32_t)1U)
#define PROG_COMFORT_CLOSURE_ENABLED                 ((uint32_t)2U)

#define PROG_OEM_SECURITY_WITHOUT_ALARM              ((uint32_t)1U)
#define PROG_OEM_SECURITY_WITH_ALARM                 ((uint32_t)2U)

#define PROG_KEY_IGN_ARM_DEFEAT_DISABLED             ((uint32_t)1U)
#define PROG_KEY_IGN_ARM_DEFEAT_ENABLED              ((uint32_t)2U)

#define PROG_LIGHT_CONTROL_PARKING_LIGHTS            ((uint32_t)1U)
#define PROG_LIGHT_CONTROL_TURN_SIGNALS              ((uint32_t)2U)

#define PROG_EXT_LIGHT_SUPERVISION_DISABLED          ((uint32_t)1U)
#define PROG_EXT_LIGHT_SUPERVISION_ENABLED           ((uint32_t)2U)

#define PROG_HEATED_SEATS_DISABLED                   ((uint32_t)1U)
#define PROG_HEATED_SEATS_ENABLED                    ((uint32_t)2U)

#define PROG_DEFROSTER_DISABLED                      ((uint32_t)1U)
#define PROG_DEFROSTER_ENABLED                       ((uint32_t)2U)

#define PROG_SEATS_TEMP_CONTROL_0C                   ((uint32_t)1U)
#define PROG_SEATS_TEMP_CONTROL_MINUS_5C             ((uint32_t)2U)
#define PROG_SEATS_TEMP_CONTROL_PLUS_5C              ((uint32_t)3U)
#define PROG_SEATS_TEMP_CONTROL_ALWAYS_ON            ((uint32_t)4U)

#define PROG_HEATED_SEATS_TEMP_LEVEL_HIGH            ((uint32_t)1U)
#define PROG_HEATED_SEATS_TEMP_LEVEL_MED             ((uint32_t)2U)
#define PROG_HEATED_SEATS_TEMP_LEVEL_LOW             ((uint32_t)3U)

#define PROG_CTL_DOORLOCK_DISABLED                   ((uint32_t)1U)
#define PROG_CTL_DOORLOCK_IGNITION                   ((uint32_t)2U)
#define PROG_CTL_DOORLOCK_BRAKE                      ((uint32_t)3U)
#define PROG_CTL_DOORLOCK_SPEED                      ((uint32_t)4U)

#define PROG_SECURE_LOCK_DISABLED                    ((uint32_t)1U)
#define PROG_SECURE_LOCK_SAFELOCK                    ((uint32_t)2U)
#define PROG_SECURE_LOCK_ENABLED                     ((uint32_t)3U)

#define PROG_TAKEOVER_ENGINE_SHUTDOWN_DOOR_OPEN      ((uint32_t)1U)
#define PROG_TAKEOVER_ENABLED_BY_OEM_REMOTE          ((uint32_t)2U)
#define PROG_TAKEOVER_ENABLED_BY_OEM_OR_AFTERMARKET  ((uint32_t)3U)
#define PROG_TAKEOVER_SKIP_DOOR_SHUTDOWN             ((uint32_t)4U)

#define PROG_DIGITAL_AUX_OFF                         ((uint32_t)1U)
#define PROG_DIGITAL_AUX_WINDOWS_ROLL_UP             ((uint32_t)2U)
#define PROG_DIGITAL_AUX_FRONT_WINDOWS_ROLL_DOWN     ((uint32_t)3U)
#define PROG_DIGITAL_AUX_REAR_WINDOWS_ROLL_DOWN      ((uint32_t)4U)
#define PROG_DIGITAL_AUX_GAS_CAP                     ((uint32_t)5U)

#endif
