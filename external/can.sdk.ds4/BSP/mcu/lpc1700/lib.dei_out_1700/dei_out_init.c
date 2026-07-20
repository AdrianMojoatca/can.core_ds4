/*==========================================================================*/
// $Id: dei_out_init.c 17934 2012-10-29 19:23:36Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "bus_status.h"
#include "dei_out_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define IID_ENGINE_RUNNING_PULSE_FRQ		    4.5  // 4 Hz
#define IID_ENGINE_RUNNING_PULSE_CYCLE      (1000/(IID_ENGINE_RUNNING_PULSE_FRQ*2*10))   // Half period on per_exec period (10ms)

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_out_pulse_after_rs_shutdown_init(void);
static void dei_out_delay_of_pulse_after_rs_shutdown(void);
static UInt16 get_pulse_duration_after_rs_shutdown(void);

static void dei_fob_evaluate(void);
static void dei_app_fob_set(void);
static void dei_app_fob_clr(void);

static void dei_out_iid_engine_running_set(void);
static void dei_out_iid_engine_running_clr(void);

static Per_Exec_Wake iid_engine_running_pulses(void);
static void dei_out_slide_open(void);
static void dei_out_slide_close(void);
static void dei_out_slide_partial(void);

void dei_fob_ign_on(void);
void dei_fob_ign_off(void);
void dei_fob_door_open(void);
void dei_fob_door_close(void);
void dei_fob_blank_init(void);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
Vrtc vrtc_pulse_delay_after_rs_shutdown;

