/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs_sensor.c 33268 2015-08-19 15:40:18Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define ATOS_RTC_MILLI_SECOND                 (72*1000)                          //ATOS ticks for 1 millisecond
#define SENSE_RS_EXT_WARNAWAY_PULSE_MAX       (900 * ATOS_RTC_MILLI_SECOND)      //<--for 900 ms
#define SENSE_RS_EXT_WARNAWAY_PULSE_MIN       (100 * ATOS_RTC_MILLI_SECOND)      //<--for 100 ms
#define SENSE_RS_EXT_WARNAWAY_NOM             (200 * ATOS_RTC_MILLI_SECOND)      //<--for 200 ms

//#define SENSE_RS_EXT_FULLTRIGGER_PULSE_MAX    (1400 * ATOS_RTC_MILLI_SECOND)      //<--for 1400 ms
#define SENSE_RS_EXT_FULLTRIGGER_PULSE_MIN    (900 * ATOS_RTC_MILLI_SECOND)       //<--for 900 ms
#define SENSE_RS_EXT_FULLTRIGGER_NOM          (1200 * ATOS_RTC_MILLI_SECOND)      //<--for 1200 ms

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Per_Exec_Wake sense_rs_sensor_wire( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Ied  sense_rs_ied_sensor ;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//
	#ifdef DEV_LPC_236x

void sense_rs_sensor_init( void )
{
    sense_rs_ied_sensor = ied_create( SENSE_RS_PIN_SENSOR , SENSE_RS_SENSOR_DEBOUNCE_DEBOUNCE   , SENSE_RS_SENSOR_DEBOUNCE_DEBOUNCE     ); //50 ms
		
	set_sensor_status( SENSE_RS_PIN_SENSOR );	
	
	per_exec_register( (Per_Exec_Func)sense_rs_sensor_wire );
}       

//--------------------------------------------------------------------------//


Per_Exec_Wake sense_rs_sensor_wire( void )
{		
	static UInt16  pulse_period  = 0;
	static Boolean edge_detected = FALSE ;
	
	
	switch( ied_detect( sense_rs_ied_sensor , SENSE_RS_PIN_SENSOR ) ) 
	{
		case -1 :
		{
			pulse_period = 0 ;
			
			edge_detected = TRUE ; 

			TRACE_DEI_SENSE_RS("\n\r[SENSOR_EDGE_-1]\n\r") ;

			set_sensor_status( TRUE );			
		}
		break;

		case +1 :
		{
			
			TRACE_DEI_SENSE_RS("\n\r[SENSOR_EDGE_+1]\n\r") ;
			
			if( (pulse_period < SENSE_RS_SENSOR_PULSE_WARNING) && edge_detected )//800ms
			{
				cmd_q_rs_insert( INPUT_SENSOR_WARNING , TIMEOUT_Q_INSERT );	
		
				TRACE_DEI_SENSE_RS("\n\r[SENSOR_WARNING]\n\r" );	

			}
			else
			{
				if( (pulse_period > SENSE_RS_SENSOR_PULSE_TRIGGER_MIN) && edge_detected )
				{
					cmd_q_rs_insert( INPUT_SENSOR_TRIGGER , TIMEOUT_Q_INSERT );	
		
					TRACE_DEI_SENSE_RS("\n\r[SENSOR_TRIGGER]\n\r" );	
				}
			}

			set_sensor_status( FALSE );
			
			edge_detected = FALSE ; 
		
		}
		break;

		default :
		{
			if( (pulse_period  < SENSE_RS_SENSOR_PULSE_TRIGGER_MAX) && edge_detected )
			{
				pulse_period ++ ; 

				if( pulse_period > SENSE_RS_SENSOR_PULSE_TRIGGER_MIN )
				{
					cmd_q_rs_insert( INPUT_SENSOR_TRIGGER , TIMEOUT_Q_INSERT );	
		
					TRACE_DEI_SENSE_RS("\n\r[SENSOR_TRIGGER]\n\r" );	
					
					edge_detected = FALSE ; 					
				}
			}
			else
			{
				edge_detected = FALSE  ; 				
			}	
		}
		break;
	}
		

	return edge_detected ? PER_EXEC_WAKE : PER_EXEC_SLEEP ; 
}

#elif defined(PLATFORM_933)
   //use the programmable input wire
Boolean ext_sensor_trigger_wire_status = FALSE;
Boolean ext_sensor_trigger_wire_old_status = FALSE;

UInt32 sense_ext_trg_wires_sts=0;
void sense_ext_trigger_status_update(UInt32 *ext_trg_status, Boolean status, UInt8 from);

void sense_rs_ext_sensor_trigger( Boolean do_init , Boolean status, UInt8 from)
{		
	static UInt32 pulse_start = 0;
	static UInt32 pulse_period = 0;
	
	sense_ext_trigger_status_update(&sense_ext_trg_wires_sts, status, from);
		if(do_init)	
		{
			ext_sensor_trigger_wire_status = (sense_ext_trg_wires_sts) ? TRUE : FALSE;
		}
		else
		{
			ext_sensor_trigger_wire_status = (sense_ext_trg_wires_sts) ? TRUE : FALSE;
			
			if(ext_sensor_trigger_wire_status != ext_sensor_trigger_wire_old_status) //transaction
			{
				if(ext_sensor_trigger_wire_status == FALSE)  //pulse release
				{
					pulse_period = T1TC - pulse_start;
					if(!ext_sensor_trigger_wire_status && get_ss_arm())
					{
						timeout_stop(sense_ext_trigger_timer);
						if(pulse_period > SENSE_RS_EXT_WARNAWAY_PULSE_MIN && pulse_period < SENSE_RS_EXT_WARNAWAY_PULSE_MAX) //there was pulse
						{
							cmd_q_rs_insert( INPUT_SENSOR_WARNING , TIMEOUT_Q_INSERT, CMD_EXT_WARNAWAY);	
							TRACE_DEI_SENSE_RS("\n\r[EXT_SENSOR_WARNING]\n\r" );	
						}
						else if(pulse_period > SENSE_RS_EXT_FULLTRIGGER_PULSE_MIN)
						{
							cmd_q_rs_insert( INPUT_SENSOR_TRIGGER , TIMEOUT_Q_INSERT, CMD_EXT_TRIGGER );	
							TRACE_DEI_SENSE_RS("\n\r[EXT_SENSOR_TRIGGER]\n\r" );	
						}
					}
				}
				else   //pulse starts
				{
					pulse_start = T1TC;
					//FIXME: longer pulse, trigger as well
					timeout_start(sense_ext_trigger_timer);
				}
			}
		}
		ext_sensor_trigger_wire_old_status = ext_sensor_trigger_wire_status;
}

void sense_ext_trigger_check_timeout(void)
{
		cmd_q_rs_insert( INPUT_SENSOR_TRIGGER , TIMEOUT_Q_INSERT, CMD_EXT_TRIGGER );	
		TRACE_DEI_SENSE_RS("\n\r[LONG_EXT_SENSOR_TRIGGER]\n\r" );	
}


/*--------------------------------------------------------------------------*/
void sense_ext_trigger_status_update(UInt32 *ext_trg_status, Boolean status, UInt8 from)
{
	if(status)
		*ext_trg_status |= (1<<from);
	else
		*ext_trg_status &= ~(1<<from);
}


#endif
//--------------------------------------------------------------------------//

