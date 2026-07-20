/*==========================================================================*/
// $Id: dei_out_horn.c 19278 2013-02-18 07:53:08Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_out_private.h"
#include "dei_siren.h"
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


//--------------------------------------------------------------------------//

void dei_out_horn( void *payload )
{    
	#define HORN_PULSE		  dei_out_horn_pulse_length_table[FEATURE(DEI_FEAT_HORN_OUTPUT_TIMING) - 1]
	
	#define FEAT_TRIGPANICONLY 	1	// Trigger and Panic only
	#define FEAT_OEMFUNC				2 // OEM Functions
	#define FEAT_NORMAL					3 // Normal

	UInt32	bits;	
	UInt8 	feature;
	UInt8 	horn_is_siren;
	
	feature=FEATURE(DEI_FEAT_HORN_FUNCTION);
	horn_is_siren=(feature==3);

	DEI_OUT_HORN_RESET();	 
	
	bits=DEI_OUT_PAYLOAD(payload);
			
	if(horn_is_siren &&((bits == DEI_SIREN_ARM) || (bits == DEI_SIREN_SECOND_ARM) || (bits == DEI_SIREN_ARM_NOTIFICATION) || (bits == DEI_SIREN_ZONE_BYPASS)) || (bits == DEI_SIREN_FULL_SILENT_ARM) )
	{
		DEI_OUT_HORN( 20 , HORN_PULSE , 20 , 1 ) ;
	}

	if((feature==FEAT_OEMFUNC) && (bits == DEI_SIREN_SECOND_ARM)) 
	{
		DEI_OUT_HORN( 20 , HORN_PULSE , 20 , 1 ) ;
	}
	
	if( horn_is_siren && (bits == DEI_SIREN_DISARM_NO_EVENT) )
	{
		DEI_OUT_HORN( 20 , HORN_PULSE , 20 , 2 ) ;				 
	}

	if( horn_is_siren && (bits == DEI_SIREN_DISARM_LOW_BATT) )
	{
		DEI_OUT_HORN( 20 , HORN_PULSE , 20 , 1 ) ;					 
	}

	if( horn_is_siren && (bits == DEI_SIREN_DISARM_TRIGGER_ON) )
	{
		DEI_OUT_HORN( 20 , HORN_PULSE , 20 , 4 ) ;	
	}

	if( horn_is_siren && (bits == DEI_SIREN_DISARM_NPC_ON) )
	{
		DEI_OUT_HORN( 20 , HORN_PULSE , 20 , 5 ) ;					 
	}
	
	if( horn_is_siren && (bits == DEI_SIREN_WARNING) )
	{
		DEI_OUT_HORN( 50 , HORN_PULSE , 20 , 10 ) ;			
	}
	
	if( horn_is_siren && (bits == DEI_SIREN_CAR_FINDER) )
	{		
		DEI_OUT_HORN( 50 , 20 , 20 , 1 ) ;			
	}
	
	if( (bits == DEI_SIREN_REMOTE_PROGRAMMING_ENTER) )
	{
		DEI_OUT_HORN( 50 , HORN_PULSE , 20 , 1 ) ;			
	}
					
	if( (bits == DEI_SIREN_REMOTE_DELETE_ENTER) )
	{
		DEI_OUT_HORN( 50 , HORN_PULSE , 20 , 2 ) ;		
	}

	if( (bits == DEI_SIREN_ZAP_ENTER) )
	{
		DEI_OUT_HORN( 50 , HORN_PULSE , 20 , 3 ) ;			
	}
	
	if( (bits == DEI_SIREN_TACHO_DELETE_ENTER) )
	{
		DEI_OUT_HORN( 50 , HORN_PULSE , 20 , 4 ) ;			
	}
	
	if( (bits == DEI_SIREN_REMOTE_PROGRAMMING_DONE) )
	{
		DEI_OUT_HORN( 50 , 20 , 20 , 1 ) ;
	}
	
	if( (bits == DEI_SIREN_PROGRAMMING_EXIT) )
	{
		DEI_OUT_HORN( 50 , HORN_PULSE , 20 , 2 ) ;		
	}
	
	if( bits == DEI_SIREN_CHIRP_THEN_TRIGGER )
	{
		DEI_OUT_HORN( 280 , 20 , 50 , 0);		
	}

	if( bits == DEI_SIREN_TRIGGER )
	{	
		DEI_OUT_HORN( 50 , 20 , 50 , 0);		
	}
	
	if( bits == DEI_SIREN_SENSOR_MAX_LIMIT )
	{
		DEI_OUT_HORN(50 , 3  , 20 , 2 ) ;
		DEI_OUT_HORN(93 , 50 , 1  , 1 ) ;
	}

	if( bits == DEI_SIREN_SENSOR_MIN_LIMIT )
	{
		DEI_OUT_HORN(50 , 3  , 20 , 1 ) ;
		DEI_OUT_HORN(73 , 50 , 1  , 1 ) ;
	}

	if( bits == DEI_SIREN_SENSOR_LEVEL_UP )
	{
		DEI_OUT_HORN(50 , 3  , 20 , 2 ) ;
	}

	if( bits == DEI_SIREN_SENSOR_LEVEL_DOWN )
	{
		DEI_OUT_HORN(50 , 3  , 20 , 1 ) ;
	}
	
	if( bits == DEI_SIREN_SENSOR_DEFAULT_SETTING )
	{
		DEI_OUT_HORN( 50 , HORN_PULSE , 20 , 3 ) ;
	}
	
	if( bits == DEI_SIREN_SENSOR_ADJUST_ENTER )
	{
		DEI_OUT_HORN( 50 , HORN_PULSE , 20 , 1 ) ;			
	}

	if( bits == DEI_SIREN_VALET_MODE_ON )
	{
		DEI_OUT_HORN(50 , 50  , 20 , 2 ) ;
	}
	
	if( bits == DEI_SIREN_VALET_MODE_OFF )
	{
		DEI_OUT_HORN(50 , 50  , 20 , 1 ) ;
	}

	if( (bits == DEI_SIREN_SENSOR_PROGRAMMING_EXIT) )
	{
		DEI_OUT_HORN( 50 , HORN_PULSE , 20 , 3 ) ;		
	}
		
	#undef  HORN_PULSE
}

//--------------------------------------------------------------------------//