UInt16 iid_engine_running_pulse_var;
UInt8 iid_engine_running_pulse_state;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//
void dei_out_init( void )
{
    prg_out_init();

    cmd_register:
    {
        static const Cmd_Array1 cmd_array[] =
        {
            { INPUT_GWR_OFF                   , (Exec_Func)dei_out_ground_when_running_status_off},
            { INPUT_GWR_ON                    , (Exec_Func)dei_out_ground_when_running_status_on },
            { INPUT_RS_ACCESSORY_OFF          , (Exec_Func)dei_out_acc_off                       },
            { INPUT_RS_ACCESSORY_ON           , (Exec_Func)dei_out_acc_on                        },
            { INPUT_RS_IGNITION_OFF           , (Exec_Func)dei_out_ign_off                       },
            { INPUT_RS_IGNITION_ON            , (Exec_Func)dei_out_ign_on                        },
            { INPUT_RS_STARTER_OFF            , (Exec_Func)dei_out_start_off                     },
            { INPUT_RS_STARTER_ON             , (Exec_Func)dei_out_start_on                      },
            { INPUT_GWR_OFF                   , (Exec_Func)dei_out_smartkey_control              },
            { INPUT_RS_STARTER_OFF            , (Exec_Func)dei_out_pulse_after_start             },
          //  { INPUT_UNLOCK_DRIVER1|INPUT_ON   , (Exec_Func)dei_out_unlock_user2                  },
            { INPUT_UNLOCK_DRIVER2|INPUT_ON   , (Exec_Func)dei_out_unlock_user2                  },
            { INPUT_UNLOCK_DRIVER3|INPUT_ON   , (Exec_Func)dei_out_unlock_user2                  },
          //  { INPUT_UNLOCK_ALL1   |INPUT_ON   , (Exec_Func)dei_out_unlock_user2                  },
            { INPUT_UNLOCK_ALL2   |INPUT_ON   , (Exec_Func)dei_out_unlock_user2                  },
            { INPUT_UNLOCK_ALL3   |INPUT_ON   , (Exec_Func)dei_out_unlock_user2                  },

            { INPUT_ARM                       , (Exec_Func)dei_out_ground_when_arm               },
            { INPUT_DISARM                    , (Exec_Func)dei_out_ground_when_arm               },
            { INPUT_RF_VALET_TOOGLE           , (Exec_Func)dei_out_ground_when_arm_deactivate    },
            { INPUT_VALET_TOGGLE              , (Exec_Func)dei_out_ground_when_arm_deactivate    },

            { INPUT_LOCK1         |INPUT_ON   , (Exec_Func)dei_out_ground_when_locked            },
            { INPUT_LOCK2         |INPUT_ON   , (Exec_Func)dei_out_ground_when_locked            },
            { INPUT_LOCK3         |INPUT_ON   , (Exec_Func)dei_out_ground_when_locked            },
            { INPUT_UNLOCK_DRIVER1|INPUT_ON   , (Exec_Func)dei_out_ground_when_locked            },
            { INPUT_UNLOCK_DRIVER2|INPUT_ON   , (Exec_Func)dei_out_ground_when_locked            },
            { INPUT_UNLOCK_DRIVER3|INPUT_ON   , (Exec_Func)dei_out_ground_when_locked            },
            { INPUT_UNLOCK_ALL1   |INPUT_ON   , (Exec_Func)dei_out_ground_when_locked            },
            { INPUT_UNLOCK_ALL2   |INPUT_ON   , (Exec_Func)dei_out_ground_when_locked            },
            { INPUT_UNLOCK_ALL3   |INPUT_ON   , (Exec_Func)dei_out_ground_when_locked            },
            { INPUT_OEM_LOCK                  , (Exec_Func)dei_out_ground_when_locked            },
            { INPUT_OEM_UNLOCK                , (Exec_Func)dei_out_ground_when_locked            },
            { INPUT_OEM_ARM                   , (Exec_Func)dei_out_ground_when_locked            },
            { INPUT_OEM_DISARM                , (Exec_Func)dei_out_ground_when_locked            },
            { INPUT_LOCK1         |INPUT_ON   , (Exec_Func)dei_out_lock                          },
            { INPUT_LOCK2         |INPUT_ON   , (Exec_Func)dei_out_lock                          },
            { INPUT_LOCK3         |INPUT_ON   , (Exec_Func)dei_out_lock                          },
            { INPUT_UNLOCK_DRIVER1|INPUT_ON   , (Exec_Func)dei_out_unlock_driver                 },
            { INPUT_UNLOCK_DRIVER2|INPUT_ON   , (Exec_Func)dei_out_unlock_driver                 },
            { INPUT_UNLOCK_DRIVER3|INPUT_ON   , (Exec_Func)dei_out_unlock_driver                 },
            { INPUT_UNLOCK_ALL1   |INPUT_ON   , (Exec_Func)dei_out_unlock_all                    },
            { INPUT_UNLOCK_ALL2   |INPUT_ON   , (Exec_Func)dei_out_unlock_all                    },
            { INPUT_UNLOCK_ALL3   |INPUT_ON   , (Exec_Func)dei_out_unlock_all                    },
            { INPUT_UNLOCK_ALL1   |INPUT_ON   , (Exec_Func)dei_out_second_unlock                 },
            { INPUT_UNLOCK_ALL2   |INPUT_ON   , (Exec_Func)dei_out_second_unlock                 },
            { INPUT_UNLOCK_ALL3   |INPUT_ON   , (Exec_Func)dei_out_second_unlock                 },
            { INPUT_TRUNK         |INPUT_ON   , (Exec_Func)dei_out_trunk_release_on              },
            { INPUT_TRUNK                     , (Exec_Func)dei_out_trunk_release_off             },
            { INPUT_ARM                       , (Exec_Func)dei_out_factory_alarm_arm             },
            { INPUT_DISARM                    , (Exec_Func)dei_out_factory_alarm_disarm          },




            { INPUT_DOME_ON                   , (Exec_Func)dei_out_domelight_on                  },
            { INPUT_DOME_OFF                  , (Exec_Func)dei_out_domelight_off                 },

                    // new for NGMM
            { INPUT_LOCK1     |INPUT_ON      , (Exec_Func)dei_out_comfort_closure                },
            { INPUT_DEFOGGER_ON               , (Exec_Func)dei_out_defroster_latched_on        },
            { INPUT_WARNING_ON               , (Exec_Func)dei_out_ext_sensor_warnaway       },
            { INPUT_TRIGGER_ON                , (Exec_Func)dei_out_sensor_trigger    },

            { INPUT_DEFOGGER_OFF              , (Exec_Func)dei_out_defroster_latched_off             },
            { INPUT_DEFOGGER_ON               , (Exec_Func)dei_out_defroster_pulsed_on              },
//          { INPUT_DEFOGGER_OFF              , (Exec_Func)dei_out_defroster_pulsed_off             },No pulse during remote stop.
            { INPUT_GND_WHEN_READY_MODE_ON    , (Exec_Func)dei_out_ground_when_ready_mode_on       },
            { INPUT_GND_WHEN_READY_MODE_OFF   , (Exec_Func)dei_out_ground_when_ready_mode_off      },
//                  { INPUT_RF_SILENT_UNLOCK          , (Exec_Func)dei_out_ground_when_arm               },
//                  { INPUT_RF_SILENT_LOCK            , (Exec_Func)dei_out_ground_when_arm               },
//                  { INPUT_IGN_STATUS_OFF            , (Exec_Func)dei_out_ign_off_for_starter_kill          },
                    { INPUT_CHIRP                     , (Exec_Func)dei_out_horn                          },
                    { INPUT_PARKING_ON                , (Exec_Func)dei_out_parking_light_on              },
                    { INPUT_PARKING_OFF               , (Exec_Func)dei_out_parking_light_off             },

                    // AntiGrind
                    //{ INPUT_GWR_ON                        , (Exec_Func)dei_out_ground_when_arm               },
                    //{ INPUT_RS_ACCESSORY_OFF          , (Exec_Func)dei_out_ground_when_arm               },

                    // Starter Kill
                    { INPUT_IGN_STATUS_ON             , (Exec_Func)dei_out_sk_ign_on },
                    { INPUT_IGN_STATUS_OFF            , (Exec_Func)dei_out_sk_ign_off },
                    { INPUT_UNLOCK_ALL1 | INPUT_ON  , (Exec_Func)dei_out_sk_unlock },
                    { INPUT_UNLOCK_ALL2 | INPUT_ON  , (Exec_Func)dei_out_sk_unlock },
                    { INPUT_UNLOCK_ALL3 | INPUT_ON  , (Exec_Func)dei_out_sk_unlock },
                    { INPUT_UNLOCK_DRIVER1 | INPUT_ON   , (Exec_Func)dei_out_sk_unlock },
                    { INPUT_UNLOCK_DRIVER2 | INPUT_ON   , (Exec_Func)dei_out_sk_unlock },
                    { INPUT_UNLOCK_DRIVER3 | INPUT_ON   , (Exec_Func)dei_out_sk_unlock },
                    { INPUT_RF_LOCK                           , (Exec_Func)dei_out_sk_lock },
                    { INPUT_RF_SILENT_LOCK            , (Exec_Func)dei_out_sk_lock },
            { INPUT_OEM_UNLOCK                      , (Exec_Func)dei_out_sk_unlock },  // SUSPECTED!
            { INPUT_OEM_LOCK                                , (Exec_Func)dei_out_sk_lock },
                    { INPUT_STARTERKILL_SUSPEND             , (Exec_Func)dei_out_sk_bac },
          { INPUT_GWR_OFF                           , (Exec_Func)dei_out_sk_gwr_off },
                    /* pulse after RS shutdown */
                    { INPUT_GWR_OFF                             , (Exec_Func)dei_out_pulse_after_rs_shutdown },

            { INPUT_GWR_ON                              , (Exec_Func)dei_out_sk_gwr_on },

                    /* control of "FOB" function */
                    { INPUT_GWR_ON                    , (Exec_Func)dei_fob_evaluate },
                    { INPUT_GWR_OFF                   , (Exec_Func)dei_fob_evaluate },
                    { INPUT_APP_FOB | INPUT_ON        , (Exec_Func)dei_app_fob_set},
                    { INPUT_APP_FOB                   , (Exec_Func)dei_app_fob_clr},
                    { INPUT_IGN_STATUS_ON             , (Exec_Func)dei_fob_ign_on},
                    { INPUT_IGN_STATUS_OFF            , (Exec_Func)dei_fob_ign_off},
                    { INPUT_ALL_DOOR_STATUS_OPEN      , (Exec_Func)dei_fob_door_open},
                    { INPUT_ALL_DOOR_STATUS_CLOSE     , (Exec_Func)dei_fob_door_close},
										
										
					{ INPUT_IID_ENGINE_RUNNING | INPUT_ON, (Exec_Func)dei_out_iid_engine_running_set},
					{ INPUT_IID_ENGINE_RUNNING           , (Exec_Func)dei_out_iid_engine_running_clr},
					
					{ INPUT_SLIDE_OPEN, (Exec_Func) dei_out_slide_open},
					{ INPUT_SLIDE_CLOSE, (Exec_Func) dei_out_slide_close},
					{ INPUT_SLIDE_PARTIAL, (Exec_Func) dei_out_slide_partial},
					
        };


        CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);

    }

  //cmd_lut_insert(INPUT_CHIRP,(Exec_Func)dei_out_horn,EXEC_MODE_PROG);

    dei_out_lock_init();
    dei_out_aux1_init();
    dei_out_aux2_init();
    dei_out_aux3_init();
    dei_out_aux4_init();

  dei_out_sk_init();

  dei_out_pulse_after_rs_shutdown_init();

  per_exec_register((Per_Exec_Func)iid_engine_running_pulses);
		
		dei_fob_blank_init();
		
}

