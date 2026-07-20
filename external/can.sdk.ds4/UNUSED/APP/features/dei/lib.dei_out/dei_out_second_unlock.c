/*==========================================================================*/
// $Id: dei_out_second_unlock.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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
#define UNLOCK_PULSE_LENGTH		 	dei_out_lock_pulse_length_table[FEATURE(DEI_FEAT_DOORLOCK_OUTPUT_DURATION)-1]
#define UNLOCK_DOUBLE_PULSE         ((FEATURE( DEI_FEAT_DOORLOCK_PULSES)==2)||(FEATURE( DEI_FEAT_DOORLOCK_PULSES)==4))	
#define SECOND_UNLOCK_WITH_FIRST    (FEATURE(DEI_FEAT_2ND_UNLOCK) == 3)
#define SECOND_UNLOCK_AFTER_FIRST   (FEATURE(DEI_FEAT_2ND_UNLOCK) == 2)

#define UNLOCK_PAUSE_LENGTH 	    30 //for 300ms
//--------------------------------------------------------------------------//

void dei_out_second_unlock( void  *payload )
{	
    if( UNLOCK_DOUBLE_PULSE && (DEI_OUT_PAYLOAD( payload ) & CMD_UNLOCK_WITH_DISARM) ) return ;
    
    DEI_OUT_LOCK_RESET();	
	DEI_OUT_SECOND_UNLOCK_RESET();
		
	if( DEI_OUT_PAYLOAD( payload ) & CMD_UNLOCK_IGNITION )
	{				
		if( SECOND_UNLOCK_WITH_FIRST )
        {    
             DEI_OUT_SECOND_UNLOCK( 2 , UNLOCK_PULSE_LENGTH , UNLOCK_PAUSE_LENGTH , 1 );			
        }

        if( SECOND_UNLOCK_AFTER_FIRST )
        {
            if( UNLOCK_DOUBLE_PULSE )
			{
				DEI_OUT_SECOND_UNLOCK( 2*UNLOCK_PULSE_LENGTH + UNLOCK_PAUSE_LENGTH + 80 , UNLOCK_PULSE_LENGTH , UNLOCK_PAUSE_LENGTH , 1 );			
			}
			else
			{
				DEI_OUT_SECOND_UNLOCK( UNLOCK_PULSE_LENGTH + 80 , UNLOCK_PULSE_LENGTH , UNLOCK_PAUSE_LENGTH , 1 );			
			}
        }				
	}
	else
	{
		DEI_OUT_SECOND_UNLOCK( 2 , UNLOCK_PULSE_LENGTH , UNLOCK_PAUSE_LENGTH , 1 );			
	}
}

//--------------------------------------------------------------------------//

#undef UNLOCK_DOUBLE_PULSE
#undef UNLOCK_PULSE_LENGTH
#undef UNLOCK_AFTER_DISARM
#undef DISARM_PULSES
#undef UNLOCK_PAUSE_LENGTH



