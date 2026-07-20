/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs_push_antenna.c 33268 2015-08-19 15:40:18Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs_private.h"
#include "dei_sc.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Per_Exec_Wake sense_rs_push_antenna_wire( void );
static Boolean       push_antenna_status(void);

void sense_rs_push_antenna_scpp_cmd_press( void );
void sense_rs_push_antenna_scpp_cmd_tap1( void );
void sense_rs_push_antenna_scpp_cmd_tap1_hold( void );
void sense_rs_push_antenna_scpp_cmd_tap2( void );
void sense_rs_push_antenna_scpp_cmd_tap2_hold( void );
void sense_rs_push_antenna_scpp_cmd_tap3( void );
void sense_rs_push_antenna_scpp_cmd_tap3_hold( void );
void sense_rs_push_antenna_scpp_cmd_tap4( void );
void sense_rs_push_antenna_scpp_cmd_tap4_hold( void );
void sense_rs_push_antenna_scpp_cmd_tap5( void );
void sense_rs_push_antenna_scpp_cmd_tap5_hold( void );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Ied  sense_rs_ied_push_antenna ; 
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void sense_rs_push_antenna_init( void )
{
	#ifdef DEV_LPC_236x
    sense_rs_ied_push_antenna = ied_create( push_antenna_status() , SENSE_RS_PUSH_ANTENNA_DEBOUNCE , SENSE_RS_PUSH_ANTENNA_DEBOUNCE );

	set_push_antenna_status( push_antenna_status() );

	per_exec_register( (Per_Exec_Func)sense_rs_push_antenna_wire ); 
	#else
	
	static const Cmd_Array1 cmd_array[] = 
    {
			{ INPUT_PUSH_VALET_PRESS_                      , (Exec_Func)sense_rs_push_antenna_scpp_cmd_press                        },
			{ INPUT_BUTTON_TAP1                      , (Exec_Func)sense_rs_push_antenna_scpp_cmd_tap1 	                        },
			{ INPUT_BUTTON_TAP1_HOLD               , (Exec_Func)sense_rs_push_antenna_scpp_cmd_tap1_hold                     },	 	
			{ INPUT_BUTTON_TAP2                   , (Exec_Func)sense_rs_push_antenna_scpp_cmd_tap2                     },
			{ INPUT_BUTTON_TAP2_HOLD                    , (Exec_Func)sense_rs_push_antenna_scpp_cmd_tap2_hold                         }, 
			{ INPUT_BUTTON_TAP3             , (Exec_Func)sense_rs_push_antenna_scpp_cmd_tap3                  }, 
			{ INPUT_BUTTON_TAP3_HOLD       , (Exec_Func)sense_rs_push_antenna_scpp_cmd_tap3_hold                  }, 
			{ INPUT_BUTTON_TAP4         , (Exec_Func)sense_rs_push_antenna_scpp_cmd_tap4              }, 
			{ INPUT_BUTTON_TAP4_HOLD           , (Exec_Func)sense_rs_push_antenna_scpp_cmd_tap4_hold                }, 
			{ INPUT_BUTTON_TAP5         , (Exec_Func)sense_rs_push_antenna_scpp_cmd_tap5              }, 
			{ INPUT_BUTTON_TAP5_HOLD           , (Exec_Func)sense_rs_push_antenna_scpp_cmd_tap5_hold                }

		};

    CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);

	#endif
	
}

//--------------------------------------------------------------------------//

#ifdef DEV_LPC_176x
void sense_rs_push_antenna_scpp_cmd_press( void )
{
	
	if(dei_sc_get_mode() != DEI_SC_MODE_SENSOR_ADJUST)
	{
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_EXT_PUSH_VALET_PRESS, TIMEOUT_Q_INSERT ) ; 
	}
	
}

void sense_rs_push_antenna_scpp_cmd_tap1( void )
{
	if(dei_sc_get_mode() != DEI_SC_MODE_SENSOR_ADJUST)
	{
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
	}

}


void sense_rs_push_antenna_scpp_cmd_tap1_hold( void )
{
	if(dei_sc_get_mode() != DEI_SC_MODE_SENSOR_ADJUST)
	{
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_EXT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
	}

}

void sense_rs_push_antenna_scpp_cmd_tap2( void )
{
	if(dei_sc_get_mode() != DEI_SC_MODE_SENSOR_ADJUST)
	{
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
	}

}


void sense_rs_push_antenna_scpp_cmd_tap2_hold( void )
{
	if(dei_sc_get_mode() != DEI_SC_MODE_SENSOR_ADJUST)
	{
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_EXT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
	}

}


