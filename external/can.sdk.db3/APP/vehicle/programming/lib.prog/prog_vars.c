/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: prog_vars.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "prog_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
#if	defined PLATFORM_CM800 || defined PLATFORM_933		//def PLATFORM_CM800
   // Task variables not required
#else

    Tsk tsk_prog ;
    Pip pip_prog ;

    Timeout prog_ign_timeout ;

#endif

Prog_Vars prog_vars ;

Feature_Entry feature_table_default[FEAT_COUNT] ;

UInt8 FEATURE_NUM = 0 ;

Feature_Entry feature_table[FEAT_COUNT] ;

Prog_Error    prog_err ;

static UInt8 prog_aux_option_count (void)
{
  UInt8 count = 0 ;

  count += (UInt8)PROG_CFG_OPTION_TRUNK_ENABLE ;
  count += (UInt8)PROG_CFG_OPTION_RIGHT_SLIDE_ENABLE ;
  count += (UInt8)PROG_CFG_OPTION_LEFT_SLIDE_ENABLE ;
  count += (UInt8)PROG_CFG_OPTION_TAIL_GLASS_ENABLE ;
  count += (UInt8)PROG_CFG_OPTION_GAS_CAP_ENABLE ;
  count += (UInt8)PROG_CFG_OPTION_HEAD_LIGHTS_ENABLE ;
  count += (UInt8)PROG_CFG_OPTION_SUNROOF_CLOSURE_ENABLE ;
  count += (UInt8)PROG_CFG_OPTION_WINDOWS_CLOSURE_ENABLE ;
  count += (UInt8)PROG_CFG_OPTION_WINDOWS_VENT_ENABLE ;

  return count ;
}

static UInt8 prog_custom1_option_count (void)
{
  return (UInt8)(1
      + (UInt8)PROG_CFG_CUSTOM1_OPT2_ENABLE
      + (UInt8)PROG_CFG_CUSTOM1_OPT3_ENABLE
      + (UInt8)PROG_CFG_CUSTOM1_OPT4_ENABLE
      + (UInt8)PROG_CFG_CUSTOM1_OPT5_ENABLE) ;
}

static UInt8 prog_custom2_option_count (void)
{
  return (UInt8)(1
      + (UInt8)PROG_CFG_CUSTOM2_OPT2_ENABLE
      + (UInt8)PROG_CFG_CUSTOM2_OPT3_ENABLE
      + (UInt8)PROG_CFG_CUSTOM2_OPT4_ENABLE
      + (UInt8)PROG_CFG_CUSTOM2_OPT5_ENABLE) ;
}

static UInt8 prog_custom3_option_count (void)
{
  return (UInt8)(1
      + (UInt8)PROG_CFG_CUSTOM3_OPT2_ENABLE
      + (UInt8)PROG_CFG_CUSTOM3_OPT3_ENABLE
      + (UInt8)PROG_CFG_CUSTOM3_OPT4_ENABLE
      + (UInt8)PROG_CFG_CUSTOM3_OPT5_ENABLE) ;
}

static UInt8 prog_custom4_option_count (void)
{
  return (UInt8)(1
      + (UInt8)PROG_CFG_CUSTOM4_OPT2_ENABLE
      + (UInt8)PROG_CFG_CUSTOM4_OPT3_ENABLE
      + (UInt8)PROG_CFG_CUSTOM4_OPT4_ENABLE
      + (UInt8)PROG_CFG_CUSTOM4_OPT5_ENABLE) ;
}

static UInt8 prog_heated_seats_level_count (void)
{
  return (UInt8)((UInt8)PROG_CFG_HEATED_SEATS_LEVEL_HIGH_ENABLE
      + (UInt8)PROG_CFG_HEATED_SEATS_LEVEL_MED_ENABLE
      + (UInt8)PROG_CFG_HEATED_SEATS_LEVEL_LOW_ENABLE) ;
}

static UInt8 prog_ctl_doorlock_option_count (void)
{
  return (UInt8)(1
      + (UInt8)PROG_CFG_CTL_DOORLOCK_IGNITION_ENABLE
      + (UInt8)PROG_CFG_CTL_DOORLOCK_BRAKE_ENABLE
      + (UInt8)PROG_CFG_CTL_DOORLOCK_SPEED_ENABLE) ;
}

static UInt8 prog_secure_lock_option_count (void)
{
  return (UInt8)(2 + (UInt8)PROG_CFG_SECURE_LOCK_SMART_ENABLE) ;
}

