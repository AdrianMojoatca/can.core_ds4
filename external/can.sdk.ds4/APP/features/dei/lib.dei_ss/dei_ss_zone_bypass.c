/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_zone_bypass.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ss_private.h"
#include "sense_rs.h"
#include "sensor_proto_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Dei_Ss_Zone_Bypass
{
    Vrtc    vrtc;
    Boolean is_first_check;

}Dei_Ss_Zone_Bypass;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
//void dei_ss_zone_bypass_start  ( void );
void dei_ss_zone_bypass_stop   ( void );
static void dei_ss_zone_bypass_timeout( void );
static void dei_ss_zone_bypass_restart( void );
void dei_ss_zone_bypass_inst_trig_off( void );
static void dei_ss_zone_bypass_inst_trig_off (void);
void dei_ss_trunk_bypass_start  ( void );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static Dei_Ss_Zone_Bypass   dei_ss_zone_bypass_vars;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

Dei_Led_Pattern dei_ss_led_zone_bypass(void)
{
Dei_Led_Pattern pat = DEI_LED_NONE;

	if(get_ss_door_bypass())
		pat |= DEI_LED_DOOR_BYPASSED;
	if(get_ss_hood_bypass())
		pat |= DEI_LED_HOOD_BYPASSED;
  if(get_ss_trunk_bypass())
		pat |= DEI_LED_TRUNK_BYPASSED;
	if(ext_sensor_trigger_wire_status)
		pat |= DEI_LED_SHOCK_SENSOR_TRIGGERED;

	return pat;
}


void dei_ss_zone_bypass_init( void )
{
	 dei_ss_zone_bypass_vars.vrtc = vrtc_f_create( (Exec_Func)dei_ss_zone_bypass_timeout ) ;

	 cmd_register:
     {
        static const Cmd_Array1 cmd_array[] =
        {
             { INPUT_OEM_LOCK                , (Exec_Func)dei_ss_zone_bypass_start  },
             { INPUT_OEM_ARM                 , (Exec_Func)dei_ss_zone_bypass_start  },
        	 { INPUT_RF_LOCK                 , (Exec_Func)dei_ss_zone_bypass_start  },
             { INPUT_RF_SILENT_LOCK          , (Exec_Func)dei_ss_zone_bypass_start  },
             { INPUT_OEM_UNLOCK              , (Exec_Func)dei_ss_zone_bypass_stop   },
             { INPUT_OEM_DISARM              , (Exec_Func)dei_ss_zone_bypass_stop   },
        	 { INPUT_RF_UNLOCK               , (Exec_Func)dei_ss_zone_bypass_stop   },
             { INPUT_RF_SILENT_UNLOCK        , (Exec_Func)dei_ss_zone_bypass_stop   },
             { INPUT_ALL_DOOR_STATUS_OPEN    , (Exec_Func)dei_ss_zone_bypass_restart},
             { INPUT_ALL_DOOR_STATUS_CLOSE   , (Exec_Func)dei_ss_zone_bypass_restart},
			 { INPUT_INST_TRIG_STATUS_OFF		 , (Exec_Func)dei_ss_zone_bypass_inst_trig_off },
             { INPUT_RF_TRUNK | INPUT_RF_EXT , (Exec_Func)dei_ss_trunk_bypass_start   },
             { INPUT_OEM_TRUNK               , (Exec_Func)dei_ss_trunk_bypass_start   },

        };

        CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL );
    }
}

//--------------------------------------------------------------------------//

void dei_ss_zone_bypass_start( void )
{
    if( get_ss_trigger()  || get_ss_panic()  ) return ;
	if( get_ign_status()  && !get_rs_gwr()   ) return ;
	if( !get_ss_arm() || get_ss_valet() ) return ;

	if( (ss_system_type & SYS_TYPE_SS) )
	{
		ATOMIC( dei_ss_zone_bypass_vars.is_first_check = TRUE );

    set_zone_bypass_status( TRUE );
    set_ss_trunk_bypass( FALSE );	// Arm or lock request stop bypassing the trunk

    vrtc_stop( dei_ss_zone_bypass_vars.vrtc ) ;

		vrtc_update( dei_ss_zone_bypass_vars.vrtc , DEI_SS_VRTC_TIMEOUT_ZONE_BYPASS_FIRST_CHECK );

		vrtc_start( dei_ss_zone_bypass_vars.vrtc ) ;

    TRACE_DEI_SS("[DEI_SS_ZONE_BYPASS_START]\n\r");
	}
}

//--------------------------------------------------------------------------//

void dei_ss_trunk_bypass_start( void )
{
    if( get_ss_trigger()  || get_ss_panic()  ) return ;
	if( get_ign_status()  && !get_rs_gwr()   ) return ;
	if( !get_ss_arm() || get_ss_valet() ) return ;

	if( (ss_system_type & SYS_TYPE_SS) )
	{
		ATOMIC( dei_ss_zone_bypass_vars.is_first_check = TRUE );

    set_ss_trunk_bypass( TRUE );

    vrtc_stop( dei_ss_zone_bypass_vars.vrtc ) ;

		vrtc_update( dei_ss_zone_bypass_vars.vrtc , DEI_SS_VRTC_TIMEOUT_ZONE_BYPASS_FIRST_CHECK );

		vrtc_start( dei_ss_zone_bypass_vars.vrtc ) ;

    TRACE_DEI_SS("[DEI_SS_TRUNK_BYPASS_START]\n\r");
	}
}
//--------------------------------------------------------------------------//