void sense_rs_push_antenna_scpp_cmd_tap3( void )
{
	if(dei_sc_get_mode() != DEI_SC_MODE_SENSOR_ADJUST)
	{
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
	}

}


void sense_rs_push_antenna_scpp_cmd_tap3_hold( void )
{
	if(dei_sc_get_mode() != DEI_SC_MODE_SENSOR_ADJUST)
	{
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_EXT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
	}

}

void sense_rs_push_antenna_scpp_cmd_tap4( void )
{
	if(dei_sc_get_mode() != DEI_SC_MODE_SENSOR_ADJUST)
	{
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
	}

}


void sense_rs_push_antenna_scpp_cmd_tap4_hold( void )
{
	if(dei_sc_get_mode() != DEI_SC_MODE_SENSOR_ADJUST)
	{
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_EXT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
	}

}

void sense_rs_push_antenna_scpp_cmd_tap5( void )
{
	if(dei_sc_get_mode() != DEI_SC_MODE_SENSOR_ADJUST)
	{
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
	}

}


void sense_rs_push_antenna_scpp_cmd_tap5_hold( void )
{
	if(dei_sc_get_mode() != DEI_SC_MODE_SENSOR_ADJUST)
	{
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_RELEASE , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
		os_tsk_wait ( 100 );
		cmd_q_rs_insert( INPUT_EXT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
	}

}

#endif


//--------------------------------------------------------------------------//

Per_Exec_Wake sense_rs_push_antenna_wire( void )
{
	static Boolean is_pressed  = FALSE ;
	static Boolean is_extended = FALSE ;
	static UInt32   time       = 0 ;

	switch( ied_detect( sense_rs_ied_push_antenna , push_antenna_status() ) )
	{
		case +1 :
		{
			cmd_q_rs_insert( INPUT_PUSH_VALET_PRESS , TIMEOUT_Q_INSERT ) ; 
			
			TRACE_DEI_SENSE_RS("\n\r[PUSH_ANTENNA_PRESS]\n\r") ; 
			
			is_pressed  = TRUE ;
			is_extended = FALSE; 

			set_push_antenna_status( TRUE );
			
			time = 0 ; 
		
			return PER_EXEC_WAKE ;
		}
		
		
		case -1 :
		{
			cmd_q_rs_insert( (is_extended ? INPUT_EXT_PUSH_VALET_RELEASE : INPUT_PUSH_VALET_RELEASE) , TIMEOUT_Q_INSERT ) ; 
			
			TRACE_ZONE
			(
				if( is_extended )
				{						
					TRACE_DEI_SENSE_RS("\n\r[PUSH_ANTENNA_EXT_RELEASE]\n\r") ; 
				}
				else
				{
					TRACE_DEI_SENSE_RS("\n\r[PUSH_ANTENNA_RELEASE]\n\r" ) ; 
				}
			)

										
			is_extended = FALSE ; 
			
			is_pressed = FALSE ; 

			set_push_antenna_status( FALSE );

			time = 0 ; 
		
			return PER_EXEC_SLEEP ; 
		}
		

		default :
		{
			if( is_pressed ) 
			{
				if( ++time == 10*17 )//1.7 SECONDS
				{
					cmd_q_rs_insert( INPUT_EXT_PUSH_VALET_PRESS, TIMEOUT_Q_INSERT ) ; 

					TRACE_DEI_SENSE_RS("\n\r[PUSH_ANTENNA_EXT_PRESS]\n\r") ; 

					is_extended = TRUE ;
				}

                return PER_EXEC_WAKE ;
			}
		}
		break;
	}
		 	
	
	return PER_EXEC_SLEEP ;
}

//--------------------------------------------------------------------------//

static Boolean  push_antenna_status(void)
{
#ifdef DEV_LPC_236x
	#if	CM800_AUTOSTART_BRAND == 1
    return (sense_rs_input_config->push_antenna_type == SENSE_RS_PUSH_ANTENNA_NORMAL)?SENSE_RS_PIN_PUSH_ANTENNA:(!SENSE_RS_PIN_PUSH_ANTENNA && (!get_rs_gwr()));	
	#else
		return (sense_rs_input_config->push_antenna_type == SENSE_RS_PUSH_ANTENNA_NORMAL)?SENSE_RS_PIN_PUSH_ANTENNA:(!SENSE_RS_PIN_PUSH_ANTENNA && (get_ign_status() && !get_rs_gwr()));
	#endif    
#else
	return TRUE;
#endif
}

//--------------------------------------------------------------------------//
