/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_shutdown.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rs_private.h"
#include "dei_ss.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Boolean dei_rs_shutdown_user_notification( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Boolean rs_progress;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//
void dei_rs_shutdown( void )
{
#define EVT_RS_MTS_BROKEN    ( (1<<DEI_RS_EVT_BRAKE               )| \
                              (1<<DEI_RS_EVT_HOOD                )| \
                            (1<<DEI_RS_EVT_HANDBRAKE           )| \
                                        (1<<DEI_RS_EVT_TRIGGER             ) )

    if( get_rs_start() )
    {
        set_rs_start( FALSE );

        cmd_q_rs_insert( INPUT_RS_STARTER_OFF , 100 , (void*)CMD_RS_START_ERROR );
    }

    if( get_rs_ign() )
    {
        set_rs_ign( FALSE );

        set_rs_ign_off_status( TRUE );

        cmd_q_rs_insert( INPUT_RS_IGNITION_OFF , 100 );
    }

    if( get_rs_acc() )
    {
        set_rs_acc( FALSE );

        cmd_q_rs_insert( INPUT_RS_ACCESSORY_OFF , 100 );
    }


  notify_send  ( NOTIFY_RUNTIME_REMAINING  , 0);
    notify_send  ( NOTIFY_RUNTIME_PROGRAMMED , 0);

    dei_prk_flash( DEI_PRK_RS_STOP );

    dei_rs_runtime_stop();

    set_rs_gwr( FALSE );

    cmd_q_rs_insert( INPUT_GWR_OFF , 100 , (void *)CMD_RS_VALID_TAKEOVER );

    if( (1<<get_rs_error_shutdown()) & EVT_RS_MTS_BROKEN )
          set_rs_mts(FALSE);

    os_tsk_wait(500);

    if(system_type_get()==SYS_TYPE_RS) { // RS only
        dei_led_flash ( DEI_LED_RESET , LED_ENABLE);
    }

    dei_rs_shutdown_user_notification() ;
    dei_system_wait_ign_off(3500);

    rs_progress=FALSE;
    dei_ign_before_rs = get_ign_status() ? TRUE : FALSE;

    if(  get_rs_error_shutdown() != DEI_RS_EVT_TRIGGER
        && (system_type_get() == (SYS_TYPE_RS | SYS_TYPE_SS)))
    {
        // I believe we do not need bypass check when disarmed/unlocked. But just to avoid further complication, I restrict the check for brake condition during unlock. This is sometimes explicitly sent from interface.
        if((get_rs_error_shutdown() == DEI_RS_EVT_BRAKE) && !get_ss_lock())
            return;
        dei_ss_zone_bypass_start();
    }

}

//--------------------------------------------------------------------------//

