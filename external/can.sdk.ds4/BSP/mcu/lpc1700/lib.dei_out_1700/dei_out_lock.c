/*==========================================================================*/
// $Id: dei_out_lock.c 19278 2013-02-18 07:53:08Z florin.olariu $
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
static void dei_out_lock_without_comfort( void*payload );
static void dei_out_lock_with_comfort1  ( void*payload );
static void dei_out_lock_with_comfort2  ( void*payload );
static void action_comfort_lock(void *payload);
static void cc_in_progress_timeout(void);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

#define LOCK_DOUBLE_PULSE        (FEATURE( DEI_FEAT_DOORLOCK_PULSES)==3)
#define LOCK_PULSE_LENGTH		 dei_out_lock_pulse_length_table[FEATURE(DEI_FEAT_DOORLOCK_OUTPUT_DURATION)-1]

#define LOCK_PAUSE_LENGTH		 24

//--------------------------------------------------------------------------//

void dei_out_lock_init(void)
{
    dei_out_lock_var.vrtc_cc_in_progress = vrtc_f_create((Exec_Func)cc_in_progress_timeout);
    cc_is_in_progress = FALSE;
    cc_progress_is_reset = TRUE;
}

//--------------------------------------------------------------------------//

void dei_out_lock( void *payload )
{
    if(cc_is_in_progress)
       return;

  DEI_OUT_LOCK_RESET();
	DEI_OUT_UNLOCK_RESET();
	DEI_OUT_SECOND_UNLOCK_RESET();
	
	if( DEI_OUT_PAYLOAD( payload ) & CMD_LOCK_OEM ) return ;
	
	if( (dei_feature_get(DEI_FEAT_COMFORT_CLOSURE)==1) || DEI_OUT_PAYLOAD( payload ) & CMD_LOCK_IGNITION )
		dei_out_lock_without_comfort( payload );
	else
	{
			if(!cc_progress_is_reset)
					return;
			action_comfort_lock(payload);
	}
}

//--------------------------------------------------------------------------//
void action_comfort_lock(void *payload)
{
    if(dei_feature_get(DEI_FEAT_COMFORT_CLOSURE)!=1
       && !get_ss_one_time_bypass()
		   )
    {
			if((DEI_OUT_PAYLOAD( payload ) & CMD_LOCK_WITH_COMFORT) && (FEATURE(DEI_FEAT_COMFORT_CLOSURE)==2))
        dei_out_lock_with_comfort1  ( payload );	
			else if((DEI_OUT_PAYLOAD( payload ) & CMD_LOCK_WITH_COMFORT) && (FEATURE(DEI_FEAT_COMFORT_CLOSURE)==3))
        dei_out_lock_with_comfort2  ( payload );
			else if((DEI_OUT_PAYLOAD( payload ) & CMD_LOCK_IGNITION) || (DEI_OUT_PAYLOAD( payload ) & CMD_UNLOCK_WITH_TRUNK_CLOSE))
				dei_out_lock_without_comfort( payload );
    }

    cc_is_in_progress = TRUE;
    cc_progress_is_reset = FALSE;
    vrtc_stop(dei_out_lock_var.vrtc_cc_in_progress);
    vrtc_update(dei_out_lock_var.vrtc_cc_in_progress, VRTC_CC_PROCESS_DURATION);
    vrtc_start(dei_out_lock_var.vrtc_cc_in_progress);
}

//--------------------------------------------------------------------------//
void cc_in_progress_timeout(void)
{
    cc_is_in_progress = FALSE;
    cc_progress_is_reset = TRUE;
}

//--------------------------------------------------------------------------//

void dei_out_lock_without_comfort( void *payload )
{
	DEI_OUT_LOCK( 0 , LOCK_PULSE_LENGTH , LOCK_PAUSE_LENGTH , LOCK_DOUBLE_PULSE ? 2 : 1 ) ; 
}

//--------------------------------------------------------------------------//

void dei_out_lock_with_comfort1( void *payload )
{
	if( LOCK_DOUBLE_PULSE )
	{
		DEI_OUT_LOCK( 1                		                  , LOCK_PULSE_LENGTH                   	 , 1 , 1 ) ; 					
		DEI_OUT_LOCK( 2+LOCK_PULSE_LENGTH + LOCK_PAUSE_LENGTH , LOCK_PULSE_LENGTH + COMFORT_PULSE_LENGTH , 1 , 1 ) ; 				
	}
	else
	{
		DEI_OUT_LOCK( 1 , LOCK_PULSE_LENGTH + COMFORT_PULSE_LENGTH , 1 , 1 ) ;
	}
}

//--------------------------------------------------------------------------//

void dei_out_lock_with_comfort2( void *payload )
{
	if( LOCK_DOUBLE_PULSE )
	{
		DEI_OUT_LOCK( 1                       		                                   , LOCK_PULSE_LENGTH    , LOCK_PAUSE_LENGTH  , 2 ) ; 					
		DEI_OUT_LOCK( 1 + (2*LOCK_PULSE_LENGTH+LOCK_PAUSE_LENGTH) + LOCK_PAUSE_LENGTH  , COMFORT_PULSE_LENGTH , 1                  , 1 ) ; 				
	}
	else
	{
		DEI_OUT_LOCK( 1                       		             , LOCK_PULSE_LENGTH    , 1  , 1 ) ; 		
		DEI_OUT_LOCK( 3 + LOCK_PULSE_LENGTH + LOCK_PAUSE_LENGTH  , COMFORT_PULSE_LENGTH , 1  , 1 ) ;	
	}
}
	
//--------------------------------------------------------------------------//	

#undef LOCK_DOUBLE_PULSE        
#undef LOCK_PULSE_LENGTH		 
#undef COMFORT_PULSE_LENGTH