//--------------------------------------------------------------------------//
void dei_out_ext_sensor_warnaway(void)
{
 if( !get_ss_sensor_silent_arm() && !get_ss_full_silent_arm() )
 {
        DEI_OUT_SENSOR_TRIGGER_RESET();
        DEI_OUT_SENSOR_TRIGGER(0, 50, 1, 1);
 }
}

//--------------------------------------------------------------------------//
void dei_out_sensor_trigger(void)
{
    if( !get_ss_sensor_silent_arm() && !get_ss_full_silent_arm() )
    {
        if(get_ss_sensor_trigger(2) || get_ss_sensor_trigger(1) || get_ss_sensor_trigger(0))  //shock or external sensor
        {
            DEI_OUT_SENSOR_TRIGGER_RESET();
            DEI_OUT_SENSOR_TRIGGER(0, 150, 1, 1);
        }
    }
}

/* delay timeout*/
static void dei_out_delay_of_pulse_after_rs_shutdown(void)
{
        UInt16 duration;
      duration = get_pulse_duration_after_rs_shutdown();

        if(!get_rs_takeover())    /* NOT for takeover */
        {
            DEI_OUT_PULSE_AFTER_RS_SHUTDOWN_RESET();
            DEI_OUT_PULSE_AFTER_RS_SHUTDOWN(0, duration, 1, 1);
        }
}