void dei_ss_zone_bypass_restart( void )
{
    if( get_zone_bypass_status() )
    {
        vrtc_stop( dei_ss_zone_bypass_vars.vrtc ) ;

	    vrtc_update( dei_ss_zone_bypass_vars.vrtc , DEI_SS_VRTC_TIMEOUT_ZONE_BYPASS );

	    vrtc_start( dei_ss_zone_bypass_vars.vrtc ) ;

        TRACE_DEI_SS("[DEI_SS_ZONE_BYPASS_RESTART]\n\r");
    }

		if(dei_ss_door_is_skip)
			cmd_q_rs_insert(INPUT_DOOR_SKIP_SET, 100, 0);  // rewind skip
}

//--------------------------------------------------------------------------//

void dei_ss_zone_bypass_stop( void )
{
	if( get_ss_trigger()  || get_ss_panic()  ) return ;

	dei_led_flash( DEI_LED_RESET , LED_ENABLE );

	set_zone_bypass_status(  FALSE );
    set_ss_trunk_bypass( FALSE );

	vrtc_stop( dei_ss_zone_bypass_vars.vrtc ) ;

    TRACE_DEI_SS("[DEI_SS_ZONE_BYPASS_STOP]\n\r");

	if(dei_ss_door_is_skip)
			cmd_q_rs_insert(INPUT_DOOR_SKIP_CLR, 100, 0);  // clear skip
}

//--------------------------------------------------------------------------//


void dei_ss_zone_bypass_timeout( void )
{
	Boolean door_bp , hood_bp , trunk_bp , inst_trig_bp ;
	Boolean ext_trig_bp;
	UInt8 ext_zones;
	ext_trig_bp = ext_sensor_trigger_wire_status;
    ext_zones = sensor_proto_zonecheck(); // check all connected wire zones

	if( get_zone_bypass_status() || get_ss_trunk_bypass() || ext_zones )
	{
	  	TRACE_DEI_SS("[DEI_SS_ZONE_BYPASS_TIMEOUT]\n\r");

        door_bp  = get_door_status () || (ext_zones & (1<<DEI_SS_TRIGGER_ZONE_DOOR));
		hood_bp  = get_hood_status ()|| (ext_zones & (1<<DEI_SS_TRIGGER_ZONE_HOOD));
		trunk_bp = get_trunk_status()|| (ext_zones & (1<<DEI_SS_TRIGGER_ZONE_TRUNK));
        inst_trig_bp = get_inst_trig_wire_status();

		set_ss_door_bypass ( door_bp  );
		set_ss_hood_bypass ( hood_bp  );

#ifdef PLATFORM_933
		if((pop_trunk_state==POP_TRUNK_STATE_WAIT10MIN)||(pop_trunk_state==POP_TRUNK_STATE_WAIT_CLOSE_2))
		{
			set_ss_trunk_bypass(TRUE);
		}
		else
		{
			set_ss_trunk_bypass( trunk_bp );
		}
#else
		set_ss_trunk_bypass( trunk_bp );
#endif

		if( hood_bp || trunk_bp || door_bp || inst_trig_bp || ext_trig_bp|| (ext_zones & (1<<DEI_SS_TRIGGER_ZONE_SENSOR3))) {
			if( !get_ss_valet() ) {
				if(inst_trig_bp) dei_led_flash(DEI_LED_DOOR_TRIGGERED , LED_ENABLE);
				else dei_led_flash( DEI_SS_LED_ZONE_BYPASS , LED_ENABLE );
            }

            if(!get_ss_panic()) {
                if( dei_ss_zone_bypass_vars.is_first_check ) {
					if( hood_bp || trunk_bp || (door_bp  && (dei_feature_get(DEI_FEAT_DOOR_TRIGGER_ERROR_CHIRP)==2))
						|| inst_trig_bp || ext_trig_bp
   					|| (ext_zones & (1<<DEI_SS_TRIGGER_ZONE_SENSOR3))
					  ){
						dei_siren_chirp( DEI_SIREN_ZONE_BYPASS );
						dei_siren_send_zone();
                    }

                notify_send( NOTIFY_ACTIVE_ZONE , DEI_SS_ZONE_OPEN_PLD );
                }
            }
        }
        else {
            if( !get_ss_valet()) {
					dei_led_flash( DEI_LED_ARM , LED_ENABLE );
			}
        }

		if( dei_ss_zone_bypass_vars.is_first_check == FALSE ) {
            set_zone_bypass_status(  FALSE );
        }
        else {
            ATOMIC( dei_ss_zone_bypass_vars.is_first_check = FALSE );
            vrtc_update( dei_ss_zone_bypass_vars.vrtc , DEI_SS_VRTC_TIMEOUT_ZONE_BYPASS );
            vrtc_start( dei_ss_zone_bypass_vars.vrtc ) ;
        }
    }
}

void dei_ss_zone_bypass_inst_trig_off( void ) {

	if(get_ss_arm()) {
		dei_led_flash( DEI_LED_ARM , LED_ENABLE );
		}

	}


//--------------------------------------------------------------------------//
