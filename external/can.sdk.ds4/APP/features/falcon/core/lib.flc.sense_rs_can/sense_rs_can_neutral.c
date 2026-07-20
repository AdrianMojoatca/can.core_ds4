/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_prk_flash.c 1106 2010-04-27 13:30:39Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs_can_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void sense_neutral       ( Boolean instant_level );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Ied     sense_rs_ied_neutral  ;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void sense_rs_can_neutral_init( void )
{
    sense_rs_ied_neutral = ied_create( FALSE , sense_rs_can_cfg->neutral_debounce , sense_rs_can_cfg->neutral_debounce );		
}

//--------------------------------------------------------------------------//

Boolean sense_rs_neutral_can( Boolean instant_level ) //--->TRUE : car IN NEUTRAL
{
    sense_neutral( instant_level );                
    
    return  trans.bit.park ;
}

//--------------------------------------------------------------------------//

void sense_neutral( Boolean instant_level )
{
	SInt8 edge_detect ;

	ATOMIC( edge_detect = ied_detect( sense_rs_ied_neutral , instant_level ) );
		
	if( edge_detect )
	{			
	   trans.bit.park =  ied_get_level( sense_rs_ied_neutral ) ; 
			
	   cmd_q_insert( trans.bit.park ? INPUT_PRK_NEUTRAL_STATUS_ON  : INPUT_PRK_NEUTRAL_STATUS_OFF  , SENSE_RS_CAN_TIMEOUT_CMD_Q);

	   TRACE_SENSE_RS_CAN("\n\r[%s]\n\r" , trans.bit.park ? "CAR_NOT_IN_NEUTRAL":"CAR_IN_NEUTRAL" );					        			
	}	
}

//--------------------------------------------------------------------------//

