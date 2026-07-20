/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs_init.c 33405 2015-08-26 21:21:53Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs_private.h"
#include "system_type.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define RS_IID_ENGINE_RUNNING_RPM_MIN    500
#define BRAKE_DETECTION_DEBOUNCE_500MS   50     // 10ms base
#define RPM_DETECTION_DEBOUNCE_1000MS  100     // 10ms base

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
#ifdef DEV_LPC_236x
static void sense_rs_prk_neutral_on_update(void);
static void sense_rs_prk_neutral_off_update(void);
#endif

static void sense_iid_engine_running_detected_init(void);
Per_Exec_Wake sense_iid_engine_running_detecting(void);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Boolean brake_status_previous;
static Boolean brake_status_current;
static UInt8   brake_detect_debounce;      // for 500ms on 10ms base.
static UInt8   rpm_detect_debounce;        // for 1sec on 10ms base.
static Boolean iid_stop_and_go_state;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void sense_rs_init( Sense_Rs_Input_Config *config )
{
#ifdef DEV_LPC_236x
	  //regist the input_prk_neutral commands
    static const Cmd_Array1 cmd_array[] =
    {
        {INPUT_PRK_NEUTRAL_STATUS_ON,  (Exec_Func)sense_rs_prk_neutral_on_update},
        {INPUT_PRK_NEUTRAL_STATUS_OFF, (Exec_Func)sense_rs_prk_neutral_off_update}
    };

    CMD_LUT_INSERT1(cmd_array, EXEC_MODE_NORMAL);
#endif

    if(config==NULL){
        //--- load default configuration if no configuration file is provided
        sense_rs_input_config = (Sense_Rs_Input_Config*)(&sense_rs_input_config_default);
    }else{
        sense_rs_input_config = config;
    }

	if(sense_rs_input_config->push_antenna_type == SENSE_RS_PUSH_ANTENNA_NOT_DEFINED)
	{
		sense_rs_input_config->push_antenna_type = SENSE_RS_PUSH_ANTENNA_NORMAL;
	}

    sense_rs_pi_init          ();

    sense_rs_rpm_init         ();
	sense_rs_push_board_init  ();
	sense_rs_push_antenna_init();

	#ifdef DEV_LPC_236x
  sense_rs_sensor_init      ();
	#endif
	sense_rs_gearbox_type     ();

	sense_iid_engine_running_detected_init();
}

#ifdef DEV_LPC_236x
//--------------------------------------------------------------------------//
static void sense_rs_prk_neutral_on_update(void)
{
        set_safety_sw_status( SAFETY_RS_ENABLE );

        TRACE_DEI_SENSE_RS("\n\r[SAFETY_SWITCH_WIRE_%s]\n\r" , (get_safety_sw_status() == SAFETY_RS_ENABLE) ? "RS_ENABLE":"RS_DISABLE" );
}

//--------------------------------------------------------------------------//
static void sense_rs_prk_neutral_off_update(void)
{
    set_safety_sw_status( SAFETY_RS_DISABLE );

    TRACE_DEI_SENSE_RS("\n\r[SAFETY_SWITCH_WIRE_%s]\n\r" , (get_safety_sw_status() == SAFETY_RS_ENABLE) ? "RS_ENABLE":"RS_DISABLE" );
}
#endif

//--------------------------------------------------------------------------//

UInt8 get_tamper_status(void)
{
	return !!sense_rs_tamper_sts;
}

void sense_rs_tamper_cmd_init_timeout( Boolean status )
{
    if( status )
    {
        if( get_tamper_status() )
        {
            cmd_q_rs_insert( INPUT_TAMPER | INPUT_ON , TIMEOUT_Q_INSERT );

    	    TRACE_DEI_SENSE_RS("\n\r[TAMPER_ALARM_AT_INIT]\n\r" );
        }
    }
}


void sense_rs_tamper_cmd_init( void )
{
    PER_EXEC_ATOMIC
    (
        if( get_tamper_status() )
        {
            out_q_remove( out_q_main , (Out_Func)sense_rs_tamper_cmd_init_timeout  , TRUE );
            out_q_insert( out_q_main , (Out_Func)sense_rs_tamper_cmd_init_timeout  , SENSE_RS_CMD_INIT_TIMEOUT , 1 , 1 , 1);
        }
    )
}


static void sense_rs_status_update(UInt32 *ps,Boolean sts, UInt8 from)
{
UInt32 mask;

	mask = 1<<from;
	if(sts)
		*ps |= mask;
	else
		*ps &= ~mask;
}


void sense_rs_tamper( Boolean do_init , Boolean status, UInt8 from)
{
Boolean old;
Boolean sts;

	old = get_tamper_status();
	sense_rs_status_update(&sense_rs_tamper_sts, status, from);
	sts = get_tamper_status(); // after the update
	if( do_init )
    {
			 sense_rs_tamper_cmd_init();
    }
    else
    {
			if(sts != old)
			{
				cmd_q_rs_insert( sts ? INPUT_TAMPER | INPUT_ON :  INPUT_TAMPER , TIMEOUT_Q_INSERT );
			}
    }
}