/*  initialize the vrtc */
static void dei_out_pulse_after_rs_shutdown_init(void)
{
        vrtc_pulse_delay_after_rs_shutdown = vrtc_f_create((Exec_Func)dei_out_delay_of_pulse_after_rs_shutdown);
}


static UInt16 get_pulse_duration_after_rs_shutdown(void)
{
    UInt8  duration_idx;

    static UInt16 duration_option[3] =
    {
        25,
        50,
        100
    }; // in milliseconds (10 milliseconds base)

    duration_idx = dei_feature_get(DEI_FEAT_DURATION_PULSE_AFTER_RS_SHUTDOWN);    // [1 : 3]

    if(duration_idx > 0 && duration_idx < 4)   // in reasonable range
      return duration_option[duration_idx - 1];
    else
        return 0;
}

//--------------------------------------------------------------------------//
void dei_out_pulse_after_rs_shutdown(void)
{
    UInt8  delay_idx;

    static UInt16 delay_option[11] =
    {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    }; // in seconds

    delay_idx = dei_feature_get(DEI_FEAT_ACT_DELAY_PULSE_AFTER_RS_SHUTDOWN);      // [1:10]

    // start delay here
    vrtc_stop(vrtc_pulse_delay_after_rs_shutdown);
    vrtc_update(vrtc_pulse_delay_after_rs_shutdown, delay_option[delay_idx] * VRTC_1SEC);
    vrtc_start(vrtc_pulse_delay_after_rs_shutdown);
}

//--------------------------------------------------------------------------//
#define FOB_BLANK_W8DOOR 1  // ignition went off while door was closed
#define FOB_BLANK_W8IGN  7  // door went open, while ignition was on
#define FOB_BLANK_BLANK 10   // we are blanking the fob because 1st door open after (or at) ignition off
#define FOB_BLANK_DELAY  8  // door just closed, but still blanking, waiting for final timeout
#define FOB_BLANK_OTHER 0    // all other KIB states we don't care about


static UInt8 fob_blank;
static struct cssos_timer *fob_blank_timer;