Boolean dei_rs_shutdown_user_notification( void )
{
    //--- return TRUE when the taleover is valid( the engine is running )

    Boolean takeover_valid = TRUE ;

	  start_restart_guard();

    switch( get_rs_error_shutdown() )
    {
        case DEI_RS_EVT_TRANSMITTER_SHUTDOWN: //--- The user press RS button for shutdown
        {
            if( get_rs_mts_progress() )//--- the activation of MTS is in progress
            {
                 if( get_rs_mts_lock()==FALSE )
                 {
                    //---  notify the user about the error of MTS activation
                    notify_send ( NOTIFY_MTS_DISABLE );
#ifdef PLATFORM_933
                                        cmd_q_rs_insert( INPUT_GND_WHEN_READY_MODE_OFF, 100 );
#endif

                    notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_RS_PROGRAMMING );
                    dei_prk_flash( DEI_PRK_MTS_OFF );
                 }
                 else
                 {
                    if( dei_rs_mts_door_checking_timeout_status() == TRUE )
                    {
                        //--- the lock command for MTS executed but the timeout for door open not expired
                        //--- nothing to do in this case
                    }
                    else
                    {
                        //--- the timeout expired
                        if( get_rs_mts() == FALSE )
                        {
                            //--- but somehow the MTS is not active
                            notify_send ( NOTIFY_MTS_DISABLE );
#ifdef PLATFORM_933
                                                        cmd_q_rs_insert( INPUT_GND_WHEN_READY_MODE_OFF, 100 );
#endif

                            notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_RS_PROGRAMMING );
                            dei_prk_flash( DEI_PRK_MTS_OFF );
                        }
                        else
                        {
                            notify_send  ( NOTIFY_RS_STOP  );
                        }
                    }
                 }
            }
            else
            {
                //--- check the door status after shutdown and disable the MTS if is the case...
                #if TRUNK_OPEN_DONT_DISABLE_MTS == 1
                    Boolean disable_mts =(get_door_status()                       ) && get_rs_mts() && !get_rs_mts_progress();
                    #warning "The Manual Transmission Mode is not disabled by trunk"
                #else
                    Boolean disable_mts =(get_door_status()  || get_trunk_status()) && get_rs_mts() && !get_rs_mts_progress();
                #endif

                                if( (get_gearbox_type() == GEARBOX_MANUAL)
                                      && get_ign_status() && get_rs_mts() )  // IGN may be cleared in variable duration due to different vehicle
                                {
                                    timeout_start(dei_rs_mts_broken_check_timer);
                                }

                if( disable_mts )
                {
                                  dei_rs_mts_is_broken();
                }
                else
                  notify_send  ( NOTIFY_RS_STOP  );
            }
        }
        break;

        case DEI_RS_EVT_HANDBRAKE : //--- The user release the handbrake
        {
            set_rs_error_shutdown( DEI_RS_EVT_MTS );

            notify_send ( NOTIFY_MTS_DISABLE );
#ifdef PLATFORM_933
                        cmd_q_rs_insert( INPUT_GND_WHEN_READY_MODE_OFF, 100 );
#endif

            if( get_rs_mts_progress() )
            {
                //--- the activation of MTS is in progress
                //--- notify about the error
                notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_RS_PROGRAMMING );

                dei_prk_flash( DEI_PRK_MTS_OFF );
            }
            else
            {
                if( get_rs_smart() )
                {
                    notify_send ( NOTIFY_SMART_MODE_OFF );  //--- disable the smart mode
                }

                if( get_rs_timer() )
                {
                    notify_send ( NOTIFY_TIMER_MODE_OFF );  //--- disable the timer mode
                }

                if( !get_ign_status() )
                {
                    //--- not takeover mode , notify about the error
                    notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_RS_PROGRAMMING );

                    dei_prk_flash( DEI_PRK_MTS_OFF );
                }
                else
                {
                    //--- takeover mode , if the smart or timer is enable notify the user on parking light
                    if( get_rs_smart() )dei_prk_flash( DEI_PRK_SMART_OFF );
                    else
                    if( get_rs_timer() ) dei_prk_flash( DEI_PRK_TMR_OFF  );
                    else dei_prk_flash( DEI_PRK_RS_STOP );

                    set_rs_takeover(TRUE);
                }

                set_rs_smart( FALSE );
                set_rs_timer( FALSE );
                set_rs_mts  ( FALSE );
            }
        }
        break;

        case DEI_RS_EVT_BRAKE: //--- the user press the brake
        {
            if( get_gearbox_type() == GEARBOX_MANUAL )
            {
                notify_send  ( NOTIFY_MTS_DISABLE );
#ifdef PLATFORM_933
                                cmd_q_rs_insert( INPUT_GND_WHEN_READY_MODE_OFF, 100 );
#endif
            }

            if( get_rs_mts_progress() )
            {
                //--- the activation of MTS is in progress
                //--- notify about the error
                notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_RS_PROGRAMMING );
                dei_prk_flash( DEI_PRK_MTS_OFF );
            }
            else
            {
                 //--- shutdown using the brake

                if( get_rs_smart() )
                {
                    notify_send ( NOTIFY_SMART_MODE_OFF );  //--- disable the smart mode
                }

                if( get_rs_timer() )
                {
                    notify_send ( NOTIFY_TIMER_MODE_OFF );  //--- disable the timer mode
                }

                if( !get_ign_status() )
                {
                    //--- not takeover mode
                   if( get_rs_mts_progress() == TRUE )
                   {
                        notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_RS_PROGRAMMING );

                        dei_prk_flash( DEI_PRK_MTS_OFF );
                   }
                   else
                   {
                                         if( !dei_rs_takeover_without_key ) notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_BRAKE );
                                         else notify_send  ( NOTIFY_RS_STOP  );

                                         dei_rs_takeover_without_key = FALSE;
                     //--- if the smart or timer is enable notify the user on parking light
                     if( get_rs_smart() ) dei_prk_flash( DEI_PRK_SMART_OFF );
                     else
                        if( get_rs_timer() ) dei_prk_flash( DEI_PRK_TMR_OFF  );
                   }
                }
                else
                {
                    //--- takeover mode , if the smart or timer is enable notify the user on parking light

                    if( get_rs_smart() )dei_prk_flash( DEI_PRK_SMART_OFF );
                    else
                    if( get_rs_timer() ) dei_prk_flash( DEI_PRK_TMR_OFF  );

                    notify_send  ( NOTIFY_RS_STOP  );
                    dei_led_flash ( DEI_LED_DISARM , LED_ENABLE);

                    set_rs_takeover(TRUE);
                }

                set_rs_smart( FALSE );
                set_rs_timer( FALSE );
                set_rs_mts  ( FALSE );
            }
        }
        break;

        case DEI_RS_EVT_MTS_LOCK_SHUTDOWN:
        {
                      if(get_rs_mts_lock() && get_rs_mts_progress())
                        {
                            set_rs_mts(TRUE);  //update 75 message now
//                          dei_rs_mts_door_checking_timeout_stop();
                        }

            set_rs_error_shutdown( DEI_RS_EVT_TRANSMITTER_SHUTDOWN );
            notify_send  ( NOTIFY_RS_STOP  );
        }
        break;

        case DEI_RS_EVT_RUNTIME_EXPIRED  :
        {
            if( get_rs_mts_progress() )
            {
                if( get_rs_mts() == FALSE )
                {
                    //--- the activation of MTS is in progress
                    //--- notify about the error
                    notify_send ( NOTIFY_MTS_DISABLE );
#ifdef PLATFORM_933
                                        cmd_q_rs_insert( INPUT_GND_WHEN_READY_MODE_OFF, 100 );
#endif

                    notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_RS_PROGRAMMING );
                    dei_prk_flash( DEI_PRK_MTS_OFF );
                    break;  //NOTIFY_ERROR already
                }
                                else
                                  notify_send  ( NOTIFY_RS_STOP  );   //mts successful
            }
            else
            {
                //--- check the door status after shutdown and disable the MTS if is the case...

                #if TRUNK_OPEN_DONT_DISABLE_MTS == 1
                    Boolean disable_mts =(get_door_status()                       ) && get_rs_mts() && !get_rs_mts_progress();
                    #warning "The Manual Transmission Mode is not disabled by trunk"
                #else
                    Boolean disable_mts =(get_door_status()  || get_trunk_status()) && get_rs_mts() && !get_rs_mts_progress();
                #endif

#ifdef PLATFORM_CM800
                if( get_ign_status() == FALSE )
                {
                    //--- runtime expired , notify about the error
                    notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_RUNTIME_EXPIRED );
                }
