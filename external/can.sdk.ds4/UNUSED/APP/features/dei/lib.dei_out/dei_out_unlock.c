/*==========================================================================*/
// $Id: dei_out_unlock.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

#define UNLOCK_DOUBLE_PULSE        ((FEATURE( DEI_FEAT_DOORLOCK_PULSES)==2)||(FEATURE( DEI_FEAT_DOORLOCK_PULSES)==4))	
#define UNLOCK_PULSE_LENGTH		 	dei_out_lock_pulse_length_table[FEATURE(DEI_FEAT_DOORLOCK_OUTPUT_DURATION)-1]
#define UNLOCK_AFTER_DISARM		   (FEATURE(DEI_FEAT_OEM_ALARM_DISARM_OUTPUT )==2)
#define DISARM_PULSES			   FEATURE(DEI_FEAT_OEM_ALARM_DISARM_PULSES)

#define UNLOCK_PAUSE_LENGTH 	   30 //for 300ms

//--------------------------------------------------------------------------//

void dei_out_unlock_driver( void *payload )
{
    dei_out_unlock(payload);

    TRACE_DEI_OUT("[DEI_OUT_UNLOCK_DRIVER]\n\r");
}

//--------------------------------------------------------------------------//

void dei_out_unlock_all( void *payload )
{
    if( UNLOCK_DOUBLE_PULSE && (DEI_OUT_PAYLOAD( payload ) & CMD_UNLOCK_WITH_DISARM) )
    {        
        dei_out_unlock(payload);

        TRACE_DEI_OUT("[DEI_OUT_UNLOCK_ALL %08X ]\n\r",DEI_OUT_PAYLOAD(payload));
    }
    else
    {
        TRACE_DEI_OUT("[DEI_OUT_UNLOCK_ALL_SKIP %08X ]\n\r",DEI_OUT_PAYLOAD(payload));                        
    }
}
//--------------------------------------------------------------------------//

void dei_out_unlock( void  *payload )
{
	DEI_OUT_LOCK_RESET();
	DEI_OUT_UNLOCK_RESET();
			
	if( DEI_OUT_PAYLOAD( payload ) & CMD_UNLOCK_WITH_DISARM )
	{		
		if( UNLOCK_AFTER_DISARM )
		{				
			DEI_OUT_UNLOCK( DISARM_PULSES*(80+30) , UNLOCK_PULSE_LENGTH , UNLOCK_PAUSE_LENGTH , UNLOCK_DOUBLE_PULSE ? 2 : 1 );			
		}
		else
		{
			DEI_OUT_UNLOCK( 1 , UNLOCK_PULSE_LENGTH , UNLOCK_PAUSE_LENGTH , UNLOCK_DOUBLE_PULSE ? 2 : 1 );			
		}
	}
	else
	{
		DEI_OUT_UNLOCK( 1 , UNLOCK_PULSE_LENGTH , UNLOCK_PAUSE_LENGTH , UNLOCK_DOUBLE_PULSE ? 2 : 1 );			
	}
}

//--------------------------------------------------------------------------//

#undef UNLOCK_DOUBLE_PULSE
#undef UNLOCK_PULSE_LENGTH
#undef UNLOCK_AFTER_DISARM
#undef DISARM_PULSES



