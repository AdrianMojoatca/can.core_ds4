/*==========================================================================*/
// $Id: dei_out_lock.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

#define LOCK_DOUBLE_PULSE        (FEATURE( DEI_FEAT_DOORLOCK_PULSES)==3)||(FEATURE( DEI_FEAT_DOORLOCK_PULSES)==4)	
#define LOCK_PULSE_LENGTH		 dei_out_lock_pulse_length_table[FEATURE(DEI_FEAT_DOORLOCK_OUTPUT_DURATION)-1]
#define COMFORT_PULSE_LENGTH	 20*100

#define LOCK_PAUSE_LENGTH		 30

//--------------------------------------------------------------------------//

void dei_out_lock( void *payload )
{
    DEI_OUT_LOCK_RESET();
	DEI_OUT_UNLOCK_RESET();
	DEI_OUT_SECOND_UNLOCK_RESET();
	
	if( DEI_OUT_PAYLOAD( payload ) & CMD_LOCK_OEM ) return ;
	
	if( (DEI_OUT_PAYLOAD( payload ) & CMD_LOCK_WITH_COMFORT) && (FEATURE(DEI_FEAT_COMFORT_CLOSURE)==2) ) 
	{
		dei_out_lock_with_comfort1  ( payload );	

		return ;
	}
	
	if( (DEI_OUT_PAYLOAD( payload ) & CMD_LOCK_WITH_COMFORT) && (FEATURE(DEI_FEAT_COMFORT_CLOSURE)==3) ) 
	{		
		dei_out_lock_with_comfort2  ( payload );

		return ;
	}

	dei_out_lock_without_comfort( payload );	
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