#endif

                                if( (get_gearbox_type() == GEARBOX_MANUAL)
                                      && get_ign_status() && get_rs_mts() )  // IGN may be cleared in variable duration due to different vehicle
                                {
                                    timeout_start(dei_rs_mts_broken_check_timer);
                                }

                if( disable_mts )
                {
                                    dei_rs_mts_is_broken();

                }
                notify_send  ( NOTIFY_RS_STOP  );
            }
        }
        break;


        case DEI_RS_EVT_LOW_TACH:
        {
            if( get_rs_mts_progress() )
            {
                //--- the activation of MTS is in progress and the user release the handbrake
                //--- notify about the error
                notify_send  ( NOTIFY_MTS_DISABLE );
                notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_RS_PROGRAMMING );
#ifdef PLATFORM_933
                                cmd_q_rs_insert( INPUT_GND_WHEN_READY_MODE_OFF, 100 );
#endif
                dei_prk_flash( DEI_PRK_MTS_OFF );
            }
            else
            {
                //--- low tach , notify about the error
                notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_LOW_TACH );
            }
        }
        break;

        case DEI_RS_EVT_OVERREV:
        {
            if( get_rs_mts_progress() )
            {
                //--- the activation of MTS is in progress and the user release the handbrake
                //--- notify about the error
                notify_send  ( NOTIFY_MTS_DISABLE );
                notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_RS_PROGRAMMING );
#ifdef PLATFORM_933
                                cmd_q_rs_insert( INPUT_GND_WHEN_READY_MODE_OFF, 100 );
#endif
                dei_prk_flash( DEI_PRK_MTS_OFF );
            }
            else
            {
                //--- overrev , notify about the error
                notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_OVERREV );
            }
        }
        break;

        case DEI_RS_EVT_TRIGGER:
        {
            if( get_rs_mts() == TRUE )
            {
                notify_send  ( NOTIFY_MTS_DISABLE );
#ifdef PLATFORM_933
                                cmd_q_rs_insert( INPUT_GND_WHEN_READY_MODE_OFF, 100 );
#endif

                if( get_ss_trigger() == FALSE )
                {
                    notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_RS_PROGRAMMING );

                    dei_prk_flash( DEI_PRK_MTS_OFF );
                }
            }

            if( get_rs_smart() )
            {
                notify_send ( NOTIFY_SMART_MODE_OFF );  //--- disable the smart mode
            }

            if( get_rs_timer() )
            {
                notify_send ( NOTIFY_TIMER_MODE_OFF );  //--- disable the timer mode
            }

            if( (get_rs_mts() == FALSE) && (get_ss_trigger() == FALSE) )
            {
                if( get_rs_smart() )dei_prk_flash( DEI_PRK_SMART_OFF );
                else
                if( get_rs_timer() ) dei_prk_flash( DEI_PRK_TMR_OFF  );
                                else
                                {
                                    dei_prk_flash( DEI_PRK_ERROR_START_TRUNK ) ;
                  notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_FEATURE_NA );
                                }

            }

            set_rs_smart( FALSE );
            set_rs_timer( FALSE );
            set_rs_mts  ( FALSE );