static UInt8 app_fob_status = 0; // off

int get_app_fob_status(void)
{
    return app_fob_status;
}

void dei_fob_evaluate(void)
{
UInt8 need;
    need = get_rs_gwr() | get_app_fob_status();
	  if(fob_blank == FOB_BLANK_BLANK || fob_blank == FOB_BLANK_DELAY)
	    need = 0;
    if(need != prg_out_shadow_read(1+PRG_OUT_FOB))
    {// change
        prg_out_fob(need);
    }
}

void set_app_fob_status(Boolean status)
{
    app_fob_status = status;
    dei_fob_evaluate();
}


void dei_app_fob_set(void)
{
    set_app_fob_status(1);
}

void dei_app_fob_clr(void)
{
    set_app_fob_status(0);
}

// manipulation of fob_blank
static void set_fob_blank(UInt8 newstate)
{
	fob_blank = newstate;
	dei_fob_evaluate();
}

void dei_fob_ign_on(void)
{
	set_fob_blank(FOB_BLANK_OTHER);
}


void dei_fob_ign_off(void)
{
	if(fob_blank == FOB_BLANK_W8IGN)
		set_fob_blank(FOB_BLANK_BLANK);
	else if(get_app_fob_status() && !get_door_status())  // connected and door closed?
		set_fob_blank(FOB_BLANK_W8DOOR);  // wait for door open
	else
		set_fob_blank(FOB_BLANK_OTHER);
}

void dei_fob_door_open(void)
{
	if(fob_blank == FOB_BLANK_W8DOOR)
		set_fob_blank(FOB_BLANK_BLANK);
	else if(get_app_fob_status() && get_ign_status()) // connected and ignition on?
		set_fob_blank(FOB_BLANK_W8IGN);  // wait for ignition off
	else
		set_fob_blank(FOB_BLANK_OTHER);
}

void dei_fob_door_close(void)
{
	if(fob_blank == FOB_BLANK_BLANK)
	{
		cssos_timer_rewind(fob_blank_timer,RT_SECOND);
		set_fob_blank(FOB_BLANK_DELAY); // delay a bit longer
	}
	else
		set_fob_blank(FOB_BLANK_OTHER);
}

void fob_blank_handler(void)
{
	set_fob_blank(FOB_BLANK_OTHER);
}

void dei_fob_blank_init(void)
{
	  fob_blank = FOB_BLANK_OTHER;
		fob_blank_timer = cssos_timer_create(fob_blank_handler);
}

//////////////////////



static void dei_out_iid_engine_running_set(void)
{
  iid_engine_running_pulse_var = IID_ENGINE_RUNNING_PULSE_CYCLE;
	iid_engine_running_pulse_state = 1;
	prg_out_iid_engine_running(TRUE);  // starts from high
  prg_out_iid_relay1(1);
}

static void dei_out_iid_engine_running_clr(void)
{
    iid_engine_running_pulse_var = 0;   // stop the timer
    prg_out_iid_engine_running(FALSE);
	prg_out_iid_relay1(0);
}

Per_Exec_Wake iid_engine_running_pulses(void)
{
	if (iid_engine_running_pulse_var == 0)
	{
		prg_out_iid_engine_running(FALSE);
		return PER_EXEC_SLEEP;
	}

	if (--iid_engine_running_pulse_var != 0)
	{
		return PER_EXEC_WAKE;
	}

	// half period?
	if (iid_engine_running_pulse_state)
	{
		iid_engine_running_pulse_state = 0;
		prg_out_iid_engine_running(FALSE);
	}
	else
	{
		iid_engine_running_pulse_state = 1;
		prg_out_iid_engine_running(TRUE);
	}

	// re-assign the PULSE CYCLE
	iid_engine_running_pulse_var = IID_ENGINE_RUNNING_PULSE_CYCLE;

	return PER_EXEC_WAKE;
}



void dei_out_slide_open(void)
{
	prg_out_slide(1+PRG_OUT_SLOPEN);
}

void dei_out_slide_close(void)
{
	prg_out_slide(1+PRG_OUT_SLCLOSE);
}

void dei_out_slide_partial(void)
{
	prg_out_slide(1+PRG_OUT_SLOPEN);
	prg_out_slide(1+PRG_OUT_SLCLOSE);
}

