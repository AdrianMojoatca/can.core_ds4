/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id$
/*==========================================================================*/

#ifndef __PROG_H__
#define __PROG_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
 #include "config_prog.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef void    (* Prog_Func) (void) ;

//--------------------------------------------------------------------------//

typedef enum
{
    FEAT_UNLOCK_DRIVER          ,
    FEAT_RELOCK                 ,
    FEAT_AUTO_LOCK              ,
    FEAT_AUTO_UNLOCK            ,
    FEAT_TRUNK                  ,
    FEAT_AUX1                   ,
    FEAT_AUX2                   ,
    FEAT_AUX3                   ,
    FEAT_AUX4                   ,
    FEAT_RF_OUTPUT              ,
    FEAT_HORN_CONFIRMATION      ,
    FEAT_COMFORT_CLOSURE        ,
    FEAT_OEM_SECURITY           ,
    FEAT_KEY_IGN_ARM_DEFEAT     ,
    FEAT_LIGHT_CONTROL          ,
    FEAT_EXT_LIGHT_SUPERVISION  ,
    FEAT_HEATED_SEATS           ,
    FEAT_DEFROSTER              ,
    FEAT_SEATS_TEMP_CONTROL     ,
    FEAT_SEATS_TEMP_LEVEL       ,
    FEAT_CTL_DOORLOCK           ,
    FEAT_SECURE_LOCK            ,
    
    FEAT_CUSTOM1                ,
    FEAT_CUSTOM2                ,
    FEAT_CUSTOM3                ,
    FEAT_CUSTOM4                ,

	FEAT_RUNTIME                ,

    FEAT_COUNT

} Feature_Name ;


typedef enum
{
  PROG_RF_OUTPUT_DIS        = 1 ,
  PROG_RF_OUTPUT_RFTD       = 2 ,
  PROG_RF_OUTPUT_SMARTSTART = 3 ,
                                
  PROG_RF_OUTPUT_COUNT 
  
}Prog_Rf_Output_Value ;

//--------------------------------------------------------------------------//

// Custom 1 feature options
typedef enum
{
    CUSTOM1_OPTION1 = 1,
#if CUSTOM1_OPT2_ENABLE == 1
    CUSTOM1_OPTION2,
#endif
#if CUSTOM1_OPT3_ENABLE == 1
    CUSTOM1_OPTION3,
#endif
#if CUSTOM1_OPT4_ENABLE == 1
    CUSTOM1_OPTION4,
#endif
#if CUSTOM1_OPT5_ENABLE == 1
    CUSTOM1_OPTION5,
#endif

    CUSTOM1_NUM_PLUS_ONE

} Prog_Custom1_Options ;

// Custom 2 feature options
typedef enum
{
    CUSTOM2_OPTION1 = 1,
#if CUSTOM2_OPT2_ENABLE == 1
    CUSTOM2_OPTION2,
#endif
#if CUSTOM2_OPT3_ENABLE == 1
    CUSTOM2_OPTION3,
#endif
#if CUSTOM2_OPT4_ENABLE == 1
    CUSTOM2_OPTION4,
#endif
#if CUSTOM2_OPT5_ENABLE == 1
    CUSTOM2_OPTION5,
#endif

    CUSTOM2_NUM_PLUS_ONE

} Prog_Custom2_Options ;

// Custom 3 feature options
typedef enum
{
    CUSTOM3_OPTION1 = 1,
#if CUSTOM3_OPT2_ENABLE == 1
    CUSTOM3_OPTION2,
#endif
#if CUSTOM3_OPT3_ENABLE == 1
    CUSTOM3_OPTION3,
#endif
#if CUSTOM3_OPT4_ENABLE == 1
    CUSTOM3_OPTION4,
#endif
#if CUSTOM3_OPT5_ENABLE == 1
    CUSTOM3_OPTION5,
#endif

    CUSTOM3_NUM_PLUS_ONE

} Prog_Custom3_Options ;

// Custom 4 feature options
typedef enum
{
    CUSTOM4_OPTION1 = 1,
#if CUSTOM4_OPT2_ENABLE == 1
    CUSTOM4_OPTION2,
#endif
#if CUSTOM4_OPT3_ENABLE == 1
    CUSTOM4_OPTION3,
#endif
#if CUSTOM4_OPT4_ENABLE == 1
    CUSTOM4_OPTION4,
#endif
#if CUSTOM4_OPT5_ENABLE == 1
    CUSTOM4_OPTION5,
#endif

    CUSTOM4_NUM_PLUS_ONE

} Prog_Custom4_Options ;