void sense_iid_engine_running_detected_init(void)
{
	System_Type dei_system_type ;
	if (nvfs_rd_s(nvfs_sys_type, &dei_system_type, sizeof(dei_system_type)) == NVFS_OK)
	{
		if ((dei_system_type & SYS_TYPE_IID) == SYS_TYPE_IID)
		{
			per_exec_register((Per_Exec_Func)sense_iid_engine_running_detecting);

			brake_status_previous = get_brake_status();
			iid_stop_and_go_state = FALSE;
		}
	}
}

static void stop_rs_iid_engine_running_indication(void)
{
	set_rs_iid_engine_running(FALSE);
	cmd_q_insert(INPUT_IID_ENGINE_RUNNING, COMMAND_INSERT_TIMEOUT);
	iid_stop_and_go_state = FALSE;
	brake_detect_debounce = 0;
	rpm_detect_debounce = 0;
}

Per_Exec_Wake sense_iid_engine_running_detecting(void)
{
	if (!get_rs_iid_engine_running())
	{
		if ((get_ign_status() && get_rpm() > RS_IID_ENGINE_RUNNING_RPM_MIN)
		)   // No remote start available if SYS_TYPE_IID enabled
		{
			set_rs_iid_engine_running(TRUE);
			cmd_q_insert(INPUT_IID_ENGINE_RUNNING | INPUT_ON, COMMAND_INSERT_TIMEOUT);
			brake_detect_debounce = 0;
			rpm_detect_debounce = 0;
			iid_stop_and_go_state = FALSE;
		}
	}
	else
	{    // iid_engine_running detected already, to handle atuo_stop cases
		if (!get_ign_status() )
		{
			stop_rs_iid_engine_running_indication();
		}
		else
		{   // ignition is still on, check the brake and rpm
			// case 1: brake on cause rpm drops. auto stop
			brake_status_current = get_brake_status();
			if (get_rpm() <= RS_IID_ENGINE_RUNNING_RPM_MIN )
			{
				if (brake_status_current && brake_status_current != brake_status_previous)
				{
					iid_stop_and_go_state = TRUE;
				}

				if (!iid_stop_and_go_state)
				{
                    if(brake_detect_debounce++ > BRAKE_DETECTION_DEBOUNCE_500MS)
					{
						if (!get_brake_status())
						{
							stop_rs_iid_engine_running_indication();
						}
					}
				}
				else
				{   // case 2: brake off cause rpm increase. auto start
					if (!brake_status_current)
					{
						if(rpm_detect_debounce++ > RPM_DETECTION_DEBOUNCE_1000MS)
						{
							stop_rs_iid_engine_running_indication();
						}
					}
					else
						rpm_detect_debounce = 0;

				}
			}
			else
			{   // RPM > THRESHOLD
				if (iid_stop_and_go_state)
				{
					iid_stop_and_go_state = FALSE;
				}

				if (brake_detect_debounce)
				{
					brake_detect_debounce = 0;   // reset debounce count
				}

				if (rpm_detect_debounce)
				{
					rpm_detect_debounce = 0;  // reset rpm debounce count
				}
			}

			brake_status_previous = brake_status_current;
		}
	}

	return PER_EXEC_SLEEP;
}




UInt8 get_ptim_stop_status(void)
{
	return !!sense_rs_tamper_sts;
}




void sense_generic(UInt32 *ps,UInt32 inpon,UInt32 inpoff,Boolean do_init,Boolean status,UInt8 from)
{
Boolean old;
Boolean sts;

	if(do_init)
	{
		*ps = 0;
	  old = !status;
	}
	else
		old = !!(*ps); // before the update
	
	sense_rs_status_update(ps, status, from);
	sts = !!(*ps); // after the update
	
	if(sts != old)
	{
		cmd_q_rs_insert( sts ? inpon :  inpoff , TIMEOUT_Q_INSERT );
	}
}



Boolean get_sense_rs_ptim_stop(void)
{
	return !!sense_rs_ptim_stop_sts;
}

void sense_rs_ptim_stop( Boolean do_init , Boolean status, UInt8 from)
{
	sense_generic(&sense_rs_ptim_stop_sts,(INPUT_PTIM_STOP| INPUT_ON),INPUT_PTIM_STOP,do_init,status,from);
}

Boolean get_sense_rs_ext_open(void)
{
	return !!sense_rs_ext_open_sts;
}

void sense_rs_ext_open( Boolean do_init , Boolean status, UInt8 from)
{
	sense_generic(&sense_rs_ext_open_sts,(INPUT_EXT_OPEN| INPUT_ON),INPUT_EXT_OPEN,do_init,status,from);
}

Boolean get_sense_rs_ext_close(void)
{
	return !!sense_rs_ext_close_sts;
}

void sense_rs_ext_close( Boolean do_init , Boolean status, UInt8 from)
{
	sense_generic(&sense_rs_ext_close_sts,(INPUT_EXT_CLOSE| INPUT_ON),INPUT_EXT_CLOSE,do_init,status,from);
}

Boolean get_sense_rs_ext_trunk(void)
{
	return !!sense_rs_ext_trunk_sts;
}

void sense_rs_ext_trunk( Boolean do_init , Boolean status, UInt8 from)
{
	sense_generic(&sense_rs_ext_trunk_sts,(INPUT_EXT_TRUNK| INPUT_ON),INPUT_EXT_TRUNK,do_init,status,from);
}


