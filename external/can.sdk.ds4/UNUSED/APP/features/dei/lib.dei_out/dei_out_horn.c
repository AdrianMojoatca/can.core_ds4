/*==========================================================================*/
// $Id: dei_out_horn.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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
	#define HORN_IS_SIREN     						(FEATURE(DEI_FEAT_HORN_FUNCTION)!=1)
	#define HORN_PULSE		  						dei_out_horn_pulse_length_table[FEATURE(DEI_FEAT_HORN_FUNCTION) - 2]
	
	DEI_OUT_HORN_RESET();		
			
	if(HORN_IS_SIREN &&((DEI_OUT_PAYLOAD(payload) & DEI_SIREN_ARM) || (DEI_OUT_PAYLOAD(payload)  & DEI_SIREN_ARM_NOTIFICATION) || (DEI_OUT_PAYLOAD(payload) & DEI_SIREN_ZONE_BYPASS)) || (DEI_OUT_PAYLOAD(payload) & DEI_SIREN_FULL_SILENT_ARM) )
	{
		DEI_OUT_HORN( 20 , HORN_PULSE , 20 , 1 ) ;	
	}

	if( HORN_IS_SIREN && (DEI_OUT_PAYLOAD(payload) & DEI_SIREN_DISARM_NO_EVENT) )
	{
		DEI_OUT_HORN( 20 , HORN_PULSE , 20 , 2 ) ;				 
	}

	if( HORN_IS_SIREN && (DEI_OUT_PAYLOAD(payload) & DEI_SIREN_DISARM_LOW_BATT) )
	{
		DEI_OUT_HORN( 20 , HORN_PULSE , 20 , 3 ) ;					 
	}

	if( HORN_IS_SIREN && (DEI_OUT_PAYLOAD(payload) & DEI_SIREN_DISARM_TRIGGER_ON) )
	{
		DEI_OUT_HORN( 20 , HORN_PULSE , 20 , 4 ) ;					 
	}

	if( HORN_IS_SIREN && (DEI_OUT_PAYLOAD(payload) & DEI_SIREN_DISARM_NPC_ON) )
	{
		DEI_OUT_HORN( 20 , HORN_PULSE , 20 , 5 ) ;					 
	}
	
	if( HORN_IS_SIREN && (DEI_OUT_PAYLOAD(payload) & DEI_SIREN_WARNING) )
	{
		DEI_OUT_HORN( 50 , HORN_PULSE , 20 , 10 ) ;			
	}
	
	if( HORN_IS_SIREN && (DEI_OUT_PAYLOAD(payload) & DEI_SIREN_CAR_FINDER) )
	{		
		DEI_OUT_HORN( 50 , 20 , 20 , 1 ) ;			
	}
	
	if( (DEI_OUT_PAYLOAD(payload) & DEI_SIREN_REMOTE_PROGRAMMING_ENTER) )
	{
		DEI_OUT_HORN( 50 , HORN_PULSE , 20 , 1 ) ;			
	}
					
	if( (DEI_OUT_PAYLOAD(payload) & DEI_SIREN_REMOTE_DELETE_ENTER) )
	{
		DEI_OUT_HORN( 50 , HORN_PULSE , 20 , 2 ) ;		
	}
	
	if( (DEI_OUT_PAYLOAD(payload) & DEI_SIREN_TACHO_DELETE_ENTER) )
	{
		DEI_OUT_HORN( 50 , HORN_PULSE , 20 , 3 ) ;			
	}
	
	if( (DEI_OUT_PAYLOAD(payload) & DEI_SIREN_REMOTE_PROGRAMMING_DONE) )
	{
		DEI_OUT_HORN( 50 , 20 , 20 , 1 ) ;
	}
	
	if( (DEI_OUT_PAYLOAD(payload) & DEI_SIREN_PROGRAMMING_EXIT) )
	{
		DEI_OUT_HORN( 50 , HORN_PULSE , 20 , 2 ) ;		
	}
	
	if( DEI_OUT_PAYLOAD(payload) & DEI_SIREN_CHIRP_THEN_TRIGGER )
	{
		DEI_OUT_HORN( 280 , 20 , 50 , 0);		
	}

	if( DEI_OUT_PAYLOAD(payload) & DEI_SIREN_TRIGGER )
	{	
		DEI_OUT_HORN( 50 , 20 , 50 , 0);		
	}
	
	
	#undef  HORN_PULSE
	#undef  HORN_IS_SIREN	
}

//--------------------------------------------------------------------------//