//--------------------------------------------------------------------------//

// CREATION OF DEFINES ACCORDING TO THE WIZARD IN CONFIG_FIRMWARE FILE.
typedef enum
{
    NO_AUX_OPTION = 1           ,
#if OPTION_TRUNK_ENABLE == 1
    TRUNK_OPTION                ,
#endif
#if OPTION_RIGHT_SLIDE_ENABLE == 1
    RIGHT_SLIDING_DOOR_OPTION   ,
#endif
#if OPTION_LEFT_SLIDE_ENABLE == 1
    LEFT_SLIDING_DOOR_OPTION    ,
#endif
#if OPTION_TAIL_GLASS_ENABLE == 1
    TAIL_GLASS_OPTION           ,
#endif
#if OPTION_GAS_CAP_ENABLE == 1
    GAS_CAP_OPTION              ,
#endif
#if OPTION_HEAD_LIGHTS_ENABLE == 1
    HEAD_LIGHTS_OPTION          ,
#endif
#if OPTION_SUNROOF_CLOSURE_ENABLE == 1
    SUNROOF_CLOSURE_OPTION      ,
#endif
#if OPTION_WINDOWS_CLOSURE_ENABLE == 1
    WINDOWS_CLOSURE_OPTION      ,
#endif
#if OPTION_WINDOWS_VENT_ENABLE == 1
    WINDOWS_VENT_OPTION         ,
#endif

    AUX_OPTION_NUM          

} Aux_Option_Name ;

//--------------------------------------------------------------------------//


typedef enum
{
#if HEATED_SEATS_LEVEL_HIGH_ENABLE == 1
    HEATED_SEATS_LEVEL_HIGH,
#endif
#if HEATED_SEATS_LEVEL_MED_ENABLE == 1
    HEATED_SEATS_LEVEL_MED,
#endif
#if HEATED_SEATS_LEVEL_LOW_ENABLE == 1
    HEATED_SEATS_LEVEL_LOW,
#endif

    HEATED_SEATS_LEVEL_NUM          

} Prog_Heated_Seats_Level ;

//--------------------------------------------------------------------------//

typedef enum
{
    UNLOCK_PRIORITY_DRIVER = 1 ,
    UNLOCK_PRIORITY_ALL        ,

} Prog_Unlock_Priority ;

//--------------------------------------------------------------------------//

typedef enum
{
    CTL_DOORLOCK_DISABLE = 1 ,

#if CTL_DOORLOCK_IGNITION_ENABLE == 1
    CTL_DOORLOCK_IGNITION    ,
#endif
#if CTL_DOORLOCK_BRAKE_ENABLE == 1
    CTL_DOORLOCK_BRAKE       , 
#endif 
#if CTL_DOORLOCK_SPEED_ENABLE == 1
    CTL_DOORLOCK_SPEED       ,
#endif
        
    CTL_DOORLOCK_NUM_PLUS_ONE,

} Prog_Ctl_Doorlock ;

//--------------------------------------------------------------------------//

typedef enum
{
    SECURE_LOCK_DISABLE = 1 ,
    SECURE_LOCK_NORMAL      ,
#if SECURE_LOCK_SMART_ENABLE == 1
    SECURE_LOCK_SMART       , 
#endif      
    SECURE_LOCK_NUM_PLUS_ONE,

} Prog_Secure_Lock ;
//--------------------------------------------------------------------------//
typedef enum
{
    RUNTIME1 = 1,
    RUNTIME2,
    RUNTIME3,

    RUNTIME_NUM          

} Prog_Runtime ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Boolean  prog_init              (void) ;
void     prog_init_reset        (void) ;

SInt8    prog_retrieve_option   (Feature_Name feature_name) ;
Boolean  prog_set_option        (UInt8 option_nb, Feature_Name feature_name) ;


#ifdef PLATFORM_CM800

    #define  prog_set_exit_callback(x)     ;
#else

    void     prog_ena               (void) ;
    void     prog_dis               (void) ;

    void     prog_set_exit_callback (Prog_Func func) ;
#endif

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

#endif


