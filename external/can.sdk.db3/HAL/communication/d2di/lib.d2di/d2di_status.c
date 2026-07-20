/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2di_private.h"
#include "status.h"
#include "d2d_code.h"
#include "wake.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
static void d2di_notify_input_ignition_on      (void * arg);
static void d2di_notify_input_ignition_off     (void * arg);
static void d2di_notify_input_rs_ignition_on   (void * arg);
static void d2di_notify_input_rs_ignition_off  (void * arg);

static void d2di_notify_brake_support          (void * arg);
static void d2di_notify_handbrake_support      (void * arg);
static void d2di_notify_hood_support           (void * arg);
static void d2di_notify_trunk_support          (void * arg);
static void d2di_notify_door_support           (void * arg);
static void d2di_notify_rpm_support            (void * arg);
static void d2di_notify_key_in_support         (void * arg);

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
void d2di_status_init (void)
{

    static const Cmd_Array1 cmd_array[] =
    {
//STATUS RECEIVED FROM CAR
        {INPUT_IGNITION                , d2di_notify_input_ignition_off },
        {INPUT_IGNITION_ON             , d2di_notify_input_ignition_on  },
        
        {INPUT_RS_IGNITION             , d2di_notify_input_rs_ignition_off },
        {INPUT_RS_IGNITION | INPUT_ON  , d2di_notify_input_rs_ignition_on  },

//STATUS BROADCAST
        {INPUT_BRAKE_STATUS_PRESS           , d2di_notify_brake_support             },
        {INPUT_BRAKE_STATUS_RELEASE         , d2di_notify_brake_support             },
        {INPUT_HANDBRAKE_STATUS_ON          , d2di_notify_handbrake_support         },
        {INPUT_HANDBRAKE_STATUS_OFF         , d2di_notify_handbrake_support         },
        {INPUT_HOOD_STATUS_OPEN             , d2di_notify_hood_support              },
        {INPUT_HOOD_STATUS_CLOSE            , d2di_notify_hood_support              },
        {INPUT_TRUNK_STATUS_OPEN            , d2di_notify_trunk_support             },
        {INPUT_TRUNK_STATUS_CLOSE           , d2di_notify_trunk_support             },
        {INPUT_ALL_DOOR_STATUS_OPEN         , d2di_notify_door_support              },
        {INPUT_ALL_DOOR_STATUS_CLOSE        , d2di_notify_door_support              },
        {INPUT_RPM_SET                      , d2di_notify_rpm_support               },
        {INPUT_RPM_CLR                      , d2di_notify_rpm_support               },
        {INPUT_KEY_IN_STATUS_ON             , d2di_notify_key_in_support            },
        {INPUT_KEY_IN_STATUS_OFF            , d2di_notify_key_in_support            },
    };

    CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
}

//--------------------------------------------------------------------------//
/* Ignition status */
static void d2di_notify_input_ignition_on (void * arg)
{
    if(!d2di_vars.ignition)
    {
        d2di_vars.ignition = TRUE;
        d2di_vars.block_sleep_restart = FALSE;
        wake_enter();
        #if DBG_D2D
            dbg_tx_text("\n[D2DI_STATUS]ign on");
        #endif    
    }
}

//--------------------------------------------------------------------------//
static void d2di_notify_input_ignition_off (void * arg)
{
    if(d2di_vars.ignition)  
    {
        d2di_vars.ignition = FALSE;
        wake_leave();
        #if DBG_D2D
            dbg_tx_text("\n[D2DI_STATUS]ign off");
        #endif    
    }
}

//--------------------------------------------------------------------------//
/* Remote starter ignition status. */
static void d2di_notify_input_rs_ignition_on (void * arg)
{
    if(!d2di_vars.rs_ignition)
    {
        d2di_vars.rs_ignition = TRUE;
        d2di_vars.block_sleep_restart = FALSE;
        wake_enter();
        #if DBG_D2D
            dbg_tx_text("\n[D2DI_STATUS]RS ign on");
        #endif    
    }
}

//--------------------------------------------------------------------------//
static void d2di_notify_input_rs_ignition_off (void * arg)
{
    if(d2di_vars.rs_ignition)
    {
        d2di_vars.rs_ignition = FALSE;
        d2di_vars.block_sleep_restart = FALSE;
        wake_leave();
        #if DBG_D2D
            dbg_tx_text("\n[D2DI_STATUS]RS ign on");
        #endif    
    }
}
//--------------------------------------------------------------------------//
void d2di_notify_brake_support (void * arg)
{
    if(d2di_mask_is_enabled(SET_BRAKE_ON))
    {
        d2di_vars.status_support.brake.bit.brake = 1;
    }
    #if DBG_D2D
            dbg_tx_text("\n[D2DI_STATUS]brake");
    #endif
}
//--------------------------------------------------------------------------//
void d2di_notify_handbrake_support (void * arg)
{
    if(d2di_mask_is_enabled(SET_HANDBRAKE_ON))
    {
        d2di_vars.status_support.brake.bit.handbrake = 1;
    }
    #if DBG_D2D
        dbg_tx_text("\n[D2DI_STATUS]handbrake");
    #endif
}
//--------------------------------------------------------------------------//
void d2di_notify_hood_support (void * arg)
{
    if(d2di_mask_is_enabled(SET_HOOD_OPEN))
    {
        d2di_vars.status_support.alarm_trigger.bit.hood = 1;
    }
    #if DBG_D2D
        dbg_tx_text("\n[D2DI_STATUS]hood");
    #endif
}
//--------------------------------------------------------------------------//
void d2di_notify_trunk_support (void * arg)
{
    if(d2di_mask_is_enabled(SET_TRUNK_OPEN))
    {
        d2di_vars.status_support.alarm_trigger.bit.trunk = 1;
    }
    #if DBG_D2D
        dbg_tx_text("\n[D2DI_STATUS]trunk");
    #endif
}
//--------------------------------------------------------------------------//
void d2di_notify_door_support (void * arg)
{
    
    Door               doors_source = {0};
    doors_source.byte = (UInt8)status_get_door(STATUS_DOOR_BYTE);
    
    if(d2di_mask_is_enabled(SET_DRIVER_OPEN))
    {        
        d2di_vars.status_support.alarm_trigger.bit.front_driver |= doors_source.bit.front_driver;
    }
    if(d2di_mask_is_enabled(SET_OTHER_OPEN))
    {
        d2di_vars.status_support.alarm_trigger.bit.front_pass  |= doors_source.bit.front_pass;
        d2di_vars.status_support.alarm_trigger.bit.rear_driver |= doors_source.bit.rear_driver;
        d2di_vars.status_support.alarm_trigger.bit.rear_pass   |= doors_source.bit.rear_pass;
    }
    
}
//--------------------------------------------------------------------------//
void d2di_notify_rpm_support (void * arg)
{
    if(d2di_mask_is_enabled(SET_RPM))
    {
        d2di_vars.status_support.tach = 1;
    }
    #if DBG_D2D
        dbg_tx_text("\n[D2DI_STATUS]tach");
    #endif
}
//--------------------------------------------------------------------------//
void d2di_notify_key_in_support(void * arg)
{
    if(d2di_mask_is_enabled(SET_KEY_IN))
    {
        d2di_vars.status_support.alarm_trigger.bit.key_in = 1;
    }
    #if DBG_D2D
        dbg_tx_text("\n[D2DI_STATUS]key in");
    #endif
}