//                      dei_prk_flash( DEI_PRK_TRIGGER ) ;
        }
        break;

        case DEI_RS_EVT_GLOWPLUG_TIMEOUT:
        {
            notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_WAIT_TO_START );
        }
        break;

        case DEI_RS_EVT_HOOD :
        {
            if( get_rs_mts_progress() )
            {
                //--- the activation of MTS is in progress and the user open the hood
                //--- notify about the error

                notify_send  ( NOTIFY_MTS_DISABLE );
                notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_RS_PROGRAMMING );
#ifdef PLATFORM_933
                                cmd_q_rs_insert( INPUT_GND_WHEN_READY_MODE_OFF, 100 );
#endif

                dei_prk_flash( DEI_PRK_MTS_OFF );
            }
            else
            {
                if(get_ss_trigger())   //trigger by hood
                {
                    dei_prk_flash( DEI_PRK_ERROR_START_HOOD ) ;
                }

                if(get_rs_smart())
                {
                    notify_send ( NOTIFY_SMART_MODE_OFF );
                    dei_prk_flash( DEI_PRK_SMART_OFF );
                    set_rs_smart( FALSE );
                }
                else if(get_rs_timer())
                {
                    notify_send ( NOTIFY_TIMER_MODE_OFF );  //--- disable the timer mode
                    dei_prk_flash( DEI_PRK_TMR_OFF  );
                    set_rs_timer( FALSE );
                }
                else
                    notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_HOOD );
            }
        }
        break;

        case DEI_RS_EVT_LOW_BATT :
            notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_LOW_BATT );

        break;

        default:
          notify_send  ( NOTIFY_RS_STOP  );
        break;
    }


    ATOMIC( dei_rs_state = (Dei_Rs_State)NULL );


//    if( get_rs_mts_progress() == FALSE )
//    {
        dei_rs_secure_lock_gwr_off();
//    }
//    else
//    {
//        //--- skip the secure lock because the we are in MTS activation mode
//        //--- and an ARM and LOCK will be executed after 4 seconds anyway
//    }

    set_rs_turbo           ( FALSE );
    set_rs_mts_progress    ( FALSE );
    set_rs_mts_lock        ( FALSE );
    set_rs_idle            ( FALSE );
    set_rs_start_successful( FALSE );

    return takeover_valid ;
}


//--------------------------------------------------------------------------//
void dei_rs_mts_broken_check(void)
{
    if(get_ign_status())
        dei_rs_mts_is_broken();
}

//--------------------------------------------------------------------------//
void dei_rs_mts_is_broken(void)
{
        set_rs_mts(FALSE);

        notify_send ( NOTIFY_MTS_DISABLE ); //--silent notification
#ifdef PLATFORM_933
        cmd_q_rs_insert( INPUT_GND_WHEN_READY_MODE_OFF, 100 );
#endif

        //---  notify the user about the error of MTS activation

        dei_prk_flash( DEI_PRK_MTS_OFF );

        notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_RS_PROGRAMMING );

        if( get_rs_smart() )
        {
                notify_send ( NOTIFY_SMART_MODE_OFF );  //--- disable the smart mode

                set_rs_smart( FALSE );
        }

        if( get_rs_timer() )
        {
                notify_send ( NOTIFY_TIMER_MODE_OFF );  //--- disable the timer mode

                set_rs_timer( FALSE );
        }
}
