/*==========================================================================*/
// $Id: dei_out_unlock.c 19278 2013-02-18 07:53:08Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_out_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void unlock_state_reset(void);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

#define UNLOCK_DOUBLE_PULSE       ((FEATURE( DEI_FEAT_DOORLOCK_PULSES)==2)||(FEATURE( DEI_FEAT_DOORLOCK_PULSES)==3))	
#define UNLOCK_PULSE_LENGTH		 		dei_out_lock_pulse_length_table[FEATURE(DEI_FEAT_DOORLOCK_OUTPUT_DURATION)-1]
#define UNLOCK_AFTER_DISARM		  	(FEATURE(DEI_FEAT_OEM_ALARM_DISARM_OUTPUT )==2)
#define DISARM_PULSES			   			1


#define UNLOCK_DOUBLE_PULSE_AS        (dei_feature_get(AS_FEAT_MODE1_FUNC3_DOOR_LOCK_PULSE_TIMING)==AS_OPTION13_7_10_SECONDS_LOCK_1_4_UNLOCK_PULSES)
#define UNLOCK_PULSE_LENGTH_AS		   as_out_unlock_pulse_length_table[dei_feature_get(AS_FEAT_MODE1_FUNC3_DOOR_LOCK_PULSE_TIMING)-1]
#define UNLOCK_AFTER_DISARM_AS		   FALSE
#define DISARM_PULSES_AS			   1

#define UNLOCK_PAUSE_LENGTH 	   30 //for 300ms

//--------------------------------------------------------------------------//

void dei_out_unlock_driver( void *payload )
{
	unlock_state_reset();
	
	if( DEI_OUT_PAYLOAD( payload ) & CMD_UNLOCK_WITH_DISARM )
	{		
      //delay between disarm and unlock should be 200ms, no matter how many unlock pulses are required      
			DEI_OUT_UNLOCK( (UNLOCK_AFTER_DISARM ? 20 : 1 ), UNLOCK_PULSE_LENGTH , UNLOCK_PAUSE_LENGTH , UNLOCK_DOUBLE_PULSE ? 2 : 1 );
	}
	else //CMD_UNLOCK_USER2, CMD_UNLOCK_OEM, CMD_UNLOCK_IGNITION, CMD_UNLOCK_WITH_TRUNK_RELEASE
	{
		  DEI_OUT_UNLOCK( 1 , UNLOCK_PULSE_LENGTH , UNLOCK_PAUSE_LENGTH , UNLOCK_DOUBLE_PULSE ? 2 : 1 );			
	}

    TRACE_DEI_OUT("[DEI_OUT_UNLOCK_DRIVER]\n\r");
}

//--------------------------------------------------------------------------//

void dei_out_unlock_all( void *payload )
{
	unlock_state_reset();
	
	if( DEI_OUT_PAYLOAD( payload ) & CMD_UNLOCK_WITH_DISARM )
	{        
			//delay between disarm and unlock should be 200ms, no matter how many unlock pulses are required      
			DEI_OUT_UNLOCK((UNLOCK_AFTER_DISARM ? 20 : 1), UNLOCK_PULSE_LENGTH , UNLOCK_PAUSE_LENGTH , UNLOCK_DOUBLE_PULSE ? 2 : 1 );

			TRACE_DEI_OUT("[DEI_OUT_UNLOCK_ALL %08X ]\n\r",DEI_OUT_PAYLOAD(payload));
	}
	else  ////CMD_UNLOCK_USER2, CMD_UNLOCK_OEM, CMD_UNLOCK_IGNITION, CMD_UNLOCK_WITH_TRUNK_RELEASE
	{
		  DEI_OUT_UNLOCK( 1 , UNLOCK_PULSE_LENGTH , UNLOCK_PAUSE_LENGTH , UNLOCK_DOUBLE_PULSE ? 2 : 1 );
			TRACE_DEI_OUT("[DEI_OUT_UNLOCK_ALL_NO_DELAY %08X ]\n\r",DEI_OUT_PAYLOAD(payload));                        
	}
}

//--------------------------------------------------------------------------//

void unlock_state_reset(void)
{
	DEI_OUT_LOCK_RESET();
	DEI_OUT_COMFORT_CLOSURE_RESET();
	DEI_OUT_UNLOCK_RESET();
	cc_is_in_progress = FALSE;
	cc_progress_is_reset = TRUE;
}


//--------------------------------------------------------------------------//

#undef UNLOCK_DOUBLE_PULSE
#undef UNLOCK_PULSE_LENGTH
#undef UNLOCK_AFTER_DISARM
#undef DISARM_PULSES