void prog_build_feature_table_default (void)
{
  UInt8 feature_num = 0 ;

  #define APPEND_FEATURE(enable, feat, option_count, default_value) \
    do { \
      if ((enable) == 1) { \
        feature_table_default[feature_num].feature = (feat) ; \
        feature_table_default[feature_num].option_number = (option_count) ; \
        feature_table_default[feature_num].option_current = (default_value) ; \
        feature_num++ ; \
      } \
    } while (0)

  APPEND_FEATURE(PROG_CFG_FEAT_RF_OUTPUT_ENABLE, FEAT_RF_OUTPUT, 3, PROG_CFG_FEAT_RF_OUTPUT_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_UNLOCK_DRIVER_ENABLE, FEAT_UNLOCK_DRIVER, 2, PROG_CFG_FEAT_UNLOCK_DRIVER_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_RELOCK_ENABLE, FEAT_RELOCK, 2, PROG_CFG_FEAT_RELOCK_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_AUTO_LOCK_ENABLE, FEAT_AUTO_LOCK, 2, PROG_CFG_FEAT_AUTO_LOCK_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_AUTO_UNLOCK_ENABLE, FEAT_AUTO_UNLOCK, 2, PROG_CFG_FEAT_AUTO_UNLOCK_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_TRUNK_ENABLE, FEAT_TRUNK, prog_aux_option_count(), DEFAULT_TRUNK_OPTION) ;
  APPEND_FEATURE(PROG_CFG_FEAT_AUX1_ENABLE, FEAT_AUX1, prog_aux_option_count(), DEFAULT_AUX1_OPTION) ;
  APPEND_FEATURE(PROG_CFG_FEAT_AUX2_ENABLE, FEAT_AUX2, prog_aux_option_count(), DEFAULT_AUX2_OPTION) ;
  APPEND_FEATURE(PROG_CFG_FEAT_AUX3_ENABLE, FEAT_AUX3, prog_aux_option_count(), DEFAULT_AUX3_OPTION) ;
  APPEND_FEATURE(PROG_CFG_FEAT_AUX4_ENABLE, FEAT_AUX4, prog_aux_option_count(), DEFAULT_AUX4_OPTION) ;
  APPEND_FEATURE(PROG_CFG_FEAT_HORN_CONFIRMATION_ENABLE, FEAT_HORN_CONFIRMATION, 5, PROG_CFG_FEAT_HORN_CONFIRMATION_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_COMFORT_CLOSURE_ENABLE, FEAT_COMFORT_CLOSURE, 3, PROG_CFG_FEAT_COMFORT_CLOSURE_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_OEM_SECURITY_ENABLE, FEAT_OEM_SECURITY, 2, PROG_CFG_FEAT_OEM_SECURITY_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_KEY_IGN_ARM_DEFEAT_ENABLE, FEAT_KEY_IGN_ARM_DEFEAT, 2, PROG_CFG_FEAT_KEY_IGN_ARM_DEFEAT_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_LIGHT_CONTROL_ENABLE, FEAT_LIGHT_CONTROL, 2, PROG_CFG_FEAT_LIGHT_CONTROL_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_EXT_LIGHT_SUPERVISION_ENABLE, FEAT_EXT_LIGHT_SUPERVISION, 2, PROG_CFG_FEAT_EXT_LIGHT_SUPERVISION_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_HEATED_SEATS_ENABLE, FEAT_HEATED_SEATS, 2, PROG_CFG_FEAT_HEATED_SEATS_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_DEFROSTER_ENABLE, FEAT_DEFROSTER, 2, PROG_CFG_FEAT_DEFROSTER_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_SEATS_TEMP_CONTROL_ENABLE, FEAT_SEATS_TEMP_CONTROL, 4, PROG_CFG_FEAT_SEATS_TEMP_CONTROL_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_SEATS_TEMP_LEVEL_ENABLE, FEAT_SEATS_TEMP_LEVEL, prog_heated_seats_level_count(), PROG_CFG_FEAT_SEATS_TEMP_LEVEL_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_CTL_DOORLOCK_ENABLE, FEAT_CTL_DOORLOCK, prog_ctl_doorlock_option_count(), PROG_CFG_FEAT_CTL_DOORLOCK_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_SECURE_LOCK_ENABLE, FEAT_SECURE_LOCK, prog_secure_lock_option_count(), PROG_CFG_FEAT_SECURE_LOCK_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_CUSTOM1_ENABLE, FEAT_CUSTOM1, prog_custom1_option_count(), PROG_CFG_FEAT_CUSTOM1_DEFAULT_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_CUSTOM2_ENABLE, FEAT_CUSTOM2, prog_custom2_option_count(), PROG_CFG_FEAT_CUSTOM2_DEFAULT_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_CUSTOM3_ENABLE, FEAT_CUSTOM3, prog_custom3_option_count(), PROG_CFG_FEAT_CUSTOM3_DEFAULT_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_CUSTOM4_ENABLE, FEAT_CUSTOM4, prog_custom4_option_count(), PROG_CFG_FEAT_CUSTOM4_DEFAULT_VALUE) ;

  #undef APPEND_FEATURE

  FEATURE_NUM = feature_num ;
}

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
