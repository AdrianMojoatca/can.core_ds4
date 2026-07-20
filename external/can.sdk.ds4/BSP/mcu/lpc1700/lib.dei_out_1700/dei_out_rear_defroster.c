/*==========================================================================*/
// $Id: dei_out_rear_defroster.c 17934 2012-10-29 19:23:36Z martin.bouchard $
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


//--------------------------------------------------------------------------//

void dei_out_rear_defogger_on( void * payload )
{  
#if 0
	if( (FEATURE( DEI_FEAT_REAR_DEFOGGER_OUTPUT_TYPE ) == DEI_DEFOGGER_LATCH) )
		//| ( FEATURE( AS_FEAT_MODE2_FUNC7_REAR_DEFOG_OUTPUT ) == AS_OPTION27_REAR_DEFOG_LATCHED ) )
	{
		prg_out_rear_defogger( TRUE ) ;
	}
	else
	{
		DEI_OUT_REAR_DEFOGGER_RESET();
		
		DEI_OUT_REAR_DEFOGGER( 2 ,100 ,1 , 1 );		
	}
#endif
	
	// NEW_FEAT : need to implement as new feature
}

//--------------------------------------------------------------------------//

void dei_out_rear_defogger_off( void * payload )
{    
#if 0
	if( (FEATURE( DEI_FEAT_REAR_DEFOGGER_OUTPUT_TYPE ) == DEI_DEFOGGER_LATCH))
//		|| (FEATURE( AS_FEAT_MODE2_FUNC7_REAR_DEFOG_OUTPUT ) == AS_OPTION27_REAR_DEFOG_LATCHED) )
	{
		prg_out_rear_defogger( FALSE );
	}
	else
	{
		DEI_OUT_REAR_DEFOGGER_RESET();
		
		DEI_OUT_REAR_DEFOGGER( 2 ,100 ,1 , 1 );		
	}
#endif
	
	// NEW_FEAT: need to implement as new feature
}

//--------------------------------------------------------------------------//
